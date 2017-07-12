#include <stdio.h>
#include <stdlib.h>
#define BIT7 0x080
#define BIT6 0x040
#define BIT5 0x020
#define B5BIT 0x01f
#define B76BIT 0x0c0
#define B54BIT 0x030
#define B4BIT 0x0f
#define B12345BIT 0x3e
#define B0BIT 0x01
#define B8BIT ((~(~0))|0xff)
#define PMT_PID (0x0111)
#define PID_MAX_NUM (200)

int Found_PID[PID_MAX_NUM] = {0};

int Return_PID_Num(int PID)
{
    int i = 0, Return_Value = 0;
    for(i = 0; i < PID_MAX_NUM; i++)
    {
        if(!Found_PID[i])
        {
            Return_Value = i;
            break;
        }
    }
    for(i = 0; i <= Return_Value; i++)
    {
        if(PID == Found_PID[i])
        {
            Return_Value = PID_MAX_NUM;
            break;
        }
    }
    return Return_Value;
}

void main()
{
    FILE *fp1;
    FILE *fp2;
    int i = 0, j = 0, k = 0, PID, N = 0;
    int temp, temp_ts_packet[188];
    int transport_error_indicator, payload_unit_start_indicator, transport_priority;


    if((fp1 = fopen("AAC_51_256K_101207A(DMS00374338).ts","rb")) == NULL)
        printf("eror fp1 open\n");
    if((fp2 = fopen("PID_0X0111  orignal.ts","wb")) == NULL)
        printf("eror fp2 open\n");

    temp = fgetc(fp1) & B8BIT;
    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp == 0x47)
        {
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp1) & B8BIT);
//			printf("temp_ts_packet=%02x %02x %02x %02x\n",temp_ts_packet[0],temp_ts_packet[1],temp_ts_packet[2],temp_ts_packet[3]);

            transport_error_indicator = (temp_ts_packet[1] & BIT7) >> 7;
            payload_unit_start_indicator = (temp_ts_packet[1] & BIT6) >> 6;
            transport_priority = (temp_ts_packet[1] & BIT5) >> 5;
            /*			printf("transport_error_indicator=%d,payload_unit_start_indicator=%d,transport_priority=%d\n",\
            					transport_error_indicator,payload_unit_start_indicator,transport_priority);
            */
            PID = (temp_ts_packet[1] & B5BIT) << 8;
            PID |= temp_ts_packet[2];
            //printf("PID=%d ",PID);
            k = Return_PID_Num(PID);
            Found_PID[k] = PID;
            if(PMT_PID == PID)
            {
                for(j=0; j<188; j++)
                    fputc(temp_ts_packet[j], fp2);
            }
        }
    }
    printf("k=%d\n ",k);
    for(i = 0; i < k; i++)
    {
        printf("PID[%d]=0x%x ",i, Found_PID[i]);
    }

    fclose(fp1);
    fclose(fp2);
}