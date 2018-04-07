//============3-1-lseek.c==is seekable or not============//
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    if(lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        printf("%s cannot seek\n", STDIN_FILENO);
    else
        printf("seek OK\n");
    exit(0);
}
