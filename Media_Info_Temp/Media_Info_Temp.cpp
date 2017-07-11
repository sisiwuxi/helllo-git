#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Media_Info_Temp.h"
 
//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------
static File_Media_Info Media_Info; 
static unsigned int u32gExifBuffAddr;
static unsigned short WholeBigEndian; 
static unsigned int u32TiffBuffAddr;

//------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local Function Prototypes
//------------------------------------------------------------------------------
static unsigned int MApp_EndianChangeL(unsigned int u32Val);
static unsigned short MApp_EndianChangeS(unsigned short u16Val);
static bool MApp_Set_WholeBigEndian(unsigned short BigEndian);

//------------------------------------------------------------------------------
// Local function Implementations
//------------------------------------------------------------------------------

//======================================================================//
// void Get_General_Info(FILE *fp1)
//======================================================================//
void Get_General_Info(FILE *fp1)
{
//Complete File Name
	strcpy(Media_Info.FileName, FILE_NAME);
//File Size
	int Size = 0;
	fseek(fp1, 0, SEEK_END);
	Size = ftell(fp1);
	fseek(fp1, 0, SEEK_SET);//revert
	if(Size / (1024 * 1024))
	{
		Media_Info.kilogyte_megabyte = 1;
        //Media_Info.FileSize = (float)Size / (float)(1024.0 * 1024.0);
	}
	else
	{
		Media_Info.kilogyte_megabyte = 0;
        //Media_Info.FileSize = (float)Size / (float)(1024.0);
	}
    Media_Info.FileSize = Size;
	return;
}

//======================================================================//
// void Photo_JPG_Format_Parser(FILE *fp1)
//======================================================================//
bool Photo_JPG_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	unsigned char* pBuff = (unsigned char*)FileFormatParserMallocBuff(JFIF_SOI_CHECKING_LEN);
	if(pBuff != NULL)
	{
		fseek(fp1, 0, SEEK_SET);
		fread(pBuff, sizeof(char), JFIF_SOI_CHECKING_LEN, fp1);
		if ((JPEG_MARKER_PREFIX == pBuff[0]) && (JPEG_MARKER_SOI == pBuff[1])) //SOI
		{
			b_Ret = true; 
		}
		FileFormatParserFreeBuff(pBuff);
	}
	return b_Ret;
}

//======================================================================//
// void Photo_BMP_Format_Parser(FILE *fp1)
//======================================================================//
bool Photo_BMP_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	unsigned char* pBuff = (unsigned char*)FileFormatParserMallocBuff(BMP_FILE_HEADER_SIZE);
	if(pBuff != NULL)
	{
		fseek(fp1, 0, SEEK_SET);
		fread(pBuff, sizeof(char), BMP_FILE_HEADER_SIZE, fp1);
		if (BMP_SIGNATURE == ((pBuff[0] << 8) | pBuff[1]))
		{
			b_Ret = true; 
		}
		FileFormatParserFreeBuff(pBuff);
	}
	return b_Ret;
}

//======================================================================//
// void Photo_PNG_Format_Parser(FILE *fp1)
//======================================================================//
bool Photo_PNG_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	unsigned char* pBuff = (unsigned char*)FileFormatParserMallocBuff(PNG_HEADER_SIZE);
	if(pBuff != NULL)
	{
		unsigned char PNG_MAGIC_NUM[8] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A}; 
		fseek(fp1, 0, SEEK_SET);
		fread(pBuff, sizeof(char), PNG_HEADER_SIZE, fp1);
		if (!memcmp(&pBuff[0], &PNG_MAGIC_NUM[0], 8)) 
		{
			b_Ret = true; 
		}
		FileFormatParserFreeBuff(pBuff);
	}
	return b_Ret;
}

//======================================================================//
// void Photo_TIFF_Format_Parser(FILE *fp1)
//======================================================================//
bool Photo_TIFF_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	unsigned char* pBuff = (unsigned char*)FileFormatParserMallocBuff(TIFF_HEADER_SIZE);
	if(pBuff != NULL)
	{
		fseek(fp1, 0, SEEK_SET); 
		fread(pBuff, sizeof(char), TIFF_HEADER_SIZE, fp1);
        IMAGE_DBG(("%x %x %x",pBuff[0],pBuff[1],pBuff[2]));
		if (((TIFF_SIGNATURE1 == pBuff[0]) && (TIFF_SIGNATURE1 == pBuff[1]) && (TIFF_SIGNATURE == pBuff[2]))
			|| ((TIFF_SIGNATURE2 == pBuff[0]) && (TIFF_SIGNATURE2 == pBuff[1]) && (TIFF_SIGNATURE == pBuff[2])))
		{
			b_Ret = true;  
		}
		FileFormatParserFreeBuff(pBuff);
	}
	return b_Ret;
}

//======================================================================//
// void Photo_GIF_Format_Parser(FILE *fp1)
//======================================================================//
bool Photo_GIF_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	unsigned char* pBuff = (unsigned char*)FileFormatParserMallocBuff(GIF_HEADER_SIZE);
	if(pBuff != NULL)
	{
		fseek(fp1, 0, SEEK_SET);
		fread(pBuff, sizeof(char), GIF_HEADER_SIZE, fp1);
		if (GIF_SIGNATURE == ((pBuff[0] << 16) | (pBuff[1] << 8) | pBuff[2]))
		{
			b_Ret = true;  
		}
		FileFormatParserFreeBuff(pBuff);
	}
	return b_Ret;
}

//======================================================================//
// void Photo_Format_Parser(FILE *fp1)
//======================================================================//
bool Photo_Format_Parser(FILE *fp1)
{ 
	bool b_Ret = false;
	if(Photo_JPG_Format_Parser(fp1)) 
	{
		b_Ret = true;
		Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_PHOTO_JPG;
		strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "JPG");
	}
	else if(Photo_BMP_Format_Parser(fp1))
	{
		b_Ret = true;
		Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_PHOTO_BMP;
		strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "BMP");
	}
	else if(Photo_PNG_Format_Parser(fp1))
	{
		b_Ret = true;
		Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_PHOTO_PNG;
		strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "PNG");
	}
	else if(Photo_TIFF_Format_Parser(fp1))
	{
		b_Ret = true;
		Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_PHOTO_TIFF;
		strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "TIFF");
	}
	else if(Photo_GIF_Format_Parser(fp1))
	{
		b_Ret = true;
		Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_PHOTO_GIF;
		strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "GIF");
	}
	else
	{
		b_Ret = false; 
		Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_NONE;
		strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "Unknown");
	}
	return b_Ret;
}


//======================================================================//
// void Music_MP3_Format_Parser(FILE *fp1)
/*
AAAAAAAA     AAABBCCD   EEEEFFGH   IIJJKLMM

A    Frame synchronizer
All bits are set to 1. It is used for finding the beginning of frame. But these values can occur many times in binary file so you should test
next values from header for validity (eg. bitrate bits arent 1111, sampling rate frequency isnt 11 etc.). But you can never be 100% sure
if you find a header.
Next method is to find the first header and then go through all frames - almost exact, but time consuming.
Be careful with the first frame! It doesnt have to start at the first Byte in file. Either TAG v2 can be included or file can contains of some
crap at the beginning.
Anyway - to find a header is a little problem.

B    MPEG version ID
00  MPEG Version 2.5 (not an official standard)
01  reserved
10  MPEG Version 2
11  MPEG Version 1
In most MP3 files these value should be 11.

C    Layer
00  reserved
01  Layer III
10  Layer II
11  Layer I
In most MP3 files these value should be 01 (because MP3 = MPEG 1 Layer 3).

D    CRC Protection
0    Protected by CRC
1    Not protected
Frames may have some form of check sum - CRC check. CRC is 16 bit long and, if exists, it follows frame header. And then comes audio data.
But almost all MP3 files doesnt contain CRC.
*/
//======================================================================//
bool Audio_MP3_Format_Parser(FILE *fp1)
{
    bool b_Ret = false;
    MS_U32  u32FirstFrameOffset = 0;
    unsigned char* pBuff = (unsigned char*)FileFormatParserMallocBuff(MP3_HEADER_SIZE);
    if(pBuff != NULL)
    {
        // Seek to the begin of file.
        fseek(fp1, 0, SEEK_SET);
        fread(pBuff, sizeof(char), MP3_HEADER_SIZE, fp1);
        //AUDIO_DBG(("%x %x %x\n",pBuff[0],pBuff[1],pBuff[2]);

        unsigned char ID3_NUM[3] = {0x49, 0x44, 0x33};//ID3
        if(memcmp(&pBuff[0], &ID3_NUM[0], 3))
        {
            AUDIO_ERR(("\nNo ID3v2 tag\n"));
            return false;
        }
        else
        {
            MS_U32 u32ID3Size = 0;

            u32ID3Size = (MS_U32) (pBuff[6] & 0x7F);
            u32ID3Size <<= 7;
            u32ID3Size |= (MS_U32) (pBuff[7] & 0x7F);
            u32ID3Size <<= 7;
            u32ID3Size |= (MS_U32) (pBuff[8] & 0x7F);
            u32ID3Size <<= 7;
            u32ID3Size |= (MS_U32) (pBuff[9] & 0x7F);
            u32FirstFrameOffset = u32ID3Size + 10;
			//AUDIO_DBG(("u32FirstFrameOffset = %d\n",u32FirstFrameOffset);
            //Seek  Next ID3 tag
            fseek(fp1, u32FirstFrameOffset, SEEK_SET);
            fread(pBuff, sizeof(char), MP3_HEADER_SIZE, fp1);
			//AUDIO_DBG(("%x %x %x\n",pBuff[0],pBuff[1],pBuff[2]);
			if(!memcmp(&pBuff[0], &ID3_NUM[0], 3))
            {
                u32ID3Size = (MS_U32) (pBuff[6] & 0x7F);
                u32ID3Size <<= 7;
                u32ID3Size |= (MS_U32) (pBuff[7] & 0x7F);
                u32ID3Size <<= 7;
                u32ID3Size |= (MS_U32) (pBuff[8] & 0x7F);
                u32ID3Size <<= 7;
                u32ID3Size |= (MS_U32) (pBuff[9] & 0x7F);
                u32FirstFrameOffset += u32ID3Size + 10;
            }
            //AUDIO_DBG(("u32FirstFrameOffset = %d\n",u32FirstFrameOffset);
            fseek(fp1, u32FirstFrameOffset, SEEK_SET);
            fread(pBuff, sizeof(char), MP3_HEADER_SIZE, fp1);
            //AUDIO_DBG(("%x %x %x\n",pBuff[0],pBuff[1],pBuff[2]);
            if (((pBuff[0] == 0xFF) && ((pBuff[1] & 0xE0) == 0xE0)) //AAAAAAAA   AAA
                 //&& ((((pBuff[1] & 0x10) >> 4) | ((pBuff[1] & 0x08) >> 3)) != 0x01) //BB Don't care BB
                 && (((pBuff[1] & 0x06) >> 1) != 0x00))
            {
                b_Ret = true;
            }
        }

        FileFormatParserFreeBuff(pBuff);
    }
    return b_Ret;
}

//======================================================================//
// bool Audio_WMA_Format_Parser(FILE *fp1)
//======================================================================//
bool Audio_WMA_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	MS_U32	u32FirstFrameOffset = 0;
	unsigned char* pBuff = (unsigned char*)FileFormatParserMallocBuff(WMA_HEADER_SIZE);
	if(pBuff != NULL)
	{
		// Seek to the begin of file.
		fseek(fp1, 0, SEEK_SET);
		fread(pBuff, sizeof(char), WMA_HEADER_SIZE, fp1);
		//AUDIO_DBG(("%x %x %x\n",pBuff[0],pBuff[1],pBuff[2]);

		unsigned char ID3_NUM[16] = {0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, \
									  0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0X6C};//ID3
		if(memcmp(&pBuff[0], &ID3_NUM[0], 16))
		{
			AUDIO_ERR(("\nNo ID3v2 tag\n"));
			return false;
		}
		else
		{
			b_Ret = true;
		}

		FileFormatParserFreeBuff(pBuff);
	}
	return b_Ret;
}



//======================================================================//
// void Audio_Format_Parser(FILE *fp1)
//======================================================================//
bool Audio_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
    if(Audio_MP3_Format_Parser(fp1))
    {
        b_Ret = true;
        Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_AUDIO_MP3;
        strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "MP3");
    }
    if(Audio_WMA_Format_Parser(fp1))
    {
        b_Ret = true;
        Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_AUDIO_WMA;
        strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "WMA");
    }
	return b_Ret;
}


//======================================================================//
// void Movie_AVI_Format_Parser(FILE *fp1)
//======================================================================//
bool Movie_AVI_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	unsigned char* pBuff = (unsigned char*)FileFormatParserMallocBuff(AVI_HEADER_SIZE);
	if(pBuff != NULL)
	{
		// Seek to the begin of file.
		fseek(fp1, 0, SEEK_SET);
		// there is no much data for RIFF 'LIST'
		if (sizeof(File_MM_Video_RIFF_LIST) > Media_Info.FileSize)
		{
			return b_Ret;
		}
		//parser header
		fread(pBuff, sizeof(char), AVI_HEADER_SIZE, fp1);
		MApp_Set_WholeBigEndian(0);
		File_MM_Video_RIFF_LIST MMRiffList;		
		MMRiffList.u32Fcc = MApp_EndianChangeL(TO32BIT(pBuff));
		MMRiffList.u32Size = MApp_EndianChangeL(TO32BIT(pBuff + 4));
		MMRiffList.u32FccListType = MApp_EndianChangeL(TO32BIT(pBuff + 8));
        MOVIE_DBG(("TIFF file size = 0x%x, Need add 8 bytes for tiff and size mark.\n", MMRiffList.u32Size));
		if (MMRiffList.u32Fcc == MKTAG('R', 'I', 'F', 'F'))
		{
			if ((MMRiffList.u32FccListType == MKTAG('A', 'V', 'I', ' '))
				|| (MMRiffList.u32FccListType == MKTAG('A', 'V', 'I', 'X')))
			{
				b_Ret = true;
			}
		}
		
		FileFormatParserFreeBuff(pBuff);
	}
	return b_Ret;
}

//======================================================================//
// void Movie_Format_Parser(FILE *fp1)
//======================================================================//
bool Movie_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	if(Movie_AVI_Format_Parser(fp1)) 
	{
		b_Ret = true;
		Media_Info.FileMMSubTypeInfo.FileMMSubType = EN_MM_SUB_MOVIE_AVI;
		strcpy(Media_Info.FileMMSubTypeInfo.FileMMSubTypeName, "AVI");
	}
	return b_Ret;
}

//======================================================================//
// void File_Format_Parser(FILE *fp1)
//======================================================================//
bool File_Format_Parser(FILE *fp1)
{
	bool b_Ret = false;
	if(Photo_Format_Parser(fp1))
	{
		b_Ret = true;
		Media_Info.FileMMTypeInfo.FileMMType = EN_MM_PHOTO;
		strcpy(Media_Info.FileMMTypeInfo.FileMMTypeName, "Image");
	}
    else if(Audio_Format_Parser(fp1))
	{
		b_Ret = true;
        Media_Info.FileMMTypeInfo.FileMMType = EN_MM_AUDIO;
		strcpy(Media_Info.FileMMTypeInfo.FileMMTypeName, "Audio");
	}
	else if(Movie_Format_Parser(fp1))
	{
		b_Ret = true;
		Media_Info.FileMMTypeInfo.FileMMType = EN_MM_MOVIE;
		strcpy(Media_Info.FileMMTypeInfo.FileMMTypeName, "Video");
	}
	else
	{
		b_Ret = false;
		Media_Info.FileMMTypeInfo.FileMMType = EN_MM_NONE;
		strcpy(Media_Info.FileMMTypeInfo.FileMMTypeName, "Unknown");
	}
	return b_Ret;
}

//======================================================================//
// unsigned short MApp_Jpeg_MarkerLen(FILE *fp1, char marker)
//======================================================================//
bool MApp_Jpeg_MarkerLen(unsigned short Marker)
{
	if ((Marker >= (JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_APP0) 
		&& Marker <= (JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_APP15)) 
		|| ((JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_COM) == Marker))
		return true;
	else
		return false;
}

//======================================================================//
// unsigned short MApp_Jpeg_SearchMarker(FILE *fp1, unsigned short File_offset, char marker)
//======================================================================//
unsigned short MApp_Jpeg_SearchMarker(FILE *fp1, unsigned short File_offset, const int marker)
{
	unsigned char* pBuff = (unsigned char *)u32gExifBuffAddr;
	if(pBuff != NULL)
	{
		unsigned short u16Marker;
		unsigned short u16Len = 0;
		unsigned int u32Offset = 0;
		unsigned int u32OffsetInBlock = 0;

		fseek(fp1, File_offset, SEEK_SET);
		u16Len = fread((void *)u32gExifBuffAddr, sizeof(char), JPEG_DI_SIZE, fp1);
		// search SOI first
		if (!((JPEG_MARKER_PREFIX == pBuff[u32Offset]) && (JPEG_MARKER_SOI == pBuff[u32Offset + 1])))
		{
            IMAGE_ERR(("start is not SOI marker: %x\n",pBuff[u32Offset + 1]));
			return false;
		}
		u32Offset = 2;
		u32OffsetInBlock = 2;
		pBuff = (unsigned char *)(u32gExifBuffAddr + u32OffsetInBlock);
		do
		{
			u16Marker = TO16BIT(pBuff);
			u16Len = TO16BIT(pBuff + 2);
            IMAGE_DBG(("mark:0x%x, len:0x%x ",u16Marker, u16Len));
			if ((u16Marker > (JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_COM)) 
				|| (u16Marker < (JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_TEM)))
			{
                IMAGE_DBG(("\nfind mark:0x%x fail\n",marker));
				return false;
			}
			if (u16Marker == (JPEG_MARKER_PREFIX << 8 | marker))
			{
                IMAGE_DBG(("\nfind mark:0x%x in 0x%x\n",marker, u32Offset));
				break;
			}
			// length field is a marker
			if (((u16Len >= (JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_COM)) 
				&& (u16Len <= (JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_COM)) 
				&& (!MApp_Jpeg_MarkerLen(u16Marker))) 
				|| (u16Len == 0))
			{	
				u16Len = 4;
				u32Offset += 4;
				u32OffsetInBlock += 4;

				u16Marker = TO16BIT(pBuff + u16Len);
				while ((u16Marker > (JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_COM)) 
					|| (u16Marker < (JPEG_MARKER_PREFIX << 8 | JPEG_MARKER_TEM)))
				{
					u16Len += 2;
					u16Marker = TO16BIT(pBuff + u16Len);
					u32Offset += 2;
					u32OffsetInBlock += 2;
					if (u32OffsetInBlock >= JPEG_DI_SIZE)
					{
						while (u32OffsetInBlock >= JPEG_DI_SIZE)
						{
							if (fread((void *)u32gExifBuffAddr, sizeof(char), JPEG_DI_SIZE, fp1) == JPEG_DI_SIZE)
							{
                                IMAGE_DBG(("\nfind mark:0x%x fail at the end of file\n",marker));
								return false;// end of file
							}
							u32OffsetInBlock -= JPEG_DI_SIZE;
						}
					}
				}
			}
			else
			{
				u32Offset += (u16Len + 2);
				u32OffsetInBlock += (u16Len + 2);
			}
			if (u32OffsetInBlock >= JPEG_DI_SIZE)
			{
				while (u32OffsetInBlock >= JPEG_DI_SIZE)
				{
					if (fread((void *)u32gExifBuffAddr, sizeof(char), JPEG_DI_SIZE, fp1) != JPEG_DI_SIZE)
					{
                        IMAGE_ERR(("\nfind mark:0x%x fail at the end of file\n",marker));
						return false;// end of file
					}
					u32OffsetInBlock -= JPEG_DI_SIZE;
				} 
			}		   
            IMAGE_DBG((" %d\n", u32Offset));
			pBuff = (unsigned char *)(u32gExifBuffAddr + u32OffsetInBlock);
		}while(1);

		return u32Offset;
	}
	return false;
}

//======================================================================//
// bool MApp_Jpeg_SOFParser(FILE *fp1, unsigned short File_offset, unsigned short SOF_offset)
//======================================================================//
bool MApp_Jpeg_SOFParser(FILE *fp1, unsigned short File_offset, unsigned short SOF_offset)
{
	unsigned char* pBuff = (unsigned char *)u32gExifBuffAddr;
	if(pBuff != NULL)
	{
		if(File_offset)
		{
			fseek(fp1, File_offset + SOF_offset, SEEK_SET);
			fread((void *)u32gExifBuffAddr, sizeof(char), JPEG_DI_SIZE, fp1);
			pBuff = (unsigned char *)(u32gExifBuffAddr); 
			Media_Info.FileMMImageJPEGDetailInfo.ThumbnailPrecision = *(pBuff + 4);
			Media_Info.FileMMImageJPEGDetailInfo.ThumbnailImageHeight = TO16BIT(pBuff + 5);
			Media_Info.FileMMImageJPEGDetailInfo.ThumbnailImageWidth = TO16BIT(pBuff + 7);
			Media_Info.FileMMImageJPEGDetailInfo.ThumbnailColorComponents = *(pBuff + 9);
		}
		else
		{
			fseek(fp1, SOF_offset, SEEK_SET);
			fread((void *)u32gExifBuffAddr, sizeof(char), JPEG_DI_SIZE, fp1);
			pBuff = (unsigned char *)(u32gExifBuffAddr); 
			Media_Info.FileMMImageJPEGDetailInfo.Precision = *(pBuff + 4);
			Media_Info.FileMMImageJPEGDetailInfo.ImageHeight = TO16BIT(pBuff + 5);
			Media_Info.FileMMImageJPEGDetailInfo.ImageWidth = TO16BIT(pBuff + 7);
			Media_Info.FileMMImageJPEGDetailInfo.ColorComponents = *(pBuff + 9);
		}
	}
	return true;
}

//======================================================================//
// static unsigned int MApp_EndianChangeL(unsigned int u32Val) 0--
//======================================================================//
static bool MApp_Set_WholeBigEndian(unsigned short BigEndian)
{
	WholeBigEndian = BigEndian;
	return true;
}


//======================================================================//
// static unsigned __int64 MApp_EndianChangeLL(unsigned __int64 u64Val)
//======================================================================//
static unsigned __int64 MApp_EndianChangeLL(unsigned __int64 u64Val)
{  
	if (WholeBigEndian)
	{
		return u64Val;
	}		
	else
	{	 
        //printf(" 0x%x ",((unsigned __int64)(u64Val & 0xff000000) << 8)>>8);
		return (unsigned __int64)EndianChangeLL(u64Val);
	}
} 

//======================================================================//
// static unsigned int MApp_EndianChangeL(unsigned int u32Val)
//======================================================================//
static unsigned int MApp_EndianChangeL(unsigned int u32Val)
{  
	if (WholeBigEndian)
	{
		return u32Val;
	}		
	else
	{
		return EndianChangeL(u32Val);
	}	
} 

//======================================================================//
// static unsigned short MApp_EndianChangeS(unsigned short u16Val)
//======================================================================//
static unsigned short MApp_EndianChangeS(unsigned short u16Val)
{
	if (WholeBigEndian)
	{
		return u16Val;
	}	
	else
	{
		return EndianChangeS(u16Val);
	}	
}

//======================================================================//
// bool MApp_Jpeg_APP1Parser(FILE *fp1, unsigned short APP1_offset)
//======================================================================//
bool MApp_Jpeg_APP1Parser(FILE *fp1, unsigned short APP1_offset)
{
	unsigned char* pBuff = (unsigned char *)u32gExifBuffAddr;
	unsigned int i = 0;
	if(pBuff != NULL)
	{
		unsigned int u32Offset = 0;
		unsigned int u32Temp = 0;
		unsigned int u32Len = 0;
		unsigned short u16Marker;
		unsigned short u16Len = 0;

		fseek(fp1, APP1_offset, SEEK_SET);
		fread((void *)u32gExifBuffAddr, sizeof(char), JPEG_DI_SIZE, fp1);
		//===========================find exif tag===============================//
		pBuff = (unsigned char *)(u32gExifBuffAddr + 4);
		u32Temp = TO32BIT(pBuff);
		if (u32Temp != JPEG_TAG_EXIF)
		{
			return false;
		}
		//===========================determine endian type===============================//
		pBuff += 6;
		u16Marker = TO16BIT(pBuff);
		if (JPEG_TIFF_BIG_ENDIAN == u16Marker)
		{
			Media_Info.FileMMImageJPEGDetailInfo.TiffBigEndian = 1;
		}
		else if (JPEG_TIFF_LITTLE_ENDIAN == u16Marker)
		{
			Media_Info.FileMMImageJPEGDetailInfo.TiffBigEndian = 0;
		}
		MApp_Set_WholeBigEndian(Media_Info.FileMMImageJPEGDetailInfo.TiffBigEndian);
		unsigned char *pTiffHdrPtr = pBuff;
		unsigned char *pExifHdrPtr;
		unsigned char *pNextIfd;
		//===========================marker should be 0x002A===============================//
		pBuff += 2;
		u16Marker = MApp_EndianChangeS(TO16BIT(pBuff));
		if (u16Marker != 0x002A)
		{
			return false;
		}
		//===========================obtain marker length===============================//
		pBuff += 2;
		if (pBuff == NULL)
		{
			return false;
		}
		u16Len = MApp_EndianChangeL(TO32BIT(pBuff));
		//===========================1st IFD start found, obtain number of entry===============================//
		pBuff += (u16Len - 4);
		// 0th IFD start here
		if (pBuff == NULL)
		{
			return false;
		}
		unsigned short u16NumOfEntry = 0;
		u16NumOfEntry = MApp_EndianChangeS(TO16BIT(pBuff));
		//===========================obtain detail tiff info===============================//
		pBuff += 2;
		for (i = 0; i < u16NumOfEntry; i++)
		{
			if (pBuff == NULL)
			{
				return false;
			} 
			u16Marker = MApp_EndianChangeS(TO16BIT(pBuff));
			u32Len = MApp_EndianChangeL(TO32BIT(pBuff + 4));
			u32Temp = MApp_EndianChangeL(TO32BIT(pBuff + 8));
			switch (u16Marker)
			{
				case JPEG_TIFF_X_RESOLUTION:
					Media_Info.FileMMImageJPEGDetailInfo.X_Resolution \
						= MApp_EndianChangeL(TO32BIT(pTiffHdrPtr + u32Temp)) / MApp_EndianChangeL(TO32BIT(pTiffHdrPtr + u32Temp + 4));
					break;

				case JPEG_TIFF_Y_RESOLUTION:
					Media_Info.FileMMImageJPEGDetailInfo.Y_Resolution \
						= MApp_EndianChangeL(TO32BIT(pTiffHdrPtr + u32Temp)) / MApp_EndianChangeL(TO32BIT(pTiffHdrPtr + u32Temp + 4));
					break;

				case JPEG_TIFF_RESOLUTION_UNIT:
                    IMAGE_DBG(("find Resolution Unit u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
					Media_Info.FileMMImageJPEGDetailInfo.ResolutionUnit = (char)(u32Temp >> 16);
					break;

				case JPEG_TIFF_TAG_SOFTWARE:
					if (u32Len >= (TIFF_MAX_LEN - 1))// max len = 20
					{
						u32Len = TIFF_MAX_LEN - 1;
					}					
                    IMAGE_DBG(("find software u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
					if ((u32Temp + u32Len) < JPEG_DI_SIZE)
					{
						memcpy(Media_Info.FileMMImageJPEGDetailInfo.Software, &pTiffHdrPtr[u32Temp], u32Len);
					}
					break;

				case JPEG_TIFF_TAG_DATETIME:
					if (u32Len >= (TIFF_MAX_LEN - 1))// max len = 20
					{
						u32Len = TIFF_MAX_LEN - 1;
					}					
                    IMAGE_DBG(("find date u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
					if ((u32Temp + u32Len) < JPEG_DI_SIZE)
					{
						memcpy(Media_Info.FileMMImageJPEGDetailInfo.DataTime, &pTiffHdrPtr[u32Temp], u32Len);
					}
					break;

				case JPEG_TIFF_TAG_TITLE:
					if (u32Len >= (TIFF_MAX_LEN - 1))// max len = 20
					{
						u32Len = TIFF_MAX_LEN - 1;
					}					
                    IMAGE_DBG(("find title u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
					if ((u32Temp + u32Len) < JPEG_DI_SIZE)
					{
						memcpy(Media_Info.FileMMImageJPEGDetailInfo.Title, &pTiffHdrPtr[u32Temp], u32Len);
					}
					break;

				case JPEG_TIFF_TAG_ORIENTATION:
                    IMAGE_DBG(("find orientation u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
					Media_Info.FileMMImageJPEGDetailInfo.Orientation = (char)(u32Temp >> 16);
					break;

				case JPEG_TIFF_TAG_EXIF_POINTER:
                    IMAGE_DBG(("find exif pointer u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
					pExifHdrPtr = &pTiffHdrPtr[u32Temp];
					break;

				default:
					break;
			}
			pBuff += 12;
		}
	//===========================EXIF IFD found, obtain number of entry===============================//
		u32Temp = MApp_EndianChangeL(TO32BIT(pBuff));
		if (u32Temp == 0)
			pNextIfd = NULL;
		else
			pNextIfd = &pTiffHdrPtr[u32Temp];

		//Exif IFD
		pBuff = pExifHdrPtr;
		if (pBuff == NULL)
		{
			return false;
		}
		u16NumOfEntry = MApp_EndianChangeS(TO16BIT(pBuff));
		pBuff += 2;	
		for (i = 0; i < u16NumOfEntry; i++)
		{
			if (pBuff == NULL)
			{
				return false;
			}
			u16Marker = MApp_EndianChangeS(TO16BIT(pBuff));
			u32Len = MApp_EndianChangeL(TO32BIT(pBuff + 4));
			u32Temp = MApp_EndianChangeL(TO32BIT(pBuff + 8));

			switch (u16Marker)
			{
			case JPEG_EXIF_TAG_DATETIME:					
                IMAGE_DBG(("find exif date u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
				if (Media_Info.FileMMImageJPEGDetailInfo.DataTime[0] == 0)
					break;
				if((u32Temp + u32Len) < JPEG_DI_SIZE)
				{
					memcpy(Media_Info.FileMMImageJPEGDetailInfo.DataTime, &pTiffHdrPtr[u32Temp], u32Len);
				}
				break;

			case JPEG_EXIF_TAG_COLOR_SPACE:
                IMAGE_DBG(("find exif color space u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
				Media_Info.FileMMImageJPEGDetailInfo.ExifColorSpace = u32Temp;
				break;

			case JPEG_EXIF_TAG_IMAGE_WIDTH:
                IMAGE_DBG(("find exif image width u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
				Media_Info.FileMMImageJPEGDetailInfo.ExifImageWidth = u32Temp;
				break;

			case JPEG_EXIF_TAG_IMAGE_HEIGHT:
                IMAGE_DBG(("find exif image width u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
				Media_Info.FileMMImageJPEGDetailInfo.ExifImageHeight = u32Temp;
				break;

			default:
				break;

			}

			pBuff += 12;
		}
	//===========================next IFD found, obtain number of entry===============================//
		if (pNextIfd == NULL)
		{
			return false;
		}
		u16NumOfEntry = MApp_EndianChangeS(TO16BIT(pNextIfd));
		pNextIfd += 2;
		for (i = 0; i < u16NumOfEntry; i++)
		{
			if (pNextIfd == NULL)
			{
				return false; 
			}
			u16Marker = MApp_EndianChangeS(TO16BIT(pNextIfd));
			u32Len = MApp_EndianChangeL(TO32BIT(pNextIfd+4));
			u32Temp = MApp_EndianChangeL(TO32BIT(pNextIfd+8));
			switch (u16Marker)
			{
			case JPEG_TIFF_COMPRESSION:
                IMAGE_DBG(("find tiff compression u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
				Media_Info.FileMMImageJPEGDetailInfo.ThumbnailCompression = (char)(u32Temp >> 16);
				break;

			case JPEG_TIFF_SOI_OFFSET:
                IMAGE_DBG(("find tiff SOI offset u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
				Media_Info.FileMMImageJPEGDetailInfo.ImageThumbnail = true;
				Media_Info.FileMMImageJPEGDetailInfo.ThumbnailOffset = u32Temp + APP1_offset + 4 + 6;
				break;

			case JPEG_TIFF_SOI_OFFSET_BYTE_COUNT:
                IMAGE_DBG(("find tiff SOI offset byte count u32Len = 0x%x, u32Temp = 0x%x\n", u32Len, u32Temp));
				Media_Info.FileMMImageJPEGDetailInfo.ThumbnailByteCount = u32Temp;
				break;

			}
			pNextIfd += 12;
		}
	}
	return true;
}
 
//======================================================================//
// bool File_Detail_Info_JPG_Parser_Base(FILE *fp1)
//======================================================================//
bool File_Detail_Info_JPG_Parser_Base(FILE *fp1, unsigned short File_offset)
{
	bool b_Ret = false;
	unsigned short SOF0_offset = 0;
	unsigned short SOF2_offset = 0;
	unsigned short APP1_offset = 0;
// search SOF0 + SOF2 + APP1 marker's offset
	SOF0_offset = MApp_Jpeg_SearchMarker(fp1, File_offset, JPEG_MARKER_SOF0);
	SOF2_offset = MApp_Jpeg_SearchMarker(fp1, File_offset, JPEG_MARKER_SOF2);
	APP1_offset = MApp_Jpeg_SearchMarker(fp1, File_offset, JPEG_MARKER_APP1);
    IMAGE_DBG(("SOF0_offset = 0x%x, SOF2_offset = 0x%x, APP1_offset = 0x%x\n", SOF0_offset, SOF2_offset, APP1_offset));
	
	if(SOF0_offset)
	{
		if(File_offset)
		{
			Media_Info.FileMMImageJPEGDetailInfo.ThumbnailImageProgressive = 0;//baseline
		}
		else
		{
			Media_Info.FileMMImageJPEGDetailInfo.ImageProgressive = 0;//baseline
		}		
		MApp_Jpeg_SOFParser(fp1, File_offset, SOF0_offset);
	}
	else if(SOF2_offset)
	{
		if(File_offset)
		{
			Media_Info.FileMMImageJPEGDetailInfo.ThumbnailImageProgressive = 1;//progressive
		}
		else
		{
			Media_Info.FileMMImageJPEGDetailInfo.ImageProgressive = 1;//progressive
		}		
		MApp_Jpeg_SOFParser(fp1, File_offset, SOF2_offset);
	}
	if(APP1_offset)
	{
		MApp_Jpeg_APP1Parser(fp1, APP1_offset);
	} 
	b_Ret = true;
	return b_Ret;
}

//======================================================================//
// bool File_Detail_Info_JPG_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_JPG_Parser(FILE *fp1)
{
	bool b_Ret = false;
	u32gExifBuffAddr = (unsigned int)FileFormatParserMallocBuff(JPEG_DI_SIZE);
	/*==============parser base data================*/
	File_Detail_Info_JPG_Parser_Base(fp1, 0);
	
	/*==============parser thumbnail data================*/
	if(Media_Info.FileMMImageJPEGDetailInfo.ImageThumbnail)
	{
		File_Detail_Info_JPG_Parser_Base(fp1, Media_Info.FileMMImageJPEGDetailInfo.ThumbnailOffset);
	}

	FileFormatParserFreeBuff((void *)u32gExifBuffAddr);
	b_Ret = true;
	return b_Ret;
}

//======================================================================//
// bool File_Detail_Info_BMP_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_BMP_Parser(FILE *fp1)
{
	bool b_Ret = false;
	u32gExifBuffAddr = (unsigned int)FileFormatParserMallocBuff(BMP_DI_SIZE + 1);
	fseek(fp1, 0, SEEK_SET);
	fread((void *)u32gExifBuffAddr, sizeof(char), BMP_DI_SIZE, fp1);
	unsigned char* pBuff = (unsigned char *)u32gExifBuffAddr;

	if(pBuff != NULL)
	{
		unsigned int u32Offset = 0;
		unsigned int u32Len = 0;
		unsigned short u16Marker;
		unsigned short u16Len = 0;
		unsigned char *pDataStart;

		fseek(fp1, 0, SEEK_SET);
		u16Marker = TO16BIT(pBuff);
		//============================file header=============================
		if(u16Marker != BMP_SIGNATURE)
		{
            IMAGE_ERR(("Marker unmatch BMP signature"));
			return false;
		}		
		//============================file length=============================
		pBuff += 2; 
		u32Len = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================data offset=============================
		pBuff += 4;
		pBuff += 4;//reserved, must value zero
		Media_Info.FileMMImageBMPDetailInfo.DataOffset = MApp_EndianChangeL(TO32BIT(pBuff));
		pDataStart = pBuff + u32Offset;
		//============================file header info length=============================
		pBuff += 4;
		u32Offset = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================bitmap width=============================
		pBuff += 4;
		Media_Info.FileMMImageBMPDetailInfo.ImageWidth = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================bitmap height=============================
		pBuff += 4;
		Media_Info.FileMMImageBMPDetailInfo.ImageHeight = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================number of color planes=============================
		pBuff += 4;
		u16Len = MApp_EndianChangeS(TO16BIT(pBuff));
		//============================bits per pixel=============================
		pBuff += 2;
		Media_Info.FileMMImageBMPDetailInfo.Precision = MApp_EndianChangeS(TO16BIT(pBuff));
		//============================compression specified=============================
		pBuff += 2;
		Media_Info.FileMMImageBMPDetailInfo.Compression = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================data size=============================
		pBuff += 4;
		Media_Info.FileMMImageBMPDetailInfo.DataSize = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================horizontal resolution=============================
		pBuff += 4;
		Media_Info.FileMMImageBMPDetailInfo.HorizontalResolution = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================vertical resolution=============================
		pBuff += 4;
		Media_Info.FileMMImageBMPDetailInfo.VerticalResolution = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================color index count=============================
		pBuff += 4;
		Media_Info.FileMMImageBMPDetailInfo.ColorIndexCount = MApp_EndianChangeL(TO32BIT(pBuff));
		//============================important color index count=============================
		pBuff += 4;
		Media_Info.FileMMImageBMPDetailInfo.ImportantColorIndexCount = MApp_EndianChangeL(TO32BIT(pBuff));
	}
	FileFormatParserFreeBuff((void *)u32gExifBuffAddr);
	b_Ret = true;
	return b_Ret;
}

//======================================================================//
// bool File_Detail_Info_PNG_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_PNG_Parser(FILE *fp1)
{
	bool b_Ret = false;
	u32gExifBuffAddr = (unsigned int)FileFormatParserMallocBuff(PNG_DI_SIZE + 1);
	fseek(fp1, 0, SEEK_SET);
	fread((void *)u32gExifBuffAddr, sizeof(char), PNG_DI_SIZE, fp1);
	unsigned char* pBuff = (unsigned char *)u32gExifBuffAddr;
	unsigned char* pChunk = pBuff;

	if(pChunk != NULL)
	{
		unsigned char PNG_SIGNATURE_NUM[8] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A}; 
		if (memcmp(&pChunk[0], &PNG_SIGNATURE_NUM[0], 8)) 
		{
            IMAGE_ERR(("Marker unmatch PNG file signature"));
			return b_Ret;
		}
		unsigned int u32Offset = 8;
		unsigned int u32Len = 0;
		unsigned int u32ChunkLen = 0;
		unsigned int u32Chunk = 0;
		bool b_End = false;
		do
		{
			if(u32Offset > PNG_DI_SIZE)
			{
				break;
			}
			pChunk = (unsigned char *)pBuff + u32Offset;
			u32ChunkLen = TO32BIT(pChunk);
			u32Len = 4;
			u32Chunk = TO32BIT(pChunk + u32Len);
			u32Len += 4;
            IMAGE_DBG(("0x%x 0x%x 0x%x\n", u32Offset, u32ChunkLen, u32Chunk));
			switch(u32Chunk)
			{
			case PNG_CHUNK_TYPE_IHDR:
				{				
				Media_Info.FileMMImagePNGDetailInfo.ImageWidth = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.ImageHeight = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.BitDepth = pChunk[u32Len];
				u32Len += 1;
				Media_Info.FileMMImagePNGDetailInfo.ColorType = pChunk[u32Len];
				u32Len += 1;
				Media_Info.FileMMImagePNGDetailInfo.CompressionMethod = pChunk[u32Len];
				u32Len += 1;
				Media_Info.FileMMImagePNGDetailInfo.FilterMethod = pChunk[u32Len];
				u32Len += 1;
				Media_Info.FileMMImagePNGDetailInfo.InterlaceMethod = pChunk[u32Len];
				u32Len += 1;
				}
				break;
			case PNG_CHUNK_TYPE_pHYs:
				{				
				Media_Info.FileMMImagePNGDetailInfo.PixelPerUnitXAxis = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.PixelPerUnitYAxis = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.UnitSpecifier = pChunk[u32Len];
				u32Len += 1;
				}
				break;
			case PNG_CHUNK_TYPE_gAMA:
				{				
				Media_Info.FileMMImagePNGDetailInfo.Gamma = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				}
				break;
			case PNG_CHUNK_TYPE_cHRM:
				{				
				Media_Info.FileMMImagePNGDetailInfo.WhitePX = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.WhitePY = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.RedX = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.RedY = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.GreenX = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.GreenY = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.BlueX = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				Media_Info.FileMMImagePNGDetailInfo.BlueY = TO32BIT(pChunk + u32Len);
				u32Len += 4;
				}
				break;
			case PNG_CHUNK_TYPE_IDAT:
				break;
			case PNG_CHUNK_TYPE_IEND:
				{
					b_End = true;
				}
				break;
			default:
				break;
			}
			if(b_End)
			{
				b_Ret = true;
				break;
			}
			u32Offset += (12 + u32ChunkLen);
			if (u32Offset >= PNG_DI_SIZE)
			{
				while (u32Offset >= PNG_DI_SIZE)
				{
					if (fread((void *)u32gExifBuffAddr, sizeof(char), PNG_DI_SIZE, fp1) != PNG_DI_SIZE)
					{
                        IMAGE_DBG(("\nfind chunk:0x%x at the end of file\n",u32Chunk));
						u32Offset -= PNG_DI_SIZE;
						break;
					}
					u32Offset -= PNG_DI_SIZE;
				}
			}
		}while(1);
		FileFormatParserFreeBuff(pBuff);
	}
	
	return b_Ret;
}


//======================================================================//
// bool File_Detail_Info_TIFF_Parser_Offset(FILE *fp1, unsigned int Offset, unsigned short len)
//======================================================================//
unsigned char* File_Detail_Info_TIFF_Parser_Offset(FILE *fp1, unsigned int Offset, unsigned short len)
{
	unsigned int malloc_size = 0;
	if((TIFF_DI_SIZE - (Offset % TIFF_DI_SIZE)) > len)
	{
		malloc_size = TIFF_DI_SIZE;
	}
	else
	{
		malloc_size = TIFF_DI_SIZE/2;
	}
	u32TiffBuffAddr = (unsigned int)FileFormatParserMallocBuff(malloc_size + 1);
	fseek(fp1, 0, SEEK_SET);
	fread((void *)u32TiffBuffAddr, sizeof(char), malloc_size, fp1);
	
	if (Offset >= malloc_size)
	{
		while (Offset >= malloc_size)
		{
			if (fread((void *)u32TiffBuffAddr, sizeof(char), malloc_size, fp1) != malloc_size)
			{
                IMAGE_DBG(("\nfind IFD Tag.\n"));
				Offset -= malloc_size;
				break;
			}
			Offset -= malloc_size;
		}
	}
	return (unsigned char* )(u32TiffBuffAddr + Offset);
}

//======================================================================//
// bool File_Detail_Info_TIFF_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_TIFF_Parser(FILE *fp1)
{
	bool b_Ret = false; 
	u32gExifBuffAddr = (unsigned int)FileFormatParserMallocBuff(TIFF_DI_SIZE + 1);
	fseek(fp1, 0, SEEK_SET);
	fread((void *)u32gExifBuffAddr, sizeof(char), TIFF_DI_SIZE, fp1);
	unsigned char* pBuff = (unsigned char *)u32gExifBuffAddr;

	unsigned short u16Marker = 0;
	unsigned int u32Len = 0;
	//============================parser file header=============================
	//============================endian marker=============================
	u16Marker = TO16BIT(pBuff);
	if (JPEG_TIFF_BIG_ENDIAN == u16Marker)
	{
		Media_Info.FileMMImageTIFFDetailInfo.TiffBigEndian = 1;
	}
	else if (JPEG_TIFF_LITTLE_ENDIAN == u16Marker)
	{
		Media_Info.FileMMImageTIFFDetailInfo.TiffBigEndian = 0;
	}
	MApp_Set_WholeBigEndian(Media_Info.FileMMImageTIFFDetailInfo.TiffBigEndian);
	u32Len += 2;
	//============================Version Number=============================
	Media_Info.FileMMImageTIFFDetailInfo.VersionNum = MApp_EndianChangeS(TO16BIT(pBuff + u32Len));
	u32Len += 2;
	//============================first IFD offset=============================
	Media_Info.FileMMImageTIFFDetailInfo.FirstIFDOffset = MApp_EndianChangeL(TO32BIT(pBuff + u32Len));
	u32Len += 4;
	//============================parser first IFD=============================
	//============================jump to fisrt IFD location=============================
	unsigned int u32Offset = Media_Info.FileMMImageTIFFDetailInfo.FirstIFDOffset;
	if (u32Offset >= TIFF_DI_SIZE)
	{
		while (u32Offset >= TIFF_DI_SIZE)
		{
			if (fread((void *)u32gExifBuffAddr, sizeof(char), TIFF_DI_SIZE, fp1) != TIFF_DI_SIZE)
			{
                IMAGE_DBG(("\nfind first IFD.\n"));
				u32Offset -= TIFF_DI_SIZE;
				break;
			}
			u32Offset -= TIFF_DI_SIZE;
		}
	}
	u32Len = u32Offset;
	//============================number of directory entries=============================
	Media_Info.FileMMImageTIFFDetailInfo.DirEntryNum = MApp_EndianChangeS(TO16BIT(pBuff + u32Len));
	u32Len += 2;
	//============================parser directory entry=============================
	unsigned short i;
	char type_len = 0;
	unsigned short true_len = 0;
	File_MM_Image_TIFF_IFD Tiff_IFD;
	for(i = 0; i < Media_Info.FileMMImageTIFFDetailInfo.DirEntryNum; i++)
	{
		Tiff_IFD.TiffIFDTag = MApp_EndianChangeS(TO16BIT(pBuff + u32Len));
		u32Len += 2;
		Tiff_IFD.TiffIFDFieldType = MApp_EndianChangeS(TO16BIT(pBuff + u32Len));
		u32Len += 2;
		Tiff_IFD.TiffIFDNum = MApp_EndianChangeL(TO32BIT(pBuff + u32Len));
		u32Len += 4;
		Tiff_IFD.TiffIFDData = MApp_EndianChangeL(TO32BIT(pBuff + u32Len));
		u32Len += 4;
		switch(Tiff_IFD.TiffIFDFieldType)
		{
		case EN_MM_TIFF_IFDFIELDTYPE_BYTE:
			type_len = 1;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_ASCII:
			type_len = 1;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_SHORT:
			type_len = 2;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_LONG:
			type_len = 4;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_RATIONAL:
			type_len = 8;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_SBYTE:
			type_len = 1;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_UNDEFINED:
			type_len = 1;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_SSHORT:
			type_len = 2;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_SLONG:
			type_len = 4;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_SRATIONAL:
			type_len = 8;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_FLOAT:
			type_len = 4;
			break;
		case EN_MM_TIFF_IFDFIELDTYPE_DOUBLE:
			type_len = 8;
			break;
		default:
			break;
		}
		true_len = type_len * Tiff_IFD.TiffIFDNum;
		//printf("[%d]0x%x %d 0x%x\n", __LINE__, Tiff_IFD.TiffIFDTag, true_len, Tiff_IFD.TiffIFDData);
		switch(Tiff_IFD.TiffIFDTag)
		{
		case TIFF_IFD_Tag_IMAGEWIDTH:
			{
				if(true_len <= 4)
				{ 
					Media_Info.FileMMImageTIFFDetailInfo.ImageWidth = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_IMAGELENGTH:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.ImageLength = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_BITSPERSAMPLE:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.BitsPerSample[0] = Tiff_IFD.TiffIFDData;
				}
				else
				{
					int j;
					unsigned char* pTemp = File_Detail_Info_TIFF_Parser_Offset(fp1, Tiff_IFD.TiffIFDData, true_len);
					for(j = 0; j < Tiff_IFD.TiffIFDNum; j++)
					{
						Media_Info.FileMMImageTIFFDetailInfo.BitsPerSample[j] = MApp_EndianChangeS(TO16BIT(pTemp + j * 2));
					}
					FileFormatParserFreeBuff((void *)u32TiffBuffAddr);
				}
			}
			break;
		case TIFF_IFD_Tag_COMPRESSION:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.Compression = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_PHOTOMETRICINTERPRETATION:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.PhotoMetricInterpretation = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_STRIPOFFSETS:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.StripOffsets = Tiff_IFD.TiffIFDData;
				}
				else
				{
				}
			}
			break;
		case TIFF_IFD_Tag_SAMPLESPERPIXEL:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.SamplesPerPixel = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_ROWPERSTRIP:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.RowsPerStrip = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_STRIPBYTECOUNTS:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.StripByteCounts = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_XRESOLUTION:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.XResolution = Tiff_IFD.TiffIFDData;
				}
				else
				{
					unsigned char* pTemp = File_Detail_Info_TIFF_Parser_Offset(fp1, Tiff_IFD.TiffIFDData, true_len);
printf("0x%x %x %x %x %x %x %x %x", pTemp[0],pTemp[1],pTemp[2],pTemp[3],pTemp[4],pTemp[5],pTemp[6],pTemp[7]);		
					Media_Info.FileMMImageTIFFDetailInfo.XResolution = MApp_EndianChangeLL(TO64BIT(pTemp));
					FileFormatParserFreeBuff((void *)u32TiffBuffAddr);
				}
			}  
			break;
		case TIFF_IFD_Tag_YRESOLUTION:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.YResolution = Tiff_IFD.TiffIFDData;
				}
				else
				{
					unsigned char* pTemp = File_Detail_Info_TIFF_Parser_Offset(fp1, Tiff_IFD.TiffIFDData, true_len);
					Media_Info.FileMMImageTIFFDetailInfo.YResolution = MApp_EndianChangeLL(TO64BIT(pTemp));
					FileFormatParserFreeBuff((void *)u32TiffBuffAddr);
				}
			}
			break;
		case TIFF_IFD_Tag_PLANARCONFIGURATION:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.PlanarConfiguration = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_RESOLUTIONUNIT:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.ResolutionUnit = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		case TIFF_IFD_Tag_PREDICTOR:
			{
				if(true_len <= 4)
				{
					Media_Info.FileMMImageTIFFDetailInfo.Predictor = Tiff_IFD.TiffIFDData;
				}
			}
			break;
		}
	}
	FileFormatParserFreeBuff(pBuff);
	b_Ret = true;
	return b_Ret;
}


//======================================================================//
// bool File_Detail_Info_GIF_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_GIF_Parser(FILE *fp1)
{
    bool b_Ret = false;
    u32gExifBuffAddr = (unsigned int)FileFormatParserMallocBuff(sizeof(File_MM_Image_GIF_Detail_Info) + 1);
    fseek(fp1, 0, SEEK_SET);
    fread((void *)u32gExifBuffAddr, sizeof(char), sizeof(File_MM_Image_GIF_Detail_Info), fp1);
    unsigned char* pBuff = (unsigned char *)u32gExifBuffAddr;
    unsigned int u32Len = 0;

    MApp_Set_WholeBigEndian(0);
    memset(Media_Info.FileMMImageGIFDetailInfo.u8Header, 0, GIF_HEADER_SIZE);
    memcpy(Media_Info.FileMMImageGIFDetailInfo.u8Header, &pBuff[u32Len], GIF_HEADER_SIZE - 1);
    u32Len += 3;
    memset(Media_Info.FileMMImageGIFDetailInfo.u8Version, 0, GIF_VERSION_SIZE);
    memcpy(Media_Info.FileMMImageGIFDetailInfo.u8Version, &pBuff[u32Len], GIF_VERSION_SIZE - 1);
    u32Len += 3;
    //=============================assign value for Output==================================
    Media_Info.FileMMImageGIFDetailInfo.eOutputFormat = E_GIF_ARGB8888;
    Media_Info.FileMMImageGIFDetailInfo.u8OutputPixelSize = 4;
    Media_Info.FileMMImageGIFDetailInfo.bStop = TRUE;
    //=============================assign value for Output==================================
    Media_Info.FileMMImageGIFDetailInfo.animation = (File_MM_Image_GIF_ANIM*)FileFormatParserMallocBuff(sizeof(File_MM_Image_GIF_ANIM) + 1);
    if (Media_Info.FileMMImageGIFDetailInfo.animation == NULL)
    {
        b_Ret = false;
        goto end_of_func;
    }
    memset(Media_Info.FileMMImageGIFDetailInfo.animation, 0, sizeof(File_MM_Image_GIF_ANIM));
    //=============================basic info==================================
    Media_Info.FileMMImageGIFDetailInfo.u32Width = MApp_EndianChangeS(TO16BIT(pBuff + u32Len));
    u32Len += 2;
    Media_Info.FileMMImageGIFDetailInfo.u32Height = MApp_EndianChangeS(TO16BIT(pBuff + u32Len));
    u32Len += 2;
    Media_Info.FileMMImageGIFDetailInfo.global_bit_pixel = 2 << (*(pBuff + u32Len) & 0x07);
    Media_Info.FileMMImageGIFDetailInfo.global_color_resolution = ((*(pBuff + u32Len) & 0x70) >> 3) + 1;
    Media_Info.FileMMImageGIFDetailInfo.has_global_colortable = (*(pBuff + u32Len) & 0x80) != 0;
    u32Len += 1;
    Media_Info.FileMMImageGIFDetailInfo.background_index = *(pBuff + u32Len);
    u32Len += 1;
    Media_Info.FileMMImageGIFDetailInfo.aspect_ratio = *(pBuff + u32Len);
    u32Len += 1;

    //=============================animation==================================
    Media_Info.FileMMImageGIFDetailInfo.animation->bg_red = 0;
    Media_Info.FileMMImageGIFDetailInfo.animation->bg_green = 0;
    Media_Info.FileMMImageGIFDetailInfo.animation->bg_blue = 0;
    Media_Info.FileMMImageGIFDetailInfo.animation->width = Media_Info.FileMMImageGIFDetailInfo.u32Width;
    Media_Info.FileMMImageGIFDetailInfo.animation->height = Media_Info.FileMMImageGIFDetailInfo.u32Height;

    Media_Info.FileMMImageGIFDetailInfo.u32TotalRowOutputted = 0;
    Media_Info.FileMMImageGIFDetailInfo.u32TotalRowProcessed = 0;
    Media_Info.FileMMImageGIFDetailInfo.frame = NULL;
    Media_Info.FileMMImageGIFDetailInfo.state = GIF_GET_NEXT_STEP;
    Media_Info.FileMMImageGIFDetailInfo.has_local_colortable = FALSE;

    Media_Info.FileMMImageGIFDetailInfo.buf = NULL;
    Media_Info.FileMMImageGIFDetailInfo.gif89.transparent =  - 1;
    Media_Info.FileMMImageGIFDetailInfo.gif89.delay_time =  - 1;

    Media_Info.FileMMImageGIFDetailInfo.gif89.input_flag =  - 1;
    Media_Info.FileMMImageGIFDetailInfo.gif89.disposal =  - 1;
    Media_Info.FileMMImageGIFDetailInfo.animation->loop = 1;
    Media_Info.FileMMImageGIFDetailInfo.in_loop_extension = FALSE;
    Media_Info.FileMMImageGIFDetailInfo.amount_needed = 0;
    Media_Info.FileMMImageGIFDetailInfo.animation->n_frames = 0;
    Media_Info.FileMMImageGIFDetailInfo.stop_after_first_frame = 0;


    if ((0 == Media_Info.FileMMImageGIFDetailInfo.u32Width) || (0 == Media_Info.FileMMImageGIFDetailInfo.u32Height))
    {
        b_Ret = false;
        goto end_of_func;
    }
    //=============================assign value for golbal color table==================================
#if 0
    if (Media_Info.FileMMImageGIFDetailInfo.has_global_colortable)
    {
        unsigned char rgb[GIF_HEADER_SIZE] = {0};
        Media_Info.FileMMImageGIFDetailInfo.global_colortable_size = 0;
        while (Media_Info.FileMMImageGIFDetailInfo.global_colortable_size < Media_Info.FileMMImageGIFDetailInfo.global_bit_pixel)
        {
            memcpy(rgb, &pBuff[u32Len], GIF_HEADER_SIZE);
            u32Len += 3;

            Media_Info.FileMMImageGIFDetailInfo.global_color_table[0][Media_Info.FileMMImageGIFDetailInfo.global_colortable_size] = rgb[0];
            Media_Info.FileMMImageGIFDetailInfo.global_color_table[1][Media_Info.FileMMImageGIFDetailInfo.global_colortable_size] = rgb[1];
            Media_Info.FileMMImageGIFDetailInfo.global_color_table[2][Media_Info.FileMMImageGIFDetailInfo.global_colortable_size] = rgb[2];
            if (Media_Info.FileMMImageGIFDetailInfo.global_colortable_size == Media_Info.FileMMImageGIFDetailInfo.background_index)
            {
                Media_Info.FileMMImageGIFDetailInfo.animation->bg_red = rgb[0];
                Media_Info.FileMMImageGIFDetailInfo.animation->bg_green = rgb[1];
                Media_Info.FileMMImageGIFDetailInfo.animation->bg_blue = rgb[2];
            }
            Media_Info.FileMMImageGIFDetailInfo.global_colortable_size++;
        }
    }
#endif
    //=============================setup output parameters==================================
    if(Media_Info.FileMMImageGIFDetailInfo.stop_after_first_frame)
    {
        Media_Info.FileMMImageGIFDetailInfo.eOutputFormat = E_GIF_ARGB8888;
        Media_Info.FileMMImageGIFDetailInfo.u8OutputPixelSize = 4;     // 4 bytes
    }
    else
    {
        Media_Info.FileMMImageGIFDetailInfo.eOutputFormat = E_GIF_ARGB1555;
        Media_Info.FileMMImageGIFDetailInfo.u8OutputPixelSize = 2;     // 2 bytes
    }


end_of_func:

    if (Media_Info.FileMMImageGIFDetailInfo.animation != NULL)
    {
        signed long i;
        for(i = 0; i < Media_Info.FileMMImageGIFDetailInfo.animation->n_frames; i++)
        {
            if(Media_Info.FileMMImageGIFDetailInfo.animation->frames[i]->pixbuf->pixels)
            {
                FileFormatParserFreeBuff(Media_Info.FileMMImageGIFDetailInfo.animation->frames[i]->pixbuf);
                FileFormatParserFreeBuff(Media_Info.FileMMImageGIFDetailInfo.animation->frames[i]);
            }
        }
        FileFormatParserFreeBuff(Media_Info.FileMMImageGIFDetailInfo.animation);
        Media_Info.FileMMImageGIFDetailInfo.animation = NULL;
    }
    FileFormatParserFreeBuff(pBuff);
    b_Ret = true;
    return b_Ret;
}

//-------------------------------------------------------------------------------------------------
// Parse ID3v1 tag
//-------------------------------------------------------------------------------------------------
MS_BOOL File_Detail_Info_MP3_Parse_ID3v1(MS_U32 u32FileBuffAddr, MP3_INFO *pMp3InfoPtr)
{
    MS_S8 *pFileBufPtr = NULL;

    pFileBufPtr = (MS_S8*)u32FileBuffAddr;

	unsigned char ID3_NUM[3] = {0x54, 0x41, 0x47};//TAG
	if(memcmp(pFileBufPtr, &ID3_NUM[0], 3))
	{
		AUDIO_ERR(("\nNo ID3v1 tag\n"));
		return false;
	}

    pMp3InfoPtr->title[0] = 0;  // indicate ASCII
    pMp3InfoPtr->title[31] = 0;
    pMp3InfoPtr->u8TitleLength = MP3_TAG_ID3v1_LENGTH;
    memcpy(pMp3InfoPtr->title, pFileBufPtr+3, 30);

    pMp3InfoPtr->artist[0] = 0;  // indicate ASCII
    pMp3InfoPtr->artist[31] = 0;
    pMp3InfoPtr->u8ArtistLength = MP3_TAG_ID3v1_LENGTH;
    memcpy(pMp3InfoPtr->artist, pFileBufPtr+33, 30);

    pMp3InfoPtr->album[0] = 0;  // indicate ASCII
    pMp3InfoPtr->album[31] = 0;
    pMp3InfoPtr->u8AlbumLength = MP3_TAG_ID3v1_LENGTH;
    memcpy(pMp3InfoPtr->album, pFileBufPtr+63, 30);

    pMp3InfoPtr->year[0] = 0;  // indicate ASCII
    pMp3InfoPtr->year[5] = 0;
    memcpy(pMp3InfoPtr->year, pFileBufPtr+93, 4);

    pMp3InfoPtr->comment[0] = 0;  // indicate ASCII
    pMp3InfoPtr->comment[31] = 0;
    memcpy(pMp3InfoPtr->comment, pFileBufPtr+97, 30);

    pMp3InfoPtr->genre = pFileBufPtr[127];

    return true;
}

//-------------------------------------------------------------------------------------------------
// Parse ID3v2 tag
//-------------------------------------------------------------------------------------------------
MS_U32 File_Detail_Info_MP3_Parse_ID3v2(MS_U32 u32FileBuffAddr, MS_U32 u32BuffSize, MP3_INFO *pMp3InfoPtr, MS_BOOL bGetMp3OffsetOnly, FILE *fp1)
{
	MS_U32 currentIdx = 0;
	MS_U32 u32FrameId = 0, u32FrameSize = 0, u32TagSize = 0, u32TagLoopSize = 0;
	MS_U8 *pDstPtr = NULL, *pFileBufPtr = NULL, u8ID3Ver = 0, u8CopyLen = 0;

	pFileBufPtr = (MS_U8*)u32FileBuffAddr;

	unsigned char ID3_NUM[3] = {0x49, 0x44, 0x33};//ID3
	if(memcmp(pFileBufPtr, &ID3_NUM[0], 3))
	{
		AUDIO_ERR(("\nNo ID3v2 tag\n"));
		return false;
	}

	u8ID3Ver = pFileBufPtr[3];

	u32TagSize = (MS_U32) (pFileBufPtr[6] & 0x7F);
	u32TagSize <<= 7;
	u32TagSize |= (MS_U32) (pFileBufPtr[7] & 0x7F);
	u32TagSize <<= 7;
	u32TagSize |= (MS_U32) (pFileBufPtr[8] & 0x7F);
	u32TagSize <<= 7;
	u32TagSize |= (MS_U32) (pFileBufPtr[9] & 0x7F);

	AUDIO_DBG(("ID3v2 tag size = 0x%x, buffer size = %d\n", u32TagSize, u32BuffSize));

	if (bGetMp3OffsetOnly == TRUE)
	{
		return (u32TagSize + 10);
	}
	currentIdx = 0x0A;
	u32TagLoopSize = u32TagSize + 10;
	while (currentIdx < u32TagLoopSize)
	{
		u32FrameId = (pFileBufPtr[currentIdx	] << 24) +
					 (pFileBufPtr[currentIdx + 1] << 16) +
					 (pFileBufPtr[currentIdx + 2] <<  8) +
					 (pFileBufPtr[currentIdx + 3]);

		u32FrameSize = (pFileBufPtr[currentIdx + 4] << 24) +
					   (pFileBufPtr[currentIdx + 5] << 16) +
					   (pFileBufPtr[currentIdx + 6] <<	8) +
					   (pFileBufPtr[currentIdx + 7]);

		if (u32FrameId == 0x00000000)
		{
			AUDIO_DBG(("Zero frame id. currentIdx = %d\n", currentIdx));
			break;
		}

		u8CopyLen = 0;
		//AUDIO_DBG(("u32FrameId=0x%x\n",u32FrameId));

		switch (u32FrameId)
		{
		case MP3_TAG_ID3v2_COMM://Comments
			pDstPtr = pMp3InfoPtr->comment;
			if (u32FrameSize > (sizeof(pMp3InfoPtr->comment)-1))
				u8CopyLen = sizeof(pMp3InfoPtr->comment)-1;
			else
				u8CopyLen = u32FrameSize;
			memset(pDstPtr, 0, sizeof(pMp3InfoPtr->comment));
			break;
		case MP3_TAG_ID3v2_TALB://album/movie/show title
			pDstPtr = pMp3InfoPtr->album;
			if (u32FrameSize > (sizeof(pMp3InfoPtr->album)-1))
				u8CopyLen = sizeof(pMp3InfoPtr->album)-1;
			else
				u8CopyLen = u32FrameSize;
			memset(pDstPtr, 0, sizeof(pMp3InfoPtr->album));
			break;
		case MP3_TAG_ID3v2_TPE1://lead performer/soloist
			pDstPtr = pMp3InfoPtr->artist;
			if (u32FrameSize > (sizeof(pMp3InfoPtr->artist)-1))
				u8CopyLen = sizeof(pMp3InfoPtr->artist)-1;
			else
				u8CopyLen = u32FrameSize;
			memset(pDstPtr, 0, sizeof(pMp3InfoPtr->artist));
			break;
		case MP3_TAG_ID3v2_TIT2://title/songname/content description
			pDstPtr = pMp3InfoPtr->title;
			if (u32FrameSize > (sizeof(pMp3InfoPtr->title)-1))
				u8CopyLen = sizeof(pMp3InfoPtr->title)-1;
			else
				u8CopyLen = u32FrameSize;
			memset(pDstPtr, 0, sizeof(pMp3InfoPtr->title));
			break;
		case MP3_TAG_ID3v2_TYER://year
			pDstPtr = pMp3InfoPtr->year;
			if (u32FrameSize > (sizeof(pMp3InfoPtr->year)-1))
				u8CopyLen = sizeof(pMp3InfoPtr->year)-1;
			else
				u8CopyLen = u32FrameSize;
			memset(pDstPtr, 0, sizeof(pMp3InfoPtr->year));
			break;
		case MP3_TAG_ID3v2_APIC://attached picture
		{
			MS_U8 strJpeg[] = "image/jpeg";
			MS_U8 strJpg[]  = "image/jpg";
			MS_U8 strPng[]  = "image/png";
			u8CopyLen=0;
			AUDIO_DBG(("pFileBufPtr = %s\n", (const char *)(pFileBufPtr + currentIdx + 10 + 1)));

			if(!(strcmp((const char *)(pFileBufPtr + currentIdx + 10 + 1), (const char *)strJpeg))
			|| !(strcmp((const char *)(pFileBufPtr + currentIdx + 10 + 1), (const char *)strJpg))
			|| !(strcmp((const char *)(pFileBufPtr + currentIdx + 10 + 1), (const char *)strPng)))
			{
				MS_U32 offset = 0;
				offset=currentIdx+11;
				AUDIO_DBG(("found image tag\n"));
				AUDIO_DBG(("offset=0x%x\n", offset));
				if(!(strcmp((const char *)(pFileBufPtr + currentIdx + 10 + 1), (const char *)strPng)))
				{
					pMp3InfoPtr->imageJpg = false;
				}
				else
				{
					pMp3InfoPtr->imageJpg = true;
				}
				//MINE type
				do
				{
					offset++;
				}while(*((MS_U8*)pFileBufPtr+offset)!=0);
				AUDIO_DBG(("MINE type : 0x%x\n", pFileBufPtr[offset]));
				offset++;

				//Picture type
				pMp3InfoPtr->imageType=pFileBufPtr[offset];
				AUDIO_DBG(("Picture type : 0x%x\n", pFileBufPtr[offset]));
				offset++;

				//Description
				AUDIO_DBG(("Description: "));
				do
				{
					AUDIO_DBG(("%c", pFileBufPtr[offset]));
					offset++;
				}while(*((MS_U8*)pFileBufPtr+offset)!=0);
				AUDIO_DBG(("\n"));

				//picture data
				offset++;

				//AUDIO_DBG(("offset=0x%x 0x%x %d\n",offset, u32FrameSize, currentIdx));
				pMp3InfoPtr->imageOffset=offset;
				pMp3InfoPtr->imageSize=u32FrameSize-(offset-currentIdx-10);
				AUDIO_DBG(("imageOffset=0x%x imageSize=0x%x\n",pMp3InfoPtr->imageOffset, pMp3InfoPtr->imageSize));
			}
			break;
		}
		default:
			break;
		}

		if(u8CopyLen != 0)
		{
			memcpy(pDstPtr, pFileBufPtr + currentIdx + 10 + 1, u8CopyLen-1);
		}
		currentIdx += (u32FrameSize + 10);
		#if 1
		if(currentIdx >= u32BuffSize)
		{
			while(currentIdx >= u32BuffSize)
			{
				if(fread((void *)u32gExifBuffAddr, sizeof(char), u32BuffSize, fp1) != u32BuffSize)
				{
					currentIdx -= u32BuffSize;
					u32TagLoopSize -= u32BuffSize;
					break;
				}
				currentIdx -= u32BuffSize;
				u32TagLoopSize -= u32BuffSize;
			}
		}
		#else
		if(currentIdx >= u32BuffSize)
		{
			AUDIO_DBG(("currentIdx %d exceeds u32BufferSize %d!\n", currentIdx, u32BuffSize));
			break;
		}
		#endif
	}


	AUDIO_DBG(("\nTitle: %s", pMp3InfoPtr->title));
	AUDIO_DBG(("\nArtist: %s", pMp3InfoPtr->artist));
	AUDIO_DBG(("\nAlbum: %s", pMp3InfoPtr->album));
	AUDIO_DBG(("\nyear: %s", pMp3InfoPtr->year));
	AUDIO_DBG(("\nComment: %s", pMp3InfoPtr->comment));


	return (u32TagSize + 10);
}


static MS_U16 Mp3BitRateTable[5][16] =
{
    {0xffff, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, MP3_INVALID_RATE}, // V1, L1
    {0xffff, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, MP3_INVALID_RATE},    // V1, L2
    {0xffff, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, MP3_INVALID_RATE},     // V1, L3
    {0xffff, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, MP3_INVALID_RATE},    // V2/V2.5, L1
    {0xffff, 8,  16, 24, 32, 40, 48, 56,  64,  80,  96, 112, 128, 144, 160, MP3_INVALID_RATE},     // V2/V2.5, L2/L3
};

static MS_U16 Mp3SampleRateTable[3][4] =
{
    {11025, 12000,  8000, MP3_INVALID_RATE}, // v2.5
    {22050, 24000, 16000, MP3_INVALID_RATE}, // v2
    {44100, 48000, 32000, MP3_INVALID_RATE}, // v1
};

//-------------------------------------------------------------------------------------------------
// Get basic MP3 info - version, layer, bit rate, sample rate, vbr info, etc.
//-------------------------------------------------------------------------------------------------
MS_U16 File_Detail_Info_MP3_Parse_GetFileTime(
        MS_U32 u32FileBuffAddr,
        MS_U32 u32FileSize,
        EN_MP3_VERSION *penVersion,
        EN_MP3_LAYER *penLayer,
        MS_U16 *pu16BitRate,
        MS_U16 *pu16SampleRate,
        MS_U8 *pbFlag,
        MS_U16 *pu16TOCLength,
        MS_U8 *pu8TOC)
{
    MS_U8 *pFileBufPtr;
    MS_U32 u32Tag, u32BitRate, u32FrameCnt = 0;
    MS_U16 u16SampleRate;
    MS_U8  u8Version, u8Layer, u8Offset;
    MS_U16 u16Mp3HeaderOffset;
    MS_U8  u8PaddingFlag;
    MS_U16 u16Mp3FileTime;
    MS_U32 u32FrameSize;

    *pbFlag = 0;
    *pu16TOCLength = 0 ;

    pFileBufPtr = (MS_U8*)u32FileBuffAddr;
    //pu16FileBufPtr = (MS_U16*)u32FileBuffAddr;

    // Seek the file to find the MP3 header, max seek 4096 bytes
    u16Mp3HeaderOffset = 0;

Mp3_Get_FirstFrame:
    while(!(pFileBufPtr[u16Mp3HeaderOffset] == 0xff
    	&&(pFileBufPtr[u16Mp3HeaderOffset+1] & 0xe0) == 0xe0))
    {
        u16Mp3HeaderOffset++;
    }
	AUDIO_DBG(("pFileBufPtr = 0x%x 0x%x\n", pFileBufPtr[u16Mp3HeaderOffset], pFileBufPtr[u16Mp3HeaderOffset+1]));

    if (!(pFileBufPtr[u16Mp3HeaderOffset] == 0xff && (pFileBufPtr[u16Mp3HeaderOffset+1] & 0xe0) == 0xe0))
    {
        AUDIO_ERR(("Not MP3 file\n"));
        return 0;   // not MP3 file
    }

    u8Version = (pFileBufPtr[u16Mp3HeaderOffset+1] >> 0x3) & 0x3;
    u8Layer = (pFileBufPtr[u16Mp3HeaderOffset+1] >> 0x1) & 0x3;
    *penVersion= (EN_MP3_VERSION)u8Version;
    *penLayer = (EN_MP3_LAYER)u8Layer;
	AUDIO_DBG(("u8Version = %d u8Layer = %d\n", u8Version, u8Layer));

    u8PaddingFlag = (pFileBufPtr[u16Mp3HeaderOffset+2] & 0x02) >> 1;
    u16SampleRate = (pFileBufPtr[u16Mp3HeaderOffset+2] & 0x0c) >> 2;
	AUDIO_DBG(("u8PaddingFlag = %d u16SampleRate = %d\n", u8PaddingFlag, u16SampleRate));
    if (u8Version == EN_MP3_VERSION_2_5) // v2.5
    {
        u16SampleRate = Mp3SampleRateTable[0][u16SampleRate];
    }
    else if (u8Version == EN_MP3_VERSION_2) // v2
    {
        u16SampleRate = Mp3SampleRateTable[1][u16SampleRate];
    }
    else if (u8Version == EN_MP3_VERSION_1) // v1
    {
        u16SampleRate = Mp3SampleRateTable[2][u16SampleRate];
    }
    else
    {
        u16SampleRate = 0xffff;
    }
    *pu16SampleRate = u16SampleRate;
	AUDIO_DBG(("u16SampleRate = %d\n", u16SampleRate));

    u8Offset = 36;  // mpeg1, channel != mono
    u32Tag = 0;
    u32Tag =  pFileBufPtr[u16Mp3HeaderOffset + u8Offset    ] << 24;
    u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 1] << 16;
    u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 2] << 8;
    u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 3];
    //u32Tag = *(MS_U32 *)(pFileBufPtr + u16Mp3HeaderOffset + u8Offset);

	AUDIO_DBG(("u32Tag = %d\n", u32Tag));
    if (u32Tag == MP3_TAG_XING)
    {
        *pbFlag |= MP3_FLAG_VBR_XING;
    }
    else if(u32Tag == MP3_TAG_VBRI)
    {
        *pbFlag |= MP3_FLAG_VBR_VBRI;
    }
    else
    {
        u8Offset = 21;  // mpeg1, channel == mono or mpeg2, channel != mono
        u32Tag = 0;
        u32Tag =  pFileBufPtr[u16Mp3HeaderOffset + u8Offset    ] << 24;
        u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 1] << 16;
        u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 2] <<  8;
        u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 3];
        if (u32Tag == MP3_TAG_XING)
        {
            *pbFlag |= MP3_FLAG_VBR_XING;
        }
        else
        {
            u8Offset = 13;  // mpeg1, channel == monoo
            u32Tag = 0;
            u32Tag =  pFileBufPtr[u16Mp3HeaderOffset + u8Offset    ] << 24;
            u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 1] << 16;
            u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 2] <<  8;
            u32Tag |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 3];
            if (u32Tag == MP3_TAG_XING)
            {
                *pbFlag |= MP3_FLAG_VBR_XING;
            }
        }
    }
	AUDIO_DBG(("*pbFlag = %d\n", *pbFlag));
    if(*pbFlag & MP3_FLAG_VBR_XING)               // VBR_Xing
    {
        if (pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 7] & XING_FRAMES)
        {
            //u32FrameCnt = *(MS_U32 *) (pFileBufPtr + u16Mp3HeaderOffset + u8Offset + 8);
            u32FrameCnt = 0;
            u32FrameCnt =  pFileBufPtr[u16Mp3HeaderOffset + u8Offset +  8] << 24;
            u32FrameCnt |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset +  9] << 16;
            u32FrameCnt |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 10] <<  8;
            u32FrameCnt |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 11];
        }

        if ((pFileBufPtr[u16Mp3HeaderOffset+1] & 0x08) == 0x08)
        {
            u32FrameCnt *= MP3_SAMPLE_PER_FRAME;
        }
        else
            u32FrameCnt *= MP3_SAMPLE_PER_FRAME_LSF;

        u32FrameCnt /= u16SampleRate;

        u32FileSize /= 1000;
        if(u32FrameCnt != 0)
        {
            *pu16BitRate = (MS_U16)(u32FileSize / u32FrameCnt);
            *pu16BitRate *= 8;
        }
        else
        {
            *pu16BitRate = MP3_INVALID_RATE;
        }

        if ((pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 7] & XING_TOC) && (pu8TOC != NULL))
        {
            *pu16TOCLength=100;
            memcpy(pu8TOC, pFileBufPtr + u16Mp3HeaderOffset + u8Offset + 16, 100);
        }

        u16Mp3FileTime = (MS_U16)u32FrameCnt;
    }
    else if(*pbFlag & MP3_FLAG_VBR_VBRI)   //VBR_VBRI
    {
        //u32FrameCnt = *(MS_U32 *) (pFileBufPtr + u16Mp3HeaderOffset + u8Offset + 14);
        u32FrameCnt = 0;
        u32FrameCnt =  pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 14] << 24;
        u32FrameCnt |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 15] << 16;
        u32FrameCnt |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 16] <<  8;
        u32FrameCnt |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 17];

        if ((pFileBufPtr[u16Mp3HeaderOffset+1] & 0x08) == 0x08)
        {
            u32FrameCnt *= MP3_SAMPLE_PER_FRAME;
        }
        else
            u32FrameCnt *= MP3_SAMPLE_PER_FRAME_LSF;

        u32FrameCnt /= u16SampleRate;

        u32FileSize /= 1000;
        if(u32FrameCnt != 0)
        {
            *pu16BitRate = (MS_U16)(u32FileSize / u32FrameCnt);
            *pu16BitRate *= 8;
        }
        else
        {
            *pu16BitRate = MP3_INVALID_RATE;
        }

        //*pu16TOCLength = *(MS_U16 *)(pFileBufPtr + u16Mp3HeaderOffset + u8Offset + 18);
        *pu16TOCLength = 0;
        *pu16TOCLength =  pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 18] << 8;
        *pu16TOCLength |= pFileBufPtr[u16Mp3HeaderOffset + u8Offset + 19];

        if(*pu16TOCLength>0 && *pu16TOCLength<100)
        {
           memcpy(pu8TOC, pFileBufPtr + u16Mp3HeaderOffset + u8Offset + 26, *pu16TOCLength);
        }

        u16Mp3FileTime =  (MS_U16)u32FrameCnt;
    }
    else  //CBR
    {
        if ((u8Version == 0x3) && (u8Layer == 0x3)) // V1 L1
        {
            u32BitRate = (MS_U32) Mp3BitRateTable[0][pFileBufPtr[u16Mp3HeaderOffset+2] >> 4];
        }
        else if ((u8Version == 0x3) && (u8Layer == 0x2)) // V1 L2
        {
            u32BitRate = (MS_U32) Mp3BitRateTable[1][pFileBufPtr[u16Mp3HeaderOffset+2] >> 4];
        }
        else if ((u8Version == 0x3) && (u8Layer == 0x1)) // V1 L3
        {
            u32BitRate = (MS_U32) Mp3BitRateTable[2][pFileBufPtr[u16Mp3HeaderOffset+2] >> 4];
        }
        //fix MP3 wrong bitrate&total time issue while playing V2/V2.5 stream, share same table for V2&V2.5
        else if (((u8Version == 0x2)||(u8Version == 0x00)) && (u8Layer == 0x3)) // V2/V2.5 L1
        {
            u32BitRate = (MS_U32) Mp3BitRateTable[3][pFileBufPtr[u16Mp3HeaderOffset+2] >> 4];
        }
        else if (((u8Version == 0x2)||(u8Version == 0x00)) && ((u8Layer == 0x2)||(u8Layer == 0x1))) // V2/V2.5 L2/L3
        {
            u32BitRate = (MS_U32) Mp3BitRateTable[4][pFileBufPtr[u16Mp3HeaderOffset+2] >> 4];
        }
        else
        {
            u32BitRate = 0xffff;
        }

        if ((u16SampleRate == 0xffff) || (u32BitRate == 0xffff))
        {
            u32FrameSize = 2;
        }
        else
        {
            if (u8Version == EN_MP3_VERSION_1)
                u32FrameSize = 144*u32BitRate*1000/u16SampleRate+u8PaddingFlag;
            else
                u32FrameSize = 72*u32BitRate*1000/u16SampleRate+u8PaddingFlag;
        }
        u16Mp3HeaderOffset += u32FrameSize;
        if (!(pFileBufPtr[u16Mp3HeaderOffset] == 0xff && (pFileBufPtr[u16Mp3HeaderOffset+1] & 0xe0) == 0xe0))
        {
            AUDIO_DBG(("Not a valid mp3 frame, Seek next!\n"));
            u16Mp3HeaderOffset -= (u32FrameSize - 1);
            goto Mp3_Get_FirstFrame;
        }

        *pu16BitRate = (MS_U16)(u32BitRate);
        u32BitRate *= 1000;
        u16Mp3FileTime =  (MS_U16)(u32FileSize / (u32BitRate >> 3));

		AUDIO_DBG(("u32BitRate = %d, u16Mp3FileTime = %d\n", u32BitRate, u16Mp3FileTime));
    }

    return u16Mp3FileTime;
}

//======================================================================//
// bool File_Detail_Info_MP3_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_MP3_Parser(FILE *fp1)
{
    bool b_Ret = false;
	MS_U32 u32Offset = 0;
	MS_U32 u32Position = 0;
	MP3_INFO MP3DetailInfo;

    u32gExifBuffAddr = (unsigned int)FileFormatParserMallocBuff(MP3_INFO_LEN + 1);
	Media_Info.FileMMAudioMP3DetailInfo.bCheckFfFb = true;
    // ================get MP3 ID3 v1 information==============================
	AUDIO_DBG(("==============MP3 ID3 v1=============\n"));
	Media_Info.FileMMAudioMP3DetailInfo.bID3TagV1 = true;
    Media_Info.FileMMAudioMP3DetailInfo.u16ID3TagV1Size = MP3_ID3_V1_LEN;
    Media_Info.FileMMAudioMP3DetailInfo.u32FileSize = Media_Info.FileSize;
	u32Position = Media_Info.FileSize - MP3_ID3_V1_LEN;
	AUDIO_DBG(("u32Position = 0x%x\n", u32Position));
	fseek(fp1, u32Position, SEEK_SET);
	fread((void *)u32gExifBuffAddr, sizeof(char), MP3_ID3_V1_LEN, fp1);
	memset((void *)&MP3DetailInfo, 0x00, sizeof(MP3_INFO));
	File_Detail_Info_MP3_Parse_ID3v1(u32gExifBuffAddr, &MP3DetailInfo);
	memcpy(&Media_Info.FileMMAudioMP3DetailInfo.stID3v1, &MP3DetailInfo, sizeof(MP3_INFO));

    // ================get MP3 ID3 v2 information==============================
	AUDIO_DBG(("==============MP3 ID3 v2=============\n"));
	fseek(fp1, 0, SEEK_SET);
	fread((void *)u32gExifBuffAddr, sizeof(char), MP3_INFO_LEN, fp1);
	u32Offset = File_Detail_Info_MP3_Parse_ID3v2(u32gExifBuffAddr, MP3_INFO_LEN, &MP3DetailInfo, false, fp1);
	memcpy(&Media_Info.FileMMAudioMP3DetailInfo.stID3v2, &MP3DetailInfo, sizeof(MP3_INFO));
	AUDIO_DBG(("u32Offset=0x%x\n", u32Offset));
	if (u32Offset != 0)
	{
		Media_Info.FileMMAudioMP3DetailInfo.bID3TagV2 = true;
		fseek(fp1, u32Offset, SEEK_SET);
		fread((void *)u32gExifBuffAddr, sizeof(char), MP3_INFO_LEN, fp1);
		memset((void *)&MP3DetailInfo, 0x00, sizeof(MP3_INFO));
		u32Offset += File_Detail_Info_MP3_Parse_ID3v2(u32gExifBuffAddr, MP3_INFO_LEN, &MP3DetailInfo, true, fp1);
	}
	Media_Info.FileMMAudioMP3DetailInfo.u32ID3TagV2Size = u32Offset;

    // ================get MP3 layer, bitrate, etc.==============================
	AUDIO_DBG(("==============MP3 layer, bitrate, etc.=============\n"));
	AUDIO_DBG(("u32Offset=0x%x\n", u32Offset));
	fseek(fp1, u32Offset, SEEK_SET);
	fread((void *)u32gExifBuffAddr, sizeof(char), MP3_INFO_LEN, fp1);
	Media_Info.FileMMAudioMP3DetailInfo.u32Duration = File_Detail_Info_MP3_Parse_GetFileTime(
								u32gExifBuffAddr,
								Media_Info.FileMMAudioMP3DetailInfo.u32FileSize,
								&Media_Info.FileMMAudioMP3DetailInfo.enVersion,
								&Media_Info.FileMMAudioMP3DetailInfo.enLayer,
								&Media_Info.FileMMAudioMP3DetailInfo.u16BitRate,
								&Media_Info.FileMMAudioMP3DetailInfo.u16SampleRate,
								&Media_Info.FileMMAudioMP3DetailInfo.u8Flag,
								&Media_Info.FileMMAudioMP3DetailInfo.u16TOCLength,
								&Media_Info.FileMMAudioMP3DetailInfo.u8TOC[0]);

    b_Ret = true;
    return b_Ret;
}



//======================================================================//
// bool File_Detail_Info_WMA_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_WMA_Parser(FILE *fp1)
{
	bool b_Ret = false;
	MS_U32 u32Offset = 0;
	MS_U32 u32Position = 0;
	MP3_INFO MP3DetailInfo;

    u32gExifBuffAddr = (unsigned int)FileFormatParserMallocBuff(MP3_INFO_LEN + 1);
	fseek(fp1, u32Position, SEEK_SET);
	fread((void *)u32gExifBuffAddr, sizeof(char), WMA_HEADER_SIZE, fp1);
	unsigned char ID3_NUM[16] = {0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, \
								  0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0X6C};//ID3
	if(memcmp(&u32gExifBuffAddr[0], &ID3_NUM[0], 16))
	{
		AUDIO_ERR(("\nNo ID3v2 tag\n"));
		return false;
	}
	else
	{
		u32Offset = 16;
		MS_U32 temp_len = ((((U32)u32gExifBuffAddr[u32Offset]) & 0x000000FF)        |
				           (((U32)u32gExifBuffAddr[u32Offset+1] << 8) & 0x0000FF00)  |
				           (((U32)u32gExifBuffAddr[u32Offset+2] << 16) & 0x00FF0000) |
				           (((U32)u32gExifBuffAddr[u32Offset+3] << 24) & 0xFF000000))
		          			+ 50;
		Media_Info.FileMMAudioWMADetailInfo.u32HeaderSize = temp_len;
		MS_BOOL bStreamFound = FALSE;
	    MS_BOOL bContentFound = FALSE;
	    MS_BOOL bFilePropertiesFound = FALSE;
	    MS_U32 u32TempHeaderSize = 0;
	    u32Offset += 4;
	    while (temp_len > u32TempHeaderSize)
	    {
	        u32TempHeaderSize += WMA_HEADER_SIZE;

	        if (u32TempHeaderSize > WMA_HEADER_SIZE)
	        {
	        	fread((void *)u32gExifBuffAddr, sizeof(char), WMA_HEADER_SIZE, fp1);
	        }

	        if (!bStreamFound)
	        {
				ID3_NUM[16] = {0x91, 0x07, 0xDC, 0xB7, 0xB7, 0xA9, 0xCF, 0x11, \
						  		0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0X65};
				while(u32Offset < WMA_HEADER_SIZE)
				{
					if (memcmp(&u32gExifBuffAddr[u32Offset], &ID3_NUM[0], 16))
					{
						bStreamFound = TRUE;
						Media_Info.FileMMAudioWMADetailInfo.u16SampleRate =
						memcpy(&Media_Info.FileMMAudioWMADetailInfo.u16SampleRate, &u32gExifBuffAddr[u32Offset+66], 2);
						memcpy(&Media_Info.FileMMAudioWMADetailInfo.u32BitRate, &u32gExifBuffAddr[u32Offset+70], 4);
						Media_Info.FileMMAudioWMADetailInfo.u32BitRate *= 8;
						AUDIO_DBG("pWmaInfo->u16SampleRate = %u\n", Media_Info.FileMMAudioWMADetailInfo.u16SampleRate);
						AUDIO_DBG("pWmaInfo->u32BitRate = %lu\n", Media_Info.FileMMAudioWMADetailInfo.u32BitRate);
						break;
					}
					u32Offset ++;
				}
			}

	        if (!bContentFound)
	            for (i = 16;i < 4096;i++)
	            {
	                if ((pu8Data[i-16] == 0x33) && (pu8Data[i-15] == 0x26) && (pu8Data[i-14] == 0xB2) && (pu8Data[i-13] == 0x75) &&
	                        (pu8Data[i-12] == 0x8E) && (pu8Data[i-11] == 0x66) && (pu8Data[i-10] == 0xCF) && (pu8Data[i-9] == 0x11) &&
	                        (pu8Data[i-8] == 0xA6) && (pu8Data[i-7] == 0xD9) && (pu8Data[i-6] == 0x00) && (pu8Data[i-5] == 0xAA) &&
	                        (pu8Data[i-4] == 0x00) && (pu8Data[i-3] == 0x62) && (pu8Data[i-2] == 0xCE) && (pu8Data[i-1] == 0x6C)
	                   )
	                {
	                    bContentFound = TRUE;
	                    u16TitleLength = pu8Data[i+8] + (pu8Data[i+9] << 8);
	                    u16AuthorLength = pu8Data[i+10] + (pu8Data[i+11] << 8);
	                    for (j = 0;j < 32;j = j + 2)
	                    {
	                        pWmaInfo->u8Title[j] = pu8Data[i+j+19];
	                        pWmaInfo->u8Title[j+1] = pu8Data[i+j+18];
	                    }
	                    for (j = 0;j < 16;j = j + 2)
	                    {
	                        pWmaInfo->u8Author[j] = pu8Data[i+j+19+u16TitleLength];
	                        pWmaInfo->u8Author[j+1] = pu8Data[i+j+18+u16TitleLength];
	                    }
	                    //WMA_DBG(printf("pWmaInfo->u8Title = %s\n",pWmaInfo->u8Title));
	                    break;
	                }
	            }

	        if (!bFilePropertiesFound)
	            for (i = 16;i < 4096;i++)
	            {
	                if ((pu8Data[i-16] == 0xA1) && (pu8Data[i-15] == 0xDC) && (pu8Data[i-14] == 0xAB) && (pu8Data[i-13] == 0x8C) &&
	                        (pu8Data[i-12] == 0x47) && (pu8Data[i-11] == 0xA9) && (pu8Data[i-10] == 0xCF) && (pu8Data[i-9] == 0x11) &&
	                        (pu8Data[i-8] == 0x8E) && (pu8Data[i-7] == 0xE4) && (pu8Data[i-6] == 0x00) && (pu8Data[i-5] == 0xC0) &&
	                        (pu8Data[i-4] == 0x0C) && (pu8Data[i-3] == 0x20) && (pu8Data[i-2] == 0x53) && (pu8Data[i-1] == 0x65)
	                   )
	                {
	                    bFilePropertiesFound = TRUE;
	                    /*
	                    pWmaInfo->u32Duration = ((U32)pu8Data[i+56]&0x000000FF)|
	                                            (((U32)pu8Data[i+57]<<8)&0x0000FF00)|
	                                            (((U32)pu8Data[i+58]<<16)&0x00FF0000)|
	                                            (((U32)pu8Data[i+59]<<24)&0xFF000000);

	                    //pWmaInfo->u32Duration = (pWmaInfo->u32Duration*0.0000001);
	                    */

	                    pWmaInfo->u32Duration = ((U32)pu8Data[i+58] & 0x000000FF) |
	                                            (((U32)pu8Data[i+59] << 8) & 0x0000FF00) |
	                                            (((U32)pu8Data[i+60] << 16) & 0x00FF0000) |
	                                            (((U32)pu8Data[i+61] << 24) & 0xFF000000);

	                    pWmaInfo->u32Duration = (pWmaInfo->u32Duration * 0.0065536);   //*(2^16 * 0.0000001)

	                    pWmaInfo->u32PacketSize = ((U32)pu8Data[i+76] & 0x000000FF)       |
	                                              (((U32)pu8Data[i+77] << 8) & 0x0000FF00)  |
	                                              (((U32)pu8Data[i+78] << 16) & 0x00FF0000) |
	                                              (((U32)pu8Data[i+79] << 24) & 0xFF000000);

	                    pWmaInfo->u32TotalPacket = ((U32)pu8Data[i+40] & 0x000000FF)       |
	                                               (((U32)pu8Data[i+41] << 8) & 0x0000FF00)  |
	                                               (((U32)pu8Data[i+42] << 16) & 0x00FF0000) |
	                                               (((U32)pu8Data[i+43] << 24) & 0xFF000000);
	                    WMA_INFO(printf("pWmaInfo->u32PacketSize = %x\n", pWmaInfo->u32PacketSize));
	                    break;
	                }
	            }
	    }
	}

    b_Ret = true;
	return b_Ret;
}

//======================================================================//
// bool File_Detail_Info_AVI_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_AVI_Parser(FILE *fp1)
{
	bool b_Ret = false;
	u32gExifBuffAddr = (unsigned int)FileFormatParserMallocBuff(AVI_DI_SIZE + 1);
	fseek(fp1, 0, SEEK_SET);
	fread((void *)u32gExifBuffAddr, sizeof(char), AVI_DI_SIZE, fp1);
	unsigned char* pBuff = (unsigned char *)u32gExifBuffAddr;
	unsigned char* pFourCC = pBuff;
	unsigned char Nstreams = 0;
	
	if(pFourCC != NULL)
	{
		//Judge whether RIFF file or not
		File_MM_Video_RIFF_LIST MMRiffList;		
		MMRiffList.u32Fcc = MApp_EndianChangeL(TO32BIT(pBuff));
		MMRiffList.u32Size = MApp_EndianChangeL(TO32BIT(pBuff + 4));
		MMRiffList.u32FccListType = MApp_EndianChangeL(TO32BIT(pBuff + 8));
		
		if (MMRiffList.u32Fcc == MKTAG('R', 'I', 'F', 'F'))
		{
			if ((MMRiffList.u32FccListType == MKTAG('A', 'V', 'I', ' '))
				|| (MMRiffList.u32FccListType == MKTAG('A', 'V', 'I', 'X')))
			{
				b_Ret = true;
			}
		}	
		else
		{
			b_Ret = false;
			return b_Ret;
		}
		//Cyclic detection
		unsigned int u32Offset = 12;
		bool b_End = false;
		do
		{	
            MOVIE_DBG(("\nu32Offset=%d\n", u32Offset));
			if(u32Offset > AVI_DI_SIZE)
			{
				break;
			}
			pFourCC = (unsigned char *)pBuff + u32Offset;
			MMRiffList.u32Fcc = MApp_EndianChangeL(TO32BIT(pFourCC));
			MMRiffList.u32Size = MApp_EndianChangeL(TO32BIT(pFourCC + 4));
            MOVIE_DBG(("\n[%d]0x%x 0x%x 0x%x \n", __LINE__, MMRiffList.u32Fcc, MMRiffList.u32Size,MMRiffList.u32FccListType));
			
			switch(MMRiffList.u32Fcc)
			{
			case MKTAG('L', 'I', 'S', 'T'):
			{
				MMRiffList.u32FccListType = MApp_EndianChangeL(TO32BIT(pFourCC + 8));
				if (MMRiffList.u32FccListType == MKTAG('m', 'o', 'v', 'i'))
	            {
		            goto end_of_func;
	            }
				if (MMRiffList.u32FccListType == MKTAG('h', 'd', 'r', 'l'))//mandatory LIST
	            {
		            u32Offset += 4;
	            }	    
				if (MMRiffList.u32FccListType == MKTAG('s', 't', 'r', 'l'))//mandatory LIST
	            {
		            u32Offset += 4;
	            }		            
	            u32Offset += 8;
				break;
			}		
			case MKTAG('a', 'v', 'i', 'h')://chunk
			{
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.MMRiffChunkGeneral.u32Fcc = MMRiffList.u32Fcc;
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.MMRiffChunkGeneral.u32Size = MMRiffList.u32Size;				
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32MicroSecPerFrame = MApp_EndianChangeL(TO32BIT(pFourCC + 8));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32MaxBytesPerSec = MApp_EndianChangeL(TO32BIT(pFourCC + 12));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32PaddingGranularity = MApp_EndianChangeL(TO32BIT(pFourCC + 16));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Flags = MApp_EndianChangeL(TO32BIT(pFourCC + 20));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32TotalFrames = MApp_EndianChangeL(TO32BIT(pFourCC + 24));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32InitialFrames = MApp_EndianChangeL(TO32BIT(pFourCC + 28));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Streams = MApp_EndianChangeL(TO32BIT(pFourCC + 32));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32SuggestedBufferSize = MApp_EndianChangeL(TO32BIT(pFourCC + 36));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Width = MApp_EndianChangeL(TO32BIT(pFourCC + 40));
				Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Height = MApp_EndianChangeL(TO32BIT(pFourCC + 44));
				
				u32Offset += (8 + MMRiffList.u32Size);
				break;
			}
			case MKTAG('s', 't', 'r', 'h')://chunk
			{ 
				Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].MMRiffChunkGeneral.u32Fcc = MMRiffList.u32Fcc;
				Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].MMRiffChunkGeneral.u32Size = MMRiffList.u32Size;
				if (MMRiffList.u32Size >= 12 * 4)
				{
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32FccType = MApp_EndianChangeL(TO32BIT(pFourCC + 8));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32FccHandler = MApp_EndianChangeL(TO32BIT(pFourCC + 12));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Flags = MApp_EndianChangeL(TO32BIT(pFourCC + 16));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u16wPriority = MApp_EndianChangeS(TO16BIT(pFourCC + 18));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u16Language = MApp_EndianChangeS(TO16BIT(pFourCC + 20));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32InitialFrames = MApp_EndianChangeL(TO32BIT(pFourCC + 24));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Scale = MApp_EndianChangeL(TO32BIT(pFourCC + 28));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Rate = MApp_EndianChangeL(TO32BIT(pFourCC + 32));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Start = MApp_EndianChangeL(TO32BIT(pFourCC + 36));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Length = MApp_EndianChangeL(TO32BIT(pFourCC + 40));				
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32SuggestedBufferSize = MApp_EndianChangeL(TO32BIT(pFourCC + 44));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Quality = MApp_EndianChangeL(TO32BIT(pFourCC + 48));
					Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32SampleSize = MApp_EndianChangeL(TO32BIT(pFourCC + 52));					
				
					if (MMRiffList.u32Size > 12 * 4)
					{
						Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].stRcFrame.u16Left = MApp_EndianChangeS(TO16BIT(pFourCC + 56));
						Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].stRcFrame.u16Top	= MApp_EndianChangeS(TO16BIT(pFourCC + 58));
						Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].stRcFrame.u16Right = MApp_EndianChangeS(TO16BIT(pFourCC + 60));
						Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].stRcFrame.u16Bottom = MApp_EndianChangeS(TO16BIT(pFourCC + 62));
				
						if (MMRiffList.u32Size > 14 * 4)
						{
                            MOVIE_ERR(("Error: Unknown info!\n"));
						}
					}
				}
				else
				{
                    MOVIE_ERR(("Error: Unknown size(strh), expected >= 48!\n"));
				}	

				switch(Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32FccType)
				{
					case MKTAG('i', 'a', 'v', 's'):
	                case MKTAG('i', 'v', 'a', 's'):
	                {
	                break;
	                }

	                case MKTAG('v', 'i', 'd', 's'):
	                {
                
	                break;
	                }

	                case MKTAG('a', 'u', 'd', 's'):
					{
					break;
					}
					
					case MKTAG('t', 'x', 't', 's'):
					{
					break;
					} 
					
					default:
					{
					break;
					}
				}
				u32Offset += (8 + MMRiffList.u32Size);	
				break;
			}		
			
			case MKTAG('s', 't', 'r', 'f'):
			{			
			switch(Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32FccType)
			{
                case MKTAG('v', 'i', 'd', 's'):
                {
				Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.MMRiffChunkGeneral.u32Fcc = MMRiffList.u32Fcc;
				Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.MMRiffChunkGeneral.u32Size = MMRiffList.u32Size;
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Size   = MApp_EndianChangeL(TO32BIT(pFourCC + 8));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Width  = MApp_EndianChangeL(TO32BIT(pFourCC + 12));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Height = MApp_EndianChangeL(TO32BIT(pFourCC + 16));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u16Planes = MApp_EndianChangeS(TO16BIT(pFourCC + 20));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u16BitCount   = MApp_EndianChangeS(TO16BIT(pFourCC + 22));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Compression= MApp_EndianChangeL(TO32BIT(pFourCC + 24));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32SizeImage  = MApp_EndianChangeL(TO32BIT(pFourCC + 28));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32XPelsPerMeter = MApp_EndianChangeL(TO32BIT(pFourCC + 32));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32YPelsPerMeter = MApp_EndianChangeL(TO32BIT(pFourCC + 36));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32ClrUsed    = MApp_EndianChangeL(TO32BIT(pFourCC + 40));
                Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32ClrImportant = MApp_EndianChangeL(TO32BIT(pFourCC + 44));
                break;
                }

                case MKTAG('a', 'u', 'd', 's'):
				{
				Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.MMRiffChunkGeneral.u32Fcc = MMRiffList.u32Fcc;
				Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.MMRiffChunkGeneral.u32Size = MMRiffList.u32Size;		
                Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16FormatTag   = MApp_EndianChangeS(TO16BIT(pFourCC + 8));
                Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16Channels    = MApp_EndianChangeS(TO16BIT(pFourCC + 10));
                Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u32SamplesPerSec = MApp_EndianChangeL(TO32BIT(pFourCC + 12));
                Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u32AvgBytesPerSec = MApp_EndianChangeL(TO32BIT(pFourCC + 16));
                Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16BlockAlign  = MApp_EndianChangeS(TO16BIT(pFourCC + 20));
                if(14 == MMRiffList.u32Size)
				{
					Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16BitsPerSample = 8;
				}
				else
				{
					Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16BitsPerSample = MApp_EndianChangeS(TO16BIT(pFourCC + 22));
				}

				int u32ExtraDataSize = MApp_EndianChangeS(TO16BIT(pFourCC + 24));
				break;
				}			
			}
			u32Offset += (8 + MMRiffList.u32Size);			
			Nstreams++;
			break;
			}

			case MKTAG('J', 'U', 'N', 'K'):
	{
            MOVIE_ERR(("%s 0x%x\n", MKTAG2STR_0(MMRiffList.u32Fcc), MMRiffList.u32Size));

			u32Offset += (8 + MMRiffList.u32Size);
			break;
	}
			
			default:
			{				
                MOVIE_ERR(("**default %s\n", MKTAG2STR_0(MMRiffList.u32Fcc)));
				//u32Offset += MMRiffList.u32Size;
				goto end_of_func;
				break;
			}
			}
			if(b_End)
			{
	b_Ret = true;
				break;
			}
			//u32Offset += (4 + MMRiffList.u32Size);
			if (u32Offset >= AVI_DI_SIZE)
			{
				while (u32Offset >= AVI_DI_SIZE)
				{
					if (fread((void *)u32gExifBuffAddr, sizeof(char), AVI_DI_SIZE, fp1) != AVI_DI_SIZE)
					{
                        MOVIE_ERR(("\nfind fourcc:0x%x at the end of file\n",MMRiffList.u32Fcc));
						u32Offset -= AVI_DI_SIZE;
						break;
					}
					u32Offset -= AVI_DI_SIZE;
				}
			}
		}while(1);
	}
end_of_func:
    FileFormatParserFreeBuff(pBuff);
	return b_Ret;
}


//======================================================================//
// bool File_Detail_Info_Parser(FILE *fp1)
//======================================================================//
bool File_Detail_Info_Parser(FILE *fp1)
{
	bool b_Ret = false; 
	switch(Media_Info.FileMMSubTypeInfo.FileMMSubType)
	{
	/*==============image================*/
	case EN_MM_SUB_PHOTO_JPG:
		b_Ret = File_Detail_Info_JPG_Parser(fp1);
		break;
	case EN_MM_SUB_PHOTO_BMP: 
		b_Ret = File_Detail_Info_BMP_Parser(fp1);
		break;
	case EN_MM_SUB_PHOTO_PNG:
		b_Ret = File_Detail_Info_PNG_Parser(fp1);
		break;
	case EN_MM_SUB_PHOTO_TIFF:
		b_Ret = File_Detail_Info_TIFF_Parser(fp1);
		break;
	case EN_MM_SUB_PHOTO_GIF:
        b_Ret = File_Detail_Info_GIF_Parser(fp1);
        break;
    /*==============music================*/
    case EN_MM_SUB_AUDIO_MP3:
        b_Ret = File_Detail_Info_MP3_Parser(fp1);
        break;
	case EN_MM_SUB_AUDIO_WMA:
		b_Ret = File_Detail_Info_WMA_Parser(fp1);
		break;
	/*==============movie================*/
	case EN_MM_SUB_MOVIE_AVI:
		b_Ret = File_Detail_Info_AVI_Parser(fp1);
		break;		
	default:
		break;
	}
	return b_Ret;
}

//======================================================================//
// void File_Detail_Info_JPG_Print(void)
//======================================================================//
void File_Detail_Info_JPG_Print(void)
	{
    MAIN_DBG(("ImageProgressive = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ImageProgressive));
				//parser SOF0 or SOF2
				if(!Media_Info.FileMMImageJPEGDetailInfo.ImageProgressive)
				{
        MAIN_DBG(("/*==========Parser SOF0=================*/\n"));
				}
				else
				{
        MAIN_DBG(("/*==========Parser SOF2=================*/\n"));
				}
    MAIN_DBG(("Precision = %d\n", Media_Info.FileMMImageJPEGDetailInfo.Precision));
    MAIN_DBG(("ImageWidth * ImageHeight= %d * %d\n", Media_Info.FileMMImageJPEGDetailInfo.ImageWidth, Media_Info.FileMMImageJPEGDetailInfo.ImageHeight));
    MAIN_DBG(("ColorComponents = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ColorComponents));
				//parser APP1
    MAIN_DBG(("/*==========Parser APP1=================*/\n"));
    MAIN_DBG(("/*==========TIFF IFD=================*/\n"));
    MAIN_DBG(("TiffBigEndian = %d\n", Media_Info.FileMMImageJPEGDetailInfo.TiffBigEndian));
    MAIN_DBG(("Orientation = %d\n", Media_Info.FileMMImageJPEGDetailInfo.Orientation));
    MAIN_DBG(("ResolutionUnit = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ResolutionUnit));
    MAIN_DBG(("X_Resolution = %d\n", Media_Info.FileMMImageJPEGDetailInfo.X_Resolution));
    MAIN_DBG(("Y_Resolution = %d\n", Media_Info.FileMMImageJPEGDetailInfo.Y_Resolution));
    MAIN_DBG(("Software = %s\n", Media_Info.FileMMImageJPEGDetailInfo.Software));
    MAIN_DBG(("DataTime = %s\n", Media_Info.FileMMImageJPEGDetailInfo.DataTime));
    MAIN_DBG(("Title = %s\n", Media_Info.FileMMImageJPEGDetailInfo.Title));
    MAIN_DBG(("/*==========EXIF IFD=================*/\n"));
    MAIN_DBG(("Exif ColorSpace = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ExifColorSpace));
    MAIN_DBG(("Exif ImageWidth * ImageHeight= %d\n", Media_Info.FileMMImageJPEGDetailInfo.ExifImageWidth, Media_Info.FileMMImageJPEGDetailInfo.ExifImageHeight));
    MAIN_DBG(("/*==========NEXT IFD=================*/\n"));
    MAIN_DBG(("ImageThumbnail = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ImageThumbnail));
				if(Media_Info.FileMMImageJPEGDetailInfo.ImageThumbnail)
				{				
        MAIN_DBG(("   ThumbnailCompression = 0x%x\n", Media_Info.FileMMImageJPEGDetailInfo.ThumbnailCompression));
        MAIN_DBG(("   ThumbnailOffset = 0x%x\n", Media_Info.FileMMImageJPEGDetailInfo.ThumbnailOffset));
        MAIN_DBG(("   ThumbnailByteCount = 0x%x\n", Media_Info.FileMMImageJPEGDetailInfo.ThumbnailByteCount));
        MAIN_DBG(("   ThumbnailImageProgressive = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ThumbnailImageProgressive));
        MAIN_DBG(("   ThumbnailPrecision = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ThumbnailPrecision));
        MAIN_DBG(("   ThumbnailImageWidth * ThumbnailImageHeight= %d\n", Media_Info.FileMMImageJPEGDetailInfo.ThumbnailImageWidth, Media_Info.FileMMImageJPEGDetailInfo.ThumbnailImageHeight));
        MAIN_DBG(("   ThumbnailColorComponents = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ThumbnailColorComponents));
				}
				
    MAIN_DBG(("ImageResolution = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ImageResolution));
    MAIN_DBG(("ImageScalefactor = %d\n", Media_Info.FileMMImageJPEGDetailInfo.ImageScalefactor));
    return;
			}

//======================================================================//
// void File_Detail_Info_BMP_Print(void)
//======================================================================//
void File_Detail_Info_BMP_Print(void)
			{
    MAIN_DBG(("DataOffset = 0x%x\n", Media_Info.FileMMImageBMPDetailInfo.DataOffset));
    MAIN_DBG(("ImageWidth * ImageHeight= %d * %d\n", Media_Info.FileMMImageBMPDetailInfo.ImageWidth, Media_Info.FileMMImageBMPDetailInfo.ImageHeight));
    MAIN_DBG(("Precision = %d\n", Media_Info.FileMMImageBMPDetailInfo.Precision));
    MAIN_DBG(("Compression = %d\n", Media_Info.FileMMImageBMPDetailInfo.Compression));
    MAIN_DBG(("DataSize = 0x%x\n", Media_Info.FileMMImageBMPDetailInfo.DataSize));
    MAIN_DBG(("HorizontalResolution = 0x%x\n", Media_Info.FileMMImageBMPDetailInfo.HorizontalResolution));
    MAIN_DBG(("VerticalResolution = 0x%x\n", Media_Info.FileMMImageBMPDetailInfo.VerticalResolution));
    MAIN_DBG(("ColorIndexCount = 0x%x\n", Media_Info.FileMMImageBMPDetailInfo.ColorIndexCount));
    MAIN_DBG(("ImportantColorIndexCount = 0x%x\n", Media_Info.FileMMImageBMPDetailInfo.ImportantColorIndexCount));
    return;
			}

//======================================================================//
// void File_Detail_Info_PNG_Print(void)
//======================================================================//
void File_Detail_Info_PNG_Print(void)
{
    MAIN_DBG(("IHDR ImageWidth * ImageHeight= %d * %d\n", Media_Info.FileMMImagePNGDetailInfo.ImageWidth, Media_Info.FileMMImagePNGDetailInfo.ImageHeight));
    MAIN_DBG(("IHDR BitDepth = %d\n", Media_Info.FileMMImagePNGDetailInfo.BitDepth));
    MAIN_DBG(("IHDR ColorType = %d\n", Media_Info.FileMMImagePNGDetailInfo.ColorType));
    MAIN_DBG(("IHDR CompressionMethod = %d\n", Media_Info.FileMMImagePNGDetailInfo.CompressionMethod));
    MAIN_DBG(("IHDR FilterMethod = %d\n", Media_Info.FileMMImagePNGDetailInfo.FilterMethod));
    MAIN_DBG(("IHDR InterlaceMethod = %d\n", Media_Info.FileMMImagePNGDetailInfo.InterlaceMethod));
    MAIN_DBG(("pHYs PixelPerUnitXAxis = %d\n", Media_Info.FileMMImagePNGDetailInfo.PixelPerUnitXAxis));
    MAIN_DBG(("pHYs PixelPerUnitYAxis = %d\n", Media_Info.FileMMImagePNGDetailInfo.PixelPerUnitYAxis));
    MAIN_DBG(("pHYs UnitSpecifier = %d\n", Media_Info.FileMMImagePNGDetailInfo.UnitSpecifier));
    MAIN_DBG(("gAMA Gamma = %d\n", Media_Info.FileMMImagePNGDetailInfo.Gamma));
    MAIN_DBG(("cHRM WhitePX = 0x%x\n", Media_Info.FileMMImagePNGDetailInfo.WhitePX));
    MAIN_DBG(("cHRM WhitePY = 0x%x\n", Media_Info.FileMMImagePNGDetailInfo.WhitePY));
    MAIN_DBG(("cHRM RedX = 0x%x\n", Media_Info.FileMMImagePNGDetailInfo.RedX));
    MAIN_DBG(("cHRM RedY = 0x%x\n", Media_Info.FileMMImagePNGDetailInfo.RedY));
    MAIN_DBG(("cHRM GreenX = 0x%x\n", Media_Info.FileMMImagePNGDetailInfo.GreenX));
    MAIN_DBG(("cHRM GreenY = 0x%x\n", Media_Info.FileMMImagePNGDetailInfo.GreenY));
    MAIN_DBG(("cHRM BlueX = 0x%x\n", Media_Info.FileMMImagePNGDetailInfo.BlueX));
    MAIN_DBG(("cHRM BlueY = 0x%x\n", Media_Info.FileMMImagePNGDetailInfo.BlueY));
    return;
}

//======================================================================//
// void File_Detail_Info_TIFF_Print(void)
//======================================================================//
void File_Detail_Info_TIFF_Print(void)
			{
    MAIN_DBG(("IFH TiffBigEndian = %d\n", Media_Info.FileMMImageTIFFDetailInfo.TiffBigEndian));
    MAIN_DBG(("IFH VersionNum = %d\n", Media_Info.FileMMImageTIFFDetailInfo.VersionNum));
    MAIN_DBG(("IFH 1stIFDOffset = 0x%x\n", Media_Info.FileMMImageTIFFDetailInfo.FirstIFDOffset));
    MAIN_DBG(("IFD TiffBigEndian = %d\n", Media_Info.FileMMImageTIFFDetailInfo.TiffBigEndian));
    MAIN_DBG(("IFD VersionNum = %d\n", Media_Info.FileMMImageTIFFDetailInfo.VersionNum));
    MAIN_DBG(("IFD FirstIFDOffset = 0x%x\n", Media_Info.FileMMImageTIFFDetailInfo.FirstIFDOffset));
    MAIN_DBG(("IFD DirEntryNum = %d\n", Media_Info.FileMMImageTIFFDetailInfo.DirEntryNum));
    MAIN_DBG(("IFD ImageWidth * ImageHeight= %d * %d\n", Media_Info.FileMMImageTIFFDetailInfo.ImageWidth, Media_Info.FileMMImageTIFFDetailInfo.ImageLength));
    MAIN_DBG(("IFD BitsPerSample = 0x%x,%x,%x,%x\n", Media_Info.FileMMImageTIFFDetailInfo.BitsPerSample[0], \
    Media_Info.FileMMImageTIFFDetailInfo.BitsPerSample[1], Media_Info.FileMMImageTIFFDetailInfo.BitsPerSample[2], \
    Media_Info.FileMMImageTIFFDetailInfo.BitsPerSample[3]));
    MAIN_DBG(("IFD Compression = %d\n", Media_Info.FileMMImageTIFFDetailInfo.Compression));
    MAIN_DBG(("IFD PhotoMetricInterpretation = %d\n", Media_Info.FileMMImageTIFFDetailInfo.PhotoMetricInterpretation));
    MAIN_DBG(("IFD StripOffsets = %d\n", Media_Info.FileMMImageTIFFDetailInfo.StripOffsets));
    MAIN_DBG(("IFD SamplesPerPixel = %d\n", Media_Info.FileMMImageTIFFDetailInfo.SamplesPerPixel));
    MAIN_DBG(("IFD RowsPerStrip = %d\n", Media_Info.FileMMImageTIFFDetailInfo.RowsPerStrip));
    MAIN_DBG(("IFD StripByteCounts = %d\n", Media_Info.FileMMImageTIFFDetailInfo.StripByteCounts));
    MAIN_DBG(("IFD XResolution = %d\n", Media_Info.FileMMImageTIFFDetailInfo.XResolution));

    MAIN_DBG(("IFD YResolution = %d\n", Media_Info.FileMMImageTIFFDetailInfo.YResolution));
    MAIN_DBG(("IFD PlanarConfiguration = %d\n", Media_Info.FileMMImageTIFFDetailInfo.PlanarConfiguration));
    MAIN_DBG(("IFD ResolutionUnit = %d\n", Media_Info.FileMMImageTIFFDetailInfo.ResolutionUnit));
    MAIN_DBG(("IFD Predictor = %d\n", Media_Info.FileMMImageTIFFDetailInfo.Predictor));
    return;
			}


//======================================================================//
// void File_Detail_Info_GIF_Print(void)
//======================================================================//
void File_Detail_Info_GIF_Print(void)
			{
    MAIN_DBG(("[GIF] u8Header = %s\n", Media_Info.FileMMImageGIFDetailInfo.u8Header));
    MAIN_DBG(("[GIF] u8Version = %s\n", Media_Info.FileMMImageGIFDetailInfo.u8Version));
    MAIN_DBG(("[GIF] u32Width * u32Height= %d * %d\n", Media_Info.FileMMImageGIFDetailInfo.u32Width, Media_Info.FileMMImageGIFDetailInfo.u32Height));
    MAIN_DBG(("[GIF] global_bit_pixel = %d\n", Media_Info.FileMMImageGIFDetailInfo.global_bit_pixel));
    MAIN_DBG(("[GIF] global_color_resolution = %d\n", Media_Info.FileMMImageGIFDetailInfo.global_color_resolution));
    MAIN_DBG(("[GIF] has_global_colortable = %d\n", Media_Info.FileMMImageGIFDetailInfo.has_global_colortable));
    MAIN_DBG(("[GIF] background_index = %d\n", Media_Info.FileMMImageGIFDetailInfo.background_index));
    MAIN_DBG(("[GIF] aspect_ratio = %d\n", Media_Info.FileMMImageGIFDetailInfo.aspect_ratio));

    return;
}


//======================================================================//
// void File_Detail_Info_MP3_Print(void)
//======================================================================//
void File_Detail_Info_MP3_Print(void)
{
    MAIN_DBG(("==================MP3 Header Info==================\n"));
	MAIN_DBG(("[MP3] bCheckFfFb = %d\n", Media_Info.FileMMAudioMP3DetailInfo.bCheckFfFb));
    if(Media_Info.FileMMAudioMP3DetailInfo.bID3TagV1)
    {
    	MAIN_DBG(("==================[MP3] ID3v1 TAG:==================\n"));
		MAIN_DBG(("[MP3] u16ID3TagV1Size = 0x%x\n", Media_Info.FileMMAudioMP3DetailInfo.u16ID3TagV1Size));
		MAIN_DBG(("[MP3] Title = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v1.title));
		MAIN_DBG(("[MP3] Artist = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v1.artist));
		MAIN_DBG(("[MP3] Album = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v1.album));
		MAIN_DBG(("[MP3] year = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v1.year));
		MAIN_DBG(("[MP3] Comment = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v1.comment));
			}
    if(Media_Info.FileMMAudioMP3DetailInfo.bID3TagV2)
    {
		MAIN_DBG(("==================[MP3] ID3v2 TAG:==================\n"));
		MAIN_DBG(("[MP3] u32ID3TagV2Size = 0x%x\n", Media_Info.FileMMAudioMP3DetailInfo.u32ID3TagV2Size));
		MAIN_DBG(("[MP3] artist = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v2.artist));
		MAIN_DBG(("[MP3] title = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v2.title));
		MAIN_DBG(("[MP3] year = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v2.year));

		MAIN_DBG(("[MP3] Comment = %s\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v2.comment));
		if(Media_Info.FileMMAudioMP3DetailInfo.stID3v2.imageJpg)
		{
			MAIN_DBG(("[MP3] JPEG "));
		}
		else
		{
			MAIN_DBG(("[MP3] PNG "));
		}
		MAIN_DBG(("imageType = %d--", Media_Info.FileMMAudioMP3DetailInfo.stID3v2.imageType));
		switch(Media_Info.FileMMAudioMP3DetailInfo.stID3v2.imageType)
		{
			case 0x03:
				AUDIO_DBG(("Cover(front)\n"));
				break;
			case 0x04:
				AUDIO_DBG(("Cover(back)\n"));
				break;
			default:
				AUDIO_DBG(("Others\n"));
				break;
			}

		MAIN_DBG(("[MP3] imageOffset = 0x%x\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v2.imageOffset));
		MAIN_DBG(("[MP3] imageSize = 0x%x\n", Media_Info.FileMMAudioMP3DetailInfo.stID3v2.imageSize));
		}
		
    MAIN_DBG(("=======[MP3] version, layer, bit rate, sample rate, vbr info, etc:=======\n"));
	MAIN_DBG(("[MP3] enVersion = %d\n", Media_Info.FileMMAudioMP3DetailInfo.enVersion));
	MAIN_DBG(("[MP3] enLayer = %d\n", Media_Info.FileMMAudioMP3DetailInfo.enLayer));
	MAIN_DBG(("[MP3] u32FileSize = %d bytes\n", Media_Info.FileMMAudioMP3DetailInfo.u32FileSize));
	MAIN_DBG(("[MP3] u16BitRate = %d Kbps\n", Media_Info.FileMMAudioMP3DetailInfo.u16BitRate));
	MAIN_DBG(("[MP3] u32Duration = %d s\n", Media_Info.FileMMAudioMP3DetailInfo.u32Duration));
	MAIN_DBG(("[MP3] u16SampleRate = %d HZ\n", Media_Info.FileMMAudioMP3DetailInfo.u16SampleRate));
	MAIN_DBG(("[MP3] u8Flag = %d\n", Media_Info.FileMMAudioMP3DetailInfo.u8Flag));
	MAIN_DBG(("[MP3] u16TOCLength = %d\n", Media_Info.FileMMAudioMP3DetailInfo.u16TOCLength));
	MAIN_DBG(("[MP3] u8TOC = %s\n", Media_Info.FileMMAudioMP3DetailInfo.u8TOC));
	return;
		}
		


//======================================================================//
// void File_Detail_Info_AVI_Print(void)
//======================================================================//
void File_Detail_Info_AVI_Print(void)
		{
    MAIN_DBG(("==================AVI Main Header Info==================\n"));
    MAIN_DBG(("'%s' (%d, 0x%x))\n",
						MKTAG2STR_0(Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.MMRiffChunkGeneral.u32Fcc), 
						Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.MMRiffChunkGeneral.u32Size, 
						Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.MMRiffChunkGeneral.u32Size));					
    MAIN_DBG(("MicroSec/Frame = %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32MicroSecPerFrame));
    MAIN_DBG(("MaxByte/Sec = %dKB\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32MaxBytesPerSec/1024));
    MAIN_DBG(("Padding Granularity = %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32PaddingGranularity));
    MAIN_DBG(("Flags = %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Flags));
				int u32Flags = Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Flags;
				if (u32Flags)
				{
					if (u32Flags & AVIF_HASINDEX)
					{
    MAIN_DBG(("  Has Index\n"));
					}
					if (u32Flags & AVIF_MUSTUSEINDEX)
					{
    MAIN_DBG(("  Must Use Index\n"));
					}
					if (u32Flags & AVIF_ISINTERLEAVED)
					{
    MAIN_DBG(("  Is Interleaved\n"));
					}
					if (u32Flags & AVIF_TRUSTCKTYPE)
					{
    MAIN_DBG(("  Trust CK Type\n"));
					}
					if (u32Flags & AVIF_WASCAPTUREFILE)
					{
    MAIN_DBG(("  Was Capture File\n"));
					}
					if (u32Flags & AVIF_COPYRIGHTED)
					{
    MAIN_DBG(("  Copyrighted\n"));
					}
				}

    MAIN_DBG(("Total Frames = %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32TotalFrames));
    MAIN_DBG(("Initial Frames = %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32InitialFrames));
    MAIN_DBG(("Streams = %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Streams));
    MAIN_DBG(("Suggested Buffer Size = %dKB\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32SuggestedBufferSize/1024));
    MAIN_DBG(("Width = %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Width));
    MAIN_DBG(("Height = %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Height));

    MAIN_DBG(("==================AVI Stream Header Info==================\n"));
				unsigned char Nstreams = 0;
				for(Nstreams = 0; Nstreams < Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Streams; Nstreams++)
				{
    MAIN_DBG(("\n'%s' (%d, 0x%x))\n",
							MKTAG2STR_0(Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].MMRiffChunkGeneral.u32Fcc), 
							Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].MMRiffChunkGeneral.u32Size, 
							Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].MMRiffChunkGeneral.u32Size));				
    MAIN_DBG(("FCC Type: = %s\n", MKTAG2STR_0(Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32FccType)));
    MAIN_DBG(("FCC Handler: = %s\n", MKTAG2STR_0(Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32FccHandler)));
    MAIN_DBG(("Flags: 0x%08x\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Flags));
					u32Flags = Media_Info.FileMMVideoAVIDetailInfo.AviMainHeader.u32Flags;
					if (u32Flags)
					{
						if (u32Flags & AVISF_DISABLED)
						{
        MAIN_DBG(("  Disabled\n"));
						}
						if (u32Flags & AVISF_VIDEO_PALCHANGES)
						{
        MAIN_DBG(("  Palette Change\n"));
						}
					}				
    MAIN_DBG(("Priority: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u16wPriority));
    MAIN_DBG(("Language: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u16Language));
    MAIN_DBG(("Initial Frames: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32InitialFrames));
					
    MAIN_DBG(("Scale: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Scale));
    MAIN_DBG(("Rate: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Rate));
    MAIN_DBG(("Start: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Start));
    MAIN_DBG(("Length: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Length));
    MAIN_DBG(("Suggested Buffer Size: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32SuggestedBufferSize));
    MAIN_DBG(("Quality: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32Quality));
    MAIN_DBG(("Sample Size: %d%s\n", Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32SampleSize, \
							(Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32SampleSize == 0 ? " (Variable)" : "")));
					
					if (Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].u32FccType != MKTAG('a', 'u', 'd', 's'))
					{
    MAIN_DBG(("Frame (X, Y, Width, Height): (%d, %d, %d, %d)\n",
								Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].stRcFrame.u16Left,
								Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].stRcFrame.u16Top,
								Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].stRcFrame.u16Right,
								Media_Info.FileMMVideoAVIDetailInfo.AviStreamHeader[Nstreams].stRcFrame.u16Bottom));
					}
				}

	
    MAIN_DBG(("==================AVI Stream Header Video Definition==================\n"));
    MAIN_DBG(("'%s' (%d, 0x%x))\n", \
						MKTAG2STR_0(Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.MMRiffChunkGeneral.u32Fcc), \
						Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.MMRiffChunkGeneral.u32Size, \
						Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.MMRiffChunkGeneral.u32Size));
    MAIN_DBG(("Size: %d 0x%x\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Size,\
						Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Size));
    MAIN_DBG(("Width: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Width));
    MAIN_DBG(("Height: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Height));
    MAIN_DBG(("Planes: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u16Planes));
    MAIN_DBG(("Bit Count: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u16BitCount));
    MAIN_DBG(("Compression: '%s'\n", MKTAG2STR_0(Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32Compression)));
    MAIN_DBG(("Size Of Image: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32SizeImage));
    MAIN_DBG(("X Pixels/Meter: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32XPelsPerMeter));
    MAIN_DBG(("Y Pixels/Meter: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32YPelsPerMeter));
    MAIN_DBG(("Color Indices In Used: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32ClrUsed));
    MAIN_DBG(("Color Indices Important: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviBmpInfoHeader.u32ClrImportant));

				
    MAIN_DBG(("==================AVI Stream Header Audio Definition==================\n"));
    MAIN_DBG(("'%s' (%d, 0x%x))\n",
						MKTAG2STR_0(Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.MMRiffChunkGeneral.u32Fcc), 
						Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.MMRiffChunkGeneral.u32Size, 
						Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.MMRiffChunkGeneral.u32Size));
    MAIN_DBG(("Format Tag: 0x%x\n", Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16FormatTag));
    MAIN_DBG(("Cannels: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16Channels));
    MAIN_DBG(("Sample Rate: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u32SamplesPerSec));
    MAIN_DBG(("Bit Rate: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u32AvgBytesPerSec * 8));
    MAIN_DBG(("Block Align: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16BlockAlign));
    MAIN_DBG(("Bits/Sample: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16BitsPerSample));
    MAIN_DBG(("Extra Data Size: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u16ExtraDataSize));
    //MAIN_DBG((" Samples/Block: %d\n", Media_Info.FileMMVideoAVIDetailInfo.AviWaveFormatEx.u32SamplePerBlock & 0xffff);
    return;
}

//======================================================================//
// void File_Detail_Info_Print(void)
//======================================================================//
void File_Detail_Info_Print(void)
{
	switch(Media_Info.FileMMTypeInfo.FileMMType)
	{
		case EN_MM_PHOTO:
		{
			switch(Media_Info.FileMMSubTypeInfo.FileMMSubType)
			{
				case EN_MM_SUB_PHOTO_JPG:
				{
					File_Detail_Info_JPG_Print();
					break;
				}
				case EN_MM_SUB_PHOTO_BMP:
				{
					File_Detail_Info_BMP_Print();
					break;
				}
				case EN_MM_SUB_PHOTO_PNG:
				{
					File_Detail_Info_PNG_Print();
					break;
				}
				case EN_MM_SUB_PHOTO_TIFF:
				{
					File_Detail_Info_TIFF_Print();
					break;
				}
				case EN_MM_SUB_PHOTO_GIF:
				{
					File_Detail_Info_GIF_Print();
					break;
				}
				default:
					break;
			}
			break;
		}

		case EN_MM_AUDIO:
		{
			case EN_MM_SUB_AUDIO_MP3:
			{
				File_Detail_Info_MP3_Print();
				break;
			}
			break;
		}

		case EN_MM_MOVIE:
		{
			case EN_MM_SUB_MOVIE_AVI:
			{
				File_Detail_Info_AVI_Print();
				break;
			}		
			break;
		}

		case EN_MM_TEXT:
		{
			break;
		}

		default:
		{
			break;
		}
	}
	return;
}
		
//======================================================================//
// void main()
//======================================================================//
void main()
{
    FILE *fp1;
//open File
    if((fp1 = fopen(FILE_NAME,"rb")) == NULL)
    {
        MAIN_ERR(("error open1\n"));
        return;
		}
//Get General Info
    Get_General_Info(fp1);
    MAIN_DBG(("\nGeneral\n"));
    MAIN_DBG(("Complete File Name = %s\n", Media_Info.FileName));

    if(Media_Info.kilogyte_megabyte)
    {
        MAIN_DBG(("File Size = %.02fMB\n", (float)Media_Info.FileSize / (float)(1024.0 * 1024.0)));
    }
    else
    {
        MAIN_DBG(("File Size = %.02fKB\n", (float)Media_Info.FileSize / (float)(1024.0)));
    }
    MAIN_DBG(("\n"));
//File Format
    if(File_Format_Parser(fp1))
    {
        MAIN_DBG(("%s -- %s\n", Media_Info.FileMMTypeInfo.FileMMTypeName, Media_Info.FileMMSubTypeInfo.FileMMSubTypeName));
    }
    else
    {
        MAIN_DBG(("Unknown file format\n"));
    }
    MAIN_DBG(("\n"));
//File Detail Info
    MApp_Set_WholeBigEndian(0);
    if(File_Detail_Info_Parser(fp1))
    {
    	File_Detail_Info_Print();
	}
    MAIN_DBG(("\n"));
//close File
	fclose(fp1);
	return;
}
