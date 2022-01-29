#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void process_exit(int s)
{
    exit(0);
}
void child_fun()
{
    int i ;
    char* a = NULL;
    pid_t tmp1=getpid();
    printf("child id is: %d\n",tmp1);
    while(1)
    {
        a = (char*) malloc( 10*1024*1024);
        if(a) 
        {
            printf("Memory Allocated at: %x\n",a);
            memset(a,1,10*1024*1024);
            sprintf(a,"%s","HelloWorld\n");
            printf("1111\n");
            sleep(15);
        } 
        else
            printf("Not Enough Memory!\n");
    }
}
void fork_child()
{
    pid_t child_process;
    int status;
    int signal_num;
    wait(&status);

    if(WIFSIGNALED(status))
        signal_num = WTERMSIG(status);

    child_process = fork();
    if(child_process == 0)
    {
        printf("fork new child process.\n");
        child_fun();
    }
}
int main()
{
    pid_t fpid;
    int i;
    printf("fork a new process.\n");
    fpid = fork();
    if(fpid > 0)
    {
        printf("fpid is parent process[%d,%d,%d]\n", fpid, getpid(), getppid());
        while(1)
        {
            signal(SIGCHLD, fork_child);
            signal(SIGTERM, process_exit);
            pause();
        }
    }
    else if(fpid == 0)
    {
        printf("fpid is child process[%d,%d,%d]\n", fpid, getpid(), getppid());
        child_fun();
    }
    else
    {
        printf("error in fork.\n");
    }
    return 0;
}
