//==============fcntl_print.c=================//
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>


int main(int argc, char *argv[])
{
    int accmode, val;
    if(argc!=2)
        printf("usage:a.out <descriptor#>");
    else
        //printf("%s %s %s\n", argv[0], argv[1], argv[2]);
#if 1
    if((val=fcntl(atoi(argv[1]),F_GETFL,0))<0)
        printf("fcntl error for fd %d",atoi(argv[1]));
    accmode = val & O_ACCMODE;
    if     (accmode==O_RDONLY) printf("read only");
    else if(accmode==O_WRONLY) printf("write only");
    else if(accmode==O_RDWR)   printf("read write");
    else printf("unknown access mode");

    if(val & O_APPEND)   printf(",append");
    if(val & O_NONBLOCK) printf(",nonblocking");
    if(val & O_SYNC) printf(",synchronous writes");
    putchar('\n');
    exit(0);
#endif
}
