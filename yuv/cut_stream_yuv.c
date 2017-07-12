#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH (0)//(64*64)//(98485681)//(188000000 * 2)//
#define START (0)//(480*480)//(0x143)//
#define UV 0
#define YUYV 1
void main()
{
    FILE *fp1, *fp2;
    int i;

    if((fp1 = fopen("352x240.yuv","rb")) == NULL)/* EIT-4172.txt    NOW_BA.txt */
        printf("eror open1\n");
    if((fp2 = fopen("352x240_luma.yuv","wb")) == NULL)
        printf("eror open2\n");
    //====================START======================
    if(START)
    {
        for(i = 0; i < START; i++)
        {
            fgetc(fp1);
        }
        fgetc(fp1);
    }
    //====================LENGTH======================
    for(i = 0; i < LENGTH; i++)
    {
        fputc(fgetc(fp1), fp2);
    }
    //====================UV======================
    if(UV)
    {
        while(!feof(fp1))
        {
            //fgetc(fp1);
            fputc(fgetc(fp1), fp2);
            fgetc(fp1);
        }
    }
    //====================YUYV======================
    if(YUYV)
    {
        while(!feof(fp1))
        {
            fputc(fgetc(fp1), fp2);//V
            fgetc(fp1);//Y
            //fgetc(fp1);//Y
            //fgetc(fp1);//U
        }
    }
    printf("\n==================cut stream %d====================\n",i);
    fclose(fp1);
    fclose(fp2);
}