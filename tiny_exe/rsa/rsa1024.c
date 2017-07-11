#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h> 
#define PRIKEY "prikey.pem"
//#define PUBKEY "pubkey.pem"
#define PUBKEY "rsa.pem"
#define BUFFSIZE 4096
#define LENGTH 256
#if 0
/************************************************************************
* RSA加密解密函数
*
* file: test_rsa_encdec.c
* gcc -Wall -O2 -o rsa1024 rsa1024.c -lcrypto -lssl
*
************************************************************************/

char *my_encrypt(char *str, char *pubkey_path, char *en)
{
	RSA *rsa = NULL;
	FILE *fp = NULL;
	//int len = 0;
	int rsa_len = 0;
printf("\n[%d,%s]\n", __LINE__, __FUNCTION__);
	if ((fp = fopen(pubkey_path, "r")) == NULL) {
	return NULL;
	}
printf("\n[%d,%s]\n", __LINE__, __FUNCTION__);
	/* 读取公钥PEM，PUBKEY格式PEM使用PEM_read_RSA_PUBKEY函数 */
	if ((rsa = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL)) == NULL) {
	return NULL;
	}
printf("\n[%d,%s]\n", __LINE__, __FUNCTION__);
	RSA_print_fp(stdout, rsa, 0);

	//len = strlen(str);
	rsa_len = RSA_size(rsa);
#if 0
	en = (char *)malloc(rsa_len + 1);
	memset(en, 0, rsa_len + 1);
#endif
	printf("\nrsa_len=%d en=%s\n", rsa_len, en);
printf("\n[%d,%s]\n", __LINE__, __FUNCTION__);
	if (RSA_public_encrypt(rsa_len, (unsigned char *)str, (unsigned char*)en, rsa, RSA_NO_PADDING) < 0) {
	return NULL;
	}
	printf("\nrsa_len=%d en=%s\n", rsa_len, en);

	RSA_free(rsa);
	fclose(fp);

	return NULL;
}

char *my_decrypt(char *str, char *prikey_path, char *de)
{
	RSA *rsa = NULL;
	FILE *fp = NULL;
	int rsa_len = 0;

	if ((fp = fopen(prikey_path, "r")) == NULL) {
	return NULL;
	}

	if ((rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL)) == NULL) {
	return NULL;
	}

	RSA_print_fp(stdout, rsa, 0);

	rsa_len = RSA_size(rsa);
#if 0
	de = (char *)malloc(rsa_len + 1);
	memset(de, 0, rsa_len + 1);
#endif
	printf("\nrsa_len=%d en=%s\n", rsa_len, de);

	if (RSA_private_decrypt(rsa_len, (unsigned char *)str, (unsigned char*)de, rsa, RSA_NO_PADDING) < 0) {
	return NULL;
	}
	printf("\nrsa_len=%d en=%s\n", rsa_len, de);

	RSA_free(rsa);
	fclose(fp);

	return NULL;
}
#endif
struct RSA1024_Public_key_raw {
    char KEYE[3];
    char KEYN[128];
};

RSA1024_Public_key_raw kRSAKeyRaw;


int main(int argc, char *argv[])
{ 
    char input[165];
    char output[257];
    memset(input, 0, sizeof(input));
    memset(output, 0, sizeof(output));

    strcpy(input,"43544954314530354630394541463336373731394637393930413042383942424133374634394532383545414133454443353736334230323542433734433643383244453230313630363136313631373432");
    //strcpy(input, "sisi");
    //char key[] = "B7DFAB06E24EF1D881465AFACCDCE57296262A69C8289B35F07DECA105EE63D7DE3455443E0837F20267FF5A681858AAFEDBD3E2AADFA8C12B4BF667590DABA2DEE2C56936E54AD83E2DB3DCCC891498D4D8FF5CC79E5B093D4E9EE6F9EA2922D4ED09A7EBBB2CD62125B9B031110E727C9A2D0B1D977AA1096A301276CFB481";
    char key[] = "7304BF0A4F4A1E17222F455F53B39DFC58065DF5F2D7FFB3F2090FE3F6720C51FB68063EDA828CED543E3A1BD614971EB4008C667451EBBB85AA8DF5F758150AAF8CE8E1BD2A27D138A813EDC98F326BE217F2C979F7BA9D0A48A02B0A3B1DD5A18249E4A48A538B44101D8D2A3A8A6CA1A9A5B9388C54198A9716272BF514B9";
    RSA *rsa = RSA_new();
    BN_hex2bn(&rsa->n, (const char *) key);
    BN_hex2bn(&rsa->e, "01374B");

    if (RSA_public_encrypt(strlen((const char *) input), (const unsigned char *) input, (unsigned char *) output, rsa, RSA_PKCS1_PADDING ))
    {
        printf("ERRO encrypt\n");
    }
    else
    {
        printf("SUC encrypt\n");
    }
    printf("input=%s\n", input);
    printf("output=%s\n", output);
#if 0
	char src[] = "9LQ2oy3sVflPFOth8bqTNdqn";
	char en[LENGTH] = {0};
	char de[LENGTH] = {0};

	printf("src is: %s\n", src);

	my_encrypt(src, PUBKEY, en);
	printf("enc is: %s\n", en);

	my_decrypt(en, PRIKEY, de);
	printf("dec is: %s\n", de);
#endif
#if 0
	if (en != NULL) {
	free(en);
	}

	if (de != NULL) {
	free(de);
	}
#endif
	return 0;
}
