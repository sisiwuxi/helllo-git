//========file2.c=========//
#include<stdio.h>
#include "file1.h"  
int called()  
{  
    return 6;  
} 
int main()  
{  
    int val;  
    val = called(); 
    printf("fail2 val=%d\n", val); 
    printStr();  
    return 0;  
}  
