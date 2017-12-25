#include<stdio.h>
#define LEN 8
const int MINNUM = -32767;
void main()
{
    int i,d[LEN]={90,5,7,44,6,2,11,56};
    int max=d[0];
    int sec_max=MINNUM;
    for(i=1;i<LEN;i++)
    {
        if(d[i] > max)
        {
            max=d[i];
            sec_max=max;
        }
        else
        {
            if(d[i] > sec_max)
                sec_max=d[i];
        }
    }
    printf("sec_max=%d\n",sec_max);
}
