/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SSLCOMMHNDLR_H
#define __SSLCOMMHNDLR_H

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "sgx_quote.h"

#include "options.h"

#define MAX_BUFF_SIZE 512
#define MAX_REC_SIZE 256
#define RETRY_INTERVAL 2

//TODO : change structure
typedef struct {
   uint8_t ias_report[2*1024];
   uint32_t ias_report_len;
   uint8_t ias_sign_ca_cert[2*1024];
   uint32_t ias_sign_ca_cert_len;
   uint8_t ias_sign_cert[2*1024];
   uint32_t ias_sign_cert_len;
   uint8_t ias_report_signature[2*1024];
   uint32_t ias_report_signature_len;
} attestation_verification_report_t;

class SSLCommHndlr
{
private:
   SSL *sslHandle;
   SSL *connectSgxSSL(const std::string &hostname, uint32_t portnum,
      const std::string &client_cert_file, const std::string &priv_key_file);
   SSL_CTX *initCTX(void);
   int openConnection(const std::string &hostname, uint32_t port);
   int verifyCertificate(SSL *ssl);
   int retryConnection();

   int verify_sgx_cert_info(uint8_t mrenclave[], uint8_t mrsigner[],
      sgx_isv_svn_t isv_svn);

   int verify_cert(X509 *crt);

   int get_quote_from_report(const uint8_t *report, const int report_len,
      sgx_quote_t *quote);

   int get_quote_from_cert(const uint8_t *der_crt, uint32_t der_crt_len,
      sgx_quote_t *q);

   int extract_x509_extension(uint8_t *ext, int ext_len, const uint8_t *oid,
      size_t oid_len, uint8_t *data, uint32_t *data_len, uint32_t data_max_len);

   int get_quote_from_extension(uint8_t *ext, size_t ext_len, sgx_quote_t *quote);

   int find_oid(const unsigned char *ext, size_t ext_len,
      const unsigned char *oid, size_t oid_len, unsigned char **val,
      size_t *len);

   void closeSgxSSL(SSL_CTX *ctx, int severFd, SSL *ssl = NULL);


   int extract_x509_extensions(const X509 *crt, attestation_verification_report_t *attn_report);

   int get_and_decode_ext(const X509 *crt, const unsigned char *oid,
           int oid_len, unsigned char *data, int data_max_len,
           unsigned int *data_len);

   int get_extension(const X509 *crt, const unsigned char *oid,
         int oid_len, const unsigned char **data,
         int *data_len);

   int EVP_DecodeBlock_wrapper(unsigned char *out, const unsigned char *in, int in_len);
public :
   static int buffer_size;
   static int rec_size;
   static unsigned int retryInterval;
   SSLCommHndlr(const std::string &hostname, uint32_t port,
      const std::string &cert_file, const std::string &key_file);
   ~SSLCommHndlr();
   int readSgxSSL(void);
   FILE *readSgxSSLToFileBuffer(std::string &stream_name, char *stream_buffer);
   int sendStreamName(std::string);
};

#endif // #define __SSLCOMMHNDLR_H
