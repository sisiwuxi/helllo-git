/*=========test.c========*/  
#include "base64.h"  
#include <stdio.h>  
  
int main(int argc,char **argv)  
{  
    if(strcmp(argv[1],"-d") == 0)  
        printf("%s\n",(char *)(base64_decode(argv[2])));  
    else  
        printf("%s\n",base64_encode(argv[1]));  
  
    return 0;  
}  
