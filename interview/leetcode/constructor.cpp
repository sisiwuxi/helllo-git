#include<iostream>

using namespace std;
class Complex
{
public:
    double real, imag;
    Complex(double r, double i) {
        real= r; imag = i;
        cout<<"Complex Constructor called"<<endl;
    }
    Complex(const Complex &c){
        real = c.real + 1; 
        imag = c.imag + 2;
        cout<<"Complex Copy Constructor called"<<endl;
    }
};

class A
{
public:
  A(){cout<<"A Constructor called"<<endl;}
  A(A &a) {cout<<"A Copy Constructor called"<<endl;}
  int v;
  A(int n) {v = n;cout<<"A redefine Constructor called"<<endl;}
  A(const A &a) {v=a.v;cout<<"A const Copy Constructor called"<<endl;}
};

void Func(A a) {}

class B {
public:
  int v;
  B(int n) {v=n;cout<<"B redefine Constructor called"<<endl;}
  B(const B &b) {v=b.v;cout<<"B const Copy Constructor called"<<endl;}
};

B b(4);
B Func_1() {return b;}

int main(){
    cout<<"case1: use object to initialize the new class"<<endl;
    Complex cl(1, 2);
    Complex c2 (cl);
    cout<<c2.real<<","<<c2.imag<<endl;
    cout<<"case2: the parameters of a Func is a object"<<endl;
    A a;
    Func(a);
    cout<<"case3: the return of a Func is a object"<<endl;
    cout<<Func_1().v<<endl;
    return 0;
}



