#include<stdio.h>
#include<stdlib.h>
#define CONDITION(cond)     (__builtin_expect((cond)!=0, 0))

#define LOG_ALWAYS_FATAL_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? 1 \
    : 0 )

#define CTC_FATAL_IF(condition) LOG_ALWAYS_FATAL_IF(condition, "[%s][%d][FATAL:%s]", __FUNCTION__, __LINE__, #condition)

void * p1=NULL;
void cmp(void* point)
{
    printf("[%p %p]\n", p1, point);
    printf("%ld\n", CONDITION(p1==point));
    printf("%ld\n", CONDITION(p1!=point));
    printf("%d\n", LOG_ALWAYS_FATAL_IF(p1==point));
    printf("%d\n", LOG_ALWAYS_FATAL_IF(p1!=point));
    printf("%d\n", CTC_FATAL_IF(p1==point));
    printf("%d\n", CTC_FATAL_IF(p1!=point));
    if(p1==point)
        printf("same\n");
    if(p1!=point)
        printf("not same\n");
    return;
}
void setAddr()
{
    p1=malloc(sizeof(int));
}
void main()
{
    setAddr();
    cmp(p1);
    if(p1!=NULL)
        free(p1);
    return;
}
