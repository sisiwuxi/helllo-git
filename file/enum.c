#include<stdio.h>
#include<unistd.h>
typedef enum {
    abc,
    def,
    hij,
    klm
} test;
int main(int argc, char *argv[])
{
    printf("\n%d %d 0x%x %d\n",abc, hij, (1<<0), (1<<0));
    return 0;
}

