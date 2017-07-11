#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    char str1[] = "MStar";
    char str2[] = "MStar";
    const char str3[] = "MStar";
    const char str4[] = "MStar";
    char *str5 = "MStar";
    char *str6 = "MStar";
    const char *str7 = "MStar";
    const char *str8 = "MStar";
    cout << ( str1 == str2 ) << endl;
    cout << ( str3 == str4 ) << endl;
    cout << ( str5 == str6 ) << endl;
    cout << ( str7 == str8 ) << endl;
    cout << endl;
}

