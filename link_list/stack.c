#include <stdio.h>

#define STACK_SIZE 50
#define TRUE 1
#define FALSE 0

typedef int StackElemType;

typedef struct
{
	StackElemType elem[STACK_SIZE];
	int top;//stack's top
}SeqStack;

void InitStack(SeqStack *S)
{
	S->top = -1;//init a NULL stack
}

int Push(SeqStack *S, StackElemType x)
{
	if(S->top == (STACK_SIZE-1)) {
		return FALSE;//stack is full
	}
	S->top++;//point a new location
	S->elem[S->top] = x;
	return TRUE;
}

int Pop(SeqStack *S, StackElemType *x)
{
	if(S->top == -1) {
		return FALSE;//stack is NULL
	} else {
		*x = S->elem[S->top];
		S->top--;
		return TRUE;
	}
}

//int GetTop(SeqStack S, StackElemType *x)
int GetTop(SeqStack *S, StackElemType *x)
{
	if(S->top == -1) {
		return FALSE;//stack is NULL
	} else {
		*x = S->elem[S->top];//read the value of top, not pop
		return TRUE;
	}
}

int IsEmpty(SeqStack *S)
{
	return ((S->top == -1) ? TRUE : FALSE);
}


