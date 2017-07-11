#include<stdio.h>
int main(int argc, char *argv[])
{
    char buffer[10];
    int n,a=5;
    n=sprintf(buffer,"%d",a);
    printf("\n[%s] %d\n",buffer,n);
    return 0;
}

