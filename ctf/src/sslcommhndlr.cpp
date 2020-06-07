/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "sslcommhndlr.h"

#define OID(N) {0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF8, 0x4D, 0x8A, 0x39, N}
const uint8_t ias_response_body_oid[] = OID(0x02);

/**
 * @Name : SSLCommHndlr [Constructor]
 * @arguments : [In] hostname SGX server IP
 *    [In] port SGX server port
 *    [In] cert_file Client certificate
 *    [In] key_file Client private key
 * @return : Returns a handle
 * @Description : Constructor member function to initialize
 * SSL context parameter
 */
SSLCommHndlr::SSLCommHndlr(const std::string &hostname, uint32_t port,
   const std::string &cert_file, const std::string &key_file)
{
   if (hostname.empty() || cert_file.empty() || key_file.empty())
      throw std::runtime_error("SSL object initialization failed");
   
   sslHandle = connectSgxSSL(hostname, port, cert_file, key_file);
   
   if (sslHandle == NULL)
      throw std::runtime_error("SSL object initialization failed");
}

/**
 * @Name : ~SSLCommHndlr [Destructor]
 * @arguments : None
 * @return : Returns None
 * @Description : Destructor member function to close SSL connection
 */
SSLCommHndlr::~SSLCommHndlr()
{
   closeSgxSSL(sslHandle->ctx, SSL_get_fd(sslHandle), sslHandle);
}

/**
 * @Name : initCTX
 * @arguments : None
 * @return : Returns a SSL context
 * @Description : Fuction to initialize SSL context
 */
SSL_CTX* SSLCommHndlr::initCTX(void)
{
   SSL_CTX *ctx;
   SSL_library_init();
   OpenSSL_add_all_algorithms();
   SSL_load_error_strings();
   ctx = SSL_CTX_new(SSLv23_client_method());
   
   if (ctx == NULL)
   {
      std::cout << "Creation of SSL context object failed" << std::endl;
      return NULL;
   }
   
   return ctx;
}

/**
 * @Name : verify_sgx_cert_info
 * @arguments : [In] mrenclave
 *    [In] mrsigned
 *    [In] isv_svn
 * @return : Returns 0 on success otherwise -1
 * @Description : Function to verify certificate extracted enclave parameters
      with Configuration file parametes
 */
int SSLCommHndlr::verify_sgx_cert_info(uint8_t mrenclave[],
   uint8_t mrsigner[], sgx_isv_svn_t isv_svn)
{
   char mr_enclave[SGX_HASH_SIZE * 2 + 1] = {0,};
   char mr_signer[SGX_HASH_SIZE * 2 + 1] = {0,};

#if 1 //TODO: Change to cout
   printf("Certificate's SGX information: \n");
   printf("MRENCLAVE = ");

   for (int i = 0; i < SGX_HASH_SIZE; ++i)
      printf("%02x", mrenclave[i]);
   printf("\n");

   printf("MRSIGNER  = ");
   for (int i = 0; i < SGX_HASH_SIZE; ++i)
      printf("%02x", mrsigner[i]);
   printf("\n");

   printf("ISVSVN  = ");
   printf("%02x\n", isv_svn);
   printf("\n");
#endif

   for (int i=0; i < SGX_HASH_SIZE; ++i)
   {
      sprintf(mr_enclave + i*2, "%02x", mrenclave[i]);
      sprintf(mr_signer + i*2, "%02x", mrsigner[i]);
   }

   if ((strcmp(mr_enclave, Options::mrEnclave().c_str()) != 0) ||
      (strcmp(mr_signer, Options::mrSigner().c_str()) != 0) ||
      (Options::isvSvn() != isv_svn))
   {
         std::cout << "It's not trusted Dealer, Disconnecting" << std::endl;
         return -1;
   }
   
   return 0;
}

/**
 * @Name : find_oid
 * @arguments : [In] ext
 *    [In] ext_len
 *    [In] oid
 *    [In] oid_len
 *    [In] val
 *    [In] len
 * @return : Returns 0 on success otherwise -1
 * @Description : Function to find oid
 */
int SSLCommHndlr::find_oid(const unsigned char *ext, size_t ext_len,
   const unsigned char *oid, size_t oid_len, unsigned char **val, size_t *len)
{
   uint8_t *p = (uint8_t *) memmem(ext, ext_len, oid, oid_len);
   
   if (p == NULL)
      return -1;

   p += oid_len;

   int i = 2;

   *len  =  p[i++] << 8;
   *len +=  p[i++];
   *val  = &p[i++];
   
   return 0;
}

#if 0
/**
 * @Name : extract_x509_extension
 * @arguments : [In] ext
 *    [In] ext_len
 *    [In] oid
 *    [In] oid_len
 *    [In] data
 *    [In] data_len
 *    [In] data_max_len
 * @return : Returns 0 on success otherwise -1
 * @Description : Function to extract values from certificate extension
 */
int SSLCommHndlr::extract_x509_extension(uint8_t *ext, int ext_len,
   const uint8_t *oid, size_t oid_len, uint8_t *data, uint32_t *data_len,
   uint32_t data_max_len)
{
   uint8_t *base64_data;
   size_t base64_data_len;

   if (find_oid(ext, ext_len, oid, oid_len, &base64_data, &base64_data_len) == -1)
      return -1;

   if (base64_data == NULL || base64_data_len > data_max_len)
      return -1;

   size_t out_len;
   int ret;
   ret = mbedtls_base64_decode(data, data_max_len, &out_len, base64_data,
      base64_data_len);

   if (ret != 0 || out_len > UINT32_MAX)
      return -1;
   
   *data_len = (uint32_t) out_len;
   
   return 0;
}

/**
 * @Name : get_quote_from_report
 * @arguments : [In] report
 *    [In] report_len
 *    [In] quote
 * @return : Returns 0 on success otherwise -1
 * @Description : Function to get quote from report
 */
int SSLCommHndlr::get_quote_from_report(const uint8_t *report,
   const int report_len, sgx_quote_t *quote)
{
   (void) report_len;

   const char *json_string = "\"isvEnclaveQuoteBody\":\"";
   char *p_begin = strstr((char *) report, json_string);

   if (p_begin == NULL)
      return -1;
   
   p_begin += strlen(json_string);

   const char *p_end = strchr(p_begin, '"');
   
   if (p_end == NULL)
      return -1;

   const int quote_base64_len = p_end - p_begin;
   uint8_t *quote_bin = (uint8_t *) malloc(quote_base64_len);
   
   if (quote_bin == NULL)
      return -1;

   size_t quote_bin_len = quote_base64_len;
   
   mbedtls_base64_decode(quote_bin, quote_base64_len,
                         &quote_bin_len,
                         (unsigned char*) p_begin, quote_base64_len);

   if (quote_bin_len > sizeof(sgx_quote_t))
   {
      free(quote_bin);
      return -1;
   }

   memset(quote, 0, sizeof(sgx_quote_t));
   memcpy(quote, quote_bin, quote_bin_len);
   free(quote_bin);
   
   return 0;
}

/**
 * @Name : get_quote_from_extension
 * @arguments : [In] ext
 *    [In] ext_len
 *    [In] quote
 * @return : Returns 0 on success otherwise -1
 * @Description : Function to get quote from extension
 */
int SSLCommHndlr::get_quote_from_extension(uint8_t *ext, size_t ext_len,
   sgx_quote_t *quote)
{
   uint8_t report[2048] = {0};
   uint32_t report_len;

   if (extract_x509_extension(ext, ext_len, ias_response_body_oid,
      sizeof(ias_response_body_oid), report, &report_len, sizeof(report)) == -1)
      return -1;

   if (get_quote_from_report(report, report_len, quote) == -1)
      return -1;
   
   return 0;
}

/**
 * @Name : get_quote_from_cert
 * @arguments : [In] der_crt
 *    [In] der_crt_len
 *    [In] q SGX quote
 * @return : Returns none
 * @Description : Function to get quote from certificate
 */
int SSLCommHndlr::get_quote_from_cert(const uint8_t *der_crt,
   uint32_t der_crt_len, sgx_quote_t *q)
{
   mbedtls_x509_crt crt;
   mbedtls_x509_crt_init(&crt);
   mbedtls_x509_crt_parse(&crt, der_crt, der_crt_len);
   
   if (get_quote_from_extension(crt.v3_ext.p, crt.v3_ext.len, q) == -1)
   {
      mbedtls_x509_crt_free(&crt);
      return -1;
   }
   
   mbedtls_x509_crt_free(&crt);
   
   return 0;
}
#endif

/**
 * @Name : verify_cert
 * @arguments : [In] crt Peer(Server) certificate
 * @return : Returns 0 on success otherwise -1
 * @Description : Function to print certificate information(i.e. MRSIGNER,
      MRENCLAVE and ISV_SVN)
 */
int SSLCommHndlr::verify_cert(X509* crt)
{
   /*
   int der_len = i2d_X509(crt, NULL);

   unsigned char der[der_len] = {0};
   unsigned char *p = der;
   i2d_X509(crt, &p);
   */


   if (Options::mrSigner().empty() || (Options::mrEnclave().empty()))
   {
      std::cout << "Dealer is running in Simulation mode, No need of SGX parameter verification" << std::endl;
      return 0;
   }

   /*
   if (get_quote_from_cert(der, der_len, &quote) == -1)
      return -1;
   */

   attestation_verification_report_t attn_report;

   if (extract_x509_extensions(crt, &attn_report) != 0)
      return -1;

   sgx_quote_t quote = {0, };

   if (get_quote_from_report(attn_report.ias_report, attn_report.ias_report_len, &quote) != 0)
      return -1;

   sgx_report_body_t *body = &quote.report_body;

   if (verify_sgx_cert_info(body->mr_enclave.m, body->mr_signer.m, body->isv_svn) == -1)
      return -1;
 
   std::cout << "Connected to trusted SGX Dealer-Out" << std::endl;

   return 0;
}

/**
 * @Name : verifyCertificate
 * @arguments : [In] ssl
 * @return : Returns integer to indicate success/failure
 * @Description : Function get the certificate from server and verify it
 */
int SSLCommHndlr::verifyCertificate(SSL* ssl)
{
   X509 *cert;
   int i;
   char *line = NULL;
   char hash[32] = {0};
   SHA256_CTX sha256;
   SHA256_Init(&sha256);

   cert = SSL_get_peer_certificate(ssl);
   
   if (cert != NULL)
   {
      if (verify_cert(cert) == -1)
      {
         return -1;
      }
      X509_free(cert);					
   }
   else 
   {
      std::cout << "verifyCertificate() : Certificate is not given by Peer" <<
         std::endl;
      return -1;
   }
   
   return 0;
}

/**
 * @Name : closeSgxSSL
 * @arguments : [In] ctx
 *    [In] serverFd
 *    [In] ssl
 * @return : Returns none
 * @Description : Function to free SSL context and terminates connection
 */
void SSLCommHndlr::closeSgxSSL(SSL_CTX *ctx, int serverFd, SSL *ssl)
{
   if (ssl != NULL)
      SSL_free(ssl);
   if (serverFd != -1)
      close(serverFd);
   if (ctx != NULL)
      SSL_CTX_free(ctx);
}

/**
 * @Name : openConnection
 * @arguments : [In] hostname
 *    [In] port
 * @return : Returns a server handle
 * @Description : Fuction to open socket connection
 */
int SSLCommHndlr::openConnection(const std::string &hostname, uint32_t port)
{
   int serverDescriptor;
   struct hostent *host = NULL;
   struct sockaddr_in addr;

   if ((host = gethostbyname(hostname.c_str())) == NULL )
   {
      std::cout << "gethostbyname() failed : " << hstrerror(h_errno);
      return -1;
   }
   
   serverDescriptor = socket(PF_INET, SOCK_STREAM, 0);
   
   if (serverDescriptor != -1)
   {
      bzero(&addr, sizeof(addr));
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = *(long*)(host->h_addr);
      if (connect(serverDescriptor,(const struct sockaddr *)&addr,
         sizeof(addr)) != 0 )
      {
         std::cout << "openConnection() failed : " << strerror(errno) << std::endl;
         return -1;
      }
   }

   return serverDescriptor;
}

/**
 * @Name : connectSgxSSL
 * @arguments : [In] hostname SGX server IP
 *    [In] portnum SGX server port
 *    [In] client_cert_file Client certificate
 *    [In] priv_key_file Client private key
 * @return : Returns none
 * @Description : Function to connect to SGX over SSL
 */
SSL *SSLCommHndlr::connectSgxSSL(const std::string &hostname,
   uint32_t portnum, const std::string &client_cert_file,
   const std::string &priv_key_file)
{
   SSL *ssl = NULL;
   SSL_CTX *ctx = NULL;
   int serverFd = -1, retval = -1;

   if ((ctx = initCTX()) == NULL)
   {
      return NULL;
   }

   if ((serverFd = openConnection(hostname, portnum)) == -1)
   {
      std::cout << "Error in connecting to SGX Dealer over SSL" << std::endl;
      SSL_CTX_free(ctx);
      return NULL;
   }

   if (SSL_CTX_use_certificate_file(ctx, client_cert_file.c_str(),
      SSL_FILETYPE_PEM) <= 0)
   {
      closeSgxSSL(ctx, serverFd);
      return NULL;
   }

   if (SSL_CTX_use_PrivateKey_file(ctx, priv_key_file.c_str(),
      SSL_FILETYPE_PEM) <= 0)
   {
      closeSgxSSL(ctx, serverFd);
      return NULL;
   }

   if (!SSL_CTX_check_private_key(ctx))
   {
      closeSgxSSL(ctx, serverFd);
      return NULL;
   }

   ssl = SSL_new(ctx);				
   if (ssl == NULL)
   {
      std::cout << "Creation of new SSL failed" << std::endl;
      closeSgxSSL(ctx, serverFd);
      return NULL;
   }

   SSL_set_fd(ssl, serverFd);
   if ((retval = SSL_connect(ssl)) != 1 )
   {
      std::cout << "SSL connect failed with error : " <<
         SSL_get_error(ssl, retval) << std::endl;
      closeSgxSSL(ctx, serverFd, ssl);
      return NULL;
   }

   //std::cout << "Connected to SGX with " << SSL_get_cipher(ssl) <<
   //   "encryption" << std::endl;
   
   if (verifyCertificate(ssl) == -1)
   {
      closeSgxSSL(ctx, serverFd, ssl);
      return NULL;
   }
   
   return ssl;
}

/**
 * @Name : readSgxSSL
 * @arguments : None
 * @return : Returns number of bytes read successfully
 * @Description : Function to read records from SGX socket
 */
int SSLCommHndlr::readSgxSSL()
{

   // send sream name
   std::string name = Options::streamName();

   std::cout << "DEBUG: sending stream name " << name <<" to dealer" << std::endl;
   if(sendStreamName(name) <= 0)
   {
       return -1;
   }
   std::cout << "DEBUG: stream name " << name <<" sent to dealer" << std::endl;

   char buffer[MAX_BUFF_SIZE] = {0};
   int received = 0, error_code = -1;

   while (1)
   {
      ERR_clear_error();
      
      while ((received = SSL_read(sslHandle, buffer, MAX_REC_SIZE)) <= 0)
      {
         error_code = SSL_get_error(sslHandle, received);
         
         if (error_code == SSL_ERROR_WANT_READ || error_code == SSL_ERROR_WANT_WRITE)
               continue;
         
         switch(error_code)
         {
            case SSL_ERROR_SYSCALL :
               // Dealer killed
               std::cout << "Connection dropped by Dealer" << std::endl;
               std::cout << "Wait, retrying on connection refused" << std::endl;
               retryConnection();
               break;
            case SSL_ERROR_ZERO_RETURN :
               // Poll timeout 
               std::cout << "Connection closed by Dealer because poll timeout occured" << std::endl;
               std::cout << "Wait, retrying for connection" << std::endl;
               retryConnection();
               break;
            default:
               std::cout << "ssl_read() failed with error : " << error_code <<
                  std::endl;
               return -1;
         }
      }
      // this will print each message received from router, use for debugging only
      std::cout << "Received buffer :" << buffer << std::endl;
   }

   return received;
}

/**
 * @Name : readSgxSSLFileBuffer
 * @arguments : csv_file_name
 * @return : Returns FILE *
 * @Description : Function to read records from SGX socket
 */
FILE* SSLCommHndlr::readSgxSSLToFileBuffer(std::string &stream_name, char *stream_buffer)
{

   int error_flag = 0;

   std::cout << "DEBUG: sending stream name " << stream_name <<" to dealer" << std::endl;
   if(sendStreamName(stream_name) <= 0)
      return NULL;

   std::cout << "DEBUG: stream name " << stream_name <<" sent to dealer" << std::endl;

   char per_message_buffer[MAX_BUFF_SIZE] = {0,};
   int received_so_far = 0, received = 0, error_code = -1;

   while (1)
   {
      received = SSL_read(sslHandle, per_message_buffer, MAX_REC_SIZE);

      if (received > 0)
      {
         received_so_far += received;
         stream_buffer = (char *)realloc(stream_buffer,received_so_far);

         if (!stream_buffer)
         {
            std::cout << "realloc for extending the dynamic file buffer failed\n" << std::endl;
            return NULL;
         }

         memcpy(stream_buffer + received_so_far - received, per_message_buffer, received);

         // this will print each message received from router, use for debugging only
         //std::cout << "Received buffer :" << per_message_buffer << std::endl;

         continue;
      }

      error_code = SSL_get_error(sslHandle, received);

      if (error_code == SSL_ERROR_WANT_READ || error_code == SSL_ERROR_WANT_WRITE) {
         continue;
      }

      switch(error_code)
      {
         case SSL_ERROR_SYSCALL :
            // Dealer killed
            std::cout << "Connection dropped by Dealer" << std::endl;
            ERR_clear_error();
            error_flag = 1;
            break;
         case SSL_ERROR_ZERO_RETURN :
            std::cout << "Connection closed by Dealer because stream"
                  " transfer is complete" << std::endl;
            ERR_clear_error();
            error_flag = 1;
            break;
         default:
            std::cout << "ssl_read() failed with error : " << error_code <<
                  std::endl;
            ERR_clear_error();
            error_flag = 1;
            break;
      }

      std::cout << "Total Size received " << received_so_far << std::endl;
      return fmemopen(stream_buffer, received_so_far, "r");

   }
   std::cout << "Total Size received " << received_so_far << std::endl;
   return fmemopen(stream_buffer, received_so_far, "r");
}


/**
 * @Name : sendStreamName
 * @arguments : stream name
 * @return : Returns number of bytes written to SSL connection
 * @Description : Function to set stream name for reading
 */
int SSLCommHndlr::sendStreamName(std::string name)
{
   int status = 0, error_code = -1;

      // clean SSL error before calling write
      ERR_clear_error();
      while ((status = SSL_write(sslHandle, name.c_str(), name.length())) <= 0)
      {
         error_code = SSL_get_error(sslHandle, status);
         
         if (error_code == SSL_ERROR_WANT_READ || error_code == SSL_ERROR_WANT_WRITE)
               continue;
         
         switch(error_code)
         {
            case SSL_ERROR_SYSCALL :
               // Dealer killed
               std::cout << "Connection dropped by Dealer" << std::endl;
               std::cout << "Wait, retrying on connection refused" << std::endl;
               retryConnection();
               break;
            case SSL_ERROR_ZERO_RETURN :
               // Poll timeout 
               std::cout << "Connection closed by Dealer" << std::endl;
               std::cout << "Wait, retrying for connection" << std::endl;
               retryConnection();
               break;
            default:
               std::cout << "ssl_write() failed with error : " << error_code <<
                  std::endl;
               return -1;
         }
      }

   if(status >0)
   {
      std::cout << "INFO : stream name send to dealer is " << name << std::endl;
   }

   return status;
}

/**
 * @Name : retryConnection
 * @return : Returns 0 on successful reconnect
 * @Description : Function to reconnect to SGX Dealer-Out
 */
int SSLCommHndlr::retryConnection()
{
   closeSgxSSL(sslHandle->ctx, SSL_get_fd(sslHandle), sslHandle);
   sslHandle = NULL;
   
   while (sslHandle == NULL)
   {
      sleep(RETRY_INTERVAL);
      
      sslHandle = connectSgxSSL(Options::sgxserverIp(),
         Options::sgxserverPort(), Options::certFile(),
         Options::privkeyFile());
   }

   // send sream name
   std::string name = Options::streamName();
   std::cout << "DEBUG: sending stream name " << name <<" to dealer (after connection retry)" << std::endl;
   if(sendStreamName(name) <= 0)
   {
       return -1;
   }

   return 0;
}

int SSLCommHndlr::extract_x509_extensions(const X509 *crt, attestation_verification_report_t *attn_report)
{
   bzero(attn_report, sizeof(*attn_report));
   size_t ias_oid_len = sizeof(ias_response_body_oid);
   
   if (get_and_decode_ext(crt,
         ias_response_body_oid + 2, ias_oid_len - 2,
         attn_report->ias_report, sizeof(attn_report->ias_report),
         &attn_report->ias_report_len) != 0)
      return -1;
   
   return 0;
}

int SSLCommHndlr::get_and_decode_ext(const X509 *crt, const unsigned char *oid,
        int oid_len, unsigned char *data, int data_max_len,
        unsigned int *data_len)
{
   const unsigned char *ext;
   int ext_len;

   if (get_extension(crt, oid, oid_len, &ext, &ext_len) != 0)
      return -1;

   if (ext_len * 3 > data_max_len * 4)
      return -1;

   int ret = EVP_DecodeBlock_wrapper(data, ext, ext_len);

   if (ret == -1)
      return -1;

   *data_len = ret;
   return 0;
}


int SSLCommHndlr::get_extension(const X509 *crt, const unsigned char *oid,
      int oid_len, const unsigned char **data,
      int *data_len)
{
  STACK_OF(X509_EXTENSION) *exts = crt->cert_info->extensions;

  int num_of_exts;

  if (exts)
     num_of_exts = sk_X509_EXTENSION_num(exts);
  else
     num_of_exts = 0;

  if (num_of_exts < 0)
     return -1;

  for (int i = 0; i < num_of_exts; i++) {
     X509_EXTENSION *ex = sk_X509_EXTENSION_value(exts, i);

     if (ex == NULL)
        return -1;

     ASN1_OBJECT *obj = X509_EXTENSION_get_object(ex);

     if (obj == NULL)
        return -1;

     if (oid_len != obj->length)
        continue;

     if (memcmp(obj->data, oid, obj->length) == 0) {
        *data = ex->value->data;
        *data_len = ex->value->length;
        break;
     }
  }
  return 0;
}


int SSLCommHndlr::EVP_DecodeBlock_wrapper(unsigned char *out,
   const unsigned char *in, int in_len)
{
  unsigned char buf[in_len];

  int ret = EVP_DecodeBlock(buf, in, in_len);

  if (ret == -1)
     return ret;

  if (in[in_len-1] == '=' && in[in_len-2] == '=')
     ret -= 2;
  else if (in[in_len-1] == '=')
     ret -= 1;

  memcpy(out, buf, ret);
  return ret;
}

int SSLCommHndlr::get_quote_from_report(const uint8_t *report,
      const int report_len, sgx_quote_t *quote)
{
  char buf[report_len + 1];

  memcpy(buf, report, report_len);
  buf[report_len] = '\0';

  const char *json_string = "\"isvEnclaveQuoteBody\":\"";
  char *p_begin = strstr(buf, json_string);

  if (p_begin == NULL)
     return -1;

  p_begin += strlen(json_string);

  const char *p_end = strchr(p_begin, '"');

  if (p_end == NULL)
     return -1;

  const int quote_base64_len = p_end - p_begin;
  uint8_t *quote_bin = (uint8_t *)malloc(quote_base64_len);
  uint32_t quote_bin_len = quote_base64_len;

  int ret = EVP_DecodeBlock(quote_bin,
        (unsigned char *) p_begin, quote_base64_len);

  if (ret == -1)
     return -1;

  quote_bin_len = ret;

  if (quote_bin_len > sizeof(sgx_quote_t))
     return -1;

  memset(quote, 0, sizeof(sgx_quote_t));
  memcpy(quote, quote_bin, quote_bin_len);
  free(quote_bin);

  return 0;
}

