#include <stdio.h>
#include <stdlib.h>
#define BIT7 0x080
#define BIT6 0x040
#define BIT5 0x020
#define B5BIT 0x01f
#define B76BIT 0x0c0
#define B54BIT 0x030
#define B4BIT 0x0f
#define B12345BIT 0x3e
#define B0BIT 0x01
#define B8BIT ((~(~0))|0xff)

void main()
{
    FILE *fp1;
    FILE *fp2;
    int i = 0;
    int temp;

    if((fp1 = fopen("data0001.ts","rb")) == NULL)
        printf("eror fp1 open\n");
    if((fp2 = fopen("new.ts","wb")) == NULL)
        printf("eror fp2 open\n");

    temp = fgetc(fp1) & B8BIT;
    fgetc(fp1);
    fgetc(fp1);

    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(0x47 == temp)
        {
            fputc((temp & B8BIT), fp2);
            for(i=1; i<188; i++)
                fputc((fgetc(fp1) & B8BIT), fp2);
        }
        fgetc(fp1);
        fgetc(fp1);
        fgetc(fp1);
        fgetc(fp1);
    }
    fclose(fp1);
    fclose(fp2);
}