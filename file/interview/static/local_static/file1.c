//========file1.c=========//
#include"file1.h"
void printStr()  
{  
    int normal = 0;  
    static int stat = 0;    //this is a static local var  
    printf("normal = %d ---- stat = %d\n",normal, stat);  
    normal++;  
    stat++;  
}  
