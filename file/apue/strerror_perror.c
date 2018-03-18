//============strerror_perror.c================//
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
    fprintf(stderr,"EACCES:%s\n",strerror(EACCES));
    errno=ENOENT;
    perror(argv[0]);
    exit(0);
}
