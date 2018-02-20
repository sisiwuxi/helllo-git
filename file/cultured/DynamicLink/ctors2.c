#include<stdio.h>
void my_init(void) __attribute__((constructor));
void my_init(void)
{
    printf("Hello ");
}
int main()
{
    printf("World too!\n");
    return 0;
}
