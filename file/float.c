#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main(){
    printf("float in memory:\n"); 
    float a;
    char s[100];
    a=0.0;
    memset(s, 0, sizeof(char)*100);
    sprintf(s,"0x%lx",*(long*)&a);
    printf("%f is: %s\n",a,s);
    a=1.0;
    memset(s, 0, sizeof(char)*100);
    sprintf(s,"0x%lx",*(long*)&a);
    printf("%f is: %s\n",a,s);
    a=9.125;
    memset(s, 0, sizeof(char)*100);
    sprintf(s,"0x%lx",*(long*)&a);
    printf("%f is: %s\n",a,s);
}
