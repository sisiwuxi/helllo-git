#include<stdio.h>
#define CAP_TO_INDEX(x)     ((x) >> 5)        /* 1 << 5 == bits in __u32 */
#define CAP_TO_MASK(x)      (1 << ((x) & 31)) /* mask for indexed __u32 */
int main(int argc, char *argv[])
{
    int value=0xdc919cd0dc919cec;
    int ret = value?0:-1;
    int cap = 17;
    if(value != 0)
    {
        printf("\n0x%llx !=0 \n");
    }
    else
    {
        printf("\n0x%llx ==0 \n");
    }
    printf("\nret =%d 0x%x\n", ret, ret);
    printf("\n %d %d\n",  CAP_TO_INDEX(cap), CAP_TO_MASK(cap));
    printf("\n 0x%llx 0x%llx\n",  CAP_TO_INDEX(cap), CAP_TO_MASK(cap));
    return 0;
}

