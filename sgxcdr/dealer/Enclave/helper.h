//TODO: Add license
#ifndef HELPER_H
#define HELPER_H

#include <ctime>
#include <string>

#include <mbedtls/net.h>
#include "mbedtls/debug.h"
#include "sgx_tae_service.h"
#include "sgx_tcrypto.h"
#include "../deps/sgx_zmq/sgx_tcdr/include/sgx_tcdr.h"

#define POLL_INTERVAL 10000
#define READ_BUFFER_SIZE 256

enum Modes {IN = 1, OUT};
enum MsgType {MSG_UNKNOWN = -1, MSG_FILENAME = 0, MSG_DATA, MSG_CLOSE_PROTECTED_FILE};
enum zmq_errcodes
{
   SUCCESS_ZMQ = 0,
   ERR_ZMQ_CONN_ROUTER_FAIL,
   ERR_ZMQ_POLL_READ_ROUTER_FAIL,
   ERR_ZMQ_POLL_READ_ROUTER_TIMEOUT,
   ERR_ZMQ_SEND_ROUTER_FAIL,
   ERR_ZMQ_RECV_ROUTER_FAIL,
   ERR_ZMQ_CLOSE_ROTER_FAIL
};

extern "C" {
int get_msg_out_router_send_to_ctf(long int thread_id, mbedtls_ssl_context *ssl,
   char *cdr_out_router_host, int cdr_out_router_port, char *key_from_kms,
   int offline, std::string &cdrPath, std::string &cdrArchPath);
int close_router_conn(zmq_handle* handle, zmq_context *context);
SGX_FILE *open_protected_fs_file(const char *dp_id, char *);
SGX_FILE *open_secure_file(const char *, const char *, char *);
int read_stream_from_file(long int, mbedtls_ssl_context *, char *, char *);
int write_to_protected_fs(SGX_FILE *file_handle, uint8_t *write_buff, size_t len);

int close_protected_fs(SGX_FILE *file_handle);
zmq_handle *conn_to_in_router(char *cdr_router_host, int cdr_router_port,
   char *dealer_identity);
int send_msg_to_in_router(zmq_handle *handle, unsigned char *msg, int len);
int get_message_type(char msg_type);
}


#endif
