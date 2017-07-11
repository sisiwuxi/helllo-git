#include<stdio.h>
#include<unistd.h>
int main(int argc, char *argv[])
{
    int ret = 0;
    ret = open("1.txt",0666);
    printf("\nret=%d\n",ret);
    return 0;
}

