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

#ifndef SGX_UCDR_H_
#define SGX_UCDR_H_

#ifdef __cplusplus
extern "C" {
#endif

	#define MAX_BUFFER_SIZE 255
	#define MAX_IDENTITY_LEN 256

	void *context;
	//void *context_router;

	void *u_sgxcdr_connect_cdr_mq(const char *host, uint16_t port,
		uint8_t pattern);

	int32_t u_sgxcdr_get_cdr_message(void *handle, uint8_t *buffer,
		uint32_t buffer_size);

	int32_t u_sgxcdr_send_cdr_message(void *handle, uint8_t *buffer,
		uint32_t buffer_size);

	int32_t u_sgxcdr_close_cdr_mq(void *handle);

	void *u_sgxcdr_connect_to_zmq_router(const char *host, uint16_t port,
		uint8_t mode, const char *identity,  int8_t *err, void *context);

	int32_t u_sgxcdr_send_zmq_router_socket(void *handle, uint8_t *buffer,
		uint32_t buffer_size);

	void u_sgxcdr_zmq_router_read_poll(void *handle, uint32_t msec,
		int8_t *result, uint8_t *buffer, uint32_t buffer_size);

	void u_sgxcdr_recv_zmq_router_socket(void *handle, uint8_t *buffer,
		uint32_t buffer_size, int32_t *actual_read);

	int32_t u_sgxcdr_close_zmq_router(void *handle, void *context);

	//TODO: 1. Performance optimization: Change buffer_size, actual_read type
	//	2. Add custom error codes

#ifdef __cplusplus
}
#endif

#endif /* SGX_UCDR_H_ */
