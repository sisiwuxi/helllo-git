/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/common.h"
#include "include/abend.h"

void MPEG2AbendInit()
{
    abend_duration = 0;
}

void MPEG2AbendDeInit()
{
    abend_duration = 0;
}

void MPEG2AbendStream(char* abend_duration_str, char* Codec_Type)
{
    int AbendDuration = atoi(abend_duration_str);
    int CodecType = atoi(Codec_Type);
    INFO("\nMPEG2AbendStream[%d-%d]\n", AbendDuration, CodecType);
    FILE *fp1, *fp2;
    bool has_SLICE = false;
    int i = 0, StartCode = 0, SLICENum=0, SLICEHeaderNum=0;
    int temp, temp_ts_packet[188] = {0};
    float FileDuration = 0.0;
    int AbendStart = 0, AbendStartNum=0, AbendEndNum=0;
    //====================open_file================//
    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
        ERR("\nfp1 error open\n");
    if((fp2 = fopen(MPEG2ToolGetOutputPath(),"wb+")) == NULL)
        ERR("\nfp2 error open\n");
    if(0 == CodecType)//slice
    {
        //====================start_time================//
        FileDuration = MPEG2ToolGetDuration();
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
        if(AbendStartNum < 0)
            AbendStartNum = 0;
        AbendEndNum = AbendStartNum + (SLICENum*AbendDuration)/(FileDuration*1000);
        if(AbendEndNum < 0)
            AbendEndNum = 0;
        DEBUG("\nAbendStart=%d, FileDuration=%f, AbendDuration=%d]\n", AbendStart, FileDuration, AbendDuration);
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
                        break;
                    }
                }
                if(!has_SLICE)
                {
                    for(i=0; i<188; i++)
                    {
                        fputc((temp_ts_packet[i] & B8BIT), fp2);
                    }
                }
                //else if(has_SLICE && !((SLICEHeaderNum>=70 && SLICEHeaderNum<=110) || (SLICEHeaderNum>=170 && SLICEHeaderNum<=190) || (SLICEHeaderNum>=270 && SLICEHeaderNum<=285)))
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
    }
    else if(1 == CodecType)//NALType
    {
        bool isMatch = false;
        int StartCode = 0, NALType = 0, MatchTimes=0;
        fseek(fp1, 0, SEEK_SET);
        while(!feof(fp1))
        {
            temp = fgetc(fp1) & B8BIT;
            if(temp == 0x47)
            {
                temp_ts_packet[0] = temp;
                for(i=1; i<188; i++)
                    temp_ts_packet[i] = (fgetc(fp1) & B8BIT);
                isMatch = false;
                for(i=0; i<188; i++)
                {
                    //StartCode = (temp_ts_packet[i]<<24)|(temp_ts_packet[i+1]<<16)|(temp_ts_packet[i+2]<<8)|(temp_ts_packet[i+3]);
                    StartCode = (temp_ts_packet[i]<<16)|(temp_ts_packet[i+1]<<8)|(temp_ts_packet[i+2]);
                    if(NAL_START_CODE == StartCode)
                    {
                        NALType = (temp_ts_packet[i+3] & BIT123456)>>1;
                        if(NALType>=16 && NALType<=23 && !isMatch)
                        {
                            isMatch=true;
                            MatchTimes++;
                            INFO("  [%d-%d-%d] ", MatchTimes, NALType, ftell(fp1));							
                        }
                    }
                }
            }
            if(!isMatch)
            {
                for(i=0; i<188; i++)
                {
                    fputc((temp_ts_packet[i] & B8BIT), fp2);
                }
            }
            else if(isMatch && MatchTimes!=2)
            {
                //INFO("  [%d-%d-%d] ", MatchTimes, NALType, ftell(fp1));
                for(i=0; i<188; i++)
                {
                    fputc((temp_ts_packet[i] & B8BIT), fp2);
                }
            }
	     else
            {
                //INFO("\n  [%d-%d-%d] jump=======\n", MatchTimes, NALType, ftell(fp1));
                fseek(fp1, (ftell(fp1) + 188*4000), SEEK_SET);
            }		 	
        }
    }
    fclose(fp1);
    fclose(fp2);
}




