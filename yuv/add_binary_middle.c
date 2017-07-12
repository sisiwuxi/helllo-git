#include <stdio.h>

#define ADD_LEN (76)

void main()
{
    FILE *fp1, *fp2;
    int i;

    if((fp1 = fopen("7368--1cc8.yuv","rb")) == NULL)
        printf("eror open1\n");
    if((fp2 = fopen("7368.yuv","wb")) == NULL)
        printf("eror open2\n");


    for(i = 0; i < ADD_LEN; i++)
    {
        fputc(fgetc(fp1), fp2);
    }

    fputc(0x00, fp2);
    fputc(0x00, fp2);
    fputc(0x01, fp2);
    fputc(0xB8, fp2);

    fputc(0x00, fp2);
    fputc(0x08, fp2);
    fputc(0x00, fp2);
    fputc(0x00, fp2);

    while(!feof(fp1))
    {
        fputc(fgetc(fp1), fp2);
    }

    fputc(0x00, fp2);
    fputc(0x00, fp2);
    fputc(0x01, fp2);
    fputc(0xB7, fp2);

    printf("\n==================add binary end====================\n");
    fclose(fp1);
    fclose(fp2);
}