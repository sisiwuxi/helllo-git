//==============fcntl_set.c=================//
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

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
