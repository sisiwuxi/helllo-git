//============heap_stack==============//
// code area
// static area: global & static initialized + uninitialized
// heap: OS, malloc/free, new/delete, low->high
// stack: compiler, locality variables, high->low


#include<malloc.h>
#include<string.h>
int a=0;    //全局初始化区 &a = (int *) 0x601044 <a>
const int ac=0;
char *p1;   //全局未初始化区 p1=0x0 *p1:Cannot access memory at address 0x0
void main()
{
	int b;							//stack DCE <optimized out>
	char s[]="abc";			//stack
	char *p2;						//stack addr(char *p2)=0x0
	char *p3="123456";	//stack pointer point to const
	static int c=1;			//static global initialized, &c=Can't take address of "c" which isn't an lvalue.
	p1 = (char*)malloc(10);	//global pointer point to heap
	p2 = (char*)malloc(20); //stack pointer point to heap addr(char *p2)=0xe5c030
	// strcpy(p2,"123456"); //stack pointer not optimized to point to const

	long addr_a = (long)&a;
	long addr_ac = (long)&ac;
	long addr_p1 = (void*)p1;
	long addr_b = (long)&b;
	long addr_s = (void*)s;
	long addr_p2 = (void*)p2;
	long addr_p3 = (void*)p3;
	long addr_c = (long)&c;

	printf("\nglobal  addr(int a=0)=0x%lx\n", addr_a);
	printf("\nconst   addr(const int ac=0)=0x%lx\n", addr_ac);
	printf("\nglobal  addr(char *p1)=0x%lx\n", addr_p1);
	printf("\nstack   addr(int b)=0x%lx\n", addr_b);
	printf("\nstack   addr(char s[]=\"abc\")=0x%lx\n", addr_s);
	printf("\nheap    addr(char *p2)=0x%lx\n", addr_p2);
	printf("\nheap    addr(char *p3=\"123456\")=0x%lx\n", addr_p3);
	printf("\nglobal  addr(static int c=1)=0x%lx\n", addr_c);

	return;
}

