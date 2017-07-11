#include <iostream>
using namespace std;

class Hello
{
    public:
        Hello()
        {
            cout << "Hello constructor" << endl;
        }
        ~Hello()
        {
            cout << "Hello destructor" << endl;
        }
};
Hello hello;
int main()
{
    cout << "main" << endl;
    return 0;
}
