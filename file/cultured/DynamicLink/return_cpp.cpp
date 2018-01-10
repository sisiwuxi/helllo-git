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
    cpp_obj& operator=(const cpp_obj& rhs)
    {
        cout << "operator=\n";
        return *this;
    }
    ~cpp_obj()
    {
        cout<<"dtor\n";
    }
};

cpp_obj return_test()
{
    cpp_obj b;
    cout<<"before return\n";
    return b;
}

int main()
{
    cpp_obj n;
    n = return_test();
}

