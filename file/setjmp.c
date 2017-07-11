//这里setjmp返回的值为0,因此我用2作为longjmp的参数.这个让代码从setjmp位置返回并且返回值为2.值被打印出,并且程序退出
#include<setjmp.h>
main()
{
    jmp_buf env;
    int i;
    i=setjmp(env);
    printf("i=%d\n",i);
    if(i!=0)
        exit(0);
    longjmp(env,2);
    printf("does this line get printed?\n");
}
