/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/************************************************************/
#define BIT7 0x080
#define BIT6 0x040
#define BIT5 0x020
#define BIT123 0x0E
#define BIT345 0x38
#define BIT4567 0xF0
#define BIT123456 0x7E
#define B5BIT 0x01f
#define B76BIT 0x0c0
#define B54BIT 0x030
#define B4BIT 0x0f
#define B12345BIT 0x3e
#define B0BIT 0x01
#define B8BIT ((~(~0))|0xff)
//=================CodecType====================//

enum
{
    CODEC_TYPE_MPEG2 = 0,
    CODEC_TYPE_HEVC,
    CODEC_TYPE_NUM,
};

//=================MPEG2====================//
#define VIDEO_SEQUENCE_CODE 0x000001e0
#define USERDATA_HEADER_CODE 0x000001b2
#define SEQUENCE_HEADER_CODE 0x000001b3
#define EXTENSION_HEADER_CODE 0x000001b5
#define SEQUENCE_END_CODE 0x000001b7
#define GOP_HEADER_CODE 0x000001b8
#define PICTURE_HEADER_CODE 0x00000100
#define SLICE_START_CODE 0x00000101
#define SLICE_END_CODE 0x000001AF
#define PES_START_CODE 0x000001BC
#define PES_END_CODE 0x000001FF
#define GROUP_START_CODE 0x000001B8
#define B543BIT 0x038
#define PATH_MAX_NUM 256
#define SLICE_HEADER_LEN 16
//=================HEVC====================//
#define NAL_START_CODE 0x000001
#define NAL_TYPE_TRAIL_N 0
#define NAL_TYPE_TRAIL_R 1
#define NAL_TYPE_TSA_N 2
#define NAL_TYPE_TSA_R 3
#define NAL_TYPE_STSA_N 4
#define NAL_TYPE_STSA_R 5
#define NAL_TYPE_RADL_N 6
#define NAL_TYPE_RADL_R 7
#define NAL_TYPE_RASL_N 8
#define NAL_TYPE_RASL_R 9
#define NAL_TYPE_VCL_N10 10
#define NAL_TYPE_VCL_N12 12
#define NAL_TYPE_VCL_N14 14
#define NAL_TYPE_VCL_N11 11
#define NAL_TYPE_VCL_N13 13
#define NAL_TYPE_VCL_N15 15
#define NAL_TYPE_BLA_W_LP 16
#define NAL_TYPE_BLA_W_RADL 17
#define NAL_TYPE_BLA_N_LP 18
#define NAL_TYPE_IDR_W_RADL 19
#define NAL_TYPE_IDR_N_LP 20
#define NAL_TYPE_CRA_NU 21
#define NAL_TYPE_RSV_IRAP_VCL22 22
#define NAL_TYPE_RSV_IRAP_VCL23 23
#define NAL_TYPE_RSV_VCL24 24
#define NAL_TYPE_RSV_VCL25 25
#define NAL_TYPE_RSV_VCL26 26
#define NAL_TYPE_RSV_VCL27 27
#define NAL_TYPE_RSV_VCL28 28
#define NAL_TYPE_RSV_VCL29 29
#define NAL_TYPE_RSV_VCL30 30
#define NAL_TYPE_RSV_VCL31 31
#define NAL_TYPE_VPS_NUT 32
#define NAL_TYPE_SPS_NUT 33
#define NAL_TYPE_PPS_NUT 34
#define NAL_TYPE_AUD_NUT 35
#define NAL_TYPE_EOS_NUT 36
#define NAL_TYPE_EOB_NUT 37
#define NAL_TYPE_FD_NUT 38
#define NAL_TYPE_PREFIX_SEI_NUT 39
#define NAL_TYPE_SUFFIX_SEI_NUT 40
#define NAL_TYPE_RSV_NVCL41 41
#define NAL_TYPE_RSV_NVCL42 42
#define NAL_TYPE_RSV_NVCL43 43
#define NAL_TYPE_RSV_NVCL44 44
#define NAL_TYPE_RSV_NVCL45 45
#define NAL_TYPE_RSV_NVCL46 46
#define NAL_TYPE_RSV_NVCL47 47
#define NAL_TYPE_UNSPEC48 48
#define NAL_TYPE_UNSPEC49 49
#define NAL_TYPE_UNSPEC50 50
#define NAL_TYPE_UNSPEC51 51
#define NAL_TYPE_UNSPEC52 52
#define NAL_TYPE_UNSPEC53 53
#define NAL_TYPE_UNSPEC54 54
#define NAL_TYPE_UNSPEC55 55
#define NAL_TYPE_UNSPEC56 56
#define NAL_TYPE_UNSPEC57 57
#define NAL_TYPE_UNSPEC58 58
#define NAL_TYPE_UNSPEC59 59
#define NAL_TYPE_UNSPEC60 60
#define NAL_TYPE_UNSPEC61 61
#define NAL_TYPE_UNSPEC62 62
#define NAL_TYPE_UNSPEC63 63
//=================parserTS====================//
#define PAT_PID (0x0000)
#define PID_MAX_NUM (20)
#define INVALID_PID 0xffffffff


#if 1
#define INFO printf
#else
#define INFO //printf
#endif

#if 1
#define WEAK_INFO //printf
#else
#define WEAK_INFO //printf
#endif


#if 0
#define DEBUG printf
#else
#define DEBUG //printf
#endif

#define ERR printf

typedef struct MPEG2Tool_s
{
    char input_path[PATH_MAX_NUM];
    char output_path[PATH_MAX_NUM];
    int start_time;
} MPEG2Tool_t;

void MPEG2ToolInit();
void MPEG2ToolDeInit();
void MPEG2ToolSetInputPath(char* input_path);
char* MPEG2ToolGetInputPath();
void MPEG2ToolSetOutputPath(char* output_path);
char* MPEG2ToolGetOutputPath();
void MPEG2ToolSetStartTime(char* time);
int MPEG2ToolGetStartTime();
float MPEG2ToolGetDuration();
void MPEG2ToolConcat(char* input1_path, char* input2_path, char* output_path);

