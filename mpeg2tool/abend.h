/************************************************************/
/* Author : sisi.wu                                         */
/* Date   : 2017/7/12                                       */
/* Email  : sisi.wu@mstarsemi.com                           */
/************************************************************/
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
#define PICTURE_START_CODE 0x00000100
#define SLICE_START_CODE 0x00000101
#define SLICE_END_CODE 0x000001AF
#define SEQUENCE_HEADER_CODE 0x000001B3
#define GROUP_START_CODE 0x000001B8
#define B543BIT 0x038

int abend_duration;

void set_input_path(char* input_path);
void set_output_path(char* output_path);
void start_time(char* time);
void abend_stream(char* abend_duration);
