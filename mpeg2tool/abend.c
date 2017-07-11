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
    abend_duration = atoi(abend_duration_str);
    INFO("\nabend_duration=%s-%d\n", abend_duration_str, abend_duration);
    FILE *fp1, *fp2;
    bool has_GOP = false, has_SLICE = false;
    int i = 0, j = 0, PID, N = 0, GOP_header = 0, PictureStartHeaderNum = 0, SLICEHeaderNum=0;
    int temp, temp_ts_packet[188];
#if 0
    int transport_error_indicator, payload_unit_start_indicator, transport_priority;
    int transport_scrambling_control, adaption_field_control, continuity_counter;

    int table_id, section_syntax_indicator;
    int section_length, transport_stream_id, version_number;
    int current_next_indicator, section_number, last_section_number;
    int program_number, network_PID, program_map_PID;
#endif

    if((fp1 = fopen("1163_2101_2108_590654828_615647547.ts","rb")) == NULL)
        printf("\nfp1 eror open\n");
    if((fp2 = fopen("1163_2101_2108_590654828_615647547_abend.ts","wb")) == NULL)
        printf("\nfp2 eror open\n");
    temp = fgetc(fp1) & B8BIT;

//=====================TS_NUM================//
    int TSNum=0;
    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp == 0x47)
        {
            for(i=1; i<188; i++)
                fgetc(fp1);
            TSNum++;
        }
    }
    fseek(fp1, 0, SEEK_SET);
    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp == 0x47)
        {
            has_GOP = false;
            has_SLICE = false;
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp1) & B8BIT);
            for(i=0; i<188; i++)
            {
                GOP_header = (temp_ts_packet[i]<<24)|(temp_ts_packet[i+1]<<16)|(temp_ts_packet[i+2]<<8)|(temp_ts_packet[i+3]);
                if(PICTURE_START_CODE == GOP_header)
                {
                    has_GOP = true;
                    PictureStartHeaderNum++;
                    i+=4;
                }
                if((GOP_header>=SLICE_START_CODE) && (GOP_header<=SLICE_END_CODE))
                {
                    has_SLICE = true;
                    SLICEHeaderNum++;
                    //printf(" 0x%x ", (GOP_header & B8BIT));
                }
            }
            if(!has_SLICE)
            {
                for(i=0; i<188; i++)
                {
                    fputc((temp_ts_packet[i] & B8BIT), fp2);
                }
            }
            else if(has_SLICE && !((SLICEHeaderNum>=70 && SLICEHeaderNum<=110) || (SLICEHeaderNum>=170 && SLICEHeaderNum<=190) || (SLICEHeaderNum>=270 && SLICEHeaderNum<=280)))
            {
                printf(" %d ", SLICEHeaderNum);
                for(i=0; i<188; i++)
                {
                    fputc((temp_ts_packet[i] & B8BIT), fp2);
                }
            }
        }
    }
    printf("\n----PictureStartHeaderNum=%d-SLICEHeaderNum=%d-----\n", PictureStartHeaderNum, SLICEHeaderNum);
    fclose(fp1);
    fclose(fp2);
}




