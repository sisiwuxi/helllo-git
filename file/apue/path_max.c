#include <errno.h>
//#include "ourhdr.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "my_err.h"

#ifdef PATH_MAX
static int pathmax=PATH_MAX;
#else
static int pathmax=0;
#endif

#define PATH_MAX_GUESS 1024
char * path_alloc(int *size)
{
    char *ptr;
    if(pathmax == 0)
    {
        errno = 0;
        if((pathmax = pathconf("/", _PC_PATH_MAX))<0)
        {
            if(errno == 0)
                pathmax = PATH_MAX_GUESS;
            else
                err_sys("pathconf error for _PC_PATH_MAX");
        }
        else
            pathmax++;
    }
    if((ptr = malloc(pathmax + 1))==NULL)
        err_sys("malloc error for pathname");
    if(size != NULL)
        *size = pathmax + 1;
    return(ptr);
}

