#include <stdio.h>
#define HEIGHT (4)//(0x90)//
#define WIDTH (6)//(0xB0)//

void main()
{
	FILE *fp1, *fp2;

	if((fp1 = fopen("2.txt","rb")) == NULL)
		printf("eror open1\n"); 
	if((fp2 = fopen("3.txt","wb")) == NULL) 
		printf("eror open2\n");

/* convert 4:2:0 planar formats to yuv 4:4:4 planar formats*/ 
    unsigned int x, y;
	unsigned int start = 0, len = 0, num = 0; 

	fseek(fp1, 0, SEEK_END);
	len = ftell(fp1);
	printf("\n* %d ", len);
	while(!feof(fp1))
	{
		start = (int)(HEIGHT * WIDTH * 1.5 * num);
		if(start >= len - 1)
		{
			break;
		}
		//printf("\n* %d ", start);
		fseek(fp1, start, SEEK_SET);
		for (y = 0; y < HEIGHT * WIDTH; ++y) 
		{ 
			fputc(fgetc(fp1), fp2);
		} 
		for (y = 0; y < HEIGHT; ++y) 
		{ 
			for (x = 0; x < WIDTH; x++) 
			{ 
				int offset = start + (int)(HEIGHT * WIDTH + (y / 2) * (WIDTH / 2) + (x / 2));
				//printf("%d ", offset);
				fseek(fp1, (int)offset, SEEK_SET);
				fputc(fgetc(fp1), fp2);
			} 
		} 
		for (y = 0; y < HEIGHT; ++y) 
		{ 
			for (x = 0; x < WIDTH; x++) 
			{ 
				int offset = start + (int)(HEIGHT * WIDTH * 1.25 + (y / 2) * (WIDTH / 2) + (x / 2));
				fseek(fp1, (int)offset, SEEK_SET);
				fputc(fgetc(fp1), fp2);
			} 
		}
		num++;
	}
 
	printf("\n==================convert end num = %d====================\n", num);
	fclose(fp1);
	fclose(fp2);
}