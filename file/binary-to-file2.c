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
    FILE *fp2;
    int i = 0;
    int k = 1;
    int temp;
    int H4temp;
    int L4temp;
    /*
    	if((fp1 = fopen("M_strings.bin","rb")) == NULL)
    		printf("eror fp1 open\n");
    	if((fp2 = fopen("M_strings.inc","wb")) == NULL)
    		printf("eror fp2 open\n");
    */
    if((fp1 = fopen("HDMI1.bin","rb")) == NULL)//"1.txt"
        printf("eror fp1 open\n");
    if((fp2 = fopen("HDMI1.txt","wb")) == NULL)
        printf("eror fp2 open\n");

    while(!feof(fp1))
    {
        temp = fgetc(fp1) & B8BIT;
        //fputc(0x30, fp2);//0
        //fputc(0x78, fp2);//x
        H4temp = (temp >> 4) & BL4BIT;
        L4temp = temp & BL4BIT;

        if((H4temp >= 0x00) && (H4temp <= 0x09))
        {
            fputc((H4temp + 0x30), fp2);
        }
        else if((H4temp >= 0x0a) && (H4temp <= 0x0f))
        {
            fputc((H4temp + 0x37), fp2);//0x57
        }
        if((L4temp >= 0x00) && (L4temp <= 0x09))
        {
            fputc((L4temp + 0x30), fp2);
        }
        else if((L4temp >= 0x0a) && (L4temp <= 0x0f))
        {
            fputc((L4temp + 0x37), fp2);//0x57
        }
        //fputc(0x2C, fp2);
        if(k % 16)
        {
            //fputc(0x20, fp2);
        }
        else
        {
            fputc(0x0D, fp2);
            fputc(0x0A, fp2);
        }
        k++;
    }
    printf("\nk=%d\n",k);
    fclose(fp1);
    fclose(fp2);
}
