//pathname.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc,char *argv[])
{
    DIR * dp;
    struct dirent *filename;
    if (argc < 2)
    {
        printf("pathname is required\n");
        exit(1);
    }

    dp = opendir(argv[1]);
    if (!dp)
    {
        fprintf(stderr,"open directory error\n");
        return 0;
    }
    while (filename = readdir(dp))
    {
        printf("filename:%-10s\td_info:%ld\t d_reclen:%us\n", \
            filename->d_name,filename->d_ino,filename->d_reclen);
    }
    closedir(dp);
    exit(0);
}

