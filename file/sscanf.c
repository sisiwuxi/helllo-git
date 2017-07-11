#include<stdio.h>
int main(int argc, char *argv[])
{
    char buf[512];
    sscanf("123456","%s",buf);
    printf("%s\n",buf);
    sscanf("iios/12ddwefd@122","%*[^/]/%[^@]",buf);
    printf("%s\n",buf);
    sscanf("1","%s",buf);
    printf("%s\n",buf);
    sscanf("dv://192.168.1.253:65001/=123","%*[^=]%*c%s",buf);
    printf("%s\n",buf);
    int pId = 0;
    sscanf("dv://192.168.1.253:65001/=123","%*[^=]%*c%d",&pId);
    printf("%d\n", pId);
    return 0;
}

