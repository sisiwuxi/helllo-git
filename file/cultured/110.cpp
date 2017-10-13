int func(int);
float func(float);

class C {
    int func(int);
    class C2 {
        int func(int);
    };
};

namespace N {
    int func(int);
    class C {
        int func(int);
    };
}


#include <Windows.h>
#include <Dbghelp.h>

int main( int argc, char* argv[] )
{
    char buffer[256];
    if(argc==2)
    {
        UnDecorateSymbolName(argv[1], buffer, 256, 0);
        printf(buffer);
    }
    else
    {
        printf("Usage: 2-4.exe DecroatedName\n");
    }
    return 0;
}

