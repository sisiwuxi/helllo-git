#include <stdio.h>

void main()
{
    FILE *fp1;
    FILE *fp2;

    if((fp1 = fopen("Rsa_N.txt","rb")) == NULL)
        printf("error fp1 open\n");
    if((fp2 = fopen("Rsa_N_2.txt","wb")) == NULL)
        printf("error fp2 open\n");

    int num = 0;
    while(!feof(fp1))
    {
        fputc(0x30, fp2);
        fputc(0x78, fp2);
        fputc(fgetc(fp1), fp2);
        fputc(fgetc(fp1), fp2);
        fputc(0x2C, fp2);
        num++;
        if(!(num%16))
        {
            fputc(0x0D, fp2);
            fputc(0x0A, fp2);
        }
    }
    fclose(fp1);
    fclose(fp2);
}
