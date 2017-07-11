#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{
    char buf[80];
    int fd;
    unlink( "zieckey_fifo" );
    mkfifo( "zieckey_fifo", 0777 );
    if ( fork() > 0 )
    {
        char s[] = "Hello!\n";
        fd = open( "zieckey_fifo", O_WRONLY );
        write( fd, s, sizeof(s) );
        //close( fd );
    }
    else
    {
        fd = open( "zieckey_fifo", O_RDONLY );
        read( fd, buf, sizeof(buf) );
        printf("The message from the pipe is:%s\n", buf );
        //close( fd );
    }
    return 0;
}
