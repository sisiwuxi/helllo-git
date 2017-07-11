/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/* Email  : sisi.wu@mstarsemi.com                           */
/************************************************************/
#define PATH_MAX_NUM 256

# if 1
#define INFO printf
#else
#define INFO //
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
void MPEG2ToolSetOutputPath(char* output_path);
void MPEG2ToolSetStartTime(char* time);
