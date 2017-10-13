#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ./a.out input.ts output.ts 0 3440227
//#define CUT_OFFSET 0x4499EC
#define MAX_NAME_LEN 20

void main(int argc,char *argv[])
{
    int i;
    int StartOffset = 0, CutOffset = 0, FileSize = 0;	
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
    fseek(fp1, 0, SEEK_END);
    FileSize = ftell(fp1);	
    if(argv[3] != NULL)
    {
        StartOffset = 	atoi(argv[3]);
        fseek(fp1, StartOffset, SEEK_SET);
        now_len = now_len+StartOffset;		
    }
    if(argv[4] != NULL)
    {
        CutOffset = atoi(argv[4]);
    }	
	
    if(CutOffset > StartOffset)
    {
        printf("\n=================[%d~%d]===================\n", StartOffset, CutOffset);		
        if((fp1 != NULL) && (fp2 != NULL))
        {
            while((now_len<CutOffset) && (CutOffset<FileSize))
            {
                fputc(fgetc(fp1),fp2);
                now_len++;
            }
        }
    }			
    else
    {
        printf("\n=================[%d~EOF]===================\n", StartOffset);		
        if((fp1 != NULL) && (fp2 != NULL))
        {
            while(now_len<FileSize)
            {
                fputc(fgetc(fp1),fp2);
                now_len++;
            }
        }			
    }
    printf("\n==================cut file end====================\n");
    fclose(fp1);
    fclose(fp2);
    return;	
}

	
