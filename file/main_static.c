#include <stdio.h>
int fun1();
static int num = 0;//fun1();
#define NUM fun1()
void main()
{
    printf("enter main\n");
    printf("enter num=%d NUM=%d\n",num,NUM);
}
int fun1()
{
    printf("enter fun1\n");
    return 1;
}
