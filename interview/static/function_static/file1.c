//========file1.c=========//
#include "file1.h"  
static int called()  
{  
return 6;  
}  
void printStr()
{  
int returnVal;  
returnVal = called();  
printf("returnVal=%d\n",returnVal);  
}  
