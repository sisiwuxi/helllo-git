#include<stdio.h>
#include<iostream>
using namespace std;

// dynamic override template, Runtime polymorphism
// class Base {
// public:
//   virtual void foo() = 0;
//   virtual void bar() = 0;
// };

// class Derived1 : public Base {
// public:
//   virtual void foo() override final { cout << "Derived1 foo" << endl; }
//   virtual void bar() override final { cout << "Derived1 bar" << endl; }
// };

// class Derived2 : public Base {
// public:
//   virtual void foo() override final { cout << "Derived2 foo" << endl; }
//   virtual void bar() override final { cout << "Derived2 bar" << endl; }
// };

// Curiously Recurring Template Pattern
template<typename T> 
class Base {
public:
  void foo() { static_cast<T *>(this)->internal_foo(); }
  void bar() { static_cast<T *>(this)->internal_bar(); }
  // void foo(const T& t) { ... }
  // void bar(const T& t) { ... }
  static int getObjCnt() { return cnt; }
protected:
  static int cnt;
};

class Derived1 : public Base<Derived1> {
public:
  void internal_foo() { cout << "Derived1 foo" << endl; }
  void internal_bar() { cout << "Derived1 bar" << endl; }
  Derived1() { cnt++; }
};

class Derived2 : public Base<Derived2> {
public:
  void internal_foo() { cout << "Derived2 foo" << endl; }
  void internal_bar() { cout << "Derived2 bar" << endl; }
  Derived2() { cnt++; }
};

template <typename T> void foo(Base<T> &obj) { obj.foo(); }
template <typename T> void bar(Base<T> &obj) { obj.bar(); }
template <typename T> int Base<T>::cnt = 0;

int main(int argc, char** argv) {
  Derived1 d1;
  Derived2 d2;
  foo(d1);
  foo(d2);
  bar(d1);
  bar(d2);
  Derived1 d11, d12, d13;
  Derived2 d21, d22;
  cout << "Derived1::getObjCnt() = " << Derived1::getObjCnt() << endl;
  cout << "Derived2::getObjCnt() = " << Derived2::getObjCnt() << endl;
  return 0;
}