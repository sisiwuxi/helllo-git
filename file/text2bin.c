#include <stdio.h>

void main()
{
    FILE *fp1;
    FILE *fp2;

    if((fp1 = fopen("1.txt","rb")) == NULL)
        printf("eror fp1 open\n");
    if((fp2 = fopen("2.txt","wb")) == NULL)
        printf("eror fp2 open\n");

    int temp = 0;
    while(!feof(fp1))
    {
        int num1 = fgetc(fp1);
        if(0x0A == num1 || 0x0D == num1 || ~0x0 == num1)
        {
            //printf("0x%x ", num1);
        }
        else if(num1 != 0x20)
        {
            //printf("0x%x ", num1);
            if(num1 >= 0x30 && num1 <= 0x39)//0~9
            {
                num1 -= 0x30;
            }
            else if(num1 >= 0x61 && num1 <= 0x66)//a~f
            {
                num1 = num1 - 0x61 + 0xa;
            }
            else if(num1 >= 0x41 && num1 <= 0x46)//A~F
            {
                num1 = num1 - 0x41 + 0xA;
            }
            int num2 = fgetc(fp1);
            //printf("0x%x ", num2);
            if(num2 != 0x20)
            {
                if(num2 >= 0x30 && num2 <= 0x39)
                {
                    num2 -= 0x30;
                }
                else if(num2 >= 0x61 && num2 <= 0x66)
                {
                    num2 = num2 - 0x61 + 0xa;
                }
                else if(num2 >= 0x41 && num2 <= 0x46)
                {
                    num2 = num2 - 0x41 + 0xA;
                }
                temp = num1 * 0x10 + num2;
            }
            else
            {
                temp = num1;
            }
            printf("--> 0x%x 0x%x 0x%x\n", num1, num2, temp);
            fputc(temp, fp2);
        }
    }
    fclose(fp1);
    fclose(fp2);
}
