#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void)
{
    unsigned long i = 1;
    if(*((char*)&i) == 1)
    {
        printf("this is little endian.\n");
    }
    else if(*((char*)&i) == 0)
    {
        printf("this is big endian. \n");
    }
    else
    {
        printf("i do not know. \n");
    }

    i = ntohl(i);
    printf("%lu \n", i);
    printf("%#x \n", i);
    printf("%#o \n", i);

    i = ntohl(i);
    printf("%lu \n", i);
    printf("%#x \n", i);
    printf("%#o \n", i);

    return 0;
}

