#include  <stdio.h>
#include  <stdlib.h>
#include  <limits.h>

void  find_out_000001(FILE *fp);
void  char_print(unsigned char a);
void  arbitrary_bit_print(unsigned int a, int n);
void  sequence_display_extension(FILE  *fp);
void  sequence_scalable_extension(FILE *fp);


/*!                 测试???
************************************************************************************************
  extension_data(0):
************************************************************************************************
*/
void  extension_data0(FILE *fp)
{
    unsigned char extension_start_code_identifier = 0;       //4-bits
   
    extension_start_code_identifier = fgetc(fp);
	extension_start_code_identifier = (extension_start_code_identifier & 240) >> 4;
	
	fseek(fp, -1, SEEK_CUR);                                 //adjust file pointer -- 00 00 01 B5
	
	if (extension_start_code_identifier == 2)                //b'0010
		sequence_display_extension(fp);
	else if(extension_start_code_identifier == 5)            //b'0101
		sequence_scalable_extension(fp);
	else {
		printf("Not extension_data(0)!\n");
		exit(1);
	}

}



/*!         
***********************************************************************************************
  user_data():
***********************************************************************************************
*/
void  user_data(FILE *fp)
{
   int  user_a, user_b;
   int  i;

   user_a = ftell(fp);
   find_out_000001(fp);
   fseek(fp, -3, SEEK_CUR);
   user_b = ftell(fp);
   fseek(fp, user_a - user_b, SEEK_CUR);

   printf("\n ----------  user_data()  ----------\n");
   for (i = 1; i <= user_b - user_a; ++i) {
       printf("%.2X  ", fgetc(fp));
	   if (i % 10 ==0)
		   putchar('\n');
   }
   putchar('\n');
}



/*!             参数的bits显示;  三个克选参数的详细显示?;   测试
************************************************************************************************            
 sequence_display_extension():     5 or 8 bits
************************************************************************************************
*/
void  sequence_display_extension(FILE  *fp)
{
   unsigned char  byte1;
   unsigned char  byte2 = 0, byte3 = 0, byte4 = 0;
   unsigned char  byte5, byte6, byte7, byte8;

   unsigned char  extension_start_code_identifier = 0;       //4-bits
   unsigned char  video_format = 0;                          //3-bits
   unsigned char  colour_description = 0;                    //1-bit
   unsigned int   display_horizontal_size = 0;               //14-bits
   unsigned char  marker_bit = 0;                            //1-bit
   unsigned int   display_vertical_size = 0;                 //14-bits

   unsigned char  colour_primaries = 0;                      //8-bits
   unsigned char  transfer_characteristices = 0;
   unsigned char  matrix_coefficients = 0;

                   /***   read raw-bytes   ***/ 
   byte1 = fgetc(fp);      
   if (byte1 & 1) {
       byte2 = fgetc(fp);    
       byte3 = fgetc(fp);      
	   byte4 = fgetc(fp);
   }
   byte5 = fgetc(fp);   byte6 = fgetc(fp);   
   byte7 = fgetc(fp);   byte8 = fgetc(fp);

                   /***  analyze parameter  ***/
   extension_start_code_identifier = (byte1 & 240) >> 4;     //mask 240 = b'1111_0000
   video_format = (byte1 & 14) >> 1;                         //mask 14  = b'0000_1110
   colour_description = (byte1 & 1) ? 1 : 0;                 //mask 1   = b'0000_0001
   if (colour_description) {
       colour_primaries = byte2;
	   transfer_characteristices = byte3;
       matrix_coefficients = byte4;
   }
   display_horizontal_size = byte5;
   display_horizontal_size = (display_horizontal_size << 6) | ((byte6 & 252) >> 2);
   marker_bit = (byte6 & 2) ? 1 : 0;
   display_vertical_size = byte6 & 1;
   display_vertical_size = (display_vertical_size << 8) | byte7;
   display_vertical_size = (display_vertical_size << 5) | ((byte8 & 248) >> 3);

                   /***   display parameter  ***/
   printf("\n ----------   sequence_display_extension()   ----------\n");
   printf("%.2X  ", byte1);   
   if (colour_description) {
       printf("%.2X  ", byte2);
	   printf("%.2X  ", byte3);
       printf("%.2X  ", byte4);
   }
   printf("%.2X  ", byte5);   printf("%.2X  ", byte6); 
   printf("%.2X  ", byte7);   printf("%.2X  ", byte8);  putchar('\n');
   
   char_print(byte1);   
   if (colour_description) {
      char_print(byte2);
	  char_print(byte3);        
	  char_print(byte4);
   }
   char_print(byte5);        char_print(byte6);
   char_print(byte7);        char_print(byte8); putchar('\n');
                 //extension_start_code_identifier
   if (extension_start_code_identifier == 2)
	   printf("extension_start_code_identifier:  %s\n", "序列显示扩展ID");
   else {
       printf("Not sequence_display_extension()!\n");
	   exit(1);
   }
                //video_format
   printf("video_format:  ");
   switch (video_format) {
   case 0:
	   printf("%s\n", "分量");             break;
   case 1:
	   printf("PAL\n");                    break;
   case 2:
	   printf("NTSC\n");                   break;
   case 3:
	   printf("SECAM\n");                  break;
   case 4:
	   printf("MAC\n");                    break;
   case 5 :
	   printf("%s\n", "未规定视频格式");   break;
   case 6:
   case 7:  
	   printf("reserved!\n");              break;
   default:
	   printf("video_format beyond range! -- Please check program!\n");
	   exit(1);
   }
   
   printf("colour_description: %d\n", colour_description);
   
   if (colour_description) {
       printf("colour_primaries:  ");
	   switch (colour_primaries) {
	   case 0:
		   printf("Prohibit!\n");  break;
	   case 1:
		   printf("ITU-R recommend 709(1990)\n");  break;
	   case 2:
		   printf("未规定的视频\n");
		   printf("未知图象特性\n");  break;
	   case 3:
		   printf("reserved!\n");  break;
	   case 4:
		   printf("ITU-R recommend 624-4 系统M\n");  break;
	   case 5:
		   printf("ITU-R recommend 624-4 系统 B,G\n");  break;
	   case 6:
		   printf("SMPTE\n");  break;
	   case 7:
		   printf("SMPTE  240M(1987)\n");  break;
	   default:
		   printf("reserved!\n");
	   } 
   
	   printf("transfer_characteristices:  ");
	   switch (transfer_characteristices) {
	   case 0:
		   printf("reserved!\n");  break;
	   case 1:
		   printf("ITU-R recommend 709(1990)\n");  break;
	   case 2:
		   printf("未规定的视频\n");
		   printf("未知图象特性\n");  break;
	   case 3:
           printf("reserved!\n");  break;
	   case 4:
           printf("ITU-R recommend 624-4 系统M\n");  
		   printf("  假设显示格玛2.8\n");   break;
	   case 5:
		   printf("ITU-R recommend 624-4 系统 B,G\n");  
		   printf("                            假设显示格玛2.8\n");   break;
	   case 6:
           printf("SMPTE 170M\n");  break;
	   case 7:
           printf("SMPTE  240M(1987)\n");  break;
	   case 8:
		   printf("线性变换特性\n");  break;
	   default:
           printf("reserved!\n");  break;
	   }

	   printf("matrix_coefficients:  ");
	   switch (matrix_coefficients) {
	   case 0: 
           printf("Prohibit!\n");  break;
	   case 1:
	       printf("ITU-R recommend 709(1990)\n");  break;
	   case 2:
	       printf("未规定的视频\n");
		   printf("未知图象特性\n");  break;
	   case 3:
		   printf("reserved!\n");  break;
	   case 4:
		   printf("FCC\n"); break;
	   case 5:
		   printf("ITU-R recommend 624-4 系统 B,G\n");  break;
	   case 6:
           printf("SMPTE 170M\n");  break;
	   case 7:
           printf("SMPTE  240M(1987)\n");  break;
	   default:
           printf("reserved!\n");  
	   }
   }
   
   printf("display_horizontal_size:  %d\n", display_horizontal_size);
   printf("marker_bit:  %d\n", marker_bit);
   printf("display_vertical_size:  %d\n", display_vertical_size);
}



/*!                   参数的bits显示;    测试
************************************************************************************************
   sequence_scalable_extension():         2 or 3 or 8 bits
************************************************************************************************
*/
void  sequence_scalable_extension(FILE *fp)
{
   unsigned char byte1, byte2;
   unsigned char byte3 = 0, byte4 = 0, byte5 = 0;
   unsigned char byte6 = 0, byte7 = 0, byte8 = 0;
   
   unsigned char extension_start_code_identifier = 0;        //4-bits
   unsigned char scalable_mode = 0;                          //2-bits
   unsigned char layer_id = 0;                               //4-bits
   
   unsigned int  lower_layer_prediction_horizontal_size = 0; //14-bits
   unsigned char marker_bit = 0;                             //1-bit
   unsigned int  lower_layer_prediction_vertical_size = 0;   //14-bits
   unsigned char horizontal_subsampling_factor_m = 0;        //5-bits
   unsigned char horizontal_subsampling_factor_n = 0;
   unsigned char vertical_subsampling_factor_m = 0;
   unsigned char vertical_subsampling_factor_n = 0;

   unsigned char picture_mux_enable = 0;                     //1-bit
   unsigned char mux_to_progressive_sequence = 0;
   unsigned char picture_mux_order = 0;                      //3-bits
   unsigned char picture_mux_factor = 0;

                    /***   read raw-byte   ***/
   byte1 = fgetc(fp);   byte2 = fgetc(fp);
   if (((byte1 & 12) >> 2) == 1) {
      byte3 = fgetc(fp);    byte4 = fgetc(fp);
	  byte5 = fgetc(fp);    byte6 = fgetc(fp);
	  byte7 = fgetc(fp);    byte8 = fgetc(fp);
   }
   if (((byte1 & 12) >> 2) == 3)
	   byte3 = fgetc(fp);

                    /***   analyze parameters   ***/
   extension_start_code_identifier = byte1 & 240;            //mask 240 = b'1111_0000
   extension_start_code_identifier = extension_start_code_identifier >> 4;
   scalable_mode = (byte1 & 12) >> 2;                        //mask 12 = b'0000_1100
   layer_id = (byte1 & 3) << 2;
   layer_id = layer_id | ((byte2 & 192) >> 6);               //mask 192 = b'1100_0000
   
   if (scalable_mode == 1) {
       lower_layer_prediction_horizontal_size = byte2 & 63;  //mask 63 = b'0011_1111
	   lower_layer_prediction_horizontal_size = (lower_layer_prediction_horizontal_size << 8) | byte3;
	   marker_bit = (byte4 & 128) ? 1 : 0;                   //mask 128 = b'1000_0000                     
	   lower_layer_prediction_vertical_size = (byte4 & 127) << 7;
	   lower_layer_prediction_vertical_size = lower_layer_prediction_vertical_size | ((byte5 & 254) >> 1);
       horizontal_subsampling_factor_m = (byte5 & 1) << 4;
	   horizontal_subsampling_factor_m = horizontal_subsampling_factor_m | ((byte6 & 240) >> 4);
       horizontal_subsampling_factor_n = (byte6 & 15) << 1;
	   horizontal_subsampling_factor_n = horizontal_subsampling_factor_n | ((byte7 & 128) >> 7);
       vertical_subsampling_factor_m = (byte7 & 124) >> 2;
       vertical_subsampling_factor_n = (byte7 & 3) << 3;
	   vertical_subsampling_factor_n = vertical_subsampling_factor_n | ((byte8 & 224) >> 5);
   }

   if (scalable_mode == 3) {
       picture_mux_enable = (byte2 & 32) ? 1 : 0;
	   if (picture_mux_enable) {
           mux_to_progressive_sequence = (byte2 & 16) ? 1 : 0;
	       picture_mux_order = (byte2 & 14) >> 1;
           picture_mux_factor = (byte2 & 1) << 2;
		   picture_mux_factor = picture_mux_factor | ((byte3 & 192) >> 6);
	   }
	   else {
	       picture_mux_order = (byte2 & 28) >> 2;
           picture_mux_factor = (byte2 & 3) << 1;
		   picture_mux_factor = picture_mux_factor | ((byte3 & 128) >> 7);
	   }
   }

                   /***   display parameters   ***/
   printf("\n ----------   sequence_scalable_extension()   ----------\n");
   printf("%.2X  ", byte1);    printf("%.2X  ", byte2);  putchar('\n');
   if (scalable_mode == 1) {
       printf("%.2X  ", byte3);    printf("%.2X  ", byte4);
	   printf("%.2X  ", byte5);    printf("%.2X  ", byte6);
	   printf("%.2X  ", byte7);    printf("%.2X  ", byte8);    putchar('\n');
   }
   if (scalable_mode == 3) {
       printf("%.2X  ", byte3);    putchar('\n');
   }
   
   char_print(byte1);    char_print(byte2);      putchar('\n');
   if (scalable_mode == 1) {
       char_print(byte3);    char_print(byte4);
	   char_print(byte5);    char_print(byte6);
	   char_print(byte7);    char_print(byte8);  putchar('\n');
   }
   if (scalable_mode == 3) 
       char_print(byte3);    putchar('\n');
         //show parameters
   if (extension_start_code_identifier == 5)
	   printf("extension_start_code_identifier:  %s\n", "序列分级扩展ID");
   else {
       printf("Not sequence_scalable_extension()!\n");
	   exit(1);
   }
   
   printf("scalable_mode:  ");
   switch (scalable_mode) {
   case 0: 
	   printf("%s\n", "数据分割");  break;
   case 1:
       printf("%s\n", "空域分级");  break;
   case 2:
       printf("%s\n", "SNR分级");   break;
   case 3:
	   printf("%s\n", "时域分级");  break;
   default:
	   printf("scalable_mode beyond the range!\n");
	   exit(1);
   }

   printf("layer_id:  ");
   arbitrary_bit_print(layer_id, 4);  putchar('\n');

   if (scalable_mode == 1) {
       printf("lower_layer_prediction_horizontal_size:  ");
         arbitrary_bit_print(lower_layer_prediction_horizontal_size, 14);  putchar('\n');
       printf("marker_bit:  %d\n", marker_bit);
       printf("lower_layer_prediction_vertical_size:  ");
         arbitrary_bit_print(lower_layer_prediction_vertical_size, 14);    putchar('\n');
       printf("horizontal_subsampling_factor_m:  ");
         arbitrary_bit_print(horizontal_subsampling_factor_m, 5);          putchar('\n');
       printf("horizontal_subsampling_factor_n:  ");
         arbitrary_bit_print(horizontal_subsampling_factor_n, 5);          putchar('\n');
	   printf("vertical_subsampling_factor_m:  ");
         arbitrary_bit_print(vertical_subsampling_factor_m, 5);            putchar('\n');
	   printf("vertical_subsampling_factor_n:  ");
         arbitrary_bit_print(vertical_subsampling_factor_n, 5);            putchar('\n');
   }

   if (scalable_mode == 3) {
	   printf("picture_mux_enable:  %d\n", picture_mux_enable);
	   if (picture_mux_enable)
		   printf("mux_to_progressive_sequence:  %d\n", mux_to_progressive_sequence);
       printf("picture_mux_order:  ");
         arbitrary_bit_print(picture_mux_order, 3);   putchar('\n');
       printf("picture_mux_factor:  ");
         arbitrary_bit_print(picture_mux_factor, 3);  putchar('\n');
   }
}

