#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH (0x6300)//(0x85b)//(98485681)//(188000000 * 2)
#define START (0)//(0x143)//
void main()
{
	FILE *fp1, *fp2;
	int i;

	if((fp1 = fopen("suzie_qcif.yuv","rb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("2.yuv","wb")) == NULL)
		printf("eror open2\n");
	if(START)
	{
	    for(i = 0; i < START; i++)
		{
			fgetc(fp1);
		}		
		fgetc(fp1);
	}
	for(i = 0; i < LENGTH; i++)
	{
		fputc(fgetc(fp1), fp2);
	}

	printf("\n==================cut stream %d====================\n",i);
	fclose(fp1);
	fclose(fp2);
}