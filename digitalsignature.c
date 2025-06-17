#include<openssl/pem.h> // Privacy Enhanced Mail - file format
#include<openssl/rsa.h>
#include<openssl/err.h> // error handling for openssl 
                        
int sign_block(const char *data,size_t data_len,unsigned char *sig,size_t *sig_len){
  FILE *fp=fopen("private.pem","r");
  if(!fp) return 0;

  EVP_PKEY *pkey=PEM_read_PrivateKey(fp,NULL,NULL,NULL);
  fclose(fp);

  EVP_MD_CTX *ctx=EVP_MD_CTX_new();
  EVP_SignInit(ctx,EVP_sha256());
  EVP_SignUpdate(ctx,data,data_len);
  int result=EVP_SignFinal(ctx,sig,(unsigned int *)sig_len,pkey);

  EVP_MD_CTX_free(ctx);
  EVP_PKEY_free(pkey);

  return result;
}

int verify_block(const char*data,size_t data_len,const char *sig,size_t *sig_len){
  FILE *fp=fopen("public.pem","r");
  if(!fp) return 0;

  EVP_PKEY *pkey=PEM_read_PrivateKey(fp,NULL,NULL,NULL);
  fclose(fp);

  EVP_MD_CTX *ctx=EVP_MD_CTX_new();
  EVP_VerifyInit(ctx, EVP_sha256());
  EVP_VerifyUpdate(ctx,data,data_len);
  int result=EVP_VerifyFinal(ctx,sig,(unsigned int)(*sig_len),pkey);

  EVP_MD_CTX_free(ctx);
  EVP_PKEY_free(pkey);

  return result;
}
