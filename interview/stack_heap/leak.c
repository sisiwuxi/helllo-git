#include<stdio.h>
#include<string.h>
void main()
{
    char* a = NULL;
    while(1)
    {
	a=(char*)malloc(10*1024*1024);
	if(a>0)
	{
	printf("a=%x\n",a);
	memset(a,1,10*1024*1024);
	sleep(15);
	}
	else
	{
	printf("no enough memory\n");
	}
    }   
}
