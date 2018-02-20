#include<stdio.h>
void main(void)
{
    unsigned int a = 6;
    int b = -20;
    (a+b > 6) ? puts("> 6") : puts("<= 6");
    printf("%ld-%ld",sizeof(unsigned int),sizeof(int));
}

