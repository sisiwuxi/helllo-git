#include <stdio.h>
#include <stdlib.h>
#define BIT7 0x080
#define BIT6 0x040
#define BIT5 0x020
#define BIT4 0x010
#define B5BIT 0x01f
#define B76BIT 0x0c0
#define B54BIT 0x030
#define B4BIT 0x0f
#define BB4BIT 0x0f0
#define B12345BIT 0x3e
#define B567BIT 0x0e0
#define B1BIT 0x02
#define B0BIT 0x01
#define B8BIT ((~(~0))|0xff)
#define BCD2Dec(x) (((x>>4) * 10) + (x&0x0F))

int SDT_CON[1024], TS_NUM = 0, content_len = 0, OFFSET = 0, DESCRIPTOR_LENGTH = 0, section_length, descriptors_loop_length;

void Parse_second_N(void)
{
    //printf("\n=======================Parse_second_N=========================\n");
    int descriptor_tag, descriptor_length;
    int i, N = 0;

    descriptor_tag = SDT_CON[OFFSET];
    //printf("descriptor_tag=%x ",descriptor_tag);
    descriptor_length = SDT_CON[OFFSET + 1];
    //printf("descriptor_length=%d ",descriptor_length);
    if(descriptor_tag == 0x48)/*service_descriptor*/
    {
        int service_type, service_provider_length, service_name_length;
        char service_provider[64], service_name[64];
        service_type = SDT_CON[OFFSET + 2];
        //printf("service_type=%x ",service_type);
        service_provider_length = SDT_CON[OFFSET + 3];
        //printf("service_provider_length=%d ",service_provider_length);
        N = service_provider_length;
        for(i=0; i<N; i++)
            service_provider[i] = SDT_CON[OFFSET + 4 + i];
        service_provider[i] = 0;
        printf("service_provider=%s ",service_provider);

        service_name_length = SDT_CON[OFFSET + 4 + N];
        //printf("service_name_length=%d ",service_name_length);
        N = service_name_length;
        for(i=0; i<N; i++)
            service_name[i] = SDT_CON[OFFSET + 5 + i];
        service_name[i] = 0;
        printf("service_name=%s ",service_name);

    }
    if(descriptor_tag == 0x47)/*bouquet_name_descriptor*/
    {
        char BouquetName[80];
        //printf("BouquetNameLength=%d ",BouquetNameLength);
        N = descriptor_length;
        for(i=0; i<N; i++)
            BouquetName[i] = SDT_CON[OFFSET + 2 + i];
        BouquetName[i] = 0;
        printf("BouquetName=%s",BouquetName);
    }
    if(descriptor_tag == 0x4B)/*NVOD_reference_descriptor*/
    {
        int u16OnId, u16TsId, u16SrvId;
        N = descriptor_length;
        for (i= 2; i< N; i+= 6)
        {
            u16TsId = SDT_CON[OFFSET + i] << 8;
            u16TsId |= SDT_CON[OFFSET + 1 + i];
            printf("\nu16TsId=0x%x ",u16TsId);
            u16OnId = SDT_CON[OFFSET + 2 + i] << 8;
            u16OnId |= SDT_CON[OFFSET + 3 + i];
            printf("u16OnId=0x%x ",u16OnId);
            u16SrvId = SDT_CON[OFFSET + 4 + i] << 8;
            u16SrvId |= SDT_CON[OFFSET + 5 + i];
            printf("u16SrvId=0x%x ",u16SrvId);
        }
    }
    if(descriptor_tag == 0x4c)/*time_shifted_service_descriptor*/
    {
        int u16RefServiceId;
        u16RefServiceId = SDT_CON[OFFSET + 2] << 8;
        u16RefServiceId |= SDT_CON[OFFSET + 3];
        printf("u16RefServiceId=0x%x ",u16RefServiceId);
    }
    DESCRIPTOR_LENGTH += descriptor_length + 1;
    OFFSET += descriptor_length + 2;
    //printf("OFFSET=%d\n",OFFSET);

    //printf("\n=======================end of Parse_second_N=========================\n");
}

void Parse_first_N(void)
{
    //printf("\n=======================Parse_first_N=========================\n");
    int service_id, EIT_schedule_flag, EIT_present_following_flag;
    int running_status, free_CA_mode;
    service_id = SDT_CON[OFFSET] << 8;
    service_id |= SDT_CON[OFFSET + 1];
    printf("\nservice_id=0x%x ",service_id);
    EIT_schedule_flag = (SDT_CON[OFFSET + 2] & B1BIT) >>1;
    //printf("EIT_schedule_flag=%d ",EIT_schedule_flag);
    EIT_present_following_flag = SDT_CON[OFFSET + 2] & B0BIT;
    //printf("EIT_present_following_flag=%d ",EIT_present_following_flag);
    running_status = (SDT_CON[OFFSET + 3] & B567BIT) >> 5;
    //printf("running_status=%d ",running_status);
    free_CA_mode = (SDT_CON[OFFSET + 3] & BIT4) >> 4;
    //printf("free_CA_mode=%d ",free_CA_mode);
    descriptors_loop_length = (SDT_CON[OFFSET + 3] & B4BIT) << 8;
    descriptors_loop_length |= SDT_CON[OFFSET + 4];
    //printf("descriptors_loop_length=%d ",descriptors_loop_length);
    OFFSET += 5;
    //printf("OFFSET=%d ",OFFSET);
    DESCRIPTOR_LENGTH = 0;
    while(DESCRIPTOR_LENGTH < (descriptors_loop_length - 4 + 1))
        Parse_second_N();
    //printf("\n=======================end of Parse_first_N=========================\n");
}

void Parse_SDT_CON()
{
    FILE *fp2;
    int i;
    int transport_stream_id, version_number, current_next_indicator;
    int section_number, last_section_number, original_network_id;
    printf("\n=======================Parse_SDT_CON=========================\n");
    if((fp2 = fopen("SDT-ACT.txt","rb")) == NULL)
        printf("eror open2\n");
    for(i=0; i<content_len; i++)
        SDT_CON[i] = fgetc(fp2);
    transport_stream_id = SDT_CON[8] << 8;
    transport_stream_id |= SDT_CON[9];
    printf("transport_stream_id=0x%x ",transport_stream_id);
    if(transport_stream_id == 0xc)
        printf("there need attention\n");
    version_number = (SDT_CON[10] & 0x3e) >> 1;
    //printf("version_number=%d ",version_number);
    current_next_indicator = SDT_CON[10] & B0BIT;
    //printf("current_next_indicator=%d ",current_next_indicator);
    section_number = SDT_CON[11];
    //printf("section_number=%d ",section_number);
    last_section_number = SDT_CON[12];
    //printf("last_section_number=%d ",last_section_number);
    original_network_id = SDT_CON[13] << 8;
    original_network_id |= SDT_CON[14];
    printf("original_network_id=0x%x ",original_network_id);
    OFFSET = 16;
    //printf("OFFSET=%d ",OFFSET);
    while(OFFSET < (section_length - 4 + 1))
        Parse_first_N();

    printf("\n=======================end of Parse_SDT_CON=========================\n");
}


void main()
{
    FILE *fp1, *fp2;
    char flag = 1;
    int i, j, PID = 0;
    int temp, temp_ts_packet[188];
    int transport_error_indicator, payload_unit_start_indicator, transport_priority;
    int transport_scrambling_control, adaption_field_control, continuity_counter;
    int table_id;


    if((fp1 = fopen("SDT-255.txt","rb")) == NULL)
        printf("eror open1\n");

    while(!feof(fp1))//for(k=0; k<2; k++)//
    {
        printf("\n ******************************************************************** \n");
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
            /*			printf("transport_scrambling_control=%d,adaption_field_control=%d,continuity_counter=%d\n",\
            					transport_scrambling_control,adaption_field_control,continuity_counter);*/
            table_id = temp_ts_packet[5];
            /*=============================PARSE_SDT_Content()=============================*/
            //if(table_id == 0x46)
            {
                int section_syntax_indicator;
                section_syntax_indicator =  (temp_ts_packet[6] & BIT7) >> 7;
                //printf("section_syntax_indicator=%d ",section_syntax_indicator);
                section_length = temp_ts_packet[6] & B4BIT;
                section_length |= temp_ts_packet[7];
                //printf("section_length=%d ",section_length);
                if(section_length <= 1021)
                {
                    if(section_length <= (188 - 8))
                        TS_NUM = 1;
                    else
                        TS_NUM = 1 + (section_length - (188 - 8) + (188 - 4)) / (188 - 4);
                    //printf("TS_NUM=%d ",TS_NUM);
                }
                if(TS_NUM >= 1)
                {
                    content_len = 0;
                    for(i=0; i<188; i++)
                        SDT_CON[content_len++] = temp_ts_packet[i];
                    for(i=1; i<TS_NUM && !feof(fp1); i++)
                    {
                        temp = fgetc(fp1) & B8BIT;
                        if(temp == 0x47)
                        {
                            temp_ts_packet[0] = temp;
                            for(j=1; j<188; j++)
                                temp_ts_packet[j] = (fgetc(fp1) & B8BIT);
                        }
                        for(j=4; j<188; j++)
                            SDT_CON[content_len++] = temp_ts_packet[j];
                    }
                }
                //printf("content_len=%d ",content_len);
                if((fp2 = fopen("SDT-ACT.txt","wb")) == NULL)
                    printf("eror open2\n");
                for(i=0; i<content_len; i++)
                    fputc(SDT_CON[i], fp2);
                fclose(fp2);
                Parse_SDT_CON();
            }
        }
    }
    fclose(fp1);
}