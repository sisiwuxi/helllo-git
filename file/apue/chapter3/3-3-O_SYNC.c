//============3-3-O_SYNC.c==============//
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "time.h"

#define BUFFSIZE 8192
#define FILESIZE 15728640

int main(void)
{
    int fd1, fd2;
    int n, total=0;
    char buf[BUFFSIZE];
    struct timeval startTime,endTime;
    float Timeuse; 

    long    i = 10000000L;    
    clock_t start, finish;    
    double  duration;    

    gettimeofday(&startTime,NULL);  
    start = clock(); 

    fd1 = open("15m.txt", O_RDWR | O_CREAT);
    //fd2 = open("15m_copy_sync.txt", O_RDWR|O_CREAT|O_SYNC);
    fd2 = open("15m_copy.txt", O_RDWR | O_CREAT);

    while((n=read(fd1,buf,BUFFSIZE))>0)
    {
        if(write(fd2,buf,n)!=n)
            printf("write error");
        if(n<0)
            printf("read error");
        else        
            total+=n;
        if(total >= FILESIZE)
            break;
    }
    close(fd1);
    close(fd2);
    gettimeofday(&endTime,NULL); 
    finish = clock(); 

    Timeuse = 1000000*(endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec); 
    Timeuse /= 1000000;  
    printf("Timeuse = %f\n",Timeuse); 

    duration = (double)(finish - start) / CLOCKS_PER_SEC;  
    printf( "duration=%f seconds\n", duration ); 
    exit(0);
}
