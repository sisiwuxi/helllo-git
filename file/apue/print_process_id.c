//===========print_process_id.c====================//
//#include "ourhdr.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("hello world from process ID %d\n", getpid());
    exit(0);
}
