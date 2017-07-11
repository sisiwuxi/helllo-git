#include <stdio.h>
#include <stdlib.h>
//#include "DbLinkList.h"

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H
typedef struct Node
{
    int data;
    struct Node *pNext;
    struct Node *pPre;
} NODE, *pNODE;

//创建双向链表
pNODE CreateDbLinkList(void);
//打印链表
void TraverseDbLinkList(pNODE pHead);
//判断链表是否为空
int IsEmptyDbLinkList(pNODE pHead);
//计算链表长度
int GetLengthDbLinkList(pNODE pHead);
//向链表插入节点
int InsertEleDbLinkList(pNODE pHead, int pos, int data);
//从链表删除节点
int DeleteEleDbLinkList(pNODE pHead, int pos);
//删除整个链表，释放内存
void FreeMemory(pNODE *ppHead);
#endif

//创建双向链表
pNODE CreateDbLinkList(void)
{
    int i, length = 0, data = 0;
    pNODE pTail = NULL, p_new = NULL;
    pNODE pHead = (pNODE)malloc(sizeof(NODE));

    if (NULL == pHead)
    {
        printf("内存分配失败！\n");
        exit(EXIT_FAILURE);
    }

    pHead->data = 0;
    pHead->pPre = NULL;
    pHead->pNext = NULL;
    pTail = pHead;

    printf("请输入想要创建链表的长度：");
    scanf("%d", &length);

    for (i=1; i<length+1; i++)
    {
        p_new = (pNODE)malloc(sizeof(NODE));

        if (NULL == p_new)
        {
            printf("内存分配失败！\n");
            exit(EXIT_FAILURE);
        }

        printf("请输入第%d个元素的值：", i);
        scanf("%d", &data);

        p_new->data = data;
        p_new->pNext = NULL;
        p_new->pPre = pTail;
        pTail->pNext = p_new;
        pTail = p_new;
    }

    return pHead;
}

//（2）这部分是获得双向链表的信息，这里和单向链表基本一致，因为遍历的时候只用到了一个指针。
//打印链表
void TraverseDbLinkList(pNODE pHead)
{
    pNODE pt = pHead->pNext;

    printf("打印链表如：");
    while (pt != NULL)
    {
        printf("%d ", pt->data);
        pt = pt->pNext;
    }
    putchar('\n');
}

//判断链表是否为空
int IsEmptyDbLinkList(pNODE pHead)
{
    pNODE pt = pHead->pNext;

    if (pt == NULL)
        return 1;
    else
        return 0;
}

//计算链表的长度
int GetLengthDbLinkList(pNODE pHead)
{
    int length = 0;
    pNODE pt = pHead->pNext;

    while (pt != NULL)
    {
        length++;
        pt = pt->pNext;
    }
    return length;
}

//（3）这部分是向双向链表插入节点，也跟单向链表很多相似的地方。我们先来看下插入节点时的示意图：
//从图中可以看到，每次我们添加一个节点都有很多地方要调节的，也就是每个节点的那两个指针，一定要认真仔细自己动手写一遍，有可能有些细节就会出错。这里有一个地方需要注意，是和单向链表不同的地方，单向链表在插入节点的时候不需要判断最后一个节点是否为空，因为这不影响程序的结果，但是对于双向链表就不一样了，因为我们后面要用到最后一个节点的一个指针指向前一个节点，如果最后一个节点是空的话（就是程序中的pt），就不存在pt->pPre了,那么程序运行到这里时就会报错，所以我们要加个判断，判断此时节点是NULL的话就不需要控制它的指针了。
//向双向链表中插入节点
int InsertEleDbLinkList(pNODE pHead, int pos, int data)
{
    pNODE pt = NULL, p_new = NULL;

    if (pos > 0 && pos < GetLengthDbLinkList(pHead)+2)
    {
        p_new = (pNODE)malloc(sizeof(NODE));

        if (NULL == p_new)
        {
            printf("内存分配失败！\n");
            exit(EXIT_FAILURE);
        }

        while (1)
        {
            pos--;
            if (0 == pos)
                break;
            pHead = pHead->pNext;
        }

        pt = pHead->pNext;
        p_new->data = data;
        p_new->pNext = pt;
        if (NULL != pt)
            pt->pPre = p_new;
        p_new->pPre = pHead;
        pHead->pNext = p_new;

        return 1;
    }
    else
        return 0;
}

//（4）这部分是从链表中删除节点，当然这里和单向链表差不多，要注意的地方和插入节点时是一样的，上面已经说明了。
//从链表中删除节点
int DeleteEleDbLinkList(pNODE pHead, int pos)
{
    pNODE pt = NULL;

    if (pos > 0 && pos < GetLengthDbLinkList(pHead) + 1)
    {
        while (1)
        {
            pos--;
            if (0 == pos)
                break;
            pHead = pHead->pNext;
        }

        pt = pHead->pNext->pNext;
        free(pHead->pNext);
        pHead->pNext = pt;
        if (NULL != pt)
            pt->pPre = pHead;

        return 1;
    }
    else
        return 0;
}

//（5）这部分是用来释放内存的，注意的地方和上面一样。
//删除整个链表，释放内存
void FreeMemory(pNODE *ppHead)
{
    pNODE pt = NULL;

    while (*ppHead != NULL)
    {
        pt = (*ppHead)->pNext;
        free(*ppHead);
        if (NULL != pt)
            pt->pPre = NULL;
        *ppHead = pt;
    }
}

//main.cpp 测试程序源文件——通过简单的交互信息来测试各个函数功能是否正确。
int main(void)
{
    int flag = 0, length = 0;
    int position = 0, value = 0;
    pNODE head = NULL;

    head = CreateDbLinkList();

    flag = IsEmptyDbLinkList(head);
    if (flag)
        printf("双向链表为空！\n");
    else
    {
        length = GetLengthDbLinkList(head);
        printf("双向链表的长度为：%d\n", length);
        TraverseDbLinkList(head);
    }

    printf("请输入要插入节点的位置和元素值(两个数用空格隔开)：");
    scanf("%d %d", &position, &value);
    flag = InsertEleDbLinkList(head, position, value);
    if (flag)
    {
        printf("插入节点成功！\n");
        TraverseDbLinkList(head);
    }
    else
        printf("插入节点失败！\n");

    flag = IsEmptyDbLinkList(head);
    if (flag)
        printf("双向链表为空，不能进行删除操作！\n");
    else
    {
        printf("请输入要删除节点的位置：");
        scanf("%d", &position);
        flag = DeleteEleDbLinkList(head, position);
        if (flag)
        {
            printf("删除节点成功！\n");
            TraverseDbLinkList(head);
        }
        else
            printf("删除节点失败！\n");
    }

    FreeMemory(&head);
    if (NULL == head)
        printf("已成功删除双向链表，释放内存完成！\n");
    else
        printf("删除双向链表失败，释放内存未完成！\n");

    return 0;
}
