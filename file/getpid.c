#include<stdio.h>
#include<unistd.h>

#define TASK_NAME_LEN 256
main()
{
    char cmd[TASK_NAME_LEN]= {0};
    char name[TASK_NAME_LEN]= {0};
    FILE *fptr;
    pid_t pid = getpid();
    int uid = getuid();
    int euid = geteuid();
    printf("pid=%d uid=%d euid=%d\n",pid, uid, euid);
    sprintf(cmd, "readlink /proc/%d/exe", pid);
    printf("cmd=%s\n", cmd);
    if((fptr=popen(cmd,"r"))!=NULL)
    {
        if(fgets(name,255,fptr)!=NULL)
        {
            printf("the path name is %s\n",name);
        }
    }
    pclose(fptr);
}

