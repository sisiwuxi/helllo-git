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
}Node, *LinkList;

void InitLinkList(LinkList *L)
{
	*L=(LinkList)malloc(sizeof(Node));//first pointer
	(*L)->next=NULL;
}

void CreateFromHead(LinkList L)
{
    Node *s;
	char c;
	int flag=1;
	while(flag){
	    c=getchar();
		if(c!='$'){
		    s=(Node*)malloc(sizeof(Node));
			s->data=c;
			s->next=L->next;
			L->next=s;
		} else {
		    flag=0;
		}
	}
}

void CreateFromTail(LinkList L)
{
	Node *r, *s;//r is tail pointer, s is new pointer
	char c;
	int flag=1;
	r=L;//when L is NULL, tail_pointer = L
	while(flag) {
		c=getchar();
		if(c!='$') {
			s=(Node*)malloc(sizeof(Node));
			s->data=c;
			r->next=s;
			r=s;
		} else {
		    flag=0;
			r->next=NULL;
		}
	}
}

Node * GetNode(LinkList L, int i)
{
	int j;
	Node *p;
	p=L;
	j=0;
	while((p->next != NULL) && (j<i)) {
		p=p->next;
		j++;
	}
	if(i == j){
		return p;
	} else {
		printf("%d larger than the linklist's length\n", i);
		return NULL;
	}
}

int ListLength(LinkList L)
{
	Node *p;
	int j;
	p=L->next;
	j=0;
	while(p!=NULL){
		p=p->next;
		j++;
	}
	return j;
}

//Node *Locate(LinkLst L, ElemType key)
int Locate(LinkList L, ElemType key)
{
	Node *p;
	p=L->next;//the first pointer
	int i=0;
	while(p!=NULL){
		if(p->data != key) {
			p=p->next;
			i++;
		} else {
			break;
		}
	}
	//return p;
	if(i < ListLength(L)) {
		printf("[%s] key=%d in the %d\n", __func__, key, i);
		return i;
	} else {
		printf("[%s] key=%d not in the linklist\n", __func__, key);
		return -1;	
	}	
}

int main(int argc, char*argv[])
{
    LinkList p1, p2;
	InitLinkList(&p1);
	CreateFromHead(p1);
	printf("p1's length=%d\n", ListLength(p1));	
	InitLinkList(&p2);
	CreateFromTail(p2);
	printf("p2's length=%d\n", ListLength(p2));
	Node* p3=GetNode(p1, 2);
	if(p3!=NULL) {
		printf("p1's second node=%d\n", p3->data);
	}
	int loc=Locate(p2, 52);
	printf("52's loc in p2=%d\n", loc);
}

