#include<stdio.h>
void my_init(void)
{
    printf("Hello ");
}
typedef void (*ctor_t)(void);
ctor_t __attribute__((section(".ctors"))) my_init_p=&my_init;
int main()
{
    printf("World!\n");
    return 0;
}
