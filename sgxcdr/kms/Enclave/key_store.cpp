#include "key_store.h"
#include <cstdio>

#define FS_KEY_NM "kms_fs_key.key"

KeyStore::KeyStore() {
	store_path = NULL;
}

void KeyStore::set_store_path(char *path) {
	size_t len = strlen(path);
	store_path = (char*) malloc(len + 1);
	memset(store_path, 0, len + 1);
	strncpy(store_path, path, len);
	store_path[len] = '\0';
}

KeyStore::~KeyStore() {
	if (store_path != NULL) {
		free(store_path);
	}
}

bool KeyStore::seal_key(char *plaintext, sgx_sealed_data_t* sealed_data,
		size_t sealed_size) {
	size_t plaintext_len = strlen(plaintext);
	sgx_status_t status = sgx_seal_data(0, NULL, plaintext_len,
			(uint8_t*) plaintext, sealed_size, sealed_data);
	if (status == SGX_SUCCESS) {
		return true;
	} else {
		return false;
	}
}

bool KeyStore::unseal_key(sgx_sealed_data_t *sealed_data, char *plaintext) {
	uint32_t plaintext_len;
	sgx_status_t status = sgx_unseal_data(sealed_data, NULL, NULL,
			(uint8_t*) plaintext, &plaintext_len);
	if (status == SGX_SUCCESS) {
		return true;
	} else {
		return false;
	}
}

int KeyStore::create_random_key(char *nm, size_t sz = DEFAULT_KEY_SIZE) {
	// create new key
	char *key_data = (char*) malloc(sz + 1);
	memset(key_data, 0, sz + 1);
	sgx_read_rand((unsigned char*) key_data, sz);

	if (!write_to_file(nm, key_data, sz)) {
		return -1;
	}

	return 0;
}

bool KeyStore::get_fs_key_filename(char *fname) {
	strncpy(fname, FS_KEY_NM, 14);
	return true;
}

bool KeyStore::create_fs_key() {
	char *key_data = (char*) malloc(17);
	memset(key_data, 0, 17);
	sgx_read_rand((unsigned char*) key_data, 16);
	key_data[16] = '\0';
	memcpy(fs_key, key_data, 16);

	return true;
}

bool KeyStore::load_fs_key() {
	size_t key_size = DEFAULT_KEY_SIZE;
	char * plain_key = (char*) malloc(key_size + 1);

	memset(plain_key, 0, key_size + 1);

	//read key from the file
	char *fname = (char*) malloc(MAX_PATH_LEN);
	get_fs_key_filename(fname);

	SGX_FILE *handle = open_secure_file(fname, "r", true);
	if (handle != NULL) {
		if (sgx_fread(fs_key, sizeof(char), DEFAULT_KEY_SIZE, handle) !=
				DEFAULT_KEY_SIZE) {
			sgx_fclose(handle);
			free(fname);
			fname=NULL;
			free(plain_key);
			plain_key=NULL;
			return false;
		}

		sgx_fclose(handle);
		free(fname);
		fname = NULL;
		free(plain_key);
		plain_key=NULL;
		return true;
	} else {
		// create new key, if not present on the disk
		if (!create_fs_key()) {
			free(fname);
			fname = NULL;
			free(plain_key);
			plain_key=NULL;
			return false;
		}

		handle = open_secure_file(fname, "w", true);
		if (handle == NULL) {
			free(fname);
			fname = NULL;
			free(plain_key);
			plain_key=NULL;
			return false;
		}

		if (sgx_fwrite(fs_key, sizeof(char), DEFAULT_KEY_SIZE, handle) !=
				DEFAULT_KEY_SIZE) {
			sgx_fclose(handle);
			free(fname);
			fname = NULL;
			free(plain_key);
			plain_key=NULL;
			return false;
		}

		sgx_fclose(handle);
		free(fname);
		fname = NULL;
		free(plain_key);
		plain_key=NULL;
		return true;
	}
}

SGX_FILE * KeyStore::open_secure_file(char *filename, const char *mode,
		bool auto_key = true) {
	SGX_FILE *handle = NULL;

	char *path = (char*) malloc(MAX_PATH_LEN);
	memset(path, 0, MAX_PATH_LEN);
	namespace_to_path(filename, path);

	// use auto key to open file
	if (auto_key) {
		handle = sgx_fopen_auto_key(path, mode);
	} else {
		handle = sgx_fopen(path, mode, &fs_key);
		//handle = sgx_fopen(path, mode, &default_key_ctr);
	}
	if (handle == NULL) {
		free(path);
		return NULL;
	}

	free(path);
	return handle;
}

bool KeyStore::write_to_file(char *fname, char *data, size_t sz =
DEFAULT_KEY_SIZE) {

	SGX_FILE *handle = open_secure_file(fname, "w", false);
	if (handle == NULL) {
		return false;
	}

	size_t count = sgx_fwrite(data, sizeof(char), sz, handle);
	sgx_fclose(handle);
	if (count != sz) {
		return false;
	} else {
		return true;
	}

	return true;
}

bool KeyStore::read_from_file(char *fname, char *data, size_t sz =
DEFAULT_KEY_SIZE) {

	SGX_FILE *handle = open_secure_file(fname, "r", false);
	if (handle == NULL) {
		return false;
	}

	sgx_fread(data, sizeof(char), sz, handle);
	sgx_fclose(handle);

	return true;
}

int KeyStore::namespace_to_path(char *nm, char *path) {
	strncpy(path, store_path, MAX_PATH_LEN);
	strncat(path, "/", MAX_PATH_LEN);
	strncat(path, nm, MAX_PATH_LEN);

	return 0;
}

int KeyStore::get_key(char *nm, char *key) {

   // check namespace is not a master key namespace
   if(strcmp(nm, FS_KEY_NM) == 0) {
      return -3;
   }

   sgx_cmac_128bit_key_t p_key;
   memcpy(&p_key, fs_key, DEFAULT_KEY_SIZE);

   // generate namespace key using fskey and namespace
   if (sgx_rijndael128_cmac_msg(&p_key, (uint8_t *) nm, strlen(nm),
         (sgx_cmac_128bit_tag_t *) key) != SGX_SUCCESS) {
      return -1;
   }

	return 0;
}

int KeyStore::delete_key(char *nm) {
	char *path = (char*) malloc(MAX_PATH_LEN);
	memset(path, 0, MAX_PATH_LEN);
	namespace_to_path(nm, path);

	sgx_remove(path);
	return 0;
}

#if 0
// ============================ test related code =======================
int test_key_store() {
	char *nm = "test_nm1";
	KeyStore ks;
	ks.set_store_path("/tmp");
	ks.load_fs_key();

	//emit_debug("KeyStore object created!");

	// test get key
	char *key = (char*) malloc(DEFAULT_KEY_SIZE + 1);
	memset(key, 0, DEFAULT_KEY_SIZE + 1);
	int r = ks.get_key(nm, key);
	if (r != 0) {
		//emit_debug("failed at line#193");
		return r;
	}

	char *key1 = (char*) malloc(DEFAULT_KEY_SIZE + 1);
	memset(key1, 0, DEFAULT_KEY_SIZE + 1);
	r = ks.get_key(nm, key);
	if (r != 0) {
		//emit_debug("failed at line#201");
		return r;
	}

	if (!strcmp(key, key1)) {
		//emit_debug("failed at line#206");
		return -1;
	}

	/*r = ks.delete_key(nm);
	 if (r != 0) {
	 return r;
	 }*/

	return 0;
}
#endif
