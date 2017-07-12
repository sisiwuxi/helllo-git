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

char* MPEG2ToolGetInputPath()
{
    return Mpeg2Tool.input_path;
}

void MPEG2ToolSetOutputPath(char* output_path)
{
    strcpy(Mpeg2Tool.output_path, output_path);
    INFO("\noutput path=%s-%s\n", output_path, Mpeg2Tool.output_path);
}

char* MPEG2ToolGetOutputPath()
{
    return Mpeg2Tool.output_path;
}

void MPEG2ToolSetStartTime(char* time)
{
    if(8 == strlen(time))/*00:00:00*/
    {
        int hour=0, min=0, sec=0;
        char hour_str[3]={0}, min_str[3]={0}, sec_str[3]={0};

        hour_str[0]=time[0];
        hour_str[1]=time[1];
        hour=atoi(hour_str);
        min_str[0]=time[3];
        min_str[1]=time[4];
        min=atoi(min_str);
        sec_str[0]=time[6];
        sec_str[1]=time[7];
        sec=atoi(sec_str);
        INFO("\n[%s-%d] [%s-%d] [%s-%d]\n", hour_str, hour, min_str, min, sec_str, sec);
        Mpeg2Tool.start_time = hour*60*60 + min*60 + sec;
    }
    else
    {
        Mpeg2Tool.start_time = 0;
    }
    INFO("\ntime=%s-%d\n", time, Mpeg2Tool.start_time);
}

int MPEG2ToolGetStartTime()
{
    return Mpeg2Tool.start_time;
}

float MPEG2ToolGetDuration(FILE *fp)
{
    if(NULL==fp)
        return 0;
    int i=0, StartCode=0, temp=0, temp_ts_packet[188] = {0};
    float input_file_duration=0.0;
    int TSTotal=0, TSNum=0, TSNum_FPES=0, TSNum_EPES=0;
    int PTS_32_30=0, PTS_29_15=0, PTS_14_0=0, PTS_Start=0, PTS_End=0;
//==============calculate_the_total_number============//
    fseek(fp, 0, SEEK_SET);
    while(!feof(fp))
    {
        temp = fgetc(fp) & B8BIT;
        if(temp == 0x47)
        {
            for(i=1; i<188; i++)
                fgetc(fp);
            TSTotal++;
        }
    }
//==============obtain_the_location_of_first_PES============//
    fseek(fp, 0, SEEK_SET);
    TSNum=0;
    while(!feof(fp))
    {
        temp = fgetc(fp) & B8BIT;
        if(temp == 0x47)
        {
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp) & B8BIT);
            for(i=0; i<188; i++)
            {
                StartCode = (temp_ts_packet[i]<<24)|(temp_ts_packet[i+1]<<16)|(temp_ts_packet[i+2]<<8)|(temp_ts_packet[i+3]);
                if((StartCode>=PES_START_CODE) && (StartCode<=PES_END_CODE))
                {
		    if((StartCode & B8BIT)>>4 != 0x0E)
		    {
                        TSNum_FPES=0;
		    }
		    else
		    {
                        TSNum_FPES = TSNum;
                        PTS_32_30 = (temp_ts_packet[i+9] & BIT123)>>1;
                        PTS_29_15 = (temp_ts_packet[i+10]<<8 | temp_ts_packet[i+11])>>1;
                        PTS_14_0 = (temp_ts_packet[i+12]<<8 | temp_ts_packet[i+13])>>1;
                        PTS_Start = (PTS_32_30<<30)|(PTS_29_15<<15)|PTS_14_0;
                        INFO("\nTSNum_FPES=%d PTS[0x%x-0x%x-0x%x=0x%x] 0x%x\n", TSNum_FPES, PTS_32_30, PTS_29_15, PTS_14_0, PTS_Start, ((StartCode & B8BIT)>>4));
		    }
                }
            }
            TSNum++;
        }
	if(TSNum_FPES!=0)
            break;
    }
//==============obtain_the_location_of_last_PES============//
    fseek(fp, (TSTotal-10)*188, SEEK_SET);
    TSNum=0;
    while(!feof(fp))
    {
        temp = fgetc(fp) & B8BIT;
        if(temp == 0x47)
        {
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp) & B8BIT);
            for(i=0; i<188; i++)
            {
                StartCode = (temp_ts_packet[i]<<24)|(temp_ts_packet[i+1]<<16)|(temp_ts_packet[i+2]<<8)|(temp_ts_packet[i+3]);
                if((StartCode>=PES_START_CODE) && (StartCode<=PES_END_CODE))
                {
		    if((StartCode & B8BIT)>>4 != 0x0E)
		    {
                        TSNum_FPES=0;
		    }
		    else
		    {
                        TSNum_EPES = (TSTotal-10)+TSNum;
                        PTS_32_30 = (temp_ts_packet[i+9] & BIT123)>>1;
                        PTS_29_15 = (temp_ts_packet[i+10]<<8 | temp_ts_packet[i+11])>>1;
                        PTS_14_0 = (temp_ts_packet[i+12]<<8 | temp_ts_packet[i+13])>>1;
                        PTS_End = (PTS_32_30<<30)|(PTS_29_15<<15)|PTS_14_0;
                        INFO("\nTSNum_EPES=%d PTS[0x%x-0x%x-0x%x=0x%x] 0x%x\n", TSNum_EPES, PTS_32_30, PTS_29_15, PTS_14_0, PTS_End, ((StartCode & B8BIT)>>4));
		    }
                }
            }
            TSNum++;
        }
    }
    input_file_duration = (PTS_End - PTS_Start)/90000.0;
    INFO("\nTSTotal[%d-%d-%d]=%f\n", TSTotal, TSNum_FPES, TSNum_EPES, input_file_duration);
    return input_file_duration;
}

