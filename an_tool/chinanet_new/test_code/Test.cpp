#include "Secure.h"
#include <stdio.h>
#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <memory.h>  
#include <cutils/properties.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <android/log.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define LOG_TAG "JNI_LOG"
 
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

typedef unsigned char uint8;
typedef unsigned int  uint32;
typedef unsigned long long uint64;

#define UNROLL_LOOPS /* Enable loops unrolling */

int main( int argc, char * argv[] )
{
    unsigned char data_r[61] = {0};
    unsigned char data_chipID[61] = {0};
    unsigned char  data_N_Data[129] = {0};


    printf("enter into  main function !!\r\n");
    RSAinit();

  
    // CTIT + SN + IN + RSA_E
    TA_OTP_Read_Data(data_r);
    printf("\nTA_OTP_Read_Data[%s]\n",data_r);

    TA_OTP_GetChipID(data_chipID);
    printf("\nTA_GetChipID:[%s]\n",data_chipID,__FUNCTION__);

    
    TA_OTP_Read_RSA_N_Data(data_N_Data);
    printf("TA_OTP_Read_RSA_N_Data \n");
    for(int i=0; i<128;i++)
    {
        printf("%02X ", *(( char *)data_N_Data + i));
    }
    printf("\r\n");

    return 0;
}
