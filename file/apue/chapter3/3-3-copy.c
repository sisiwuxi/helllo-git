//============3-3-copy.c==============//
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFFSIZE 8192

void set_fl(int fd, int flags)/*flags are file status flags to turn on*/
{
    int val;
    if((val=fcntl(fd,F_GETFL,0))<0)
        printf("fcntl F_GETFL error");
    val|=flags;/*turn on flags*/
    if(fcntl(fd,F_SETFL,val)<0)
        printf("fcntl F_SETFL error");
}

void clr_fl(int fd, int flags)/*flags are file status flags to turn off*/
{
    int val;
    if((val=fcntl(fd,F_GETFL,0))<0)
        printf("fcntl F_GETFL error");
    val&=~flags;/*turn flags off*/
    if(fcntl(fd,F_SETFL,val)<0)
        printf("fcntl F_SETFL error");
}

int main(void)
{
    int n;
    char buf[BUFFSIZE];

    while((n=read(STDIN_FILENO,buf,BUFFSIZE))>0)
    {
        //set_fl(STDOUT_FILENO, O_SYNC);
        if(write(STDOUT_FILENO,buf,n)!=n)
            printf("write error");
        if(n<0)
            printf("read error");
    }
    exit(0);
}

