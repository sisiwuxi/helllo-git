#include <iostream>
#include <string>
using namespace std;


class O {
    int a;
};
class A {
    int a;
    static int b; // .data, shared among all instances of the class and not part of the instance's memory layout
};
class B {
    int a;
    char b; // align 4
};
class C {
    float a;
    int b;
    char c; // align 4
};
class D {
    double a; // 8
    float b;
    int c;
    char d; // align 8
};
int main() {
    int i = 0;
    string *p=new string[2];
    p[0]= "Hello ";
    p[1]= "M ";
    cout << sizeof(O) << endl;
    cout << sizeof(A) << endl;
    cout << sizeof(B) << endl;
    cout << sizeof(C) << endl;
    cout << sizeof(D) << endl;
    cout << sizeof(p) << endl; // the size of pointer itself, 64-bit architecture
    cout << sizeof(string) << endl;
    for(i=0; i <sizeof(p)/sizeof(string); i++)
        cout << p[i];
    cout << endl;
    return 0;
}
#if 0
int main(int argc,char* argv[])
{
    int i = 0;
    string strArr[]= { "M ", "Shanghai ", "STBFAE "};
    string *p=new string[2];
    p[0]= "Hello ";
    p[1]= "M ";
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
