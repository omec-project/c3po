#include <unistd.h>
#include <string>

#include "Enclave_t.h"
#include "ssl_conn_hdlr.h"
#include "helper.h"

TLSConnectionHandler* connectionHandler;

void ssl_conn_init(long int mode, enclave_params_t *params) {
  try {
    connectionHandler = new TLSConnectionHandler(params);
    connectionHandler->initialize_dealer_mode(mode);
  } catch (std::exception e) {
	return;
  }
}

void ssl_conn_handle(long int thread_id, thread_info_t* thread_info) {
  connectionHandler->handle(thread_id, thread_info);
}

void ssl_conn_teardown(void) {
  delete connectionHandler;
}

void offline_handle(thread_info_t* thread_info)
{
	mbedtls_printf("from enclave : %s and %d \n", thread_info->cdr_router_host, thread_info->cdr_router_port);
	char key[17] = {0};

	if (!connectionHandler)
		return;

	if (connectionHandler->get_key_from_kms(key) != 0) {
		mbedtls_printf("ERROR: Offline mode :failed to get key from KMS\n");
		return;
	}

	mbedtls_ssl_context *ssl=NULL;
	int offline_mode = 1;
	std::string cdrPath;
	std::string cdrArchPath;

	connectionHandler->get_cdr_filepath(cdrPath, cdrArchPath);

	get_msg_out_router_send_to_ctf(1, ssl,
			thread_info->cdr_router_host,
			thread_info->cdr_router_port, key, offline_mode,
			cdrPath, cdrArchPath);
}
