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

int RSAinit(void);
int RSA_Decrypt(char* inputbuf, int inputlen, char* outputbuf) ;
int terminalEncrypt(char* authinput,int inputlen, char* outbuf) ;
int readRandom(char* rand) ;
int SHA256(char* inputbuf, int inputlen, char* outputbuf) ;

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

#ifdef __cplusplus
}
#endif

#endif
