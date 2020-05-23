#include <stdio.h>
#include <malloc.h>

#define MAXSIZE 100
#define ERROR -1
#define OK 0

typedef int ElemType;

typedef struct
{
    ElemType elem[MAXSIZE];
    int last;//last_index
}SeqList;


SeqList *InitList()
{
    SeqList *p;
    p = (SeqList *)malloc(sizeof(SeqList));
    p->last = -1;
    return p;
}

int Locate(SeqList *L, ElemType e)
{
    int i;
    for(i=0; i<=L->last; i++) {
        if(L->elem[i] == e) {
            return i+1;
        }
    }    
    return ERROR;
}

ElemType GetData(SeqList *L, int i)
{
    if((i>=1) && (i<=L->last+1)) {
        return L->elem[i-1];
    } else {
        printf("%d is overflow\n", i);
        return ERROR;
    }
}

int InsList(SeqList *L, int i, ElemType e)
{
    int k;
    if((i<1) || (i>L->last + 2)) {
        printf("i is illegal\n");
        return ERROR;
    }
    if(L->last >= (MAXSIZE-1)) {
        printf("list is full\n");
        return ERROR;
    }
    if(L->last > -1) {
        for(k=L->last; k>=(i-1); k--) {
            L->elem[k+1] = L->elem[k];
        }
    }
    L->elem[i-1] = e;
    L->last++;
    return OK;
}

int DelList(SeqList *L, int i, ElemType *e)
{
    int k;
    if((i<1) || (i>L->last + 1)) {
        printf("i is illegal\n");
        return ERROR;
    }
    *e = L->elem[i-1];
    for(k=i; k<=L->last; k++) {
        L->elem[k-1] = L->elem[k];
    }
    L->last--;
    return OK;
}

int main(int argc, char*argv[])
{
    SeqList *L = InitList();
    printf("%d\n", L->last);
    int locate = Locate(L, 2);
    printf("locate=%d\n", locate);
    ElemType et = GetData(L, 1);
    printf("et=%d\n", et);
    InsList(L, 1, 6);
    printf("Insert 0:[%d,%d]\n", L->last, L->elem[0]);
    InsList(L, 1, 7);
    printf("Insert 1:[%d,%d]\n", L->last, L->elem[0]);
    InsList(L, 1, 67);
    printf("Insert 2:[%d,%d]\n", L->last, L->elem[0]);
    ElemType del_elem = 0;
    DelList(L, 2, &del_elem);
    printf("Del 2:[%d,%d]\n", L->last, del_elem);
    return 0;
}

