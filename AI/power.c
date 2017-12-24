#include<stdio.h>

void main()
{
    int i, powers=1;
    float base_num = 1.01, result=1.0;
    printf("Pls input base_num & powers\n");
    scanf("%f %d", &base_num, &powers);     
    for(i=0;i<powers;i++)
    {
        result = result * base_num;
    }
    printf("%f^%d=%f\n", base_num, powers, result);
}
