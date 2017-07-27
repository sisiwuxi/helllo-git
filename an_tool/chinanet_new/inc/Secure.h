/****************************************************************************
 * File Name: Secure.h
 *
 * Description: 
 *      
 * Author:
 *
 *
 * Revision History: 
 *
****************************************************************************/
#ifndef __MAPLF_H__
#define __MAPLF_H__

#ifdef __cplusplus
extern "C" {
#endif
#define SHA256_DIGEST_SIZE    128   // 256-bits
#define SIGNATURE_LEN               256
#define RSA_PUBLIC_KEY_N_LEN 256
#define RSA_PUBLIC_KEY_N_LEN_1024 128
#define RSA_PUBLIC_KEY_E_LEN 4
#define RSA_PUBLIC_KEY_LEN          (RSA_PUBLIC_KEY_N_LEN+RSA_PUBLIC_KEY_E_LEN)
#define RSA_PUBLIC_KEY_LEN_1024          (RSA_PUBLIC_KEY_N_LEN_1024+RSA_PUBLIC_KEY_E_LEN)
#define AES_KEY_LEN 16
#define AES_KEY_LEN 16
#define HMAC_KEY_LEN 32


int TA_GetSN(unsigned char* pSNValue, int* pnSNValue);
int sha256(const unsigned char *message, unsigned int len, unsigned char *digest);
int RSAinit(void);
int Command_execv(const char *pathname, char * const argv[]);
int RSA_Decrypt(char* inputbuf, int inputlen, char* outputbuf) ;
int terminalEncrypt(char* authinput,int inputlen, char* outbuf) ;
int MSTAR_readRandom(char* rand) ;
int SHA256(char* inputbuf, int inputlen, char* outputbuf) ;
int MSTAR_Hex_RSA_Decrypt(char* inputbuf, int inputlen, char* outputbuf) ;
int MSTAR_Hex_SHA256_Decrypt(const unsigned char *message, unsigned int len, unsigned char *digest);
int TA_TMtokenSyn(unsigned char* pTMtokenValue, int* pnTMtokenValue);
int TA_SPubKeyCertify(unsigned char* pKeyPackage, int* pnKeyPackage,unsigned char* pRandom_TM, int* pnRandom_TM, unsigned char* pTimeStamp_TM, int* pnTimeStamp_TM, unsigned char* pAuthenticator, int* pnAuthenticator, int* pAlgorithmID_H, int* pAlgorithmID_Auth);
int Factory_IN_Certify(unsigned char* pSN,unsigned char* pMAC,unsigned char* pIN,unsigned char* pchipid,unsigned char* MD5,unsigned char* OutIN);
int TA_Factory_Certify_TimeStamp(unsigned char* pTimeStampValue, int* pnTimeStampValue);
int TA_Factory_Certify_Random(unsigned char* pRandomValue, int* pnRandomValue);
int Factory_TMtoken_Certify(unsigned char* pAuthenticator,unsigned char* pSN,unsigned char* pChipID,unsigned char* pIN,unsigned char* SHA256_SN_CHIPID_IN_T2_R2_TEST);
int TA_OTP_SetChipID(unsigned char* pChipIDValue);
int TA_OTP_GetChipID(unsigned char* pChipIDValue);
int TA_OTP_Write_SN_Data(unsigned char* pSNDataValue);
int TA_OTP_Write_IN_Data(unsigned char* pINDataValue) ;
int TA_OTP_Write_RSA_N_Data(void);
int TA_OTP_Write_RSA_E_Data(void);
int TA_OTP_Read_RSA_N_Data(unsigned char* RSA_N);
int TA_OTP_Read_RSA_E_Data(unsigned char* RSA_E) ;
int TA_OTP_Write_M_Data(void) ;
int TA_OTP_Read_Data(unsigned char* pReadDataValue) ;
int TA_OTP_Write_M_SN_IN_Data(unsigned char* pMSNINDataValue);
int TA_OTP_MaxReservedSize();
int TA_OTP_Lock();
void RSA_test(void);

/////////////////////////////////////MD5///////////////////////////////////////////////////////////
typedef struct  
{  
    unsigned int count[2];  
    unsigned int state[4];  
    unsigned char buffer[64];     
}MD5_CTX;  
   
                           
#define F(x,y,z) ((x & y) | (~x & z))  
#define G(x,y,z) ((x & z) | (y & ~z))  
#define H(x,y,z) (x^y^z)  
#define I(x,y,z) (y ^ (x | ~z))  
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))  
#define FF(a,b,c,d,x,s,ac) \  
          { \  
          a += F(b,c,d) + x + ac; \  
          a = ROTATE_LEFT(a,s); \  
          a += b; \  
          }  
#define GG(a,b,c,d,x,s,ac) \  
          { \  
          a += G(b,c,d) + x + ac; \  
          a = ROTATE_LEFT(a,s); \  
          a += b; \  
          }  
#define HH(a,b,c,d,x,s,ac) \  
          { \  
          a += H(b,c,d) + x + ac; \  
          a = ROTATE_LEFT(a,s); \  
          a += b; \  
          }  
#define II(a,b,c,d,x,s,ac) \  
          { \  
          a += I(b,c,d) + x + ac; \  
          a = ROTATE_LEFT(a,s); \  
          a += b; \  
          }                                              
void MD5Init(MD5_CTX *context);  
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);  
void MD5Final(MD5_CTX *context,unsigned char digest[16]);  
void MD5Transform(unsigned int state[4],unsigned char block[64]);  
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);  
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);  
///////////////////////////////////////////////////////////////////////////////////////////////////

int Reboot_Recovery_Update(void);
int Reboot_Recovery_Restore_factory(void);

#ifdef __cplusplus
}
#endif

#endif
