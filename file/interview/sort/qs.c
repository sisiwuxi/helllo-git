#include<stdio.h>

#define LEN 8
void print_s(int s[], int n)
{  
    int k;
    for(k=0; k<n; k++)
    {  
        printf("%d ", s[k]);  
    }  
    printf("\n");  
} 
void swap(int s[],int i,int j)  
{  
    int temp;  
    temp=s[i];  
    s[i]=s[j];  
    s[j]=temp;  
    print_s(s, LEN);
}  
void QuickSort(int s[],int low,int high)  
{  
    int i;  
    int last;//记录基准的位置  
    if(low<high)//当数组中的元素个数大于1时，才进行操作  
    {  
        last=low;//选取第一个元素作为基准  
        printf("==============[%d,%d]\n", last, high); 
        //把小于基准元与大于基准元的分开，last记录它们分开的界限  
        for(i=low+1;i<=high;i++)  
        {  
            printf("[%d,%d,%d] ", s[i],s[low],s[last]);
            if(s[i]<s[low])  
                swap(s,++last,i);
        }  
        swap(s,last,low);//基准元与界限交换，这样的话，基准元两边就是一边大于，一边小于；  
        QuickSort(s,low,last-1);  //对左区间递归排序  
        QuickSort(s,last+1,high);//对右区间递归排序  
    }  
} 
void main()
{
//    int s[LEN]={3,6,80,23,6,44,19,7};
    int s[LEN]={3,6,80,23,6,44,19,7};
    int i,j,temp;
    QuickSort(s,0,LEN-1);
    printf("The result:\n"); 
    print_s(s, LEN);
}
