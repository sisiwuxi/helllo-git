#include "Secure.h"
#include <MsTypes.h>
#include <drvAESDMA.h>
#include <stdio.h>
#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <memory.h>  
//#include <drvAESDMA.h>
#include <drvCIPHER.h>
#include "drvMMIO.h"
#include <cutils/properties.h>
#include <drvCA.h>
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
#define SHA256_BLOCK_SIZE  ( 512 / 8)

typedef struct {
    unsigned int tot_len;
    unsigned int len;
    unsigned char block[2 * SHA256_BLOCK_SIZE];
    MS_U32 h[8];
} sha256_ctx;

typedef struct {
	uint32_t total[2];
	uint32_t state[8];
	uint8_t buffer[64];
} sha256_context;


#define UNPACK32(x, str)                      \
{                                             \
    *((str) + 3) = (uint8) ((x)      );       \
    *((str) + 2) = (uint8) ((x) >>  8);       \
    *((str) + 1) = (uint8) ((x) >> 16);       \
    *((str) + 0) = (uint8) ((x) >> 24);       \
}

#define PACK32(str, x)                        \
{                                             \
    *(x) =   ((uint32) *((str) + 3)      )    \
           | ((uint32) *((str) + 2) <<  8)    \
           | ((uint32) *((str) + 1) << 16)    \
           | ((uint32) *((str) + 0) << 24);   \
}

#define SHFR(x, n)    (x >> n)
#define ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define CH(x, y, z)  ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

#define SHA256_F1(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SHA256_F2(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SHA256_F3(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
#define SHA256_F4(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))

/* Macros used for loops unrolling */

#define SHA256_SCR(i)                         \
{                                             \
    w[i] =  SHA256_F4(w[i -  2]) + w[i -  7]  \
          + SHA256_F3(w[i - 15]) + w[i - 16]; \
}

uint32 sha256_k[64] =
            {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
             0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
             0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
             0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
             0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
             0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
             0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
             0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
             0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
             0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
             0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
             0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
             0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
             0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
             0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
             0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};


#define SHA256_EXP(a, b, c, d, e, f, g, h, j)               \
{                                                           \
    t1 = wv[h] + SHA256_F2(wv[e]) + CH(wv[e], wv[f], wv[g]) \
         + sha256_k[j] + w[j];                              \
    t2 = SHA256_F1(wv[a]) + MAJ(wv[a], wv[b], wv[c]);       \
    wv[d] += t1;                                            \
    wv[h] = t1 + t2;                                        \
}

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i) {				\
	(n) = ( (unsigned long) (b)[(i)    ] << 24 )	\
	    | ( (unsigned long) (b)[(i) + 1] << 16 )	\
	    | ( (unsigned long) (b)[(i) + 2] <<  8 )	\
	    | ( (unsigned long) (b)[(i) + 3]       );	\
}
#endif
#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i) {				\
	(b)[(i)    ] = (unsigned char) ( (n) >> 24 );	\
	(b)[(i) + 1] = (unsigned char) ( (n) >> 16 );	\
	(b)[(i) + 2] = (unsigned char) ( (n) >>  8 );	\
	(b)[(i) + 3] = (unsigned char) ( (n)       );	\
}
#endif

uint32 sha256_h0[8] =
            {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
             0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};


MS_U8 u8RSA_Decrypt_key2048[RSA_PUBLIC_KEY_LEN+RSA_PUBLIC_KEY_N_LEN]={
    0x84 ,0x62 ,0x76 ,0xCC ,0xBD ,0x5A ,0x5A ,0x40 ,0x30 ,0xC0 ,0x96 ,0x40 ,0x87 ,0x28 ,0xDB ,0x85 ,
    0xED ,0xED ,0x9F ,0x3E ,0xDE ,0x4E ,0x65 ,0xE6 ,0x7B ,0x1B ,0x78 ,0x17 ,0x87 ,0x9D ,0xF6 ,0x16 ,
    0xC3 ,0xD3 ,0x27 ,0xBC ,0xB4 ,0x5A ,0x3  ,0x13 ,0x35 ,0xB0 ,0x96 ,0x5A ,0x96 ,0x41 ,0x74 ,0x4E ,
    0xB9 ,0xD1 ,0x77 ,0x96 ,0xF7 ,0x8D ,0xE2 ,0xE7 ,0x15 ,0x9  ,0x65 ,0x9C ,0x46 ,0x79 ,0xEA ,0xF0 ,
    0x91 ,0x67 ,0x35 ,0xFA ,0x69 ,0x4C ,0x83 ,0xF7 ,0xDC ,0xCF ,0x97 ,0x20 ,0xF2 ,0xA5 ,0xBA ,0x72 ,
    0x80 ,0x9D ,0x55 ,0x79 ,0x17 ,0xDC ,0x6E ,0x60 ,0xA5 ,0xE7 ,0xE  ,0x9E ,0x89 ,0x9B ,0x46 ,0x6  ,
    0x52 ,0xFC ,0x64 ,0x56 ,0x2  ,0x8  ,0x9A ,0x96 ,0x41 ,0xE2 ,0x4F ,0xDB ,0xB6 ,0x60 ,0xC3 ,0x38 ,
    0xDF ,0xF4 ,0x97 ,0x81 ,0x5D ,0x12 ,0x2  ,0xAE ,0x2B ,0x9F ,0x9  ,0x29 ,0xB9 ,0x9D ,0x51 ,0x45 ,
    0xD2 ,0x9E ,0x2B ,0xAF ,0x64 ,0xCA ,0x9A ,0x6  ,0x4E ,0x94 ,0x35 ,0x67 ,0xF7 ,0x8E ,0x4  ,0x7B ,
    0x24 ,0x38 ,0xA0 ,0xDF ,0xE7 ,0x5F ,0x1E ,0x6D ,0x29 ,0x8E ,0x30 ,0xD7 ,0x83 ,0x8C ,0xB4 ,0x41 ,
    0xD2 ,0xFD ,0xBF ,0x5B ,0x18 ,0xCA ,0x50 ,0xD1 ,0x27 ,0xD1 ,0xF6 ,0x7D ,0x54 ,0x3E ,0x80 ,0x5F ,
    0x20 ,0xDC ,0x88 ,0x82 ,0xCF ,0xBE ,0xE1 ,0x46 ,0x2A ,0xD6 ,0x63 ,0xB9 ,0xB9 ,0x9D ,0xA3 ,0xC7 ,
    0x68 ,0x3E ,0x48 ,0xCE ,0x6A ,0x62 ,0x6F ,0xD1 ,0x6A ,0xC3 ,0xB6 ,0xDE ,0xF3 ,0x39 ,0x25 ,0xEC ,
    0xF6 ,0x79 ,0x20 ,0xB5 ,0xF2 ,0x30 ,0x25 ,0x6E ,0x99 ,0xAE ,0x39 ,0x56 ,0xDA ,0xAF ,0x83 ,0xD6 ,
    0xB8 ,0x49 ,0x15 ,0x78 ,0x81 ,0xCC ,0x3C ,0x4F ,0x66 ,0x5D ,0x95 ,0x7E ,0x31 ,0xD4 ,0x37 ,0x2A ,
    0xBE ,0xFC ,0xB4 ,0x66 ,0xF8 ,0x91 ,0x1  ,0xA  ,0x53 ,0x3C ,0x3C ,0xAB ,0x86 ,0xB9 ,0x80 ,0xB7 ,
    0x0  ,0x1  ,0x0  ,0x1,
    0x73 ,0xA1 ,0x79 ,0x78 ,0xCD ,0x6F ,0x8C ,0xE3 ,0x02 ,0x72 ,0x45 ,0x0A ,0xE9 ,0xC3 ,0x83 ,0x33 ,
    0x11 ,0x25 ,0xFB ,0x71 ,0x33 ,0x45 ,0xB0 ,0xF5 ,0xC6 ,0xD3 ,0xB0 ,0x6A ,0x84 ,0xF7 ,0x31 ,0x0A ,
    0xA3 ,0x52 ,0xDD ,0x23 ,0x93 ,0x38 ,0x07 ,0xF5 ,0x90 ,0x70 ,0xC4 ,0x73 ,0x2D ,0x48 ,0xD0 ,0xA9 ,
    0x2E ,0xDE ,0xCA ,0x21 ,0x1F ,0xEB ,0x5B ,0xA4 ,0x51 ,0x98 ,0x9A ,0x9B ,0x0C ,0x67 ,0xD3 ,0x10 ,
    0xFB ,0x3F ,0xF6 ,0x42 ,0xDA ,0x14 ,0x8E ,0x34 ,0x42 ,0x37 ,0x44 ,0x1E ,0xF0 ,0x32 ,0x57 ,0x8F ,
    0x49 ,0xCD ,0xA2 ,0xE9 ,0x99 ,0x30 ,0xDD ,0x6C ,0x27 ,0x9C ,0x91 ,0x05 ,0x62 ,0xD6 ,0x30 ,0x2C ,
    0xB7 ,0xF1 ,0x62 ,0x46 ,0x49 ,0x92 ,0x70 ,0x4C ,0x0C ,0x11 ,0xFB ,0x84 ,0xC2 ,0x60 ,0xF4 ,0x43 ,
    0xCA ,0x41 ,0xDE ,0xCF ,0x8C ,0x96 ,0x7D ,0xA6 ,0xD9 ,0xCD ,0x18 ,0x78 ,0xCC ,0x7A ,0x9F ,0x90 ,
    0x24 ,0x74 ,0xE2 ,0xA9 ,0x38 ,0x93 ,0x3D ,0x2B ,0x51 ,0xC8 ,0xEF ,0x06 ,0xD4 ,0x60 ,0x19 ,0xEA ,
    0x21 ,0x33 ,0x79 ,0xF6 ,0x48 ,0x93 ,0x8A ,0x95 ,0x3C ,0x12 ,0x8F ,0x45 ,0x8D ,0x42 ,0x1E ,0x0E ,
    0x86 ,0x21 ,0x44 ,0xBE ,0x86 ,0xFA ,0x87 ,0x7F ,0x68 ,0x40 ,0xF6 ,0xCF ,0xF3 ,0x6E ,0xAC ,0x3E ,
    0x64 ,0xAB ,0x68 ,0x2D ,0xAA ,0xE8 ,0xAE ,0x19 ,0x46 ,0x69 ,0xE2 ,0xB3 ,0xE2 ,0x00 ,0xE2 ,0xC6 ,
    0xE7 ,0x75 ,0x14 ,0x3C ,0xB1 ,0x3A ,0x45 ,0xD1 ,0x18 ,0x45 ,0x88 ,0x3F ,0x4B ,0xD0 ,0x6C ,0x64 ,
    0x20 ,0xBA ,0xBD ,0x38 ,0x42 ,0x69 ,0xAA ,0x08 ,0x7A ,0x3A ,0x98 ,0xDD ,0xF9 ,0x45 ,0x74 ,0xF5 ,
    0xD7 ,0x8E ,0x2B ,0xAD ,0x67 ,0x1D ,0x42 ,0x57 ,0x2E ,0x48 ,0x85 ,0x22 ,0xF0 ,0xC9 ,0x1F ,0x77 ,
    0x42 ,0xC2 ,0x28 ,0x5C ,0x8E ,0xE2 ,0x0C ,0x3D ,0xE3 ,0xB0 ,0x60 ,0xD1 ,0xAA ,0x99 ,0x87 ,0xE1
};


static const MS_U8 rsa_d_128[128] = {
#if 0
    0x1a, 0x15, 0xb2, 0x45, 0x65, 0x70, 0xf2, 0x6b, 0x34, 0x4a, 0xe9, 0xb5, 0x8d, 0x73, 0x45, 0x2a,
    0x8d, 0xac, 0x14, 0x7f, 0xc6, 0x03, 0xd3, 0x18, 0x8f, 0x67, 0xe3, 0xe8, 0xcf, 0xd4, 0xdd, 0x0d,
    0x4f, 0xa5, 0x63, 0x3d, 0x89, 0x74, 0xfa, 0x04, 0x33, 0x99, 0x44, 0xf5, 0xfb, 0x10, 0xeb, 0x65,
    0x84, 0xd0, 0x7d, 0x55, 0xd8, 0xf3, 0x27, 0xed, 0x3e, 0x44, 0xb9, 0xc8, 0x43, 0x29, 0x36, 0x51,
    0x93, 0x34, 0x65, 0x48, 0x4b, 0xe1, 0xf9, 0xe7, 0x43, 0x99, 0xcf, 0x81, 0x06, 0x6a, 0xd9, 0xb6,
    0x0f, 0x20, 0xd2, 0x9f, 0x9c, 0x52, 0x53, 0xea, 0x7c, 0xfe, 0x12, 0x0d, 0xf0, 0xf1, 0x7b, 0xff,
    0xea, 0x26, 0xf2, 0x73, 0xd3, 0x99, 0x68, 0x89, 0xdc, 0x0a, 0x5e, 0xe1, 0x5b, 0x72, 0xac, 0x6e,
    0xcd, 0x0e, 0x9e, 0x88, 0xd9, 0x57, 0x79, 0x69, 0x7f, 0x7d, 0x95, 0xd8, 0x07, 0xc4, 0xec, 0x61,
#else
/*
   0x4F,0x35,0xC9,0x74,0xDF,0xDA,0x7A,0xD6,0xA3,0x5C,0xB1,0x97,0x69,0xF6,0xCB,0xFE,
   0x63,0x54,0x6F,0x4E,0xAF,0x72,0x90,0x13,0xD6,0x97,0xCD,0x2A,0x5F,0x77,0x26,0x5C,
   0xFB,0xDE,0x9A,0x46,0x8D,0x6C,0x3D,0x60,0x26,0xD5,0xA8,0x61,0xEB,0x45,0x79,0xA4,
   0x0B,0x8F,0x74,0x83,0x76,0xFA,0xCF,0xF1,0x4F,0xEC,0x15,0x82,0x24,0x15,0x38,0x95,
   0xFB,0x6D,0x16,0x10,0x0D,0xE0,0x20,0x95,0xAB,0xF2,0x9B,0xA3,0xB8,0x4F,0xC8,0xC6,
   0xB6,0xE2,0x0E,0x83,0x15,0x42,0x48,0xCF,0x5A,0xF9,0x85,0x04,0xE3,0xFE,0xC8,0x7C,
   0x95,0xD3,0x50,0x9D,0x1A,0x3F,0x6C,0xD7,0x14,0x99,0xF8,0x65,0x95,0x7F,0x50,0xC1,
   0xE8,0x76,0x3A,0xA9,0x3E,0x27,0xFD,0x36,0xD7,0x4D,0x53,0x57,0xC9,0xF9,0x8D,0xCF,
*/
/*
   0x41,0x8D,0x5B,0xFC,0x45,0x8D,0x67,0x21,0x92,0xBD,0x94,0x4F,0xBA,0x37,0x69,0xBD,
   0x66,0x90,0xAD,0x25,0xD5,0x53,0x81,0x1B,0xF9,0x53,0xAD,0xD1,0x68,0x5E,0x82,0x13,
   0x7B,0x2E,0x02,0x41,0x3D,0x9F,0x3B,0xE4,0xAE,0x8B,0x7D,0xF2,0xA5,0x6D,0xDA,0x08,
   0xCD,0x08,0x3A,0x7C,0x70,0x28,0xA0,0x10,0x33,0x9D,0x16,0xAF,0xCF,0xBD,0xC1,0x3A,
   0x2B,0x54,0xBB,0x68,0x21,0x4E,0x1D,0x5E,0xA0,0x03,0xB7,0x67,0xEB,0xDB,0xFC,0x46,
   0x9A,0x75,0x76,0xCC,0x8E,0x19,0xDC,0xD5,0x00,0xD1,0x3A,0x54,0x20,0x1B,0x90,0x07,
   0x3F,0x73,0xF5,0xFE,0x3A,0xF2,0xCA,0x6E,0x41,0xE5,0x45,0x41,0xAB,0x5A,0x82,0xEB,
   0x92,0x01,0xDE,0xB1,0x5A,0x92,0xFD,0x44,0xCC,0x20,0x0E,0xE1,0x0E,0x43,0xF7,0xBB,
*/
/*
    0xb7,0xdf,0xab,0x06,0xe2,0x4e,0xf1,0xd8,0x81,0x46,0x5a,0xfa,0xcc,0xdc,0xe5,
    0x72,0x96,0x26,0x2a,0x69,0xc8,0x28,0x9b,0x35,0xf0,0x7d,0xec,0xa1,0x05,0xee,
    0x63,0xd7,0xde,0x34,0x55,0x44,0x3e,0x08,0x37,0xf2,0x02,0x67,0xff,0x5a,0x68,
    0x18,0x58,0xaa,0xfe,0xdb,0xd3,0xe2,0xaa,0xdf,0xa8,0xc1,0x2b,0x4b,0xf6,0x67,
    0x59,0x0d,0xab,0xa2,0xde,0xe2,0xc5,0x69,0x36,0xe5,0x4a,0xd8,0x3e,0x2d,0xb3,
    0xdc,0xcc,0x89,0x14,0x98,0xd4,0xd8,0xff,0x5c,0xc7,0x9e,0x5b,0x09,0x3d,0x4e,
    0x9e,0xe6,0xf9,0xea,0x29,0x22,0xd4,0xed,0x09,0xa7,0xeb,0xbb,0x2c,0xd6,0x21,
    0x25,0xb9,0xb0,0x31,0x11,0x0e,0x72,0x7c,0x9a,0x2d,0x0b,0x1d,0x97,0x7a,0xa1,
    0x09,0x6a,0x30,0x12,0x76,0xcf,0xb4,0x81,
  */
0x9b,0x15,0x1b,0x15,0xcc,0x4c,0x1d,0x1c,0x5c,0x22,0x4f,0xed,0xc0,0x58,0xdd,
0xf9,0x1c,0x76,0x43,0x3a,0x3b,0xff,0xe9,0x01,0x67,0x30,0xc2,0x01,0xe3,0x21,
0x46,0xbe,0x2d,0x54,0x9c,0x65,0xa0,0x4e,0xde,0x4a,0xb7,0xa9,0xe7,0x6a,0x5d,
0xe8,0xb6,0xc8,0x41,0x6d,0xa1,0x32,0x60,0x78,0xf4,0x0b,0x56,0x9c,0xe6,0x68,
0xfa,0x1c,0x57,0x6e,0xbc,0xb5,0x76,0x76,0xa7,0xe5,0x72,0x37,0x08,0xdd,0x36,
0x9c,0x35,0xd8,0xde,0xa9,0xf0,0xf8,0xce,0xd5,0xe3,0xca,0xea,0xe0,0xfc,0x10,
0x3a,0x42,0x2c,0xcd,0x4c,0x98,0xc9,0xdb,0x5f,0x57,0x76,0x48,0xe0,0xb6,0x8c,
0x65,0x5e,0xdc,0x1f,0x22,0xa5,0xdb,0xd9,0xf4,0x62,0x64,0x76,0x52,0x5f,0x55,
0xe3,0xe7,0xd4,0x42,0x12,0x55,0x2e,0x0f
#endif
};

static const MS_U8 rsa_e_128[4] = {
#if 0
    0x00, 0x01, 0x00, 0x01
#else
// 0x00, 0x01, 0x37, 0x4B
//00, 0x02, 0x68, 0xEB
     0x00,0x01,0x4D,0x0B
#endif
};

static const MS_U8 rsa_n_128[128] = {
#if 0
    0x8e, 0xbe, 0xc4, 0x8e, 0x1a, 0x21, 0x00, 0x6e, 0x45, 0xe6, 0xfd, 0x71, 0xdf, 0xb2, 0x6b, 0x01,
    0x41, 0xfa, 0xea, 0x89, 0x08, 0x1d, 0x44, 0x27, 0xba, 0x3c, 0xf3, 0x30, 0x79, 0xb5, 0x5f, 0xa3,
    0xed, 0x3f, 0x29, 0xf5, 0xab, 0x3b, 0xc1, 0x9b, 0x65, 0x5a, 0xa5, 0x85, 0x37, 0x62, 0x20, 0x49,
    0x89, 0xca, 0x59, 0x66, 0xe9, 0x1b, 0x35, 0x88, 0xc4, 0xb3, 0xbb, 0x31, 0x3b, 0x86, 0x0b, 0x14,
    0x81, 0x86, 0xee, 0xac, 0x29, 0xd6, 0x79, 0x0e, 0x50, 0x5d, 0x36, 0x41, 0x3c, 0x2e, 0x4a, 0x4f,
    0xb4, 0x33, 0x69, 0xd5, 0x94, 0x00, 0x72, 0x67, 0x9c, 0x82, 0x76, 0x9d, 0x46, 0xc9, 0xe3, 0x96,
    0xbe, 0x38, 0xe0, 0x10, 0xa5, 0x3b, 0x86, 0xe7, 0x6d, 0xf3, 0x19, 0xae, 0x11, 0xb2, 0x55, 0x54,
    0xaf, 0xb0, 0xf2, 0x3e, 0xb4, 0x24, 0xab, 0x5d, 0x78, 0x53, 0xdf, 0x4b, 0xb5, 0x04, 0xb7, 0x05,
 #else
 /*
    0x73,0x04,0xBF,0x0A,0x4F,0x4A,0x1E,0x17,0x22,0x2F,0x45,0x5F,0x53,0xB3,0x9D,0xFC,
    0x58,0x06,0x5D,0xF5,0xF2,0xD7,0xFF,0xB3,0xF2,0x09,0x0F,0xE3,0xF6,0x72,0x0C,0x51,
    0xFB,0x68,0x06,0x3E,0xDA,0x82,0x8C,0xED,0x54,0x3E,0x3A,0x1B,0xD6,0x14,0x97,0x1E,
    0xB4,0x00,0x8C,0x66,0x74,0x51,0xEB,0xBB,0x85,0xAA,0x8D,0xF5,0xF7,0x58,0x15,0x0A,
    0xAF,0x8C,0xE8,0xE1,0xBD,0x2A,0x27,0xD1,0x38,0xA8,0x13,0xED,0xC9,0x8F,0x32,0x6B,
    0xE2,0x17,0xF2,0xC9,0x79,0xF7,0xBA,0x9D,0x0A,0x48,0xA0,0x2B,0x0A,0x3B,0x1D,0xD5,
    0xA1,0x82,0x49,0xE4,0xA4,0x8A,0x53,0x8B,0x44,0x10,0x1D,0x8D,0x2A,0x3A,0x8A,0x6C,
    0xA1,0xA9,0xA5,0xB9,0x38,0x8C,0x54,0x19,0x8A,0x97,0x16,0x27,0x2B,0xF5,0x14,0xB9,
    */
 /*
    0x87,0x63,0xCE,0xC7,0xE5,0x53,0x06,0x37,0x18,0x92,0xB8,0x99,0x75,0x52,0x37,0x19,
    0x57,0xA8,0xA8,0x86,0xB3,0xDF,0xF2,0xC7,0x52,0x7F,0x06,0xA6,0x81,0x5E,0xB9,0x75,
    0xF9,0x58,0xD5,0xD0,0x39,0xE3,0x32,0xF4,0xAC,0x7F,0x16,0xF9,0x90,0x90,0x0A,0x98,
    0x04,0xED,0x9A,0xB2,0x4D,0x6E,0xB5,0xA9,0xC1,0x51,0x92,0xF6,0x48,0x2E,0x7B,0x8E,
    0xBF,0xCB,0x41,0xE6,0xDA,0x96,0x5D,0x29,0x1A,0x81,0xA6,0x08,0xEC,0x8F,0xD3,0x38,
    0xA1,0xE5,0x44,0x4C,0x06,0x5A,0x6D,0x1A,0x53,0x1B,0x11,0xE7,0x0E,0x4A,0x97,0x06,
    0xCC,0x4C,0x93,0x27,0x79,0x28,0x01,0x44,0xF8,0x3B,0xC6,0xBE,0x37,0x98,0x3B,0xE3,
    0x36,0x23,0x35,0x34,0x2B,0xAD,0xB6,0xBD,0x8E,0xFA,0x0D,0x16,0x4F,0x8A,0x13,0xB1,
 */
 /*
    0x9b,0x15,0x1b,0x15,0xcc,0x4c,0x1d,0x1c,0x5c,0x22,0x4f,0xed,0xc0,0x58,0xdd,
    0xf9,0x1c,0x76,0x43,0x3a,0x3b,0xff,0xe9,0x01,0x67,0x30,0xc2,0x01,0xe3,0x21,
    0x46,0xbe,0x2d,0x54,0x9c,0x65,0xa0,0x4e,0xde,0x4a,0xb7,0xa9,0xe7,0x6a,0x5d,
    0xe8,0xb6,0xc8,0x41,0x6d,0xa1,0x32,0x60,0x78,0xf4,0x0b,0x56,0x9c,0xe6,0x68,
    0xfa,0x1c,0x57,0x6e,0xbc,0xb5,0x76,0x76,0xa7,0xe5,0x72,0x37,0x08,0xdd,0x36,
    0x9c,0x35,0xd8,0xde,0xa9,0xf0,0xf8,0xce,0xd5,0xe3,0xca,0xea,0xe0,0xfc,0x10,
    0x3a,0x42,0x2c,0xcd,0x4c,0x98,0xc9,0xdb,0x5f,0x57,0x76,0x48,0xe0,0xb6,0x8c,
    0x65,0x5e,0xdc,0x1f,0x22,0xa5,0xdb,0xd9,0xf4,0x62,0x64,0x76,0x52,0x5f,0x55,
    0xe3,0xe7,0xd4,0x42,0x12,0x55,0x2e,0x0f,*/
0xb7,0xdf,0xab,0x06,0xe2,0x4e,0xf1,0xd8,0x81,0x46,0x5a,0xfa,0xcc,0xdc,0xe5,
0x72,0x96,0x26,0x2a,0x69,0xc8,0x28,0x9b,0x35,0xf0,0x7d,0xec,0xa1,0x05,0xee,
0x63,0xd7,0xde,0x34,0x55,0x44,0x3e,0x08,0x37,0xf2,0x02,0x67,0xff,0x5a,0x68,
0x18,0x58,0xaa,0xfe,0xdb,0xd3,0xe2,0xaa,0xdf,0xa8,0xc1,0x2b,0x4b,0xf6,0x67,
0x59,0x0d,0xab,0xa2,0xde,0xe2,0xc5,0x69,0x36,0xe5,0x4a,0xd8,0x3e,0x2d,0xb3,
0xdc,0xcc,0x89,0x14,0x98,0xd4,0xd8,0xff,0x5c,0xc7,0x9e,0x5b,0x09,0x3d,0x4e,
0x9e,0xe6,0xf9,0xea,0x29,0x22,0xd4,0xed,0x09,0xa7,0xeb,0xbb,0x2c,0xd6,0x21,
0x25,0xb9,0xb0,0x31,0x11,0x0e,0x72,0x7c,0x9a,0x2d,0x0b,0x1d,0x97,0x7a,0xa1,
0x09,0x6a,0x30,0x12,0x76,0xcf,0xb4,0x81
 #endif
};

static const MS_U8 u8Signature[] = {
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05};
static const MS_U8 u8Signaturebackup[]= {
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05};
		
unsigned char KeyPackage_Session_rsa_n_128[128] = {0};
unsigned char KeyPackage_Session_rsa_e_128[4] = {0};

static int SwapKey(MS_U64 u32addr,MS_U64 u32len)
{
    MS_U64 i = 0,j = 0;
    MS_U64 num = u32len/(2*4); 
    MS_U8 temp = 0;
    for (i = 0;i < num;i++) {
        for (j = 0;j < 4;j++) {
           temp = ((unsigned char *)u32addr)[i*4+j];
           ((unsigned char *)u32addr)[i*4+j] = ((unsigned char *)u32addr)[u32len-(i+1)*4+j];
           ((unsigned char *)u32addr)[u32len-(i+1)*4+j] = temp;
        }
    }
    return 0;
}

static int SwapArray(MS_U64 u32addr,MS_U64 u32len)
{
    MS_U64 i;
    MS_U8 temp;
    if (0 == u32addr || 0 == u32len ) {
        printf("u32addr and u32len can not be 0");
        return -1;
    }
    i = 0;
    temp = 0;
    for (i = 0;i < u32len/2;i++) {
        temp = ((unsigned char *)u32addr)[i];
       ((unsigned char *)u32addr)[i] = ((unsigned char *)u32addr)[u32len-1-i];
       ((unsigned char *)u32addr)[u32len-1-i] = temp;
    }
    return 0;
}


static void mem_dump(uint8_t *addr, MS_U32 length) 
{
    uint8_t *p = addr;
    MS_U32 i;
    for (i = 0; i < length; i++) {
        printf("%02X", *p);
        p++;
        if ((i+1)%16 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
}



int RSA2048HWencrypt(unsigned char *Signature, unsigned char *PublicKey_N, unsigned char *PublicKey_D, unsigned char *Sim_SignOut)
{
   printf("IN\n");
   DRVAESDMA_RESULT result;
   DrvAESDMA_RSASig RSASig;
   DrvAESDMA_RSAOut RSAOut;
   DrvAESDMA_RSAKey RSAKey;
   result = DRVAESDMA_OK;
   //MDrv_AESDMA_Init(0x00, 0x20000000, 2); // wait the AESDMA.a
   MDrv_CIPHER_Init();
   memset(&RSASig,0,sizeof(RSASig));
   memset(&RSAOut,0,sizeof(RSAOut));
   SwapArray((MS_U64)Signature,SHA256_DIGEST_SIZE);
   memcpy((char*)&RSASig+SIGNATURE_LEN-SHA256_DIGEST_SIZE,(char*)Signature,SHA256_DIGEST_SIZE);
   memset(&RSAKey,0,sizeof(RSAKey));
   memcpy((char*)(RSAKey.u32KeyN),(char*)PublicKey_N,RSA_PUBLIC_KEY_N_LEN);
   memcpy((char*)(RSAKey.u32KeyE),(char*)PublicKey_D,RSA_PUBLIC_KEY_N_LEN);
   // little and big endian change,4 bytes one time
   SwapKey((MS_U64)(RSAKey.u32KeyE),RSA_PUBLIC_KEY_N_LEN);

   result=MDrv_RSA_Calculate(&RSASig,&RSAKey,E_DRVAESDMA_RSA2048_PRIVATE);
   if (DRVAESDMA_OK != result) {
      printf("RSA HW encrypt error1\n");
      return -1;
   }
   while(DRVAESDMA_OK != MDrv_RSA_IsFinished());
   result =  MDrv_RSA_Output(E_DRVAESDMA_RSA2048_PRIVATE,&RSAOut);
   if (DRVAESDMA_OK != result) {
      printf("RSA HW encrypt error2\n");
      return -1;
   }
   SwapArray((MS_U64)&RSAOut,SIGNATURE_LEN);
   memcpy((char*)Sim_SignOut,(char*)&RSAOut,sizeof(RSAOut));
   printf("OK\n");
   return 0;
}

int RSA2048HWdecrypt(unsigned char *Signature, unsigned char *PublicKey_N, unsigned char *PublicKey_E, unsigned char *Sim_SignOut)
{
   printf("IN\n");
   if (NULL == Signature || NULL == PublicKey_N || NULL == PublicKey_E || NULL == Sim_SignOut) {
       printf("parameters Error\n");
       return -1;
   }
   DRVAESDMA_RESULT result = DRVAESDMA_OK;
   DrvAESDMA_RSASig RSASig;
   DrvAESDMA_RSAOut RSAOut;
   MDrv_CIPHER_Init();
   memcpy(&RSASig,Signature,sizeof(RSASig));
   memset(&RSAOut,0,sizeof(RSAOut));
   DrvAESDMA_RSAKey RSAKey;
   memset(&RSAKey,0,sizeof(RSAKey));
   memcpy((unsigned char*)(RSAKey.u32KeyN),PublicKey_N,RSA_PUBLIC_KEY_N_LEN);
   memcpy((unsigned char*)(RSAKey.u32KeyE),PublicKey_E,RSA_PUBLIC_KEY_E_LEN);
   
   result=MDrv_RSA_Calculate(&RSASig,&RSAKey,E_DRVAESDMA_RSA2048_PUBLIC);
   if (DRVAESDMA_OK != result) {
      printf("RSA HW decrypt error1\n");
      return -1;
   }
   memset((MS_U8*)&RSAOut,0,sizeof(DrvAESDMA_RSAOut));
   while(DRVAESDMA_OK != MDrv_RSA_IsFinished());
   result= MDrv_RSA_Output(E_DRVAESDMA_RSA2048_PUBLIC, &RSAOut);
   if (DRVAESDMA_OK != result) {
      printf("RSA HW decrypt error2\n");
      return -1;
   }
   memcpy(Sim_SignOut,&RSAOut,sizeof(RSAOut));
   printf("OK\n");
   return 0;
}

void RSA1024_Swap(MS_U32 *pu32RSAOut, MS_U32 u32Size)
{
    MS_U32 u32TmpArray[64]= {0}, u32i;

    memset( u32TmpArray, 0, sizeof(u32TmpArray));
    for( u32i=0 ; u32i<u32Size && u32i<64 ; u32i++ )
    {
        u32TmpArray[(u32Size-1)-u32i]= pu32RSAOut[u32i];
    }
    memcpy( pu32RSAOut, u32TmpArray, u32i*sizeof(MS_U32));
}

void DumpBuf1024(const MS_U8 *buf, MS_U32 len)
{
    MS_U32 i = 0;
    for (i = 0; i < len; i++) {
      //  printf("0x%02x%c", buf[i], ((i % 16) == 15) ? '\n' : ' ');
    }
  //  printf("\n");
}

void RSA1024_ByteSwap(MS_U32 *pu32RSAOut)
{
    MS_U8 *pu8Data;
    MS_U32 u32i;

    for( u32i=0 ; u32i<64 ; u32i++ )
    {
        pu8Data = (MS_U8 *)(&pu32RSAOut[u32i]);
        pu32RSAOut[u32i] = ((MS_U32) pu8Data[0])<<24 |((MS_U32) pu8Data[1])<<16 | ((MS_U32) pu8Data[2])<<8 |(MS_U32) pu8Data[3];
    }
}

char FromStrToChar(char *str)
{
    if(NULL == str)
        return '\0';
 
    int a[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
 
    unsigned int i,sum = 0;
 
    for(i = 0; i < 2; ++ i)
    {
        if(str[i] >= '0' && str[i] <= '9')
        {
            sum += a[str[i] - '0'] * (16/pow(16,i));
        }
        else
        {
            sum += a[str[i] - 'A' + 10] * (16/pow(16,i));
        }
    }
         
    return (char)sum;
 
}

void StringConvert(char *str)
{
    unsigned int len = strlen(str);
    if(len & 1)
    {
        *str = '\0';
        return ;
    }
 
    unsigned int i, nCount = 0;
    char a[3] = {0};
 
    for(i = 0; i < len / 2; ++ i)
    {       
        a[0] = str[i*2];
        a[1] = str[i*2 + 1];
        a[2] = '\0';
 
        str[nCount ++] = FromStrToChar(a);
    }
    str[nCount] = '\0';
}

int RSA1024_Routine(const MS_U8 *pu8RSAIn, MS_U32 u32RSAInLen, const MS_U8 *pu8KeyE, MS_U32 u32KeyELen, const MS_U8 *pu8KeyN, MS_U32 u32KeyNLen, MS_U8 *pu8RSAOut, DrvAESDMA_RSAMode eMode)
{
    DrvAESDMA_RSASig stSig;
    DrvAESDMA_RSAKey stKey;
    DrvAESDMA_RSAOut stRSAOut; //= (DrvAESDMA_RSAOut *) pu8RSAOut;
    MS_U32 u32OffsetE, u32OffsetN, u32OffsetA, u32KeyLen= 0;

    memset(&stSig, 0, sizeof(DrvAESDMA_RSASig));
    memset(&stKey, 0, sizeof(DrvAESDMA_RSAKey));
    memset(&stRSAOut, 0, sizeof(DrvAESDMA_RSAOut));

    switch (eMode)
    {
    case E_DRVAESDMA_RSA1024_PUBLIC:
    case E_DRVAESDMA_RSA1024_PRIVATE:
        u32KeyLen = 128;
        break;
    case E_DRVAESDMA_RSA2048_PUBLIC:
    case E_DRVAESDMA_RSA2048_PRIVATE:
        u32KeyLen = 256;
        break;
    case E_DRVAESDMA_RSA256_PUBLIC:
    case E_DRVAESDMA_RSA256_PRIVATE:
        u32KeyLen = 32;
        break;
    default:
        return DRVAESDMA_INVALID_PARAM;
    }
    /// LSB Input
    u32OffsetE = (sizeof(stKey.u32KeyE)-u32KeyELen) / sizeof(MS_U32);
    memcpy( &(stKey.u32KeyE[u32OffsetE]), pu8KeyE, u32KeyELen);
    RSA1024_Swap( stKey.u32KeyE, 64);

    /// MSB Input
    u32OffsetN = (sizeof(stKey.u32KeyN)-u32KeyNLen) / sizeof(MS_U32);
    memcpy( &stKey.u32KeyN[u32OffsetN], pu8KeyN, u32KeyNLen);

    /// MSB Input
    u32OffsetA = (sizeof(stSig.u32Sig)-u32RSAInLen) / sizeof(MS_U32);
    memcpy( &stSig.u32Sig[u32OffsetA], pu8RSAIn , u32RSAInLen);

    printf("=======================RSAKey.u32KeyN==========================\n");
    //DumpBuf1024((unsigned char *)stKey.u32KeyN, 256);
    printf("=======================RSAKey.u32KeyE==========================\n");
    //DumpBuf1024((unsigned char *)stKey.u32KeyE, 256);
    printf("=======================RSASig=================================\n");
    //DumpBuf1024((unsigned char *)stSig.u32Sig, 256);

    if( DRVAESDMA_OK!=MDrv_RSA_Calculate(&stSig, &stKey, eMode) )
    {
        printf("Error: MDrv_RSA_Calculate failed\n");		
        return -1;
    }
    while(MDrv_RSA_IsFinished()!=DRVAESDMA_OK)
    {
        printf("..");
    }

    MDrv_RSA_Output(eMode, &stRSAOut);

    printf("1=======================RSAout==========================\n");

    //DumpBuf1024((unsigned char *)stRSAOut.u32RSAOut, 256);
    RSA1024_Swap(&stRSAOut.u32RSAOut[0], (u32KeyLen/4));
    RSA1024_ByteSwap(&stRSAOut.u32RSAOut[0]);

    memcpy(pu8RSAOut, &stRSAOut.u32RSAOut[0],u32KeyNLen);
    return 0;
}

int terminalEncrypt(char* authinput,int inputlen, char* outbuf) 
{
#if 1   //RSA1024
{
    printf("\nterminalEncrypt RSA_128 authinput==>\n");
    DumpBuf1024((unsigned char *)authinput, 256);

    int ret = -1;

    ret = RSA1024_Routine((uint8_t *) authinput, 128, rsa_d_128, 128, rsa_n_128, 128, (uint8_t *)outbuf, E_DRVAESDMA_RSA1024_PRIVATE);
    if(ret == -1)
    {
        printf("RSA Decryption Fail!!!\n");
        return -1;
    }
    printf(" Encrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outbuf, 128);
    return 0;
}
#else    //RSA2048
     int ret = -1;
    // RSA encrypt
    printf("==>2048 terminalEncrypt DATA authinput:\n");
    mem_dump((uint8_t *)authinput, inputlen);
    ret = RSA2048HWencrypt((uint8_t *)authinput,u8RSA_Decrypt_key2048,u8RSA_Decrypt_key2048+RSA_PUBLIC_KEY_LEN,(uint8_t *)outbuf);
    if (0 != ret) 
    {
        printf("2048 RSA HW encrypt error\n");
        return -1;
    } 
     printf("==>2048 terminalEncrypt DATA outbuf:\n");
    mem_dump((uint8_t *)outbuf, inputlen);

    return 0;
#endif
}

int RSA_Decrypt(char* inputbuf, int inputlen, char* outputbuf) 
{
#if 1   //RSA1024
{
     printf("\nRSA_Decrypt RSA_128 inputbuf==>\n");
    DumpBuf1024((unsigned char *)inputbuf, 128);

    int ret = -1;
    ret = RSA1024_Routine( (uint8_t *)inputbuf, 128, rsa_e_128, 4, rsa_n_128, 128, (uint8_t *)outputbuf, E_DRVAESDMA_RSA1024_PUBLIC);
    if(ret == -1)
    {
        printf("RSA Encryption Fail!!!\n");
        return -1;
    }
    printf("Decrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outputbuf, 256);

    return 0;
}
#else   //RSA2048
    int ret = -1;
    // RSA decrypt 
    ret = RSA2048HWdecrypt((uint8_t *)inputbuf,u8RSA_Decrypt_key2048,u8RSA_Decrypt_key2048+RSA_PUBLIC_KEY_N_LEN,(uint8_t *)outputbuf);
    if (0 != ret) {
        printf("2048 RSA HW decrypt error\n");
        return -1;
    }
    printf("2048 RSA_DecryptHW outputbuf:\n");
    mem_dump((uint8_t *)outputbuf, inputlen);
    	
    return 0;
#endif
}


static const MS_U8 EKpri_rsa_d_128[128] = {
   0x4F,0x35,0xC9,0x74,0xDF,0xDA,0x7A,0xD6,0xA3,0x5C,0xB1,0x97,0x69,0xF6,0xCB,0xFE,
   0x63,0x54,0x6F,0x4E,0xAF,0x72,0x90,0x13,0xD6,0x97,0xCD,0x2A,0x5F,0x77,0x26,0x5C,
   0xFB,0xDE,0x9A,0x46,0x8D,0x6C,0x3D,0x60,0x26,0xD5,0xA8,0x61,0xEB,0x45,0x79,0xA4,
   0x0B,0x8F,0x74,0x83,0x76,0xFA,0xCF,0xF1,0x4F,0xEC,0x15,0x82,0x24,0x15,0x38,0x95,
   0xFB,0x6D,0x16,0x10,0x0D,0xE0,0x20,0x95,0xAB,0xF2,0x9B,0xA3,0xB8,0x4F,0xC8,0xC6,
   0xB6,0xE2,0x0E,0x83,0x15,0x42,0x48,0xCF,0x5A,0xF9,0x85,0x04,0xE3,0xFE,0xC8,0x7C,
   0x95,0xD3,0x50,0x9D,0x1A,0x3F,0x6C,0xD7,0x14,0x99,0xF8,0x65,0x95,0x7F,0x50,0xC1,
   0xE8,0x76,0x3A,0xA9,0x3E,0x27,0xFD,0x36,0xD7,0x4D,0x53,0x57,0xC9,0xF9,0x8D,0xCF,
};

static const MS_U8 EKpri_rsa_e_128[4] = {
   0x00,0x01,0x37,0x4B
};

static const MS_U8 EKpri_rsa_n_128[128] = {
    0x73,0x04,0xBF,0x0A,0x4F,0x4A,0x1E,0x17,0x22,0x2F,0x45,0x5F,0x53,0xB3,0x9D,0xFC,
    0x58,0x06,0x5D,0xF5,0xF2,0xD7,0xFF,0xB3,0xF2,0x09,0x0F,0xE3,0xF6,0x72,0x0C,0x51,
    0xFB,0x68,0x06,0x3E,0xDA,0x82,0x8C,0xED,0x54,0x3E,0x3A,0x1B,0xD6,0x14,0x97,0x1E,
    0xB4,0x00,0x8C,0x66,0x74,0x51,0xEB,0xBB,0x85,0xAA,0x8D,0xF5,0xF7,0x58,0x15,0x0A,
    0xAF,0x8C,0xE8,0xE1,0xBD,0x2A,0x27,0xD1,0x38,0xA8,0x13,0xED,0xC9,0x8F,0x32,0x6B,
    0xE2,0x17,0xF2,0xC9,0x79,0xF7,0xBA,0x9D,0x0A,0x48,0xA0,0x2B,0x0A,0x3B,0x1D,0xD5,
    0xA1,0x82,0x49,0xE4,0xA4,0x8A,0x53,0x8B,0x44,0x10,0x1D,0x8D,0x2A,0x3A,0x8A,0x6C,
    0xA1,0xA9,0xA5,0xB9,0x38,0x8C,0x54,0x19,0x8A,0x97,0x16,0x27,0x2B,0xF5,0x14,0xB9,
};

int EKpri_Hash_terminalEncrypt(char* authinput,int inputlen, char* outbuf) 
{
#if 1   //RSA1024
{
    MS_U8 authinput_buff[256]= {0};
    memset(authinput_buff,  0,  256*sizeof(MS_U8));	

    memcpy(authinput_buff, authinput,inputlen);
	
    LOGI("\nterminalEncrypt RSA_128 authinput==>\n");
    DumpBuf1024((unsigned char *)authinput_buff, inputlen);

    int ret = -1;

    ret = RSA1024_Routine((uint8_t *) authinput_buff, 128, EKpri_rsa_d_128, 128, EKpri_rsa_n_128, 128, (uint8_t *)outbuf, E_DRVAESDMA_RSA1024_PRIVATE);
    if(ret == -1)
    {
        LOGI("RSA Decryption Fail!!!\n");
        return -1;
    }
    LOGI(" Encrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outbuf, 128);
{
    MS_U32 i = 0;
    for (i = 0; i < 128; i++) {
        LOGI("0x%02x", outbuf[i]);
    }
}	
	
    //strcpy(outbuf, "A9D27B6575F99C78716732F7BD631B9512781AB2D595FFA472F14D63FC1A1F1F2508D34B6B6F0B5437285D7880BCA0E1274923B0F27F27281987EE0E986C1C3A17D113EB139074BA66B38AC1F276E3AB8E41EC24391BA7E082D51FD0553D68968A62FB7EDB7A04630EA82633A5E9971F8D6A17D0AC2FDA1372A9D01539649BD1");	
    return 0;
}
#else    //RSA2048
     int ret = -1;
    // RSA encrypt
    printf("==>2048 terminalEncrypt DATA authinput:\n");
    mem_dump((uint8_t *)authinput, inputlen);
    ret = RSA2048HWencrypt((uint8_t *)authinput,u8RSA_Decrypt_key2048,u8RSA_Decrypt_key2048+RSA_PUBLIC_KEY_LEN,(uint8_t *)outbuf);
    if (0 != ret) 
    {
        printf("2048 RSA HW encrypt error\n");
        return -1;
    } 
     printf("==>2048 terminalEncrypt DATA outbuf:\n");
    mem_dump((uint8_t *)outbuf, inputlen);

    return 0;
#endif
}

int EKpri_Hash_RSA_Decrypt(char* inputbuf, int inputlen, char* outputbuf) 
{
#if 1   //RSA1024
{
     printf("\nRSA_Decrypt RSA_128 inputbuf==>\n");
    DumpBuf1024((unsigned char *)inputbuf, 128);

    int ret = -1;
    ret = RSA1024_Routine( (uint8_t *)inputbuf, 128, EKpri_rsa_e_128, 4, EKpri_rsa_n_128, 128, (uint8_t *)outputbuf, E_DRVAESDMA_RSA1024_PUBLIC);
    if(ret == -1)
    {
        printf("RSA Encryption Fail!!!\n");
        return -1;
    }
    printf("Decrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outputbuf, 256);

    return 0;
}
#else   //RSA2048
    int ret = -1;
    // RSA decrypt 
    ret = RSA2048HWdecrypt((uint8_t *)inputbuf,u8RSA_Decrypt_key2048,u8RSA_Decrypt_key2048+RSA_PUBLIC_KEY_N_LEN,(uint8_t *)outputbuf);
    if (0 != ret) {
        printf("2048 RSA HW decrypt error\n");
        return -1;
    }
    printf("2048 RSA_DecryptHW outputbuf:\n");
    mem_dump((uint8_t *)outputbuf, inputlen);
    	
    return 0;
#endif
}

int Hex_Factory_IN_Certify_Hash_RSA_Decrypt(char* inputbuf, int inputlen, char* outputbuf)
{
     StringConvert((char *)inputbuf);  
     LOGI("\nHex_Factory_IN_Certify_Hash_RSA_Decrypt inputbuf==>\n");
     DumpBuf1024((unsigned char *)inputbuf, 128);

    int ret = -1;
   ret = RSA1024_Routine( (uint8_t *)inputbuf, 128, EKpri_rsa_d_128, 4, EKpri_rsa_n_128, 128, (uint8_t *)outputbuf, E_DRVAESDMA_RSA1024_PUBLIC);
    if(ret == -1)
    {
        LOGI("RSA Encryption Fail!!!\n");
        return -1;
    }
    LOGI("Decrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outputbuf, 256);

    return 0;
}


int Hex_EKpri_Hash_RSA_Decrypt(char* inputbuf, int inputlen, char* outputbuf) 
{
#if 1   //RSA1024
{
     StringConvert((char *)inputbuf);  
     LOGI("\nRSA_Decrypt RSA_128 inputbuf==>\n");
     DumpBuf1024((unsigned char *)inputbuf, 128);

    int ret = -1;
   ret = RSA1024_Routine( (uint8_t *)inputbuf, 128, EKpri_rsa_e_128, 4, EKpri_rsa_n_128, 128, (uint8_t *)outputbuf, E_DRVAESDMA_RSA1024_PUBLIC);
    if(ret == -1)
    {
        LOGI("RSA Encryption Fail!!!\n");
        return -1;
    }
    LOGI("Decrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outputbuf, 256);

    return 0;
}
#else   //RSA2048
    int ret = -1;
    // RSA decrypt 
    ret = RSA2048HWdecrypt((uint8_t *)inputbuf,u8RSA_Decrypt_key2048,u8RSA_Decrypt_key2048+RSA_PUBLIC_KEY_N_LEN,(uint8_t *)outputbuf);
    if (0 != ret) {
        printf("2048 RSA HW decrypt error\n");
        return -1;
    }
    printf("2048 RSA_DecryptHW outputbuf:\n");
    mem_dump((uint8_t *)outputbuf, inputlen);
    	
    return 0;
#endif
}


int EKpri_Hash_sessionEncrypt(char* authinput,int inputlen, char* outbuf) 
{
    MS_U8 authinput_buff[256]= {0};
    memset(authinput_buff,  0,  256*sizeof(MS_U8));	

    memcpy(authinput_buff, authinput,inputlen);
	
    LOGI("\nterminalEncrypt RSA_128 authinput==>\n");
    DumpBuf1024((unsigned char *)authinput_buff, inputlen);

    int ret = -1;

    //ret = RSA1024_Routine((uint8_t *) authinput_buff, inputlen, rsa_e_128, 4, rsa_n_128, 128, (uint8_t *)outbuf, E_DRVAESDMA_RSA1024_PUBLIC);
    ret = RSA1024_Routine((uint8_t *) authinput_buff, inputlen, KeyPackage_Session_rsa_e_128, 4, KeyPackage_Session_rsa_n_128, 128, (uint8_t *)outbuf, E_DRVAESDMA_RSA1024_PUBLIC);
    if(ret == -1)
    {
        LOGI("RSA Decryption Fail!!!\n");
        return -1;
    }
    LOGI(" Encrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outbuf, 128);
	
    MS_U32 i = 0;
    for (i = 0; i < 128; i++) 
        LOGI("0x%02x", outbuf[i]);
	
    return 0;
}

int EKpri_Hash_sessionDecrypt(char* inputbuf, int inputlen, char* outputbuf) 
{
    // StringConvert((char *)inputbuf);  
     printf("\EKpri_Hash_sessionDecrypt  RSA_128 inputbuf==>\n");
     DumpBuf1024((unsigned char *)inputbuf, 128);

    int ret = -1;
    ret = RSA1024_Routine((uint8_t *) inputbuf, 128, rsa_d_128, 128, rsa_n_128, 128, (uint8_t *)outputbuf, E_DRVAESDMA_RSA1024_PRIVATE);
    if(ret == -1)
    {
        printf("RSA Encryption Fail!!!\n");
        return -1;
    }
    printf("Decrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outputbuf, 256);

    return 0;
}

int MSTAR_Hex_RSA_Decrypt(char* inputbuf, int inputlen, char* outputbuf)
{
     //StringConvert((char *)inputbuf);
    MS_U8 output[256]= {0};
    memset(output,  0,  256*sizeof(MS_U8));	
	
     LOGI("\n Hex_RSA_Decrypt inputbuf==>\n");
    DumpBuf1024((unsigned char *)inputbuf, 128);

    int ret = -1;
    ret = RSA1024_Routine( (uint8_t *)inputbuf, 128, rsa_e_128, 4, rsa_n_128, 128, (uint8_t *)output, E_DRVAESDMA_RSA1024_PUBLIC);
    if(ret == -1)
    {
        LOGI("RSA Hex_RSA_Decrypt Fail!!!\n");
        return -1;
    }
    LOGI("Hex_RSA_Decrypt Result:\n"); 
    DumpBuf1024((unsigned char *)outputbuf, 256);
    LOGI("Rerurn Result:\n"); 	
    strncpy((char*)outputbuf, (const char*)(output+64) , 64*sizeof(MS_U8));
    DumpBuf1024((unsigned char *)outputbuf, 64);
    return 0;
}
	

/////////////////////////////////////////////////////////////////////////////////////////////

/* SHA-256 functions */

void sha256_process(sha256_context * ctx, uint8_t data[64])
{
	uint32_t temp1, temp2;
	uint32_t W[64];
	uint32_t A, B, C, D, E, F, G, H;

	GET_UINT32_BE(W[0], data, 0);
	GET_UINT32_BE(W[1], data, 4);
	GET_UINT32_BE(W[2], data, 8);
	GET_UINT32_BE(W[3], data, 12);
	GET_UINT32_BE(W[4], data, 16);
	GET_UINT32_BE(W[5], data, 20);
	GET_UINT32_BE(W[6], data, 24);
	GET_UINT32_BE(W[7], data, 28);
	GET_UINT32_BE(W[8], data, 32);
	GET_UINT32_BE(W[9], data, 36);
	GET_UINT32_BE(W[10], data, 40);
	GET_UINT32_BE(W[11], data, 44);
	GET_UINT32_BE(W[12], data, 48);
	GET_UINT32_BE(W[13], data, 52);
	GET_UINT32_BE(W[14], data, 56);
	GET_UINT32_BE(W[15], data, 60);

#define SHR(x,n) ((x & 0xFFFFFFFF) >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (32 - n)))

#define S0(x) (ROTR(x, 7) ^ ROTR(x,18) ^ SHR(x, 3))
#define S1(x) (ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))

#define F0(x,y,z) ((x & y) | (z & (x | y)))
#define F1(x,y,z) (z ^ (x & (y ^ z)))

#define R(t)					\
(						\
	W[t] = S1(W[t - 2]) + W[t - 7] +	\
		S0(W[t - 15]) + W[t - 16]	\
)

#define P(a,b,c,d,e,f,g,h,x,K) {		\
	temp1 = h + S3(e) + F1(e,f,g) + K + x;	\
	temp2 = S2(a) + F0(a,b,c);		\
	d += temp1; h = temp1 + temp2;		\
}

	A = ctx->state[0];
	B = ctx->state[1];
	C = ctx->state[2];
	D = ctx->state[3];
	E = ctx->state[4];
	F = ctx->state[5];
	G = ctx->state[6];
	H = ctx->state[7];

	P(A, B, C, D, E, F, G, H, W[0], 0x428A2F98);
	P(H, A, B, C, D, E, F, G, W[1], 0x71374491);
	P(G, H, A, B, C, D, E, F, W[2], 0xB5C0FBCF);
	P(F, G, H, A, B, C, D, E, W[3], 0xE9B5DBA5);
	P(E, F, G, H, A, B, C, D, W[4], 0x3956C25B);
	P(D, E, F, G, H, A, B, C, W[5], 0x59F111F1);
	P(C, D, E, F, G, H, A, B, W[6], 0x923F82A4);
	P(B, C, D, E, F, G, H, A, W[7], 0xAB1C5ED5);
	P(A, B, C, D, E, F, G, H, W[8], 0xD807AA98);
	P(H, A, B, C, D, E, F, G, W[9], 0x12835B01);
	P(G, H, A, B, C, D, E, F, W[10], 0x243185BE);
	P(F, G, H, A, B, C, D, E, W[11], 0x550C7DC3);
	P(E, F, G, H, A, B, C, D, W[12], 0x72BE5D74);
	P(D, E, F, G, H, A, B, C, W[13], 0x80DEB1FE);
	P(C, D, E, F, G, H, A, B, W[14], 0x9BDC06A7);
	P(B, C, D, E, F, G, H, A, W[15], 0xC19BF174);
	P(A, B, C, D, E, F, G, H, R(16), 0xE49B69C1);
	P(H, A, B, C, D, E, F, G, R(17), 0xEFBE4786);
	P(G, H, A, B, C, D, E, F, R(18), 0x0FC19DC6);
	P(F, G, H, A, B, C, D, E, R(19), 0x240CA1CC);
	P(E, F, G, H, A, B, C, D, R(20), 0x2DE92C6F);
	P(D, E, F, G, H, A, B, C, R(21), 0x4A7484AA);
	P(C, D, E, F, G, H, A, B, R(22), 0x5CB0A9DC);
	P(B, C, D, E, F, G, H, A, R(23), 0x76F988DA);
	P(A, B, C, D, E, F, G, H, R(24), 0x983E5152);
	P(H, A, B, C, D, E, F, G, R(25), 0xA831C66D);
	P(G, H, A, B, C, D, E, F, R(26), 0xB00327C8);
	P(F, G, H, A, B, C, D, E, R(27), 0xBF597FC7);
	P(E, F, G, H, A, B, C, D, R(28), 0xC6E00BF3);
	P(D, E, F, G, H, A, B, C, R(29), 0xD5A79147);
	P(C, D, E, F, G, H, A, B, R(30), 0x06CA6351);
	P(B, C, D, E, F, G, H, A, R(31), 0x14292967);
	P(A, B, C, D, E, F, G, H, R(32), 0x27B70A85);
	P(H, A, B, C, D, E, F, G, R(33), 0x2E1B2138);
	P(G, H, A, B, C, D, E, F, R(34), 0x4D2C6DFC);
	P(F, G, H, A, B, C, D, E, R(35), 0x53380D13);
	P(E, F, G, H, A, B, C, D, R(36), 0x650A7354);
	P(D, E, F, G, H, A, B, C, R(37), 0x766A0ABB);
	P(C, D, E, F, G, H, A, B, R(38), 0x81C2C92E);
	P(B, C, D, E, F, G, H, A, R(39), 0x92722C85);
	P(A, B, C, D, E, F, G, H, R(40), 0xA2BFE8A1);
	P(H, A, B, C, D, E, F, G, R(41), 0xA81A664B);
	P(G, H, A, B, C, D, E, F, R(42), 0xC24B8B70);
	P(F, G, H, A, B, C, D, E, R(43), 0xC76C51A3);
	P(E, F, G, H, A, B, C, D, R(44), 0xD192E819);
	P(D, E, F, G, H, A, B, C, R(45), 0xD6990624);
	P(C, D, E, F, G, H, A, B, R(46), 0xF40E3585);
	P(B, C, D, E, F, G, H, A, R(47), 0x106AA070);
	P(A, B, C, D, E, F, G, H, R(48), 0x19A4C116);
	P(H, A, B, C, D, E, F, G, R(49), 0x1E376C08);
	P(G, H, A, B, C, D, E, F, R(50), 0x2748774C);
	P(F, G, H, A, B, C, D, E, R(51), 0x34B0BCB5);
	P(E, F, G, H, A, B, C, D, R(52), 0x391C0CB3);
	P(D, E, F, G, H, A, B, C, R(53), 0x4ED8AA4A);
	P(C, D, E, F, G, H, A, B, R(54), 0x5B9CCA4F);
	P(B, C, D, E, F, G, H, A, R(55), 0x682E6FF3);
	P(A, B, C, D, E, F, G, H, R(56), 0x748F82EE);
	P(H, A, B, C, D, E, F, G, R(57), 0x78A5636F);
	P(G, H, A, B, C, D, E, F, R(58), 0x84C87814);
	P(F, G, H, A, B, C, D, E, R(59), 0x8CC70208);
	P(E, F, G, H, A, B, C, D, R(60), 0x90BEFFFA);
	P(D, E, F, G, H, A, B, C, R(61), 0xA4506CEB);
	P(C, D, E, F, G, H, A, B, R(62), 0xBEF9A3F7);
	P(B, C, D, E, F, G, H, A, R(63), 0xC67178F2);

	ctx->state[0] += A;
	ctx->state[1] += B;
	ctx->state[2] += C;
	ctx->state[3] += D;
	ctx->state[4] += E;
	ctx->state[5] += F;
	ctx->state[6] += G;
	ctx->state[7] += H;
}




void sha256_transf(sha256_ctx *ctx, const unsigned char *message,
                   unsigned int block_nb)
{
    uint32 w[64];
    uint32 wv[8];
    uint32 t1, t2;
    const unsigned char *sub_block;
    int i;

#ifndef UNROLL_LOOPS
    int j;
#endif

    for (i = 0; i < (int) block_nb; i++) {
        sub_block = message + (i << 6);

#ifndef UNROLL_LOOPS
        for (j = 0; j < 16; j++) {
            PACK32(&sub_block[j << 2], &w[j]);
        }

        for (j = 16; j < 64; j++) {
            SHA256_SCR(j);
        }

        for (j = 0; j < 8; j++) {
            wv[j] = ctx->h[j];
        }

        for (j = 0; j < 64; j++) {
            t1 = wv[7] + SHA256_F2(wv[4]) + CH(wv[4], wv[5], wv[6])
                + sha256_k[j] + w[j];
            t2 = SHA256_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);
            wv[7] = wv[6];
            wv[6] = wv[5];
            wv[5] = wv[4];
            wv[4] = wv[3] + t1;
            wv[3] = wv[2];
            wv[2] = wv[1];
            wv[1] = wv[0];
            wv[0] = t1 + t2;
        }

        for (j = 0; j < 8; j++) {
            ctx->h[j] += wv[j];
        }
#else
        PACK32(&sub_block[ 0], &w[ 0]); PACK32(&sub_block[ 4], &w[ 1]);
        PACK32(&sub_block[ 8], &w[ 2]); PACK32(&sub_block[12], &w[ 3]);
        PACK32(&sub_block[16], &w[ 4]); PACK32(&sub_block[20], &w[ 5]);
        PACK32(&sub_block[24], &w[ 6]); PACK32(&sub_block[28], &w[ 7]);
        PACK32(&sub_block[32], &w[ 8]); PACK32(&sub_block[36], &w[ 9]);
        PACK32(&sub_block[40], &w[10]); PACK32(&sub_block[44], &w[11]);
        PACK32(&sub_block[48], &w[12]); PACK32(&sub_block[52], &w[13]);
        PACK32(&sub_block[56], &w[14]); PACK32(&sub_block[60], &w[15]);

        SHA256_SCR(16); SHA256_SCR(17); SHA256_SCR(18); SHA256_SCR(19);
        SHA256_SCR(20); SHA256_SCR(21); SHA256_SCR(22); SHA256_SCR(23);
        SHA256_SCR(24); SHA256_SCR(25); SHA256_SCR(26); SHA256_SCR(27);
        SHA256_SCR(28); SHA256_SCR(29); SHA256_SCR(30); SHA256_SCR(31);
        SHA256_SCR(32); SHA256_SCR(33); SHA256_SCR(34); SHA256_SCR(35);
        SHA256_SCR(36); SHA256_SCR(37); SHA256_SCR(38); SHA256_SCR(39);
        SHA256_SCR(40); SHA256_SCR(41); SHA256_SCR(42); SHA256_SCR(43);
        SHA256_SCR(44); SHA256_SCR(45); SHA256_SCR(46); SHA256_SCR(47);
        SHA256_SCR(48); SHA256_SCR(49); SHA256_SCR(50); SHA256_SCR(51);
        SHA256_SCR(52); SHA256_SCR(53); SHA256_SCR(54); SHA256_SCR(55);
        SHA256_SCR(56); SHA256_SCR(57); SHA256_SCR(58); SHA256_SCR(59);
        SHA256_SCR(60); SHA256_SCR(61); SHA256_SCR(62); SHA256_SCR(63);

        wv[0] = ctx->h[0]; wv[1] = ctx->h[1];
        wv[2] = ctx->h[2]; wv[3] = ctx->h[3];
        wv[4] = ctx->h[4]; wv[5] = ctx->h[5];
        wv[6] = ctx->h[6]; wv[7] = ctx->h[7];

        SHA256_EXP(0,1,2,3,4,5,6,7, 0); SHA256_EXP(7,0,1,2,3,4,5,6, 1);
        SHA256_EXP(6,7,0,1,2,3,4,5, 2); SHA256_EXP(5,6,7,0,1,2,3,4, 3);
        SHA256_EXP(4,5,6,7,0,1,2,3, 4); SHA256_EXP(3,4,5,6,7,0,1,2, 5);
        SHA256_EXP(2,3,4,5,6,7,0,1, 6); SHA256_EXP(1,2,3,4,5,6,7,0, 7);
        SHA256_EXP(0,1,2,3,4,5,6,7, 8); SHA256_EXP(7,0,1,2,3,4,5,6, 9);
        SHA256_EXP(6,7,0,1,2,3,4,5,10); SHA256_EXP(5,6,7,0,1,2,3,4,11);
        SHA256_EXP(4,5,6,7,0,1,2,3,12); SHA256_EXP(3,4,5,6,7,0,1,2,13);
        SHA256_EXP(2,3,4,5,6,7,0,1,14); SHA256_EXP(1,2,3,4,5,6,7,0,15);
        SHA256_EXP(0,1,2,3,4,5,6,7,16); SHA256_EXP(7,0,1,2,3,4,5,6,17);
        SHA256_EXP(6,7,0,1,2,3,4,5,18); SHA256_EXP(5,6,7,0,1,2,3,4,19);
        SHA256_EXP(4,5,6,7,0,1,2,3,20); SHA256_EXP(3,4,5,6,7,0,1,2,21);
        SHA256_EXP(2,3,4,5,6,7,0,1,22); SHA256_EXP(1,2,3,4,5,6,7,0,23);
        SHA256_EXP(0,1,2,3,4,5,6,7,24); SHA256_EXP(7,0,1,2,3,4,5,6,25);
        SHA256_EXP(6,7,0,1,2,3,4,5,26); SHA256_EXP(5,6,7,0,1,2,3,4,27);
        SHA256_EXP(4,5,6,7,0,1,2,3,28); SHA256_EXP(3,4,5,6,7,0,1,2,29);
        SHA256_EXP(2,3,4,5,6,7,0,1,30); SHA256_EXP(1,2,3,4,5,6,7,0,31);
        SHA256_EXP(0,1,2,3,4,5,6,7,32); SHA256_EXP(7,0,1,2,3,4,5,6,33);
        SHA256_EXP(6,7,0,1,2,3,4,5,34); SHA256_EXP(5,6,7,0,1,2,3,4,35);
        SHA256_EXP(4,5,6,7,0,1,2,3,36); SHA256_EXP(3,4,5,6,7,0,1,2,37);
        SHA256_EXP(2,3,4,5,6,7,0,1,38); SHA256_EXP(1,2,3,4,5,6,7,0,39);
        SHA256_EXP(0,1,2,3,4,5,6,7,40); SHA256_EXP(7,0,1,2,3,4,5,6,41);
        SHA256_EXP(6,7,0,1,2,3,4,5,42); SHA256_EXP(5,6,7,0,1,2,3,4,43);
        SHA256_EXP(4,5,6,7,0,1,2,3,44); SHA256_EXP(3,4,5,6,7,0,1,2,45);
        SHA256_EXP(2,3,4,5,6,7,0,1,46); SHA256_EXP(1,2,3,4,5,6,7,0,47);
        SHA256_EXP(0,1,2,3,4,5,6,7,48); SHA256_EXP(7,0,1,2,3,4,5,6,49);
        SHA256_EXP(6,7,0,1,2,3,4,5,50); SHA256_EXP(5,6,7,0,1,2,3,4,51);
        SHA256_EXP(4,5,6,7,0,1,2,3,52); SHA256_EXP(3,4,5,6,7,0,1,2,53);
        SHA256_EXP(2,3,4,5,6,7,0,1,54); SHA256_EXP(1,2,3,4,5,6,7,0,55);
        SHA256_EXP(0,1,2,3,4,5,6,7,56); SHA256_EXP(7,0,1,2,3,4,5,6,57);
        SHA256_EXP(6,7,0,1,2,3,4,5,58); SHA256_EXP(5,6,7,0,1,2,3,4,59);
        SHA256_EXP(4,5,6,7,0,1,2,3,60); SHA256_EXP(3,4,5,6,7,0,1,2,61);
        SHA256_EXP(2,3,4,5,6,7,0,1,62); SHA256_EXP(1,2,3,4,5,6,7,0,63);

        ctx->h[0] += wv[0]; ctx->h[1] += wv[1];
        ctx->h[2] += wv[2]; ctx->h[3] += wv[3];
        ctx->h[4] += wv[4]; ctx->h[5] += wv[5];
        ctx->h[6] += wv[6]; ctx->h[7] += wv[7];
#endif /* !UNROLL_LOOPS */
    }
}


void sha256_final(sha256_ctx *ctx, unsigned char *digest)
{
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;

#ifndef UNROLL_LOOPS
    int i;
#endif

    block_nb = (1 + ((SHA256_BLOCK_SIZE - 9)
                     < (ctx->len % SHA256_BLOCK_SIZE)));

    len_b = (ctx->tot_len + ctx->len) << 3;
    pm_len = block_nb << 6;

    memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
    ctx->block[ctx->len] = 0x80;
    UNPACK32(len_b, ctx->block + pm_len - 4);

    sha256_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
    for (i = 0 ; i < 8; i++) {
        UNPACK32(ctx->h[i], &digest[i << 2]);
    }
#else
   UNPACK32(ctx->h[0], &digest[ 0]);
   UNPACK32(ctx->h[1], &digest[ 4]);
   UNPACK32(ctx->h[2], &digest[ 8]);
   UNPACK32(ctx->h[3], &digest[12]);
   UNPACK32(ctx->h[4], &digest[16]);
   UNPACK32(ctx->h[5], &digest[20]);
   UNPACK32(ctx->h[6], &digest[24]);
   UNPACK32(ctx->h[7], &digest[28]);
#endif /* !UNROLL_LOOPS */
}



void sha256_update(sha256_ctx *ctx, const unsigned char *message,
                   unsigned int len)
{
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char *shifted_message;

    tmp_len = SHA256_BLOCK_SIZE - ctx->len;
    rem_len = len < tmp_len ? len : tmp_len;

    memcpy(&ctx->block[ctx->len], message, rem_len);

    if (ctx->len + len < SHA256_BLOCK_SIZE) {
        ctx->len += len;
        return;
    }

    new_len = len - rem_len;
    block_nb = new_len / SHA256_BLOCK_SIZE;

    shifted_message = message + rem_len;

    sha256_transf(ctx, ctx->block, 1);
    sha256_transf(ctx, shifted_message, block_nb);

    rem_len = new_len % SHA256_BLOCK_SIZE;

    memcpy(ctx->block, &shifted_message[block_nb << 6],
           rem_len);

    ctx->len = rem_len;
    ctx->tot_len += (block_nb + 1) << 6;
}

void sha256_init(sha256_ctx *ctx)
{
#ifndef UNROLL_LOOPS
    int i;
    for (i = 0; i < 8; i++) {
        ctx->h[i] = sha256_h0[i];
    }
#else
    ctx->h[0] = sha256_h0[0]; ctx->h[1] = sha256_h0[1];
    ctx->h[2] = sha256_h0[2]; ctx->h[3] = sha256_h0[3];
    ctx->h[4] = sha256_h0[4]; ctx->h[5] = sha256_h0[5];
    ctx->h[6] = sha256_h0[6]; ctx->h[7] = sha256_h0[7];
#endif /* !UNROLL_LOOPS */

    ctx->len = 0;
    ctx->tot_len = 0;
}

int MSTAR_Hex_SHA256_Decrypt(const unsigned char *message, unsigned int len, unsigned char *digest)
{
    char input[83];//SN+CHIPID+IN+T2+R2
    memset(input,  0,  83); 
    char output[33];
    memset(output,  0,  33);   	
    char SN[] = "018999902016040812345678";	
    char ChipID[] = "1234";
    char IN[] = "9LQ2oy3sVflPFOth8bqTNdqn";	
    char MAC[]="5A0B6CBDB7E9";
	
    strcpy(input, SN);
    strcat(input, ChipID);
    strcat(input, IN);
    strcat(input, (const char *)(message+4));
    input[24] = 0x00;	
    input[25] = 0x00;
    input[26] = 0x00;
    input[27] = 0x00;
LOGI("\n ================input=============== \n");	
{
    MS_U32 i = 0;
    for (i = 0; i < 83; i++) {
        LOGI("0x%02x", input[i]);
    }
}	
    sha256((const unsigned char *)input,82, (unsigned char *)output);
LOGI("\n ================output=============== \n");		
{
    MS_U32 i = 0;
    for (i = 0; i < 33; i++) {
        LOGI("0x%02x", output[i]);
    }
}		
LOGI("\n =output= %s\n", output);		
#if 0	
    char encrypt[46]; 	
    memset(encrypt,  0,  46); 
    char decrypt[17];      
    memset(decrypt,  0,  17); 

    MS_U32 i = 0;	
    for (i = 0; i < 33; i++) {
        if(output[i]>='a'&&output[i]<='z')
            encrypt[i] = output[i] - 32;
	else
	    encrypt[i] = output[i];
    LOGI("\n output[i]=(%x %x)\n",output[i], encrypt[i]);	 	
    }
LOGI("\n =encrypt= %s\n", encrypt);		
	

    strcat(encrypt, MAC);
LOGI("\n ================encrypt=============== \n");		
{
    MS_U32 i = 0;
    for (i = 0; i < 46; i++) {
        LOGI("0x%02x", encrypt[i]);
    }
}		
LOGI("\n ==encrypt=%s \n", encrypt);		

    MD5_CTX md5;  
    MD5Init(&md5);                
    MD5Update(&md5,(unsigned char *)encrypt,strlen((char *)encrypt));  
    MD5Final(&md5,(unsigned char *)decrypt);    
LOGI("\n ================decrypt=============== \n");		
{
    MS_U32 i = 0;
    for (i = 0; i < 17; i++) {
        LOGI("0x%02x", decrypt[i]);
    }
}	
#endif
    strcpy((char *)digest, (const char *)output);	
	
     return 0;

}

int sha256(const unsigned char *message, unsigned int len, unsigned char *digest)
{
    sha256_ctx ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, message, len);
    sha256_final(&ctx, digest);
    //  LOGI("sha256 digest: %s\n", digest);
	
    unsigned char output[2 * 32 + 1];
    int i;
    output[2 * 32] = '\0';
    for (i = 0; i < (int) 32 ; i++) 
       sprintf((char *) output + 2 * i, "%02X", digest[i]);

   //  digest=output;
    // printf("sha256 output: %s\n", output);
     LOGI("1 sha256 output: %s\n", output);
     strcpy((char *)digest, (const char *)output);
     LOGI("1 sha256 digest: %s\n", digest);

     return 0;

}


MS_U32 ulrand(void) 
{
     return (
     (((unsigned long)rand()<<24)&0xFF000000ul)
    |(((unsigned long)rand()<<12)&0x00FFF000ul)
    |(((unsigned long)rand()    )&0x00000FFFul));
}


int MSTAR_readRandom(char* rand) 
{
   // MS_U32 random = 0 ;
    //random = MDrv_CA_Random();  
  //  sprintf(rand, "%d", random);
 //   printf("rand = %s\n",rand);
    MS_U32 ul;
    char temp[32];
    memset(temp,  0,  32);

    srand(time(NULL));
    ul=ulrand();
    sprintf((char *)temp, "%02X", ul);
    strcpy(rand, temp);
    ul=ulrand();
    sprintf((char *)temp, "%02X", ul);	
    strcat(rand, temp);
    ul=ulrand();
    sprintf((char *)temp, "%02X", ul);	
    strcat(rand, temp);
    ul=ulrand();
    sprintf((char *)temp, "%02X", ul);	
    strcat(rand, temp);
#if 0
    MS_U32 i = 0;
    for (i = 0; i < 16; i++) {
        LOGI("0x%02x%c", rand[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    LOGI("\n");	
#endif	
    return 0;
}

unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
                           
void MD5Init(MD5_CTX *context)  
{  
     context->count[0] = 0;  
     context->count[1] = 0;  
     context->state[0] = 0x67452301;  
     context->state[1] = 0xEFCDAB89;  
     context->state[2] = 0x98BADCFE;  
     context->state[3] = 0x10325476;  
}  
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen)  
{  
    unsigned int i = 0,index = 0,partlen = 0;  
    index = (context->count[0] >> 3) & 0x3F;  
    partlen = 64 - index;  
    context->count[0] += inputlen << 3;  
    if(context->count[0] < (inputlen << 3))  
       context->count[1]++;  
    context->count[1] += inputlen >> 29;  
      
    if(inputlen >= partlen)  
    {  
       memcpy(&context->buffer[index],input,partlen);  
       MD5Transform(context->state,context->buffer);  
       for(i = partlen;i+64 <= inputlen;i+=64)  
           MD5Transform(context->state,&input[i]);  
       index = 0;          
    }    
    else  
    {  
        i = 0;  
    }  
    memcpy(&context->buffer[index],&input[i],inputlen-i);  
}  
void MD5Final(MD5_CTX *context,unsigned char digest[16])  
{  
    unsigned int index = 0,padlen = 0;  
    unsigned char bits[8];  
    index = (context->count[0] >> 3) & 0x3F;  
    padlen = (index < 56)?(56-index):(120-index);  
    MD5Encode(bits,context->count,8);  
    MD5Update(context,PADDING,padlen);  
    MD5Update(context,bits,8);  
    MD5Encode(digest,context->state,16);  
}  
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len)  
{  
    unsigned int i = 0,j = 0;  
    while(j < len)  
    {  
         output[j] = input[i] & 0xFF;    
         output[j+1] = (input[i] >> 8) & 0xFF;  
         output[j+2] = (input[i] >> 16) & 0xFF;  
         output[j+3] = (input[i] >> 24) & 0xFF;  
         i++;  
         j+=4;  
    }  
}  
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len)  
{  
     unsigned int i = 0,j = 0;  
     while(j < len)  
     {  
           output[i] = (input[j]) |  
                       (input[j+1] << 8) |  
                       (input[j+2] << 16) |  
                       (input[j+3] << 24);  
           i++;  
           j+=4;   
     }  
}  
void MD5Transform(unsigned int state[4],unsigned char block[64])  
{  
     unsigned int a = state[0];  
     unsigned int b = state[1];  
     unsigned int c = state[2];  
     unsigned int d = state[3];  
     unsigned int x[64];  
     MD5Decode(x,block,64);  
     FF(a, b, c, d, x[ 0], 7, 0xd76aa478); /* 1 */  
 FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */  
 FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */  
 FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */  
 FF(a, b, c, d, x[ 4], 7, 0xf57c0faf); /* 5 */  
 FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */  
 FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */  
 FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */  
 FF(a, b, c, d, x[ 8], 7, 0x698098d8); /* 9 */  
 FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */  
 FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */  
 FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */  
 FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */  
 FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */  
 FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */  
 FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */  
   
 /* Round 2 */  
 GG(a, b, c, d, x[ 1], 5, 0xf61e2562); /* 17 */  
 GG(d, a, b, c, x[ 6], 9, 0xc040b340); /* 18 */  
 GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */  
 GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */  
 GG(a, b, c, d, x[ 5], 5, 0xd62f105d); /* 21 */  
 GG(d, a, b, c, x[10], 9,  0x2441453); /* 22 */  
 GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */  
 GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */  
 GG(a, b, c, d, x[ 9], 5, 0x21e1cde6); /* 25 */  
 GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */  
 GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */  
 GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */  
 GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */  
 GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8); /* 30 */  
 GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */  
 GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */  
   
 /* Round 3 */  
 HH(a, b, c, d, x[ 5], 4, 0xfffa3942); /* 33 */  
 HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */  
 HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */  
 HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */  
 HH(a, b, c, d, x[ 1], 4, 0xa4beea44); /* 37 */  
 HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */  
 HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */  
 HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */  
 HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */  
 HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */  
 HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */  
 HH(b, c, d, a, x[ 6], 23,  0x4881d05); /* 44 */  
 HH(a, b, c, d, x[ 9], 4, 0xd9d4d039); /* 45 */  
 HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */  
 HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */  
 HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */  
   
 /* Round 4 */  
 II(a, b, c, d, x[ 0], 6, 0xf4292244); /* 49 */  
 II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */  
 II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */  
 II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */  
 II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */  
 II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */  
 II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */  
 II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */  
 II(a, b, c, d, x[ 8], 6, 0x6fa87e4f); /* 57 */  
 II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */  
 II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */  
 II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */  
 II(a, b, c, d, x[ 4], 6, 0xf7537e82); /* 61 */  
 II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */  
 II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */  
 II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */  
     state[0] += a;  
     state[1] += b;  
     state[2] += c;  
     state[3] += d;  
} 

int TA_GetSN(unsigned char* pSNValue, int* pnSNValue)
{
     if((NULL == pSNValue) && (NULL == pnSNValue))
          return 0;

      RSAinit();

     property_set("M.SN", "018999902016040812345678");  //TEST
     //property_set("M.SN", "00070022000208A5C8889706");  //TEST
     property_get("M.SN", (char*)pSNValue, NULL);
	 LOGI("\n TA_GetSN--> pSNValue=%s \n", pSNValue);	 
     *pnSNValue = strlen((const char*)pSNValue);
	 LOGI("\n TA_GetSN--> pnSNValue=%d \n", *pnSNValue);	 	 
     return 0;
}

int TA_OTP_SetChipID(unsigned char* pChipIDValue)
{
       if(NULL == pChipIDValue)
          return 0;
       MS_U8 ChipID[9] = "\0";
	strcpy((char *)ChipID, (char *)pChipIDValue);   
	LOGI("TA_OTP_SetChipID  ChipID:%s\n",ChipID);
	MDrv_CA_OTP_SetDeviceId(ChipID,8);
	LOGI("TA_SetChipID  SUC!!!!!!!!!!\n");
	return 0;
}

int TA_OTP_GetChipID(unsigned char* pChipIDValue)
{
      if(NULL == pChipIDValue)
          return 0;
      MS_U8 ChipID[9] = "\0";
      MDrv_CA_OTP_GetDeviceId(ChipID,8);
      strcpy((char *)pChipIDValue, (char *)ChipID); 
      LOGI("TA_GetChipID:%s\n",pChipIDValue);
      //printf("TA_GetChipID:%s\n",pChipIDValue);
      return 0;
}

int TA_OTP_Write_M_Data(void)    //M "CTIT"
{
#if 0
    MS_U8 M[5] = "CTIT";
    MDrv_CA_OTP_WriteReserved(M,4);
    LOGI("TA_OTP_Write_M_Data  SUC!!!!!!!!!!\n");
#endif
    return 0;
}

int TA_OTP_Write_SN_Data(unsigned char* pSNDataValue) 
{
#if 0
    if(NULL == pSNDataValue)
         return 0;
    MS_U8 SN[25] = "\0"; 
    strcpy((char *)SN, (char *)pSNDataValue); 
    LOGI("TA_OTP_Write_SN_Data  SN:%s\n",SN);
    MDrv_CA_OTP_WriteReserved(SN,24);
    LOGI("TA_OTP_Write_SN_Data  SUC!!!!!!!!!!\n");
#endif
    return 0;
}

int TA_OTP_Write_IN_Data(unsigned char* pINDataValue)
{
#if 0
    if(NULL == pINDataValue)
         return 0;
    MS_U8 IN[25] = "\0"; 
    strcpy((char *)IN, (char *)pINDataValue); 
    LOGI("TA_OTP_Write_IN_Data  IN:%s\n",IN);
    MDrv_CA_OTP_WriteReserved(IN,24);
    LOGI("TA_OTP_Write_IN_Data  SUC!!!!!!!!!!\n");
#endif
    return 0;
}

int TA_OTP_Write_M_SN_IN_Data(unsigned char* pMSNINDataValue)   // M+SN+IN+RSAE
{
    if(NULL == pMSNINDataValue)
         return 0;
    MS_U8 M_SN_IN_RSAE[61] = "\0"; 
    strcpy((char *)M_SN_IN_RSAE, (char *)pMSNINDataValue); 
    LOGI("TA_OTP_Write_IN_Data  pMSNINDataValue:%s\n",M_SN_IN_RSAE);
    MDrv_CA_OTP_WriteReserved(M_SN_IN_RSAE,60);
    LOGI("TA_OTP_Write_M_SN_IN_Data  SUC!!!!!!!!!!\n");
    return 0;
}

int TA_OTP_Write_RSA_N_Data(void) 
{
	/*
    MDrv_CA_OTP_SetRSAKey(E_CA_RSA_KEY1,EKpri_rsa_n_128,128);
    LOGI("TA_OTP_Write_RSA_N_Data  SUC!!!!!!!!!!\n");
    return 0;
	*/
	
    MS_BOOL ret =0;
	
	ret =MDrv_CA_OTP_SetRSAKey(E_CA_RSA_KEY1,EKpri_rsa_n_128,128);
    if(ret == TRUE)
    {
        LOGI("TA_OTP_Write_RSA_N_Data  SUC!!!!!!!!0!\n");
    }
    else
    {
        ret =MDrv_CA_OTP_SetRSAKey(E_CA_RSA_KEY1,EKpri_rsa_n_128,128);
    }
	
	 return 0;
}

int TA_OTP_Write_RSA_E_Data(void) 
{
#if 0
    MDrv_CA_OTP_SetSecretKey(E_CA_SECRET_KEY1,EKpri_rsa_e_128,4);
    LOGI("TA_OTP_Write_RSA_D_Data  SUC!!!!!!!!!!\n");
#endif
    return 0;
}

int TA_OTP_Read_RSA_N_Data(unsigned char* RSA_N)
{
    MDrv_CA_OTP_Write(0x3CB4, 0x40000);   // unlock read rsa data 
    
    if(NULL == RSA_N)
          return 0;
    MS_U8 RSAN[129] = "\0";
    MDrv_CA_OTP_GetRSAKey(E_CA_RSA_KEY1,RSAN,128);  
  //  strcpy((char *)RSA_N, (char *)RSAN);
    memcpy(RSA_N,RSAN,sizeof(RSAN));

    LOGI("TA_OTP_Read_RSA_N_Data  SUC!!!!!!!!!!\n");
    LOGI("\n =======>RSAN OUTPUT:\n");
    for(int i=0; i<128;i++)
    {
        LOGI("%02X ", *(( char *)RSA_N + i));
     }
	 
    return 0;
}

int TA_OTP_Lock(void)
{
   MDrv_CA_OTP_Lock(E_LockWord_CA_reserved0);
   MDrv_CA_OTP_Lock(E_LockWord_CA_reserved1);
   MDrv_CA_OTP_Lock(E_LockWord_CA_reserved2);
   MDrv_CA_OTP_Lock(E_LockWord_CA_reserved3);
 //  MDrv_CA_OTP_Lock(E_LockWord_RSA_1_W);
   return 0;
}

int TA_OTP_Read_RSA_E_Data(unsigned char* RSA_E) 
{
#if 0
    int ret = 0;
    if(NULL == RSA_E)
          return 0;
    MS_U8 RSAE[5] = "\0";
    MDrv_CA_OTP_GetSecretKey(E_CA_SECRET_KEY1,RSAE,4);
    ret = MDrv_CA_OTP_Read(0x3CB4);
    LOGI("MDrv_CA_OTP_Read 0x3CB4====>ret:%d\n",ret);
    strcpy((char *)RSA_E, (char *)RSAE);

    LOGI("TA_OTP_Read_RSA_N_Data  SUC!!!!!!!!!!\n");
    LOGI("\n =======>RSAE OUTPUT:\n");
    for(int i=0; i<4;i++)
    {
        LOGI("%02X ", *(( char *)RSA_E + i));
        if(i%16==15)LOGI("\n");
    }
#endif
    return 0;
}

int TA_OTP_Read_Data(unsigned char* pReadDataValue) 
{
      MS_U8 SN_IN_M_RSAE[61] = "\0"; 
      MDrv_CA_OTP_ReadReserved(SN_IN_M_RSAE,60);
      strcpy((char *)pReadDataValue, (char *)SN_IN_M_RSAE); 
      LOGI("TA_OTP_Read_Data:%s\n",pReadDataValue);
      LOGI("TA_OTP_Read_RSA_N_Data  SUC!!!!!!!!!!\n");
      return 0;
}

int TA_OTP_MaxReservedSize()
{
	 int 	MaxReservedSize = 0;
        MaxReservedSize = MDrv_CA_MaxReservedSize();
	 LOGI("===>MaxReservedSize:=%d\n",MaxReservedSize);	
	 return MaxReservedSize;
}

int TA_Factory_Certify_TimeStamp(unsigned char* pTimeStampValue, int* pnTimeStampValue)
{
    if((NULL == pTimeStampValue) && (NULL == pnTimeStampValue))
          return 0;
    time_t t;  
    t = time(NULL);  
    struct tm *lt;  
    int ii = time(&t);  
    LOGI("ii = %d\n", ii);  
    t = time(NULL);  
    lt = localtime(&t);  
    strftime((char *)pTimeStampValue, 24, "%Y%m%d%H%M%S", lt); 
    *pnTimeStampValue = strlen((const char*)pTimeStampValue);
    LOGI("==>nowtime = %s,pTimeStampValue=%d\n", pTimeStampValue,*pnTimeStampValue); 
    return 0;
}

int TA_Factory_Certify_Random(unsigned char* pRandomValue, int* pnRandomValue)
{
    if((NULL == pRandomValue) && (NULL == pnRandomValue))
          return 0;
    MS_U8 Random[33] = "\0";
    MSTAR_readRandom((char *)Random);
    strcpy((char *)pRandomValue, (char *)Random); 
    *pnRandomValue = strlen((const char*)pRandomValue);
     LOGI("12==>TA_Factory_Certify_Random:%s,len=%d\n",pRandomValue,*pnRandomValue);
    return 0;
}

int TA_TMtokenSyn(unsigned char* pTMtokenValue, int* pnTMtokenValue)
{
      LOGI("\n 00  -- TA_TMtokenSyn pTMtokenValue=%s\n", pTMtokenValue);
      property_set("M.TMtoken", (const char*)pTMtokenValue);
      property_get("M.TMtoken", (char*)pTMtokenValue, NULL);
      *pnTMtokenValue = strlen((const char*)pTMtokenValue);
      LOGI("\n TA_TMtokenSyn pnTMtokenValue=%d\n", *pnTMtokenValue);	  
      return 0;
}

int strcopy_ex (void* dest ,void* src)
{
      if (dest== NULL || src == NULL)
      return -1 ;
      unsigned char* to = (unsigned char*) dest ;
      unsigned char* from = (unsigned char*) src ;
      while (*to++ = *from++) ;
      return 0 ;
}


int Factory_IN_Certify(unsigned char* pSN,unsigned char* pMAC,unsigned char* pIN,unsigned char* pchipid,unsigned char* MD5,unsigned char* OutIN)
{
     LOGI("\n Factory_IN_Certify pSN=%s  pMAC=%s  pIN=%s \n", pSN, pMAC,pIN);
     int ret = -1;

     char pIN_T_R_Hash_Decrypt_Out[0x100] = "\0";
     ret = Hex_EKpri_Hash_RSA_Decrypt((char *)pIN,strlen((char *) pIN), (char *)pIN_T_R_Hash_Decrypt_Out);

     if (0 != ret) 
     {
        LOGI(" Factory_IN_Certify  RSA HW pIN error!!!!!!\n");
        return -1;
     }

     char IN_T_R_Hash_Decrypt_Out[55] = "\0";
     char IN[25] = "\0";
     strncpy((char *)IN_T_R_Hash_Decrypt_Out,pIN_T_R_Hash_Decrypt_Out+74,54); 
     strncpy((char *)IN,IN_T_R_Hash_Decrypt_Out,24); 
     strcpy((char *)OutIN,IN); 
     
	
     LOGI("OutIN:%s\n",OutIN);
     LOGI("0000IN_T_R_Hash_Decrypt_Out:%s\n",IN_T_R_Hash_Decrypt_Out);

     StringConvert((char *)pchipid);
     unsigned char ChipID[5] = "\0";
     memset(ChipID, 0, sizeof(ChipID));
     memcpy(ChipID,pchipid,sizeof(ChipID));

    char SN_CHIPID_IN_T_R[83] = "\0";

    strcat((char *)SN_CHIPID_IN_T_R, (const char *)pSN);
    strcat((char *)SN_CHIPID_IN_T_R, (const char *)ChipID);
    memcpy(SN_CHIPID_IN_T_R+28,IN_T_R_Hash_Decrypt_Out,54); 

    unsigned char SHA265_OUT_SN_CHIPID_IN_T_R[65] = "\0";
    for(int i=0;i<32;i++)
        SHA265_OUT_SN_CHIPID_IN_T_R[i] = 0;

  
    sha256((const unsigned char *)SN_CHIPID_IN_T_R,82, (unsigned char *)SHA265_OUT_SN_CHIPID_IN_T_R);
	

    //////////////////////MD5_SHA265_OUT_SN_CHIPID_IN_T_R+MAC///////////////////////////

     LOGI("\n011==>ChipID:");
     for (MS_U32 i = 0; i <  12; i++) {
       LOGI("0x%02x ", pMAC[i]);
     }

    unsigned char IN_SHA256_MAC[64+12+1] = "\0";
    unsigned char MD5_SHA256_MAC_OUT[17] = "\0";


     memcpy(IN_SHA256_MAC,(const char *)SHA265_OUT_SN_CHIPID_IN_T_R,64);
     memcpy(IN_SHA256_MAC+64,pMAC,12);



   // LOGI("\n IN_SHA256_MAC OUTPUT:\n");
   // for(int i=0; i<76;i++)
   // {
      //  LOGI("%02X ", *(( char *)IN_SHA256_MAC + i));
       // if(i%16==15)LOGI("\n");
   // }

	

   //unsigned char IN_TEST_SHA256_MAC[] ="68E7B9E38A1B75C0A4B51D90F57353631A7174847DEA5FEDB14FF7D9782702775A0B6CBDB7E9"; 

   // LOGI("\nIN_TEST_SHA256_MAC OUTPUT:\n");
  //  LOGI("===========>Len=%d\n",strlen((char *)IN_TEST_SHA256_MAC));
   // for(int i=0; i<strlen((char *)IN_TEST_SHA256_MAC);i++)
   // {
   //     LOGI("%02X ", *(( char *)IN_TEST_SHA256_MAC + i));
    //    if(i%16==15)LOGI("\n");
   // }


///////////////////////////MD5///////////////////////////////////
    MD5_CTX md5;  
    MD5Init(&md5);                
    MD5Update(&md5,IN_SHA256_MAC,76); 
    MD5Final(&md5,MD5_SHA256_MAC_OUT);          


    unsigned char IN_MD5_SHA256_MAC_OUT[2 * 16+1 ] = "\0";
    //MS_U32 i = 0;	
    for (MS_U32 i = 0; i < 16 ; i++) 
       sprintf((char *) IN_MD5_SHA256_MAC_OUT + 2 * i, "%02X", MD5_SHA256_MAC_OUT[i]);

    LOGI("\n MD5= %s\n", MD5);   
    LOGI("\n MD5_SHA256_MAC_OUT = %s\n", IN_MD5_SHA256_MAC_OUT); 

     if (0 != memcmp(MD5,IN_MD5_SHA256_MAC_OUT,32)) {
        LOGI("Fail!!!!!!!Certify MD5 is:\n");
        return -1;  //Hash Certify Fail
    } else {
        LOGI("MD5 equal SUC!!!!!!!!!!!!!!!!!!!!!!!\n");
	 return 0;
    }

     return -1; 

}


int Factory_TMtoken_Certify(unsigned char* pAuthenticator,unsigned char* pSN,unsigned char* pchipid,unsigned char* pIN,unsigned char* OUT_SHA256_SN_CHIPID_IN_T2_R2)
{
   //Result=000,ResultDesc=??,ChallengeCode=33C5742698AE750839CFCED7AB39A68CC46B5FA4208A701A4845C542035AC0E5EDB6744272A2A4EE988BC0F56015B1F95DED8D3EFC0C31AE10C3C4362B628A2C2948BDA3BACEE60A054C85005FC21A97D20A1E393D47D0113E3FEB615E1345DC333AC1A60C9F1404EFFAF0EF461879143544D67F5987A0348A4E887226B4642A
    char ChallengeCode_Decrypt_Out[0x100] = "\0";
    int ret = 0;
    ret = Hex_EKpri_Hash_RSA_Decrypt((char *)pAuthenticator,strlen((char *) pAuthenticator), (char *)ChallengeCode_Decrypt_Out);

     if (0 != ret) 
     {
        LOGI(" Factory_IN_Certify  RSA HW pIN error!!!!!!\n");
        return -1;
     }

 //    LOGI("\n ChallengeCode_Decrypt_Out OUTPUT:\n");
   //  for(int i=0; i<128;i++)
  //   {
  //      LOGI("%02X ", *(( char *)ChallengeCode_Decrypt_Out + i));
   //     if(i%16==15)LOGI("\n");
  //   }

     char Decrypt_pAuthenticator_out[53] = "\0";
     strncpy((char *)Decrypt_pAuthenticator_out,ChallengeCode_Decrypt_Out+76,52); 
     printf("Decrypt_pAuthenticator_out:%s\n",Decrypt_pAuthenticator_out);

     //CTIT20160728173300null201607281709118019418423950762
	 
    char T2_R2_out[31] = "\0";
    strncpy((char *)T2_R2_out,Decrypt_pAuthenticator_out+22,30);
    LOGI("T2_R2_out:%s\n",T2_R2_out);

    //SN+CHIPID+IN+T2_R2    test =018999902016040812345678ffff9LQ2oy3sVflPFOth8bqTNdqn201607281709118019418423950762

   char SN_CHIPID_IN_T2_R2[83] = "\0";

   StringConvert((char *)pchipid);
   unsigned char ChipID[5] = "\0";
   memset(ChipID, 0, sizeof(ChipID));
   memcpy(ChipID,pchipid,sizeof(ChipID));

   strcpy((char *)SN_CHIPID_IN_T2_R2, (const char *)pSN);
   strcat((char *)SN_CHIPID_IN_T2_R2, (const char *)ChipID);
   strcat((char *)SN_CHIPID_IN_T2_R2, (const char *)pIN);
   strcat((char *)SN_CHIPID_IN_T2_R2, T2_R2_out);


   MS_U8 SHA265_OUT_SN_CHIPID_IN_T2_R2[64] = "\0";
   LOGI("\n======[1]SN_CHIPID_IN_T2_R2:%s\n",SN_CHIPID_IN_T2_R2);
   sha256((const unsigned char *)SN_CHIPID_IN_T2_R2,82, (unsigned char *)SHA265_OUT_SN_CHIPID_IN_T2_R2);

   strcpy((char *)OUT_SHA256_SN_CHIPID_IN_T2_R2, (const char *)SHA265_OUT_SN_CHIPID_IN_T2_R2);


   LOGI("SHA265_OUT_SN_CHIPID_IN_T2_R2:%s\n",SHA265_OUT_SN_CHIPID_IN_T2_R2);
   LOGI("OUT_SHA256_SN_CHIPID_IN_T2_R2:%s\n",OUT_SHA256_SN_CHIPID_IN_T2_R2);
   
   return 0; 
}




int TA_SPubKeyCertify(unsigned char* pKeyPackage, int* pnKeyPackage,unsigned char* pRandom_TM, int* pnRandom_TM, unsigned char* pTimeStamp_TM, int* pnTimeStamp_TM, unsigned char* pAuthenticator, int* pnAuthenticator, int* pAlgorithmID_H, int* pAlgorithmID_Auth)
{
      LOGI("\n TA_SPubKeyCertify pKeyPackage=%s pRandom_TM=%s pTimeStamp_TM=%s\n", pKeyPackage, pRandom_TM, pTimeStamp_TM);
      *pAlgorithmID_H = 1;//SHA256
      *pAlgorithmID_Auth = 0;//RSA1024
       MS_U8 KeyPackage[]= {0};
       LOGI("\n=====================step1=====================\n");	

     int ret = -1;
     //char Kcert[0x200] = "\0";
     //strcopy_ex(&Kcert,pKeyPackage);

     char *Kspub=strtok((char *)pKeyPackage, ","); 
     char *ValidTim=strtok(NULL, ","); 
     char *EKpri=strtok(NULL, ",");

     LOGI("\n*********************************************************\n");
     LOGI("pKeyPackage: %s\n", pKeyPackage);
     //LOGI("Kcert: %s\n", Kcert);
     LOGI("Kspub: %s\n", Kspub);
     LOGI("ValidTim: %s\n", ValidTim);	
     LOGI("EKpri: %s\n", EKpri);
     LOGI("\n*********************************************************\n");

//T
    MS_U8 T2[15] = "\0";
    time_t t;  
    t = time(NULL);  
    struct tm *lt;  
    int ii = time(&t);  
    printf("ii = %d\n", ii);  
    t = time(NULL);  
    lt = localtime(&t);  
    char nowtime[15] = "\0";;  
    memset(nowtime, 0, sizeof(nowtime));  
    strftime(nowtime, 24, "%Y%m%d%H%M%S", lt);  
    LOGI("==>nowtime = %s\n", nowtime); 

    if((atoll((const char*)ValidTim) <  atoll((const char*)pTimeStamp_TM)) || (atoll((const char*)ValidTim) <  atoll((const char*)nowtime)))
    {
         LOGI("atoll(ValidTim)=%lld,atoll(pTimeStamp_TM)=%lld,atoll(nowtime)=%lld,,,Time Certify Fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!:\n",atoll((const char*)ValidTim),atoll((const char*)pTimeStamp_TM),atoll((const char*)nowtime));
         return -1;
    }
	 
     char EKpri_Hash_Decrypt_Out[0x100] = "\0";
     MS_U8 Certify_EKpri_Hash[] = "79F8B8FD680D97593713A0CD2984F45071945171E046FAE4F20DAC3DDF3E8025";

     LOGI("\n*****************************16****************************\n");
     mem_dump((uint8_t *)EKpri,strlen((char *) EKpri));
     LOGI("\n*****************************16****************************\n");
												   
    ret = Hex_EKpri_Hash_RSA_Decrypt((char *)EKpri,strlen((char *) EKpri), (char *)EKpri_Hash_Decrypt_Out);
    if (0 != ret) 
    {
        LOGI(" EKpri_Hash_RSA_Decrypt  RSA HW encrypt error!!!!!!\n");
        return -1;
    }

    strncpy((char *)EKpri_Hash_Decrypt_Out,EKpri_Hash_Decrypt_Out+64,64); 

    LOGI("Decrypt Valid Data:\n");
    mem_dump((uint8_t *)EKpri_Hash_Decrypt_Out,64);

// sha(Kspub + ,+ValidTim)
     MS_U8 Kspub_ValidTim[400] = "\0";
     strcat((char *)Kspub_ValidTim, (const char *)Kspub);
     strcat((char *)Kspub_ValidTim, ",");
     strcat((char *)Kspub_ValidTim, (const char *)ValidTim);
     memcpy(Kspub_ValidTim,Kspub_ValidTim,strlen((char *) Kspub)+strlen((char *) ValidTim)+1); 
     MS_U8 Hash_Kspub_ValidTim_out[64] =  "\0";
     sha256((const unsigned char *)Kspub_ValidTim,strlen((char *) Kspub)+strlen((char *) ValidTim)+1, (unsigned char *)Hash_Kspub_ValidTim_out);
     LOGI("\n======[1]Hash_Kspub_ValidTim_out:%s\n",Hash_Kspub_ValidTim_out);
     printf("\n======[1]Hash_Kspub_ValidTim_out:%s\n",Hash_Kspub_ValidTim_out);
    
   // check data
   // if (0 != memcmp(EKpri_Hash_Decrypt_Out,Certify_EKpri_Hash,64)) {                //Test
        if (0 != memcmp(EKpri_Hash_Decrypt_Out,Hash_Kspub_ValidTim_out,64)) {
        LOGI("Certify_EKpri_Hash data not equal\n");
        LOGI("Certify_EKpri_Hash data bakcup is:\n");
        mem_dump((uint8_t *)EKpri_Hash_Decrypt_Out,64);
        return -1;  //Hash Certify Fail
    } else {
        LOGI("22 data equal SUC!!!!!!!!!!!!!!!!!!!!!!!\n");
    }

    LOGI("\n=====================step2=====================\n");	
	
    MS_U8 M[] = "CTIT";
    MS_U8 H[65] =  "\0";//hash(SN+IV+IN)=hash(SN+T2+R2+IN)
    MS_U8 H_Hash[150] = "\0";
    strcpy((char *)H_Hash, "018999902016040812345678");
    strcat((char *)H_Hash, (const char *)pTimeStamp_TM);
    strcat((char *)H_Hash, (const char *)pRandom_TM);
    strcat((char *)H_Hash, "9LQ2oy3sVflPFOth8bqTNdqn");
    LOGI("\n======[1]pTimeStamp_TM:%s\n",pTimeStamp_TM);
    LOGI("\n======[1]pRandom_TM:%s\n",pRandom_TM);
    LOGI("\n======[1]strlen(SN+T2+R2+IN) :%d\n",24+strlen((char *) pTimeStamp_TM)+strlen((char *) pRandom_TM)+24);
    LOGI("\n======[1]H_Hash:%s\n",H_Hash);
    sha256((const unsigned char *)H_Hash,24+strlen((char *) pTimeStamp_TM)+strlen((char *) pRandom_TM)+24, (unsigned char *)H);
    LOGI("\n======[1]H:%s\n",H);	
    char M_H_Random_TimeStamp[0x200];
    memset(M_H_Random_TimeStamp,0,sizeof(M_H_Random_TimeStamp));	

    strcpy(M_H_Random_TimeStamp, (char *)M);
    LOGI("\n======[1]M_H_Random_TimeStamp:%s\n",M_H_Random_TimeStamp);
    strcat((char *)M_H_Random_TimeStamp, (char *)H);  
    LOGI("\n======[2]M_H_Random_TimeStamp:%s\n",M_H_Random_TimeStamp);
	
//T:
    //strcpy((char *)T2, "20160529163940");
    strcpy((char *)T2, (char *)nowtime);
    strcat((char *)M_H_Random_TimeStamp, (char *)T2);  	
    LOGI("\n======[4]M_H_Random_TimeStamp:%s\n",M_H_Random_TimeStamp);
	
//R:
    MS_U8 Random[32] = "\0";
    MSTAR_readRandom((char *)Random);
     LOGI("12==>Random:%s\n",Random);
    //strcpy((char *)Random, "01234567891234560123456789123456");	
    strcat((char *)M_H_Random_TimeStamp, (char *)Random);  	
    LOGI("\n======[3]M_H_Random_TimeStamp:%s,strlen((char *) M_H_Random_TimeStamp):%d\n",M_H_Random_TimeStamp,strlen((char *) M_H_Random_TimeStamp));
	
    char M_H_Random_TimeStamp_OUT[0x100];
    memset(M_H_Random_TimeStamp_OUT,0,sizeof(M_H_Random_TimeStamp_OUT));	

//N:
   unsigned char Str_Session_rsa_n_128[257] = "\0";
   memset(Str_Session_rsa_n_128, 0, sizeof(Str_Session_rsa_n_128));
 //  strncpy((char *)Str_Session_rsa_n_128,Kspub+58,256); 
   memcpy(Str_Session_rsa_n_128,Kspub+58,256); 
   StringConvert((char *)Str_Session_rsa_n_128);  
   memset(KeyPackage_Session_rsa_n_128, 0, sizeof(KeyPackage_Session_rsa_n_128));
   memcpy(KeyPackage_Session_rsa_n_128,Str_Session_rsa_n_128,128); 
   LOGI("Session_rsa_n_128:");
   for (MS_U32 i = 0; i < 128; i++) {
        LOGI("0x%02x", KeyPackage_Session_rsa_n_128[i]);
    }
   
//E:
   unsigned char Str_Session_rsa_e_128[] = "00000000";
   strncpy((char *)Str_Session_rsa_e_128+2,Kspub+318,6); 
   LOGI("2 Str_Session_rsa_e_128:%s",Str_Session_rsa_e_128);
   StringConvert((char *)Str_Session_rsa_e_128);  
   memset(KeyPackage_Session_rsa_e_128, 0, sizeof(KeyPackage_Session_rsa_e_128));
   memcpy(KeyPackage_Session_rsa_e_128,Str_Session_rsa_e_128,4);  
   LOGI("Session_rsa_e_128:");
   for (MS_U32 i = 0; i < 4; i++) {
        LOGI("0x%02x", KeyPackage_Session_rsa_e_128[i]);
    }

    ret = EKpri_Hash_sessionEncrypt((char *)M_H_Random_TimeStamp,strlen((char *) M_H_Random_TimeStamp), (char *)M_H_Random_TimeStamp_OUT);
	
    if (0 != ret) 
    {
        LOGI(" EKpri_Hash_terminalEncrypt  RSA HW encrypt error!\n");
        return -1;
    }
  
    unsigned char output[2 * 128 + 1];
    output[2 * 128] = '\0';
    //MS_U32 i = 0;	
    for (MS_U32 i = 0; i < (int) 128 ; i++) 
       sprintf((char *) output + 2 * i, "%02X", M_H_Random_TimeStamp_OUT[i]);

     strcpy((char *)pAuthenticator, (const char *)output);
     *pnAuthenticator = strlen((const char *)pAuthenticator);
     LOGI("\n TA_SPubKeyCertify pAuthenticator=[%s,%d]\n", pAuthenticator, *pnAuthenticator);
	 
     return 0;

}

/*
void test(const unsigned char *vector, unsigned char *digest,
          unsigned int digest_size)
{
    unsigned char output[2 * 64 + 1];
    int i;

    output[2 * digest_size] = '\0';

    for (i = 0; i < (int) digest_size ; i++) {
       sprintf((char *) output + 2 * i, "%02X", digest[i]);
    }

    printf("H: %s\n", output);
    if (strcmp((char *) vector, (char *) output)) {
        fprintf(stderr, "Test failed.\n");
       // exit(EXIT_FAILURE);
    }
}
*/

unsigned char HexToAsc(unsigned char aChar)
{
    if((aChar>=0x30)&&(aChar<=0x39))
        aChar -= 0x30;
    else if((aChar>=0x41)&&(aChar<=0x46))
        aChar -= 0x37;
    else if((aChar>=0x61)&&(aChar<=0x66))
        aChar -= 0x57;
    else aChar = 0xff;
    return aChar; 
} 

#define RECOVERY_ENV_FILE "/cache/recovery/command"

int Command_execv(const char *pathname, char * const argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        unsetenv("LD_PRELOAD");
        execv(pathname, argv);
        LOGI("execv command fail:%s",strerror(errno));
        exit(-1);
    }
    else
    {
        if(waitpid(pid, NULL, 0) == pid)
         {
            LOGI("successfully get child %d\n",pid);
            return 0;
         }
        else
        {
            LOGI("some error occured\n");
            return -1;
        }
    }
    return -1;
}

int Reboot_Recovery_Update(void)
{
    MS_U32 m_fd = -1;
    char const* argv1[] = {"mkdir", "/cache/recovery", NULL,NULL};
    Command_execv("/system/bin/mkdir",(char * const *)argv1);
	
    m_fd = open(RECOVERY_ENV_FILE, O_RDWR | O_CREAT,0777);
    if(m_fd != -1)
    {
         MS_U8 str[42];
         memset(str, 0, sizeof(str));
         snprintf((char*)str, 42, "--update_package=/cache/update_signed.zip");
         write(m_fd, (MS_U8*)str, sizeof(str));
    }
    else
    {
        return -1;
    }

    char const* argv2[] = {"reboot", "recovery", NULL,NULL};
    Command_execv("/system/bin/reboot",(char * const *)argv2);

    return 0;
}

int Reboot_Recovery_Restore_factory(void)
{
    MS_U32 m_fd = -1;
    char const* argv1[] = {"mkdir", "/cache/recovery", NULL,NULL};
    Command_execv("/system/bin/mkdir",(char * const *)argv1);
	
    m_fd = open(RECOVERY_ENV_FILE, O_RDWR | O_CREAT,0777);
    if(m_fd != -1)
    {
         MS_U8 str[11];
         memset(str, 0, sizeof(str));
         snprintf((char*)str, 11, "--wipe_data");
         write(m_fd, (MS_U8*)str, sizeof(str));
    }
    else
    {
        return -1;
    }

    char const* argv2[] = {"reboot", "recovery", NULL,NULL};
    Command_execv("/system/bin/reboot",(char * const *)argv2);

    return 0;
}



int RSAinit(void)
{

    char const* argv2[] = {"chmod", "777", "/dev/miomap",NULL};
   // Command_execv("/system/bin/chmod",(char * const *)argv2);

	system("chmod 777 /dev/miomap");

    LOGI("\n=====================RSAinit1=====================\n");		
    MsOS_Init();
	
    LOGI("\n=====================RSAinit2=====================\n");	
    MDrv_MMIO_Init();
	
    LOGI("\n=====================RSAinit3=====================\n");		
    MDrv_CA_Init(); 
	
    LOGI("\n=====================RSAinit4=====================\n");	
    MDrv_CIPHER_Init();

	
	property_set("M.ddr.size", "2");
	property_set("M.emmc.size", "8");
	property_set("M.ouid", "001A9A");
	property_set("M.version.software", "131072");
	property_set("M.info.hardware", "100001"); 
   
    return 0;
}




int main( int argc, char * argv[] )
{

    RSAinit();

#if 0
#if 1   
#if 0   //1024
    MS_U8 u8SignatureOut[0x100] = "\0";
    int ret = -1;
    ret = terminalEncrypt((char *)u8Signature,128, (char *)u8SignatureOut);
    if (0 != ret) 
    {
        printf(" terminalEncrypt  RSA HW encrypt error!\n");
        return -1;
    }
    ret =RSA_Decrypt((char *)u8SignatureOut,128,(char *)u8SignatureOut);
    if (0 != ret)
    {
        printf(" RSA_Decrypt  RSA HW Decrypt error!\n");
        return -1;
    } 

   printf("TEST   RSA HW decrypt:\n");
   mem_dump((uint8_t *)u8SignatureOut, 0x100);
   // check data
    if (0 != memcmp(u8SignatureOut,u8Signaturebackup,0x80)) {
        printf("data not equal\n");
        printf("data bakcup is:\n");
        mem_dump((uint8_t *)u8Signaturebackup,0x80);
    } else {
        printf("data equal\n");
    }

#else
    int ret = -1;
    MS_U8 u8Signature1[] = "79F8B8FD680D97593713A0CD2984F45071945171E046FAE4F20DAC3DDF3E8025";
    MS_U8 u8SignatureBF[] = "79F8B8FD680D97593713A0CD2984F45071945171E046FAE4F20DAC3DDF3E8025";
    char u8SignatureOut1[0x100] = "\0";
    MS_U8 u8SignatureBFTest[] = "36ECE08A839926DCE0758CA71C0A557D6C95B69BB6101B8F484683EFB058C3F1459B286CAA9138931BF000A6A429C8F3E2B9E9CC44B98187AB342FC7EC18038E67F45BA3AF468C0FF2FFF79A49F54FF7439054FE25514AA0876789061A1DFE61F1ACDC5FCD51E1DC27638AB920214F2FC47F7EC687F2842C87E26F4EA1B05C1E";
													   
#if 0
    ret = EKpri_Hash_terminalEncrypt((char *)u8Signature1,strlen((char *) u8Signature1), (char *)u8SignatureOut1);
    if (0 != ret) 
    {
        printf(" terminalEncrypt  RSA HW encrypt error!\n");
        return -1;
    }

    ret =EKpri_Hash_RSA_Decrypt((char *)u8SignatureOut1,strlen((char *) u8SignatureOut1),(char *)u8SignatureOut1);
    if (0 != ret)
    {
        printf(" RSA_Decrypt  RSA HW Decrypt error!\n");
        return -1;
    } 
   // check data
    if (0 != memcmp(u8SignatureOut1,u8SignatureBF,strlen((char *) u8SignatureOut1))) {
        printf("data not equal\n");
        printf("data bakcup is:\n");
        mem_dump((uint8_t *)u8SignatureBF,strlen((char *) u8SignatureBF));
    } else {
        printf("data equal SUC!!!!!!!!!!!!!!!!!!!!!!!\n");
    }
#endif

#if 1
    ret = EKpri_Hash_sessionEncrypt((char *)u8Signature1,strlen((char *) u8Signature1), (char *)u8SignatureOut1);
    if (0 != ret) 
    {
        printf(" terminalEncrypt  RSA HW encrypt error!\n");
        return -1;
    }

    ret =EKpri_Hash_sessionDecrypt((char *)u8SignatureOut1,128,(char *)u8SignatureOut1);//1024
    if (0 != ret)
    {
        printf(" RSA_Decrypt  RSA HW Decrypt error!\n");
        return -1;
    } 

    strncpy((char *)u8SignatureOut1,u8SignatureOut1+64,64); 

    printf("Decrypt Valid Data:\n");
    mem_dump((uint8_t *)u8SignatureOut1,64);

   // check data
    if (0 != memcmp(u8SignatureOut1,u8SignatureBF,64)) {
        printf("data not equal\n");
        printf("data bakcup is:\n");
        mem_dump((uint8_t *)u8SignatureBF,strlen((char *) u8SignatureBF));
    } else {
        printf("data equal SUC!!!!!!!!!!!!!!!!!!!!!!!\n");
    }
	
#endif


#if 0 //TEST A
    ret =Hex_EKpri_Hash_RSA_Decrypt((char *)u8SignatureBFTest,128,(char *)u8SignatureOut1);//1024
    if (0 != ret)
    {
        printf(" RSA_Decrypt  RSA HW Decrypt error!\n");
        return -1;
    } 

    strncpy((char *)u8SignatureOut1,u8SignatureOut1+64,64); 

    printf("Decrypt Valid Data:\n");
    mem_dump((uint8_t *)u8SignatureOut1,64);

   // check data
    if (0 != memcmp(u8SignatureOut1,u8SignatureBF,64)) {
        printf("data not equal\n");
        printf("data bakcup is:\n");
        mem_dump((uint8_t *)u8SignatureBF,strlen((char *) u8SignatureBF));
    } else {
        printf("data equal SUC!!!!!!!!!!!!!!!!!!!!!!!\n");
    }
#endif



#endif
#else

#if 1
    MS_U8 u8Signature[] = "0189999020160408123456785A0B6CBDB7E900000189999020160408123456785A0B6CBDB7E900000189999020160408123456785A0B6CBDB7E9000000000000";
    MS_U8 u8Signaturebackup[] = "0189999020160408123456785A0B6CBDB7E900000189999020160408123456785A0B6CBDB7E900000189999020160408123456785A0B6CBDB7E9000000000000";
#else
    MS_U8 u8Signature[] = {
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05};
    MS_U8 u8Signaturebackup[]= {
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05,
        0x53 ,0x45 ,0x43 ,0x55 ,0x52 ,0x49 ,0x54 ,0x59 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x03 ,0x00 ,0x00,
        0x5f ,0x09 ,0x4c ,0x87 ,0x86 ,0xdc ,0x0a ,0xa9 ,0x59 ,0x44 ,0x32 ,0x7a ,0xc8 ,0xa8 ,0x98 ,0x05};

#endif

	
    MS_U8 u8SignatureOut[0x100] = "\0";

    int ret = -1;
    ret = terminalEncrypt((char *)u8Signature,128, (char *)u8SignatureOut);
    if (0 != ret) 
    {
        printf(" terminalEncrypt  RSA HW encrypt error!\n");
        return -1;
    }
    ret =RSA_Decrypt((char *)u8SignatureOut,128,(char *)u8SignatureOut);
    if (0 != ret)
    {
        printf(" RSA_Decrypt  RSA HW Decrypt error!\n");
        return -1;
    } 

   printf("TEST   RSA HW decrypt:\n");
   mem_dump((uint8_t *)u8SignatureOut, 0x100);
   // check data
    if (0 != memcmp(u8SignatureOut,u8Signaturebackup,0x80)) {
        printf("data not equal\n");
        printf("data bakcup is:\n");
        mem_dump((uint8_t *)u8Signaturebackup,0x80);
    } else {
        printf("data equal\n");
    }

#endif

#if 1
    MS_U8 SN[25] = "\0";
    int sn_len=0;	

    TA_GetSN(SN, &sn_len);
    printf("SN:%s\n", SN);

   MS_U8 Random[25] = "\0";
   MSTAR_readRandom((char *)Random);
   printf("1Random:%s\n", Random);


////////////////////SHA256//////////////////////////////

    const unsigned char vectors[] ="C23E33BE8759B0D1D8D1008FB923F7B4A9425B9F1E80C42EC7A27C47BF06E066";	
    char message1[82] = 
    {0x30,0x31,0x38,0x39,0x39,0x39,0x39,0x30,0x32,0x30,0x31,0x36,0x30,0x34,0x30,0x38,0x31,0x32,0x33,0x34,
    0x35,0x36,0x37,0x38,0x00,0x00,0x00,0x00,0x39,0x4C,0x51,0x32,0x6F,0x79,0x33,0x73,
    0x56,0x66,0x6C,0x50,0x46,0x4F,0x74,0x68,0x38,0x62,0x71,0x54,0x4E,0x64,0x71,0x6E,0x32,0x30,0x31,0x32,
    0x30,0x31,0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
    0x30,0x30,0x30,0x30,0x30,0x30};  //SN+CHIPID+IN+T+R   018999902016040812345678^@^@^@^@9LQ2oy3sVflPFOth8bqTNdqn201201010000000000000000000000
   // char HexStrToBuf[] = "018999902016040812345678^@^@^@^@9LQ2oy3sVflPFOth8bqTNdqn201201010000000000000000000000";

 // FILE *file;
 //   int fileSize;
 //  file = fopen("./data/SN_CHIPID_IN_T_R.txt", "r");
  //  fseek(file, 0, SEEK_END);
  //  fileSize = ftell(file);
  //  fseek(file, 0, SEEK_SET);
  //  message1= (char*)malloc(fileSize + 1);
  //  fread(message1, sizeof(char), 86, file);
  //  message1[fileSize] = 0;
   // printf("%s", message1);
																	   
    unsigned char digest[32];
    for(int i=0;i<32;i++)
        digest[i] = 0;
    printf("SHA-256 Test vectors\n");

    printf("TEST 256====\n");  
    for(int i=0;i<82;i++)  
    {  
	printf("%02X%c", message1[i], ((i % 16) == 15) ? '\n' : ' ');
    }  
    printf("\n");
  
    sha256((const unsigned char *)message1,82, (unsigned char *)digest);
    printf("SHA256 OUTPUT:\n");
    for(int i=0; i<32; i++)
    {
        printf("%02X ", *(( char *)digest + i));
        if(i%16==15)printf("\n");
    }
	
    printf("\n");
    
    test(vectors, digest, 32);
   
///////////////////MD5TEST////////////////////////
 //   unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3  
    // SHA256 + MAC
    unsigned char encrypt[] ="C23E33BE8759B0D1D8D1008FB923F7B4A9425B9F1E80C42EC7A27C47BF06E0665A0B6CBDB7E9"; 
    unsigned char decrypt[16];      
    MD5_CTX md5;  
    MD5Init(&md5);                
    MD5Update(&md5,encrypt,strlen((char *)encrypt));  
    MD5Final(&md5,decrypt);          
    printf("MD5TEST :");  
    for(int i=0;i<16;i++)  
    {  
        printf("%02X",decrypt[i]);  
    }  
    printf("\n");
#endif

    time_t t;  
    t = time(NULL);  
    struct tm *lt;  
    int ii = time(&t);  
    printf("ii = %d\n", ii);  
    t = time(NULL);  
    lt = localtime(&t);  
    char nowtime[24];  
    memset(nowtime, 0, sizeof(nowtime));  
    strftime(nowtime, 24, "%Y%m%d%H%M%S", lt);  
    printf("nowtime = %s\n", nowtime); 

//printf("waiting Reboot_Recovery Test.........................................................");

//usleep(1000*1000*5);
//Reboot_Recovery_Update();

     unsigned char pKeyPackage[] = "30819F300D06092A864886F70D010101050003818D0030818902818100B7DFAB06E24EF1D881465AFACCDCE57296262A69C8289B35F07DECA105EE63D7DE3455443E0837F20267FF5A681858AAFEDBD3E2AADFA8C12B4BF667590DABA2DEE2C56936E54AD83E2DB3DCCC891498D4D8FF5CC79E5B093D4E9EE6F9EA2922D4ED09A7EBBB2CD62125B9B031110E727C9A2D0B1D977AA1096A301276CFB4810203014D0B,20991231000000,36ECE08A839926DCE0758CA71C0A557D6C95B69BB6101B8F484683EFB058C3F1459B286CAA9138931BF000A6A429C8F3E2B9E9CC44B98187AB342FC7EC18038E67F45BA3AF468C0FF2FFF79A49F54FF7439054FE25514AA0876789061A1DFE61F1ACDC5FCD51E1DC27638AB920214F2FC47F7EC687F2842C87E26F4EA1B05C1E";
     char *Kspub=strtok((char *)pKeyPackage, ","); 
     char *ValidTim=strtok(NULL, ","); 
     char *EKpri=strtok(NULL, ",");

// sha(Kspub + ,+ValidTim)
     MS_U8 Kspub_ValidTim[400] = "\0";
     strcat((char *)Kspub_ValidTim, (const char *)Kspub);
     strcat((char *)Kspub_ValidTim, ",");
     strcat((char *)Kspub_ValidTim, (const char *)ValidTim);
     memcpy(Kspub_ValidTim,Kspub_ValidTim,strlen((char *) Kspub)+strlen((char *) ValidTim)+1); 
     MS_U8 Hash_Kspub_ValidTim_out[64] =  "\0";
     sha256((const unsigned char *)Kspub_ValidTim,strlen((char *) Kspub)+strlen((char *) ValidTim)+1, (unsigned char *)Hash_Kspub_ValidTim_out);
     LOGI("\n======[1]Hash_Kspub_ValidTim_out:%s\n",Hash_Kspub_ValidTim_out);
     printf("\n======[1]Hash_Kspub_ValidTim_out:%s\n",Hash_Kspub_ValidTim_out);

     MS_U8 Certify_EKpri_Hash[] = "79F8B8FD680D97593713A0CD2984F45071945171E046FAE4F20DAC3DDF3E8025";

   // check data
    if (0 != memcmp(Hash_Kspub_ValidTim_out,Certify_EKpri_Hash,64)) {
        LOGI("Certify_EKpri_Hash data not equal\n");
        LOGI("Certify_EKpri_Hash data bakcup is:\n");
        mem_dump((uint8_t *)Certify_EKpri_Hash,strlen((char *) Certify_EKpri_Hash));
        return -1;
    } else {
        LOGI("data equal SUC!!!!!!!!!!!!!!!!!!!!!!!\n");
    }

     
    char pTimeStamp_TM[] ="201606162318"; 
    if((atoll((const char*)ValidTim) <  atoll((const char*)pTimeStamp_TM)) || (atoll((const char*)ValidTim) <  atoll((const char*)nowtime)))
    {
         printf("atoll(ValidTim)=%lld,atoll(pTimeStamp_TM)=%lld,atoll(nowtime)=%lld,,,Time Certify Fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!:\n",atoll((const char*)ValidTim),atoll((const char*)pTimeStamp_TM),atoll((const char*)nowtime));
         return -1;
    }
    else
    {
        printf("TEST TIME SUC!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf("atoll(ValidTim)=%lld,atoll(pTimeStamp_TM)=%lld,atoll(nowtime)=%lld\n",atoll((const char*)ValidTim),atoll((const char*)pTimeStamp_TM),atoll((const char*)nowtime));
    }

//test 

   printf("\n*********************************************************\n");
   printf("pKeyPackage: %s\n", pKeyPackage);
   printf("Kspub: %s\n", Kspub);
   printf("ValidTim: %s\n", ValidTim);	
   printf("EKpri: %s\n", EKpri);

//N:
   unsigned char Str_Session_rsa_n_128[257] = "\0";
   memset(Str_Session_rsa_n_128, 0, sizeof(Str_Session_rsa_n_128));
   //strncpy((char *)Str_Session_rsa_n_128,Kspub+58,256); 
   memcpy(Str_Session_rsa_n_128,Kspub+58,256); 
   StringConvert((char *)Str_Session_rsa_n_128);  
   memset(KeyPackage_Session_rsa_n_128, 0, sizeof(KeyPackage_Session_rsa_n_128));
   memcpy(KeyPackage_Session_rsa_n_128,Str_Session_rsa_n_128,128); 
   LOGI("Session_rsa_n_128:");
 //  for (MS_U32 i = 0; i < 128; i++) {
      //  printf("0x%02x", KeyPackage_Session_rsa_n_128[i]);
   // }
       MS_U32 i = 0;
    for (i = 0; i < 128; i++) {
        printf("0x%02x%c", KeyPackage_Session_rsa_n_128[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    printf("\n");
   
//E:
   unsigned char Str_Session_rsa_e_128[] = "00000000";
   strncpy((char *)Str_Session_rsa_e_128+2,Kspub+318,6); 
   LOGI("2 Str_Session_rsa_e_128:%s",Str_Session_rsa_e_128);
   StringConvert((char *)Str_Session_rsa_e_128);  
   memset(KeyPackage_Session_rsa_e_128, 0, sizeof(KeyPackage_Session_rsa_e_128));
   memcpy(KeyPackage_Session_rsa_e_128,Str_Session_rsa_e_128,4);  
   LOGI("Session_rsa_e_128:");
  // for (MS_U32 i = 0; i < 4; i++) {
      //  printf("0x%02x", KeyPackage_Session_rsa_e_128[i]);
    //}

    for (i = 0; i < 4; i++) {
        printf("0x%02x%c", KeyPackage_Session_rsa_e_128[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    printf("\n");	

//TEST!!!!!!!!!!!!!!!!!!!!!!!!

#endif
printf("==================================TEST FA 1=========================================================");


     unsigned char SN1[] = "018999902016040812345678";
     unsigned char MAC1[] = "5A0B6CBDB7E9";
     unsigned char IN1[] = "09A1C38A2C2D0DE8A029EDA731A0676AA89332A279F43845F22082D2227DD65C7646DA2CEDB0D748B8FBA9ED2D951B459CAA3777D971367CD2E54E1C95F82C71EF326032248345DEAB34AC13072290B65260BA2E5BB1F24A34C1297C18B09DA1CB71F52183D8403EE079F76FC3CA37A5E8B1F6D45EBFEC6232F1AC22485ADA85";
     unsigned char chipid[] = "66666666";
     unsigned char MD51[] = "9AA8FB7837DF0110A864AD533A79AB06";
     unsigned char OUTIN[25] = "\0";
	 


    Factory_IN_Certify(SN1,MAC1,IN1,chipid,MD51,OUTIN);
     printf("OUTIN:::::%s\n",OUTIN);

    printf("==================================TEST FA 2=========================================================");

     unsigned char IN[] = "9LQ2oy3sVflPFOth8bqTNdqn";
     unsigned char pAuthenticator[] = "33C5742698AE750839CFCED7AB39A68CC46B5FA4208A701A4845C542035AC0E5EDB6744272A2A4EE988BC0F56015B1F95DED8D3EFC0C31AE10C3C4362B628A2C2948BDA3BACEE60A054C85005FC21A97D20A1E393D47D0113E3FEB615E1345DC333AC1A60C9F1404EFFAF0EF461879143544D67F5987A0348A4E887226B4642A";
     unsigned char SN[] = "018999902016040812345678";
     unsigned char ChipID[] = "66666666";
     unsigned char OUT_SHA256_SN_CHIPID_IN_T2_R2_TEST[83] = "\0";

     Factory_TMtoken_Certify(pAuthenticator,SN,ChipID,IN,OUT_SHA256_SN_CHIPID_IN_T2_R2_TEST);

    printf("OUT_SHA256_SN_CHIPID_IN_T2_R2_TEST:::::%s\n",OUT_SHA256_SN_CHIPID_IN_T2_R2_TEST);
	
    return 0;
}
