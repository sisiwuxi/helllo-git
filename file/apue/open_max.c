#include <errno.h>
//#include "ourhdr.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "my_err.h"
#define RLIMIT 1
#if RLIMIT
#include <limits.h>
#include <sys/resource.h>
static long openmax;
#else
#ifdef OPEN_MAX
static int openmax=OPEN_MAX;
#else
static int openmax=0;
#endif
#endif

#define OPEN_MAX_GUESS 256
int open_max(void)
{
#if RLIMIT
    struct rlimit rl;
#endif
    if(openmax == 0)
    {
        errno = 0;
#if RLIMIT
        if((openmax = sysconf(_SC_OPEN_MAX))<0 || openmax==LONG_MAX)
#else
        if((openmax = sysconf(_SC_OPEN_MAX))<0)
#endif
        {
#if RLIMIT
	    if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
                printf("can't get file limit");
            if(rl.rlim_max == RLIM_INFINITY)
                openmax = OPEN_MAX_GUESS;
            else
                openmax = rl.rlim_max;		
#else
            if(errno == 0)
                openmax = OPEN_MAX_GUESS;
#endif
            err_sys("sysconf error for _SC_PATH_MAX");
        }
	else
	    printf("\nopenmax=%d", openmax);
    }
    return(openmax);
}

void main()
{
    open_max();
    printf("\nopenmax=%d", openmax);
    return;
}
