//参见《数据结构（C语言版）》严蔚敏 吴伟民 第19页
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFESIBLE -1
#define OVERFLOW -2

typedef int Status;
typedef struct Node {
    int data;
    struct Node *next;
} LNode,*LinkList;


LinkList InitList();
void DestroyList(LinkList L);
LinkList Clearlist(LinkList L);
Status ListEmpty(LinkList L);
int ListLength(LinkList L);
LNode GetElem(LinkList L,int i);
Status Compare(LinkList p,LNode e);
int LocatElem(LinkList L,LNode e);
LinkList PriorElem(LinkList L,LNode e);
LinkList NextElem(LinkList L,LNode e);
LinkList ListInsert(LinkList L,int locate,LNode e);
LinkList InsertHead(LinkList L,LNode e);
LinkList InsertTail(LinkList L,LNode e);
void Vist(LinkList p);
void ListTraverse(LinkList L);

LinkList InitList()
{
    LinkList L;
    L=(LinkList)malloc(sizeof(LNode));
    if(L==NULL)
    {
        printf("Creat L point fail!\n");
        exit(1);
    }
    L->data=0;
    L->next=NULL;
    return L;
}
void DestroyList(LinkList L)
{
    LinkList p;
    for(p=L->next; p!=NULL; p=p->next)
    {
        L->next=NULL;
        free(L);
        L=p;
    }
}
LinkList Clearlist(LinkList L)
{
    LinkList p;
    for(p=L->next; p!=NULL; p=p->next)
        p->data=0;
    return L;
}
Status ListEmpty(LinkList L)
{
    Status status;
    if(L->data==0)
        status=TRUE;
    else
        status=FALSE;
    return status;
}
int ListLength(LinkList L)
{
    return L->data;
}
LNode GetElem(LinkList L,int i)
{
    LNode s;
    LinkList p;
    int j;
    if(i<1||i>ListLength(L))
    {
        printf("%d don't exit in the lenght.\n",i);
        exit(1);
    }
    else
    {
        for(j=1,p=L->next; j<i; p=p->next,j++)
            ;
        s.data=p->data;
    }
    return s;
}
Status Compare(LinkList p,LNode e)
{
    Status status;
    if(p->data==e.data)
        status=TRUE;
    else
        status=FALSE;
    return status;
}
int LocatElem(LinkList L,LNode e)
{
    Status s;
    LinkList p;
    int locate;
    int i;
    for(i=0,p=L; i<ListLength(L); i++,p=p->next)
    {
        if((s=Compare(p,e))==FALSE)
            continue;
        else
        {
            locate=i;
            break;
        }
    }
    if(i>=ListLength(L))
    {
        printf("%d don't exit in the LinkList.\n",e.data);
        exit(1);
    }
    return locate;
}
LinkList PriorElem(LinkList L,LNode e)
{
    LinkList p=NULL;
    int locate;
    int i;
    locate=LocatElem(L,e);
    if(locate==0||locate==1)
    {
        /*printf("PriorElem:locate=%d",locate);*/
        p=L->next;
    }
    else
    {
        for(i=1,p=L->next; i<locate-1; p=p->next,i++)
            ;
    }
    return p;
}
LinkList NextElem(LinkList L,LNode e)
{
    LinkList p=NULL;
    int locate;
    int i;
    locate=LocatElem(L,e);
    if(locate>=ListLength(L))
    {
        /*p=NULL;
        exit(1);*/
        for(i=1,p=L->next; i<ListLength(L); p=p->next,i++)
            ;
    }
    else
    {
        for(i=1,p=L->next; i<locate; p=p->next,i++)
            ;
        p=p->next;
    }
    return p;
}
LinkList ListInsert(LinkList L,int locate,LNode e)
{
    LinkList p,pre;
    p=(LinkList)malloc(sizeof(LNode));
    if(p==NULL)
    {
        printf("Creat p Node fail!\n");
        exit(1);
    }/*printf("GetElem(L,locate)=%d",GetElem(L,locate).data);*/
    pre=PriorElem(L,GetElem(L,locate));
    printf("%d",pre->data);
    if(pre==NULL)
    {
        printf("pre return Error!");
        exit(1);
    }
    p->data=e.data;
    p->next=pre->next;
    pre->next=p;
    L->data+=1;
    return L;
}
LinkList InsertHead(LinkList L,LNode e)
{
    LinkList p;
    p=(LinkList)malloc(sizeof(LNode));
    if(p==NULL)
    {
        printf("Creat Node fail!\n");
        exit(1);
    }
    p->data=e.data;
    p->next=L->next;
    L->next=p;
    p=NULL;
    L->data+=1;
    return L;
}
LinkList InsertTail(LinkList L,LNode e)
{
    LinkList p,tail;
    p=(LinkList)malloc(sizeof(LNode));
    if(p==NULL)
    {
        printf("Creat Node fail!\n");
        exit(1);
    }
    p->data=e.data;
    p->next=NULL;
    for(tail=L; tail->next; tail=tail->next)
        ;
    tail->next=p;
    L->data+=1;
    return L;
}
LinkList ListDelete(LinkList L,int locate,LNode e)
{
    LinkList p,pre;
    pre=PriorElem(L,GetElem(L,locate));
    if(pre==NULL)
    {
        printf("ListDelete:pre==NULL\n");
        exit(1);
    }
    p=pre->next;
    pre->next=p->next;
    e.data=p->data;
    printf("%d has been delete\n",e.data);
    free(p);
    L->data-=1;
    return L;
}
void Vist(LinkList p)
{
    printf("The Elem is %d\n",p->data);
}
void ListTraverse(LinkList L)
{
    LinkList p;
    int i;
    for(i=1,p=L->next; i<=ListLength(L); i++,p=p->next)
        Vist(p);
}

int main()
{
    LinkList head/*,tail*/;
    LinkList p/*,pre,pTemp*/;
    /*LinkList headOld;*/
    LNode e,test;
    char ch;
    int flag=1,i;
    head=InitList();
    printf("Input data(quit for 'q'):");
    scanf("%d",&e.data);
    {
        while(flag==1)
        {
            if((ch=getchar())=='q')
                flag=0;
            else
            {
                head=InsertTail(head,e);
                puts("save successful");
            }
            if(flag!=0)
            {
                printf("Input data:");
                scanf("%d",&e.data);
            }
            else
                break;
        }
    }
    puts("the List is:");
    ListTraverse(head);
    printf("List length is%d\n",ListLength(head));
    printf("input i=");
    scanf("%d",&i);
    printf("%d",GetElem(head,i).data);
    printf("\ntest.data=");
    scanf("%d",&test.data);
    printf("locate=%d\n",LocatElem(head,test));
    /*printf("delete i=");
    scanf("%d",&i);
    head=ListDelete(head,i,test);
    ListTraverse(head);*/
    printf("List length is %d\n",ListLength(head));
    printf("input insert data:");
    scanf("%d",&test.data);
    head=ListInsert(head,5,test);
    ListTraverse(head);
    p=PriorElem(head,test);
    printf("\npriodata is:%d",p->data);
    p=NextElem(head,test);
    printf("\nnextdata is:%d",p->data);
    puts("\nthe List is:");
    ListTraverse(head);
    printf("List length is%d\n",ListLength(head));
    DestroyList(head);
    return 0;
}
