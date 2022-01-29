#include <stdio.h> 
#include <stdlib.h> 
#include <iostream> 
#include <string>
#include <cstdlib> 
using namespace std;
int fn1(void), fn2(void), fn3(void), fn4 (void); 
int main(void)
{
    string str("hello");
    _onexit(fn1);
    _onexit(fn2);
    _onexit(fn3);
    _onexit(fn4);
    printf("This is executed first.\n");
    return 0;
}

int fn1()
{
    printf( "1next.\n" );
    return 0;
}
int fn2()
{
    printf( "2executed.\n" );
    return 0;
}
int fn3()
{
    printf( "3is \n" );
    return 0;
}
int fn4()
{
    printf( "4This \n" );
    return 0;
}


