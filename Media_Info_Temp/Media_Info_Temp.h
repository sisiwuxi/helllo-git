//------------------------------------------------------------------------------------------------
// Local Definition
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  System Data Type
//-------------------------------------------------------------------------------------------------
/// data type unsigned char, data length 1 byte
typedef unsigned char               MS_U8;                              // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short              MS_U16;                             // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned long               MS_U32;                             // 4 bytes
/// data type signed char, data length 1 byte
typedef signed char                 MS_S8;                              // 1 byte
/// data type signed short, data length 2 byte
typedef signed short                MS_S16;                             // 2 bytes
/// data type signed int, data length 4 byte
typedef signed long                 MS_S32;                             // 4 bytes
/// data type float, data length 4 byte
typedef float                       MS_FLOAT;                           // 4 bytes
/// data type unsigned char, data length 1 byte
typedef unsigned char               BOOLEAN;                            // 1 byte
/// data type unsigned char, data length 1 byte
typedef unsigned char               MS_BOOL;                            // 1 byte

/// data type null pointer
#define NULL                        0

#define FILE_NAME "E:/MM/music/happy.wma"
//E:/MM/photo/H-bars1.jpg, 2_Progressive.jpg, UVS100629-001.BMP, IMG-17.800x600.png, 1.63M.TIF, MTR.gif
//E:/MM/video/bj.avi
//"E:/MM/image/MTR.gif"
//"E:/MM/music/old boy.mp3", "E:/MM/music/happy.wma"


#define NULL                                        0
#define FALSE                                       0
#define TRUE                                        1
#define MAX_FILE_NAME_LEN							256
#define TIFF_MAX_LEN								20

//------------------------------------------------------------------------------
// logo
//------------------------------------------------------------------------------
#define MAIN_ERR(x)                         printf x;
#define MAIN_DBG(x)                         printf x;
#define IMAGE_ERR(x)                        printf x;
#define IMAGE_DBG(x)                        //printf x;;
#define AUDIO_ERR(x)                        printf x;
#define AUDIO_DBG(x)                        printf x;
#define MOVIE_ERR(x)                        printf x;
#define MOVIE_DBG(x)                        //printf x;

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------

#define FileFormatParserMallocBuff(u32Size) 		((void*)malloc(u32Size))
#define FileFormatParserFreeBuff(pBuf)				(free(pBuf))

#define TO16BIT(x) (unsigned short) ((x)[0]<<8 | (x)[1])
#define TO32BIT(x) (unsigned int) ((x)[0]<<24 | (x)[1]<<16 |(x)[2]<<8 |(x)[3])
#define TO64BIT(x) (unsigned __int64) ((unsigned __int64)(x)[0]<<56 | (unsigned __int64)(x)[1]<<48 | (unsigned __int64)(x)[2]<<40 | (unsigned __int64)(x)[3]<<32 | (unsigned __int64)(x)[4]<<24 | (unsigned __int64)(x)[5]<<16 | (unsigned __int64)(x)[6]<<8 | (unsigned __int64)(x)[7])

#define EndianChangeS(x) (((x & 0xff) << 8) | ((x & 0xff00) >> 8))
#define EndianChangeL(x) (((x & 0xff) << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | ((x & 0xff000000) >> 24))
#define EndianChangeLL(x) (unsigned __int64)((unsigned __int64)((x & 0xff) << 56) | (unsigned __int64)((x & 0xff00) << 40) | (unsigned __int64)((x & 0xff0000) << 24) | (unsigned __int64)((x & 0xff000000) << 8) | (unsigned __int64)((x & 0xff00000000) >> 8) | (unsigned __int64)((x & 0xff0000000000) >> 24) | (unsigned __int64)((x & 0xff000000000000) >> 40) | (unsigned __int64)((x & 0xff00000000000000) >> 56))

#define MKTAG(a,b,c,d) ((unsigned int)a | ((unsigned int)b << 8) | ((unsigned int)c << 16) | ((unsigned int)d << 24))
// 2 FourCC in 1 printf is enough for me
static unsigned char u8TagBuff[16];
static unsigned char *pu8TagBuff_0 = u8TagBuff, *pu8TagBuff_1 = u8TagBuff + 8;
#define MKTAG2STR_0(value) \
        (pu8TagBuff_0[0] = (unsigned char)(value), \
            pu8TagBuff_0[1] = (unsigned char)(value >> 8), \
            pu8TagBuff_0[2] = (unsigned char)(value >> 16), \
            pu8TagBuff_0[3] = (unsigned char)(value >> 24), \
            pu8TagBuff_0[4] = 0, \
            pu8TagBuff_0)
#define MKTAG2STR_1(value) \
        (pu8TagBuff_1[0] = (unsigned char)(value), \
            pu8TagBuff_1[1] = (unsigned char)(value >> 8), \
            pu8TagBuff_1[2] = (unsigned char)(value >> 16), \
            pu8TagBuff_1[3] = (unsigned char)(value >> 24), \
            pu8TagBuff_1[4] = 0, \
            pu8TagBuff_1)

//--------------------------------------------------------------------------------------------------
// image
//--------------------------------------------------------------------------------------------------
//===================================JPEG========================================//
#define JFIF_SOI_CHECKING_LEN						640
#define JPEG_MARKER_PREFIX							0xFF
#define JPEG_MARKER_SOI								0xD8
#define JPEG_MARKER_APP1							0xE1
#define JPEG_MARKER_SOF0							0xC0
#define JPEG_MARKER_SOF2							0xC2
#define JPEG_MARKER_COM								0xFE
#define JPEG_MARKER_TEM								0x01
#define JPEG_MARKER_APP0							0xE1
#define JPEG_MARKER_APP15							0xEF
#define JPEG_DI_SIZE								4096
#define JPEG_TAG_EXIF								0x45786966
#define JPEG_TIFF_BIG_ENDIAN						0x4D4D
#define JPEG_TIFF_LITTLE_ENDIAN						0x4949
#define JPEG_TIFF_X_RESOLUTION						0x011A
#define JPEG_TIFF_Y_RESOLUTION						0x011B
#define JPEG_TIFF_RESOLUTION_UNIT					0x0128
#define JPEG_TIFF_TAG_SOFTWARE						0x0131
#define JPEG_TIFF_TAG_DATETIME						0x0132
#define JPEG_TIFF_TAG_TITLE							0x010E
#define JPEG_TIFF_TAG_ORIENTATION					0x0112
#define JPEG_TIFF_TAG_EXIF_POINTER					0x8769
#define JPEG_EXIF_TAG_DATETIME						0x9003
#define JPEG_TIFF_COMPRESSION						0x0103
#define JPEG_TIFF_SOI_OFFSET						0x0201
#define JPEG_TIFF_SOI_OFFSET_BYTE_COUNT				0x0202
#define JPEG_EXIF_TAG_COLOR_SPACE					0xA001
#define JPEG_EXIF_TAG_IMAGE_WIDTH					0xA002
#define JPEG_EXIF_TAG_IMAGE_HEIGHT					0xA003
//===================================BMP========================================//
#define BMP_FILE_HEADER_SIZE						14
#define BMP_SIGNATURE								0x424D
#define BMP_DI_SIZE									100
//===================================PNG========================================//
#define PNG_HEADER_SIZE								33
#define PNG_DI_SIZE									4096
#define PNG_CHUNK_TYPE_IHDR							0x49484452
#define PNG_CHUNK_TYPE_pHYs							0x70485973
#define PNG_CHUNK_TYPE_gAMA							0x67414d41
#define PNG_CHUNK_TYPE_cHRM							0x6348524d
#define PNG_CHUNK_TYPE_IDAT							0x49444154
#define PNG_CHUNK_TYPE_IEND							0x49454e44
//===================================TIFF========================================//
#define TIFF_HEADER_SIZE							4
#define TIFF_SIGNATURE1								0x49
#define TIFF_SIGNATURE2								0x4D
#define TIFF_SIGNATURE								0x2A
#define TIFF_DI_SIZE								4096
#define TIFF_IFD_Tag_IMAGEWIDTH						0x0100
#define TIFF_IFD_Tag_IMAGELENGTH					0x0101
#define TIFF_IFD_Tag_BITSPERSAMPLE					0x0102
#define TIFF_IFD_Tag_COMPRESSION					0x0103
#define TIFF_IFD_Tag_PHOTOMETRICINTERPRETATION		0x0106
#define TIFF_IFD_Tag_STRIPOFFSETS					0x0111
#define TIFF_IFD_Tag_SAMPLESPERPIXEL				0x0115
#define TIFF_IFD_Tag_ROWPERSTRIP					0x0116
#define TIFF_IFD_Tag_STRIPBYTECOUNTS				0x0117
#define TIFF_IFD_Tag_XRESOLUTION					0x011A
#define TIFF_IFD_Tag_YRESOLUTION					0x011B
#define TIFF_IFD_Tag_PLANARCONFIGURATION			0x011C
#define TIFF_IFD_Tag_RESOLUTIONUNIT					0x0128
#define TIFF_IFD_Tag_PREDICTOR						0x013D
#define TIFF_IFD_Tag_COLORMAP						0x0140
//===================================GIF========================================//
#define GIF_HEADER_SIZE                             4
#define GIF_VERSION_SIZE                            4
#define GIF_SIGNATURE								0x474946
#define GIF_MAX_FRAMES                              (64)



//--------------------------------------------------------------------------------------------------
// music
//--------------------------------------------------------------------------------------------------
//===================================MP3========================================//
#define MP3_HEADER_SIZE                             4096
#define MP3_ID3_V1_LEN                              128
#define MP3_INFO_LEN                                (512 * 8)
#define MP3_TOC_LEN                                  100
#define MP3_INVALID_RATE                            0xffff
#define MP3_TAG_XING                                 0x58696E67
#define MP3_TAG_VBRI                                 0x56425249
#define MP3_FLAG_VBR_XING                           0x01
#define MP3_FLAG_VBR_VBRI                           0x02
#define XING_FRAMES                                  0x00000001
#define XING_BYTES                                   0x00000002
#define XING_TOC                                     0x00000004
#define XING_SCALE                                   0x00000008
#define MP3_SAMPLE_PER_FRAME                        1152UL
#define MP3_SAMPLE_PER_FRAME_LSF                    576UL

#define MP3_TAG_ID3v1_LENGTH                        4 //if ID3v2 < 4 bytes , we display ID3v1 tag
#define MP3_TAG_ID3v2_COMM                          0x434f4d4d
#define MP3_TAG_ID3v2_TALB                          0x54414C42
#define MP3_TAG_ID3v2_TPE1                          0x54504531
#define MP3_TAG_ID3v2_TCOM                          0x54434F4D
#define MP3_TAG_ID3v2_TEXT                          0x54455854
#define MP3_TAG_ID3v2_TIT1                          0x54495431
#define MP3_TAG_ID3v2_TIT2                          0x54495432
#define MP3_TAG_ID3v2_TLEN                          0x544C454E
#define MP3_TAG_ID3v2_TYER                          0x54594552
#define MP3_TAG_ID3v2_APIC                          0x41504943
//===================================WMA========================================//
#define WMA_HEADER_SIZE                             4096

//--------------------------------------------------------------------------------------------------
// movie
//--------------------------------------------------------------------------------------------------
//===================================AVI========================================//
#define AVI_HEADER_SIZE							12
#define AVI_DI_SIZE								4096
// AVI MAIN HEADER FLAG
#define AVIF_HASINDEX                             (0x00000010)
#define AVIF_MUSTUSEINDEX                         (0x00000020)
#define AVIF_ISINTERLEAVED                        (0x00000100)
#define AVIF_TRUSTCKTYPE                          (0x00000800)
#define AVIF_WASCAPTUREFILE                       (0x00010000)
#define AVIF_COPYRIGHTED                          (0x00020000)
// AVI STREAM HEADER FLAG
#define AVISF_DISABLED                              (0x00000001)
#define AVISF_VIDEO_PALCHANGES                     (0x00010000)

//--------------------------------------------------------------------------------------------------
// Enumeration
//--------------------------------------------------------------------------------------------------
typedef enum
{
	EN_MM_NONE = 0,
	EN_MM_PHOTO,
    EN_MM_AUDIO,
	EN_MM_MOVIE,
	EN_MM_TEXT,
	EN_MM_MAX
} EN_MM_TYPE;

typedef enum
{
	EN_MM_SUB_NONE = 0,
//image
	EN_MM_SUB_PHOTO_JPG,
	EN_MM_SUB_PHOTO_BMP,
	EN_MM_SUB_PHOTO_PNG,
	EN_MM_SUB_PHOTO_TIFF,
	EN_MM_SUB_PHOTO_GIF,
//audio
    EN_MM_SUB_AUDIO_MP3,
    EN_MM_SUB_AUDIO_AAC,
    EN_MM_SUB_AUDIO_PCM,
    EN_MM_SUB_AUDIO_WAV,
    EN_MM_SUB_AUDIO_WMA,
    EN_MM_SUB_AUDIO_OGG,
//video
	EN_MM_SUB_MOVIE_AVI,
	EN_MM_SUB_MOVIE_TS,
	EN_MM_SUB_MOVIE_MPG,
	EN_MM_SUB_MOVIE_DAT,
	EN_MM_SUB_MOVIE_MOV,
	EN_MM_SUB_MOVIE_MKV,
	EN_MM_SUB_MOVIE_3GP,
	EN_MM_SUB_MOVIE_WMV,
	EN_MM_SUB_MOVIE_FLV,
	EN_MM_SUB_MOVIE_MP4,
	EN_MM_SUB_MOVIE_VOB,
	EN_MM_SUB_MOVIE_MPA,
//text
	EN_MM_SUB_TEXT,
	EN_MM_SUB_MAX
} EN_MM_SUB_TYPE;

typedef enum
{
	EN_MM_TIFF_IFDFIELDTYPE_BYTE = 1,
	EN_MM_TIFF_IFDFIELDTYPE_ASCII,
	EN_MM_TIFF_IFDFIELDTYPE_SHORT,
	EN_MM_TIFF_IFDFIELDTYPE_LONG,
	EN_MM_TIFF_IFDFIELDTYPE_RATIONAL,
	EN_MM_TIFF_IFDFIELDTYPE_SBYTE,
	EN_MM_TIFF_IFDFIELDTYPE_UNDEFINED,
	EN_MM_TIFF_IFDFIELDTYPE_SSHORT,
	EN_MM_TIFF_IFDFIELDTYPE_SLONG,
	EN_MM_TIFF_IFDFIELDTYPE_SRATIONAL,
	EN_MM_TIFF_IFDFIELDTYPE_FLOAT,
	EN_MM_TIFF_IFDFIELDTYPE_DOUBLE,
	EN_MM_TIFF_IFDFIELDTYPE_NUM
} EN_MM_TIFF_IFDFIELDTYPE_LEN;


//------------------------------------------------------------------------------------------------
// Local struct
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// image
//------------------------------------------------------------------------------------------------
typedef struct
{
	bool						ImageProgressive;
	char						Precision;
	int							ImageWidth;
	int							ImageHeight;
	char						ColorComponents;
	char						TiffBigEndian;
	char						Orientation;
	char						ResolutionUnit;
	int							X_Resolution;
	int							Y_Resolution;
	char						Software[TIFF_MAX_LEN];
	char						DataTime[TIFF_MAX_LEN];
	char						Title[TIFF_MAX_LEN];
	int							ExifColorSpace;
	int							ExifImageWidth;
	int							ExifImageHeight;
	bool						ImageThumbnail;
	char						ThumbnailCompression;
	int 						ThumbnailOffset;
	int 						ThumbnailByteCount;
	bool						ThumbnailImageProgressive;
	char						ThumbnailPrecision;
	int							ThumbnailImageWidth;
	int							ThumbnailImageHeight;
	char						ThumbnailColorComponents;
	char						ImageResolution;
	char						ImageScalefactor;
} File_MM_Image_JPEG_Detail_Info;

typedef struct
{
	int							DataOffset;
	int							ImageWidth;
	int							ImageHeight;
	short						Precision;
	int							Compression;
	int							DataSize;
	int							HorizontalResolution;
	int							VerticalResolution;
	int							ColorIndexCount;
	int							ImportantColorIndexCount;
} File_MM_Image_BMP_Detail_Info;


typedef struct
{
	int							ImageWidth;
	int							ImageHeight;
	char						BitDepth;
	char						ColorType;
	char						CompressionMethod;
	char						FilterMethod;
	char						InterlaceMethod;
	int							PixelPerUnitXAxis;
	int							PixelPerUnitYAxis;
	char						UnitSpecifier;
	int							Gamma;
	int							WhitePX;
	int							WhitePY;
	int							RedX;
	int							RedY;
	int							GreenX;
	int							GreenY;
	int							BlueX;
	int							BlueY;
} File_MM_Image_PNG_Detail_Info;

typedef struct
{
	short						TiffIFDTag;
	short						TiffIFDFieldType;
	int							TiffIFDNum;
	int							TiffIFDData;
}File_MM_Image_TIFF_IFD;

typedef struct
{
	short						TiffBigEndian;
	short						VersionNum;
	int							FirstIFDOffset;
	short						DirEntryNum;
	int							ImageWidth;
	int							ImageLength;
	short						BitsPerSample[4];
	short						Compression;
	short						PhotoMetricInterpretation;
	int							StripOffsets;
	short						SamplesPerPixel;
	short						RowsPerStrip;
	int							StripByteCounts;
	__int64						XResolution;
	__int64						YResolution;
	short						PlanarConfiguration;
	short						ResolutionUnit;
	short						Predictor;
}File_MM_Image_TIFF_Detail_Info;

typedef char File_MM_Image_GIF_RGB[3][256];

typedef enum
{
    E_GIF_ARGB8888  = 0,
    E_GIF_ARGB1555,
} File_MM_Image_GIF_OUTPUT_FORMAT;

typedef enum
{
    RETAIN,
    DISPOSE,
    REVERT
} File_MM_Image_GIF_FRAME_ACTION;

typedef enum
{
    GIF_GET_NEXT_STEP,
    GIF_LZW_FILL_BUFFER,
    GIF_LZW_CLEAR_CODE,
    GIF_GET_LZW,
    GIF_DECODE_ROWS,
    GIF_WAIT_FREE_SLOT,
    GIF_DECODE_ONE_FRAME_DONE,
    GIF_DONE
}File_MM_Image_GIF_DECODE;

typedef struct
{
    signed long n_channels;
    signed long width, height;
    signed long rowstride;
    char* pixels;
    unsigned char has_alpha;
}File_MM_Image_GIFPIXEL_INFO;

typedef struct
{
    File_MM_Image_GIFPIXEL_INFO* pixbuf;
    signed long x_offset;
    signed long y_offset;
    signed long delay_time;
    signed long elapsed;
    File_MM_Image_GIF_FRAME_ACTION action;
    unsigned char bg_transparent;
}File_MM_Image_GIF_FRAME;

typedef struct
{
    signed long transparent;
    signed long delay_time;
    signed long input_flag;
    signed long disposal;
}File_MM_Image_GIF89;

typedef struct
{
    char bg_red;
    char bg_green;
    char bg_blue;
    signed long n_frames;
    signed long total_time;
    File_MM_Image_GIF_FRAME *frames[GIF_MAX_FRAMES];
    signed long width, height;
    signed long loop;
}File_MM_Image_GIF_ANIM;


typedef struct
{
    char u8Header[GIF_HEADER_SIZE];
    char u8Version[GIF_VERSION_SIZE];
    int u32DataProcessed;

    int u32Width;
    int u32Height;
    int u32Pitch;
    int u32BytePerOutputRow;

    File_MM_Image_GIF_OUTPUT_FORMAT eOutputFormat;
    char u8OutputPixelSize;
    int u32OutputWidth;
    int u32OutputHeight;
    int u32OutputPitch;
    char u8FreeSlotNum;
    char u8DecodeSlotIndex;
    int u32DisplayFrameIndex;
    int u32FrameBuffSize;
    int u32FrameInfoArrayBuffAddr;
    int u32FrameInfoArrayBuffSize;

    char u8Factor;
    char *pu8OutputBuffer;

    int u32TotalRowProcessed;
    int u32TotalRowOutputted;
    int u32LoopNumber;
    int u32TotalFramesInOneLoop;

    unsigned char bStop;

    File_MM_Image_GIF_ANIM* animation;
    File_MM_Image_GIF_FRAME* frame;
    File_MM_Image_GIF89 gif89;

    signed long state;

    File_MM_Image_GIF_RGB global_color_table;
    signed long global_colortable_size;
    int global_bit_pixel;
    int global_color_resolution;
    int background_index;
    unsigned char  has_global_colortable;
    unsigned char  stop_after_first_frame;
    unsigned char  has_local_colortable;
    unsigned char  ZeroDataBlock;
    File_MM_Image_GIF_RGB frame_color_map;
    signed long frame_colormap_size;
    int frame_bit_pixel;
    int aspect_ratio;

    signed long frame_len;
    signed long frame_height;
    signed long frame_interlace;
    signed long x_offset;
    signed long y_offset;

    char* buf;
    char extension_label;
    char extension_flag;
    char block_count;
    char block_buf[280];
    signed long block_ptr;
    signed long old_state;
    int ptr;
    int size;
    int amount_needed;
    unsigned char  in_loop_extension;

    signed long code_curbit;
    signed long code_lastbit;
    signed long code_done;
    signed long code_last_byte;
    signed long lzw_code_pending;

    char lzw_set_code_size;
    signed long lzw_fresh;
    signed long lzw_code_size;
    signed long lzw_max_code;
    signed long lzw_max_code_size;
    signed long lzw_firstcode;
    signed long lzw_oldcode;
    signed long lzw_clear_code;
    signed long lzw_end_code;
    signed long* lzw_sp;
    signed long lzw_table[2][4096];   //[(1 << MAX_LZW_BITS)];
    signed long lzw_stack[8193];       //[(1 << (MAX_LZW_BITS))* 2+1];
    signed long draw_xpos;
    signed long draw_ypos;
    signed long draw_pass;
}File_MM_Image_GIF_Detail_Info;

//------------------------------------------------------------------------------------------------
// music
//------------------------------------------------------------------------------------------------
typedef enum
{
    EN_MP3_LAYER_RESERVED = 0x0,
    EN_MP3_LAYER_3 = 0x1,
    EN_MP3_LAYER_2 = 0x2,
    EN_MP3_LAYER_1 = 0x3,
    EN_MP3_LAYER_NOT_DETERMINE_YET = 0xFF,
} EN_MP3_LAYER;

typedef enum
{
    EN_MP3_VERSION_2_5 = 0x0,
    EN_MP3_VERSION_RESERVED = 0x1,
    EN_MP3_VERSION_2 = 0x2,
    EN_MP3_VERSION_1 = 0x3,
    EN_MP3_VERSION_NOT_DETERMINE_YET = 0xFF,
} EN_MP3_VERSION;

typedef struct
{
    MS_U8   title[32];
    MS_U8   artist[32];
    MS_U8   album[32];
    MS_U8   year[6];
    MS_U8   comment[32];
    MS_U8   genre;
    MS_U8   u8TitleLength;
    MS_U8   u8ArtistLength;
    MS_U8   u8AlbumLength;
    BOOLEAN imageJpg;
    MS_U8   imageType;
    MS_U32  imageOffset;
    MS_U32  imageSize;
    EN_MP3_LAYER en_layer;
    MS_U32  u32ID3TagSize;
} MP3_INFO;

typedef struct
{
    MS_U16 u16BitRate;
    MS_U16 u16SampleRate;
    MS_U8 u8Flag;
    EN_MP3_VERSION enVersion;
    EN_MP3_LAYER enLayer;

    BOOLEAN bID3TagV1;
    MS_U16 u16ID3TagV1Size;
    BOOLEAN bID3TagV2;
    MS_U32 u32ID3TagV2Size;
    BOOLEAN bCheckFfFb;
    MP3_INFO stID3v1;
    MP3_INFO stID3v2;

    MS_U32 u32FileSize;
    MS_U32 u32MusicDataBytesLeft;
    MS_U32 u32Duration;
    MS_U32 u32CurrentPlayTime;
    MS_U32 u32PlayTimer;
    MS_U16 u16TOCLength;
    MS_U8 u8TOC[MP3_TOC_LEN]; // Table of Contents for VBR file seek
}File_MM_Audio_MP3_Detail_Info;


typedef struct
{
    MS_U16 u32BitRate;
    MS_U16 u16SampleRate;
    MS_U32 u32Duration;
    MS_U32 u32HeaderSize;
    MS_U32 u32PacketSize;
    MS_U32 u32TotalPacket;
    MS_U32 u32SkipPacket;
    MS_U32 u32FileSize;
    MS_U32 u32CurrentPlayTime;
    MS_U32 u32PlayTimer;
    MS_U8 u8Title[128];
    MS_U8 u8Author[128];
}File_MM_Audio_WMA_Detail_Info;

//------------------------------------------------------------------------------------------------
// movie
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
// AVI
//------------------------------------------------------------------------------------------------
typedef struct
{
    int u32Fcc;
    int u32Size;//Not including the first eight bytes:u32Fcc and u32Size
} File_MM_Video_RIFF_CHUNK_GENERAL, *PFile_MM_Video_RIFF_CHUNK_GENERAL;

typedef struct
{
    int u32Fcc;//fourcc--four-character-code
    int u32Size;//Not including the first eight bytes:u32Fcc and u32Size
    int u32FccListType;//concrete type
} File_MM_Video_RIFF_LIST, *PFile_MM_Video_RIFF_LIST;

typedef struct
{
    File_MM_Video_RIFF_CHUNK_GENERAL MMRiffChunkGeneral;
    int u32MicroSecPerFrame;
    int u32MaxBytesPerSec;
    int u32PaddingGranularity;
    int u32Flags;
    int u32TotalFrames;
    int u32InitialFrames;
    int u32Streams;
    int u32SuggestedBufferSize;
    int u32Width;
    int u32Height;
    int u32Reserved[4];
} File_MM_Video_AVI_MAIN_HEADER, *PFile_MM_Video_AVI_MAIN_HEADER;

typedef struct
{
    File_MM_Video_RIFF_CHUNK_GENERAL MMRiffChunkGeneral;

    int u32FccType;
    int u32FccHandler;
    int u32Flags;
    short u16wPriority;
    short u16Language;
    int u32InitialFrames;
    int u32Scale;
    int u32Rate;
    int u32Start;
    int u32Length;
    int u32SuggestedBufferSize;
    int u32Quality;
    int u32SampleSize;
   struct
   {
        short u16Left;
        short u16Top;
        short u16Right;
        short u16Bottom;
    } stRcFrame;
} File_MM_Video_AVI_STREAM_HEADER, *PFile_MM_Video_AVI_STREAM_HEADER;

typedef struct
{
    File_MM_Video_RIFF_CHUNK_GENERAL MMRiffChunkGeneral;

    int u32Size;
    int u32Width;
    int u32Height;
    short u16Planes;
    short u16BitCount;
    int u32Compression;
    int u32SizeImage;
    int u32XPelsPerMeter;
    int u32YPelsPerMeter;
    int u32ClrUsed;
    int u32ClrImportant;
} File_MM_Video_AVI_BITMAP_INFO_HEADER, *PFile_MM_Video_AVI_BITMAP_INFO_HEADER;

typedef struct
{
    File_MM_Video_RIFF_CHUNK_GENERAL MMRiffChunkGeneral;

    short u16FormatTag;
    short u16Channels;
    int u32SamplesPerSec;
    int u32AvgBytesPerSec;
    short u16BlockAlign;
    short u16BitsPerSample;
    short u16ExtraDataSize;
    short u16EncodeOpt;
    int u32ChannelMask;
} File_MM_Video_AVI_WAVE_FORMATEX, *PFile_MM_Video_AVI_WAVE_FORMATEX;

typedef struct
{
    File_MM_Video_AVI_MAIN_HEADER AviMainHeader;
    File_MM_Video_AVI_STREAM_HEADER AviStreamHeader[4];
    File_MM_Video_AVI_BITMAP_INFO_HEADER AviBmpInfoHeader;
    File_MM_Video_AVI_WAVE_FORMATEX AviWaveFormatEx;
}File_MM_Video_AVI_Detail_Info;


//------------------------------------------------------------------------------------------------
// multimedia
//------------------------------------------------------------------------------------------------
typedef struct
{
	char						FileMMTypeName[8];
	EN_MM_TYPE					FileMMType;
} File_MM_Type_Info;

typedef struct
{
	char						FileMMSubTypeName[8];
	EN_MM_SUB_TYPE				FileMMSubType;
} File_MM_SubType_Info;


typedef struct
{
//General Info
	char								FileName[MAX_FILE_NAME_LEN];
	bool								kilogyte_megabyte;
	float								FileSize;
//File Format
	File_MM_Type_Info					FileMMTypeInfo;
	File_MM_SubType_Info				FileMMSubTypeInfo;
//File Detail Info
//File Image
	File_MM_Image_JPEG_Detail_Info		FileMMImageJPEGDetailInfo;
	File_MM_Image_BMP_Detail_Info		FileMMImageBMPDetailInfo;
	File_MM_Image_PNG_Detail_Info		FileMMImagePNGDetailInfo;
	File_MM_Image_TIFF_Detail_Info		FileMMImageTIFFDetailInfo;
    File_MM_Image_GIF_Detail_Info       FileMMImageGIFDetailInfo;
//File Audio
    File_MM_Audio_MP3_Detail_Info       FileMMAudioMP3DetailInfo;
    File_MM_Audio_WMA_Detail_Info       FileMMAudioWMADetailInfo;
//File Video
	File_MM_Video_AVI_Detail_Info		FileMMVideoAVIDetailInfo;
} File_Media_Info;



