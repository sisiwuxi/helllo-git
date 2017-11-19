#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/common.h"

#define DBG_LEVEL TOOL_DBG_INFO

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
            DBG_INFO("[%d-%d] ",i, Found_PID[i]);
            break;
        }
    }
    if(PID_MAX_NUM == i)
        DBG_ERR("The number of PID is greater than %d", PID_MAX_NUM);
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
        DBG_ERR("\nfp1 error open\n");
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
            DBG_ERR("temp_ts_packet=%02x %02x %02x %02x\n",temp_ts_packet[0],temp_ts_packet[1],temp_ts_packet[2],temp_ts_packet[3]);

            transport_error_indicator = (temp_ts_packet[1] & BIT7) >> 7;
            payload_unit_start_indicator = (temp_ts_packet[1] & BIT6) >> 6;
            transport_priority = (temp_ts_packet[1] & BIT5) >> 5;
            DBG_ERR("transport_error_indicator=%d,payload_unit_start_indicator=%d,transport_priority=%d\n",\
                  transport_error_indicator,payload_unit_start_indicator,transport_priority);

            PID = (temp_ts_packet[1] & B5BIT) << 8;
            PID |= temp_ts_packet[2];
            DBG_ERR("PID=%d ",PID);

            if(PAT_PID == PID)
            {
                transport_scrambling_control = (temp_ts_packet[3] & B76BIT) >> 6;
                adaption_field_control = (temp_ts_packet[3] & B54BIT) >> 4;
                continuity_counter = temp_ts_packet[3] & B4BIT;
                DBG_ERR("transport_scrambling_control=%d,adaption_field_control=%d,continuity_counter=%d\n", transport_scrambling_control,adaption_field_control,continuity_counter);
                /*----------parse PAT---------*/
                DBG_INFO("\n----------parse PAT---------\n");
                table_id = temp_ts_packet[5];
                DBG_INFO("table_id=%d ",table_id);
                section_syntax_indicator = (temp_ts_packet[6] & BIT7) >> 7;
                DBG_INFO("section_syntax_indicator=%d ",section_syntax_indicator);
                section_length = (temp_ts_packet[6] & B4BIT) << 8;
                section_length |= temp_ts_packet[7];
                DBG_INFO("section_length=%d ",section_length);
                transport_stream_id = temp_ts_packet[8] << 8;
                transport_stream_id |= temp_ts_packet[9];
                DBG_INFO("transport_stream_id=%d ",transport_stream_id);
                version_number = (temp_ts_packet[10] & B12345BIT) >> 1;
                DBG_INFO("version_number=%d ",version_number);
                current_next_indicator = temp_ts_packet[10] & B0BIT;
                DBG_INFO("current_next_indicator=%d ",current_next_indicator);
                section_number = temp_ts_packet[11];
                DBG_INFO("section_number=%d ",section_number);
                last_section_number = temp_ts_packet[12];
                DBG_INFO("last_section_number=%d ",last_section_number);
                N = (section_length - 9) / 4;
                DBG_INFO("N=%d ",N);
                for(i=0; i<N; i++)
                {
                    program_number = temp_ts_packet[13 + i*4] << 8;
                    program_number |= temp_ts_packet[14 + i*4];
                    DBG_INFO("program_number=%d ",program_number);
                    if(program_number == 0)
                    {
                        network_PID = (temp_ts_packet[15 + i*4] & B5BIT) << 8;
                        network_PID |= temp_ts_packet[16 + i*4];
                        DBG_INFO("network_PID=%d ",network_PID);
                    }
                    else
                    {
                        program_map_PID = (temp_ts_packet[15 + i*4] & B5BIT) << 8;
                        program_map_PID |= temp_ts_packet[16 + i*4];
                        DBG_INFO("program_map_PID=%d ",program_map_PID);
                    }
                }
                DBG_INFO("\n----------end of parse PAT---------\n");
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
    DBG_INFO("\nPIDNum=%d \n",PIDNum);
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
	DBG_INFO("\nSequenceHeader=0x%lx", offset);	
	for(i = 0; i < strlen(parseData); i++)
	{
		DBG_INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void ParseExtensionHeader(long int offset, char* parseData)
{
	int i = 0;
	DBG_INFO("\nExtensionHeader=0x%lx", offset);	
	for(i = 0; i < strlen(parseData); i++)
	{
		DBG_INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void ParsePictureHeader(long int offset, char* parseData)
{
	int i = 0;
	DBG_INFO("\nPictureHeader=0x%lx", offset);	
	for(i = 0; i < strlen(parseData); i++)
	{
		DBG_INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void ParseGOPHeader(long int offset, char* parseData)
{
	int i = 0;
	DBG_INFO("\nGOPHeader=0x%lx", offset);	
	for(i = 0; i < strlen(parseData); i++)
	{
		DBG_INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void ParseSliceHeader(long int offset, char* parseData)
{
	int i = 0;
	DBG_INFO("\nSliceHeader=0x%lx ", offset);	
	for(i = 0; i < SLICE_HEADER_LEN; i++)
	{
		DBG_INFO(" 0x%x ", parseData[i]);
	}
	return;
}

void PrintH264NALType(int NALType)
{
    switch(NALType)
    {
        case H264_NAL_TYPE_NONE:
            DBG_INFO("[Unspecified]");	
            break;		
        case H264_NAL_TYPE_SLICE:
            DBG_INFO("[%d SLICE Code slice of a non-IDR picture slice_layer_without_partitioning_rbsp()]", NALType);	
            break;	
        case H264_NAL_TYPE_DPA:
            DBG_INFO("[%d DPA Coded slice data partition A slice_data_partition_a_layer_rbsp()]", NALType);	
            break;		
        case H264_NAL_TYPE_DPB:
            DBG_INFO("[%d DPB Coded slice data partition B slice_data_partition_b_layer_rbsp()]", NALType);	
            break;	
        case H264_NAL_TYPE_DPC:
            DBG_INFO("[%d DPC Coded slice data partition C slice_data_partition_c_layer_rbsp()]", NALType);	
            break;		
        case H264_NAL_TYPE_IDR:
            DBG_INFO("[%d IDR Coded slice of an IDR picture slice_layer_without_partitioning_rbsp()]", NALType);	
            break;	
        case H264_NAL_TYPE_SEI:
            DBG_INFO("[SEI Supplemental enhancement information(SEI) sei_rbsp()]");	
            break;		
        case H264_NAL_TYPE_SPS:
            DBG_INFO("[SPS Sequence parameter set seq_parameter_set_rbsp()]");	
            break;		
        case H264_NAL_TYPE_PPS:
            DBG_INFO("[PPS Picture parameter set pic_parameter_set_rbsp()]");	
            break;		
        case H264_NAL_TYPE_AUD:
            DBG_INFO("[AUD Access unit delimiter accsess_unit_delimiter_rbsp()]");	
            break;	
        case H264_NAL_TYPE_EOSEQ:
            DBG_INFO("[EOSEQ End of sequence end_of_seq_rbsp()]");	
            break;		
        case H264_NAL_TYPE_EOSTREAM:
            DBG_INFO("[EOSTREAM End of stream end_of_stream_rbsp()]");	
            break;
        case H264_NAL_TYPE_FILL:
            DBG_INFO("[FILL Filler data filler_data_rbsp()]");	
            break;
        case H264_NAL_TYPE_SEQPSE:
            DBG_INFO("[SEQPSE Sequence parameter set extension seq_parameter_set_extension_rbsp()]");	
            break;			
        case H264_NAL_TYPE_PNU:
            DBG_INFO("[PNU Prefix NAL unit prefix_nal_unit_rbsp()]");	
            break;		
        case H264_NAL_TYPE_SSPS:
            DBG_INFO("[SSPS Subset sequence parameter set subset_seq_parameter_set_rbsp()]");	
            break;
        case H264_NAL_TYPE_R16:
            DBG_INFO("[Reserved 16]");	
            break;			
        case H264_NAL_TYPE_R17:
            DBG_INFO("[Reserved 17]");	
            break;			
        case H264_NAL_TYPE_R18:
            DBG_INFO("[Reserved 18]");				
            break;	
        case H264_NAL_TYPE_CS:
            DBG_INFO("[CS Coded slice of an auxiliary coded picture without partitioning slice_layer_without_partitioning_rbsp()]");				
            break;		
        case H264_NAL_TYPE_CSE:
            DBG_INFO("[CSE Coded slice extension slice_layer_extension_rbsp()]");				
            break;			
        case H264_NAL_TYPE_R21:
            DBG_INFO("[Reserved 21]");				
            break;
        case H264_NAL_TYPE_R22:
            DBG_INFO("[Reserved 23]");				
            break;
        case H264_NAL_TYPE_R23:
            DBG_INFO("[Reserved 23]");				
            break;		
        case H264_NAL_TYPE_U24:
            DBG_INFO("[Unspecified 24]");				
            break;
        case H264_NAL_TYPE_U25:
            DBG_INFO("[Unspecified 25]");				
            break;
        case H264_NAL_TYPE_U26:
            DBG_INFO("[Unspecified 26]");				
            break;	
        case H264_NAL_TYPE_U27:
            DBG_INFO("[Unspecified 27]");				
            break;
        case H264_NAL_TYPE_U28:
            DBG_INFO("[Unspecified 28]");				
            break;
        case H264_NAL_TYPE_U29:
            DBG_INFO("[Unspecified 29]");				
            break;
        case H264_NAL_TYPE_U30:
            DBG_INFO("[Unspecified 30]");				
            break;
        case H264_NAL_TYPE_U31:
            DBG_INFO("[Unspecified 31]");				
            break;			
        default:
            break;
    }	
}

void PrintHEVCNALType(int NALType)
{
    switch(NALType)
    {
        case HEVC_NAL_TYPE_TRAIL_N:
            DBG_WRN("\n[TRAIL_N,VLC,B-Slice,Coded slice segment of a non-TSA, non-STSA trailing picture, slice_segment_layer_rbsp]");	
            break;
        case HEVC_NAL_TYPE_TRAIL_R:
            DBG_WRN("\n[TRAIL_R,VLC,P-Slice,Coded slice segment of a non-TSA, non-STSA trailing picture, slice_segment_layer_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_TSA_N:
            DBG_WRN("\n[TSA_N,VLC,Coded slice segment of a TSA picture, slice_segment_layer_rbsp]");	
            break;		
        case HEVC_NAL_TYPE_TSA_R:
            DBG_WRN("\n[TSA_R,VLC,Coded slice segment of a TSA picture, slice_segment_layer_rbsp]");	
            break;		
        case HEVC_NAL_TYPE_STSA_N:
            DBG_WRN("\n[STSA_N,VLC,Coded slice segment of an STSA picture, slice_segment_layer_rbsp]");	
            break;		
        case HEVC_NAL_TYPE_STSA_R:
            DBG_WRN("\n[STSA_R,VLC,Coded slice segment of an STSA picture, slice_segment_layer_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_RADL_N:
            DBG_WRN("\n[RADL_N,VLC,Coded slice segment of a RADL picture, slice_segment_layer_rbsp]");	
            break;		
        case HEVC_NAL_TYPE_RADL_R:
            DBG_WRN("\n[RADL_R,VLC,Coded slice segment of a RADL picture, slice_segment_layer_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_RASL_N:
            DBG_WRN("\n[RASL_N,VLC,Coded slice segment of a RASL picture, slice_segment_layer_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_RASL_R:
            DBG_WRN("\n[RASL_R,VLC,Coded slice segment of a RASL picture, slice_segment_layer_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_VCL_N11:
            DBG_WRN("\n[VCL_N11,VLC,Reserved non-IRAP sub-layer reference VCL NAL unit types]");	
            break;	
        case HEVC_NAL_TYPE_VCL_N13:
            DBG_WRN("\n[VCL_N13,VLC,Reserved non-IRAP sub-layer reference VCL NAL unit types]");	
            break;	
        case HEVC_NAL_TYPE_VCL_N15:
            DBG_WRN("\n[VCL_N15,VLC,Reserved non-IRAP sub-layer reference VCL NAL unit types]");	
            break;
        case HEVC_NAL_TYPE_BLA_W_LP:
            DBG_WRN("\n[BLA_W_LP,VLC,Coded slice segment of a BLA picture, slice_segment_layer_rbsp]");	
            break;		
        case HEVC_NAL_TYPE_BLA_W_RADL:
            DBG_WRN("\n[BLA_W_RADL,VLC,Coded slice segment of a BLA picture, slice_segment_layer_rbsp]");	
            break;		
        case HEVC_NAL_TYPE_BLA_N_LP:
            DBG_WRN("\n[BLA_N_LP,VLC,Coded slice segment of a BLA picture, slice_segment_layer_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_IDR_W_RADL:
            DBG_WRN("\n[IDR_W_RADL,VLC,Coded slice segment of an IDR picture, slice_segment_layer_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_IDR_N_LP:
            DBG_WRN("\n[IDR_N_LP,VLC,Coded slice segment of an IDR picture, slice_segment_layer_rbsp]");	
            break;			
        case HEVC_NAL_TYPE_CRA_NU:
            DBG_WRN("\n[CRA_NU,VLC,Coded slice segment of an CRA picture, slice_segment_layer_rbsp]");	
            break;
        case HEVC_NAL_TYPE_RSV_IRAP_VCL22:
            DBG_WRN("\n[RSV_IRAP_VCL22,VLC,Reserved IRAP VCL NAL unit types]");	
            break;	
        case HEVC_NAL_TYPE_RSV_IRAP_VCL23:
            DBG_WRN("\n[RSV_IRAP_VCL23,VLC,Reserved IRAP VCL NAL unit types]");	
            break;	
        case HEVC_NAL_TYPE_RSV_VCL24:
            DBG_WRN("\n[RSV_VCL24,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;		
        case HEVC_NAL_TYPE_RSV_VCL25:
            DBG_WRN("\n[RSV_VCL25,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;	
        case HEVC_NAL_TYPE_RSV_VCL26:
            DBG_WRN("\n[RSV_VCL26,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;				
        case HEVC_NAL_TYPE_RSV_VCL27:
            DBG_WRN("\n[RSV_VCL27,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;	
        case HEVC_NAL_TYPE_RSV_VCL28:
            DBG_WRN("\n[RSV_VCL28,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;		
        case HEVC_NAL_TYPE_RSV_VCL29:
            DBG_WRN("\n[RSV_VCL29,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;	
        case HEVC_NAL_TYPE_RSV_VCL30:
            DBG_WRN("\n[RSV_VCL30,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;	
        case HEVC_NAL_TYPE_RSV_VCL31:
            DBG_WRN("\n[RSV_VCL31,VLC,Reserved non-IRAP VCL NAL unit types]");	
            break;		
        case HEVC_NAL_TYPE_VPS_NUT:
            DBG_WRN("\n[VPS_NUT,non-VLC,Video parameter set, video_parameter_set_rbsp]");	
            break;				
        case HEVC_NAL_TYPE_SPS_NUT:
            DBG_WRN("\n[SPS_NUT,non-VLC,Sequence parameter set, seq_parameter_set_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_PPS_NUT:
            DBG_WRN("\n[PPS_NUT,non-VLC,Picture  parameter set, pic_parameter_set_rbsp]");	
            break;			
        case HEVC_NAL_TYPE_AUD_NUT:
            DBG_WRN("\n[AUD_NUT,non-VLC,Access unit delimiter, access_unit_delimiter_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_EOS_NUT:
            DBG_WRN("\n[EOS_NUT,non-VLC,End of sequence, end_of_seq_rbsp]");	
            break;			
        case HEVC_NAL_TYPE_EOB_NUT:
            DBG_WRN("\n[EOB_NUT,non-VLC,End of bitstream, end_of_bitstream_rbsp]");	
            break;	
        case HEVC_NAL_TYPE_FD_NUT:
            DBG_WRN("\n[FD_NUT,non-VLC,Filler data, filler_data_rbsp]");	
            break;			
        case HEVC_NAL_TYPE_PREFIX_SEI_NUT:
            DBG_WRN("\n[PREFIX_SEI_NUT,non-VLC,Supplemental enhancement information, sei_rbsp]");	
            break;
        case HEVC_NAL_TYPE_SUFFIX_SEI_NUT:
            DBG_WRN("\n[SUFFIX_SEI_NUT,non-VLC,Supplemental enhancement information, sei_rbsp]");	
            break;			
        case HEVC_NAL_TYPE_RSV_NVCL41:
            DBG_WRN("\n[RSV_NVCL41,non-VLC,Reserved]");	
            break;	
        case HEVC_NAL_TYPE_RSV_NVCL42:
            DBG_WRN("\n[RSV_NVCL42,non-VLC,Reserved]");	
            break;	
        case HEVC_NAL_TYPE_RSV_NVCL43:
            DBG_WRN("\n[RSV_NVCL43,non-VLC,Reserved]");	
            break;			
        case HEVC_NAL_TYPE_RSV_NVCL44:
            DBG_WRN("\n[RSV_NVCL44,non-VLC,Reserved]");	
            break;
        case HEVC_NAL_TYPE_RSV_NVCL45:
            DBG_WRN("\n[RSV_NVCL45,non-VLC,Reserved]");	
            break;	
        case HEVC_NAL_TYPE_RSV_NVCL46:
            DBG_WRN("\n[RSV_NVCL46,non-VLC,Reserved]");	
            break;	
        case HEVC_NAL_TYPE_RSV_NVCL47:
            DBG_WRN("\n[RSV_NVCL47,non-VLC,Reserved]");	
            break;				
        case HEVC_NAL_TYPE_UNSPEC48:
            DBG_INFO("\n[UNSPEC48,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC49:
            DBG_INFO("\n[UNSPEC49,non-VLC,Unspecified]");	
            break;			
        case HEVC_NAL_TYPE_UNSPEC50:
            DBG_INFO("\n[UNSPEC50,non-VLC,Unspecified]");	
            break;	
        case HEVC_NAL_TYPE_UNSPEC51:
            DBG_INFO("\n[UNSPEC51,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC52:
            DBG_INFO("\n[UNSPEC52,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC53:
            DBG_INFO("\n[UNSPEC53,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC54:
            DBG_INFO("\n[UNSPEC54,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC55:
            DBG_INFO("\n[UNSPEC55,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC56:
            DBG_INFO("\n[UNSPEC56,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC57:
            DBG_INFO("\n[UNSPEC57,non-VLC,Unspecified]");	
            break;			
        case HEVC_NAL_TYPE_UNSPEC58:
            DBG_INFO("\n[UNSPEC58,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC59:
            DBG_INFO("\n[UNSPEC59,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC60:
            DBG_INFO("\n[UNSPEC60,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC61:
            DBG_INFO("\n[UNSPEC61,non-VLC,Unspecified]");	
            break;
        case HEVC_NAL_TYPE_UNSPEC62:
            DBG_INFO("\n[UNSPEC62,non-VLC,Unspecified]");	
            break;			
        case HEVC_NAL_TYPE_UNSPEC63:
            DBG_INFO("\n[UNSPEC63,non-VLC,Unspecified]");	
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
        DBG_ERR("\nfp1 error open\n");
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
        DBG_ERR("\nfp1 error open\n");
        goto FINISH;
    }
    int temp, temp_ts_packet[188];
    int i = 0, StartCode = 0;//Picture_coding_type = 0, video_format = 0;
    //char SequenceHeader[12+1] = {0}, ExtensionHeader[22 + 1] = {0}, GOPHeader[8+1]={0}, PictureHeader[8+1]={0};
    char SliceHeader[SLICE_HEADER_LEN+1]={0};
    DBG_INFO("\n============Collect_mpeg2_header===============\n");	
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
		      strncpy(SliceHeader, (const char *)(temp_ts_packet+i+4), SLICE_HEADER_LEN*sizeof(char));
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
                    DBG_INFO("\nSequenceHeader=[0x%x, 0x%lx]", (StartCode & B8BIT), ftell(fp1));
                    break;
                case USERDATA_HEADER_CODE:	
                    DBG_INFO("\nUserdataHeader=[0x%x, 0x%lx]", (StartCode & B8BIT), ftell(fp1));					
                    break;					
                case EXTENSION_HEADER_CODE:
		      strncpy(ExtensionHeader, temp_ts_packet[i+4], 22*sizeof(char));
		      ParseExtensionHeader(ftell(fp1), ExtensionHeader);	
                    break;						
                case SEQUENCE_END_CODE:
                    DBG_INFO("\nSequenceEnd=[0x%x, 0x%lx]", (StartCode & B8BIT), ftell(fp1));
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

void MPEG2ToolParseH264()
{
    FILE *fp1;
    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
    {
        DBG_ERR("\nfp1 error open\n");
        goto FINISH;
    }

    int temp, temp_ts_packet[188];
    int i = 0, StartCode = 0, NALType = 0;
    DBG_INFO("\n============Collect_H264_header===============\n");
	
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
                StartCode = (temp_ts_packet[i]<<16)|(temp_ts_packet[i+1]<<8)|(temp_ts_packet[i+2]);
                if(H264_NAL_START_CODE == StartCode)
                {
			NALType = (temp_ts_packet[i+3] & BIT01234);
			DBG_INFO("\n  [000001%02x-%d] 0x%lx", temp_ts_packet[i+3], NALType, ftell(fp1));	
			PrintH264NALType(NALType);
			//if(NALType>=16 && NALType<=23)
			    		
                }
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
        DBG_ERR("\nfp1 error open\n");
        goto FINISH;
    }

    int temp, temp_ts_packet[188];
    int i = 0, StartCode = 0, NALType = 0;
    DBG_INFO("\n============Collect_HEVC_header===============\n");
	
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
                if(HEVC_NAL_START_CODE == StartCode)
                {
			NALType = (temp_ts_packet[i+4] & BIT123456)>>1;
			PrintHEVCNALType(NALType);
			if(NALType>=16 && NALType<=23)
			    DBG_INFO("  [000001%02x-%d] 0x%lx", temp_ts_packet[i+4], NALType, ftell(fp1));			
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
		case PARSE_H264:
			MPEG2ToolParseH264();
			break;				
		case PARSE_HEVC:
			MPEG2ToolParseHEVC();
			break;
		default:
			break;				
	}
	return;
}

void MPEG2AddSEI(void) 
{
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    int location=0;
    int temp, temp1, temp2, temp3, temp4;

    if((fp1 = fopen(MPEG2ToolGetInputPath(),"rb")) == NULL)
    {
        DBG_ERR("\nfp1 error open\n");
        goto FINISH;
    }
    if((fp2 = fopen(MPEG2ToolGetOutputPath(),"wb+")) == NULL)
    {
        DBG_ERR("\nfp2 error open\n");
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
            DBG_INFO("\n ============find out [%d-0x%x]============= ", location,  temp);
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


