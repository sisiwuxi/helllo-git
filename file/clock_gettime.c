#include<stdio.h>
#include<sys/time.h>
#include<time.h>

int main(int argc,char *argv[])
{
    struct timespec time1 = {0, 0};

    clock_gettime(CLOCK_REALTIME, &time1);
    printf("CLOCK_REALTIME: %d, %d\n", time1.tv_sec, time1.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    printf("CLOCK_MONOTONIC: %d, %d\n", time1.tv_sec, time1.tv_nsec);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    printf("CLOCK_PROCESS_CPUTIME_ID: %d, %d\n", time1.tv_sec, time1.tv_nsec);

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time1);
    printf("CLOCK_THREAD_CPUTIME_ID: %d, %d\n", time1.tv_sec, time1.tv_nsec);

    printf("\n%d\n", time(NULL));

    sleep(5);

    return 0;
}
