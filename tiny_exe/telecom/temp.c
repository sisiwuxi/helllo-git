#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_LENGTH_1 86
#define TOTAL_LENGTH_2 268

void main(int argc,char *argv[])
{
    int i=0; 
    int len=0;
    char SN[]="018999902016040812345678";
    char ChipID[]="1234";
    char IN[]="9LQ2oy3sVflPFOth8bqTNdqn";
    char T[]="20120101000000";
    char R[]="0000000000000000";//0000000000000000
    char MAC[]="5A0B6CBDB7E9";
    char Total1[100] = {0};
    char SHA[] = "C23E33BE8759B0D1D8D1008FB923F7B4A9425B9F1E80C42EC7A27C47BF06E066";
    char Total2[300] = {0};

    strcpy(Total1,SN);
    strcat(Total1,ChipID);
    strcat(Total1,IN);
    strcat(Total1,T);
    strcat(Total1,R);
    printf("\nTotal1=%s\n",Total1);

    char output[2 * 32 + 1];
    output[2 * 32] = '\0';
    printf("sha256 digest: [%s,%s]\n", SHA, output);
    for (i = 0; i < (int) 32 ; i++) 
       sprintf((char *) output + 2 * i, "%02X", SHA[i]);

     //SHA=output;
     printf("sha256 digest: [%s,%s]\n", SHA, output);
}

//====================alignment4_endian======================//
#if 0
void main(int argc,char *argv[])
{
    int i=0; 
    int len=0;
    FILE *fp1=NULL;
    FILE *fp2=NULL;
    char SN[]="018999902016040812345678";
    char ChipID[]="0000";//00000000
    char IN[]="9LQ2oy3sVflPFOth8bqTNdqn";
    char T[]="20120101000000";
    char R[]="0000000000000000";//0000000000000000
    char MAC[]="5A0B6CBDB7E9";
    char Total1[100] = {0};
    char SHA[] = "C23E33BE8759B0D1D8D1008FB923F7B4A9425B9F1E80C42EC7A27C47BF06E066";
    char Total2[300] = {0};

    strcpy(Total1,SN);
    strcat(Total1,ChipID);
    strcat(Total1,IN);
    strcat(Total1,T);
    strcat(Total1,R);
    printf("\nTotal1=%s\n",Total1);

    if((fp1 = fopen("total1.bin","wb")) == NULL)
    {
        printf("error open1\n"); 
        return;
    }
    len=strlen(Total1);
    printf("\n 0x%x 0x%x \n",Total1[0],Total1[1]);
    for(i=0;i<len;i+=4)
    {
        if((i+3) >= len)
        {
        fputc(0x30,fp1);
        }
        else
        {        
        fputc(Total1[i+3],fp1);
        }

        if((i+2) >= len)
        {
        fputc(0x30,fp1);
        }
        else
        {        
        fputc(Total1[i+2],fp1);
        }

        if((i+1) >= len)
        {
        fputc(0x30,fp1);
        }
        else
        {        
        fputc(Total1[i+1],fp1);
        }

        fputc(Total1[i],fp1);
    }
    fclose(fp1);
//===============================================//
    strcpy(Total2,SHA);
    strcat(Total2,MAC);
    printf("\nTotal2=%s\n",Total2);
    if((fp2 = fopen("sha.bin","wb")) == NULL)
    {
        printf("error open2\n"); 
        return;
    }
    len=strlen(Total2);
    for(i=0;i<len;i+=4)
    {
        fputc(Total2[i+3],fp2);
        fputc(Total2[i+2],fp2);
        fputc(Total2[i+1],fp2);
        fputc(Total2[i],fp2);
    }
    fclose(fp2);
//===============================================//
}

#endif
