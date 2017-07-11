/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/* Email  : sisi.wu@mstarsemi.com                           */
/************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
MPEG2Tool_t Mpeg2Tool;

void MPEG2ToolInit()
{
    memset((void *)&Mpeg2Tool, 0x00, sizeof(MPEG2Tool_t));
}

void MPEG2ToolDeInit()
{
    memset((void *)&Mpeg2Tool, 0x00, sizeof(MPEG2Tool_t));
}

void MPEG2ToolSetInputPath(char* input_path)
{
    strcpy(Mpeg2Tool.input_path, input_path);
    INFO("\ninput path=%s-%s\n", input_path, Mpeg2Tool.input_path);
}

void MPEG2ToolSetOutputPath(char* output_path)
{
    strcpy(Mpeg2Tool.output_path, output_path);
    INFO("\noutput path=%s-%s\n", output_path, Mpeg2Tool.output_path);
}

void MPEG2ToolSetStartTime(char* time)
{
    Mpeg2Tool.start_time = 3;
    INFO("\ntime=%s-%d\n", time, Mpeg2Tool.start_time);
}
