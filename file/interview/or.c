#include<stdio.h>
void main()
{
    int x;
    int countx=0;
    printf("Please input x\n");
    scanf("%d", &x);
    while(x)
    {
        countx++;
        printf("\n%d & %d=", x, x-1);
        x=x&(x-1);
        printf("%d", x);
    }
    printf("\ncountx=%d", countx);
    return;
}
