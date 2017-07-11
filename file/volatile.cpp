#include <iostream>
#include <string>
using namespace std;

int main()
{
    volatile const int a=10;
    int *c=(int*)&a;
    *c=5;
    cout<<a<<endl;
    return 0;
}
