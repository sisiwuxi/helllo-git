#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define BL4BIT ((~(~0))|0x0f)
#define BH4BIT ((~(~0))|0xf0)
#define MAX_FILE_LENGTH 100

char InputPath[MAX_FILE_LENGTH]={0};
char OutputPath[MAX_FILE_LENGTH]={0};

void set_input_path(char* input_path)
{
    strcpy(InputPath, (const char *)input_path);
printf("\n[%s~%s]",InputPath, input_path);
}

void set_output_path(char* output_path)
{
    strcpy(OutputPath, (const char *)output_path);
printf("\n[%s~%s]",OutputPath, output_path);
}

int check_magic_word(FILE *fp1, FILE *fp2)
{
	int temp1, temp2, temp3, temp4, temp5;

	if(0==ftell(fp1))
	{
	temp1 = fgetc(fp1) & B8BIT;
	temp2 = fgetc(fp1) & B8BIT;
	temp3 = fgetc(fp1) & B8BIT;
	temp4 = fgetc(fp1) & B8BIT;
	temp5 = fgetc(fp1) & B8BIT;
	if((0x65==temp1)&&(0x64==temp2)&&(0x69==temp3)&&(0x64==temp4)&&(0x31==temp5))
	{
		return 1;
	}
	else
	{
printf("\n[%d,%s](%d,%d,%d,%d,%d)",__LINE__,__FUNCTION__, temp1, temp2, temp3, temp4, temp5);
		return 0;	
	}
	}
	if(0x297==ftell(fp1))
	{
	temp1 = fgetc(fp1) & B8BIT;
	temp2 = fgetc(fp1) & B8BIT;
	temp3 = fgetc(fp1) & B8BIT;
	temp4 = fgetc(fp1) & B8BIT;
	temp5 = fgetc(fp1) & B8BIT;
	if((0x65==temp1)&&(0x64==temp2)&&(0x69==temp3)&&(0x64==temp4)&&(0x32==temp5))
	{
		fputc(0x0D, fp2);
		fputc(0x0A, fp2);
		return 1;	
	}
	else
	{
printf("\n[%d,%s](%d,%d,%d,%d,%d)",__LINE__,__FUNCTION__, temp1, temp2, temp3, temp4, temp5);
		return 0;	
	}	
	}
	return 1;
}	

void convert()
{
printf("\n[%d,%s]====open======",__LINE__,__FUNCTION__);

	FILE *fp1=NULL;
	FILE *fp2=NULL;
	int i = 0;
	int k = 1;
	int temp1;
	int temp2;

	if(strlen((const char *)InputPath) > 0)
	{
		if((fp1 = fopen(InputPath,"rb")) == NULL)
		{
			printf("input file open error\n");
			goto error_exit;
		}
	}
	else
	{
		goto error_exit;
	}
	if(strlen((const char *)OutputPath) > 0)
	{	
		if((fp2 = fopen(OutputPath,"wb")) == NULL)
		{
			printf("output file open error\n");
			goto error_exit;
		}
	}
	else
	{
		goto error_exit;
	}
printf("\n[%d,%s]====start======",__LINE__,__FUNCTION__);
	while(!feof(fp1))
	{
		if(!check_magic_word(fp1, fp2))
		{
			printf("\n[%d,%s]format is incorrect ftell=%ld\n",__LINE__,__FUNCTION__, ftell(fp1));
			goto error_exit;
		}
		temp1 = fgetc(fp1) & B8BIT;
		temp2 = fgetc(fp1) & B8BIT;
		if((0x0D==temp1) && (0x0A==temp2))
		{
		}
		else if((0x30==temp1) && (0x78==temp2))
		{
			fputc((fgetc(fp1) & B8BIT), fp2);
			fputc((fgetc(fp1) & B8BIT), fp2);
			temp1 = fgetc(fp1) & B8BIT;//0x20
		}		
	}
printf("\n[%d,%s]====stop======",__LINE__,__FUNCTION__);
	fclose(fp1);
	fclose(fp2);
	fp1=NULL;
	fp2=NULL;
	error_exit:
	{
printf("\n[%d,%s]====error_exit======",__LINE__,__FUNCTION__);
		if(fp1!=NULL)
		{
			fclose(fp1);
			fp1=NULL;
		}
		if(fp2!=NULL)
		{
			fclose(fp2);
			fp2=NULL;
		}
	}
}
