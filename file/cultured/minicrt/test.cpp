#include<iostream>
#include<string>
using namespace std;
int main(int argc,char* argv[])
{
    string* msg=new string("Hello World");
    cout<<*msg<<endl;
    delete msg;
    return 0;
}
