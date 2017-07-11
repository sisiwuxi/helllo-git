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
#define BL4BIT ((~(~0))|0x0f)
#define BH4BIT ((~(~0))|0xf0)

void main()
{
    FILE *fp1;

    int i = 0;
    int k = 1;
    int temp;
    int H4temp;
    int L4temp;

    if((fp1 = fopen("1.pgm","rb")) == NULL)//"1.txt"
        printf("eror fp1 open\n");

    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        if(temp!=0)
            printf("\nk=%d 0x%x\n",k, temp);
        k++;
    }
    printf("\nk=%d\n",k);
    fclose(fp1);
}
