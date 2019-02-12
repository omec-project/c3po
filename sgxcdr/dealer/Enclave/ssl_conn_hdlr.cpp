// TODO: add license
// dpdk formatting
// remove dead/commented code
#include "ssl_conn_hdlr.h"

#include <exception>
#include <mbedtls/net.h>
#include <mbedtls/debug.h>
#include "glue.h"
#include "helper.h"
#include "crypto.h"
#include "kms_client.h"

/*
 AAD can not be set as stream name. We support named/unnamed stream for live mode.
 for unnamed stream, there will be no identity at dealer OUT side. unnamed stream
 can match to any named stream.
*/

static Crypto AESGCM(CRYPTO_TYPE_AES_GCM);

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
TLSConnectionHandler::TLSConnectionHandler(enclave_params_t *p) {
	int ret;

	//process enclave parameters
	kmsserver = (char*) p->kmsserver;
	kmsport = (char*) p->kmsport;
	keynamespace = (char*) p->keynamespace;

        peercert = NULL;
	m_cdrPath = string(p->cdrpath);
	m_cdrArchPath = string(p->cdrarchpath);

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

	mbedtls_ssl_config_init(&conf);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	/*
	 * We use only a single entropy source that is used in all the threads.
	 */

	/*
	 * 1. Load the certificates and private RSA key
	 */
	//mbedtls_printf("\n  . Loading the server cert. and key...");
	/*
	 * This demonstration program uses embedded test certificates.
	 * Instead, you may want to use mbedtls_x509_crt_parse_file() to read the
	 * server and CA certificates, as well as mbedtls_pk_parse_keyfile().
	 ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *) mbedtls_test_srv_crt,
	 mbedtls_test_srv_crt_len);
	 if (ret != 0) {
	 mbedtls_printf(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
	 throw std::runtime_error("");
	 }

	 ret = mbedtls_x509_crt_parse(&cachain, (const unsigned char *) mbedtls_test_cas_pem,
	 mbedtls_test_cas_pem_len);
	 if (ret != 0) {
	 mbedtls_printf(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
	 throw std::runtime_error("");
	 }

	 mbedtls_pk_init(&pkey);
	 ret = mbedtls_pk_parse_key(&pkey, (const unsigned char *) mbedtls_test_srv_key,
	 mbedtls_test_srv_key_len, NULL, 0);
	 if (ret != 0) {
	 mbedtls_printf(" failed\n  !  mbedtls_pk_parse_key returned %d\n\n", ret);
	 throw std::runtime_error("");
	 }

	 */

	// load CA chain
	// compute buffer length along with NULL terminating character; mbedtls expects length including it.
	size_t ca_chain_length = strlen(mozilla_ca_bundle) + 1;
	ret = mbedtls_x509_crt_parse(&cachain,
			(const unsigned char*) mozilla_ca_bundle, ca_chain_length);
	if (ret != 0) {
		mbedtls_printf(
				" failed to load CA chain\n  !  mbedtls_x509_crt_parse returned %d\n\n",
				ret);
		throw std::runtime_error("");
	}
	// set verification to optional.
	// DP/CTF will have CA signed certificates but key manager will have self-signed certificate
	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
	mbedtls_ssl_conf_ca_chain(&conf, &cachain, NULL);
	//mbedtls_printf(" ok\n");

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

	dealer_run_mode = 0;

	// request key from KMS
	//mbedtls_printf("\n Enclave Params: %s %s \n\n", params->kms_server, params->key_namespace);
	key_from_kms = (char *) malloc(17);
	memset(key_from_kms, 0, 17);
	if (get_key_from_kms(key_from_kms) != 0) {
		mbedtls_printf("ERROR: Failed to get key from KMS\n");
		throw std::runtime_error("");
	}

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

	free(key_from_kms);

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_free();
#endif

#if defined(_WIN32)
	mbedtls_printf( "  Press Enter to exit this program.\n" );
	fflush( stdout ); getchar();
#endif
}

/**
 * @Name : get_key_from_kms
 * @arguments : [In] identifier
 * @arguments : [Out] key
 * @return : Returns 0 on success, on failure returns error code
 * @Description : Function to get key from KMS
 */
int TLSConnectionHandler::get_key_from_kms(char *key) {
	client_opt_t opt;
	unsigned char buf[1024];
	client_opt_init(&opt);
	opt.debug_level = 1;
	opt.server_name = kmsserver.c_str();
	opt.server_port = kmsport.c_str();
	memcpy(opt.key_namespace, keynamespace.c_str(), KEY_NAMESPACE_LEN);
	// make sure string is NULL terminated
	opt.key_namespace[KEY_NAMESPACE_LEN - 1] = 0;
	opt.auth_mode = MBEDTLS_SSL_VERIFY_OPTIONAL;

	int r = kms_client(opt, NULL, 0, buf, sizeof buf, &pkey, &srvcert,
			&cachain);
	if (r == 0) {
		memcpy(key, buf, KEY_SIZE);
		key[16] = '\0';

		//mbedtls_printf("\nKey retrieved from KMS: ");
		//for(int i=0; i<KEY_SIZE; i++)
		//	mbedtls_printf("%02x ", key[i]);
		//mbedtls_printf("\n");

		return 0;
	}
	return r;
}

/**
 * @Name : initialize_dealer_mode
 * @arguments : [In] run_mode
 *		[In] thread_info
 * @return : Returns none
 * @Description : Function to initialize Dealer mode
 */
void TLSConnectionHandler::initialize_dealer_mode(int run_mode) {
	dealer_run_mode = run_mode;
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
	size_t lenOut;
	mbedtls_net_context *client_fd = &thread_info->client_fd;
	unsigned char buf[1024];
	unsigned char message[1024];
	mbedtls_ssl_context ssl;
	const char *close_msg = "close";
	zmq_handle *handle = NULL;
	zmq_context *context = NULL;
	uint8_t *enc_str = NULL;

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

	// verify client certificate before actual read/write happens
	if (!VerifyCertificate(&ssl)) {
		goto thread_exit;
	}
	/*
	 * 6. Read the HTTP Request
	 */
	//mbedtls_printf("  [ #%ld ]  < Read from client\n", thread_id);
	//mbedtls_printf("  %d  < Read from client\n", dealer_run_mode);
	if (dealer_run_mode == IN) {

		// Generate client IP string
		memset(buf, 0, sizeof(buf));

		/*
		 snprintf(buf, TLSConnectionHandler::ip_str_len, "%d.%d.%d.%d",
		 ssl.cli_id[0], ssl.cli_id[1],
		 ssl.cli_id[2], ssl.cli_id[3]);
		 len = strlen((char *) buf);
		 */

		// read identity from DP
		ret = mbedtls_ssl_read(&ssl, buf, 1024);
		len = strlen((char*) buf);
		switch (ret) {
		case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
			break;
		case 0:
		case MBEDTLS_ERR_NET_CONN_RESET:
			break;
		}
		mbedtls_printf("INFO: identity read from DP: %s\n", buf);

		// set crypto parameters
		AESGCM.set_params_aesgcm((uint8_t *)key_from_kms, buf, len);

		// Prepend message type : 0
		memset(message, 0, sizeof(message));
		message[0] = MSG_FILENAME + '0';
		memcpy(message + 1, buf, len);
		//memcpy(message, buf, len);
		if ((handle = conn_to_in_router(thread_info->cdr_router_host,
				thread_info->cdr_router_port, (char *) message)) == NULL) {
			goto thread_exit;
		}

		if (send_msg_to_in_router(handle, message, strlen((char *) message))
				== -1) {
			goto thread_exit;
		}

		while (1) {
			len = sizeof(buf) - 1;
			memset(buf, 0, sizeof(buf));
			while ((ret = mbedtls_ssl_read(&ssl, buf, len)) <= 0) {

				if (ret == MBEDTLS_ERR_SSL_WANT_READ
						|| ret == MBEDTLS_ERR_SSL_WANT_WRITE)
					continue;

				switch (ret) {
				case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
					mbedtls_printf(
							"  [ #%ld ]  Connection was closed gracefully\n",
							thread_id);
					goto thread_exit;

				case 0:
				case MBEDTLS_ERR_NET_CONN_RESET:
					mbedtls_printf("  [ #%ld ]  Connection was reset by peer\n",
							thread_id);
					memset(message, 0, sizeof(message));
					message[0] = MSG_CLOSE_PROTECTED_FILE + '0';
					memcpy(message + 1, close_msg, strlen(close_msg)); // +1 for msg type
					send_msg_to_in_router(handle, message,
							strlen((char *) message));
					close_router_conn(handle,context);
					goto thread_exit;

				default:
					mbedtls_printf(
							"  [ #%ld ]  mbedtls_ssl_read returned -0x%04x\n",
							thread_id, -ret);
					goto thread_exit;
				}
			} // while

			len = ret;
			//mbedtls_printf("Read data : %s\n=====\n", (char *) buf);
			enc_str = (uint8_t *) malloc(TLSConnectionHandler::enc_str_size);

			if (enc_str == NULL) {
				mbedtls_printf("Memory allocation failed for encryption buffer\n");
				continue;
			}

			memset(enc_str, 0, TLSConnectionHandler::enc_str_size);

			// Prepend msg type - 1 for DATA
			// mbedtls_printf("DEBUG : sending data %s to in-router", buf);
			memset(message, 0, 1024);
			message[0] = MSG_DATA + '0';
			memcpy(message + 1, buf, len);
			enc_str[0] = message[0];
			AESGCM.encrypt((char *) message+1, len, (char *)
					enc_str+1, &lenOut);
			send_msg_to_in_router(handle, enc_str, lenOut + 1);// +1 for msg type
			free(enc_str);
		}	 //WHILE 1
	} else if (dealer_run_mode == OUT) {
		int offline = 0;
		if (get_msg_out_router_send_to_ctf(thread_id, &ssl,
				thread_info->cdr_router_host, thread_info->cdr_router_port,
				key_from_kms, offline, m_cdrPath, m_cdrArchPath)
				== MBEDTLS_ERR_NET_CONN_RESET) {
			goto thread_exit;
		}
	}
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
	uint32_t flags;
	int ret;

	//mbedtls_printf("VerifyCertificate(): verifying certificate ...");
	mbedtls_x509_crt * peercert;
	//mbedtls_x509_crt *peercert=new mbedtls_x509_crt;
	//mbedtls_x509_crt_init(peercert);
	peercert = (mbedtls_x509_crt *) mbedtls_ssl_get_peer_cert(ssl);
	if (peercert != NULL) {
		ret = mbedtls_x509_crt_verify(peercert, &cachain, NULL, NULL, &flags,
		NULL, NULL);
		if (ret == 0) {
			//DP or CTF on the other side
			//mbedtls_printf("VerifyCertificate(): certificate verified successfully\n\n");
			if (dealer_run_mode == IN)
				mbedtls_printf("Connection from trusted DP\n\n");
			else if (dealer_run_mode == OUT)
				mbedtls_printf("Connection from trusted CTF\n\n");
			return true;
		} else {

			// ###################### <TODO>: check with Somnath ###############
			// This section was verifying the self-signed certificate from KMS.
			// We have that verification code with KMS client. Here we will verify
			// only DP/CTF CA signed certificates.
			// Removing SGX quote verification code from Dealer's server side verification
			// #################################################################
			mbedtls_printf("VerifyCertificate(): return code: %X \n\n", ret);
			return false;
		}
	} else // no peer certificate
	{
		mbedtls_printf(
				"VerifyCertificate(): failed to get peer certificate \n\n");
		return false;
	}
}
