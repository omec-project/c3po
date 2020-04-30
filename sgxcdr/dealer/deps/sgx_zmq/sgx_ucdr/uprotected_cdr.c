/*
 * Copyright 2019-present Open Networking Foundation
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#include "uprotected_cdr.h"

#define randof(num) (int) ((float) (num) * random () / (RAND_MAX + 1.0))

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, "[sgx_uprotected_cdr:%d] " fmt, __LINE__, ##args)
#else
#define DEBUG_PRINT(...)
#endif

/**
 * @Name : u_sgxcdr_connect_cdr_mq
 * @arguments : [In] Host
 *		[In] Port
 *		[In] Pattern cand be PUSH or PUSH
 * @return : Returns ZMQ handle incase of success, NULL incase of error
 * @Description : Function to connect to ZMQ in PUSH/PULL pattern
 */
void *u_sgxcdr_connect_cdr_mq(const char *host, uint16_t port, uint8_t pattern)
{
	void *sender = NULL;
	void *receiver = NULL;
	char address[MAX_BUFFER_SIZE];

	if (host == NULL)
		return NULL;

	context = zmq_ctx_new();
	memset(address, 0, MAX_BUFFER_SIZE);
	sprintf(address, "tcp://%s:%d", host, port);
	switch (pattern) {
	case 0:
		sender = zmq_socket(context, ZMQ_PUSH);
		zmq_connect(sender, address);
		return sender;

	case 1:
		receiver = zmq_socket(context, ZMQ_PULL);
		zmq_connect(receiver, address);
		return receiver;

	default:
		return NULL;
	}

	return NULL;
}

/**
 * @Name : u_sgxcdr_get_cdr_message
 * @arguments : [In] Handle
 *		[Out] Buffer
 *		[In] buffer_size
 * @return : Returns actual read number of bytes
 * @Description : Function to get message from the ZMQ
 */
int32_t u_sgxcdr_get_cdr_message(void *handle, uint8_t *buffer,
		uint32_t buffer_size)
{
	int32_t actual_read = -1;

	if (handle == NULL || buffer == NULL)
		return -1;

	int size = zmq_recv(handle, buffer, buffer_size, 0);

	if (size != -1) {
		//size can exceed the value of buffer_size, if msg was truncated
		actual_read = (size > buffer_size)?buffer_size:size;
	}

	return actual_read;
}

/**
 * @Name : u_sgxcdr_send_cdr_message
 * @arguments : [In] Handle
 *		[In] Buffer
 *		[In] buffer_size
 * @return : Returns number of bytes sent
 * @Description : Function to send a message to the ZMQ
 */
int32_t u_sgxcdr_send_cdr_message(void *handle, uint8_t *buffer,
		uint32_t buffer_size)
{

	if (handle == NULL || buffer == NULL || buffer_size == 0)
		return -1;
	
	int size = zmq_send(handle, buffer, buffer_size, 0);

	return size;
}

/**
 * @Name : u_sgxcdr_close_cdr_mq
 * @arguments : [In] Handle
 * @return : Returns 0 on success
 * @Description : Function closes the connection to the ZMQ
 */
int32_t u_sgxcdr_close_cdr_mq(void *handle)
{
	int32_t flag_close = -1, flag_ctx_destroy = -1;

	if (handle == NULL)
		return -1;
	
	flag_close = !(zmq_close(handle)) ? 0 : -1;
	flag_ctx_destroy = !(zmq_ctx_destroy(context)) ? 0 : -1;

	if (!flag_close && !flag_ctx_destroy)
		return 0;

	return -1;
}

/**
 * @Name : u_sgxcdr_connect_to_zmq_router
 * @arguments : [In] host CDR_ROUTER_IP
 *		[In] port CDR_ROUTER_PORT
 *		[In] mode
 *		[In] identity of dealer
 *		[Out] err
 * @return : Returns ZMQ handle incase of success, NULL incase of error
 * @Description : Function to connect to ZMQ Router
 */
void *u_sgxcdr_connect_to_zmq_router(const char *host, uint16_t port,
	uint8_t mode, const char *identity, int8_t *err, void *context)

{
	void *dealer = NULL;
	int res = 0;
	char address[MAX_BUFFER_SIZE];

	if (host == NULL)
		return NULL;

	memset(address, 0, MAX_BUFFER_SIZE);
	sprintf(address, "tcp://%s:%d", host, port);
	*err = 0;

	context  = zmq_ctx_new();

	dealer = zmq_socket(context, ZMQ_DEALER);

	char id[MAX_IDENTITY_LEN];
	memset(id, 0, MAX_IDENTITY_LEN);

	if (identity == NULL)
		sprintf(id, "%04X-%04X", randof(0x10000), randof(0x10000));
	else
		sprintf(id, "%s", identity);

	zmq_setsockopt(dealer, ZMQ_IDENTITY, id, strlen(id));

	res = zmq_connect(dealer, address);

	if (res != 0) {
		DEBUG_PRINT("Error in connecting to router. Error : %d\n", errno);
		*err = errno;
	}

	return dealer;
}

/**
 * @Name : u_sgxcdr_send_zmq_router_socket
 * @arguments : [In] Handle
 *		[In] Buffer
 *		[In] buffer_size
 * @return : Returns number of bytes sent
 * @Description : Function to send data to router socket
 */
int32_t u_sgxcdr_send_zmq_router_socket(void *handle, uint8_t *buffer,
		uint32_t buffer_size)
{

	if (handle == NULL || buffer == NULL || buffer_size == 0) 
		return -1;

	int sent = zmq_send(handle, (char *)buffer, buffer_size, 0);

	return sent;
}

/**
 * @Name : u_sgxcdr_send_zmq_router_read_poll
 * @arguments : [In] Handle
 *		[In] msec
 *		[Out] result
 *		[Out] buffer
 *		[In] buffer_len
 * @return : Returns void
 * @Description : Function polls zmq socket for msec ms and if data avilable
 *		reads data
 */
void u_sgxcdr_zmq_router_read_poll(void *handle, uint32_t msec, int8_t *result,
		uint8_t *buffer, uint32_t buffer_size)
{
	*result = -1;
	
	if (handle == NULL || buffer == NULL || buffer_size == 0)
		return;

	zmq_pollitem_t items[] = {
		{ handle,   0, ZMQ_POLLIN, 0 },
	};

	zmq_poll(items, 1, msec);

	if (items[0].revents & ZMQ_POLLIN) {
		int size = zmq_recv(handle, buffer, buffer_size, 0);

		if (size != -1) {
			//size can exceed the value of buffer_size, if msg was truncated
			int readb = (size > buffer_size) ? buffer_size : size;
			*result = readb;
		}
	}

	return;
}

/**
 * @Name : u_sgxcdr_recv_zmq_router_socket
 * @arguments : [In] Handle
 *		[Out] buffer
 *		[In] buffer_size
 *		[Out] actual_read
 * @return : Returns void
 * @Description : Function get data of buffer_size length from router socket
 */
void u_sgxcdr_recv_zmq_router_socket(void *handle, uint8_t *buffer,
	uint32_t buffer_size, int32_t *actual_read)
{
	if (handle == NULL || buffer == NULL || buffer_size == 0) {
		*actual_read = -1;
		return;
	}

	int32_t len = zmq_recv(handle, buffer, buffer_size, 0);

	*actual_read = -1;
	if (len != -1) {
		//len can exceed the value of buffer_size, if msg was truncated
		int readb = (len > buffer_size)?buffer_size:len;
		*actual_read = readb;
	}
}

/**
 * @Name : u_sgxcdr_close_zmq_router
 * @arguments : [In] Handle
 * @return : Returns 0 on success, -1 on error
 * @Description : Function closes the connection to the ZMQ router
 */
int32_t u_sgxcdr_close_zmq_router(void *handle, void *context )
{
	int32_t flag_close = -1, flag_ctx_destroy = -1;

	if (handle == NULL)
		return -1;

	flag_close = !(zmq_close(handle)) ? 0 : -1;

	flag_ctx_destroy = !(zmq_ctx_destroy(context)) ? 0 : -1;

	if (!flag_close && !flag_ctx_destroy)
	{
		return 0;
	}

	return -1;
}
