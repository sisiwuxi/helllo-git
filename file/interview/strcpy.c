#include<stdio.h>
#define LEN 20

char *strcpy(char *strDest, const char *strSrc)
{
    if((NULL == strDest) || (NULL == strSrc))
    {
	printf("Error\n");
        return;
    }
    int i;
    char *address = strDest;
    for(i=0;;i++)
    {
        if(*strSrc != '\0')//0
        {
            *strDest = *strSrc;
            strDest++;
            strSrc++;
        }
        else
        {
            printf("End of strcpy\n");
            break;
        }
    }
    printf("strlen=%d",i);
    return address;
}

void main()
{
    char Src[LEN];
    char Dest[LEN];
    printf("PLease inpute Str:\n"); 
    gets(Src);   
    strcpy(Dest, Src);
    printf("\n[%s,%s]\n",Dest,Src);
    return;
}
