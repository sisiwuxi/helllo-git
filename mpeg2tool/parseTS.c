#include <stdio.h>
#include <stdlib.h>
#include "include/common.h"

/*13BIT*/
int Found_PID[PID_MAX_NUM] = {0};
static int PIDNum = 0;

//optimization//
void CollectAllPID(int PID)
{
    int i = 0;
    for(i = 0; i < PID_MAX_NUM; i++)
    {
        if(PID == Found_PID[i])
            return;
    }
    for(i = 0; i < PID_MAX_NUM; i++)
    {
        if(Found_PID[i] == INVALID_PID)
        {
            Found_PID[i] = PID;
            INFO("[%d-%d] ",i, Found_PID[i]);
            break;
        }
    }
    if(PID_MAX_NUM == i)
        ERR("The number of PID is greater than %d", PID_MAX_NUM);
    return;
}
void ParsePAT()
{
    FILE *fp1;
    int i = 0, PID = 0, N = 0;
    int temp, temp_ts_packet[188];
    int transport_error_indicator, payload_unit_start_indicator, transport_priority;

    int transport_scrambling_control, adaption_field_control, continuity_counter;
    int table_id, section_syntax_indicator;
    int section_length, transport_stream_id, version_number;
    int current_next_indicator, section_number, last_section_number;
    int program_number, network_PID, program_map_PID;
    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
    {
        ERR("\nfp1 error open\n");
        goto FINISH;
    }
    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp == 0x47)
        {
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp1) & B8BIT);
            DEBUG("temp_ts_packet=%02x %02x %02x %02x\n",temp_ts_packet[0],temp_ts_packet[1],temp_ts_packet[2],temp_ts_packet[3]);

            transport_error_indicator = (temp_ts_packet[1] & BIT7) >> 7;
            payload_unit_start_indicator = (temp_ts_packet[1] & BIT6) >> 6;
            transport_priority = (temp_ts_packet[1] & BIT5) >> 5;
            DEBUG("transport_error_indicator=%d,payload_unit_start_indicator=%d,transport_priority=%d\n",\
                  transport_error_indicator,payload_unit_start_indicator,transport_priority);

            PID = (temp_ts_packet[1] & B5BIT) << 8;
            PID |= temp_ts_packet[2];
            DEBUG("PID=%d ",PID);

            if(PAT_PID == PID)
            {
                transport_scrambling_control = (temp_ts_packet[3] & B76BIT) >> 6;
                adaption_field_control = (temp_ts_packet[3] & B54BIT) >> 4;
                continuity_counter = temp_ts_packet[3] & B4BIT;
                DEBUG("transport_scrambling_control=%d,adaption_field_control=%d,continuity_counter=%d\n", transport_scrambling_control,adaption_field_control,continuity_counter);
                /*----------parse PAT---------*/
                INFO("\n----------parse PAT---------\n");
                table_id = temp_ts_packet[5];
                INFO("table_id=%d ",table_id);
                section_syntax_indicator = (temp_ts_packet[6] & BIT7) >> 7;
                INFO("section_syntax_indicator=%d ",section_syntax_indicator);
                section_length = (temp_ts_packet[6] & B4BIT) << 8;
                section_length |= temp_ts_packet[7];
                INFO("section_length=%d ",section_length);
                transport_stream_id = temp_ts_packet[8] << 8;
                transport_stream_id |= temp_ts_packet[9];
                INFO("transport_stream_id=%d ",transport_stream_id);
                version_number = (temp_ts_packet[10] & B12345BIT) >> 1;
                INFO("version_number=%d ",version_number);
                current_next_indicator = temp_ts_packet[10] & B0BIT;
                INFO("current_next_indicator=%d ",current_next_indicator);
                section_number = temp_ts_packet[11];
                INFO("section_number=%d ",section_number);
                last_section_number = temp_ts_packet[12];
                INFO("last_section_number=%d ",last_section_number);
                N = (section_length - 9) / 4;
                INFO("N=%d ",N);
                for(i=0; i<N; i++)
                {
                    program_number = temp_ts_packet[13 + i*4] << 8;
                    program_number |= temp_ts_packet[14 + i*4];
                    INFO("program_number=%d ",program_number);
                    if(program_number == 0)
                    {
                        network_PID = (temp_ts_packet[15 + i*4] & B5BIT) << 8;
                        network_PID |= temp_ts_packet[16 + i*4];
                        INFO("network_PID=%d ",network_PID);
                    }
                    else
                    {
                        program_map_PID = (temp_ts_packet[15 + i*4] & B5BIT) << 8;
                        program_map_PID |= temp_ts_packet[16 + i*4];
                        INFO("program_map_PID=%d ",program_map_PID);
                    }
                }
                INFO("\n----------end of parse PAT---------\n");
                break;
            }
        }
    }
FINISH:
    if(fp1!=NULL)
        fclose(fp1);
    return;
}
void PrintPIDs()
{
    int i=0;
    //===============totalNum===========//
    for(i = 0; i < PID_MAX_NUM; i++)
    {
        if(Found_PID[i] != INVALID_PID)
            PIDNum++;
    }
    INFO("\nPIDNum=%d \n",PIDNum);
    //===============PAT->PMT===========//
    for(i = 0; i < PIDNum; i++)
    {
        if(PAT_PID == Found_PID[i])
        {
            ParsePAT();
            break;
        }
    }
    //===============PMT->video&audio===========//
}



void ParseSequenceHeader(long int offset, char* parseData)
{
	int i = 0;
	INFO("\nSequenceHeader=0x%lx", offset);	
	for(i = 0; i < strlen(parseData); i++)
	{
		INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void ParseExtensionHeader(long int offset, char* parseData)
{
	int i = 0;
	INFO("\nExtensionHeader=0x%lx", offset);	
	for(i = 0; i < strlen(parseData); i++)
	{
		INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void ParsePictureHeader(long int offset, char* parseData)
{
	int i = 0;
	INFO("\nPictureHeader=0x%lx", offset);	
	for(i = 0; i < strlen(parseData); i++)
	{
		INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void ParseGOPHeader(long int offset, char* parseData)
{
	int i = 0;
	INFO("\nGOPHeader=0x%lx", offset);	
	for(i = 0; i < strlen(parseData); i++)
	{
		INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void ParseSliceHeader(long int offset, char* parseData)
{
	int i = 0;
	INFO("\nSliceHeader=0x%lx ", offset);	
	for(i = 0; i < SLICE_HEADER_LEN; i++)
	{
		INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void PrintNALType(int NALType)
{
    switch(NALType)
    {
        case NAL_TYPE_TRAIL_N:
            INFO("\n[TRAIL_N,VLC,B-Slice,Coded slice segment of a non-TSA, non-STSA trailing picture, slice_segment_layer_rbsp]");	
            break;
        case NAL_TYPE_TRAIL_R:
            INFO("\n[TRAIL_R,VLC,P-Slice,Coded slice segment of a non-TSA, non-STSA trailing picture, slice_segment_layer_rbsp]");	
            break;	
        case NAL_TYPE_TSA_N:
            INFO("\n[TSA_N,VLC,Coded slice segment of a TSA picture, slice_segment_layer_rbsp]");	
            break;		
        case NAL_TYPE_TSA_R:
            INFO("\n[TSA_R,VLC,Coded slice segment of a TSA picture, slice_segment_layer_rbsp]");	
            break;		
        case NAL_TYPE_STSA_N:
            INFO("\n[STSA_N,VLC,Coded slice segment of an STSA picture, slice_segment_layer_rbsp]");	
            break;		
        case NAL_TYPE_STSA_R:
            INFO("\n[STSA_R,VLC,Coded slice segment of an STSA picture, slice_segment_layer_rbsp]");	
            break;	
        case NAL_TYPE_RADL_N:
            INFO("\n[RADL_N,VLC,Coded slice segment of a RADL picture, slice_segment_layer_rbsp]");	
            break;		
        case NAL_TYPE_RADL_R:
            INFO("\n[RADL_R,VLC,Coded slice segment of a RADL picture, slice_segment_layer_rbsp]");	
            break;	
        case NAL_TYPE_RASL_N:
            INFO("\n[RASL_N,VLC,Coded slice segment of a RASL picture, slice_segment_layer_rbsp]");	
            break;	
        case NAL_TYPE_RASL_R:
            INFO("\n[RASL_R,VLC,Coded slice segment of a RASL picture, slice_segment_layer_rbsp]");	
            break;	
        case NAL_TYPE_VCL_N11:
            INFO("\n[VCL_N11,VLC,Reserved non-IRAP sub-layer reference VCL NAL unit types]");	
            break;	
        case NAL_TYPE_VCL_N13:
            INFO("\n[VCL_N13,VLC,Reserved non-IRAP sub-layer reference VCL NAL unit types]");	
            break;	
        case NAL_TYPE_VCL_N15:
            INFO("\n[VCL_N15,VLC,Reserved non-IRAP sub-layer reference VCL NAL unit types]");	
            break;
        case NAL_TYPE_BLA_W_LP:
            INFO("\n[BLA_W_LP,VLC,Coded slice segment of a BLA picture, slice_segment_layer_rbsp]");	
            break;		
        case NAL_TYPE_BLA_W_RADL:
            INFO("\n[BLA_W_RADL,VLC,Coded slice segment of a BLA picture, slice_segment_layer_rbsp]");	
            break;		
        case NAL_TYPE_BLA_N_LP:
            INFO("\n[BLA_N_LP,VLC,Coded slice segment of a BLA picture, slice_segment_layer_rbsp]");	
            break;	
        case NAL_TYPE_IDR_W_RADL:
            INFO("\n[IDR_W_RADL,VLC,Coded slice segment of an IDR picture, slice_segment_layer_rbsp]");	
            break;	
        case NAL_TYPE_IDR_N_LP:
            INFO("\n[IDR_N_LP,VLC,Coded slice segment of an IDR picture, slice_segment_layer_rbsp]");	
            break;			
        case NAL_TYPE_CRA_NU:
            INFO("\n[CRA_NU,VLC,Coded slice segment of an CRA picture, slice_segment_layer_rbsp]");	
            break;
        case NAL_TYPE_RSV_IRAP_VCL22:
            INFO("\n[RSV_IRAP_VCL22,VLC,Reserved IRAP VCL NAL unit types]");	
            break;	
        case NAL_TYPE_RSV_IRAP_VCL23:
            INFO("\n[RSV_IRAP_VCL23,VLC,Reserved IRAP VCL NAL unit types]");	
            break;	
        case NAL_TYPE_RSV_VCL24:
            INFO("\n[RSV_VCL24,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;		
        case NAL_TYPE_RSV_VCL25:
            INFO("\n[RSV_VCL25,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;	
        case NAL_TYPE_RSV_VCL26:
            INFO("\n[RSV_VCL26,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;				
        case NAL_TYPE_RSV_VCL27:
            INFO("\n[RSV_VCL27,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;	
        case NAL_TYPE_RSV_VCL28:
            INFO("\n[RSV_VCL28,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;		
        case NAL_TYPE_RSV_VCL29:
            INFO("\n[RSV_VCL29,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;	
        case NAL_TYPE_RSV_VCL30:
            INFO("\n[RSV_VCL30,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;	
        case NAL_TYPE_RSV_VCL31:
            INFO("\n[RSV_VCL31,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;		
        case NAL_TYPE_VPS_NUT:
            INFO("\n[VPS_NUT,non-VLC,Video parameter set, video_parameter_set_rbsp]");	
            break;				
        case NAL_TYPE_SPS_NUT:
            INFO("\n[SPS_NUT,non-VLC,Sequence parameter set, seq_parameter_set_rbsp]");	
            break;	
        case NAL_TYPE_PPS_NUT:
            INFO("\n[PPS_NUT,non-VLC,Picture  parameter set, pic_parameter_set_rbsp]");	
            break;			
        case NAL_TYPE_AUD_NUT:
            INFO("\n[AUD_NUT,non-VLC,Access unit delimiter, access_unit_delimiter_rbsp]");	
            break;	
        case NAL_TYPE_EOS_NUT:
            INFO("\n[EOS_NUT,non-VLC,End of sequence, end_of_seq_rbsp]");	
            break;			
        case NAL_TYPE_EOB_NUT:
            INFO("\n[EOB_NUT,non-VLC,End of bitstream, end_of_bitstream_rbsp]");	
            break;	
        case NAL_TYPE_FD_NUT:
            INFO("\n[FD_NUT,non-VLC,Filler data, filler_data_rbsp]");	
            break;			
        case NAL_TYPE_PREFIX_SEI_NUT:
            INFO("\n[PREFIX_SEI_NUT,non-VLC,Supplemental enhancement information, sei_rbsp]");	
            break;
        case NAL_TYPE_SUFFIX_SEI_NUT:
            INFO("\n[SUFFIX_SEI_NUT,non-VLC,Supplemental enhancement information, sei_rbsp]");	
            break;			
        case NAL_TYPE_RSV_NVCL41:
            INFO("\n[RSV_NVCL41,non-VLC,Reserved]");	
            break;	
        case NAL_TYPE_RSV_NVCL42:
            INFO("\n[RSV_NVCL42,non-VLC,Reserved]");	
            break;	
        case NAL_TYPE_RSV_NVCL43:
            INFO("\n[RSV_NVCL43,non-VLC,Reserved]");	
            break;			
        case NAL_TYPE_RSV_NVCL44:
            INFO("\n[RSV_NVCL44,non-VLC,Reserved]");	
            break;
        case NAL_TYPE_RSV_NVCL45:
            INFO("\n[RSV_NVCL45,non-VLC,Reserved]");	
            break;	
        case NAL_TYPE_RSV_NVCL46:
            INFO("\n[RSV_NVCL46,non-VLC,Reserved]");	
            break;	
        case NAL_TYPE_RSV_NVCL47:
            INFO("\n[RSV_NVCL47,non-VLC,Reserved]");	
            break;				
        case NAL_TYPE_UNSPEC48:
            WEAK_INFO("\n[UNSPEC48,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC49:
            WEAK_INFO("\n[UNSPEC49,non-VLC,Unspecified]");	
            break;			
        case NAL_TYPE_UNSPEC50:
            WEAK_INFO("\n[UNSPEC50,non-VLC,Unspecified]");	
            break;	
        case NAL_TYPE_UNSPEC51:
            WEAK_INFO("\n[UNSPEC51,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC52:
            WEAK_INFO("\n[UNSPEC52,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC53:
            WEAK_INFO("\n[UNSPEC53,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC54:
            WEAK_INFO("\n[UNSPEC54,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC55:
            WEAK_INFO("\n[UNSPEC55,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC56:
            WEAK_INFO("\n[UNSPEC56,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC57:
            WEAK_INFO("\n[UNSPEC57,non-VLC,Unspecified]");	
            break;			
        case NAL_TYPE_UNSPEC58:
            WEAK_INFO("\n[UNSPEC58,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC59:
            WEAK_INFO("\n[UNSPEC59,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC60:
            WEAK_INFO("\n[UNSPEC60,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC61:
            WEAK_INFO("\n[UNSPEC61,non-VLC,Unspecified]");	
            break;
        case NAL_TYPE_UNSPEC62:
            WEAK_INFO("\n[UNSPEC62,non-VLC,Unspecified]");	
            break;			
        case NAL_TYPE_UNSPEC63:
            WEAK_INFO("\n[UNSPEC63,non-VLC,Unspecified]");	
            break;	
        default:
            break;
    }	
}

void MPEG2ToolParsePIDs()
{
    FILE *fp1;
    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
    {
        ERR("\nfp1 error open\n");
        goto FINISH;
    }
//=================Collect_pid==================//
    int i = 0, PID = 0;
    int temp, temp_ts_packet[188];
    //int transport_error_indicator = 0, payload_unit_start_indicator = 0, transport_priority = 0;

    memset(&Found_PID, INVALID_PID, sizeof(Found_PID));

    temp = fgetc(fp1) & B8BIT;
    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp == 0x47)
        {
            temp_ts_packet[0] = temp;
            for(i=1; i<188; i++)
                temp_ts_packet[i] = (fgetc(fp1) & B8BIT);
#if 0
            transport_error_indicator = (temp_ts_packet[1] & BIT7) >> 7;
            payload_unit_start_indicator = (temp_ts_packet[1] & BIT6) >> 6;
            transport_priority = (temp_ts_packet[1] & BIT5) >> 5;
#endif
            PID = (temp_ts_packet[1] & B5BIT) << 8;
            PID |= temp_ts_packet[2];
            CollectAllPID(PID);
        }
    }
    PrintPIDs();
FINISH:
    if(fp1!=NULL)
        fclose(fp1);
    return;
}


void MPEG2ToolParseMPEG2()
{
    FILE *fp1;
    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
    {
        ERR("\nfp1 error open\n");
        goto FINISH;
    }
    int temp, temp_ts_packet[188];
    int i = 0, StartCode = 0, Picture_coding_type = 0, video_format = 0;
    char SequenceHeader[12+1] = {0}, ExtensionHeader[22 + 1] = {0}, GOPHeader[8+1]={0}, PictureHeader[8+1]={0}, SliceHeader[SLICE_HEADER_LEN+1]={0};
    INFO("\n============Collect_mpeg2_header===============\n");	
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
		      strncpy(SliceHeader, temp_ts_packet+i+4, SLICE_HEADER_LEN*sizeof(char));
		      ParseSliceHeader(ftell(fp1), SliceHeader);						
                }
#if 0				
                switch(StartCode)
                {
                case VIDEO_SEQUENCE_CODE:
		      strncpy(SequenceHeader, temp_ts_packet[i+4], 12*sizeof(char));
		      ParseSequenceHeader(ftell(fp1), SequenceHeader);	
                    break;
                case SEQUENCE_HEADER_CODE:
                    INFO("\nSequenceHeader=[0x%x, 0x%lx]", (StartCode & B8BIT), ftell(fp1));
                    break;
                case USERDATA_HEADER_CODE:	
                    INFO("\nUserdataHeader=[0x%x, 0x%lx]", (StartCode & B8BIT), ftell(fp1));					
                    break;					
                case EXTENSION_HEADER_CODE:
		      strncpy(ExtensionHeader, temp_ts_packet[i+4], 22*sizeof(char));
		      ParseExtensionHeader(ftell(fp1), ExtensionHeader);	
                    break;						
                case SEQUENCE_END_CODE:
                    INFO("\nSequenceEnd=[0x%x, 0x%lx]", (StartCode & B8BIT), ftell(fp1));
                    break;
                case GOP_HEADER_CODE:
		      strncpy(GOPHeader, temp_ts_packet[i+4], 8*sizeof(char));
		      ParseGOPHeader(ftell(fp1), GOPHeader);	
                    break;
                case PICTURE_HEADER_CODE:
		      strncpy(PictureHeader, temp_ts_packet[i+4], 8*sizeof(char));
		      ParsePictureHeader(ftell(fp1), PictureHeader);	
                    break;
                default:
                    break;
                }
#endif				
            }
        }
    }

FINISH:
    if(fp1!=NULL)
        fclose(fp1);
    return;
}

void MPEG2ToolParseHEVC()
{
    FILE *fp1;
    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
    {
        ERR("\nfp1 error open\n");
        goto FINISH;
    }

    int temp, temp_ts_packet[188];
    int i = 0, StartCode = 0, NALType = 0;
    INFO("\n============Collect_HEVC_header===============\n");
	
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
                if(NAL_START_CODE == StartCode)
                {
			NALType = (temp_ts_packet[i+4] & BIT123456)>>1;
			PrintNALType(NALType);
			if(NALType>=16 && NALType<=23)
			    INFO("  [%d-%d] ", NALType, ftell(fp1));			
                }
            }
        }
    }

FINISH:
    if(fp1!=NULL)
        fclose(fp1);
    return;
}


void MPEG2ToolParser(char* Parse_Type)
{	
	_ParseType ParseType = atoi(Parse_Type);
	switch(ParseType)
	{
		case PARSE_PIDs:
			MPEG2ToolParsePIDs();
			break;
		case PARSE_MPEG2:
			MPEG2ToolParseMPEG2();
			break;	
		case PARSE_HEVC:
			MPEG2ToolParseHEVC();
			break;
		default:
			break;				
	}
	return;
}

void MPEG2AddSEI()
{
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    int location=0;
    int temp, temp1, temp2, temp3, temp4;

    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
    {
        ERR("\nfp1 error open\n");
        goto FINISH;
    }
    if((fp2 = fopen(MPEG2ToolGetOutputPath(),"wb+")) == NULL)
    {
        ERR("\nfp2 error open\n");
        goto FINISH;
    }

    while(!feof(fp1))
    {
        fseek(fp1, location, SEEK_SET);
        temp=0;
        temp1=fgetc(fp1);
        temp2=fgetc(fp1);
        temp3=fgetc(fp1);
        temp4=fgetc(fp1);
        temp = (temp1<<24) |(temp2<<16) | (temp3<<8) | temp4;
        //printf("[%d-0x%x-0x%x] ",location, temp1, temp);
        if((0x00000126 == temp) || (0x00000128 == temp) || (0x0000012A == temp))
        {   //00 00 01 4E 01 93 01 12 80
            //printf("\n ============find out [%d-0x%x-0x%x-0x%x-0x%x=0x%x]============= \n", location, temp1, temp2, temp3, temp4, temp);
            INFO("\n ============find out [%d-0x%x]============= ", location,  temp);
            fputc((0x00 & B8BIT), fp2);
            fputc((0x00 & B8BIT), fp2);
            fputc((0x01 & B8BIT), fp2);
            fputc((0x4E & B8BIT), fp2);
            fputc((0x01 & B8BIT), fp2);
            fputc((0x93 & B8BIT), fp2);
            fputc((0x01 & B8BIT), fp2);
            fputc((0x12 & B8BIT), fp2);
            fputc((0x80 & B8BIT), fp2);
            fputc((temp1 & B8BIT), fp2);
            fputc((temp2 & B8BIT), fp2);
            fputc((temp3 & B8BIT), fp2);
            fputc((temp4 & B8BIT), fp2);
            location=location+4;
        }
        else
        {
            fputc((temp1 & B8BIT), fp2);
            location++;
        }
    }

FINISH:
    if(fp1!=NULL)
        fclose(fp1);
    if(fp2!=NULL)
        fclose(fp2);
    return;
}


