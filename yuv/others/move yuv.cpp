#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
void main()
{
	FILE *fp1, *fp2, *fp3;
	int len = 0;
	int i;

	if((fp1 = fopen("[823x442]_Y.yuv","rb")) == NULL)
		printf("eror open1\n");
	if((fp2 = fopen("[820x442]_Y.yuv","wb")) == NULL)
		printf("eror open2\n");


	fseek(fp1, 0, SEEK_END);
	len = ftell(fp1);
	printf("len = %d\n", len);
	fseek(fp1, 0, SEEK_SET);

	for(i = 0; i < len; i++)
	{
		if(!(i%820) || !(i%821) || !(i%822))
		{
			printf("i = %d ", i);
			fgetc(fp1);
		}
		else
		{
			fputc(fgetc(fp1), fp2);
		}	
	}
	printf("\ni = %d\n", i);
	fclose(fp1);
	fclose(fp2);


	if((fp2 = fopen("[820x442]_Y.yuv","rb")) == NULL)
		printf("eror open2\n");
	fseek(fp2, 0, SEEK_END);
	len = ftell(fp2);
	printf("len = %d\n", len);
	fseek(fp1, 0, SEEK_SET);
	
	if((fp3 = fopen("[820x442]_Y_pad.yuv","wb")) == NULL)
		printf("eror open3\n");
	for(i = 0; i < len; i++)
	{
		fputc(fgetc(fp2), fp3);
	}
	for(i = len; i < (820 * 442); i++)
	{
		fputc(0, fp3);
	}		
	printf("\ni = %d\n", i);
	fclose(fp2);
	fclose(fp3);
}
*/

void main()
{
	FILE *fp1, *fp2;
	int len = 0;
	int surplus = 0;
	int i;

	if((fp1 = fopen("[720x480]_V.yuv","rb")) == NULL)
		printf("eror open1\n");
	if((fp2 = fopen("[716x480]_V.yuv","wb")) == NULL)
		printf("eror open2\n");


	fseek(fp1, 0, SEEK_END);
	len = ftell(fp1);
	printf("len = %d\n", len);
	fseek(fp1, 0, SEEK_SET);
	
	surplus = (len - (716 * 480 / 2)) / 2;
	printf("surplus = %d\n",  surplus);
	fseek(fp1, surplus, SEEK_SET);

	for(i = 0; i < (716 * 480 / 2); i++)
	{
		fputc(fgetc(fp1), fp2);
	}

	printf("\ni = %d\n", i);
	fclose(fp1);
	fclose(fp2);
}