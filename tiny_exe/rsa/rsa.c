#if 1
#include <stdio.h>
int candp(int a,int b,int c)
{ 
    int r=1;
    b=b+1;
    while(b!=1)
    {
        r=r*a;
        r=r%c;
        b--;
    }
    printf("%d\n",r);
    return r;
}

void main()
{
    int p,q,e,d,m,n,t,c,r;
    char s;
    printf("please input the p: ");
    scanf("%d",&p);
    printf("please input the q: ");
    scanf("%d",&q);
    n=p*q;
    printf("the n is %3d\n",n);
    t=(p-1)*(q-1);
    printf("the t is %3d\n",t);
    printf("please input the e: ");
    scanf("%d",&e);
    if(e<1||e>t)
    {
        printf("e is error,please input again: ");
        scanf("%d",&e);
    }
    d=1;
    while(((e*d)%t)!=1)   
        d++;
    printf("then caculate out that the d is %d\n",d);
    printf("the cipher please input 1\n");
    printf("the plain please input 2\n");
    scanf("%d",&r);
    switch(r)
    {
        case 1: printf("input the m: "); /*输入要加密的明文数字*/
            scanf("%d",&m);
            c=candp(m,e,n);
            printf("the cipher is %d\n",c);break;
        case 2: printf("input the c: "); /*输入要解密的密文数字*/
            scanf("%d",&c);
            m=candp(c,d,n);
            printf("the cipher is %d\n",m);break;
    }
    //getch();
}
/*
please input the p: 23
please input the q: 37
the n is 851
the t is 792
please input the e: 5
then caculate out that the d is 317
the cipher please input 1
the plain please input 2
1
input the m: 56
318
the cipher is 318
*/
#endif


//===============================================================================//
#if 0
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
//将十进制数转换成二进制，用于检验大素数 p 和 q 
int zhuan_huan(int b,int a[],int k) 
{ 
	int t,temp=-1; 
	while(b>0)
	{ 
		t=b%2; 
		temp++; 
		a[temp]=t; 
		b=b/2; 
	} 
	return temp; 
} 
//欧几里得算法，用于判断加密指数 e 是否符合要求 
int gcd(int n,int b) 
{ 
	int r1=n,r2=b,r; 
	while(r2>0)
	{ 
		r=r1%r2; 
		r1=r2; 
		r2=r; 
	} 
	return r1; 
} 
//扩展欧几里得算法求乘法逆元，即求解密指数 d 
int extend(int n,int b) 
{ 
	int q,r,r1=n,r2=b,t,t1=0,t2=1,i=1; 
	while(r2>0) 
	{ 
		q=r1/r2; 
		r=r1%r2; 
		r1=r2; 
		r2=r; 
		t=t1-q*t2; 
		t1=t2; 
		t2=t; 
	} 
	if(t1>=0) 
		return t1%n; 
	else
	{
		while((t1+i*n)<0) 
			i++; 
		return t1+i*n; 
	} 
} 
//检验大素数，符合要求返回 1，否则返回 0 
int Witness(int a,int n) 
{ 
	int d=1,k,r=n-1,i,x,b[1000]; 
	k=zhuan_huan(r,b,1000); 
	for(i=k;i>=0;i--)
	{ 
		x=d; 
		d=(d*d)%n; 
		if((d==1)&&(x!=1)&&(x!=n-1)) 
			return 0; 
		if(b[i]==1) d=(d*a)%n; 
	} 
	if(d!=1) 
		return 0; 
	else 
		return 1; 
} 
//快速计算模指数 
int js_mod(int a,int b,int n) 
{ 
	int x=0,y=1,k,i,s[1000]; 
	k=zhuan_huan(b,s,1000); 
	for(i=k;i>=0;i--)
	{ 
		x=2*x; 
		y=(y*y)%n; 
		if(s[i]==1)
		{ 
			x++; 
			y=(y*a)%n; 
		} 
	} 
	return y; 
} 
void main() 
{ 
	int p;
	int q;
	int e;
	int d;
	int n;
	
	int m[1000];
	int c[10000];//c[10000]存放加密后的数字密文，m[1000]存放解密后 的数字明文，即英文明文在 zimu_biao[69]中的下标。 
	int i,j; //i,j 用于循环遍历数组 
	int mi_yue; //用户输入的密钥 
	int count=1; //统计输入密钥的次数，count>3 时将不允许用户再输入。 
	char min_wen[1000],re_min_wen[1000];//分别为用户输入的明文、密文，解密后的明文。 
	//密钥生成 
	char zimu_biao[69]="abcdefghijklmnopqrstuvwxyz,ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789'.?!"; 
	int yn;
	printf("请输入您要解密的明文文件（小写英文表示）:\n"); 
	printf("******************************************************\n"); 
	gets(&c); 
	printf("******************************************************\n"); 


d=0x4F35C974DFDA7AD6A35CB19769F6CBFE63546F4EAF729013D697CD2A5F77265CFBDE9A468D6C3D6026D5A861EB4579A40B8F748376FACFF14FEC158224153895FB6D16100DE02095ABF29BA3B84FC8C6B6E20E83154248CF5AF98504E3FEC87C95D3509D1A3F6CD71499F865957F50C1E8763AA93E27FD36D74D5357C9F98DCF;
n=0x7304BF0A4F4A1E17222F455F53B39DFC58065DF5F2D7FFB3F2090FE3F6720C51FB68063EDA828CED543E3A1BD614971EB4008C667451EBBB85AA8DF5F758150AAF8CE8E1BD2A27D138A813EDC98F326BE217F2C979F7BA9D0A48A02B0A3B1DD5A18249E4A48A538B44101D8D2A3A8A6CA1A9A5B9388C54198A9716272BF514B9;

//解密过程 
	for(i=0;i<strlen(min_wen);i++) 
		m[i]=js_mod(c[i],d,n); 

	for(i=0;i<strlen(m);i++) 
		printf("%c",m[i]); 
}

//主函数
#if 0
void main() 
{ 
	int p;
	int q;
	int e;
	int d;
	int n;
	
	int m[1000];
	int c[10000];//c[10000]存放加密后的数字密文，m[1000]存放解密后 的数字明文，即英文明文在 zimu_biao[69]中的下标。 
	int i,j; //i,j 用于循环遍历数组 
	int mi_yue; //用户输入的密钥 
	int count=1; //统计输入密钥的次数，count>3 时将不允许用户再输入。 
	char min_wen[1000],re_min_wen[1000];//分别为用户输入的明文、密文，解密后的明文。 
	//密钥生成 
	char zimu_biao[69]="abcdefghijklmnopqrstuvwxyz,ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789'.?!"; 
	int yn;
	printf("请输入您要发送的明文文件（小写英文表示）:\n"); 
	printf("******************************************************\n"); 
	gets(min_wen); 
	printf("******************************************************\n"); 
	printf("\n 加密开始，请按要求操作。 。 。\n\n"); 
	printf("请输入第一个大素数 p：\n"); 
	while(1)
	{ 
		scanf("%d",&p); 
		if(Witness(2,p)==1)
		{ 
			printf("您输入的第一个大素数 %d 符合要求\n",p); 
			break; 
		} 
		else 
			printf("您输入的 %d 不是素数,请重新输入：\n",p); 
	} 
	printf("请输入第二个大素数 q：\n"); 
	while(1)
	{ 
		scanf("%d",&q); 
		if(Witness(2,q))
		{ 
			printf("您输入的第二个大素数 %d 符合要求\n",q); 
			break; 
		} 
		else 
			printf("您输入的 %d 不是素数,请重新输入：\n",q); 
	} 
	n=p*q; 
	yn=(p-1)*(q-1); 
	printf("请输入加密指数（整数）e,且 0<e<%d\n",yn);
//下面由用户设定加密指数 
	while(1)
	{ 
		scanf("%d",&e); 
		if(gcd(yn,e)==1)
		{ 
			printf("您输入加密指数 %d 与 %d 互素,符合要求\n",e,yn); 
			break; 
		} 
		else 
			printf("您输入加密指数 %d 与 %d 不互素,请重新输入。 。 。\n",e,yn); 
	}
	d=extend(yn,e); //求解密指数 d 
	printf("\n\n 请记住您的两个大素数分别为 p=%d(保密),q=%d(保密),模数 n=%d(公开),欧 拉 函 数 yn=%d( 保 密 ), 加 密 指 数 e=%d( 公 钥 , 公 开 ), 。 。 。 解 密 指 数 d=%d( 私 钥 , 保 密)\n\n",p,q,n,yn,e,d); 
//明文转换过程 
	/* scanf("%s",min_wen); printf("%s",min_wen); */ 
	for(i=0;i<strlen(min_wen);i++) 
		for(j=0;j<68;j++) //for(j=0;j<26;j++) 
			if(min_wen[i]==zimu_biao[j]) 
				m[i]=j;//将字符串明文换成数字，并存到整型数组 m 里面，即明文的 另一种表示方法 
//加密过程 
	for(i=0;i<strlen(min_wen);i++) 
		c[i]=js_mod(m[i],e,n); 
	printf("输出密文：\n"); 
	printf("******************************************************\n"); 
	for(i=0;i<strlen(min_wen);i++) printf("%d",c[i]); 
		printf("\n******************************************************\n"); 
//解密过程 
	for(i=0;i<strlen(min_wen);i++) 
		m[i]=js_mod(c[i],d,n); 
	for(i=0;i<strlen(min_wen);i++) 
		re_min_wen[i]=zimu_biao[m[i]]; 
//提示用户解密 
	printf("\n\n 您有 3 次输入密钥的机会，密钥正确后将进行解密显示明文，3 次输入错误 解密将终止，请注意。 。 。\n\n"); 
	while(1)
	{ 
		scanf("%d",&mi_yue); 
		if(mi_yue==d)
		{ 
			printf("密钥输入正确，您得到的明文为：\n\n"); 
			for(i=0;i<strlen(min_wen);i++) 
				printf("%c",re_min_wen[i]); 
			printf("\n\n"); 
			break; 
		} 
		else{
			printf("您第%d 次输入的密钥错误，请重新输入。 。 。\n",count); 
			count++; 
			if(count>3)
			{ 
				printf("\n 您已%d 次输入的密钥错误，将不允许继续输入\n",count-1); break; 
			} 
		} 
	} 
} 
#endif
#endif



