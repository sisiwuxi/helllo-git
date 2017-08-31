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

//=================parserTS====================//
#define PAT_PID (0x0000)
#define PID_MAX_NUM (20)
#define INVALID_PID 0xffffffff


#if 1
#define INFO printf
#else
#define INFO //printf
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

