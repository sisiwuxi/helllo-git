#include<iostream>
using namespace std;
struct cpp_obj
{
    cpp_obj()
    {
        cout << "ctor\n";
    }
    cpp_obj(const cpp_obj& c)
    {
        cout << "copy ctor\n";
    }
    cpp_obj& operator=(const cpp_obj&rhs)
    {
        cout << "copy ctor\n";
    }
}
