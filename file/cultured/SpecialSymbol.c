#include <stdio.h>
extern char __executable_start[];
extern char __etext[];
extern char edata[];
extern char end[];

int main()
{
    printf("Executable start %X\n",__executable_start);
    printf("Text End %X\n",__etext);
    printf("Data End %X\n",edata);
    printf("Executable End %X\n",end);
    return 0;
}
