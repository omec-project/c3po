//
// Created by fanz on 4/7/17.
//

#ifndef MBEDTLS_SGX_SSL_CONTEXT_H
#define MBEDTLS_SGX_SSL_CONTEXT_H

#include "mbedtls/ssl.h"
#include "mbedtls/net.h"

#define MAX_PATH_LEN 1024
typedef struct {
  mbedtls_net_context client_fd;
  int thread_complete;
  const mbedtls_ssl_config *config;
  unsigned char client_ip[16];
  size_t ip_len;
} thread_info_t;

typedef struct {
	char store_path[MAX_PATH_LEN];
}enclave_params_t;

#endif //MBEDTLS_SGX_SSL_CONTEXT_H
