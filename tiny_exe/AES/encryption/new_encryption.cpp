#include <time.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <sys/timeb.h>
#include <sys/types.h> 
#include "s_box.h"
#define BIT1 0x01
/*proclaim's state and encrypt matrix in ervery turn*/
int first_s[4][4],encrypt_matrix[4][4];
/*s_box_transform*/
int SubBytes(int s)
{
	int x,y;
	y=s & 0x0f;
	x=(s>>4) & 0x0f;
	s=Sbox[x][y];
	return s;
}
/*reverse_s_box_transform*/
int rver_SubBytes(int s)
{
	int x,y;
	y=s & 0x0f;
	x=(s>>4) & 0x0f;
	s=ver_Sbox[x][y];
	return s;
}
/*shiftrows change*/
void shiftrows(int after_sbox[4][4],int after_rotary_row[4][4])
{
	int i,j;
            for(i=0;i<4;i++)
			{
	        	for(j=0;j<4;j++)
				{
	    	    	switch(i)
					{
		        	case 0:
		    	    	after_rotary_row[i][j]=after_sbox[i][j];
			        	break;
		     	    case 1:
			    	    if(j<3)
				    	    after_rotary_row[i][j]=after_sbox[i][j+1];
			    	    else 
					        after_rotary_row[i][j]=after_sbox[i][j-3];
				        break;
		    	    case 2:
			    	    if(j<2)
			    	    	after_rotary_row[i][j]=after_sbox[i][j+2];
			    	    else 
			    	    	after_rotary_row[i][j]=after_sbox[i][j-2];
			        	break;
		    	    case 3:
			    	    if(j<1)
				    	    after_rotary_row[i][j]=after_sbox[i][j+3];
			        	else 
				    	    after_rotary_row[i][j]=after_sbox[i][j-1];
			    	    break;
			        default:
			        	break;
					}  		
				}
			}
}
/*transfer to new encrypt matrix*/
void get_new_encrypt(int turn)
{
	int i,j,temp=0;
	int temp_row3[4];/*the third row of the old encrypt matrix*/
	for(i=0;i<4;i++)
		temp_row3[i]=encrypt_matrix[i][3];
	temp=temp_row3[0];
	for(i=0;i<4;i++)
	{	
		if(i<3)
			temp_row3[i]=temp_row3[i+1];	
		else
			temp_row3[i]=temp;
	}
	for(i=0;i<4;i++)
	{
		temp_row3[i]=SubBytes(temp_row3[i]);
	}
	if(turn>=9)
		temp=0x1b*(turn-8);
	else
		temp=(int)pow(2,(int)((turn*4-4)/4));
	temp_row3[0]^=temp;
	/*obtain the new encrypt_matrix*/
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
	    	if(!(j%4))
			{
			   	encrypt_matrix[i][j]=encrypt_matrix[i][j]^temp_row3[i];
			}
		}
	}   
	for(i=0;i<4;i++)
	{
		for(j=1;j<4;j++)
		{
			encrypt_matrix[i][j]^=encrypt_matrix[i][j-1];
		}
	}  
}
/*column multiply change*/
int multiply(int c,int a)
{
	int result=0,x,y;
	x=(a & 0x0f0)>>4;
	y=a & 0x0f;
	if(c==1)
		result=a;
	else if(c==2)
		result=multiply_02[x][y];
	else if(c==3)
		result=multiply_03[x][y];
	return result;
}
int column_multiply(int c[],int a[])
{
    int temp=0,i;
	for(i=0;i<4;i++)
		temp=temp^multiply(c[i],a[i]);
	return temp;
}
void encryption(int s[16],int result[16])
{
	int i,j,k,s_matrix[4][4],after_sbox[4][4],after_rotary_row[4][4],*p,turn=1;
	int encrypt[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
/*	int encrypt[]={0x77,0x75,0x78,0x69,0x61,0x6e,0x64,0x63,0x61,0x69,0x6b,0x65,0x78,0x75,0x61,0x6e};*/
	int mixcolumn[4][4]={{0x02,0x03,0x01,0x01},{0x01,0x02,0x03,0x01},{0x01,0x01,0x02,0x03},{0x03,0x01,0x01,0x02}};
	int after_mixcolumn[4][4]={0},column_temp[4],a_temp[4];
	/*get the s matrix*/
    for(i=0;i<16;i++)
	    s_matrix[i%4][i/4]=s[i];

	/*get the encrypt matrix*/
    p=encrypt;
    for(i=0;i<16;i++)
	    encrypt_matrix[i%4][i/4]=*(p+i);

	/*just xor*/
    for(i=0;i<4;i++)
	    for(j=0;j<4;j++)
	        first_s[i][j]=s_matrix[i][j]^encrypt_matrix[i][j];

    while(turn<10)
	{
		/*SubBytes*/
	    for(i=0;i<4;i++)
	        for(j=0;j<4;j++)
		        after_sbox[i][j]=SubBytes(first_s[i][j]);
		/*shiftrows*/
        shiftrows(after_sbox,after_rotary_row);
		/*mixcolumn*/
        for(i=0;i<4;i++)
		{	
	        for(j=0;j<4;j++)
			{
		        for(k=0;k<4;k++)
				{
		            column_temp[k]=mixcolumn[i][k];
			    	a_temp[k]=after_rotary_row[k][j];
				}
			    after_mixcolumn[i][j]=column_multiply(column_temp,a_temp);
			}
		}
		/*obtain new encrypt matrix
        get_new_encrypt(turn);*/
		turn++;
		/*get the encrypt matrix
        p=encrypt;
        for(i=0;i<16;i++)
			encrypt_matrix[i%4][i/4]=*(p+i);*/
		/*just xor*/
        for(i=0;i<4;i++)
	       for(j=0;j<4;j++)
	          first_s[i][j]=after_mixcolumn[i][j]^encrypt_matrix[i][j];
	}
	/*the last turn*/
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		    after_sbox[i][j]=SubBytes(first_s[i][j]);
    shiftrows(after_sbox,after_rotary_row);
    get_new_encrypt(turn);
    for(i=0;i<4;i++)
	    for(j=0;j<4;j++)
	        first_s[i][j]=after_rotary_row[i][j]^encrypt_matrix[i][j];
	/*change to one-D matrix*/
    for(i=0;i<16;i++)
	   result[i]=first_s[i%4][i/4];
}
/*need input encrypter*/
int main(void)
{
	int i,num=0,turn=0,temp=0;
	int s[16]={0},result[16]={0};
	FILE *fp1,*fp2;
    struct _timeb timebuffer,timebuffer1;
    char *timeline,*timeline1;
    int encrypt[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

	if((fp1=fopen("figprtES_1.ts","rb"))==NULL)
	{
		printf("cannot open file1.\n");
	}
	if((fp2=fopen("newTS.ts","wb"))==NULL)
	{
		printf("cannot open file2.\n");
	}
	/*find out the change's times*/
	fseek(fp1,0,SEEK_END);
	num=ftell(fp1);
	fseek(fp1,0,SEEK_SET);
/*	printf("num=%d num/16=%d (num%16)=%d\n",num,num/16,num%16);*/
	if(num%16)
		turn=num/16+1;
	else
		turn=num/16;
/*	printf("turn=%d\n",turn);*/


/*	printf("The cryptographic is:\n");
	for(i=0;i<16;i++)
		printf("%02x",encrypt[i]);
	printf("\n");
    printf("The plain text is:\n");
	while(num)
	{
		temp=fgetc(fp1);
		printf("%02x",temp);
		num--;
	}
	fseek(fp1,0,SEEK_SET);*/
    printf("\n");
    _ftime( &timebuffer );
    timeline = ctime( & ( timebuffer.time ) );
    printf( "The time is %.19s.%hu %s\n", timeline, timebuffer.millitm, &timeline[20] );

	while(turn)
	{
		/*for the last state,if less than 16,add 0*/
		if((turn==1) && (num%16))
		{
			for(i=0;i<(num%16);i++)
	    	    s[i]=(int)fgetc(fp1);
			for(i=(num%16);i<16;i++)
				s[i]=0;
		}
		else
		{
			for(i=0;i<16;i++)
				s[i]=(int)fgetc(fp1);
		}
		encryption(s,result);
		turn--;
        for(i=0;i<16;i++)
			fputc((char)result[i],fp2);
	}
	printf("end encrypt.\n");

    _ftime( &timebuffer1 );
    timeline1 = ctime( & ( timebuffer1.time ) );
    printf( "The time is %.19s.%hu %s\n", timeline1, timebuffer1.millitm, &timeline1[20] );

	fclose(fp1);
	fclose(fp2);
/*	if((fp2=fopen("newTS.ts","rb"))==NULL)
	{
		printf("cannot open file2.\n");
	}
	temp=fgetc(fp2);
	printf("\nThe cipher text is:\n");
	while(temp!=EOF)
	{
		printf("%02x",temp);
		temp=fgetc(fp1);
	}
    printf("\n");
	fclose(fp2);*/
	return 0; 
}

