//
// Created by fanz on 4/7/17.
//

#ifndef MBEDTLS_SGX_SSL_CONTEXT_H
#define MBEDTLS_SGX_SSL_CONTEXT_H

#include "mbedtls/ssl.h"
#include "mbedtls/net.h"

typedef struct {
  mbedtls_net_context client_fd;
  int thread_complete;
  const mbedtls_ssl_config *config;
  unsigned char client_ip[16];
  size_t ip_len;
  char cdr_router_host[16];
  size_t cdr_router_port;
} thread_info_t;

typedef struct {
	char kmsserver[16];
	char kmsport[6];
	char keynamespace[33];
	char cdrpath[4096];
	char cdrarchpath[4096];
}enclave_params_t;

#endif //MBEDTLS_SGX_SSL_CONTEXT_H
