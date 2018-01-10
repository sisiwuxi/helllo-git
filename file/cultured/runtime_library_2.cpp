#include<iostream>
#include<string>
using namespace std;
string v;
double foo()
{
    cout<<"foo"<<endl;
    return 1.0;
}
double g=foo();
int main()
{
    cout<<"main"<<endl;
}
