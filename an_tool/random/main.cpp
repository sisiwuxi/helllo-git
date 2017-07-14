
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <unistd.h>
#include <stdio.h>
#include <MsOS.h>
#include <MsTypes.h>
#include <MsCommon.h>
//#include "MMAPInfo.h"
#include "drvMMIO.h"
#include "drvSEM.h"
#include "drvMBX.h"
#include "drvBDMA.h"
#include "drvDSCMB.h"
#include "drvMIU.h"
#include "drvSYS.h"
#include "drvWDT.h"
#include "drvCMAPool.h"
#include <drvCA.h>
#include <utils/Log.h>

#define TRNG_NUM 16777216
#define B8BIT ((~(~0))|0xff)
extern MS_BOOL MDrv_CA_Init(void);
extern MS_U32 MDrv_CA_Random(void);

using namespace android;

void DriverInitBase(void)
{
    MsOS_MPool_Init();
    MsOS_Init();
    MDrv_MMIO_Init();
    MDrv_MIU_Init();
    MDrv_MIU_SetIOMapBase();
    MDrv_SEM_Init();
    MBX_Result mbxResult;
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 1500;
    eHKCPU = E_MBX_CPU_MIPS;
    mbxResult = MDrv_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs);
    if(E_MBX_SUCCESS != mbxResult)
    {
        ALOGE("MBX init error\n");
    }
    MDrv_MBX_Enable(TRUE);
    MDrv_BDMA_Init(0x00); //in current utopia code, the input parameter isn't used
    MDrv_SYS_Init(); // sys init should do before mapi demux init due to hw capability
    return;
}

int main(int argc, char** argv)
{
    MS_BOOL ret = false;
    //DriverInitBase();
    MDrv_MMIO_Init();
    ret = MDrv_CA_Init();
    //ret = MDrv_CA_STR_Init();
    ALOGD("\nhello ret=%d \n", ret);
    MS_U32 random = 0;
    int i = 0;
    FILE *fp_rng = NULL;
    if((fp_rng = fopen("/data/random/64.txt","wb")) == NULL)
        ALOGD("\nfp_rng error open\n");
    while(i < 16777216)
    {
        random = MDrv_CA_Random();
        fputc((random>>24)&B8BIT, fp_rng);
        fputc((random>>16)&B8BIT,fp_rng);
        fputc((random>>8)&B8BIT, fp_rng);
        fputc(random&B8BIT, fp_rng);
        i++;
    }
    fclose(fp_rng);
    return 0;
}
