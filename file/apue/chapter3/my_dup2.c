#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int my_dup(int fd1, int newfd);

int main(int argc, char *argv[])
{
    char buffer[] = "asdasfaff";
    printf("Input file descriptor\n");
    int val;
    scanf("%d", &val);
    my_dup(STDOUT_FILENO, val);
    if (write(val, buffer, strlen(buffer)) == -1)
    {
        printf("write error");
        exit(-1);
    }
    printf("\nSuccess!");
    exit(0);
}

int my_dup(int fd1, int newfd)
{
    int temp_fd;
    if (newfd < 0)
        printf("unsupported file descripter！");
    printf("fd1=%d newfd=%d\n", fd1, newfd);
    if (fd1 == newfd)
        return newfd;

    if(close(newfd)==0)/*0 1 2*/
    {
        temp_fd = dup(fd1);
        printf("close newfd=%d temp_fd=%d\n", newfd, temp_fd);
        return temp_fd;
    }
    else
    {
        while(1)
        {
            temp_fd = dup(fd1);
            printf("while fd1=%d temp_fd=%d\n", fd1, temp_fd);
            if (newfd == temp_fd)
            {
                return newfd;
            }
        }
    }
}
