#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define B8BIT ((~(~0))|0xff)
#define HEADER (54)
void main()
{
    FILE *fp1, *fp2;
    int i;
    int temp_three[3] = {0};

    if((fp1 = fopen("01.bmp","rb")) == NULL)
        printf("eror open1\n");
    if((fp2 = fopen("001.bmp","wb")) == NULL)
        printf("eror open2\n");
    for(i=0; i<HEADER; i++)
        fputc(fgetc(fp1) & B8BIT, fp2);
    while(!feof(fp1))
    {
        for(i=0; i<3; i++)
            temp_three[i] = (fgetc(fp1) & B8BIT);
        if(temp_three[0] == 0 && temp_three[1] == 0 && temp_three[2] == 0)
        {
            fputc(0x0A, fp2);
            fputc(0x0A, fp2);
            fputc(0x0A, fp2);
        }
        else
        {
            fputc(temp_three[0], fp2);
            fputc(temp_three[1], fp2);
            fputc(temp_three[2], fp2);
        }
    }

    printf("\n==================cut stream %d====================\n",i);
    fclose(fp1);
    fclose(fp2);
}