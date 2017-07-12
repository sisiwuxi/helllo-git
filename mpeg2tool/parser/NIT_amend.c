#include <stdio.h>
#include <stdlib.h>

#define B5BIT 0x01f
#define B8BIT ((~(~0))|0xff)
#define START (0xBC0)
#define NIT_PID (0x0010)

void main()
{
    FILE *fp1, *fp2;
    int i = 0, j = 0, PID = 0;
    int temp, temp_ts_packet[188] = {0};

    if((fp1 = fopen("simple_vid.mux","rb")) == NULL)
    {
        printf("fp1 error open\n");
    }
    if((fp2 = fopen("simple_vid_NIT.mux","wb")) == NULL)
    {
        printf("fp2 error open\n");
    }
    //jump to original TS data
    if(START)
    {
        for(i = 0; i < START; i++)
        {
            fputc(fgetc(fp1), fp2);
        }
    }

    while(!feof(fp1))
        //for(j = 0; j < 2; j++)
    {
        temp = fgetc(fp1) & B8BIT;
        //printf("temp = 0x%x ",temp);
#if 1
        if(temp == 0x47)
        {
            fputc(temp, fp2);
            temp_ts_packet[1] = fgetc(fp1);
            temp_ts_packet[2] = fgetc(fp1);
            fputc(temp_ts_packet[1], fp2);
            fputc(temp_ts_packet[2], fp2);
            PID = (temp_ts_packet[1] & B5BIT) << 8;
            PID |= temp_ts_packet[2];
            //printf("PID=0x%x \n",PID);
            if(NIT_PID == PID)
            {
                for(i = 0; i < 0x1f; i++)
                {
                    fputc((fgetc(fp1) & B8BIT), fp2);
                }
                for(i = 0; i < 0x04; i++)
                {
                    fgetc(fp1);
                }
                fputc(0x0B, fp2);
                fputc(0x06, fp2);
                fputc(0x18, fp2);
                fputc(0x00, fp2);
                for(i = 0; i < 0x04; i++)
                {
                    fputc((fgetc(fp1) & B8BIT), fp2);
                }
                for(i = 0; i < 0x04; i++)
                {
                    fgetc(fp1);
                }
                fputc(0x00, fp2);
                fputc(0x68, fp2);
                fputc(0x75, fp2);
                fputc(0x00, fp2);
                for(i = 0; i < 0x04; i++)
                {
                    fputc((fgetc(fp1) & B8BIT), fp2);
                }
                for(i = 0; i < 0x8a; i++)//188-50 = 138
                {
                    fputc((fgetc(fp1) & B8BIT), fp2);
                }
            }
            else
            {
                for(i=1; i<186; i++)
                {
                    fputc((fgetc(fp1) & B8BIT), fp2);
                }
            }
        }
#endif
    }
    fclose(fp1);
    fclose(fp2);
}
