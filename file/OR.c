#include<stdio.h>

int func1()
{
    printf("\n func1 \n");
    return 1;
}
int func2()
{
    printf("\n func2 \n");
    return 1;
}
int func3()
{
    printf("\n func3 \n");
    return 1;
}
int main(int argc, char *argv[])
{
    if(func1() || func2() || func3())
    {
        printf("\n OR finish\n");
    }
    return 0;
}

