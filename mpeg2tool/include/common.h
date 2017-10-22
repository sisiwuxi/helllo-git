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
#define BIT01234 0x1F
#define B5BIT 0x01f
#define B76BIT 0x0c0
#define B54BIT 0x030
#define B4BIT 0x0f
#define B12345BIT 0x3e
#define B0BIT 0x01
#define B8BIT ((~(~0))|0xff)
//=================ParseType====================//
typedef enum
{
    PARSE_PIDs = 0,
    PARSE_MPEG2,
    PARSE_H264,    
    PARSE_HEVC,
    PARSE_NUM,
}_ParseType;
//=================CodecType====================//
typedef enum
{
    CODEC_TYPE_MPEG2 = 0,
    CODEC_TYPE_HEVC,
    CODEC_TYPE_NUM,
}_CodecType;

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
//=================H264====================//
#define H264_NAL_START_CODE 0x000001
enum {
    H264_NAL_TYPE_NONE = 0,
    H264_NAL_TYPE_SLICE = 1,
    H264_NAL_TYPE_DPA = 2,
    H264_NAL_TYPE_DPB = 3,
    H264_NAL_TYPE_DPC = 4,
    H264_NAL_TYPE_IDR = 5,
    H264_NAL_TYPE_SEI = 6,
    H264_NAL_TYPE_SPS = 7,
    H264_NAL_TYPE_PPS = 8,
    H264_NAL_TYPE_AUD = 9,
    H264_NAL_TYPE_EOSEQ = 10,
    H264_NAL_TYPE_EOSTREAM = 11,
    H264_NAL_TYPE_FILL = 12,
    H264_NAL_TYPE_SEQPSE = 13,
    H264_NAL_TYPE_PNU = 14,
    H264_NAL_TYPE_SSPS = 15,
    H264_NAL_TYPE_R16 = 16,
    H264_NAL_TYPE_R17 = 17,
    H264_NAL_TYPE_R18 = 18,
    H264_NAL_TYPE_CS = 19,
    H264_NAL_TYPE_CSE = 20,
    H264_NAL_TYPE_R21 = 21,
    H264_NAL_TYPE_R22 = 22,
    H264_NAL_TYPE_R23 = 23,
    H264_NAL_TYPE_U24 = 24,
    H264_NAL_TYPE_U25 = 25,
    H264_NAL_TYPE_U26 = 26,
    H264_NAL_TYPE_U27 = 27,
    H264_NAL_TYPE_U28 = 28,
    H264_NAL_TYPE_U29 = 29,
    H264_NAL_TYPE_U30 = 30,
    H264_NAL_TYPE_U31 = 31
} H264NALType;
//=================HEVC====================//
#define HEVC_NAL_START_CODE 0x000001

enum {
    HEVC_NAL_TYPE_TRAIL_N = 0,
    HEVC_NAL_TYPE_TRAIL_R = 1,
    HEVC_NAL_TYPE_TSA_N = 2,
    HEVC_NAL_TYPE_TSA_R = 3,
    HEVC_NAL_TYPE_STSA_N = 4,
    HEVC_NAL_TYPE_STSA_R = 5,
    HEVC_NAL_TYPE_RADL_N = 6,
    HEVC_NAL_TYPE_RADL_R = 7,
    HEVC_NAL_TYPE_RASL_N = 8,
    HEVC_NAL_TYPE_RASL_R = 9,
    HEVC_NAL_TYPE_VCL_N10 = 10,
    HEVC_NAL_TYPE_VCL_N12 = 12,
    HEVC_NAL_TYPE_VCL_N14 = 14,
    HEVC_NAL_TYPE_VCL_N11 = 11,
    HEVC_NAL_TYPE_VCL_N13 = 13,
    HEVC_NAL_TYPE_VCL_N15 = 15,
    HEVC_NAL_TYPE_BLA_W_LP = 16,
    HEVC_NAL_TYPE_BLA_W_RADL = 17,
    HEVC_NAL_TYPE_BLA_N_LP = 18,
    HEVC_NAL_TYPE_IDR_W_RADL = 19,
    HEVC_NAL_TYPE_IDR_N_LP = 20,
    HEVC_NAL_TYPE_CRA_NU = 21,
    HEVC_NAL_TYPE_RSV_IRAP_VCL22 = 22,
    HEVC_NAL_TYPE_RSV_IRAP_VCL23 = 23,
    HEVC_NAL_TYPE_RSV_VCL24 = 24,
    HEVC_NAL_TYPE_RSV_VCL25 = 25,
    HEVC_NAL_TYPE_RSV_VCL26 = 26,
    HEVC_NAL_TYPE_RSV_VCL27 = 27,
    HEVC_NAL_TYPE_RSV_VCL28 = 28,
    HEVC_NAL_TYPE_RSV_VCL29 = 29,
    HEVC_NAL_TYPE_RSV_VCL30 = 30,
    HEVC_NAL_TYPE_RSV_VCL31 = 31,
    HEVC_NAL_TYPE_VPS_NUT = 32,
    HEVC_NAL_TYPE_SPS_NUT =  33,
    HEVC_NAL_TYPE_PPS_NUT =  34,
    HEVC_NAL_TYPE_AUD_NUT =  35,
    HEVC_NAL_TYPE_EOS_NUT =  36,
    HEVC_NAL_TYPE_EOB_NUT = 37,
    HEVC_NAL_TYPE_FD_NUT = 38,
    HEVC_NAL_TYPE_PREFIX_SEI_NUT = 39,
    HEVC_NAL_TYPE_SUFFIX_SEI_NUT = 40,
    HEVC_NAL_TYPE_RSV_NVCL41 = 41,
    HEVC_NAL_TYPE_RSV_NVCL42 = 42,
    HEVC_NAL_TYPE_RSV_NVCL43 = 43,
    HEVC_NAL_TYPE_RSV_NVCL44 = 44,
    HEVC_NAL_TYPE_RSV_NVCL45 = 45,
    HEVC_NAL_TYPE_RSV_NVCL46 = 46,
    HEVC_NAL_TYPE_RSV_NVCL47 = 47,
    HEVC_NAL_TYPE_UNSPEC48 = 48,
    HEVC_NAL_TYPE_UNSPEC49 = 49,
    HEVC_NAL_TYPE_UNSPEC50 = 50,
    HEVC_NAL_TYPE_UNSPEC51 = 51,
    HEVC_NAL_TYPE_UNSPEC52 = 52,
    HEVC_NAL_TYPE_UNSPEC53 = 53,
    HEVC_NAL_TYPE_UNSPEC54 = 54,
    HEVC_NAL_TYPE_UNSPEC55 = 55,
    HEVC_NAL_TYPE_UNSPEC56 = 56,
    HEVC_NAL_TYPE_UNSPEC57 = 57,
    HEVC_NAL_TYPE_UNSPEC58 = 58,
    HEVC_NAL_TYPE_UNSPEC59 = 59,
    HEVC_NAL_TYPE_UNSPEC60 = 60,
    HEVC_NAL_TYPE_UNSPEC61 = 61,
    HEVC_NAL_TYPE_UNSPEC62 = 62,
    HEVC_NAL_TYPE_UNSPEC63 = 63
} HEVCNALType;
//=================parserTS====================//
#define PAT_PID (0x0000)
#define PID_MAX_NUM (20)
#define INVALID_PID 0xffffffff


///ASCII color code
#define ASCII_COLOR_RED                          "\033[1;31m"
#define ASCII_COLOR_WHITE                       "\033[1;37m"
#define ASCII_COLOR_YELLOW                   "\033[1;33m"
#define ASCII_COLOR_BLUE                         "\033[1;36m"
#define ASCII_COLOR_GREEN                      "\033[1;32m"
#define ASCII_COLOR_END                          "\033[0m"

typedef enum
{
    TOOL_DBG_NONE = 0,
    TOOL_DBG_ERR, 		
    TOOL_DBG_WRN,
    TOOL_DBG_INFO,
    TOOL_DBG_ALL
}TOOL_DBG_LEVEL_e;

#if 1
// Who want to use DBG_INFO for debugging should define DBG_LEVEL by itself.
#define TOOL_PRINT(fmt, args...)      ({do{printf(fmt, ##args);}while(0);})
#define DBG_ERR(fmt, args...)       ({do{if(DBG_LEVEL>=TOOL_DBG_ERR){printf(ASCII_COLOR_RED"[MI ERR ]: %s[%d]: ", __FUNCTION__,__LINE__);printf(fmt, ##args);printf(ASCII_COLOR_END);}}while(0);})
#define DBG_WRN(fmt, args...)       ({do{if(DBG_LEVEL>=TOOL_DBG_WRN){printf(ASCII_COLOR_YELLOW);printf(fmt, ##args);printf(ASCII_COLOR_END);}}while(0);})
#define DBG_INFO(fmt, args...)      ({do{if(DBG_LEVEL>=TOOL_DBG_INFO){printf(ASCII_COLOR_GREEN);printf(fmt, ##args);printf(ASCII_COLOR_END);}}while(0);})
#define DBG_PPID_INFO(fmt, args...)	({do{if(DBG_LEVEL>=TOOL_DBG_INFO){printf(ASCII_COLOR_GREEN"[MI PPID INFO]: ");printf("[CallingPId :%d]",IPCThreadState::self()->getCallingPid());printf(fmt, ##args);printf(ASCII_COLOR_END);}}while(0);})
#define DBG_ENTER()                 ({do{if(DBG_LEVEL>=TOOL_DBG_ALL){printf(ASCII_COLOR_BLUE">>>%s[%d] \n",__FUNCTION__,__LINE__);printf(ASCII_COLOR_END);}}while(0);})
#define DBG_EXIT_OK()               ({do{if(DBG_LEVEL>=TOOL_DBG_ALL){printf(ASCII_COLOR_BLUE"<<<%s[%d] \n",__FUNCTION__,__LINE__);printf(ASCII_COLOR_END);}}while(0);})
#else
#define MI_PRINT(fmt, args...)
#define DBG_ERR(fmt, args...)
#define DBG_WRN(fmt, args...)
#define DBG_INFO(fmt, args...)
#define DBG_PPID_INFO(fmt, args...)
#define DBG_ENTER()
#define DBG_EXIT_OK()
#endif

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

