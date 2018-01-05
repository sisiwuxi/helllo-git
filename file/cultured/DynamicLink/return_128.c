/*==========return_128.c=============*/
#include<stdio.h>
typedef struct big_thing
{
    char buf[128];
}big_thing;

big_thing return_test()
{
    big_thing b;
    b.buf[0] = 0;
    printf("%ld\n", sizeof(b));
    return b;
}

int main()
{
    big_thing n = return_test();
}

