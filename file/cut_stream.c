#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUT_OFFSET 0x4499EC
#define MAX_NAME_LEN 20

void main(int argc,char *argv[])
{
    int i;
    printf("There are %d arguments passed to main\n", argc);
    for(i=0; i<=argc; i++)
    {
        printf("argv[%d]:%s\n",i,argv[i]);
    }
    char inputfile[MAX_NAME_LEN] = {0};
    char outputfile[MAX_NAME_LEN] = {0};
    FILE *fp1=NULL, *fp2=NULL;
    int now_len = 0;
    int total_len = 0;
    char temp1, temp2, temp3;
    if(argv[1] != NULL)
    {
        strcpy(inputfile, argv[1]);
        if((fp1 = fopen(inputfile,"rb")) == NULL)
        {
            printf("eror open1\n");
            return;
        }
    }
    if(argv[2] != NULL)
    {
        strcpy(outputfile, argv[2]);
        if((fp2 = fopen(outputfile,"wb")) == NULL)
        {
            printf("eror open2\n");
            return;
        }
    }
    if((fp1 != NULL) && (fp2 != NULL))
    {
        printf("\n==================1====================\n");
        fseek(fp1, 0, SEEK_SET);
        //fseek(fp1, CUT_OFFSET, SEEK_SET);
        //====================START======================
        //while(!feof(fp1))
        printf("\n==================2====================\n");
        while(now_len<CUT_OFFSET)
        {
            fputc(fgetc(fp1),fp2);
            now_len++;
        }
        printf("\n==================cut stream end====================\n");
        fclose(fp1);
        fclose(fp2);
    }
}
