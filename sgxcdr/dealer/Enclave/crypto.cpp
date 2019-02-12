#include "crypto.h"

#include <mbedtls/net.h>
#include <mbedtls/debug.h>
#include "stdlib.h"
#include <string.h>
#include "ssl_conn_hdlr.h"

//<TODO> this must be replaced after KMS integration
uint8_t default_aad[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7 };
uint8_t default_ctr[16] = {0xb2, 0x4b, 0xf2, 0xf7, 0x7a, 0xc5, 0xec, 0x0c, 0x5e, 0x1f, 0x4d, 0xc1, 0xae, 0x46, 0x5e, 0x75};
static Crypto AESGCM(CRYPTO_TYPE_AES_GCM);
static Crypto AESCTR(CRYPTO_TYPE_AES_CTR);

void emit_debug(char* msg) {
	mbedtls_printf("%s\n", msg);
}

void emit_sizet(size_t s) {
	mbedtls_printf("%d\n", s);
}

Crypto::Crypto(int t = CRYPTO_TYPE_AES_GCM) {
	type = t;
	aad = NULL;
	aad_size = 0;
	ctr_num_bit_size = 32;
}

Crypto::~Crypto() {
	if (aad != NULL) {
		delete[] aad;
		aad = NULL;
	}
}

int Crypto::encrypt(char *in, size_t len, char *out, size_t *lenOut) {
	if (type == CRYPTO_TYPE_AES_GCM) {
		return encrypt_aesgcm(in, len, out, lenOut);
	} else if (type == CRYPTO_TYPE_AES_CTR) {
		return encrypt_aesctr(in, len, out);
	} else {
		return CRYPTO_TYPE_UNKNOWN;
	}
}

int Crypto::decrypt(char *in, size_t len, char *out, size_t *lenOut) {
	if (type == CRYPTO_TYPE_AES_GCM) {
		return decrypt_aesgcm(in, len, out, lenOut);
	} else if (type == CRYPTO_TYPE_AES_CTR) {
		return decrypt_aesctr(in, len, out);
	} else {
		return CRYPTO_TYPE_UNKNOWN;
	}
}

// ************************* AES-GCM APIs *********************************************************
int Crypto::encrypt_aesgcm(char *decMessageIn, size_t len,
	char *encMessageOut, size_t *lenOut)
{
	uint8_t *origMessage = (uint8_t *) decMessageIn;
	uint8_t p_dst[BUFLEN] = { 0 };
	*lenOut = len + SGX_AESGCM_MAC_SIZE + SGX_AESGCM_IV_SIZE;
	sgx_status_t status;

	// Generate the IV (nonce)
	sgx_read_rand(p_dst + SGX_AESGCM_MAC_SIZE, SGX_AESGCM_IV_SIZE);

	status = sgx_rijndael128GCM_encrypt(&aesgcm_key, origMessage, len,
			p_dst + SGX_AESGCM_MAC_SIZE + SGX_AESGCM_IV_SIZE,
			p_dst + SGX_AESGCM_MAC_SIZE, SGX_AESGCM_IV_SIZE, aad, aad_size,
			(sgx_aes_gcm_128bit_tag_t *) (p_dst));
	
	if (status != SGX_SUCCESS) {
		if( status == SGX_ERROR_MAC_MISMATCH) {
			return -2;
		}
		return -1;
	}

	memcpy(encMessageOut, p_dst, *lenOut);

	return 0;
}

int Crypto::decrypt_aesgcm(char *encMessageIn, size_t len,
	char *decMessageOut, size_t *lenOut)
{
	uint8_t *encMessage = (uint8_t *) encMessageIn;
	uint8_t p_dst[BUFLEN] = { 0 };
	sgx_status_t status;
	*lenOut = len - SGX_AESGCM_MAC_SIZE - SGX_AESGCM_IV_SIZE;

	status = sgx_rijndael128GCM_decrypt(&aesgcm_key,
			encMessage + SGX_AESGCM_MAC_SIZE + SGX_AESGCM_IV_SIZE, *lenOut,
			p_dst, encMessage + SGX_AESGCM_MAC_SIZE, SGX_AESGCM_IV_SIZE, aad,
			aad_size, (sgx_aes_gcm_128bit_tag_t *) encMessage);

	if (status != SGX_SUCCESS) {
		if( status == SGX_ERROR_MAC_MISMATCH) {
			return -2;
		}
		return -1;
	}
	memcpy(decMessageOut, p_dst, *lenOut);

	return 0;
}

int Crypto::set_params_aesgcm(sgx_aes_gcm_128bit_key_t k, uint8_t* a = NULL,
		size_t s = 0) {
	memcpy(aesgcm_key, k, SGX_AESGCM_KEY_SIZE);

	if (a != NULL) {
		aad_size = s;
		if (aad == NULL) {
			aad = new uint8_t[aad_size];
		} else {
			delete[] aad;
			aad = new uint8_t[aad_size];
		}
		memcpy(aad, a, aad_size);
	} else {
		aad = NULL;
		aad_size = 0;
	}
  return 0;
}

// ************************* AES-GCM APIs ends here ***********************************************

// ************************* AES-CTR APIs  ********************************************************
int Crypto::encrypt_aesctr(char *plain_str, size_t src_len, char *enc_str) {
   const uint32_t ctr_num_bit_size = 32;
   uint8_t ctr[16] = {0xb2, 0x4b, 0xf2, 0xf7, 0x7a, 0xc5, 0xec, 0x0c, 0x5e, 0x1f, 0x4d, 0xc1, 0xae, 0x46, 0x5e, 0x75};
   sgx_aes_ctr_128bit_key_t key;

   memset(key, 0, sizeof(key));
   key[2] = 'z';

   if (sgx_aes_ctr_encrypt((const sgx_aes_ctr_128bit_key_t*) key,
      (uint8_t *) plain_str, src_len, ctr, ctr_num_bit_size,
      (uint8_t *)enc_str) != SGX_SUCCESS) {
      return -1;
   }
   return 0;
}

int Crypto::decrypt_aesctr(char *enc_str, size_t src_len, char *plain_str) {
   const uint32_t ctr_num_bit_size = 32;
   uint8_t ctr[16] = {0xb2, 0x4b, 0xf2, 0xf7, 0x7a, 0xc5, 0xec, 0x0c, 0x5e, 0x1f, 0x4d, 0xc1, 0xae, 0x46, 0x5e, 0x75};
   sgx_aes_ctr_128bit_key_t key;

   memset(key, 0, sizeof(key));
   key[2] = 'z';

   if (sgx_aes_ctr_decrypt((const sgx_aes_ctr_128bit_key_t*) key,
      (uint8_t *) enc_str, src_len, ctr, ctr_num_bit_size,
      (uint8_t *)plain_str) != SGX_SUCCESS) {
      return -1;
   }
   return 0;
}

int Crypto::set_params_aesctr(sgx_aes_ctr_128bit_key_t k, uint8_t *c) {
	//set key
	memcpy(aesctr_key, k, 16);

	// set counter
	if (c == NULL) {
		memcpy(ctr, default_ctr, 16);
	} else {
		memcpy(ctr, c, 16);
	}

	ctr_num_bit_size = 32;
 return 0;
}
// ************************* AES-CTR APIs ends here ***********************************************

#if 0
// ******************************* Testing ********************************************************
class Test {
public:
	size_t max;
	char *str;
	size_t str_len;
	size_t test_size;

	Test() {
		max = 1024;
		str = NULL;
		test_size = 25;
		str_len = 0;
	}

	~Test() {
		if (str != NULL) {
			free(str);
			str = NULL;
		}
	}

	size_t random_length();
	void random_string();
	void alloc(size_t);
	void test_aesgcm(Crypto &);
	void test_aesctr(Crypto &);
};

void Test::alloc(size_t len) {
	// free previous allocation, if any
	if (str != NULL) {
		free(str);
	}
	// allocate new buffer of size "len"
	str = (char*) malloc(len + 1);
	memset(str, 0, len + 1);
	str_len = len;

}

size_t Test::random_length() {
	// generate random message length not more than "max"
	// if random length is zero, then make it 25
	size_t length;
	sgx_read_rand((unsigned char*) &length, 8);
	length = length % max;
	if (length <= 0)
		length = 25;

	return length;
}

void Test::random_string() {
	size_t len = random_length();
	alloc(len);
	// create random string of size "length"
	sgx_read_rand((unsigned char*) str, len);
	str[len] = '\0';
}

void Test::test_aesgcm(Crypto &AESGCM) {
	for (int i = 0; i < test_size; i++) {
		random_string();
		int ret;

		size_t encMessageLen = (SGX_AESGCM_MAC_SIZE + SGX_AESGCM_IV_SIZE
				+ str_len);
		char *encMessage = (char *) malloc((encMessageLen + 1) * sizeof(char));

		ret = AESGCM.encrypt(str, str_len, encMessage, &encMessageLen);
		encMessage[encMessageLen] = '\0';
		if (ret != 0) {
			emit_debug("Test failed!");
		}

		// The decrypted message will contain the same message as the original one.
		size_t decMessageLen = str_len;
		char *decMessage = (char *) malloc((decMessageLen + 1) * sizeof(char));

		ret = AESGCM.decrypt(encMessage, encMessageLen, decMessage, &decMessageLen);
		decMessage[decMessageLen] = '\0';
		if (ret != 0) {
			emit_debug("Test failed!");
		}

		if (strncmp(str, decMessage, str_len) == 0) {
			emit_debug("Test successful!");
		} else {
			emit_debug("Test failed!");
		}

		//clean up
		free(encMessage);
		encMessage = NULL;
		free(decMessage);
		decMessage = NULL;
		free(str);
		str = NULL;
	}
}

void Test::test_aesctr(Crypto &AESCTR) {
	for (int i = 0; i < test_size; i++) {
		random_string();
		int ret;
		size_t lenOut;

		char *encMessage = (char *) malloc((str_len + 1) * sizeof(char));
		ret = AESCTR.encrypt(str, str_len, encMessage, &lenOut);
		encMessage[str_len] = '\0';
		if (ret != 0) {
			emit_debug("Test failed!");
		}

		// The decrypted message will contain the same message as the original one.
		size_t decMessageLen = str_len;
		char *decMessage = (char *) malloc((decMessageLen + 1) * sizeof(char));

		ret = AESCTR.decrypt(encMessage, str_len, decMessage, &decMessageLen);
		decMessage[decMessageLen] = '\0';
		if (ret != 0) {
			emit_debug("Test failed!");
		}

		if (strncmp(str, decMessage, str_len) == 0) {
			emit_debug("Test successful!");
		} else {
			emit_debug("Test failed!");
		}

		//clean up
		free(encMessage);
		encMessage = NULL;
		free(decMessage);
		decMessage = NULL;
		free(str);
		str = NULL;
	}
}

void test_crypto(int size) {
	Test t1;
	t1.test_size = size;

	emit_debug("testing GCM mode");
	// test aes-gcm mode
	Crypto aesgcm(CRYPTO_TYPE_AES_GCM);
	aesgcm.set_params_aesgcm(default_key_gcm, default_aad, sizeof(default_aad));
	t1.test_aesgcm(aesgcm);

	Test t2;
	t2.test_size = size;

	emit_debug("testing CTR mode");
	// test aes-ctr mode
	Crypto aesctr(CRYPTO_TYPE_AES_CTR);
	aesgcm.set_params_aesctr(default_key_ctr, default_ctr);
	t2.test_aesctr(aesctr);
}
// ******************************* Testing ends here **********************************************
#endif
