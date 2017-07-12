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

void main()
{
    FILE *fp1;
    int i = 0, PID, N = 0, N1 = 0, N2 = 0, offset = 0;
    int temp, temp_ts_packet[188];
    int transport_error_indicator, payload_unit_start_indicator, transport_priority;
    int transport_scrambling_control, adaption_field_control, continuity_counter;

    int table_id, section_syntax_indicator;
    int section_length, program_number, version_number;
    int current_next_indicator, section_number, last_section_number;
    int PCR_PID, program_info_length;
    int stream_type, elementary_PID, ES_info_length;

    if((fp1 = fopen("5-PID_0X21.ts","rb")) == NULL)
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
            printf("temp_ts_packet=%02x %02x %02x %02x\n",temp_ts_packet[0],temp_ts_packet[1],temp_ts_packet[2],temp_ts_packet[3]);

            transport_error_indicator = (temp_ts_packet[1] & BIT7) >> 7;
            payload_unit_start_indicator = (temp_ts_packet[1] & BIT6) >> 6;
            transport_priority = (temp_ts_packet[1] & BIT5) >> 5;
            printf("transport_error_indicator=%d,payload_unit_start_indicator=%d,transport_priority=%d\n",\
                   transport_error_indicator,payload_unit_start_indicator,transport_priority);

            PID = (temp_ts_packet[1] & B5BIT) << 8;
            PID |= temp_ts_packet[2];
            printf("PID=%d ",PID);

            transport_scrambling_control = (temp_ts_packet[3] & B76BIT) >> 6;
            adaption_field_control = (temp_ts_packet[3] & B54BIT) >> 4;
            continuity_counter = temp_ts_packet[3] & B4BIT;
            printf("transport_scrambling_control=%d,adaption_field_control=%d,continuity_counter=%d\n",\
                   transport_scrambling_control,adaption_field_control,continuity_counter);

            /*----------parse PMT---------*/
            printf("\n----------parse PMT_2---------\n");


            table_id = temp_ts_packet[5];
            printf("table_id=%d ",table_id);
            section_syntax_indicator = (temp_ts_packet[6] & BIT7) >> 7;
            printf("section_syntax_indicator=%d ",section_syntax_indicator);
            section_length = (temp_ts_packet[6] & B4BIT) << 8;
            section_length |= temp_ts_packet[7];
            printf("section_length=%d ",section_length);

            program_number = temp_ts_packet[8] << 8;
            program_number |= temp_ts_packet[9];
            printf("program_number=0x%x ",program_number);

            version_number = (temp_ts_packet[10] & B12345BIT) >> 1;
            printf("version_number=%d ",version_number);
            current_next_indicator = temp_ts_packet[10] & B0BIT;
            printf("current_next_indicator=%d ",current_next_indicator);
            section_number = temp_ts_packet[11];
            printf("section_number=%d ",section_number);
            last_section_number = temp_ts_packet[12];
            printf("last_section_number=%d ",last_section_number);
            PCR_PID = (temp_ts_packet[13] & B5BIT) << 8;
            PCR_PID |= temp_ts_packet[14];
            printf("PCR_PID=%d ",PCR_PID);
            program_info_length = (temp_ts_packet[15] & B4BIT) << 8;
            program_info_length |= temp_ts_packet[16];
            printf("program_info_length=%d ",program_info_length);
            N = program_info_length;
            printf("N=%d ",N);
            offset = N;
            N1 = (section_length - 12 - offset) / 5;/*maybe not this value,depend on N2*/
            printf("N1=%d ",N1);
            for(i=0; i<N1; i++)
            {
                if((offset + 17 + i*5 -4) >= section_length)
                    break;
                stream_type = temp_ts_packet[offset + 17 + i*5];
                printf("stream_type=%d ",stream_type);
                elementary_PID = (temp_ts_packet[offset + 18 + i*5] & B5BIT) << 8;
                elementary_PID |= temp_ts_packet[offset + 19 + i*5];
                printf("elementary_PID=%d ",elementary_PID);
                ES_info_length = (temp_ts_packet[offset + 20 + i*5] & B4BIT) << 8;
                ES_info_length |= temp_ts_packet[offset + 21 + i*5];
                printf("ES_info_length=%d ",ES_info_length);
                N2 = ES_info_length;
                printf("N2=%d ",N2);
                if(N2 > 0)
                    offset += N2;
            }
            printf("\n----------end of parse PMT_2---------\n");
        }
    }
    fclose(fp1);
}