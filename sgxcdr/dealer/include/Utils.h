#pragma once

#include "sgx_error.h"
#include "sgx_eid.h"     /* sgx_enclave_id_t */

#ifndef TRUE
# define TRUE 1
#endif

#ifndef FALSE
# define FALSE 0
#endif

#if defined(_MSC_VER)
#define TOKEN_FILENAME   "Enclave.token"
#define ENCLAVE_FILENAME "Enclave.signed.dll"
#elif defined(__GNUC__)
#define TOKEN_FILENAME   "enclave.token"
#define ENCLAVE_FILENAME "enclave.signed.so"
#endif

void print_error_message(sgx_status_t ret);
int initialize_enclave(sgx_enclave_id_t *eid);

#if defined(_MSC_VER)
int query_sgx_status();
#endif

#if defined(__cplusplus)
extern "C" {
#endif

//extern sgx_enclave_id_t global_eid;    /* global enclave id */


typedef struct _sgx_errlist_t {
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

    
#if defined(__cplusplus)
}
#endif
