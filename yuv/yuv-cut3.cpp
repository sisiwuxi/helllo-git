#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	FILE *fp1, *fp2;
	int len = 0;
	int i, k = 0;

	if((fp1 = fopen("[720x480]_V.yuv","rb")) == NULL)/* V8Y8 U8Y8 */
		printf("eror open1\n"); 
	if((fp2 = fopen("[716x480]_V.yuv","wb")) == NULL)
		printf("eror open2\n");

	fseek(fp1, 0, SEEK_END);
	len = ftell(fp1);
	printf("len = %d\n", len);

	fseek(fp1, 0, SEEK_SET);
	for(i = 0; i < 716 * 480 / 2; i++)
	{
		fputc(fgetc(fp1), fp2);	
	}
	printf("i = %d\n", i);
	fclose(fp1);
	fclose(fp2);
}