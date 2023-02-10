#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;
    char* p = (char*)malloc(10);
    char* pt=p;
    for(i=0; i<10; i++)
    {
        p[i] = 'z';
    }
    delete p; // C++, free + destructor(free resource like memory, handle, network in runtime)
    pt[1] = 'x';
    free(pt); // C, malloc, calloc, realloc
    return 0;
}

int free() {
    int *ptr = malloc(sizeof(int));
    if (ptr != NULL) {
        *ptr = 10;
        free(ptr); // 释放内存
        ptr = NULL; // 避免野指针
    }
}

int destructor() {
    int *ptr = new int;
    if (ptr != NULL) {
        *ptr = 10;
        delete ptr; // 释放内存并调用析构函数
        ptr = NULL; // 避免野指针
    }
}