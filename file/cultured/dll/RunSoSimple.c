/*
RunSoSimple.c
$ gcc -o RunSoSimple RunSoSimple.c -ldl
$ ./RunSoSimple /lib/x86_64-linux-gnu/libm-2.15.so
*/
#include<stdio.h>
#include<dlfcn.h>

int main(int argc, char* argv[])
{
    void* handle;
    double (*func)(double);
    char* error;
 
    handle=dlopen(argv[1],RTLD_NOW);
    if(handle == NULL)
    {
        printf("Open library %s error:%s\n",argv[1], dlerror());
        return -1;
    }
    func=dlsym(handle,"sin");
    if((error=dlerror()) != NULL)
    {
        printf("Symbol sin not found:%s\n",error);
        goto exit_runso;
    }
    printf("%f\n", func(3.1415926/2));
    exit_runso:
        dlclose(handle);
}
