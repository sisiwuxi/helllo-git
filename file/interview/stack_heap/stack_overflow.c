#include<stdio.h>
int main()
{
char name[8];
printf("Please type your name:");
gets(name);
printf("Hello.%s!",name);
return 0;
}
