#include <iostream>  
#include <cstdlib>  
using namespace std;  
int func1();  
//_onexit 使用方法   函数必须是 带有int类型返回值的无参数函数  
//_onexit 包含在cstdlib中  原始是c语言中的库函数  
int main(int argc,char * argv[])  
{  
    _onexit(func1);//无论函数放到main中哪个位置都是最后执行  
    cout<<"this is the first one"<<endl;  
      
    cout<<"this"<<endl;  
}  
  
int func1()  
{  
    cout<<"this executed at last time"<<endl;  
    return 0;  
}  
