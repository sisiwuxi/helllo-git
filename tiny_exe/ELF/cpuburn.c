#include <stdio.h>
#include <pthread.h>

static void *func(void *arg)
{
    int i = 0;
    printf("enter %s\n", __FUNCTION__);
    while(1)
        i++;
    printf("Leave %s\n", __FUNCTION__);
    return NULL;
}

int main()
{
    pthread_t thread;
    pthread_attr_t attr;
    int num = 0;
     
    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for(num = 0; num < 100; num++)
        pthread_create(&thread, &attr, func, NULL);

    while(1)
        sched_yield();
    return 0;
}
