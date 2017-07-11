#include <stdio.h>
int main()
{
    printf("\033[30;47m\033[1m");
    int x=100;
    printf("&x=%p\n",&x);
    printf("pagesize=%d\n",getpagesize());
    printf("\033[40;47m\033[0m");
    printf("\n");
}
