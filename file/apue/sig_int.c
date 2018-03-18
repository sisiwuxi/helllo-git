//======================process_control.c===============//
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "my_err.h"
#include <signal.h>

static void sig_int(int);/*our signal-catching function*/

int main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;
    if(signal(SIGINT, sig_int)==SIG_ERR)
        err_sys("signal error");
    printf("%%");/*print prompt*/
    while(fgets(buf,MAXLINE,stdin)!=NULL)
    {
        buf[strlen(buf)-1] = 0;/*replace newline with null*/
        if((pid = fork())<0)
            err_sys("fork error");
        else if(pid == 0)/*child*/
        {
            execlp(buf, buf, (char*)0);//unistd.h
            err_ret("couldn't execute:%s", buf);
            exit(127);
        }
        if((pid = waitpid(pid,&status,0))<0)/*parent*/
            err_sys("waitpid error");
        printf("%%");
    }
    exit(0);
}

void sig_int(int signo)
{
    printf("interrupt\n%% ");
    //exit(0);
}
