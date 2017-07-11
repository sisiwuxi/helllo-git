#include <stdio.h>
#define RENOVATE_LEN 0x1B7

void main()
{
    FILE *fp1, *fp2;
    int len = 0;

    if((fp1 = fopen("ntfs_1.BIN","rb")) == NULL)
        printf("error open1\n");
    if((fp2 = fopen("\\\\.\\H:","rb+")) == NULL)
        printf("error open2\n");

    while(!feof(fp2) && (len < 16))
    {
        printf("0x%02x ", fgetc(fp2));
        len++;
    }

    len = 0;
    fseek(fp1, 0, SEEK_SET);
    fseek(fp2, 0, SEEK_SET);

    while(!feof(fp1) && (len < RENOVATE_LEN))
    {
        fputc(fgetc(fp1), fp2);
        len++;
    }

    len = 0;
    fseek(fp2, 0, SEEK_SET);
    printf("\n");
    while(!feof(fp2) && (len < 16))
    {
        printf("0x%02x ", fgetc(fp2));
        len++;
    }

    printf("\n==============MBR Renovate End, write length = %d==============\n", len);
    fclose(fp1);
    fclose(fp2);
}