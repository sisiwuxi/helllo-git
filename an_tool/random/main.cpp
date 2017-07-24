/************************************************************/
/* Author : sisi.wu                                         		*/
/* Date   : 2017/7/18                                       	*/
/************************************************************/
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <unistd.h>
#include <stdio.h>
#include <MsOS.h>
#include <MsTypes.h>
#include <MsCommon.h>
#include "drvMMIO.h"
#include <drvCA.h>

#define B8BIT ((~(~0))|0xff)

using namespace android;

int main(int argc, char* argv[])
{
    MS_BOOL ret = false;
    int i=0, output_len = 0;
    char output_name[256]= {0};
    MS_U32 random = 0;
    FILE *fp_rng = NULL;

    if(1==argc)
    {
        output_len=128*1024*1024;
        sprintf(output_name,"/data/random/%d.dat",output_len);
        printf("\nDefault:[%d,%s]\n", output_len, output_name);
    }
    else if(2==argc)
    {
        output_len=atoi(argv[1]);
        sprintf(output_name,"/data/random/%d.dat",output_len);
        printf("\n[%d,%s]\n", output_len, output_name);
    }
    else
    {
        printf("\nEx.# ./TestRandom 128\n");
        return -1;
    }
    MDrv_MMIO_Init();
    ret = MDrv_CA_Init();
    if(ret!=1)
        goto FAIL;
    if((fp_rng = fopen(output_name,"wb")) == NULL)
    {
        ALOGD("\nfp_rng error open\n");
        goto FAIL;
    }
    while(i < (int)(output_len/4))		
    {
        random = MDrv_CA_Random();
        fputc((random>>24)&B8BIT, fp_rng);
        fputc((random>>16)&B8BIT,fp_rng);
        fputc((random>>8)&B8BIT, fp_rng);
        fputc(random&B8BIT, fp_rng);
        i++;
    }
FAIL:
    if(fp_rng!=NULL)
        fclose(fp_rng);
    printf("\n====stop_time====\n");
    return 0;
}
