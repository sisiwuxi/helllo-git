/*!         1、 验证 matrix ;  5、若干参数显示 
************************************************************************************************
analyze_para2.C:  (have a little differents with analyze_para.C)
       input: file pointer;
	   read raw-bytes, then analyze parameters;
************************************************************************************************
*/
#include  <stdio.h>
#include  <stdlib.h>
#include  <limits.h>

void  char_print(unsigned char a);
void  arbitrary_bit_print(unsigned int a, int n);
void  find_out_000001(FILE *fp);

void  matrix_processing(FILE *fp);
void  show_matrix(FILE *fp);


/*!            ?????????矩阵处理函数
***********************************************************************************************
1、sequence_header():
       input: file pointer  
	   read raw-bytes: 8 + 64 + 64        
	   output: display raw-bytes in Hex、Binary and show parameters   
***********************************************************************************************
*/           
void  analyze_para_of_sequence_header(FILE *fp)
{
	unsigned char  byte1, byte2, byte3, byte4;
	unsigned char  byte5, byte6, byte7, byte8;

    unsigned int   horizontal_size_value = 0;          // 12bits "uimsbf"
	unsigned int   vertical_size_value = 0;    
	unsigned char  aspect_ratio_information = 0;      // 4bits  "uimsbf"
    unsigned char  frame_rate_code = 0;
    unsigned int   bit_rate_value = 0;                 // 18bits "uimsbf"
    unsigned char  marker_bit = 0;                     // 1bit   "bslbf"
    unsigned int   vbv_buffer_size_value = 0 ;         // 10bits "uimsbf"
    unsigned char  constrained_parameters_flag = 0 ;   // 1bit
	unsigned char  load_intra_quantiser_matrix = 0;
	unsigned char  load_non_intra_quantiser_matrix = 0;

	             /***   read raw-bytes   ***/
    byte1 = fgetc(fp);     byte2 = fgetc(fp);
	byte3 = fgetc(fp);     byte4 = fgetc(fp);
	byte5 = fgetc(fp);     byte6 = fgetc(fp);
	byte7 = fgetc(fp);     byte8 = fgetc(fp);
    
                 /***  analyze parameter  ***/
    horizontal_size_value = (byte1 << 4);
	horizontal_size_value = horizontal_size_value | (byte2 >> 4);
    vertical_size_value = byte2 & 15;                        // mask 15 = b'0000_1111
	vertical_size_value = vertical_size_value << 8;
	vertical_size_value = vertical_size_value | byte3;
    aspect_ratio_information = (byte4 >> 4);
    frame_rate_code = byte4 & 15;
    bit_rate_value = byte5;
	bit_rate_value = (bit_rate_value << 8) | byte6;
	bit_rate_value = (bit_rate_value << 2) | (byte7 >> 6);
    marker_bit = (byte7 & 32) ? 1 : 0;                       // mask 32 = b'0010_0000
    vbv_buffer_size_value = byte7 & 31;                      // mask 31 = b'0001_1111
	vbv_buffer_size_value = (vbv_buffer_size_value << 5) | (byte8 >> 3);
    constrained_parameters_flag = (byte8 & 4) ? 1 : 0;       // mask 4  = b'0000_0100
    load_intra_quantiser_matrix = (byte8 & 2) ? 1 : 0;       // mask 2  = b'0000_0010
       //???判断是否调用矩阵
	load_non_intra_quantiser_matrix = (byte8 & 1) ? 1 : 0;   // mask 1  = b'0000_0001
        //???判断是否调用矩阵

    //printf("\n----------  1、sequence_header()  ----------\n");
	printf("\n                 ----------  1、sequence_header()  ----------\n");
	             /***  display raw-bytes and binary  ***/
	printf("%.2X  ", byte1);   printf("%.2X  ", byte2); 
	printf("%.2X  ", byte3);   printf("%.2X  ", byte4);
	printf("%.2X  ", byte5);   printf("%.2X  ", byte6); 
	printf("%.2X  ", byte7);   printf("%.2X  ", byte8);  putchar('\n');
	
    char_print(byte1);        char_print(byte2);
	char_print(byte3);        char_print(byte4);
	char_print(byte5);        char_print(byte6);
	char_print(byte7);        char_print(byte8); putchar('\n');


                 /***  display parameter  ***/
	
	printf("horizontal_size_value  ");  arbitrary_bit_print(horizontal_size_value, 12);
	printf(": %u\n", horizontal_size_value);  
	printf("vertical_size_value    ");   arbitrary_bit_print(vertical_size_value, 12);
    printf(": %u\n", vertical_size_value); 
	    //aspect_ratio_information 显示
	printf("aspect_ratio_information        ");  
	arbitrary_bit_print(aspect_ratio_information, 4); printf(": ");
	switch (aspect_ratio_information) {
	case 0:
        printf("prohibit!\n");    break;
	case 1:
		printf("1.0\n");          break;
	case 2:
		printf("DAR  3/4\n");     break;
	case 3:
        printf("DAR  9/16\n");    break;
	case 4:
		printf("DAR  1/2.21\n");  break;
	default :
		printf("reserved!\n");
	}
	    //frame_rate_code 显示           
	printf("frame_rate_code                 ");
	arbitrary_bit_print(frame_rate_code, 4); printf(": ");
	switch (frame_rate_code) {
	case 0:
		printf("prohibit!\n");    break;
	case 1:
		printf("23.976\n");       break;
	case 2:
		printf("24\n");           break;
	case 3:
		printf("25\n");           break;
	case 4:
		printf("29.97\n");        break;
	case 5:
		printf("30\n");           break;
	case 6:
		printf("50\n");           break;
	case 7:
		printf("59.94\n");        break;
	case 8:
		printf("60\n");           break;
	default :
		printf("reserved!\n");
	}
	
	printf("bit_rate_value  ");  
	arbitrary_bit_print(bit_rate_value, 18);  printf(":\n");
	
	printf("marker_bit  %d:\n", marker_bit);
	
	printf("vbv_buffer_size_value ");  
	arbitrary_bit_print(vbv_buffer_size_value, 10);   printf(":\n");
	
	printf("constrained_parameters_flag     %d:\n", constrained_parameters_flag);
	printf("load_intra_quantiser_matrix     %u:\n", load_intra_quantiser_matrix);
	printf("load_non_intra_quantiser_matrix %u:\n", load_non_intra_quantiser_matrix);
	putchar('\n');  putchar('\n');

	                    /***   matrix processing   ***/
    matrix_processing(fp);
}



/*! 2、sequence_extension():            6bytes      ???显示
***********************************************************************************************
    extension_start_code = 00 00 01 B5
*/  
void  analyze_para_of_sequence_extension(FILE  *fp)
{
    unsigned char  byte1, byte2, byte3;
	unsigned char  byte4, byte5, byte6;

    unsigned char  extension_start_code_identifier = 0;    //4-bits
	unsigned char  profile_and_level_indication = 0;       //8-bits
	unsigned char  progressive_sequence = 0;               //1-bit
	unsigned char  chroma_format = 0;                      //2-bits
	unsigned char  horizontal_size_extension = 0;           
	unsigned char  vertical_size_extension = 0;
	unsigned int   bit_rate_extension = 0;                 //12-bits
	unsigned char  marker_bit = 0;                         //1-bit
	unsigned char  vbv_buffer_size_extension = 0;          //8-bits
	unsigned char  low_delay = 0;                          //1-bit
	unsigned char  frame_rate_extension_n = 0;             //2-bits
	unsigned char  frame_rate_extension_d = 0;             //5-bits
	//next_start_code()  如何处理？？？ ----  不处理 不显示
     
	             /***   read raw-bytes   ***/  
    byte1 = fgetc(fp);     byte2 = fgetc(fp);
	byte3 = fgetc(fp);     byte4 = fgetc(fp);
	byte5 = fgetc(fp);     byte6 = fgetc(fp);

	             /***  analyze parameter  ***/
    extension_start_code_identifier = (byte1 & 240) >> 4;  //mask 240 = b'1111_0000
    profile_and_level_indication = (byte1 & 15) << 4;      //mask 15 = b'0000_1111
    profile_and_level_indication = profile_and_level_indication | ((byte2 & 240) >> 4);
    progressive_sequence = (byte2 & 8) ? 1 : 0;  
    chroma_format = (byte2 & 6) >> 1;                      //mask 6 = b'0000_0110         
    horizontal_size_extension = (byte2 & 1) << 1;          //mask 128 = b'1000_0000
    horizontal_size_extension = horizontal_size_extension | ((byte3 & 128) >> 7);
    vertical_size_extension = (byte3 & 96) >> 5;           //mask 96 = b'0110_0000
    bit_rate_extension = (byte3 & 31) << 7;                //mask 31 = b'0001_1111
    bit_rate_extension = bit_rate_extension | ((byte4 & 254) >> 1);
    marker_bit = (byte4 & 1) ? 1 : 0;
    vbv_buffer_size_extension = byte5;
    low_delay = (byte6 & 128) ? 1 : 0;
    frame_rate_extension_n = (byte6 & 96) >> 5;      
    frame_rate_extension_d = byte6 & 31;
    
	printf("\n               ----------  2、sequence_extension()  ----------\n");
	             /***  display raw-bytes and binary  ***/
	printf("%.2X  ", byte1);   printf("%.2X  ", byte2); 
	printf("%.2X  ", byte3);   printf("%.2X  ", byte4);
	printf("%.2X  ", byte5);   printf("%.2X  ", byte6);   putchar('\n');
	
    char_print(byte1);        char_print(byte2);
	char_print(byte3);        char_print(byte4);
	char_print(byte5);        char_print(byte6);   putchar('\n');  putchar('\n'); 
	
	            /***  display parameter  ***/
	printf("extension_start_code_identifier   ");   
	arbitrary_bit_print(extension_start_code_identifier, 4);  printf(": ");
	if (extension_start_code_identifier == 1)               //b'0001
		printf("%s\n", "序列扩展ID");
	else
		printf("Not sequence_extension()!\n");
           //profile level 显示
    printf("profile_and_level_indication  "); 
	arbitrary_bit_print(profile_and_level_indication, 8);  printf(": ");
	switch ((profile_and_level_indication & 112) >> 4) {
	case 1:
		printf("High Profile\n"); break;
	case 2:
		printf("Spatial Profile\n"); break;
	case 3:
		printf("SNR\n"); break;
	case 4:
		printf("Main Profile\n"); break;
	case 5:
		printf("Simple Profile\n"); break;
	default:
		printf("profile -- reserved!\n");  
	}
	printf("                                        ");
	switch (profile_and_level_indication & 15) {
	case 10:
		printf("Low Level\n");  break;
	case 8:
		printf("Main Level\n"); break;
	case 6:
		printf("HIGH-1440\n");  break;
	case 4:
		printf("High Level\n"); break;
	default:
		printf("level -- reserved!\n");
	} 
    printf("                                        ");
	printf("%s%d\n", "换码位为", (profile_and_level_indication & 128) ? 1 : 0);

	printf("progressive_sequence  %u: ", progressive_sequence);
	if (progressive_sequence)
		printf("frame\n");
	else
		printf("frame & field\n");

	printf("chroma_format "); arbitrary_bit_print(chroma_format, 2); printf("       : ");
	switch (chroma_format) {
	case 0:
		printf("chroma_format -- reserved!\n"); break;
	case 1:
		printf("4 : 2 : 0\n"); break;
	case 2:
		printf("4 : 2 : 2\n"); break;
	case 3: 
		printf("4 : 4 : 4\n"); break;
	default:
		printf("chroma_format -- beyond the range!\n");
	}
	
	printf("horizontal_size_extension  ");  
	arbitrary_bit_print(horizontal_size_extension, 2);  printf(": \n");
    printf("vertical_size_extension    ");
	arbitrary_bit_print(vertical_size_extension, 2);    printf(": \n");
    printf("bit_rate_extension: ");
    arbitrary_bit_print(bit_rate_extension, 12);        putchar('\n');
	
	printf("marker_bit: %u\n", marker_bit);
	printf("vbv_buffer_size_extension: ");
    char_print(vbv_buffer_size_extension);              putchar('\n');
	printf("low_delay:  %u\n", low_delay);
	printf("frame_rate_extension_n: ");
    arbitrary_bit_print(frame_rate_extension_n, 2);     putchar('\n');
	printf("frame_rate_extension_d: ");
    arbitrary_bit_print(frame_rate_extension_d, 5);     putchar('\n');
}



/*! 3、group_of_pictures_header():          4bytes
***********************************************************************************************
     group_start_code = 00 00 01 B8
*/  
void  analyze_para_of_group_of_pictures_header(FILE *fp)
{
	unsigned char  byte1, byte2, byte3, byte4;
    
	unsigned int   time_code = 0;         //25-bits   
	unsigned char  closed_gop = 0;        //1-bit
	unsigned char  broken_link = 0;       //1-bit

	byte1 = fgetc(fp);
	byte2 = fgetc(fp);
	byte3 = fgetc(fp);
	byte4 = fgetc(fp);

    time_code = byte1;
	time_code = time_code << 8;
	time_code = (time_code | byte2) << 8;
	time_code = (time_code | byte3) << 1;
	time_code = time_code | ((byte4 & 128) >> 7);
    closed_gop = (byte4 & 64) ? 1 : 0;
    broken_link = (byte4 & 32) ? 1 : 0;

	printf("\n           ----------  3、group_of_pictures_header()  ----------\n");
	            /***  display raw-bytes and binary  ***/
	printf("%.2X  ", byte1);   printf("%.2X  ", byte2); 
	printf("%.2X  ", byte3);   printf("%.2X  ", byte4);   putchar('\n');
	
    char_print(byte1);        char_print(byte2);
	char_print(byte3);        char_print(byte4);   putchar('\n');  putchar('\n');               
	                
	               /***  display parameter  ***/
    printf("time_code   : ");
    arbitrary_bit_print(time_code, 25);     putchar('\n');
	printf("closed_gop  : %u\n", closed_gop);
	printf("broken_link : %u\n", broken_link);
}




/*! 4、pictures_header():   4 or 5bytes info (I-frame;  extra_information_picture--保留)
***********************************************************************************************
     picture_start_code = 00 00 01 00
*/
void  analyze_para_of_pictures_header(FILE *fp)
{
    unsigned char  byte1, byte2, byte3, byte4;
	unsigned char  byte5 = 0;

	unsigned int   temporal_reference = 0;                 //10-bits
	unsigned char  picture_coding_type = 0;                //3-bits
	unsigned int   vbv_delay = 0;                          //16-bits
	unsigned char  extra_bit_picture = 0;                  //1-bit
	unsigned char  extra_information_picture = 0;          //8-bits
	unsigned char  next_start_code = 0;              //???

    byte1 = fgetc(fp);
	byte2 = fgetc(fp);
	byte3 = fgetc(fp);
	byte4 = fgetc(fp);
	if (byte4 & 4)
		byte5 = fgetc(fp);

    temporal_reference = byte1;                            //mask 192 = b'1100_000  
	temporal_reference = (temporal_reference << 2) | ((byte2 & 192) >> 6);
    picture_coding_type = (byte2 & 56) >> 3;               //mask 56= b'0011_1000
	vbv_delay = (byte2 & 7);                               //mask 7 = b'0000_0111
    vbv_delay = vbv_delay << 8;
    vbv_delay = (vbv_delay | byte3) << 5;
	vbv_delay = vbv_delay | ((byte4 & 248) >> 3);          //mask 248 = b'1111_1000
    extra_bit_picture = (byte4 & 4) ? 1 : 0;               //mask 4 = b'0000_0100
    if (extra_bit_picture == 0)
        next_start_code = (byte4 & 3);                     //mask 3 = b'0000_0011
	else {
	    extra_information_picture = (byte4 & 3); 
		extra_information_picture = extra_information_picture << 6;
		extra_information_picture = extra_information_picture | ((byte5 & 252) >> 2);
		next_start_code = (byte5 & 3);                     //mask 252 = b'1111_1100
	}

	printf("\n               ----------  4、pictures_header()  ----------\n");
                            /***  display raw-bytes and binary  ***/
	printf("%.2X  ", byte1);   printf("%.2X  ", byte2); 
	printf("%.2X  ", byte3);   printf("%.2X  ", byte4);   
	if (extra_bit_picture) 
	   printf("%.2X  ", byte5);
	putchar('\n');

	char_print(byte1);        char_print(byte2);
	char_print(byte3);        char_print(byte4); 
	if (extra_bit_picture) 
		char_print(byte5);
	putchar('\n');  putchar('\n');      

	                           /***  display parameter  ***/
	     //temporal_reference  display
	printf("temporal_reference  ");
	arbitrary_bit_print(temporal_reference, 10);  printf(":    ");
	if (temporal_reference != 0)
		printf("It should be zero!!!");
	putchar('\n');
         
	     //picture_coding_type  display
	printf("picture_coding_type      ");  
	arbitrary_bit_print(picture_coding_type, 3);  printf(": ");
	switch (picture_coding_type) {
	case  0:
		printf("forbidden!!!\n"); break;
	case  1:
		printf("Intra-coding(I)\n"); break;
	case  2:
		printf("Predictive-coding(P)\n"); break;
	case  3:
		printf("Bi-Predictive-coding(B)\n"); break;
	case  4:
		printf("Should not be used!!!\n"); break;
	default :
		printf("reserved!!!\n");
	}

	    //others  display
	printf("vbv_delay  ");
    arbitrary_bit_print(vbv_delay, 16);       printf(": \n");
      
    printf("extra_bit_picture %d:\n", extra_bit_picture);
	if (extra_bit_picture) {
	    printf("extra_information_picture : ");
        arbitrary_bit_print(extra_information_picture, 8);       putchar('\n');
    }
	printf("next_start_code  "); 
    arbitrary_bit_print(next_start_code, 2);  printf(": \n");
}



/*! 5、pictures_coding_extension():     7bytes         ??? 3个参数信息显示
***********************************************************************************************
     extension_start_code = 00 00 01 B5
*/
void  analyze_para_of_picture_coding_extension(FILE  *fp)
{
    unsigned char  byte1, byte2, byte3, byte4, byte5;
	unsigned char  byte6 = 0, byte7 = 0;
 
	unsigned char  extension_start_code_identifier = 0;    //4-bits
    unsigned char  f_code00 = 0;
	unsigned char  f_code01 = 0; 
    unsigned char  f_code10 = 0;
	unsigned char  f_code11 = 0;
	unsigned char  intra_dc_precision = 0;                 //2-bits
	unsigned char  picture_structure = 0;
	unsigned char  top_field_first = 0;                    //1-bit
	unsigned char  frame_pred_frame_dct = 0;
	unsigned char  concealment_motion_vectors = 0;
	unsigned char  q_scale_type = 0;
	unsigned char  intra_vlc_format = 0;
	unsigned char  alternate_scan = 0;
	unsigned char  repeat_first_field = 0;
	unsigned char  chroma_420_type = 0;
	unsigned char  progressive_frame = 0;
	unsigned char  composite_display_flag = 0;
	unsigned char  next_start_code = 0;
	
	unsigned char  v_axis = 0;                             //1-bit
	unsigned char  field_sequence = 0;                     //3-bits
    unsigned char  sub_carrier = 0;                        //1-bit
    unsigned char  burst_amplitude = 0;                    //7-bits
    unsigned char  sub_carrier_phase = 0;                  //8-bits
	               
	byte1 = fgetc(fp);    byte2 = fgetc(fp);
	byte3 = fgetc(fp);    byte4 = fgetc(fp);
	byte5 = fgetc(fp);
	if (byte5 & 64) {
	   byte6 = fgetc(fp);
	   byte7 = fgetc(fp);
	}
                  
	              /***    analyze parameters    ***/
	extension_start_code_identifier = (byte1 & 240);       //mask  240 = b'1111_0000
	extension_start_code_identifier = extension_start_code_identifier >> 4;
    f_code00 = byte1 & 15;                                 //mask  15 = b'0000_1111
    f_code01 = byte2 & 240;
	f_code01 = f_code01 >> 4;
    f_code10 = byte2 & 15;
    f_code11 = byte3 & 240;
	f_code11 = f_code11 >> 4;
    intra_dc_precision = (byte3 & 12) >> 2;                //mask 12 = b'0000_1100
    picture_structure = byte3 & 3;                         //mask 3 = b'0000_0011
	top_field_first = (byte4 & 128) ? 1 : 0;               //mask 128 = b'1000_0000
    frame_pred_frame_dct = (byte4 & 64) ? 1 : 0;           //mask  64 = b'0100_0000
    concealment_motion_vectors = (byte4 & 32) ? 1 : 0;     //mask  32 = b'0010_0000
    q_scale_type = (byte4 & 16) ? 1 : 0;                   //mask  16 = b'0001_0000
    intra_vlc_format = (byte4 & 8) ? 1 : 0;                //mask   8 = b'0000_1000
    alternate_scan = (byte4 & 4) ? 1 : 0;                  //mask   4 = b'0000_0100
    repeat_first_field = (byte4 & 2) ? 1 : 0;              //mask   2 = b'0000_0010
    chroma_420_type = (byte4 & 1) ? 1 : 0;                 //mask   1 = b'0000_0001
    progressive_frame = (byte5 & 128) ? 1 : 0;
    composite_display_flag = (byte5 & 64) ? 1 : 0;
	if (composite_display_flag) {
		v_axis = (byte5 & 32) ? 1 : 0;                     //mask  32 = b'0010_0000
	    field_sequence = (byte5 & 28) >> 2;                //mask  28 = b'0001_1100
        sub_carrier = (byte5 & 2) ? 1 : 0;                 //mask   2 = b'0000_0010
        burst_amplitude = (byte5 & 1) << 6;                //mask   1 = b'0000_0001
		burst_amplitude = burst_amplitude | ((byte6 & 252) >> 2);
		sub_carrier_phase = (byte6 & 3) << 6;
		sub_carrier_phase = sub_carrier_phase | ((byte7 & 252) >> 2);
	    next_start_code = byte7 & 3;
	}
	else
        next_start_code = byte5 & 63;

	printf("\n          ----------  5、pictures_coding_extension()  ----------\n");
                   /***  display raw-bytes and binary  ***/
	printf("%.2X  ", byte1);   printf("%.2X  ", byte2); 
	printf("%.2X  ", byte3);   printf("%.2X  ", byte4);
	printf("%.2X  ", byte5); 
	if (composite_display_flag) {
	    printf("%.2X  ", byte6); 
		printf("%.2X  ", byte7); 
	}
    putchar('\n');
	
    char_print(byte1);        char_print(byte2);
	char_print(byte3);        char_print(byte4);   
	char_print(byte5);
	if (composite_display_flag) {
	   char_print(byte6);
	   char_print(byte7);
	}
	putchar('\n');  putchar('\n'); 

                            /***  display parameter  ***/
          //extension_start_code_identifier  display
	printf("extension_start_code_identifier  "); 
	arbitrary_bit_print(extension_start_code_identifier, 4); printf(": ");
    if (extension_start_code_identifier == 8)            //b'1000
		printf("%s\n", "图象编码扩展ID");
	else
		printf("Not pictures_coding_extension()!\n");
          //f_code[][]  display
	printf("f_code00  "); 
	arbitrary_bit_print(f_code00, 4);  printf(":\n");
	printf("f_code01  "); 
	arbitrary_bit_print(f_code01, 4);  printf(":\n");
	printf("f_code10  "); 
	arbitrary_bit_print(f_code10, 4);  printf(":\n");
	printf("f_code11  "); 
	arbitrary_bit_print(f_code11, 4); printf(":\n");
          //intra_dc_precision、picture_structure   display
	printf("intra_dc_precision ");  
	arbitrary_bit_print(intra_dc_precision, 2); printf(": ");
	switch (intra_dc_precision) {
	case  0:
		printf("8 bit\n");  break;
	case  1:
		printf("9 bit\n");  break;
	case  2:
        printf("10 bit\n");  break;
	case  3:
        printf("11 bit\n");  break;
	default:
		printf("intra_dc_precision beyond the range!!!\n");
	}
    printf("picture_structure  ");
	arbitrary_bit_print(picture_structure, 2); printf(": ");
	switch (picture_structure) {
	case  0:
		printf("reserved\n");      break;
	case  1: 
		printf("top-field\n");     break; 
	case  2:
		printf("bottom-field\n");  break;
	case  3:
		printf("frame\n");         break;
	default:
		printf("picture_structure is wrong!!!\n");
	}
           //others  display
	printf("top_field_first : %d\n", top_field_first);   //?????显示其他信息
	printf("frame_pred_frame_dct : %d\n", frame_pred_frame_dct);
	printf("concealment_motion_vectors : %d\n", concealment_motion_vectors);
	printf("q_scale_type : %d\n", q_scale_type);
	printf("intra_vlc_format : %d\n", intra_vlc_format);
	printf("alternate_scan : %d\n", alternate_scan);
	printf("repeat_first_field : %d\n", repeat_first_field);  //?????显示其他信息
	printf("chroma_420_type : %d\n", chroma_420_type);
	printf("progressive_frame : %d\n", progressive_frame);    //?????显示其他信息
	printf("composite_display_flag : %d\n", composite_display_flag);  
	      //others2  display
	if (composite_display_flag) {
       printf("v_axis : %d\n", v_axis);
	   printf("field_sequence : ");
	     arbitrary_bit_print(field_sequence, 3);     putchar('\n');
	   printf("sub_carrier : %d\n", sub_carrier);
	   printf("burst_amplitude : ");
         arbitrary_bit_print(burst_amplitude, 7);    putchar('\n');
	   printf("sub_carrier_phase : ");
	     arbitrary_bit_print(sub_carrier_phase, 8);  putchar('\n');
	   printf("next_start_code : ");
	     arbitrary_bit_print(next_start_code, 2);    putchar('\n');
	}
    else {
	   printf("next_start_code : ");
	     arbitrary_bit_print(next_start_code, 6);    putchar('\n');
	}
}



/*!           ???验证 矩阵函数
************************************************************************************************
   matrix_processing():
************************************************************************************************
*/
void  matrix_processing(FILE *fp)
{
   unsigned int  position_a, position_b;

   position_a = ftell(fp);
   find_out_000001(fp);
   fseek(fp, -3, SEEK_CUR);
   position_b = ftell(fp);
   fseek(fp, position_a - position_b, SEEK_CUR);
   if ((position_b - position_a) == 0)
	    ;                                           //??? ";" or "return"
   else if ((position_b - position_a) == 64)
	   show_matrix(fp);
   else if ((position_b - position_a) == 128) {
       show_matrix(fp);
	   putchar('\n');   putchar('\n');
	   show_matrix(fp);
   }
   else {
       printf("Please check sequence_header()!\n");
       printf("It is not 0 or 64 or 128!\n");
       exit(1);
   } 
}


/*!
**********************************************************************************************
   show_matrix():
**********************************************************************************************
*/
void  show_matrix(FILE *fp)      //edit by ckx -- 070120
{
    unsigned int  i;
	unsigned char c[64] = {0};

    c[0]  = fgetc(fp);    c[1]  = fgetc(fp);
	c[8]  = fgetc(fp);    c[16] = fgetc(fp);
    c[9]  = fgetc(fp);    c[2]  = fgetc(fp);
	c[3]  = fgetc(fp);    c[10] = fgetc(fp);
	c[17] = fgetc(fp);    c[24] = fgetc(fp);
	c[32] = fgetc(fp);    c[25] = fgetc(fp);
	c[18] = fgetc(fp);    c[11] = fgetc(fp);
	c[4]  = fgetc(fp);    c[5]  = fgetc(fp);
	c[12] = fgetc(fp);    c[19] = fgetc(fp);
	c[26] = fgetc(fp);    c[33] = fgetc(fp);
	c[40] = fgetc(fp);    c[48] = fgetc(fp);
	c[41] = fgetc(fp);    c[34] = fgetc(fp);
	c[27] = fgetc(fp);    c[20] = fgetc(fp);
	c[13] = fgetc(fp);    c[6]  = fgetc(fp);
	c[7]  = fgetc(fp);    c[14] = fgetc(fp);
	c[21] = fgetc(fp);    c[28] = fgetc(fp);
	c[35] = fgetc(fp);    c[42] = fgetc(fp);
	c[49] = fgetc(fp);    c[56] = fgetc(fp);
	c[57] = fgetc(fp);    c[50] = fgetc(fp);
	c[43] = fgetc(fp);    c[36] = fgetc(fp);
	c[29] = fgetc(fp);    c[22] = fgetc(fp);
	c[15] = fgetc(fp);    c[23] = fgetc(fp);
	c[30] = fgetc(fp);    c[37] = fgetc(fp);
	c[44] = fgetc(fp);    c[51] = fgetc(fp);
	c[58] = fgetc(fp);    c[59] = fgetc(fp);
	c[52] = fgetc(fp);    c[45] = fgetc(fp);
	c[38] = fgetc(fp);    c[31] = fgetc(fp);
	c[39] = fgetc(fp);    c[46] = fgetc(fp);
	c[53] = fgetc(fp);    c[60] = fgetc(fp);
	c[61] = fgetc(fp);    c[54] = fgetc(fp);
	c[47] = fgetc(fp);    c[55] = fgetc(fp);
	c[62] = fgetc(fp);    c[63] = fgetc(fp);

	for (i = 1; i <= 64; ++i) {      
	    //printf("%.2X   ", c);
		printf("%6d", c[i - 1]);
		if ((i % 8) == 0)
			putchar('\n');
	}
   // putchar('\n');
}

