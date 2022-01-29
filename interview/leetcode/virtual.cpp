#include<iostream>
using namespace std;

class Base0{
};

class Base1{
  virtual void func1(){}
  virtual void func11(){}
public:
  virtual ~Base1();
};

class Base2{
  virtual void func2(){}
};

class DerivedFromOne: public Base2
{
  virtual void func2() {}
  virtual void func22() {}
};

class DerivedFromTwo: public Base1, public Base2
{
  virtual void func3() {}
};

int main() {
  cout << "size of Base0 " << sizeof(Base0) << endl;
  cout << "size of Base1 " << sizeof(Base1) << endl;
  cout << "size of Base2 " << sizeof(Base2) << endl;
  cout << "size of DerivedFromOne " << sizeof(DerivedFromOne) << endl;
  cout << "size of DerivedFromTwo " << sizeof(DerivedFromTwo) << endl;
  return 0;
}
