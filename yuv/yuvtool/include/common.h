/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/08/06                                       */
/************************************************************/

#if 1
#define INFO printf
#else
#define INFO //printf
#endif

#if 1
#define DEBUG printf
#else
#define DEBUG //printf
#endif

#define ERR printf
#define PATH_MAX_NUM 256

#define   COLORy  128
#define   COLORu  128
#define   COLORv   255

typedef struct YUVTool_s
{
    char input_path[PATH_MAX_NUM];
    char output_path[PATH_MAX_NUM];
} YUVTool_t;

void YUVToolInit();
void YUVToolDeInit();
void YUVToolSetInputPath(char* input_path);
char* YUVToolGetInputPath();
void YUVToolSetOutputPath(char* output_path);
char* YUVToolGetOutputPath();
void YUVToolNewFile(char* ColorType, char* Width, char* Height, char* ColorY, char* ColorU, char* ColorV);
void YUVTooldeTile(char* Width, char* Height, char* Line_Num, char* Small_Line);

