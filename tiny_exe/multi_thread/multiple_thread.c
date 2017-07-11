/*
multiplae thread programming in linux
 */
#include<pthread.h>
#include<stdio.h>
#include<sys/time.h>
#include<string.h>
#include<unistd.h>

#define MAX 10

pthread_t thread[2];
pthread_mutex_t mut;
int number=0, i;

void *thread1()
{
    printf("thread 1 start\n");
    for(i=0;i< MAX;i++)
    {
        printf("thread 1[%d]=%d \n",i, number);
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        sleep(2);
    } 
    printf("thread 1 finished\n");
    pthread_exit(NULL);
}

void *thread2()
{
    printf("thread 2 start\n");
    for(i=0;i<MAX;i++)
    {
        printf("thread 2[%d]=%d \n",i, number);
        pthread_mutex_lock(&mut);
        number++;
        pthread_mutex_unlock(&mut);
        sleep(3);
    }
    printf("thread 2 finished\n");
    pthread_exit(NULL);
}

void thread_create(void)
{
    int temp=0;
    memset(&thread, 0, sizeof(thread));
    if((temp=pthread_create(&thread[0], NULL, thread1, NULL)) != 0)
    {
        printf("thread 1 create fail\n");
    }
    else
    {
        printf("thread 1 created\n");
    }
    if((temp=pthread_create(&thread[1], NULL, thread2, NULL)) != 0)
    {
        printf("thread 2 fail\n");
    }
    else
    {    
        printf("thread 2 created\n");
    }
}



void thread_wait(void)
{
    if(thread[0]!=0)
    {
        pthread_join(thread[0],NULL);
        printf("thread 1 finished\n");
    }
    if(thread[1]!=0)
    {
        pthread_join(thread[1],NULL);
        printf("thread 2 finished\n");
    }
}

int main()
{
    pthread_mutex_init(&mut,NULL);
    printf("thread main\n");
    thread_create();
    printf("thread main wait sub thread finish\n");
    thread_wait();

    return 0;
}

