#include<stdio.h>
union
{
    int i;
    char x[2];
}a;

void main()
{
    a.x[0]=10;
    a.x[1]=1;
    printf("%d",a.i);
}

