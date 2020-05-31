//$ gcc copy.c -ocopy
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1

void main(int argc, char* argv[])
{
    if(argc != 3){
        fprintf(stderr, "usage %s src dst \n", argv[0]);
        exit(0);
    }
    int fr=open(argv[1], O_RDONLY);
    if(fr == -1) {
	perror("open read!");exit(1);
    }
#if 0
/*
sync
fsync
fdatasync
*/
    int fw=open(argv[2], O_WRONLY|O_CREAT|O_DIRECT, 0644);
    posix_memalign(&buf, 512, 1024);
    int fw=open(argv[2], O_WRONLY|O_CREAT|O_SYNC, 0644);
    int fw=open(argv[2], O_WRONLY|O_CREAT, 0644);
#endif
    int fw=open(argv[2], O_WRONLY|O_CREAT, 0644);
    if(fw == -1) {
	perror("open write!");exit(1);
    }
    char buf[BUF_SIZE] = {};
    int ret = 0;
    while((ret = read(fr, buf, BUF_SIZE)) > 0){
        write(fw, buf, ret);
    }
   
    close(fr);
    close(fw);
}
