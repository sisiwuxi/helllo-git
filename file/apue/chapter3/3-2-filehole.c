//============3-2-filehole.c==============//
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

char buf1[]="abcdefghij";
char buf2[]="ABCDEFGHIJ";

int main(void)
{
    int fd;
    if((fd=creat("file.hole",O_RDWR))<0)
        printf("create error");
    if(write(fd,buf1,10)!=10)
        printf("buf1 write error");
    if(lseek(fd, 40, SEEK_SET) == -1)
        printf("lseek error\n");
    if(write(fd,buf2,10)!=10)
        printf("buf1 write error");
    exit(0);
}
