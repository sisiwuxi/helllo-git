#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
void main()
{
	FILE *fp1, *fp2;
	int len = 0, i;

	if((fp1 = fopen("[720x480n]_Y_new.yuv","wb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("[720x480n]_Y.yuv","rb")) == NULL)
		printf("eror open2\n");

	fseek(fp2, 0, SEEK_END);
	len = ftell(fp2);
	printf("len = %d\n", len);
	fseek(fp2, 0, SEEK_SET);

	while(!feof(fp2))
	{
		fputc(fgetc(fp2), fp1);
	}
	for(i = 0; i < len; i++)
	{
		fputc(128, fp1);
	}

	fclose(fp1);
	fclose(fp2);

}
*/

void main()
{
	FILE *fp1, *fp2, *fp3, *fp4;
	int len = 0;

	if((fp1 = fopen("[720x480n_new].yuv","wb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("[720x480n]_Y.yuv","rb")) == NULL)
		printf("eror open2\n");
	if((fp3 = fopen("[720x480n]_U.yuv","rb")) == NULL)
		printf("eror open3\n");
	if((fp4 = fopen("[720x480n]_V.yuv","rb")) == NULL)
		printf("eror open4\n");

	while(!feof(fp2))
	{
		fputc(fgetc(fp2), fp1);
	}
	while(!feof(fp3))
	{
		fputc(fgetc(fp3), fp1);
	}
	while(!feof(fp4))
	{
		fputc(fgetc(fp4), fp1);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
}

/*
void main()
{
	FILE *fp1, *fp2, *fp3, *fp4;
	int i, len = 0;

	if((fp1 = fopen("[25809_JPEG].yuv","wb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("[25809]_Y.yuv","rb")) == NULL)
		printf("eror open2\n");
	if((fp3 = fopen("[25809]_U.yuv","rb")) == NULL)
		printf("eror open3\n");
	if((fp4 = fopen("[25809]_V.yuv","rb")) == NULL)
		printf("eror open4\n");

	for(i = 0; i < (719 * 479 / 2); i++)
	{
		fputc(fgetc(fp2), fp1);
		fputc(fgetc(fp3), fp1);
		fputc(fgetc(fp2), fp1);
		fputc(fgetc(fp4), fp1);
		
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
}
*/