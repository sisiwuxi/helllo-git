#include "add.h"

int g_count;

int add(int num)
{
     g_count +=num;
     printf("[%s %s]: g_count=%d\n",__FILE__, __FUNCTION__, g_count);
     return 0;
}
