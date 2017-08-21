#include <stdio.h>
#define ADD_LEN (0x3180)

void main()
{
    FILE *fp1, *fp2;
    int i;

    if((fp1 = fopen("2.yuv","rb")) == NULL)
        printf("eror open1\n");
    if((fp2 = fopen("3.yuv","wb")) == NULL)
        printf("eror open2\n");

    while(!feof(fp1))
    {
        fputc(fgetc(fp1), fp2);
    }
    for(i = 0; i < ADD_LEN; i++)
    {
        fputc(0x80, fp2);//0x00 -- green, 0xff -- rose, 0x80 -- black
    }

    printf("\n==================add binary end====================\n");
    fclose(fp1);
    fclose(fp2);
}