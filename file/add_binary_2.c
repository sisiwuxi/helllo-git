#include <stdio.h>
#define ADD_LEN (11)

void main()
{
    FILE *fp1, *fp2;
    int temp[ADD_LEN + 1] = {0xD6,0xDD,0xBB,0x7F,0xED,0xE7,0x37,0xDE,0x36,0x00,0x00};
    //int temp[ADD_LEN + 1] = {0xFF,0xF1,0x50,0x80,0x07,0x5F,0xFC,0xDE,0x36,0x00,0x00};
    int i;

    if((fp1 = fopen("abc.aac","rb")) == NULL)
        printf("eror open1\n");
    if((fp2 = fopen("abc_new.aac","wb")) == NULL)
        printf("eror open2\n");

    for(i = 0; i < ADD_LEN; i++)
    {
        fputc(temp[i], fp2);
    }
    while(!feof(fp1))
    {
        fputc(fgetc(fp1), fp2);
    }

    printf("\n==================add binary end====================\n");
    fclose(fp1);
    fclose(fp2);
}