#include <stdio.h>

#define M 50
#define TRUE 1
#define FALSE 0

typedef int StackElemType;

typedef struct
{
	StackElemType elem[STACK_SIZE];
	StackElemType top[2];//two stack's top
}DqStack;

void InitDqStack(DqStack *S)
{
	S->top[0] = -1;// -1, 0
	S->top[1] = M;// M-1, M
}

int PushDqStack(SeqStack *S, StackElemType x, int i)
{
	if(S->top[0]+1 == S->top[1]) {
		return FALSE;//stack is full
	}
	switch(i) {
		case 0:
			S->top[0]++;
			S->elem[S->top[0]] = x;
			break;
		case 1:
			S->top[1]--;
			S->elem[S->top[1]] = x;
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

int PopDqStack(SeqStack *S, StackElemType *x, int i)
{
	switch(i) {
		case 0:
			if(S->top[0] == -1) return FALSE;
			*x = S->elem[S->top[0]];
			S->top[0]--;
			break;
		case 1:
			if(S->top[1] == M) return FALSE;
			*x = S->elem[S->top[1]];			
			S->top[1]++;
			break;
		default:
			return FALSE;
	}
	return TRUE;
}



