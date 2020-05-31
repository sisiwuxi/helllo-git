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
	if(NULL==L) {
		return ERROR;
	}
	Node *p;
	int j;
	p=L->next;//the first pointer
	j=0;//count
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
		return ERROR;	
	}	
}

int InsList(LinkList L, int i, ElemType e)
{
	Node *pre, *s;//s is new pointer
	int k=0;
	pre=L;//first pointer
	while((pre!=NULL) && (k<(i-1))){
		pre=pre->next;
		k++;
	}
	if(NULL==pre){
	    printf("[%s] i=%d is larger than L's length %d\n", __func__, i, k);
		return ERROR;
	} else {
		s=(Node*)malloc(sizeof(Node));
		s->data=e;
		s->next=pre->next;
		pre->next=s;
		return OK;
	}
}


int DelList(LinkList L, int i, ElemType *e)
{
	Node *pre, *r;//r is pointer will be delected. pre is the pointer before r. 
	int k=0;
	pre=L;//first pointer
	while((pre->next!=NULL) && (k<(i-1))){
		pre=pre->next;
		k++;
	}
	if(NULL==pre){
	    printf("[%s] i=%d is larger than L's length %d\n", __func__, i, k);
		return ERROR;
	} else {
		r=pre->next;
		pre->next=pre->next->next;
		*e = r->data;
		free(r);
		return OK;
	}
}

LinkList MergeLinkList(LinkList LA, LinkList LB)
{
	Node *pa, *pb;
	Node *r;
	LinkList LC;
	pa=LA->next;//the first pointer of A
	pb=LB->next;//the first pointer of B
	LC=LA;//use LA's address
	LC->next=NULL;
	r=LC;//r is LC's last pointer
	while(pa!=NULL && pb!=NULL) {
		if(pa->data <= pb->data) {
			r->next = pa;//insert pa to r
			r=r->next;//r=pa; r move to next
			pa=pa->next;//pa move to next
		} else {
			r->next = pb;
			r=r->next;//r=pb;
			pb=pb->next;
		}	
	}
	if(pa) {
		r->next=pa;
	} else {
		r->next=pb;
	}
	free(pb);
	return LC;
}

int main(int argc, char*argv[])
{
    LinkList p1, p2;
	InitLinkList(&p1);
	printf("[init]p1's length=%d\n", ListLength(p1));	
	CreateFromHead(p1);
	printf("[CreateFromHead]p1's length=%d\n", ListLength(p1));		
	InitLinkList(&p2);
	printf("[init]p2's length=%d\n", ListLength(p1));	
	CreateFromTail(p2);
	printf("p2's length=%d\n", ListLength(p2));
	Node* p3=GetNode(p1, 2);
	if(p3!=NULL) {
		printf("p1's second node=%d\n", p3->data);
	}
	int loc=Locate(p2, 52);
	printf("52's loc in p2=%d\n", loc);
	
	printf("\n=============insert=============\n");	
	printf("p1's original length=%d\n", ListLength(p1));	
	InsList(p1, 2, 8);
	printf("p1's length=%d\n", ListLength(p1));	
	
	printf("\n=============delete=============\n");	
	printf("p1's original length=%d\n", ListLength(p1));
	ElemType e=0;
	DelList(p1, 4, &e);
	printf("del %d, p1's length=%d\n", e, ListLength(p1));	
	
	printf("\n=============merge=============\n");	
	printf("p1's original length=%d\n", ListLength(p1));
	printf("p2's original length=%d\n", ListLength(p2));
	LinkList p4;
	p4 = MergeLinkList(p1,p2);
	printf("merged p4's length=%d\n", ListLength(p4));		
}

