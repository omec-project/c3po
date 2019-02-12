#include "/opt/intel/sgxsdk/include/sgx_quote.h"
#include "/home/inteluser/il_spr_ssl_dcs-tls_attestation_framework/deps/local/include/mbedtls/x509.h"
#include "/home/inteluser/il_spr_ssl_dcs-tls_attestation_framework/deps/local/include/mbedtls/ssl.h"

void print_sgx_crt_info(X509* crt);

void get_quote_from_report(const uint8_t* report /* in */,
                           const int report_len  /* in */,
                           sgx_quote_t* quote);

void get_quote_from_cert(
    const uint8_t* der_crt,
    uint32_t der_crt_len,
    sgx_quote_t* q
);

int verify_sgx_cert_extensions
(
    const uint8_t* der_crt,
    uint32_t der_crt_len
);

