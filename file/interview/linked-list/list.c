/*
head->1->2->3->4->5->6->tail
*/

struct Node
{   
    int data;  
    Node *next;
};
typedef struct Node Node;

Node *ReverseList(Node *head)
{
    if(head == NULL && head->next == NULL)
        return head;
    Node *p1=head;
    Node *p2=p1->next;
    Node *p3=p2->next;
    p1->next=NULL;
    while(p3!=NULL)
    {
         p2->next=p1;
         p1=p2;
         p2=p3;
         p3=p3->next;
    }   
    p2->next=p1;
    head=p2;
    return head;
}

Node Merge(Node *head1, Node *head2)
{
    if(head1 == NULL)
        return head2;
    if(head2 == NULL)
        return head1;
    Node *head=NULL,*p1=NULL,*p2=NULL;
    if(head1->data < head2->data)
    {
        head=head1;
        p1=head1->next;
        p2=head2;
    }
    else
    {
        head=head2;
        p1=head2->next;
        p2=head1;
    }
    Node *pcur=head;
    while(p1!=NULL && p2!=NULL)
    {
        if(p1->data<=p2->data)
        {
            pcur->next = p1;
            pcur=p1;
            p1=p1->next;
        }
        else
        {
            pcur->next = p2;
            pcur=p2;
            p2=p2->next;
        }
    }
    if(p1!=NULL)
        pcur->next=p1;
    if(p2!=NULL)
        pcur->next=p2;
    return head;
}

Node *MergeRecursive(Node *head1,Node *head2)
{
    if(head1=NULL)
        return head2;
    if(head2=NULL)
        return head1;
    Node *head=NULL;
    if(head1->data < head2->data)
    {
        head=head1;
        head->next = MergeRecursive(head1->next,head2);
    }
    else
    {
        head=head2;
        head->next = MergeRecursive(head1,head2->next);
    }
    return head;
}

bool check(const Node* head)
{
    if(head==NULL)
        return false;
    Node *low=head;
    Node *high=head->next;
    while(high!=NULL && high->next!=NULL)
    {
        low=low->next;
        high=high->next->next;
        if(low==high)
            return true;
    }
    return false;
}
