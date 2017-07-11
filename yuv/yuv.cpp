#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	FILE *fp1, *fp2;
	int len = 0;
	int i;
	int tempu = 0;
	int tempy = 0;
	int tempv = 0;
	if((fp1 = fopen("719x479.yuv","rb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("720x480.yuv","wb")) == NULL)
		printf("eror open2\n");

	fseek(fp1, 0, SEEK_END);
	len = ftell(fp1);
	printf("len = %d\n", len);
	fseek(fp1, 0, SEEK_SET);

	for(i = 0; i < 716 * 480 * 2; i++)
	{
		if(!(i % (716 * 2)) && (i > 0))
		{
			fputc(fgetc(fp1), fp2);
			fputc(0, fp2);
			fputc(0, fp2);
			fputc(0, fp2);
			fputc(0, fp2);
			fputc(0, fp2);
			fputc(0, fp2);
			fputc(0, fp2);
			fputc(0, fp2);
		}
		else
		{
			fputc(fgetc(fp1), fp2);
		}
	}
	fputc(0, fp2);
	fputc(0, fp2);
	fputc(0, fp2);
	fputc(0, fp2);
	fputc(0, fp2);
	fputc(0, fp2);
	fputc(0, fp2);
	fputc(0, fp2);
	fclose(fp1);
	fclose(fp2);

	if((fp1 = fopen("720x480.yuv","rb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("720x480_uv.yuv","wb")) == NULL)
		printf("eror open2\n");
	for(i = 0; i < 720 * 480 * 2; i++)
	{
		if(i % 4 == 0)
		{
			fputc(fgetc(fp1), fp2);
		}
		else if(i % 4 == 1)/*V*/
		{
			tempv = fgetc(fp1);
		}
		else if(i % 4 == 2)
		{
			tempy = fgetc(fp1);
		}
		else/*U*/
		{
			tempu = fgetc(fp1);
			fputc(tempu, fp2);
			fputc(tempy, fp2);
			fputc(tempv, fp2);
		}
	}
	fclose(fp1);
	fclose(fp2);
}