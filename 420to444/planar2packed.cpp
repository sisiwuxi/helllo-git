#include <stdio.h>

#define HEIGHT (6)//(0x90)
#define WIDTH (4)//(0xB0)

void main()
{
	FILE *fp1, *fp2;

	if((fp1 = fopen("3.txt","rb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("4.txt","wb")) == NULL) 
		printf("eror open2\n");

    unsigned int x, y; 
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
		//yuv444
		for (y = 0; y < HEIGHT; ++y) 
		{ 
			for (x = 0; x < WIDTH; x++) 
			{ 
				//Y
				int offset = start + y * WIDTH + x;
				fseek(fp1, (int)offset, SEEK_SET);
				fputc(fgetc(fp1), fp2);
				//U
				offset = start + HEIGHT * WIDTH + y * WIDTH + x;
				fseek(fp1, (int)offset, SEEK_SET);
				fputc(fgetc(fp1), fp2);
				//V
				offset = start + HEIGHT * WIDTH * (1 + (int)(yuv_offset - 1)/2) + y * WIDTH + x;
				fseek(fp1, (int)offset, SEEK_SET);
				fputc(fgetc(fp1), fp2);
			} 
		} 
		num++;
	}
	printf("\n==================convert end num=%d====================\n", num);
	fclose(fp1);
	fclose(fp2);
}