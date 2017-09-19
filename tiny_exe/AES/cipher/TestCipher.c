/************************************************************/
/* Author : sisi.wu                                         		*/
/* Date   : 2017/9/18                                       	*/
/************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>

#if 1
char input_name[256]= {0};
char output_name[256]= {0};
char new_name[256]= {0};
#if 1
static const unsigned char key16[16]=
{   0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0,
    0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0,0x12
};
#endif

int aes_encrypt()
{
    FILE *fp_in = NULL;
    FILE *fp_out = NULL;
    unsigned char iv[AES_BLOCK_SIZE];
    int i=0;
    int len = 0, cur_len = 0;
    unsigned char input[AES_BLOCK_SIZE];
    unsigned char output[AES_BLOCK_SIZE];
    for(i=0; i<AES_BLOCK_SIZE; ++i)
        iv[i]=0;
    AES_KEY aes_key;
    if(AES_set_encrypt_key((unsigned char*)key16,128,&aes_key) < 0)
    {
        return 0;
    }
    if((fp_in = fopen(input_name,"rb")) == NULL)
    {
        printf("\nfp_in error open\n");
        goto FAIL;
    }
    if((fp_out = fopen(output_name,"wb")) == NULL)
    {
        printf("\nfp_out error open\n");
        goto FAIL;
    }
    fseek(fp_in,0,SEEK_SET);
    fseek(fp_out,0,SEEK_SET);
    while(!feof(fp_in))
    {
        fgetc(fp_in);
    }
    len = ftell(fp_in);
    fseek(fp_in,0,SEEK_SET);
    while(cur_len<len)
    {
        memset(input, 0 ,AES_BLOCK_SIZE);
        memset(output, 0 ,AES_BLOCK_SIZE);
        for(i=0; i<AES_BLOCK_SIZE; ++i)
            input[i]=fgetc(fp_in);
        AES_encrypt(input, output, &aes_key);
        for(i=0; i<AES_BLOCK_SIZE; ++i)
            fputc(output[i], fp_out);
        cur_len+=AES_BLOCK_SIZE;
    }
FAIL:
    if(fp_in!=NULL)
        fclose(fp_in);
    if(fp_out!=NULL)
        fclose(fp_out);
    return 1;
}

int aes_decrypt()
{
    FILE *fp_out = NULL;
    FILE *fp_new = NULL;
    unsigned char iv[AES_BLOCK_SIZE];
    int i=0;
    int len = 0, cur_len = 0;
    unsigned char input[AES_BLOCK_SIZE];
    unsigned char output[AES_BLOCK_SIZE];

    for(i=0; i<AES_BLOCK_SIZE; ++i)
        iv[i]=0;
    AES_KEY aes_key;
    if(AES_set_decrypt_key((unsigned char*)key16,128,&aes_key) < 0)
    {
        return 0;
    }
    if((fp_out = fopen(output_name,"rb")) == NULL)
    {
        printf("\nfp_out error open\n");
        goto FAIL;
    }
    if((fp_new = fopen(new_name,"wb")) == NULL)
    {
        printf("\nfp_in error open\n");
        goto FAIL;
    }
    fseek(fp_out,0,SEEK_SET);
    fseek(fp_new,0,SEEK_SET);
    while(!feof(fp_out))
    {
        fgetc(fp_out);
    }
    len = ftell(fp_out);
    printf("\n[%d,%s]:len=%d\n",__LINE__, __FUNCTION__, len);

    fseek(fp_out,0,SEEK_SET);
    while(cur_len<len)
    {
        memset(input, 0 ,AES_BLOCK_SIZE);
        memset(output, 0 ,AES_BLOCK_SIZE);
        for(i=0; i<AES_BLOCK_SIZE; ++i)
            input[i]=fgetc(fp_out);
        AES_decrypt(input, output, &aes_key);
        for(i=0; i<AES_BLOCK_SIZE; ++i)
            fputc(output[i], fp_new);
        cur_len+=AES_BLOCK_SIZE;
    }
FAIL:
    if(fp_out!=NULL)
        fclose(fp_out);
    if(fp_new!=NULL)
        fclose(fp_new);
    return 1;
}

int main(int argc,char *argv[])
{
    int i;
    printf("\nargc:%d\n",argc);
    for(i=0; i<=argc; i++)
    {
        printf("argv[%d]:%s\n",i,argv[i]);
    }
    if(argc!=3)
    {
        printf("\nplease input Ex.# ./TestCipher input.txt output.txt\n");
        return -1;
    }
    sprintf(input_name,"%s",argv[1]);
    sprintf(output_name,"%s",argv[2]);
    sprintf(new_name,"%s","new.txt");
    printf("\n[%d,%s]:%s--%s--%s\n",__LINE__, __FUNCTION__, input_name, output_name, new_name);

    printf("\n[%d,%s]=========start aes_encrypt============\n",__LINE__, __FUNCTION__);
    if(!aes_encrypt())
    {
        printf("encrypt error\n");
        return -1;
    }
    printf("\n[%d,%s]=========start aes_decrypt============\n",__LINE__, __FUNCTION__);
    if(!aes_decrypt())
    {
        printf("decrypt error\n");
        return -1;
    }
    printf("\n[%d,%s]=========end============\n",__LINE__, __FUNCTION__);

    printf("\n====stop_time====\n");
    return 0;
}
#endif
#if 0
#define MSG_LEN 128

int aes_encrypt(char* in, char* key, char* out)
{
    if(!in || !key || !out) return 0;
    unsigned char iv[AES_BLOCK_SIZE];
    int i=0;
    for(i=0; i<AES_BLOCK_SIZE; ++i)
        iv[i]=0;
    AES_KEY aes;
    if(AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return 0;
    }
    int len=strlen(in);
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, iv, AES_ENCRYPT);
    return 1;
}

int aes_decrypt(char* in, char* key, char* out)
{
    if(!in || !key || !out) return 0;
    unsigned char iv[AES_BLOCK_SIZE];
    int i=0;
    for(i=0; i<AES_BLOCK_SIZE; ++i)
        iv[i]=0;
    AES_KEY aes;
    if(AES_set_decrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return 0;
    }
    int len=strlen(in);
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, iv, AES_DECRYPT);
    return 1;
}

int main(int argc,char *argv[])
{
    char sourceStringTemp[MSG_LEN];
    char dstStringTemp[MSG_LEN];
    char key[AES_BLOCK_SIZE];
    int i;
    memset((char*)sourceStringTemp, 0 ,MSG_LEN);
    memset((char*)dstStringTemp, 0 ,MSG_LEN);
    strcpy((char*)sourceStringTemp, "123456789 123456789 123456789 12a");
    //strcpy((char*)sourceStringTemp, argv[1]);

    for(i = 0; i < 16; i++)
    {
        key[i] = 32 + i;
    }
    if(!aes_encrypt(sourceStringTemp,key,dstStringTemp))
    {
        printf("encrypt error\n");
        return -1;
    }
    printf("enc %d:",strlen((char*)dstStringTemp));
    for(i= 0; dstStringTemp[i]; i+=1) {
        printf("%x",(unsigned char)dstStringTemp[i]);
    }
    memset((char*)sourceStringTemp, 0 ,MSG_LEN);
    if(!aes_decrypt(dstStringTemp,key,sourceStringTemp))
    {
        printf("decrypt error\n");
        return -1;
    }
    printf("\n");
    printf("dec %d:",strlen((char*)sourceStringTemp));
    printf("%s\n",sourceStringTemp);
    for(i= 0; sourceStringTemp[i]; i+=1) {
        printf("%x",(unsigned char)sourceStringTemp[i]);
    }
    printf("\n====stop_time====\n");
    return 0;
}
#endif


