#include "Enclave_t.h"
#include "ssl_conn_hdlr.h"

TLSConnectionHandler* connectionHandler;

void ssl_conn_init(enclave_params_t *params) {
  connectionHandler = new TLSConnectionHandler(params);
}

void ssl_conn_handle(long int thread_id, thread_info_t* thread_info) {
  connectionHandler->handle(thread_id, thread_info);
}

void ssl_conn_teardown(void) {
  delete connectionHandler;
}
