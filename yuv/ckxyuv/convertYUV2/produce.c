#include  <stdio.h>
#include  <stdlib.h>

#define   WIDTH   716//720
#define   HEIGHT  480

#define   COLORy  128
#define   COLORu  128
#define   COLORv  255
#if 0
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;	
	int   i, j;	
//3840x2160xY  ->  3840x2160xYUV420
	inFILE = fopen("y.bin", "rb");
	outFILE = fopen("test3840x2160.yuv", "wb");

	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j < 3840; ++j)
		{
			fputc(fgetc(inFILE), outFILE);
		}
	}	
	
	c = 128;
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j < (3840/2); ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");	
}
#endif
#if 0//error error 3840x2160 64x64
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j, k, t, inFILElen=0, temp = 0, offset = 0;	
//3840x2160xY  ->  3840x2160xYUV420
	inFILE = fopen("y.bin", "rb");
	outFILE = fopen("test3840x2160.yuv", "wb");

	fseek(inFILE, 0, SEEK_END);
	inFILElen = ftell(inFILE);	
	fseek(inFILE, 0, SEEK_SET);
	
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j <60 ; ++j)//3840/64
		{
			offset = (i/64)*0x3C000 + j*0x1000 + (i%64)*0x40;
			fseek(inFILE, offset, SEEK_SET);
			for(k=0; k<64; k++)
			{
			    fputc(fgetc(inFILE), outFILE);
			}			
		}
	}
	
	c = 128;
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j < (3840/2); ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");	
}
#endif
#if 0//error 3840x2160 32x32
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j, k, t, inFILElen=0, temp = 0, offset = 0;	
//3840x2160xY  ->  3840x2160xYUV420
	inFILE = fopen("y.bin", "rb");
	outFILE = fopen("test3840x2160.yuv", "wb");

	fseek(inFILE, 0, SEEK_END);
	inFILElen = ftell(inFILE);	
	fseek(inFILE, 0, SEEK_SET);
	
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j <120 ; ++j)//3840/32
		{
			offset = (i/32)*0x1E000 + j*0x400 + (i%32)*0x20;
			fseek(inFILE, offset, SEEK_SET);
			for(k=0; k<32; k++)
			{
			    fputc(fgetc(inFILE), outFILE);
			}			
		}
	}
	
	c = 128;
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j < (3840/2); ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");	
}
#endif
#if 0
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j, k, t, inFILElen=0, temp = 0, offset = 0;	

	inFILE = fopen("test3840x2160.yuv", "rb");
	outFILE = fopen("3840x2160.yuv", "wb");
	
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j <(3840/2) ; ++j)
		{
			offset = i*3840 + j;
			fseek(inFILE, offset, SEEK_SET);
			fputc(fgetc(inFILE), outFILE);
			offset = i*3840  + (j + 1920);
			fseek(inFILE, offset, SEEK_SET);
			fputc(fgetc(inFILE), outFILE);	
		}
	}

	c = 128;
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j < (3840/2); ++j)
		{
			fputc(c, outFILE);
		}
	}
	
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");	
}
#endif

#if 0//error 3840x2160 16x32
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j, k, t, inFILElen=0, temp = 0, offset = 0;	
//3840x2160xY  ->  3840x2160xYUV420
	inFILE = fopen("y.bin", "rb");
	outFILE = fopen("test3840x2160.yuv", "wb");

	fseek(inFILE, 0, SEEK_END);
	inFILElen = ftell(inFILE);	
	fseek(inFILE, 0, SEEK_SET);
	
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j <240 ; ++j)//3840/16
		{
			offset = (i/32)*0x1E000 + j*0x200 + (i%32)*0x10;
			fseek(inFILE, offset, SEEK_SET);
			for(k=0; k<16; k++)
			{
			    fputc(fgetc(inFILE), outFILE);
			}			
		}
	}
	
	c = 128;
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j < (3840/2); ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");	
}
#endif

#if 1//error 3840x2160 32x16
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j, k, t, inFILElen=0, temp = 0, offset = 0;	
//3840x2160xY  ->  3840x2160xYUV420
	inFILE = fopen("y.bin", "rb");
	outFILE = fopen("test3840x2160.yuv", "wb");

	fseek(inFILE, 0, SEEK_END);
	inFILElen = ftell(inFILE);	
	fseek(inFILE, 0, SEEK_SET);
	
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j <120 ; ++j)//3840/32
		{
			offset = (i/16)*0xF000 + j*0x200 + (i%16)*0x20;
			fseek(inFILE, offset, SEEK_SET);
			for(k=0; k<32; k++)
			{
			    fputc(fgetc(inFILE), outFILE);
			}			
		}
	}
	
	c = 128;
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j < (3840/2); ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");	
}
#endif


#if 0//ok 1920x1080 16x32
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j, k, t, inFILElen=0, temp = 0, offset = 0;	

	inFILE = fopen("y.bin", "rb");
	outFILE = fopen("test1920x1080.yuv", "wb");

	fseek(inFILE, 0, SEEK_END);
	inFILElen = ftell(inFILE);	
	fseek(inFILE, 0, SEEK_SET);
	
	for (i = 0; i < 1080; ++i)
	{
		for (j = 0; j <120 ; ++j)//1920/16
		{
			offset = (i/32)*0xF000 + j*0x200 + (i%32)*0x10;
			fseek(inFILE, offset, SEEK_SET);
			for(k=0; k<16; k++)
			{
			    fputc(fgetc(inFILE), outFILE);
			}			
		}
	}
	
	c = 128;
	for (i = 0; i < 1080; ++i)
	{
		for (j = 0; j < (1920/2); ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");	
}
#endif

#if 0
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j;	
// 716x480  ->  3840x2160 
	inFILE = fopen("test.yuv", "rb");
	outFILE = fopen("test3840x2160.yuv", "wb");

	for (i = 0; i < HEIGHT; ++i)
	{
		for (j = 0; j < WIDTH; ++j)
		{
			c = fgetc(inFILE);
			fputc(c, outFILE);
		}
		fseek(inFILE , i*WIDTH , SEEK_SET);
		for (j = 0; j < WIDTH; ++j)
		{
			c = fgetc(inFILE);
			fputc(c, outFILE);
		}		
		fseek(inFILE , i*WIDTH , SEEK_SET);
		for (j = 0; j < WIDTH; ++j)
		{
			c = fgetc(inFILE);
			fputc(c, outFILE);
		}	
		fseek(inFILE , i*WIDTH , SEEK_SET);
		for (j = 0; j < WIDTH; ++j)
		{
			c = fgetc(inFILE);
			fputc(c, outFILE);
		}	
		fseek(inFILE , i*WIDTH , SEEK_SET);
		for (j = 0; j < WIDTH; ++j)
		{
			c = fgetc(inFILE);
			fputc(c, outFILE);
		}			
		for (j = 0; j < 260; ++j) 
		{
			c = 0;
			fputc(c, outFILE);
		}
	}
	for (i = 0; i < 1680; ++i)
	{
		for (j = 0; j < 3840; ++j)
		{
			fputc(255, outFILE);
		}
	}
	c = 128;
	for (i = 0; i < 2160; ++i)
	{
		for (j = 0; j < (3840/2); ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");	
}
#endif
#if 0
void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j;

	inFILE = fopen("YUV422_[716x480]_Y_NEW.yuv", "rb");
	if (NULL == inFILE) 
	{
		printf("Can not open to read.\n");
		exit(0);
	}
	outFILE = fopen("test.yuv", "wb");

//-------------------------------------------------------------------------------
//get 716x480---720x480
	for (i = 0; i < HEIGHT; ++i)
	{
		for (j = 0; j < WIDTH; ++j)
		{
			c = fgetc(inFILE);
			fputc(c, outFILE);
		}
	}

	c = 128;
	for (i = 0; i < HEIGHT; ++i)
	{
		for (j = 0; j < WIDTH; ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

//-------------------------------------------------------------------------------
// 716x480  ->  720x480 
	inFILE = fopen("test.yuv", "rb");
	outFILE = fopen("test2.yuv", "wb");

	for (i = 0; i < HEIGHT; ++i)
	{
		for (j = 0; j < WIDTH; ++j)
		{
			c = fgetc(inFILE);
			fputc(c, outFILE);
		}

		for (j = 0; j < 4; ++j) 
		{
			c = 0;
			fputc(c, outFILE);
		}
	}

	c = 128;
	for (i = 0; i < HEIGHT; ++i)
	{
		for (j = 0; j < 720; ++j)
		{
			fputc(c, outFILE);
		}
	}
	fclose(inFILE);
	fclose(outFILE);

	printf("Process OK.\n");

//--------------------------------------------------------------------------------------
//YUV  -> yuyv
	inFILE = fopen("E:\\c\\ckxyuv\\convertyuv2\\test2.yuv", "rb");
	outFILE = fopen("E:\\c\\ckxyuv\\convertyuv2\\test3.yuv", "wb");

	for (i = 0; i < HEIGHT; ++i)
	{
		for (j = 0; j < 360; ++j)
		{
			c = fgetc(inFILE);
			fputc(c, outFILE);

			c = 128;
			fputc(c, outFILE);

			c = fgetc(inFILE);
			fputc(c, outFILE);

			c = 128;
			fputc(c, outFILE);
		}
	}

	fclose(inFILE);
	fclose(outFILE);
}
#endif
