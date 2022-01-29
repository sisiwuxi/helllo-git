#include<stdio.h>
#include<string.h>

void main() {
    char tmp = 0;
    char s[] = "revert askj";
    printf("s=%s\n",s);
    int n = strlen(s);
    for (int i=0; i<n/2; i++) {
        tmp = s[i];
        s[i] = s[n-i-1];
        s[n-i-1] = tmp;
    }
    printf("s=%s\n",s);
    return;
}