#include <stdio.h>
#include <stdlib.h>
#define BIT7 0x080
#define BIT6 0x040
#define BIT5 0x020
#define B5BIT 0x01f
#define B76BIT 0x0c0
#define B54BIT 0x030
#define B4BIT 0x0f
#define B8BIT ((~(~0))|0xff)

void main()
{
    FILE *fp1,*fp2;
    int i = 0, PID, flag = 0;
    int temp, temp_ts_packet[188];
    int transport_error_indicator, payload_unit_start_indicator, transport_priority;
    int transport_scrambling_control, adaption_field_control, continuity_counter;

    if((fp1 = fopen("data2.ts","rb")) == NULL)
        printf("eror open\n");
    if((fp2 = fopen("EIT.ts","wb")) == NULL)
        printf("eror open\n");

    temp = fgetc(fp1) & B8BIT;
    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp == 0x47)
        {
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp1) & B8BIT);
            //printf("temp_ts_packet=%02x %02x %02x %02x\n",temp_ts_packet[0],temp_ts_packet[1],temp_ts_packet[2],temp_ts_packet[3]);

            transport_error_indicator = (temp_ts_packet[1] & BIT7) >> 7;
            payload_unit_start_indicator = (temp_ts_packet[1] & BIT6) >> 6;
            transport_priority = (temp_ts_packet[1] & BIT5) >> 5;
            /*printf("transport_error_indicator=%d,payload_unit_start_indicator=%d,transport_priority=%d\n",\
            		transport_error_indicator,payload_unit_start_indicator,transport_priority);*/

            PID = (temp_ts_packet[1] & B5BIT) << 8;
            PID |= temp_ts_packet[2];
            //printf("PID=%d ",PID);

            transport_scrambling_control = (temp_ts_packet[3] & B76BIT) >> 6;
            adaption_field_control = (temp_ts_packet[3] & B54BIT) >> 4;
            continuity_counter = temp_ts_packet[3] & B4BIT;
            /*printf("transport_scrambling_control=%d,adaption_field_control=%d,continuity_counter=%d\n",\
            		transport_scrambling_control,adaption_field_control,continuity_counter);*/
            if(PID == 0x0012)
            {
                for(i=0; i<188; i++)
                    fputc(temp_ts_packet[i],fp2);
                flag++;
            }
        }
    }
    printf("flag=%d\n",flag);
    fclose(fp1);
    fclose(fp2);
}