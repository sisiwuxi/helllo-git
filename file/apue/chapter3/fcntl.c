/*create sub progress in function*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    pid_t pid;
    //以追加的形式打开文件
    int fd = fd = open("test.txt", O_TRUNC | O_RDWR | O_APPEND | O_CREAT, 0777);
    if(fd < 0)
    {
        perror("open");
        return -1;
    }
    printf("fd = %d\n", fd);

    fcntl(fd, F_SETFD, 0);//关闭fd的close-on-exec标志
    write(fd, "hello c program\n", strlen("hello c program!\n"));
    pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return -1;
    }
    if(pid == 0)
    {
        printf("fd = %d\n", fd);
        int ret = execl("./main", "./main", (char *)&fd, NULL);
        if(ret < 0)
        {
            perror("execl");
            exit(-1);
        }
        exit(0);
    }

    wait(NULL);
    write(fd, "hello c++ program!\n", strlen("hello c++ program!\n"));
    close(fd);
    return 0;
}
