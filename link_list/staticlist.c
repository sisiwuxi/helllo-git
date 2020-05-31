/*
No pointer
*/

#include <stdio.h>
#include <malloc.h>

#define MAXSIZE 100
#define ERROR -1
#define OK 0
#define MaxSize 100

typedef int ElemType;

typedef struct
{
	ElemType data;
	int cursor;
}Component, StaticList[MaxSize];

void initial(StaticList space, int *av)//av is null head pointer
{
	int k;
	space[0].cursor=-1;
	for(k=1;j<MaxSize-1;k++) {
		space[k].cursor = k+1;
	}
	space[MaxSize-1].cursor=-1;
	*av=1;
}

int getnode(StaticList space, int *av)
{
	int i;
	j=*av;
	*av=space[*av].cursor;
	return i;
}

void freenode(StaticList space, int *av, int k)
{
	space[k].cursor = *av;
	*av = k;
}