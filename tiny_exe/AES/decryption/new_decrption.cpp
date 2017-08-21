#include <stdio.h>
#include <math.h>
#include <sys/timeb.h>
#include <time.h>
#include <sys/types.h> 
#include "s_box.h"
#define BIT1 0x01
int de_s_matrix[4][4],decrypt_matrix[4][4];
/*Inv_shiftrows change*/
void Inv_shiftrows(int first[4][4],int after_Inv_shiftrows[4][4])
{
	int i,j;
    for(i=0;i<4;i++)
		{
	    	for(j=0;j<4;j++)
			{
	    		switch(i)
				{
		    	case 0:
		    		after_Inv_shiftrows[i][j]=first[i][j];
			    	break;
		    	case 1:
			    	if(j>0)
				    	after_Inv_shiftrows[i][j]=first[i][j-1];
			    	else 
					    after_Inv_shiftrows[i][j]=first[i][j+3];
				    break;
		    	case 2:
			    	if(j>1)
			    		after_Inv_shiftrows[i][j]=first[i][j-2];
			    	else 
			    		after_Inv_shiftrows[i][j]=first[i][j+2];
			    	break;
		    	case 3:
			    	if(j>2)
				    	after_Inv_shiftrows[i][j]=first[i][j-3];
			    	else 
				    	after_Inv_shiftrows[i][j]=first[i][j+1];
			    	break;
			    default:
			    	break;
				}  		
			}
		}
}
int SubBytes(int s)//s_box_transform
{
	int x,y;
	y=s & 0x0f;
	x=(s>>4) & 0x0f;
	s=Sbox[x][y];
	return s;
}
int Inv_SubBytes(int s)//reverse_s_box_transform
{
	int x,y;
	y=s & 0x0f;
	x=(s>>4) & 0x0f;
	s=ver_Sbox[x][y];
	return s;
}
/*line multiply column,just save the low right bits*/
int multiply(int c,int a)
{
	int result=0,x=0,y=0;
	x=(a & 0x0f0)>>4;
	y=a & 0x0f;
	if(c==14)/*0x0e*/
		result=multiply_02[x][y]^multiply_04[x][y]^multiply_08[x][y];
	else if(c==11)/*0x0b*/
		result=a^multiply_02[x][y]^multiply_08[x][y];
	else if(c==13)/*0x0d*/
		result=a^multiply_04[x][y]^multiply_08[x][y];
	else if(c==9)/*0x09*/
		result=a^multiply_08[x][y];
	return result;
}
int inv_column_multiply(int c[],int a[])
{
    int temp=0,i;
	for(i=0;i<4;i++)
		temp=temp^multiply(c[i],a[i]);
	return temp;
}
void get_new_decrypt(int turn)
{
	int i,j,temp=0;
	int temp_row[4];/*the first row of new encrypt matrix*/
    /*obtain the new encrypt_matrix's last three columns*/
	for(i=0;i<4;i++)
	{
		for(j=3;j>=1;j--)
		{
			decrypt_matrix[i][j]=decrypt_matrix[i][j]^decrypt_matrix[i][j-1];
		}
	}   
	/*obtain the new encrypt_matrix's first columns*/
	/*obtain the third column for calculate the first column*/
	for(i=0;i<4;i++)
		temp_row[i]=decrypt_matrix[i][3];
	/*move to left one step*/
	temp=temp_row[0];
	for(i=0;i<3;i++)	
		temp_row[i]=temp_row[i+1];	
	temp_row[3]=temp;
	/*SubBytes change*/
	for(i=0;i<4;i++)
	{
		temp_row[i]=SubBytes(temp_row[i]);
	}
	/*get the turn constant value*/
	if(turn>=9)
		temp=0x1b*(turn-8);
	else
		temp=(int)pow(2,(int)((turn*4-4)/4));
	/*just xor*/
	temp_row[0]^=temp;
	for(i=0;i<4;i++)
		decrypt_matrix[i][0]^=temp_row[i];
}  
void decryption(int de_s[16],int last[16])
{
	int i,j,k,turn=10,first[4][4],inv_column_temp[4],first_temp[4];
	int after_Inv_column_multiply[4][4],after_Inv_shiftrows[4][4],result[4][4];
	int decrypt[]={0xd0,0x14,0xf9,0xa8,0xc9,0xee,0x25,0x89,0xe1,0x3f,0x0c,0xc8,0xb6,0x63,0x0c,0xa6};
/*	int decrypt[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	int decrypt[]={0x77,0x75,0x78,0x69,0x61,0x6e,0x64,0x63,0x61,0x69,0x6b,0x65,0x78,0x75,0x61,0x6e};*/
	int invmixcolumn[4][4]={{0x0e,0x0b,0x0d,0x09},{0x09,0x0e,0x0b,0x0d},{0x0d,0x09,0x0e,0x0b},{0x0b,0x0d,0x09,0x0e}};
	/*
    the start decryption which lose invmixcolumn change
    */
	/*get the de_s matrix*/
    for(i=0;i<16;i++)
		de_s_matrix[i%4][i/4]=de_s[i];
	/*get the encrypt matrix*/
    for(i=0;i<16;i++)
	    decrypt_matrix[i%4][i/4]=decrypt[i];
	/*just xor*/
	for(i=0;i<4;i++)
	   for(j=0;j<4;j++)
		   first[i][j]=de_s_matrix[i][j]^decrypt_matrix[i][j];
	/*Inv_shiftrows change*/
    Inv_shiftrows(first,after_Inv_shiftrows);
	/*Inv_SubBytes change*/
    for(i=0;i<4;i++)
	    for(j=0;j<4;j++)
		    de_s_matrix[i][j]=Inv_SubBytes(after_Inv_shiftrows[i][j]);
    /*
    for other decryption except first and last
    */     
    while(turn>1)
	{
		/*obtain new decrypt_matrix*/
	    get_new_decrypt(turn);
		/*just xor*/
	    for(i=0;i<4;i++)
	        for(j=0;j<4;j++)
	            first[i][j]=de_s_matrix[i][j]^decrypt_matrix[i][j];
        /*inv_column_multiply change*/
        for(i=0;i<4;i++)
		{	
	    	for(j=0;j<4;j++)
			{
	 	        for(k=0;k<4;k++)
				{
		            inv_column_temp[k]=invmixcolumn[i][k];
			        first_temp[k]=first[k][j];
				}
		        after_Inv_column_multiply[i][j]=inv_column_multiply(inv_column_temp,first_temp);
			}
		}
		/*Inv_shiftrows change*/
    	Inv_shiftrows(after_Inv_column_multiply,after_Inv_shiftrows);
		/*Inv_SubBytes change*/
    	for(i=0;i<4;i++)
	        for(j=0;j<4;j++)
		        de_s_matrix[i][j]=Inv_SubBytes(after_Inv_shiftrows[i][j]);
		    turn--;
	}
    /*
    the last decryption just need xor
    */
	/*obtain new decrypt_matrix*/
    get_new_decrypt(turn);
	/*obtain the result matrix*/
	for(i=0;i<4;i++)
	    for(j=0;j<4;j++)
		    result[i][j]=de_s_matrix[i][j]^decrypt_matrix[i][j];
    for(i=0;i<16;i++)/*change to one-D matrix*/
	    last[i]=result[i%4][i/4];
}
void main() 
{
	int i,num=0,time=0,temp=0;
	int de_s[16]={0},last[16]={0};
	FILE *fp1,*fp2;
	struct _timeb timebuffer,timebuffer1;
    char *timeline,*timeline1; 
	if((fp1=fopen("newTS.ts","rb"))==NULL)
	{
		printf("cannot open file1.\n");
	}
	if((fp2=fopen("figprtES_2.ts","wb"))==NULL) 
	{
		printf("cannot open file2.\n");
	}
	fseek(fp1,0,SEEK_END);
	num=ftell(fp1);  
	fseek(fp1,0,SEEK_SET);
/*	printf("num=%d num/16=%d (num%16)=%d\n",num,num/16,num%16); */
	if(num%16)
		time=num/16+1;
	else
		time=num/16;
	printf("time=%d\n",time); 

    _ftime( &timebuffer );
    timeline = ctime( & ( timebuffer.time ) );
    printf( "The time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );
/*
	printf("The cryptographic key is:\n");
	for(i=0;i<16;i++)
		printf("%02x",encrypt[i]);
	printf("\n");
    printf("The cipher text is:\n");
	while(num)
	{
		temp=fgetc(fp1);
		printf("%02x",temp);
		num--;
	}
	fseek(fp1,0,SEEK_SET);
*/
	while(time)
	{
	    for(i=0;i<16;i++)
		{
	    	if(fread(&de_s[i],sizeof(char),1,fp1)!=1)
			{
		        if(feof(fp1))
			    	fclose(fp1);
			    printf("file1 read error\n");
			}
		}

/*	    for(i=0;i<16;i++)
			de_s[i]=(int)fgetc(fp1);*/
		decryption(de_s,last);
		time--;
		for(i=0;i<16;i++)
			fputc((char)last[i],fp2);
	}
	printf("end decrypt.\n");

    _ftime( &timebuffer1 );
    timeline1 = ctime( & ( timebuffer1.time ) );
    printf( "The time is %.19s.%hu %s", timeline1, timebuffer1.millitm, &timeline1[20] );

	fclose(fp1);
	fclose(fp2);
/*	if((fp2=fopen("figprtES_2.ts","rb"))==NULL)
	{
		printf("cannot open file2.\n");
	}
	temp=fgetc(fp2);
	printf("\nThe plain text is:\n");
	while(temp!=EOF)
	{
		printf("%02x",temp);
		temp=fgetc(fp1);
	}
    printf("\n");
	fclose(fp2);*/
}
