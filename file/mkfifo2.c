#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
int main( int argc, char **argv )
{
    mode_t mode = 0666;
    if ( argc !=2 )
    {
        printf( "Usage:[%s] fifo_filename\n", argv[0] );
        return -1;
    }
    if (mkfifo( argv[1], mode)<0 )
    {
        perror( "mkfifo");
        return -1;
    }
    return 0;
}
