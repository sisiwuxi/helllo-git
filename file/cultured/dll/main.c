#include <dlfcn.h>
#include <stdio.h>

#define NONE "\033[m"
#define RED "\033[0;32;31m"

extern int g_count;

int main(int argc, char **argv)
{
        void* pdlhandle;
        char* pszerror;

        int (*mytest)(int num);

        pdlhandle = dlopen("./libtest.so", RTLD_LAZY);
        pszerror = dlerror();
        if (0 != pszerror)
        {
            printf("%s", pszerror);
            return 1;
        }

        mytest = dlsym(pdlhandle, "test");
        pszerror = dlerror();
        if (0 != pszerror)
        {
            printf("%s", pszerror);
            return 1;
        }

        mytest(3);   //defined in test.c (libtest.c)
        printf(RED"[%s %s]"NONE": g_count=%d\n",__FILE__, __FUNCTION__, g_count); //[%s %s]以红色打印

        dlclose(pdlhandle);
        return 0;
}
