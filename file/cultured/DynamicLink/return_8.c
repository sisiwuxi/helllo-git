/*==========return_8.c=============*/
long foo()
{
    return 0x12345678abcd;
}
#if 0
#include<stdio.h>
void main()
{
    printf("sizof(long)=%ld\n", sizeof(long));//8 in 64bit
}
#endif
