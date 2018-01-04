/*
$ gcc -o runso runso.c -ldl
$ ./runso /lib/x86_64-linux-gnu/libm-2.15.so sin d2.0 d
5, fun: sin [4,8,8]
Segmentation fault (core dumped)

*/

#define SETUP_STACK	\
i=2;			\
while(++i<(argc-1)){	\
    switch(argv[i][0]){	\
        case 'i':	\
            asm volatile("push %0" ::	\
            "r"(&argv[i][1]));	\
            esp+=4;	\
            break;	\
        case 'd':	\
            atof(&argv[i][1]);	\
            asm volatile("subl $8, %esp\n" \
            "fstpl (%esp)");	\
            esp+=8;	\
            break;	\
        case 's':	\
            asm volatile("push %0" ::	\
            "r"(&argv[i][1]));	\
            esp+=8;	\
            break;	\
        default:	\
            printf("error argument type");	\
            goto exit_runso;	\
    }	\
}

#define RESTORE_STACK	\
    asm volatile("add %0, %%esp" :: "r"(esp))

#include<stdio.h>
#include<dlfcn.h>

int main(int argc, char* argv[])
{
    void* handle;
    char* error;
    int i;
    int esp=0;
    void* func;

    handle=dlopen(argv[1], RTLD_NOW);
    if(handle == 0)
    {
        printf("can't find library: %s\n", argv[1]);
        return -1; 
    }
    printf("%d, fun: %s [%lx,%lx,%lx]\n", argc, argv[2], sizeof(int), sizeof(double), sizeof(char *));
    func = dlsym(handle,argv[2]);
    if((error=dlerror())!=NULL)
    {
        printf("find symbol %s error:%s\n", argv[2],error);
        goto exit_runso;
    }
    switch(argv[argc-1][0])
    {
        case 'i':
        {
            int (*func_int)()=func;
            SETUP_STACK;
            int ret=func_int();
            RESTORE_STACK;
            printf("ret=%d\n",ret);
            break;
        }
        case 'd':
        {
            double (*func_double)()=func;
            SETUP_STACK;
            double ret=func_double();
            RESTORE_STACK;
            printf("ret=%f\n",ret);
            break;
        }
        case 's':
        {
            char* (*func_str)()=func;
            SETUP_STACK;
            char* ret=func_str();
            RESTORE_STACK;
            printf("ret=%s\n",ret);
            break;
        }
        case 'v':
        {
            double (*func_void)()=func;
            SETUP_STACK;
            func_void();
            RESTORE_STACK;
            printf("ret=void\n");
            break;
        }
    }//end of switch
    exit_runso:
    dlclose(handle);
}
