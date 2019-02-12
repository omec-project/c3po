/*
 * Copyright (c) 2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SGX_TCDR_H_
#define SGX_TCDR_H_


#ifdef __cplusplus
extern "C" {
#endif

	#include "sgx_tprotected_fs.h"

	typedef void zmq_handle;
	typedef void zmq_context;

	typedef enum _zmq_pattern_t {
		SGX_ZMQ_PUSH,
		SGX_ZMQ_PULL,
	} zmq_pattern_t;


	zmq_handle *sgx_connect_cdr_mq(const char *host, uint16_t port,
		zmq_pattern_t pattern);

	int32_t sgx_get_cdr_message(zmq_handle *handle, uint8_t *buffer,
		uint32_t buffer_size);

	int32_t sgx_send_cdr_message(zmq_handle *handle, uint8_t *buffer,
		uint32_t buffer_size);

	int32_t sgx_close_cdr_mq(zmq_handle *handle);

	SGX_FILE *sgx_open_cdr_file(const char *filename, const char *mode,
		sgx_key_128bit_t *key);

	int32_t sgx_close_cdr_file(SGX_FILE *file);

	int sgx_read_cdr_file(void *ptr_buff, size_t size, size_t count,
		SGX_FILE *file);

	int sgx_write_cdr_file(void *ptr_buff, size_t size, size_t count,
		SGX_FILE *file);

   int sgx_flush_cdr_file(SGX_FILE *file);
	//TODO: Add custom error codes

#ifdef __cplusplus
}
#endif


#endif /* SGX_TCDR_H_ */
