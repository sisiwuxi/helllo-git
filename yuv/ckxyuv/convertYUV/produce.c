#include  <stdio.h>
#include  <stdlib.h>

#define   WIDTH   720
#define   HEIGHT  576

#define   COLORy  128
#define   COLORu  128
#define   COLORv  255

void  main(void)
{
	FILE    *outFILE;
	unsigned char  c1, c2, c3;
	int   i, j;

	outFILE = fopen("test_720_576.yuv", "wb");
	if (NULL == outFILE) 
	{
		printf("Can not open to write.\n");
		exit(0);
	}

	printf("Process OK.\n");
	c1 = COLORy;
	c2 = COLORu;
	c3 = COLORv;
	for (i = 0; i < HEIGHT; ++i)
	{
		for (j = 0; j < WIDTH / 2; ++j){
			fputc(c1, outFILE);
			fputc(c2, outFILE);
			fputc(c1, outFILE);
			fputc(c3, outFILE);
		}
	}

	printf("Process OK.\n");

	fclose(outFILE);
	
}
