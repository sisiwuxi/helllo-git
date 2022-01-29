#include <stdio.h>

#define TRUE 1
#define FALSE 0

typedef int StackElemType;

typedef struct node
{
	StackElemType data;
	struct node *next;
}LinkStackNode, *LinkStack;

int LinkStackPush(LinkStack top, StackElemType x)
{
	LinkStackNodet *temp;
	temp = (LinkStackNode*)malloc(sizeof(LinkStackNode));
	if(temp==NULL) return FALSE;
	temp->data = x;
	temp->next = top->next;//insert after top
	top->next = temp;
	return TRUE;
}

int LinkStackPush(LinkStack top, StackElemType *x)
{
	LinkStackNodet *temp;
	temp = top->next;
	if(temp==NULL) return FALSE;
	top->next = temp->next;
	*x = temp->data;
	free(temp);
	return TRUE;
}