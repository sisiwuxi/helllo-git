#include<stdio.h>
#include<stdlib.h>
int a=3;
int main(int argc, char* argv[])
{
    int *p=(int *)malloc(sizeof(int));
    printf("p=0x%x\n", p);
    scanf("%d",p);
    printf("%d",a+*p);
    free(p);
}
