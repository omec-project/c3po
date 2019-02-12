// TODO: add license
// dpdk formatting
// remove dead/commented code
#include "ssl_conn_hdlr.h"

#include <exception>
#include <mbedtls/net.h>
#include <mbedtls/debug.h>
#include "glue.h"
#include "key_store.h"
#include "memmem.h"
#include <sgx_quote.h>

#include "ra.h"
#include "ra-challenger.h"

void print_sgx_quote(sgx_quote_t*);

extern "C" {
void create_key_and_x509(mbedtls_pk_context*, mbedtls_x509_crt*);
}

const int TLSConnectionHandler::ip_str_len = 16;
const int TLSConnectionHandler::enc_str_size = 256;

// ************ helper functions *******************************************

// ****************************************************************************

/**
 * @Name : TLSConnectionHandler
 * @return : Returns none
 * @Description : Function to initialize SSL config and other stuff
 */
TLSConnectionHandler::TLSConnectionHandler(enclave_params_t *params) {
	int ret;

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	unsigned char alloc_buf[100000];
#endif
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_context cache;
#endif

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_init( alloc_buf, sizeof(alloc_buf) );
#endif

#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_init(&cache);
#endif

	mbedtls_entropy_init(&entropy);
	mbedtls_pk_init(&pkey);
	mbedtls_x509_crt_init(&srvcert);
	mbedtls_x509_crt_init(&cachain);
	// create key and cert only once
	create_key_and_x509(&pkey, &srvcert);

        peercert = NULL;
	mbedtls_ssl_config_init(&conf);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	// load CA chain
	// compute buffer length along with NULL terminating character; mbedtls expects length including it.
	size_t ca_chain_length = strlen(ca_bundle) + 1;
	ret = mbedtls_x509_crt_parse(&cachain, (const unsigned char*) ca_bundle,
			ca_chain_length);
	if (ret != 0) {
		mbedtls_printf(
				" failed to load CA chain\n  !  mbedtls_x509_crt_parse returned %d\n\n",
				ret);
		throw std::runtime_error("");
	}
	// set verification to optional.
	// KMS client will have self-signed certificate
	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
	mbedtls_ssl_conf_ca_chain(&conf, &srvcert, NULL);

	/*
	 * 1b. Seed the random number generator
	 */
	//mbedtls_printf("  . Seeding the random number generator...");
	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
			(const unsigned char *) pers.c_str(), pers.length())) != 0) {
		mbedtls_printf(" failed: mbedtls_ctr_drbg_seed returned -0x%04x\n",
				-ret);
		throw std::runtime_error("");
	}
	// mbedtls_printf(" ok\n");

	/*
	 * 1c. Prepare SSL configuration
	 */
	//mbedtls_printf("  . Setting up the SSL data....");
	if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER,
			MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		mbedtls_printf(
				" failed: mbedtls_ssl_config_defaults returned -0x%04x\n",
				-ret);
		throw std::runtime_error("");
	}

	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

	/*
	 * setup debug
	 */
	mbedtls_ssl_conf_dbg(&conf, mydebug, NULL);
	// if debug_level is not set (could be set via other constructors), set it to 0
	if (debug_level < 0) {
		debug_level = 0;
	}
	mbedtls_debug_set_threshold(debug_level);

	/* mbedtls_ssl_cache_get() and mbedtls_ssl_cache_set() are thread-safe if
	 * MBEDTLS_THREADING_C is set.
	 */
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_conf_session_cache(&conf, &cache, mbedtls_ssl_cache_get,
			mbedtls_ssl_cache_set);
#endif

	//mbedtls_ssl_conf_ca_chain(&conf, &cachain, NULL);
	if ((ret = mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n",
				ret);
		throw std::runtime_error("");
	}

	// load key store master key
	ks.set_store_path(params->store_path);
	if (!ks.load_fs_key()) {
		throw std::runtime_error("failed to load master key");
	}

	// print my own measurements
	mbedtls_printf("\n =========== My measurements =======================\n");
	sgx_quote_t qt = { 0, };
	get_quote_from_cert1(srvcert.raw.p, srvcert.raw.len, &qt);
	print_sgx_quote(&qt);
	mbedtls_printf("\n ===================================================\n\n");
}

/**
 * @Name : ~TLSConnectionHandler
 * @return : Returns none
 * @Description : Function to free SSL config and Cerificate
 */
TLSConnectionHandler::~TLSConnectionHandler() {
	mbedtls_x509_crt_free(&srvcert);
	mbedtls_x509_crt_free(peercert);
	mbedtls_pk_free(&pkey);
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_free (&cache);
#endif
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	mbedtls_ssl_config_free(&conf);

	sgx_thread_mutex_destroy(&mutex);

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_free();
#endif

#if defined(_WIN32)
	mbedtls_printf( "  Press Enter to exit this program.\n" );
	fflush( stdout ); getchar();
#endif
}

/**
 * @Name : handle
 * @arguments : [In] thread_id
 *		[In] thread_info
 * @return : Returns none
 * @Description : Function to handle Dealer-In and Dealer-Out
 */
void TLSConnectionHandler::handle(long int thread_id,
		thread_info_t *thread_info) {
	int ret, len;
	mbedtls_net_context *client_fd = &thread_info->client_fd;
	unsigned char buf[1024];
	mbedtls_ssl_context ssl;
	char *key = NULL;

	// thread local data
	mbedtls_ssl_config _conf;
	memcpy(&_conf, &this->conf, sizeof(mbedtls_ssl_config));
	thread_info->config = &_conf;
	thread_info->thread_complete = 0;

	/* Make sure memory references are valid */
	mbedtls_ssl_init(&ssl);

	//mbedtls_printf("  [ #%ld ]  Setting up SSL/TLS data\n", thread_id);

	/*
	 * 4. Get the SSL context ready
	 */
	if ((ret = mbedtls_ssl_setup(&ssl, thread_info->config)) != 0) {
		mbedtls_printf(
				"  [ #%ld ]  failed: mbedtls_ssl_setup returned -0x%04x\n",
				thread_id, -ret);
		goto thread_exit;
	}

	if ((ret = mbedtls_ssl_set_client_transport_id(&ssl, thread_info->client_ip,
			thread_info->ip_len)) != 0) {
		mbedtls_printf(
				"mbedtls_ssl_set_client_transport_id() returned -0x%x\n\n",
				-ret);
		goto thread_exit;
	}

	//mbedtls_printf("client_fd is %d\n", client_fd->fd);
	mbedtls_ssl_set_bio(&ssl, client_fd, mbedtls_net_send_ocall,
			mbedtls_net_recv_ocall, NULL);

	/*
	 * 5. Handshake
	 */
	//mbedtls_printf("  [ #%ld ]  Performing the SSL/TLS handshake\n", thread_id);
	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ
				&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			mbedtls_printf(
					"  [ #%ld ]  failed: mbedtls_ssl_handshake returned -0x%04x\n",
					thread_id, -ret);
			goto thread_exit;
		}
	}

	//mbedtls_printf("  [ #%ld ]  ok\n", thread_id);

   //TODO: Check if standby KMS is requesting for master key.
	// verify client certificate before actual read/write happens
	if (!VerifyCertificate(&ssl)) {
		mbedtls_printf("VerifyCertificate: failed!!!!!");
		goto thread_exit;
	}
	/*
	 * 6. Read the HTTP Request
	 */
	mbedtls_printf("  [ #%ld ]  < Read from client\n", thread_id);

	do {
		len = sizeof(buf) - 1;
		memset(buf, 0, sizeof(buf));
		ret = mbedtls_ssl_read(&ssl, buf, len);

		if (ret == MBEDTLS_ERR_SSL_WANT_READ
				|| ret == MBEDTLS_ERR_SSL_WANT_WRITE)
			continue;

		if (ret <= 0) {
			switch (ret) {
			case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
				mbedtls_printf("  [ #%ld ]  connection was closed gracefully\n",
						thread_id);
				goto thread_exit;

			case MBEDTLS_ERR_NET_CONN_RESET:
				mbedtls_printf("  [ #%ld ]  connection was reset by peer\n",
						thread_id);
				goto thread_exit;

			default:
				mbedtls_printf(
						"  [ #%ld ]  mbedtls_ssl_read returned -0x%04x\n",
						thread_id, -ret);
				goto thread_exit;
			}
		}

		len = ret;
		mbedtls_printf("  [ #%ld ]  %d bytes read\n=====\n%s\n=====\n",
				thread_id, len, (char *) buf);

		if (ret > 0)
			break;
	} while (1);

	key = (char*) malloc(DEFAULT_KEY_SIZE + 1);
	memset(key, 0, DEFAULT_KEY_SIZE + 1);
	if (ks.get_key((char*) buf, key) != 0) {
		mbedtls_printf("  [ #%ld ]  failed to get key\n", thread_id);
		goto thread_exit;
	}

	/*
	 * 7. Write the Response
	 */
	mbedtls_printf("  [ #%ld ]  > Write to client:\n", thread_id);

	memcpy(buf, key, DEFAULT_KEY_SIZE);
	buf[DEFAULT_KEY_SIZE] = 0;
	len = DEFAULT_KEY_SIZE;

	while ((ret = mbedtls_ssl_write(&ssl, buf, len)) <= 0) {
		if (ret == MBEDTLS_ERR_NET_CONN_RESET) {
			mbedtls_printf("  [ #%ld ]  failed: peer closed the connection\n",
					thread_id);
			goto thread_exit;
		}

		if (ret != MBEDTLS_ERR_SSL_WANT_READ
				&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			mbedtls_printf(
					"  [ #%ld ]  failed: mbedtls_ssl_write returned -0x%04x\n",
					thread_id, ret);
			goto thread_exit;
		}
	}

	len = ret;
	mbedtls_printf("  [ #%ld ]  %d bytes written\n=====\n%s\n=====\n",
			thread_id, len, (char *) buf);

	mbedtls_printf("  [ #%ld ]  . Closing the connection...", thread_id);

	while ((ret = mbedtls_ssl_close_notify(&ssl)) < 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ
				&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			mbedtls_printf(
					"  [ #%ld ]  failed: mbedtls_ssl_close_notify returned -0x%04x\n",
					thread_id, ret);
			goto thread_exit;
		}
	}

	//mbedtls_printf(" ok\n");

	ret = 0;

	thread_exit:

#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		mbedtls_printf("  [ #%ld ]  Last error was: -0x%04x - %s\n\n",
				thread_id, -ret, error_buf);
	}
#endif

	//mbedtls_printf(" yo cleanup\n");

	mbedtls_ssl_free(&ssl);

	thread_info->config = NULL;
	thread_info->thread_complete = 1;
}

const string TLSConnectionHandler::pers = "ssl_pthread_server";
sgx_thread_mutex_t TLSConnectionHandler::mutex = SGX_THREAD_MUTEX_INITIALIZER;

void TLSConnectionHandler::mydebug(void *ctx, int level, const char *file,
		int line, const char *str) {
	(void) ctx;
	(void) level;
	long int thread_id = 0;
	sgx_thread_mutex_lock(&mutex);

	mbedtls_printf("%s:%04d: [ #%ld ] %s", file, line, thread_id, str);

	sgx_thread_mutex_unlock(&mutex);
}

bool TLSConnectionHandler::VerifyCertificate(mbedtls_ssl_context *ssl) {
	char buf[1024];

	mbedtls_printf("VerifyCertificate(): verifying certificate ...");
	mbedtls_x509_crt * peercert;
	peercert = (mbedtls_x509_crt *) mbedtls_ssl_get_peer_cert(ssl);

	//uncomment following line to by pass certificate verification (useful when SGX_MODE=SIM)
	//return true;

	if (peercert != NULL) {
		// print certificate information
		mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", peercert);
		mbedtls_printf("Cert Info: %s\n", buf);

		int r = verify_sgx_cert_extensions(peercert->raw.p, peercert->raw.len);
		if (r != 0) {
			mbedtls_printf("failed to verify the SGX report\n");
			return false;
		}

		sgx_quote_t q = { 0, };
		get_quote_from_cert1(peercert->raw.p, peercert->raw.len, &q);
		print_sgx_quote(&q);

		//************ SGX parameter verification *************************

		// validate MRENCLAVE
		char mr_enclave[SGX_HASH_SIZE * 2 + 1] = {0};
		for (int i = 0; i < SGX_HASH_SIZE; ++i) {
			mbedtls_snprintf(mr_enclave + i * 2, SGX_HASH_SIZE, "%02x",
					q.report_body.mr_enclave.m[i]);
		}

		int index = 0;
		int measurement_count = sizeof(dealerMeasurements)/sizeof(dealerMeasurements[0]);
		bool found = false;

      // validate MRENCLAVE
		for ( ; index < measurement_count; index++) {
			if (strncmp(dealerMeasurements[index].mrenclave,
					mr_enclave, SGX_HASH_SIZE * 2) == 0) {
				found = true;
				break;
			}
		}

		if (!found) {
			mbedtls_printf("Key requested from untrusted client!\n");
			mbedtls_printf("Provided mrenclave %s\n", mr_enclave);
			return false;
		}

		// validate MRSIGNER
		char mr_signer[200];
		memset(mr_signer, 0, 200);
		for (int i = 0; i < SGX_HASH_SIZE; ++i) {
			mbedtls_snprintf(mr_signer + i * 2, 200, "%02x",
					q.report_body.mr_signer.m[i]);
		}

		if (strcmp(mr_signer, dealerMeasurements[index].mrsigner) != 0) {
			mbedtls_printf("Key requested from untrusted client!\n");
			mbedtls_printf(
					"Whitelisted signer: %s\n %d Rejecting the key request!\n\n",
					dealerMeasurements[index].mrsigner, index);
			return false;
		}

		// validate ProductID
		if (q.report_body.isv_prod_id != dealerMeasurements[index].prodid) {
			mbedtls_printf(
					"ProductID verification failed!!\nExpected ProductID: %u, provided ProductID: %u\n\n",
					dealerMeasurements[index].prodid, q.report_body.isv_prod_id);
			return false;
		}

		// validate ISVSVN
		if (q.report_body.isv_svn != dealerMeasurements[index].isvsvn) {
			mbedtls_printf(
					"ISV SVN verification failed!!\nExpected SVN: %u, provided SVN: %u\n\n",
					dealerMeasurements[index].isvsvn, q.report_body.isv_svn);
			return false;
		}

		mbedtls_printf("SGX report verified successfully!!\n\n");
		return true;
	} else {
		mbedtls_printf(
				"Certificate verification failed!!\n Failed to get peer certificate!\n\n");
		return false;
	}
}

void print_sgx_quote(sgx_quote_t *q) {
	mbedtls_printf("\n======= SGX Quote ========\n");
	mbedtls_printf("  . MRENCLAVE = ");
	for (int i = 0; i < SGX_HASH_SIZE; ++i)
		mbedtls_printf("%02x", q->report_body.mr_enclave.m[i]);
	mbedtls_printf("\n");

	mbedtls_printf("  . MRSIGNER  = ");
	for (int i = 0; i < SGX_HASH_SIZE; ++i)
		mbedtls_printf("%02x", q->report_body.mr_signer.m[i]);
	mbedtls_printf("\n");

	mbedtls_printf("  . ISVSVN  = ");
	mbedtls_printf("%02x\n", q->report_body.isv_svn);
	//mbedtls_printf("MRENCLAVE: %s\n", q->report_body.mr_enclave.m);
	//mbedtls_printf("MRSIGNER: %s\n", q->report_body.mr_signer.m);
	mbedtls_printf("============================\n");
}
