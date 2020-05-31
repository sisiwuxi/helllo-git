#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if 1
int main(void) {
    write(1,"hello",5);//write to kernel's cache, not exit->flush
    fork();
}
#else
int main(void) {
    printf("hello");
    fflush(stdout);
    _exit(0);
}
int main(void) {
    setbuf(stdout, NULL);
    printf("hello");
    _exit(0);
}
int main(void) {
    printf("hello\n");
    fork();//father + sun, flush user's cache twice
}
int main(void) {
    printf("hello");
    fork();
}
int main(void) {
    printf("hello");
    exit(0);
}
int main(void) {
    printf("hello");
    _exit(0);
}
#endif
