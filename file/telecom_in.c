#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_LENGTH_1 86
#define TOTAL_LENGTH_2 268

void main(int argc,char *argv[])
{
    char SN[24]="018999902016040812345678";
    char ChipID[8]="00000000";
    char IN[24]="9LQ2oy3sVflPFOth8bqTNdqn";
    char T[14]="20120101000000";
    char R[16]="0000000000000000";
    char MAC[12]="5A0B6CBDB7E9";
    char Total1[172] = {0};
    char SHA[TOTAL_LENGTH_1] = {0};
    char Total2[TOTAL_LENGTH_2] = {0};
    printf("\n SN=%s\n",SN);
    strcpy(Total1,SN);
    printf("\n 1=%s\n",Total1);
    strcpy(Total1,ChipID);
    printf("\n 2=%s\n",Total1);
    strcpy(Total1,IN);
    strcpy(Total1,T);
    strcpy(Total1,R);
    printf("\nTotal1=%s\n",Total1);

}
