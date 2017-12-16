#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#include  "analyze_para2.C"
#include  "analyze_extension_and_user_data.C"

int   check_PID(unsigned char head2, unsigned char head3, int *pid_cnt, int *empty_cnt);
void  analyze_TS_head(unsigned char c2, unsigned char c4, unsigned char *isPES, 
					                                      unsigned char *isAdapt); 
void  find_out_000001(FILE *fp);
void  find_out_startcode(FILE *fp, unsigned char startcode);
int   next_is_000001(FILE *fp);
int   next_is_startcode(FILE *fp, unsigned char startcode);
void  char_print(unsigned char a);
void  arbitrary_bit_print(unsigned int a, int n);


//section C functions
void  analyze_para_of_sequence_header(FILE *fp);          //1   ??little
void  analyze_para_of_sequence_extension(FILE *fp);       //2
void  extension_data0(FILE *fp);
void  user_data(FILE *fp);
void  analyze_para_of_group_of_pictures_header(FILE *fp);  //3
void  analyze_para_of_pictures_header(FILE *fp);           //4
void  analyze_para_of_picture_coding_extension(FILE *fp);  //5   ??little

//common 1-5
void  analyze_para_of_picture_coding_extension(FILE  *fp);
void  matrix_processing(FILE *fp);
void  show_matrix(FILE *fp);

//not always
void  extension_data0(FILE *fp);
void  user_data(FILE *fp);
void  sequence_display_extension(FILE  *fp);
void  sequence_scalable_extension(FILE *fp);
