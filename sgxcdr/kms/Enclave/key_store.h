/*
 * key_store.h
 *
 */

#ifndef KMS_ENCLAVE_KEY_STORE_H_
#define KMS_ENCLAVE_KEY_STORE_H_

#include <string.h>
#include <stdlib.h>
#include "sgx_trts.h"
//#include "sgx_tae_service.h"
#include "sgx_tprotected_fs.h"
#include "sgx_tcrypto.h"
#include "sgx_tseal.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sgx_tseal.h>
#include <sgx_utils.h>
#include <sgx_tcrypto.h>
#include <stdlib.h>
#include <sgx_tprotected_fs.h>

// namespace format: <namespace-id>
#define	MAX_PATH_LEN	1024
#define DEFAULT_KEY_SIZE	16 // 128 bits

class KeyStore {
public:
	KeyStore();
	~KeyStore();

	//APIs
	int get_key(char*, char*);
	void set_store_path(char*);
	bool load_fs_key();
	int delete_key(char*);

private:
	// key store path where sealed keys will be stored
	char *store_path;
	// master key used to protect all name space keys
	sgx_key_128bit_t fs_key;

	// Internal APIs
	bool seal_key(char*, sgx_sealed_data_t*, size_t);
	bool unseal_key(sgx_sealed_data_t *, char*);
	int create_random_key(char*, size_t);
	bool get_fs_key_filename(char *);
	bool create_fs_key();


	// File handling
	SGX_FILE *open_secure_file(char*, const char*, bool);
	bool write_to_file(char*, char*, size_t);
	bool read_from_file(char*, char*, size_t);
	int namespace_to_path(char*, char*);

};

// test
int test_key_store();

#endif /* KMS_ENCLAVE_KEY_STORE_H_ */
