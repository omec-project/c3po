#include "sgx_crt_info.h" 

void get_quote_from_cert
(
    const uint8_t* der_crt,
    uint32_t der_crt_len,
    sgx_quote_t* q
)
{
    mbedtls_x509_crt crt;
    mbedtls_x509_crt_init(&crt);
    mbedtls_x509_crt_parse(&crt, der_crt, der_crt_len);
    get_quote_from_extension(crt.v3_ext.p, crt.v3_ext.len, q);
    mbedtls_x509_crt_free(&crt);
}


void print_sgx_crt_info(X509* crt)
{
   int der_len = i2d_X509(crt, NULL);
   //assert(der_len > 0);

   unsigned char der[der_len];
   unsigned char *p = der;
   i2d_X509(crt, &p);

   sgx_quote_t quote;
   get_quote_from_cert(der, der_len, &quote);
   sgx_report_body_t* body = &quote.report_body;

   printf("Certificate's SGX information:\n");
   printf("  . MRENCLAVE = ");
   for (int i=0; i < SGX_HASH_SIZE; ++i) printf("%02x", body->mr_enclave.m[i]);
   printf("\n");

   printf("  . MRSIGNER  = ");
   for (int i=0; i < SGX_HASH_SIZE; ++i) printf("%02x", body->mr_signer.m[i]);
   printf("\n");
}
