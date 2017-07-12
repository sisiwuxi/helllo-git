/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/* Email  : sisi.wu@mstarsemi.com                           */
/************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"
#include "abend.h"

void MPEG2AbendInit()
{
    abend_duration = 0;
}

void MPEG2AbendDeInit()
{
    abend_duration = 0;
}

void MPEG2AbendStream(char* abend_duration_str)
{
    int AbendDuration = atoi(abend_duration_str);
    INFO("\nabend_duration=%s-%d\n", abend_duration_str, AbendDuration);
    FILE *fp1, *fp2;
    bool has_SLICE = false;
    int i = 0, StartCode = 0, SLICENum=0, SLICEHeaderNum=0;
    int temp, temp_ts_packet[188] = {0};
    float FileDuration = 0.0;
    int AbendStart = 0, AbendStartNum=0, AbendEndNum=0;
#if 0
    int transport_error_indicator, payload_unit_start_indicator, transport_priority;
    int transport_scrambling_control, adaption_field_control, continuity_counter;

    int table_id, section_syntax_indicator;
    int section_length, transport_stream_id, version_number;
    int current_next_indicator, section_number, last_section_number;
    int program_number, network_PID, program_map_PID;
#endif

    //====================open_file================//
    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
        ERR("\nfp1 error open\n");
    if((fp2 = fopen(MPEG2ToolGetOutputPath(),"wb+")) == NULL)
        ERR("\nfp2 error open\n");
    //====================start_time================//
    FileDuration = MPEG2ToolGetDuration(fp1);
    if(MPEG2ToolGetStartTime() > FileDuration)
        AbendStart = 0;
    else
        AbendStart = MPEG2ToolGetStartTime();
    fseek(fp1, 0, SEEK_SET);
    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp == 0x47)
        {
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp1) & B8BIT);
            for(i=0; i<188; i++)
            {
                StartCode = (temp_ts_packet[i]<<24)|(temp_ts_packet[i+1]<<16)|(temp_ts_packet[i+2]<<8)|(temp_ts_packet[i+3]);
                if((StartCode>=SLICE_START_CODE) && (StartCode<=SLICE_END_CODE))
                {
                    SLICENum++;
                    //printf(" 0x%x ", (StartCode & B8BIT));
                }
            }
        }
    }
    AbendStartNum = (AbendStart/FileDuration)*SLICENum;
    AbendEndNum = AbendStartNum + (SLICENum*AbendDuration)/(FileDuration*1000);
    INFO("\nAbendStart=%d, FileDuration=%f, AbendDuration=%d]\n", AbendStart, FileDuration, AbendDuration);
    INFO("\n----[%d,%d,%d]----\n", SLICENum, AbendStartNum, AbendEndNum);
    //====================abend_slice================//
    fseek(fp1, 0, SEEK_SET);
    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp == 0x47)
        {
            has_SLICE = false;
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp1) & B8BIT);
            for(i=0; i<188; i++)
            {
                StartCode = (temp_ts_packet[i]<<24)|(temp_ts_packet[i+1]<<16)|(temp_ts_packet[i+2]<<8)|(temp_ts_packet[i+3]);
                if((StartCode>=SLICE_START_CODE) && (StartCode<=SLICE_END_CODE))
                {
                    has_SLICE = true;
                    SLICEHeaderNum++;
                    //printf(" 0x%x ", (StartCode & B8BIT));
                }
            }
            if(!has_SLICE)
            {
                for(i=0; i<188; i++)
                {
                    fputc((temp_ts_packet[i] & B8BIT), fp2);
                }
            }
            else if(has_SLICE && !(SLICEHeaderNum>=AbendStartNum && SLICEHeaderNum<=AbendEndNum))
            {
                //printf(" %d ", SLICEHeaderNum);
                for(i=0; i<188; i++)
                {
                    fputc((temp_ts_packet[i] & B8BIT), fp2);
                }
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
}




