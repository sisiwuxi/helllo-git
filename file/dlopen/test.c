#include "add.h"

#define NONE "\033[m"
#define BLUE "\033[0;32;34m"

int test(int num)
{
   add(num); //defined in add.c (libadd.so)
   printf(BLUE"[%s %s]"NONE": g_count=%d\n",__FILE__, __FUNCTION__, g_count); //[%s %s]以蓝色打印
   return 0;
}
