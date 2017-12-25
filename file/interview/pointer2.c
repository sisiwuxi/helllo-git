#include<stdio.h>
void main()
{
    int a_int = 0x4600;
    int *p_int = &a_int;
    printf("p_int=%p -> ",p_int);
    p_int++;
    printf("p_int=%p\n",p_int);
    long a_long = 0x4600;
    long *p_long = &a_long;
    printf("p_long=%p -> ",p_long);
    p_long++;
    printf("p_long=%p\n",p_long);
}

