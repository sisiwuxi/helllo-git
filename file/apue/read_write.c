#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#define BUFFSIZE 8192
int main(void)
{
    int n;
    char buf[BUFFSIZE];
    while((n=read(STDIN_FILENO,buf, BUFFSIZE))>0)
    {
        if(write(STDOUT_FILENO, buf, n) != n)
            fprintf(stderr,"write error");
    }
    if(n<0)
        fprintf(stderr,"read error");
    exit(0);
}
