#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int flag = 0;
void printf_txt(char *temp)
{
    FILE *fp3;
    if((fp3 = fopen("EIT-printf.txt","ab")) == NULL)
        printf("eror open3\n");
    else
    {
        fprintf(fp3,"%s",temp);
        fprintf(fp3,"%c%c",13,10);
        fclose(fp3);
    }
}

void Parse_second_N(void)
{
    //printf("\n=======================Parse_second_N=========================\n");
    int descriptor_tag, descriptor_length;
    int i, N = 0;

    descriptor_tag = SDT_CON[OFFSET];
    //printf("descriptor_tag=%x ",descriptor_tag);
    descriptor_length = SDT_CON[OFFSET + 1];
    //printf("descriptor_length=%d ",descriptor_length);
    if(descriptor_tag == 0x4D)/* short_event_descriptor*/
    {
        int ISO639_2_language_code;
        int event_name_length, text_length;
        char event_name_char[64], text_char[64];
        ISO639_2_language_code = SDT_CON[OFFSET + 2] << 16;
        ISO639_2_language_code |= SDT_CON[OFFSET + 3] << 8;
        ISO639_2_language_code |= SDT_CON[OFFSET + 4];
        //printf("ISO639_2_language_code=0x%x ",ISO639_2_language_code);
        event_name_length = SDT_CON[OFFSET + 5];
        //printf("event_name_length=%d ",event_name_length);

        for(i=0; i<event_name_length; i++)
            event_name_char[i] = SDT_CON[OFFSET + 6 + i];
        event_name_char[i] = 0;
        if(strlen(event_name_char) && flag)
            //printf("event_name_char=%s ",event_name_char);
            printf_txt(event_name_char);

        text_length = SDT_CON[OFFSET + 6 + event_name_length];
        //printf("text_length=%d ",text_length);

        for(i=0; i<text_length; i++)
            text_char[i] = SDT_CON[OFFSET + 6 + event_name_length + i];
        text_char[i] = 0;
        if(strlen(text_char) && flag)
            //printf("text_char=%s ",text_char);
            printf_txt(text_char);
    }
    else if(descriptor_tag == 0x54)/* content_descriptor*/
    {
        int N = 0, i;
        int Content_nibble_level1 = 0, Content_nibble_level2 = 0;
        int user_nibble1 = 0, user_nibble2 = 0;

        N = descriptor_length / 2;
        for(i=0; i<N; i++)
        {
            Content_nibble_level1 = SDT_CON[OFFSET + i*2 + 2] >> 4;
            Content_nibble_level2 = SDT_CON[OFFSET + i*2 + 2] & 0x0F;
            printf("\nContent_nibble_level1 = %d, Content_nibble_level2 = %d\n",Content_nibble_level1, Content_nibble_level2);
            user_nibble1 = SDT_CON[OFFSET + i*2 + 3] >> 4;
            user_nibble2 = SDT_CON[OFFSET + i*2 + 3] & 0x0F;
            printf("\nuser_nibble1 = %d, user_nibble2 = %d\n",user_nibble1, user_nibble2);
        }
    }
    DESCRIPTOR_LENGTH += descriptor_length + 2;
    OFFSET += descriptor_length + 2;
    //printf("OFFSET=%d\n",OFFSET);

    //printf("\n=======================end of Parse_second_N=========================\n");
}

void Parse_first_N(void)
{
    //printf("\n=======================Parse_first_N=========================\n");
    int event_id;
    int start_time_date, start_time_time, duration;
    int running_status, free_CA_mode, descriptors_loop_length;
    event_id = SDT_CON[OFFSET] << 8;
    event_id |= SDT_CON[OFFSET + 1];
    if(flag)
    {
        //printf("\nevent_id=0x%x ",event_id);
        char temp[10];
        sprintf(temp,"%d ",event_id);
        printf_txt(temp);
    }
    start_time_date = SDT_CON[OFFSET + 2] << 8;
    start_time_date |= SDT_CON[OFFSET + 3];
    if(flag)
    {
        //printf("start_time_date=0x%x ",start_time_date);
        char temp[10];
        sprintf(temp,"%06x ",start_time_date);
        printf_txt(temp);
    }

    start_time_time = SDT_CON[OFFSET + 4] << 16;
    start_time_time |= SDT_CON[OFFSET + 5] << 8;
    start_time_time |= SDT_CON[OFFSET + 6];
    if(flag)
    {
        //printf("start_time_time=0x%06x ",start_time_time);
        char temp[10];
        sprintf(temp,"%06x ",start_time_time);
        printf_txt(temp);
    }

    duration = SDT_CON[OFFSET + 7] << 16;
    duration |= SDT_CON[OFFSET + 8] << 8;
    duration |= SDT_CON[OFFSET + 9];
    if(flag)
    {
        //printf("duration=0x%06x ",duration);
        char temp[10];
        sprintf(temp,"%06x ",duration);
        printf_txt(temp);
    }

    running_status = (SDT_CON[OFFSET + 10] & B567BIT) >> 5;
    //printf("running_status=%d ",running_status);
    free_CA_mode = (SDT_CON[OFFSET + 10] & BIT4) >> 4;
    //printf("free_CA_mode=%d ",free_CA_mode);
    descriptors_loop_length = (SDT_CON[OFFSET + 10] & B4BIT) << 8;
    descriptors_loop_length |= SDT_CON[OFFSET + 11];
    //printf("descriptors_loop_length=%d ",descriptors_loop_length);
    OFFSET += 12;
    //printf("OFFSET=%d ",OFFSET);
    DESCRIPTOR_LENGTH = 0;
    while(DESCRIPTOR_LENGTH < (descriptors_loop_length - 4 + 1))
        Parse_second_N();
    //printf("\n=======================end of Parse_first_N=========================\n");
}

void Parse_EIT_CON()
{
    FILE *fp2;
    int i;
    int service_id, version_number, current_next_indicator;
    int section_number, last_section_number, transport_stream_id, original_network_id;
    int segment_last_section_number, last_table_id;
//printf("\n=======================Parse_EIT_CON=========================\n");
    if((fp2 = fopen("EITNOW.txt","rb")) == NULL)
        printf("eror open2\n");
    for(i=0; i<content_len; i++)
        SDT_CON[i] = fgetc(fp2);
    service_id = SDT_CON[8] << 8;
    service_id |= SDT_CON[9];
    if(service_id == 4)
        flag = 1;
    else
        flag = 0;
    if(flag)
        //printf("service_id=0x%x ",service_id);
        version_number = (SDT_CON[10] & B12345BIT) >> 1;
    if(flag)
    {
        //printf("version_number=%d ",version_number);
        char temp[50];
        sprintf(temp,"%d ",version_number);
        printf_txt(temp);
    }
    current_next_indicator = SDT_CON[10] & B0BIT;
    if(flag)
    {
        //printf("current_next_indicator=%d ",current_next_indicator);
        char temp[50];
        sprintf(temp,"%d ",current_next_indicator);
        printf_txt(temp);
    }
    section_number = SDT_CON[11];
    if(flag)
    {
        //printf("section_number=%d ",section_number);
        char temp[50];
        sprintf(temp,"%d ",section_number);
        printf_txt(temp);
    }
    last_section_number = SDT_CON[12];
    if(flag)
    {
        //printf("last_section_number=%d ",last_section_number);
        char temp[50];
        sprintf(temp,"%d ",last_section_number);
        printf_txt(temp);
    }
    transport_stream_id = SDT_CON[13] << 8;
    transport_stream_id |= SDT_CON[14];
    if(flag)
    {
        //printf("transport_stream_id=0x%x ",transport_stream_id);
        char temp[50];
        sprintf(temp,"0x%x ",transport_stream_id);
        printf_txt(temp);
    }
    original_network_id = SDT_CON[15] << 8;
    original_network_id |= SDT_CON[16];
    if(flag)
    {
        //printf("original_network_id=0x%x ",original_network_id);
        char temp[50];
        sprintf(temp,"0x%x ",original_network_id);
        printf_txt(temp);
    }
    segment_last_section_number = SDT_CON[17];
    if(flag)
    {
        //printf("segment_last_section_number=%d ",segment_last_section_number);
        char temp[50];
        sprintf(temp,"0x%x ",segment_last_section_number);
        printf_txt(temp);
    }
    last_table_id = SDT_CON[18];
    if(flag)
    {
        //printf("last_table_id=0x%x ",last_table_id);
        char temp[50];
        sprintf(temp,"0x%x ",last_table_id);
        printf_txt(temp);
    }

    OFFSET = 19;
    //printf("OFFSET=%d ",OFFSET);
    while(OFFSET < (section_length - 4 + 1))
        Parse_first_N();
    /*	if(flag)
    	{
    		//printf("\n=======================end of Parse_EIT_CON=========================\n");
    		char temp[50];
    		sprintf(temp,"%s ","\n=======================end of Parse_EIT_CON=========================\n");
    		printf_txt(temp);
    	}*/
    fclose(fp2);
}


void main()
{
    FILE *fp1, *fp2, *fp3;
    int i, j, PID = 0;
    int temp, temp_ts_packet[188];
    int transport_error_indicator, payload_unit_start_indicator, transport_priority;
    int transport_scrambling_control, adaption_field_control, continuity_counter;
    int table_id;


    if((fp1 = fopen("3950--0x4e.txt","rb")) == NULL)/* EIT-4172.txt    NOW_BA.txt */
        printf("eror open1\n");
    if((fp3 = fopen("EIT-printf.txt","wb")) == NULL)
        printf("eror open3\n");

    while(!feof(fp1))//while(!feof(fp1))//for(k=0; k<2; k++)//
    {
        //printf("\n ******************************************************************** \n");

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
            //printf("table_id=0x%x ",table_id);
            if(table_id == 0x4e && flag)
            {
                //printf("\n==================Now transport stream for-back-info====================\n");
                char temp[100];
                sprintf(temp,"%s ","\n==================Now transport stream for-back-info====================\n");
                printf_txt(temp);
            }
            else if(table_id == 0x4f && flag)
            {
                //printf("\n==================Other transport stream for-back-info====================\n");
                char temp[100];
                sprintf(temp,"%s ","\n==================Other transport stream for-back-info====================\n");
                printf_txt(temp);
            }
            else if(table_id >= 0x50 && table_id <= 0x5f && flag)
            {
                //printf("\n==================Now transport stream time-info====================\n");
                char temp[100];
                sprintf(temp,"%s ","\n==================Now transport stream time-info====================\n");
                printf_txt(temp);
            }
            else if(table_id >= 0x60 && table_id <= 0x6f && flag)
            {
                //printf("\n==================Other transport stream time-info====================\n");
                char temp[100];
                sprintf(temp,"%s ","\n==================Other transport stream time-info====================\n");
                printf_txt(temp);
            }



            /*=============================PARSE_SDT_Content()=============================*/

            {
                int section_syntax_indicator;
                section_syntax_indicator =  (temp_ts_packet[6] & BIT7) >> 7;
                //printf("section_syntax_indicator=%d ",section_syntax_indicator);
                section_length = temp_ts_packet[6] & B4BIT;
                section_length |= temp_ts_packet[7];
                //printf("section_length=%d ",section_length);
                if(section_length <= 4093)
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
                if((fp2 = fopen("EITNOW.txt","wb")) == NULL)
                    printf("eror open2\n");
                else
                {
                    for(i=0; i<content_len; i++)
                        fputc(SDT_CON[i], fp2);
                    fclose(fp2);
                    Parse_EIT_CON();
                }

            }
        }
    }
    fclose(fp1);
    fclose(fp3);
}