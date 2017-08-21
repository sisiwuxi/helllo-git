/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/common.h"

YUVTool_t YUVTool;

void YUVToolInit()
{
    memset((void *)&YUVTool, 0x00, sizeof(YUVTool_t));
    return;
}

void YUVToolDeInit()
{
    memset((void *)&YUVTool, 0x00, sizeof(YUVTool_t));
    return;
}

void YUVToolSetInputPath(char* input_path)
{
    strcpy(YUVTool.input_path, input_path);
    DEBUG("\ninput path=%s-%s\n", input_path, YUVTool.input_path);
}

char* YUVToolGetInputPath()
{
    return YUVTool.input_path;
}

void YUVToolSetOutputPath(char* output_path)
{
    strcpy(YUVTool.output_path, output_path);
    DEBUG("\noutput path=%s-%s\n", output_path, YUVTool.output_path);
}

char* YUVToolGetOutputPath()
{
    return YUVTool.output_path;
}

void YUVToolNewFile(char* ColorType, char* Width, char* Height, char* ColorY, char* ColorU, char* ColorV)
{
    int UV_len = 0, width = 0,  height = 0;
    char output_path[PATH_MAX_NUM] = {0};
    FILE *outFILE=NULL;
    int i, j;	
    int color_y = COLORy, color_u = COLORu, color_v = COLORv;	

    width = atoi(Width);
    height = atoi(Height);

    if(!strcmp(ColorType,"YUV420"))
    {
        UV_len = width*height/2;
    }
    else if(!strcmp(ColorType,"YUV422"))
    {
        UV_len = width*height;
    }
    else if(!strcmp(ColorType,"YUV444"))
    {
        UV_len = width*height*2;
    }
    strcpy(output_path, YUVToolGetOutputPath());
    if(0==strlen(output_path))
    {
        sprintf(output_path, "%dx%d_%s.yuv", width, height, ColorType);
    }

    outFILE = fopen(output_path, "wb");
    if (NULL == outFILE)
    {
        ERR("Can not open to write.\n");
        exit(0);
    }

#if 0
0	0	0  --green
128 128 128--gray
255 128 128--white
0     128 128--black
255 255 255--rose
#endif
    color_y = atoi(ColorY);
    color_u = atoi(ColorU);
    color_v = atoi(ColorV);

    for (i = 0; i < height; ++i)
    {
        for (j = 0; j < width; ++j) 
	 {
            fputc(color_y, outFILE);
        }
    }
    printf("\n UV_len =%d \n", UV_len);	
    for (i = 0; i < UV_len/2; ++i)
    {
        fputc(color_u, outFILE);		
    }
    for (i = 0; i < UV_len/2; ++i)
    {
        fputc(color_v, outFILE);		
    }	
    if (outFILE)
    {
        fclose(outFILE);
	 outFILE=NULL;
    }
}



