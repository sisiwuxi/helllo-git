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
void main()
{
//    int s[LEN]={3,6,80,23,6,44,19,7};
    int s[LEN]={3,6,80,23,5,44,19,7};
    int i,j,temp;
    int n=8;
    for(i =0 ; i< (n-1); ++i) 
    {  
print_s(s, LEN);
        for(j = 0; j < (n-i-1); ++j) 
        {  
            if(s[j] > s[j+1])  
            {  
                temp = s[j] ; s[j] = s[j+1] ;  s[j+1] = temp;  
            }  
        }
    }  

    printf("The result:\n"); 
    print_s(s, LEN);
}
