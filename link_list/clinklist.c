#include <stdio.h>
#include <malloc.h>

#define MAXSIZE 100
#define ERROR -1
#define OK 0

typedef int ElemType;

typedef struct Node
{
	ElemType data;
	struct Node *next;
}Node;
//}Node, *LinkList;

typedef struct Node *LinkList;

void InitCLinkList(LinkList *CL)
{
	*CL=(LinkList)malloc(sizeof(Node));
	(*CL)->next=*CL;
}

void CreateCLinkList(LinkList CL)
{
	Node *rear, *s;
	char c;
	rear=CL;
	c=getchar();
	while(c!='$') {
		s=(Node*)malloc(sizeof(Node));
		s->data=c;
		rear->next=s;
		rear=s;//rear=rear->next;
		c=getchar();
	}
	rear->next=CL;
}

int CListLength(LinkList CL)
{
	if(NULL==CL) {
		return ERROR;
	}
	Node *p;
	int j;
	p=CL;//->next;//the head pointer
	j=0;//count
	//while(p->next!=CL->next){
	while(p->next!=CL){		
		p=p->next;
		j++;
	}
	return j;
}

/*LC=LA,LB*/
LinkList merge_H(LinkList LA, LinkList LB)
{
	Node *p, *q;
	p=LA;
	q=LB;
	while(p->next != LA) {
		p=p->next;
	}//p goto LA rear
	while(q->next != LB) {
		q=q->next;
	}//q goto LB rear
	p->next=LB->next;//LA'rear'next=LB_first
	q->next=LA;//LB'rear'rear=LA_head
	free(LB);
	return LA;
}

//RA/RB has rear point
LinkList merge_R(LinkList RA, LinkList RB)
{
    Node *p;
	p=RA->next;//p is RA's head
	RA->next = RB->next->next; //RA'rear point RB's first
	free(RB->next);//free RA'head
	RB->next=p;
	return PB;
}

int main(int argc, char*argv[])
{
    LinkList cl1, cl2;
	InitCLinkList(&cl1);
	printf("[init]cl1's length=%d\n", CListLength(cl1));	
	CreateCLinkList(cl1);
	printf("[init]cl1's length=%d\n", CListLength(cl1));	
}
