/*
lseek read and add on a file opened with O_APPEND
test.txt before 00000000001111111111222222222233333333334444444444
test.txt after 000000000011111111112222222222333333333344444444447777777777
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFSIZE 10//8192

int main()
{
    pid_t pid;
    char buf[BUFFSIZE];
    int i, n=0;
    //以追加的形式打开文件，不能用O_CREAT，否则读不到数据
    int fd = fd = open("test.txt", O_RDWR | O_APPEND, 0777);
    if(fd < 0)
    {
        perror("open");
        return -1;
    }
    printf("[%d]fd = %d\n", __LINE__, fd);
/*==================lseek+read==================*/
    if(lseek(fd, 10, SEEK_SET) == -1)
        printf("%d cannot lseek\n", fd);
    else
        printf("lseek OK\n");

    if((n=read(fd,buf,BUFFSIZE))>0)
    {
        printf("read %d\n", n);
        for(i=0;i<n;i++)
        {
            printf("%d",buf[i]);
        }
    }    
    else
        printf("read error\n");
    printf("\n");
/*==================lseek+write==================*/
    if(lseek(fd, 10, SEEK_SET) == -1)
        printf("%d cannot lseek\n", fd);
    else
        printf("lseek OK\n");
    for(i=0;i<BUFFSIZE;i++)
    {
        buf[i]=0x37;
    }
    n=BUFFSIZE;
    if(write(fd,buf,n)!=n)
        printf("write error");
    else
    {
        printf("write %d ok\n", n);
        lseek(fd, 10, SEEK_SET);
        read(fd,buf,n);
        for(i=0;i<n;i++)
        {
            printf("%d",buf[i]);
        }
    }
    return 0;
}
