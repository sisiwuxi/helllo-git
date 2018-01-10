#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[])
{
    int input;
    int n;
    int *p1=NULL;
    int *p2=NULL;

    if((p2=(int *)malloc(5*sizeof(int)))==NULL)
    {
        printf("malloc memory fail");
        exit(1);
    }

    printf("p2 addr: 0x%8X\n",p2);

    for(n=0; n<5; n++) //初始化
    {
        *(p2+n)=n;
    }

    printf("Enter new size: ");
    scanf("%d",&input);

    p1=(int *)realloc(p2,(input+5)*sizeof(int));

    if(p1==NULL)
    {
        printf("Error (re)allocating memory");
        exit(1);
    }
    printf("numbers1 addr: %8X\n",(int)p1);

    for(n=0;n<(input+5);n++) //输出从numbers2拷贝来的数据
    {
        printf("%d ",*(numbers1+n));
    }

    free(p1);//此处不需要free p2，因为在realloc()时已经释放
    p1=NULL;
    //free(p2);//不能再次释放
    return 0;
}
