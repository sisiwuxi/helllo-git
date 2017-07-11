#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    unsigned int i = 1;
    if(*((char*)&i) == 0)
    {
        printf("this is big endian. \n");
    }
    else if(*((char*)&i) == 1)
    {
        printf(" this is little endian. \n");
    }
    else
    {
        printf("sorry, i do not know . \n");
    }
    return 0;
}
