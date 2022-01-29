/*
str1="ABCDEFDE",str2="DE",location=4
*/
#include<stdio.h>
#include<string.h>
#define STR_LEN 10
#define DEBUG printf
#define INFO printf
void main()
{
    char str1[STR_LEN],str2[STR_LEN];
    int len1,len2;
    int i,j,location;
input_str:
    INFO("Please input str1:\n");
    gets(str1);
    //scanf("%s", str1);
    INFO("Please input str2:\n");
    gets(str2);
    //scanf("%s", str2);
    len1=strlen(str1);
    len2=strlen(str2);
    DEBUG("len = %d %d\n",len1,len2);
    if(len1<len2)
    {
        INFO("str2 is longer than str1, input again please\n");
        goto input_str;
    }
    else
    {
        for(i=0; i<len1; i++)
        {
            if(str2[0] == str1[i])
            {
                location = i;
                for(j=0; j<len2; j++)
                {
                    if(str2[j] == str1[location+j])
                    {}
                    else
                    {
                        break;
                    }
                }
                if(j == len2)
                {
                    INFO("match in %d\n",location);
                    break;
                }
            }
        }
        location += 1;
        if(i < len1)
        {
            INFO("first match in %d\n",location);
        }
        else
        {
            INFO("can't match,try again?\n");
        }
    }
}
