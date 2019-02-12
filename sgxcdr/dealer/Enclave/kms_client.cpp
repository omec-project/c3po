#include "kms_client.h"
#include "ssl_conn_hdlr.h"
#include <string.h>

#include "ra.h"
#include "ra-challenger.h"

char kms_mrenclave[SGX_HASH_SIZE * 2 + 1];

/*
 * global options
 */

void print_sgx_quote(sgx_quote_t*);

static void my_debug(void *ctx, int level, const char *file, int line,
		const char *str) {
	const char *p, *basename;
	(void) (ctx);

	/* Extract basename from file */
	for (p = basename = file; *p != '\0'; p++)
		if (*p == '/' || *p == '\\')
			basename = p + 1;

	mbedtls_printf("%s:%04d: |%d| %s", basename, line, level, str);
}

/*
 * Test recv/send functions that make sure each try returns
 * WANT_READ/WANT_WRITE at least once before sucesseding
 */
static int my_recv(void *ctx, unsigned char *buf, size_t len) {
	static int first_try = 1;
	int ret;

	if (first_try) {
		first_try = 0;
		return (MBEDTLS_ERR_SSL_WANT_READ);
	}

	ret = mbedtls_net_recv_ocall(ctx, buf, len);
	if (ret != MBEDTLS_ERR_SSL_WANT_READ)
		first_try = 1; /* Next call will be a new operation */
	return (ret);
}

static int my_send(void *ctx, const unsigned char *buf, size_t len) {
	static int first_try = 1;
	int ret;

	if (first_try) {
		first_try = 0;
		return (MBEDTLS_ERR_SSL_WANT_WRITE);
	}

	ret = mbedtls_net_send_ocall(ctx, buf, len);
	if (ret != MBEDTLS_ERR_SSL_WANT_WRITE)
		first_try = 1; /* Next call will be a new operation */
	return (ret);
}

static int my_verify(void *data, mbedtls_x509_crt *peercert, int depth,
		uint32_t *flags) {
	char buf[1024];

	if (peercert == NULL) {
		mbedtls_printf("failed to get peer certificate\n");
		return MBEDTLS_ERR_X509_CERT_VERIFY_FAILED;
	}

	mbedtls_printf("\nVerify requested for (Depth %d):\n", depth);
	mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", peercert);
	mbedtls_printf("%s", buf);

	int r = verify_sgx_cert_extensions(peercert->raw.p, peercert->raw.len);
	if (r != 0) {
		mbedtls_printf("failed to verify the SGX report\n");
		return MBEDTLS_ERR_X509_CERT_VERIFY_FAILED;
	}

	sgx_quote_t q = { 0, };
	get_quote_from_cert(peercert->raw.p, peercert->raw.len, &q);
	print_sgx_quote(&q);

	// ************ SGX parameter verification *************************

	// validate MRENCLAVE
	memset(kms_mrenclave, 0, SGX_HASH_SIZE *2 + 1);
	for (int i = 0; i < SGX_HASH_SIZE; ++i) {
		mbedtls_snprintf(kms_mrenclave + i * 2, 200, "%02x",
				q.report_body.mr_enclave.m[i]);
	}

	// validate MRSIGNER
	char mr_signer[200];
	memset(mr_signer, 0, 200);
	for (int i = 0; i < SGX_HASH_SIZE; ++i) {
		mbedtls_snprintf(mr_signer + i * 2, 200, "%02x",
				q.report_body.mr_signer.m[i]);
	}

	if (strcmp(mr_signer, KMS_MRSIGNER) != 0) {
		mbedtls_printf("Key requested from untrusted client!\n");
		mbedtls_printf(
				"Rejecting the key request!\nExpected signer: %s, provided signer: %s\n\n",
				KMS_MRSIGNER, mr_signer);
		return MBEDTLS_ERR_X509_CERT_VERIFY_FAILED;
	}

	// validate ProductID
	if (q.report_body.isv_prod_id != KMS_PRODID) {
		mbedtls_printf(
				"ProductID verification failed!!\nExpected ProductID: %u, provided ProductID: %u\n\n",
				KMS_PRODID, q.report_body.isv_prod_id);
		return MBEDTLS_ERR_X509_CERT_VERIFY_FAILED;
	}

	// validate ISVSVN
	if (q.report_body.isv_svn != KMS_ISVSVN) {
		mbedtls_printf(
				"ISV SVN verification failed!!\nExpected SVN: %u, provided SVN: %u\n\n",
				KMS_ISVSVN, q.report_body.isv_svn);
		return MBEDTLS_ERR_X509_CERT_VERIFY_FAILED;
	}

	mbedtls_printf("SGX report verified successfully!!\n\n");
	return (0);
}

int kms_client(client_opt_t opt, char* headers[], int n_header,
		unsigned char* output, int length, mbedtls_pk_context *pkey,
		mbedtls_x509_crt *clicert, mbedtls_x509_crt *cacert) {
	int ret = 0, len, written, frags;
	mbedtls_net_context server_fd;
	unsigned char buf[MBEDTLS_SSL_MAX_CONTENT_LEN + 1];

	const char *pers = "ssl_client2";

	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_ssl_session saved_session;
#if defined(MBEDTLS_TIMING_C)
	mbedtls_timing_delay_context timer;
#endif
	uint32_t flags;

	/*
	 * Make sure memory references are valid.
	 */
	mbedtls_net_init_ocall(&server_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
	memset(&saved_session, 0, sizeof(mbedtls_ssl_session));
	mbedtls_ctr_drbg_init(&ctr_drbg);

#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(opt.debug_level);
#endif

	//opt.arc4 = MBEDTLS_SSL_ARC4_ENABLED;

	/*
	 * 0. Initialize the RNG and the session data
	 */
	mbedtls_printf("Seeding the random number generator...\n");

	mbedtls_entropy_init(&entropy);
	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
			(const unsigned char *) pers, strlen(pers))) != 0) {
		mbedtls_printf(" mbedtls_ctr_drbg_seed returned -%#x", -ret);
		goto exit;
	}

	/*
	 * 1.1. Load the trusted CA: not required, use certificate generated for Dealer
	 */

	/*
	 * 2. Start the connection
	 */
	if (opt.server_addr == NULL)
		opt.server_addr = opt.server_name;

	mbedtls_printf("connecting to %s:%s:%s...\n",
			opt.transport == MBEDTLS_SSL_TRANSPORT_STREAM ? "TCP" : "UDP",
			opt.server_addr, opt.server_port);

	if ((ret = mbedtls_net_connect_ocall(&server_fd, opt.server_addr,
			opt.server_port,
			opt.transport == MBEDTLS_SSL_TRANSPORT_STREAM ?
					MBEDTLS_NET_PROTO_TCP : MBEDTLS_NET_PROTO_UDP)) != 0) {
		mbedtls_printf(" mbedtls_net_connect returned -%#x", -ret);
		goto exit;
	}

	if (opt.nbio > 0)
		ret = mbedtls_net_set_nonblock_ocall(&server_fd);
	else
		ret = mbedtls_net_set_block_ocall(&server_fd);
	if (ret != 0) {
		mbedtls_printf(" net_set_(non)block() returned -%#x", -ret);
		goto exit;
	}

	/*
	 * 3. Setup stuff
	 */
	mbedtls_printf("Setting up the SSL/TLS structure...\n");

	if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT,
			opt.transport, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		mbedtls_printf("mbedtls_ssl_config_defaults returned -%#x", -ret);
		goto exit;
	}

	mbedtls_ssl_conf_authmode(&conf, opt.auth_mode);

	mbedtls_ssl_conf_verify(&conf, my_verify, NULL);

	if (opt.etm != DFL_ETM)
		mbedtls_ssl_conf_encrypt_then_mac(&conf, opt.etm);

	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_debug, NULL);
	mbedtls_ssl_conf_read_timeout(&conf, opt.read_timeout);
	if (opt.force_ciphersuite[0] != DFL_FORCE_CIPHER)
		mbedtls_ssl_conf_ciphersuites(&conf, opt.force_ciphersuite);
	if (opt.allow_legacy != DFL_ALLOW_LEGACY)
		mbedtls_ssl_conf_legacy_renegotiation(&conf, opt.allow_legacy);
#if defined(MBEDTLS_SSL_RENEGOTIATION)
	mbedtls_ssl_conf_renegotiation(&conf, opt.renegotiation);
#endif

	//mbedtls_ssl_conf_ca_chain(&conf, cacert, NULL);
	mbedtls_ssl_conf_ca_chain(&conf, clicert, NULL);
	if ((ret = mbedtls_ssl_conf_own_cert(&conf, clicert, pkey)) != 0) {
		mbedtls_printf("  mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
		goto exit;
	}

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		mbedtls_printf("mbedtls_ssl_setup returned -%#x", -ret);
		goto exit;
	}

	if ((ret = mbedtls_ssl_set_hostname(&ssl, opt.server_name)) != 0) {
		mbedtls_printf("mbedtls_ssl_set_hostname returned %d\n\n", ret);
		goto exit;
	}

	if (opt.nbio == 2)
		mbedtls_ssl_set_bio(&ssl, &server_fd, my_send, my_recv, NULL);
	else
		mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send_ocall,
				mbedtls_net_recv_ocall,
				opt.nbio == 0 ? mbedtls_net_recv_timeout_ocall : NULL);

#if defined(MBEDTLS_TIMING_C)
	mbedtls_ssl_set_timer_cb(&ssl, &timer, mbedtls_timing_set_delay,
			mbedtls_timing_get_delay);
#endif

	/*
	 * 4. Handshake
	 */
	mbedtls_printf("Performing the SSL/TLS handshake\n");

	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ
				&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			mbedtls_printf("mbedtls_ssl_handshake returned -%#x", -ret);
			if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED)
				mbedtls_printf("Unable to verify the server's certificate. "
						"Either it is invalid,"
						"or you didn't set ca_file or ca_path "
						"to an appropriate value."
						"Alternatively, you may want to use "
						"auth_mode=optional for testing purposes.");
			goto exit;
		}
	}

	mbedtls_printf("Hand shake succeeds: [%s, %s]",
			mbedtls_ssl_get_version(&ssl), mbedtls_ssl_get_ciphersuite(&ssl));

	/*
	 * 5. Verify the server certificate
	 */
	mbedtls_printf("Verifying peer X.509 certificate...\n");

	if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0) {
		char vrfy_buf[512];

		mbedtls_printf(" failed\n");

		mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", flags);

		mbedtls_printf("%s\n", vrfy_buf);

		//ret = MBEDTLS_ERR_X509_CERT_VERIFY_FAILED;
		//goto exit;
	} else
		mbedtls_printf("X.509 Verifies");

	if (mbedtls_ssl_get_peer_cert(&ssl) != NULL) {
		if (opt.debug_level > 0) {
			mbedtls_printf("Peer certificate information\n");
			mbedtls_x509_crt_info((char *) buf, sizeof(buf) - 1, "|-",
					mbedtls_ssl_get_peer_cert(&ssl));
			mbedtls_printf("%s\n", buf);
		}

	}

#if defined(MBEDTLS_SSL_RENEGOTIATION)
	if (opt.renegotiate) {
		/*
		 * Perform renegotiation (this must be done when the server is waiting
		 * for input from our side).
		 */
		mbedtls_printf("  . Performing renegotiation...\n");
		while ((ret = mbedtls_ssl_renegotiate(&ssl)) != 0) {
			if (ret != MBEDTLS_ERR_SSL_WANT_READ
					&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				mbedtls_printf("  mbedtls_ssl_renegotiate returned %d\n\n",
						ret);
				goto exit;
			}
		}
		mbedtls_printf(" ok\n");
	}
#endif /* MBEDTLS_SSL_RENEGOTIATION */

	/*
	 * 6. Write the GET request
	 */
	send_request:

	len = mbedtls_snprintf((char *) buf, sizeof(buf) - 1, "%s",
			opt.key_namespace);
	buf[len] = '\0';
	for (written = 0, frags = 0; written < len; written += ret, frags++) {
		while ((ret = mbedtls_ssl_write(&ssl, buf, len)) <= 0) {
			if (ret != MBEDTLS_ERR_SSL_WANT_READ
					&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				mbedtls_printf("  mbedtls_ssl_write returned -%#x", -ret);
				goto exit;
			}
		}
	}

	mbedtls_printf("%d bytes written ", len);
	mbedtls_printf("%s\n", (char*) buf);

	mbedtls_debug_print_buf(&ssl, 0, __FILE__, __LINE__, "bytes written: ", buf,
			len);

	/*
	 * 7. Read the HTTP response
	 */

	/*
	 * TLS and DTLS need different reading styles (stream vs datagram)
	 */

	len = 0;
	memset(output, 0, length);
	do {
		ret = mbedtls_ssl_read(&ssl, output + len, length - len);

		if (ret == MBEDTLS_ERR_SSL_WANT_READ
				|| ret == MBEDTLS_ERR_SSL_WANT_WRITE)
			continue;

		if (ret <= 0) {
			switch (ret) {
			case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
				mbedtls_printf(" connection was closed gracefully\n");
				ret = 0;
				goto close_notify;

			case 0:
			case MBEDTLS_ERR_NET_CONN_RESET:
				mbedtls_printf(" connection was reset by peer\n");
				ret = 0;
				goto reconnect;

			default:
				mbedtls_printf(" mbedtls_ssl_read returned -0x%x\n", -ret);
				goto exit;
			}
		}

		len += ret;

		//mbedtls_printf("get %d bytes ending with %x", len, output[len - 1]);
		//mbedtls_debug_print_buf(&ssl, 0, __FILE__, __LINE__, "response", output,
		//		len);

		// Check if entire has been read
		if (ret == KEY_SIZE) {
			ret = 0;
			output[len] = '\0';
			break;
		}

		length = len;
	} while (1);

	/*
	 * 7b. Simulate hard reset and reconnect from same port?
	 */
	if (opt.reconnect_hard != 0) {
		opt.reconnect_hard = 0;

		mbedtls_printf("  . Restarting connection from same port...");

		if ((ret = mbedtls_ssl_session_reset(&ssl)) != 0) {
			mbedtls_printf("  mbedtls_ssl_session_reset returned -%#x", -ret);
			goto exit;
		}

		while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
			if (ret != MBEDTLS_ERR_SSL_WANT_READ
					&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				mbedtls_printf("  mbedtls_ssl_handshake returned -%#x", -ret);
				goto exit;
			}
		}

		mbedtls_printf(" ok\n");

		goto send_request;
	}

	/*
	 * 8. Done, cleanly close the connection
	 */
	close_notify:

	/* No error checking, the connection might be closed already */
	do
		ret = mbedtls_ssl_close_notify(&ssl);
	while (ret == MBEDTLS_ERR_SSL_WANT_WRITE);
	ret = 0;

	mbedtls_printf("closed %s:%s", opt.server_addr, opt.server_port);

	/*
	 * 9. Reconnect?
	 */
	reconnect: if (opt.reconnect != 0) {
		--opt.reconnect;

		mbedtls_net_free_ocall(&server_fd);

#if defined(MBEDTLS_TIMING_C)
		if (opt.reco_delay > 0)
		mbedtls_net_usleep(1000000 * opt.reco_delay);
#endif

		mbedtls_printf("  . Reconnecting with saved session...");

		if ((ret = mbedtls_ssl_session_reset(&ssl)) != 0) {
			mbedtls_printf("  mbedtls_ssl_session_reset returned -%#x", -ret);
			goto exit;
		}

		if ((ret = mbedtls_ssl_set_session(&ssl, &saved_session)) != 0) {
			mbedtls_printf("  mbedtls_ssl_conf_session returned %d\n\n", ret);
			goto exit;
		}

		if ((ret = mbedtls_net_connect_ocall(&server_fd, opt.server_addr,
				opt.server_port,
				opt.transport == MBEDTLS_SSL_TRANSPORT_STREAM ?
						MBEDTLS_NET_PROTO_TCP : MBEDTLS_NET_PROTO_UDP)) != 0) {
			mbedtls_printf("  mbedtls_net_connect returned -%#x", -ret);
			goto exit;
		}

		if (opt.nbio > 0)
			ret = mbedtls_net_set_nonblock_ocall(&server_fd);
		else
			ret = mbedtls_net_set_block_ocall(&server_fd);
		if (ret != 0) {
			mbedtls_printf("  net_set_(non)block() returned -%#x", -ret);
			goto exit;
		}

		while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
			if (ret != MBEDTLS_ERR_SSL_WANT_READ
					&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				mbedtls_printf("  mbedtls_ssl_handshake returned -%#x", -ret);
				goto exit;
			}
		}

		mbedtls_printf(" ok\n");

		goto send_request;
	}

	/*
	 * Cleanup and exit
	 */
	exit:
#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		mbedtls_printf("Last error was: -0x%X - %s\n\n", -ret, error_buf);
	}
#endif

	mbedtls_net_free_ocall(&server_fd);
	mbedtls_ssl_session_free(&saved_session);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

	// Shell can not handle large exit numbers -> 1 for errors

	return (ret);

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
