#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

int alloc_memory(char *p , int size)
{
    printf("\nbefore malloc %p\n",p);
    p = (char *)malloc(size);
    if(!p)
    {
        printf("malloc error  \n");
        return -1;
    }

    //len of malloc(0)
    printf("len of malloc(%d)  is  %d  ,the ture is %d\n",size,strlen(p),malloc_usable_size(p));

    //the first member 
    printf("the first member of malloc(%d) is %p:%d \n",size,p,*p);

    //set the first member
    *p = 10;
    printf("set the first member of malloc(%d) is %p:%d \n",size,p,*p);

    //memcpy
    memset(p,'\0',12);
    memcpy(p,"01234567890123456789",12);
    printf("after memcpy , the content is %s   len is %d  , the ture is %d \n",p,strlen(p),malloc_usable_size(p));

    free(p);
    p = NULL;

    printf("\n");
}


int main(int argc ,char **argv)
{
    int size = -1;

    char *p = NULL;

    //malloc(0)
    size = 0;
    alloc_memory(p,size);
    
    //malloc(5)
    size = 24;
    alloc_memory(p,size);

    //malloc(20)
    size = 25;
    alloc_memory(p,size);
    return 0;
}
