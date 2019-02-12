#include "sgx_tcdr.h"
#include "sgx_tcdr_t.h"

/**
 * @Name : sgx_connect_cdr_mq
 * @arguments : [In] Host
 *    [In] Port
 *    [In] Pattern cand be PUSH or PUSH
 * @return : Returns a ZMQ handle, NULL in case of error
 * @Description : Function to connect to ZMQ in PUSH/PULL pattern
 */
zmq_handle *sgx_connect_cdr_mq(const char *host, uint16_t port,
		zmq_pattern_t pattern)
{
	sgx_status_t status;
	zmq_handle *handle = NULL;

	if (host == NULL)
		return NULL;

	status = u_sgxcdr_connect_cdr_mq(&handle, host, port, pattern);

	if (status != SGX_SUCCESS || handle == NULL)
		return NULL;

	return handle;
}

/**
 * @Name : sgx_get_cdr_message
 * @arguments : [In] Handle
 *       [Out] Buffer
 *       [In] buffer_size
 * @return : Returns numbers of bytes read into buffer
 * @Description : Function to get message from the ZMQ
 */
int32_t sgx_get_cdr_message(zmq_handle *handle, uint8_t *buffer,
		uint32_t buffer_size)
{
	sgx_status_t status;
	int32_t retval = -1;

	if (handle == NULL || buffer == NULL || buffer_size == 0)
		return -1;

	status = u_sgxcdr_get_cdr_message(&retval, handle, buffer, buffer_size);

	return retval;
}

/**
 * @Name : sgx_send_cdr_message
 * @arguments : [In] Handle
 *       [In] Buffer
 *       [In] buffer_size
 * @return : Returns number of bytes written
 * @Description : Function to send a message to the ZMQ
 */
int32_t sgx_send_cdr_message(zmq_handle *handle, uint8_t *buffer,
		uint32_t buffer_size)
{
	sgx_status_t status;
	int32_t retval = -1;

	if (handle == NULL || buffer == NULL || buffer_size == 0)
		return -1;
	
	status = u_sgxcdr_send_cdr_message(&retval, handle, buffer, buffer_size);

	return retval;
}

/**
 * @Name : sgx_close_cdr_mq
 * @arguments : [In] Handle
 * @return : 0 - success, -1 - error
 * @Description : Function closes the connection to the ZMQ
 */
int32_t sgx_close_cdr_mq(zmq_handle *handle)
{
	sgx_status_t status;
	int32_t retval = -1;

	if (handle == NULL)
		return -1;
	
	status = u_sgxcdr_close_cdr_mq(&retval, handle);

	return retval;
}

/**
 * @Name : sgx_open_cdr_file
 * @arguments : [In] filename
 *          [In] mode
 *          [In] key used for encryption
 * @return : Returns a file handle
 * @Description : Function opens a SGX protected FS file
 */
SGX_FILE *sgx_open_cdr_file(const char *filename, const char *mode,
		sgx_key_128bit_t *key)
{
	return sgx_fopen(filename, mode, key);
}

/**
 * @Name : sgx_close_cdr_file
 * @arguments : [In] file handle to close a file
 * @return : Returns 0 - success, -1 - error
 * @Description : Function to close a SGX protected FS file
 */
int32_t sgx_close_cdr_file(SGX_FILE *file)
{
	return sgx_fclose(file);
}

/**
 * @Name : sgx_read_cdr_file
 * @arguments : [Out] ptr_buff Pointer to a buffer to receive the read data
 *          [In] size of each block
 *          [In] count The number of blocks to be read
 *          [In] file handle
 * @return : Returns a number of blocks that were read from the file
 * @Description : Function reads a requested amount of data from the protected
 *       FS file
 */
int sgx_read_cdr_file(void *ptr_buff, size_t size, size_t count, SGX_FILE *file)
{
	return sgx_fread(ptr_buff, size, count, file);
}

/**
 * @Name : sgx_write_cdr_file
 * @arguments : [In] ptr_buff Pointer to a buffer contains data to write to file
 *          [In] size of each block
 *          [In] count The number of blocks to be written
 *          [In] file handle
 * @return : Returns a number of blocks that were written to the file
 * @Description : Function writes the given amount of data to the protected
 *          FS file
 */
int sgx_write_cdr_file(void *ptr_buff, size_t size, size_t count,
		SGX_FILE *file)
{
	return sgx_fwrite(ptr_buff, size, count, file);
}

/**
 * @Name : sgx_flush_cdr_file
 * @arguments : [In] file handle to flush a file
 * @return : Returns 0 - success, 1 - error
 * @Description : Function forces a cache flush
 */
int sgx_flush_cdr_file(SGX_FILE *file)
{
	return sgx_fflush(file);
}

int ecall_sgx_tcdr()
{
	return 0;
}
