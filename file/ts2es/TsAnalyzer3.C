/*!
*************************************************************************************************
*************************************************************************************************
  TS流视频参数提取软件：

  Input : TS文件名、PID(十进制)、TS包长(188 or 204);
         TS文件名和PID通过问答的方式输入；
  
*************************************************************************************************
*************************************************************************************************
*/
#include "TsAnalyzerHead.h"

char                     tsfile[30];    //!!!!!!!!!!!!   name of TS file   !!!!!!!!!!!
unsigned  int            PID;           //!!!!!!!!!!!!          PID        !!!!!!!!!!!
#define   TSpacketSize   188            //!!!!!!!!!!!!      188 or 204     !!!!!!!!!!!

int main(void)
{  //common variables 
   FILE           *fptr_read,  *fptr_write;
   unsigned char  head_c1, head_c2, head_c3, head_c4;
   unsigned char  c1, c2, c3, c4;
   unsigned char  c;
   int            position_a, position_b;       //notice !!!unsigned int or int
   int            i;
   //variables of section A
   int            is_rightPID;
   int            all_tspacket_cnt = 0, empty_tspacket_cnt= 0, PID_tspacket_cnt = 0;
   //variables of section B
   unsigned char  payload_unit_start_indicator = 0;
   unsigned char  adaptation_field_control = 0;
   unsigned int   temp = 0;    
   //variables of section C
   unsigned char  startcode;
   unsigned int   c1c2c3c4 = 0;
   //variables of section D
   int            cnt = 0;
   unsigned char  picture_coding_type;

/*!    input: TS文件名和PID
*************************************************************************************
*/
   printf("Please input a name of ts file: ");
   scanf("%s", tsfile);
   printf("Please input a PID(hex): ");
   scanf("%x", &PID);
   
   putchar('\n');
   printf("%s Report: \n", tsfile);
   printf("PID: %d(hex)   %d(decimal)", PID, PID);
   
/*!
**********************************************************************************************
**********************************************************************************************
Section A:
    1、pick-up all the TS packets with the specified PID;
	2、count all_tspacket_cnt、empty_tspacket_cnt、PID_tspacket_cnt
**********************************************************************************************
**********************************************************************************************
*/  
   if ((fptr_read = fopen(tsfile, "rb")) == NULL) {
      printf("Can not open the TS file --bye!\n\n");
	  exit(1);
   }
   fptr_write = fopen("TSonePID.ts", "wb");   //???"wb" or "ab"
         
          ///////////////////////////////////////////////    
          //check  the sync_byte: binary,   0100 0111  //
          //                      hexx,     oX47       //
          //                      decimal,  71         //
          /////////////////////////////////////////////// 
  
   while ((head_c1 = fgetc(fptr_read)) == 71) {      
      ++all_tspacket_cnt;
	  is_rightPID = 0;
      head_c2 = fgetc(fptr_read);
      head_c3 = fgetc(fptr_read);
      head_c4 = fgetc(fptr_read);

	  is_rightPID = check_PID(head_c2, head_c3, &PID_tspacket_cnt, &empty_tspacket_cnt);

      fseek(fptr_read, -4, SEEK_CUR);              
                                 //如果指定的视频包，则复制出来
	  if (is_rightPID) {
		 for (i = 0; i < TSpacketSize; ++i) {
	        c= fgetc(fptr_read);
		    fputc(c, fptr_write);
		 }
	  }
      else 
         fseek(fptr_read, TSpacketSize, SEEK_CUR);
   } //??end of while

   printf("\n*******************************   section A   ********************************\n");
   printf("all_tspacket_cnt   :  %d\n", all_tspacket_cnt);
   printf("PID_tspacket_cnt   :  %d\n", PID_tspacket_cnt);
   printf("empty_tspacket_cnt :  %d\n", empty_tspacket_cnt);
   printf("\nSection A is ok.");
   fclose(fptr_read);
   fclose(fptr_write);
/*************************          end of section A            *****************************/
 


/*!
**********************************************************************************************
**********************************************************************************************
Section B:
     1、remove the heads of TS and PES.
	 2、假设前提：00 00 01 E0 00 00 ... 00 00 01 (B3 or 00)
**********************************************************************************************
**********************************************************************************************
*/
   if ((fptr_read = fopen("TSonePID.ts", "rb")) == NULL) {
       printf("Can not open TSonePID.ts --bye!\n\n");
	   exit(1);		   
   }
   fptr_write = fopen("ESvideoData.ts", "wb");

   while ((head_c1 = fgetc(fptr_read)) == 71) {     
       position_a = ftell(fptr_read) - 1;
	   head_c2 = fgetc(fptr_read);
       head_c3 = fgetc(fptr_read);
       head_c4 = fgetc(fptr_read);
     
	   analyze_TS_head(head_c2, head_c4, &payload_unit_start_indicator, &adaptation_field_control);
	   
	   switch (adaptation_field_control) {
	   
	   /*********************        保留         ************************/
	   case 0:   
		   printf("adaptation_field_control = 0, ISO/IEC reserved!\n");
		   fseek(fptr_read, TSpacketSize - 4, SEEK_CUR);     //skip this TS packet
		   break;
	   
	   /*********************     only 调整字段   ************************/
	   case 2:   
		   c = fgetc(fptr_read);                             //mode1
		   if (c != 183)
			   printf("Contradiction, It is not 183!\n");
		   fseek(fptr_read, TSpacketSize - 5, SEEK_CUR);
           
		   //fseek(fptr_read, TSpacketSize - 4, SEEK_CUR);   //mode2:当不读数和判断时
		   break;
	   
	   /*********************     only 有效负载   ************************/
	   case 1:  //!!! 
		   if (!payload_unit_start_indicator) {              //*** 无调整，无PES ***
			   for (i = 0; i < (TSpacketSize - 4); ++i) {    //47 xx xx xx ...ES data
			       c = fgetc(fptr_read);
				   fputc(c, fptr_write);
			   }
		   }
		   else {                                            //*** 无调整，有PES ***
		       //find_out_000001(fptr_read);                 //47 XX XX XX 00 00 01 XX
			   if (!next_is_000001(fptr_read)) {
				     printf("When no adapt and have PES, after 4TSheader it should be 000001!\n");
		             exit(1);
			   }
			   else
					 c = fgetc(fptr_read);
			   
			   if ((c & 240) != 224) {                    //no EX; no video
				   printf("There is one TS packet, not video!\n");
				   fseek(fptr_read, TSpacketSize - 8, SEEK_CUR);
			   }
			   else {                                        //video
                   c1 = fgetc(fptr_read);
			       c2 = fgetc(fptr_read);
				   if ((c1 == 0) && (c2 == 0)) {             
				       find_out_000001(fptr_read);   //假设前提：00 00 01 E0 00 00 ... 00 00 01
					   fseek(fptr_read, -3, SEEK_CUR);       //此处000001属于ES，所以回调3个位置
					   position_b = ftell(fptr_read);
					   for (i = 0; i < TSpacketSize - (position_b - position_a); ++i) {
					       c = fgetc(fptr_read);
				           fputc(c, fptr_write);
					   }
				   }
				   else {
				       temp = c1;
					   temp = temp << 8;
					   temp = temp | c2;
					   fseek(fptr_read, temp, SEEK_CUR);
					   position_b = ftell(fptr_read);
				       for (i = 0; i < TSpacketSize - (position_b - position_a); ++i) {
					       c = fgetc(fptr_read);
				           fputc(c, fptr_write);
					   }
				   }
			   }
		   }
		   break;
  
	   /*********************   调整字段 + 有效负载   ************************/
       case 3:  //!!!
           if (!payload_unit_start_indicator) {              //*** 调整 + ES ***
		       c = fgetc(fptr_read);
		       fseek(fptr_read, c, SEEK_CUR);
			   position_b = ftell(fptr_read);
			   for (i = 0; i < TSpacketSize - (position_b - position_a); ++i) {
			       c = fgetc(fptr_read);
				   fputc(c, fptr_write);
			   }
		   }
		   else {                                            //*** 调整 + PES +ES ***
			   find_out_000001(fptr_read);                   //TS -> PES：直接查找00 00 01
			   c = fgetc(fptr_read);
			   position_b = ftell(fptr_read);
               if ((c & 240) != 224) {                      //no EX; no video
				   printf("There is one TS packet, not video!\n");
				   fseek(fptr_read, TSpacketSize - (position_b - position_a), SEEK_CUR);
			   }
			   else {                                        //video
			       c1 = fgetc(fptr_read);
			       c2 = fgetc(fptr_read);
				   if ((c1 == 0) && (c2 == 0)) {             
				       find_out_000001(fptr_read);           //???00 00 01 E0 00 00 ... 00 00 01
                       fseek(fptr_read, -3, SEEK_CUR);       //此处000001属于ES，所以回调3个位置
					   position_b = ftell(fptr_read);
					   for (i = 0; i < TSpacketSize - (position_b - position_a); ++i) {
					       c = fgetc(fptr_read);
				           fputc(c, fptr_write);
					   }
				   }
				   else {
					   temp = c1;
					   temp = temp << 8;
					   temp = temp | c2;
                       fseek(fptr_read, temp, SEEK_CUR);
                       position_b = ftell(fptr_read);
				       for (i = 0; i < TSpacketSize - (position_b - position_a); ++i) {
					       c = fgetc(fptr_read);
				           fputc(c, fptr_write);
					   }
				   }
			   }
		   }
		   break;

	   /*********************     impossible     ********************/
	   default :
		   printf("adaptation_field_control can not beyond b'11!\n");
		   exit(1);

	   } //??end of switch

   } //??end of while; end of loop
   printf("\n*******************************   section B   ********************************\n");
   printf("\nSection B is ok.\n");
   fclose(fptr_read);
   fclose(fptr_write);
/*************************          end of section B            *****************************/



/*!
**********************************************************************************************
**********************************************************************************************
Section C:
     1、input a ES stream file ---- which has no TS head and PES head;
     2、find out the startcodes, then transfer file pointer, call sub-function to read raw-bytes
        and analyze parameters;
	 3、
**********************************************************************************************
**********************************************************************************************
*/
   if ((fptr_read = fopen("ESvideoData.ts", "rb")) == NULL) {
      printf("Can not open ESvideoData.ts --bye!\n\n");
	  exit(1);
   }
      
   printf("*******************************   section C   *********************************\n");
       //1、sequence_header():
   startcode = 179;                                          //B3 = b'1011_0011 = 179 
   find_out_startcode(fptr_read, startcode);                 //直到找到型
   analyze_para_of_sequence_header(fptr_read);  //?????函数编写
      
       //2、sequence_extension():
   startcode = 181;                                          //B5 = b'1011_0101 = 181
   if (next_is_startcode(fptr_read, startcode))              //下一个就是型
	   analyze_para_of_sequence_extension(fptr_read); //????函数编写 //B5之后4-bits放入函数中解析，
   else {                                                            //或者，根据位置来判断调用哪个
	   printf("ISO/IEC 1172-2!\n");
	   exit(1);
   }

       //extension_and_user_data(0):
   c1 = fgetc(fptr_read);
   c2 = fgetc(fptr_read);
   c3 = fgetc(fptr_read);
   c4 = fgetc(fptr_read);
   c1c2c3c4 = c1;
   c1c2c3c4 = (c1c2c3c4 << 8) | c2;
   c1c2c3c4 = (c1c2c3c4 << 8) | c3;
   c1c2c3c4 = (c1c2c3c4 << 8) | c4;
        //add by ckx --- 070120
   while ((c1c2c3c4 == 437) || (c1c2c3c4 == 434)) {
      if (c1c2c3c4 == 437)
		 extension_data0(fptr_read);
	 
	  if (c1c2c3c4 == 434)
         user_data(fptr_read);

	   c1 = fgetc(fptr_read);
       c2 = fgetc(fptr_read);
       c3 = fgetc(fptr_read);
       c4 = fgetc(fptr_read);
       c1c2c3c4 = c1;
       c1c2c3c4 = (c1c2c3c4 << 8) | c2;
       c1c2c3c4 = (c1c2c3c4 << 8) | c3;
       c1c2c3c4 = (c1c2c3c4 << 8) | c4;
   }
   fseek(fptr_read, -4, SEEK_CUR);
   /*
   if ((c1c2c3c4 == 437) || (c1c2c3c4 == 434)) {
       if (c1c2c3c4 == 437)
		   extension_data0(fptr_read);   
	   //else   //edit by ckx --- 070120
	   if (c1c2c3c4 == 434)
		   user_data(fptr_read); //???函数编写
   
   }
   else
	   fseek(fptr_read, -4, SEEK_CUR);                       //go back 4-bytes
   */
       
      //3、group_of_picture_header(); extension_and_user_data(1):
   startcode = 184;
   if (next_is_startcode(fptr_read, 184))  {                 //B8 = b'1011_1000 = 184
       //analyze_para_of_group_of_picture_header(fptr_read);
	   analyze_para_of_group_of_pictures_header(fptr_read);
	   startcode = 178;                                      //B2 = b'1011_0010 = 178
	   if (next_is_startcode(fptr_read, startcode))
		   user_data(fptr_read);//???函数编写
	   else
		   fseek(fptr_read, -4, SEEK_CUR);
   }
   else
	   fseek(fptr_read, -4, SEEK_CUR);                       //go back 4-bytes

       //4、picture_header():
   if (next_is_startcode(fptr_read, 0))
	   //analyze_para_of_picture_header(fptr_read);
	   analyze_para_of_pictures_header(fptr_read);
   else {
	   printf("no picture_header()!\n");
	   exit(1);
   }

       //5、picture_coding_extension():
   startcode = 181;                                          //B5 = b'1011_0101 = 181
   if (next_is_startcode(fptr_read, startcode))
	   analyze_para_of_picture_coding_extension(fptr_read);
   else {
       printf("no picture_coding_extension()!\n");
	   exit(1);
   } 
   printf("\nSection D is ok.\n");


/*!
**********************************************************************************************
**********************************************************************************************
Section D:
       1、统计picture中多少slices; 
       2、显示序列中picture_coding_type;
**********************************************************************************************
**********************************************************************************************
*/
   if ((fptr_read = fopen("TSonePID.ts", "rb")) == NULL) {
 //if ((fptr_read = fopen("ESvideoData.ts", "rb")) == NULL) {  
        printf("Can not the file --bye!\n\n");
	    exit(1);
   }
   printf("*******************************   Section D   ********************************\n");

           /***************   序列中picture的类型   *****************/
   find_out_startcode(fptr_read, 179);                   // B3 = 179
   position_a = ftell(fptr_read);                        //position_a 在第一个000001B3之后
	 
   find_out_startcode(fptr_read, 179);
   fseek(fptr_read, -4, SEEK_CUR);
   position_b = ftell(fptr_read);                        //position_b 在第二个000001B3之前

   fseek(fptr_read, position_a - position_b, SEEK_CUR);  //文件指针调整到position_a
   while (ftell(fptr_read) < position_b) {
	   find_out_startcode(fptr_read, 0);
	   if (ftell(fptr_read) < position_b) {
		  ++cnt;
	      c = fgetc(fptr_read);  //printf("%.2X  ", c);  //DEBUG!!!
		  c = fgetc(fptr_read);  //printf("%.2X  ", c);
          picture_coding_type = (c & 56) >> 3;
		 
		  printf("The %d picture :  ", cnt);
		  switch (picture_coding_type) {
		  case 0: 
		      printf("prohibit!\n");  break;
		  case 1:
			  printf("I\n");   break;
		  case 2:
              printf("P\n");   break;
		  case 3:
			  printf("B\n");   break;
		  case 4:
			  printf("不应使用\n");  break;
		  case 5:
		  case 6:
		  case 7:
			  printf("reserved!\n"); break;
		  default:
			  printf("picture_coding_type beyond the range!\n");
			  exit(1);
		  }
	   }
   }
   putchar('\n');
   printf("A sequence has %d pictures\n", cnt);
   
            /***************   统计picture中多少slices   *****************/
   cnt = 0;
   rewind(fptr_read);
   find_out_startcode(fptr_read, 179);          //查找第一个B3
	 
   find_out_startcode(fptr_read, 0);            //第一个B3之后的第一个00
   position_a = ftell(fptr_read);               //position_a 在第一个00000100之后

   find_out_startcode(fptr_read, 0);
   fseek(fptr_read, -4, SEEK_CUR);
   position_b = ftell(fptr_read);               //position_b 在第二个00000100之前

   fseek(fptr_read, position_a - position_b, SEEK_CUR);
   while (ftell(fptr_read) < position_b) {
	   find_out_000001(fptr_read);
	   if (ftell(fptr_read) < position_b) {
		  c = fgetc(fptr_read);
		  if ((c >=1) && (c <= 175))         //slice_start_code: 01-AF
			 ++cnt;
	   }
   }
   printf("A picture  has %d slices\n", cnt);
   
   fclose(fptr_read);
   printf("Section D is ok.\n");
   printf("*****************************   end of repoter   ******************************\n\n\n");
/*************************          end of section D            *****************************/
} //end of main??




/*!
***********************************************************************************************
辅助函数：
***********************************************************************************************
*/

/*!    check PID 
***********************************************************************************************
*/
int  check_PID(unsigned char head2, unsigned char head3, int *pid_cnt, int *empty_cnt)
{
   unsigned char  mask_PID1 = 31;         //b'0001_1111 ;   or (char) 31
   unsigned int   PIDinfile = 0;          // int or unsigned int ???
   unsigned int   PIDempty  = 8191;       //b'1_1111_1111_1111 = 8191 decimal

   PIDinfile = (head2 & mask_PID1) << 8;
   PIDinfile = PIDinfile | head3;
   
   if (PIDinfile == PID) {
       ++*pid_cnt;
       return 1;
   }
   else if (PIDinfile == PIDempty) { 
	   ++*empty_cnt;
	   return 0;
   }
   else
	   return 0;
}


/*!    analyze parameters: payload_unit_start_indicator、adaptation_field_control
***********************************************************************************************
*/
void  analyze_TS_head(unsigned char c2, unsigned char c4, 
					  unsigned char *isPES, unsigned char *isAdapt)
{
	*isPES = (c2 & 64) ? 1 : 0;                            //mask 64 = b'0100_0000
	*isAdapt = (c4 & 48) >> 4;                             //mask 48 = b'0011_0000
}


/*!    find out ox000001: 直到找到型
***********************************************************************************************
*/
void  find_out_000001(FILE *fp)   //quicker
{
    unsigned char  c1, c2, c3;

    c1 = fgetc(fp);
	c2 = fgetc(fp);
	c3 = fgetc(fp);
	
	while (!((c1 == 0) && (c2 == 0) && (c3 == 1))) {
		if ((c1 == 0) || (c2 == 0) || (c3 == 0)) {
		   fseek(fp, -2, SEEK_CUR);
           c1 = fgetc(fp);
	       c2 = fgetc(fp);
	       c3 = fgetc(fp);
		}
		else {
		   c1 = fgetc(fp);
	       c2 = fgetc(fp);
	       c3 = fgetc(fp);
		}
	}
}


/*!    next is ox000001: 下一个就是型
*************************************************************************************************
*/
int next_is_000001(FILE *fp)
{
    unsigned char  c;

	if ((c = fgetc(fp)) == 0) {
		if ((c = fgetc(fp)) == 0) {
			if ((c = fgetc(fp)) == 1) 
			    return 1;
	        else
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 0;
}


/*!       find out ox000001XX: 直到找到型
**********************************************************************************************
*/
void  find_out_startcode(FILE *fp, unsigned char startcode)   //quicker
{
    unsigned char  c1, c2, c3, c4;

	c1 = fgetc(fp);
	c2 = fgetc(fp);
	c3 = fgetc(fp);
	c4 = fgetc(fp);
	
	while (!((c1 == 0) && (c2 == 0) && (c3 == 1) && (c4 == startcode))) {
		if ((c1 == 0) || (c2 == 0) || (c3 == 0) || (c4 == 0)) {
		   fseek(fp, -3, SEEK_CUR);
           c1 = fgetc(fp);
	       c2 = fgetc(fp);
	       c3 = fgetc(fp);
	       c4 = fgetc(fp);
		}
		else {
		   c1 = fgetc(fp);
	       c2 = fgetc(fp);
	       c3 = fgetc(fp);
	       c4 = fgetc(fp);
		}
	}
}


/*!    next is ox000001XX: 下一个就是型
*************************************************************************************************
*/
int  next_is_startcode(FILE *fp, unsigned char startcode)
{
    unsigned char  c;

	if ((c = fgetc(fp)) == 0) {
		if ((c = fgetc(fp)) == 0) {
			if ((c = fgetc(fp)) == 1) {
				if ((c = fgetc(fp)) == startcode)
			        return 1;
				else
					return 0;
			}
			else
		        return 0;
		}
		else 
			return 0;
	}
	else
		return 0;
}


/*!   assistant-1、bits-print:
***********************************************************************************************
*/ 
void  arbitrary_bit_print(unsigned int a, int n)    
{
    int  i;
	int  mask = 1 << (n - 1);

	for (i = 1; i <= n; ++i) {
		putchar((a & mask) ? '1' : '0');
	    a = a << 1;
		if (i % CHAR_BIT == 0 && i < n)
			putchar(' ');
	}
}


/*!   assistant-2、char-print:
***********************************************************************************************
*/
void  char_print(unsigned char a)
{
   int  i;
   int  n = sizeof(char) * CHAR_BIT;
   int  mask = 1 << (n - 1);

   for (i = 1; i <= n; ++i) {
	   putchar(((a & mask) == 0) ? '0' : '1');
	   a = a << 1;
	   if (i % 4 == 0 && i < n)
		   putchar('_');
   }
   putchar(' ');
}