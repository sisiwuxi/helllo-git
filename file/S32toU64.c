#include<stdio.h>
#include<unistd.h>
int main(int argc, char *argv[])
{
    unsigned long long ull = -1;
    signed long long temp_ull = 0;
    signed long long temp_sl = 0;
    signed long sl = 123;
    temp_ull = (signed long long)ull;
    temp_sl = (signed long long)sl;
    printf("\n sl = 0x%lx , ull=0x%llx, temp_ull=0x%llx tem_sl=0x%llx\n",sl, ull, temp_ull, temp_sl);
    if(temp_sl > temp_ull)
    {
        printf("\n sl = %ld > ull=%lld\n",sl, ull);
    }
    else
    {
        printf("\nsl = %ld < ull=%lld\n",sl, ull);
    }
    unsigned long long test1 = 0;
    unsigned long long test2 = 0;
    unsigned long long subtract_test = 0;
    test1=0x1a5c9;
    test2=0x1ad1e;
    subtract_test=test2-test1;
    printf("\n0x%llx-0x%llx=0x%llx\n",test1,test2,subtract_test);
    return 0;
}

