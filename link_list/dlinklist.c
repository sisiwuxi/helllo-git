/*
have two pointer
(p->prior)->next = (p->next)->prior
*/

#include <stdio.h>
#include <malloc.h>

#define MAXSIZE 100
#define ERROR -1
#define OK 0

typedef int ElemType;

typedef struct DNode
{
    ElemType data;
	struct DNode *prior, *next;
}Dnode, *DoubleList;
/*
init
l->next=l
l->prior=l
*/
void InitDLinkList(DoubleList *DL)
{
	*DL=(DoubleList)malloc(sizeof(Dnode));
	(*DL)->prior=*DL;
	(*DL)->next=*DL;
}

int DListLength(DoubleList DL)
{
	if(NULL==DL) {
		return ERROR;
	}
	Dnode *p;
	int j;
	p=DL;
	j=0;
	while(p->next!=DL){		
		p=p->next;
		j++;
	}
	return j;
}
/*
Insert
s->prior=p->prior//left
p->prior->next=s//left
s->next=p//middle
p->prior=s//right
*/
int DlinkIns(DoubleList L, int i, ElemType e)
{
	Dnode *s, *p;
	int k=0;
	p=L;
	while((p->next != NULL) && (k<i)) {
		p=p->next;
		k++;
	}
	if(p->next=L && k!=i) {//larger than list length
		printf("location is illegal\n");
		return ERROR;
	}
	s=(Dnode*)malloc(sizeof(Dnode));
	if(s) {
		s->data = e;
		s->prior = p->prior;
		p->prior->next=s;
		s->next=p;
		p->prior=s;
		return OK;
	} else {
		return ERROR;
	}
}
/*
delete
p->prior->next=p->next
p->next->prior=p->prior
*/
int DlinkDel(DoubleList L, int i, ElemType *e)
{
	Dnode *p;
	int k=0;
	p=L;
	while((p->next!=NULL) && (k<i))
	{
		p=p->next;
		k++;
	}
	if(p->next==L && k<i) {//larger than list length
		return ERROR;
	} else {
		*e = p->data;
		p->prior->next = p->next;
		p->next->prior = p->prior;
		free(p);
		return OK;
	}
}

int main(int argc, char*argv[])
{
    DoubleList dl1;
	InitDLinkList(&dl1);
	printf("[init]dl1's length=%d\n", DListLength(dl1));
}
