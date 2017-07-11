#include <iostream>
#include <string>
using namespace std;

class A {
    int a;
    static int b;
};
class B {
    int a;
    char b;
};
class C {
    float a;
    int b;
    char c;
};
class D {
    double a;
    float b;
    int c;
    char d;
};
int main() {
    int i = 0;
    string *p=new string[2];
    p[0]= "Hello ";
    p[1]= "MStar ";
    cout << sizeof(A) << endl;
    cout << sizeof(B) << endl;
    cout << sizeof(C) << endl;
    cout << sizeof(D) << endl;
    for(i=0; i <sizeof(p)/sizeof(string); i++)
        cout << p[i];
    cout << endl;
    return 0;
}
#if 0
int main(int argc,char* argv[])
{
    int i = 0;
    string strArr[]= { "MStar ", "Shanghai ", "STBFAE "};
    string *p=new string[2];
    p[0]= "Hello ";
    p[1]= "MStar ";
    cout <<sizeof(strArr) <<endl;
    cout <<sizeof(p) <<endl;
    cout <<sizeof(string) <<endl;
    for(i=0; i <sizeof(strArr)/sizeof(string); i++)
        cout << strArr[i];
    for(i=0; i <sizeof(p)/sizeof(string); i++)
        cout << p[i];
    cout << endl;
}
#endif
