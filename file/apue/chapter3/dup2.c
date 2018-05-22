#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
//#include <io.h>
#define STDOUT 1
int main(void)
{
    int nul,oldstdout;
    char msg[] = "This is a test";
    /* create a file */
    nul = open("DUMMY.FIL",O_CREAT | O_RDWR | S_IREAD | S_IWRITE);
    /* create a duplicate handle for standard output */
    oldstdout = dup(STDOUT);
    /*
    redirect standard output to DUMMY.FIL
    by duplicating the file handle onto the
    file handle for standard output.
    */
    dup2(nul,STDOUT);
    /* close the handle for DUMMY.FIL */
    close(nul);
    /* will be redirected into DUMMY.FIL */
    write(STDOUT,msg,strlen(msg));
    /* restore original standard output handle */
    dup2(oldstdout,STDOUT);
    /* close duplicate handle for STDOUT */
    close(oldstdout);
    return 0;
}
