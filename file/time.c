#include <time.h>
#include <stdio.h>
#include <string.h>
#if 0
int main()
{
    time_t timer;
    struct tm *tblock;
    timer=time(0);
    tblock=localtime(&timer);
    printf("Local time is: %s",asctime(tblock));
    return 0;
}
#endif

int main() {
    struct tm t;
    char str[80];
    t.tm_sec=1;
    t.tm_min=3;
    t.tm_hour=7;
    t.tm_mday=22;
    t.tm_mon=11;
    t.tm_year=56;
    t.tm_wday=4;
    t.tm_yday=0;
    t.tm_isdst=0;
    strcpy(str,asctime(&t));
    printf("%s",str);
    return 0;
}
