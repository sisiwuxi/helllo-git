#include<stdio.h>
#include<stdlib.h>
//int atexit(void (*function)(void)); 
void foo(void)
{
    printf("bye!\n");
}
int main()
{
    atexit(&foo);
    printf("end of main\n");
}
