#include <stdio.h>
#include <stdlib.h>
#define ATSM_CODE 0x4154534D

void main()
{
    FILE *fp1;

    int i, num = 0, offset = 0, filelen = 0;
    char c1,c2,c3,c4;
    int ATSM_Code = 0x00;

    if((fp1 = fopen("uuid_keys.table","rb")) == NULL)//"1.txt"
        printf("error fp1 open\n");
    while(!feof(fp1))
    {
	fgetc(fp1);
        filelen++;
    }
    fseek(fp1, 0, SEEK_SET);
    while(!feof(fp1) && (offset < (filelen-4)))
    {
	c1=fgetc(fp1);
	c2=fgetc(fp1);
	c3=fgetc(fp1);
	c4=fgetc(fp1);
        ATSM_Code = (c1<<24)|(c2<<16)|(c3<<8)|(c4);
        if(ATSM_CODE == ATSM_Code)
        {
            printf("\n 0x%lx ",(ftell(fp1)-4));
	    num++;
	    fseek(fp1, (ftell(fp1)-51), SEEK_SET);//-4-12-35=-51
            for(i=0; i<35; i++)
            {
                printf("%c",fgetc(fp1));
            }
        }
        offset++;
        fseek(fp1, offset, SEEK_SET);
    }
    printf("\n num=%d\n",num);
    fclose(fp1);
}
