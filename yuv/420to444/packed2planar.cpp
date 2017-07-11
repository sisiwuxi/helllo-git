#include <stdio.h>

#define HEIGHT (4)//(0x90)
#define WIDTH (6)//(0xB0)

void main()
{
	FILE *fp1, *fp2;

	if((fp1 = fopen("4.txt","rb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("5.txt","wb")) == NULL) 
		printf("eror open2\n");

	//yuv444
	int i = 0;
	unsigned int start = 0, len = 0, num = 0; 

	fseek(fp1, 0, SEEK_END);
	len = ftell(fp1);
	printf("\n* %d ", len);
	//420 -- 1.5, 422 -- 2, 444 -- 3
	unsigned int yuv_offset = 3;	
	while(!feof(fp1))
	{	
		start = (int)(HEIGHT * WIDTH * yuv_offset * num);
		if(start >= len - 1)
		{
			break;
		}
		fseek(fp1, (int)start, SEEK_SET);
		//Y
		for(i = 0; i < HEIGHT * WIDTH; i++)
		{
			fputc(fgetc(fp1), fp2);
			fgetc(fp1);
			fgetc(fp1);
		}
		//U
		fseek(fp1, (int)start, SEEK_SET);
		for(i = 0; i < HEIGHT * WIDTH; i++)
		{
			fgetc(fp1);
			fputc(fgetc(fp1), fp2);
			fgetc(fp1);
		}
		//V
		fseek(fp1, (int)start, SEEK_SET);
		for(i = 0; i < HEIGHT * WIDTH; i++)
		{
			fgetc(fp1);
			fgetc(fp1);
			fputc(fgetc(fp1), fp2);
		}
		num++;
	}
	printf("\n==================convert end num=%d====================\n", num);
	fclose(fp1);
	fclose(fp2);
}