#include  <stdio.h>
#include  <stdlib.h>

#define   WIDTH   716//720
#define   HEIGHT  480

#define   COLORy  128
#define   COLORu  128
#define   COLORv  255

void  main(void)
{
	FILE    *inFILE,  *outFILE;
	unsigned char  c;
	int   i, j;

	inFILE = fopen("E:\\c\\ckxyuv\\convertyuv2\\[720x480]_Y_NEW.yuv", "rb");
	if (NULL == inFILE) 
	{
		printf("Can not open to read.\n");
		exit(0);
	}
	outFILE = fopen("E:\\c\\ckxyuv\\convertyuv2\\test.yuv", "wb");

//-------------------------------------------------------------------------------
//get 716x480
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
	inFILE = fopen("E:\\c\\ckxyuv\\convertyuv2\\test.yuv", "rb");
	outFILE = fopen("E:\\c\\ckxyuv\\convertyuv2\\test2.yuv", "wb");

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