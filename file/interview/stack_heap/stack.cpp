#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    char str1[] = "M";
    char str2[] = "M";
    const char str3[] = "M";
    const char str4[] = "M";
    char *str5 = "M";
    char *str6 = "M";
    const char *str7 = "M";
    const char *str8 = "M";
    cout << ( str1 == str2 ) << endl;
    cout << ( str3 == str4 ) << endl;
    cout << ( str5 == str6 ) << endl;
    cout << ( str7 == str8 ) << endl;
    cout << endl;
}

