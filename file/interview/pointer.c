#include<stdio.h>

void main()
{
    char a[]="hello";
    printf("[%d]\n",__LINE__);
    a[0]='X';
    printf("[%d]\n",__LINE__);
    char *p="world";//指向常量字符串
    printf("[%d]\n",__LINE__);
    //p[0]='X';   
    printf("[%d]\n",__LINE__);
}
