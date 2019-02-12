//TODO: Add license
#include <time.h>
#include <errno.h>
#include <string.h>

#include "helper.h"
#include "crypto.h"
#include "kms_client.h"
#include "ssl_conn_hdlr.h"
#include "Enclave_t.h"

#define BUFLEN 2048

#define CSV_EXTN ".csv"
#define CSV_EXTN_LEN 4

static Crypto AESGCM(CRYPTO_TYPE_AES_GCM);

/**
 * @Name : get_message_type
 * @arguments : [In] msg_type
 * @return : Returns message type
 * @Description : Function to check message type
 */
int get_message_type(char msg_type)
{
   if (msg_type - '0' == MSG_FILENAME)  
      return MSG_FILENAME;
   else if (msg_type - '0' == MSG_DATA)
      return MSG_DATA; 
   else if (msg_type - '0' == MSG_CLOSE_PROTECTED_FILE)
      return MSG_CLOSE_PROTECTED_FILE;
   else 
      return MSG_UNKNOWN; 
}

/**
 * @Name : get_msg_out_router_send_to_ctf
 * @arguments : [In] thread_id
 *		[In] ssl mbedtls ssl context
 *    [In] cdr_out_router_host
 *    [In] cdr_out_router_port
 * @return : Returns 0 on success
 * @Description : Function to read messages from Router-Out and send those
      records to CTF
 */
int get_msg_out_router_send_to_ctf(long int thread_id, mbedtls_ssl_context *ssl,
   char *cdr_out_router_host, int cdr_out_router_port, char *key_from_kms,
   int offline, std::string &cdrPath, std::string &cdrArchPath)
{
   zmq_handle* handle = NULL;
   zmq_context *context = NULL;
   SGX_FILE *protected_fp = NULL;
   char protected_filename[4096] = {0};
   sgx_status_t status;
   char read_buff[READ_BUFFER_SIZE];
   char dealer_identity[50];
   int read_bytes = 0, send_bytes = 0, ret;
   int8_t err = 0;
   size_t lenOut;

   memset(dealer_identity, 0, 50);

   // read identity from CTF
   unsigned char buf[256];
   char stream_name[256];
   memset(stream_name, 0, 256);
   memset(buf, 0, 256);

   if (offline) {
	static int counter = 1;
	snprintf(dealer_identity, 50, "2%d", counter++);
	mbedtls_printf("dealer identity %s\n", dealer_identity);
   } else {
	   ret = mbedtls_ssl_read(ssl, buf, 256);
	   if(ret <=0)
	   {
	       mbedtls_printf("failed to read stream name, mbedtls_ssl_read returned -0x%04x\n",-ret);
	       return -1;
	   }
	   
	   char stream_type = buf[0];
	   for(int i=1; i<256; i++)
	   {
	       if(buf[i] == '\0')
	         break;
	       stream_name[i-1] = buf[i];
	   }
	   mbedtls_printf("DEBUG: stream type: %c, stream name: %s", stream_type, stream_name);
	
		switch (stream_type) {
		case '0': // live stream with name
			snprintf(dealer_identity, 50, "0%s", stream_name);
			break;
	
		case '1': // archived stream from a file
			mbedtls_printf("INFO : reading from an archive stream named: %s\n",
					stream_name);
			if (read_stream_from_file(thread_id, ssl, stream_name, key_from_kms) < 0) {
				mbedtls_printf(
						"ERROR : failed reading from an archive stream %s, checking for live stream\n",
						stream_name);
				return -1;
			} else {
				mbedtls_printf("INFO : stream read from an archive %s \n",
						stream_name);
				return 0;
			}
			break;
	
		case '2': // live steam, read from any avialable stream
	
			memset(dealer_identity, 0, 50);
			snprintf(dealer_identity, 50, "2%s", stream_name);
			break;
	
		default:
			mbedtls_printf("Invalid stream type, it must be 0,1 or 2");
			return -1;
	
		}
	
	   mbedtls_printf("INFO: dealer's identity: %s\n", dealer_identity);
   }

   status  = u_sgxcdr_connect_to_zmq_router(&handle, cdr_out_router_host,
         cdr_out_router_port, 0, dealer_identity, &err, &context);

   if (status != SGX_SUCCESS || handle == NULL || err != 0) {
      mbedtls_printf("Connecting to Router-Out %s:%d failed with error %d\n",
         cdr_out_router_host, cdr_out_router_port, err);
      return ERR_ZMQ_CONN_ROUTER_FAIL;
   }
   mbedtls_printf("Connected to Router-Out : %s:%d\n", cdr_out_router_host,
      cdr_out_router_port);

   while(1) {
   status = u_sgxcdr_send_zmq_router_socket((int *) &send_bytes, handle,
      (uint8_t *) dealer_identity, strlen(dealer_identity));
   if (status != SGX_SUCCESS || send_bytes == -1) {
      mbedtls_printf("Send zmq router socket failed\n");
      return ERR_ZMQ_SEND_ROUTER_FAIL;
   }
   //mbedtls_printf("Sent data is %s and number of bytes are %d \n",
   //   dealer_identity, send_bytes);

   int retval = -1;
   memset(read_buff, 0, READ_BUFFER_SIZE);
   status = u_sgxcdr_zmq_router_read_poll(handle, POLL_INTERVAL,
      (int8_t *) &retval, (uint8_t *) &read_buff, READ_BUFFER_SIZE);
   if (status != SGX_SUCCESS) {
      mbedtls_printf("Zmq read poll failed\n");
      return ERR_ZMQ_POLL_READ_ROUTER_FAIL;
   }
   if (retval == -1) {
      mbedtls_printf("No data present for read poll after %d(ms) timeout \n", POLL_INTERVAL);
      //close_router_conn(handle);
      //return ERR_ZMQ_POLL_READ_ROUTER_TIMEOUT;
      //try reading again after timeout
      continue;
   } else{
      //mbedtls_printf("First message on read poll : %s and number of bytes are %d\n", read_buff, retval);
      mbedtls_printf("");
      break;
   }
   } // while(1) ends here

   if (get_message_type(read_buff[0]) == MSG_FILENAME) {
	   // crypto setup
      strncpy(protected_filename, read_buff + 1, strlen(read_buff) -1);

      AESGCM.set_params_aesgcm((uint8_t *)key_from_kms,
               (unsigned char *)protected_filename,
               strlen((const char *) protected_filename));

      if (cdrPath.at(cdrPath.size() - 1) != '/')
               cdrPath.append("/");

      cdrPath.append(protected_filename, strlen(protected_filename));

      // Append MRENCLAVE of KMS before ".extn"
      size_t pos = cdrPath.find_last_of(".");
      if (pos == string::npos)
         pos = cdrPath.size();

      cdrPath.insert(pos, "_");
      cdrPath.insert(pos + 1, kms_mrenclave);

      if ((protected_fp = open_secure_file(cdrPath.c_str(), "a+",  key_from_kms)) == NULL) {
         mbedtls_printf("Unable to open protected fs file for %s\n", cdrPath.c_str());
         return -1;
      }
   }

   //NEED TO REMOVE
   uint8_t *decrypted_str = NULL;

   while(1) {

      memset(read_buff, 0, READ_BUFFER_SIZE);
      read_bytes = -1;

      status = u_sgxcdr_recv_zmq_router_socket(handle, (uint8_t *) &read_buff,
         READ_BUFFER_SIZE, &read_bytes);

      if (status != SGX_SUCCESS || read_bytes == -1) {
         mbedtls_printf("Recv zmq router socket failed\n");
	      // continue reading data, try again
	      //continue;
         return ERR_ZMQ_RECV_ROUTER_FAIL;
      } else {
         decrypted_str = (uint8_t *) malloc(read_bytes + 1);// +1 for '\0'
         memset(decrypted_str, 0, read_bytes + 1);

         if (get_message_type(read_buff[0]) == MSG_DATA) {
            AESGCM.decrypt(read_buff+1, read_bytes-1,
                                     (char *) decrypted_str, &lenOut);
            //mbedtls_printf("decrypted buff is %s \n", decrypted_str);
         } else if (get_message_type(read_buff[0]) == MSG_CLOSE_PROTECTED_FILE) {
            close_protected_fs(protected_fp);
            protected_fp = NULL;
            close_router_conn(handle, context);

            if (cdrArchPath.at(cdrArchPath.size() - 1) != '/')
              cdrArchPath.append("/");

            cdrArchPath.append(protected_filename, strlen(protected_filename));

            // Append MRENCLAVE of KMS before ".extn"
            size_t pos = cdrArchPath.find_last_of(".");
            if (pos == string::npos)
              pos = cdrArchPath.size();

            cdrArchPath.insert(pos, "_");
            cdrArchPath.insert(pos + 1, kms_mrenclave);

            mbedtls_printf("moving file %s to %s \n", cdrPath.c_str(),
                    cdrArchPath.c_str());

            ocall_rename_file(cdrPath.c_str(), cdrArchPath.c_str());

            return -1;
            // return error code to indicate discard connection with CTF
         }

         if (protected_fp)
            write_to_protected_fs(protected_fp, decrypted_str, lenOut);
            //mbedtls_printf("DEBUG : Received buffer %s from out-router\n",
            //   decrypted_str);
      }

      if (!offline) {
	      while ((ret = mbedtls_ssl_write(ssl, (unsigned char *) decrypted_str,
	         lenOut)) <= 0) {
	         //if (ret == -80)
	         //mbedtls_printf(" failed: %d\n",ret);
	         if (ret == MBEDTLS_ERR_NET_CONN_RESET) {
	            mbedtls_printf("  [ #%ld ]  failed: peer closed the connection\n",
	               thread_id);
	            close_protected_fs(protected_fp);
	            close_router_conn(handle, context);
                    mbedtls_printf("Got return %d from close router conn for thread id %ld\n",ret,thread_id);
                    return MBEDTLS_ERR_NET_CONN_RESET;
	         }
	         if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
	            mbedtls_printf("  [ #%ld ]  failed: mbedtls_ssl_write returned -0x%04x\n",thread_id, ret);
	            return -1;
	   	   }
	      }
      }
      if (decrypted_str)
         free(decrypted_str);
   }// while 1 
   close_protected_fs(protected_fp);
   close_router_conn(handle, context);
   return 0;
}

/**
 * @Name : close_router_conn
 * @arguments : [In] handle
 * @return : Returns 0 on success and -1 on error
 * @Description : Function to close connection with router
 */
int close_router_conn(zmq_handle* handle, zmq_context *context)
{
   int retval = -1;

   if (u_sgxcdr_close_zmq_router((int *) &retval, handle, context) != SGX_SUCCESS) {
      mbedtls_printf("Close zmq router failed\n");
      return -1;
   }

   if (retval) {
      mbedtls_printf("Failed to close router connection\n\n");
      return -1;
   }
   mbedtls_printf("Closed router connection \n\n");
   return 0;
}

/* TODO :
   1. Should have sgx key input parameter
   2. Able to search whether DP named protected file is present or not
*/ 

/**
 * @Name : open_protected_fs_file
 * @arguments : [In] filename
 * @return : Returns file handle on success and NULL on failure
 * @Description : Function to open Protected FS file
 */
SGX_FILE *open_protected_fs_file(const char *filename, char *key)
{
   const char *mode = "a+";

   return open_secure_file(filename, mode, key);
}

SGX_FILE *open_secure_file(const char *filename, const char *mode, char *key_from_kms)
{
	SGX_FILE *file_handle = NULL;
	sgx_key_128bit_t key;

	// convert string key to sgx key data type
	memset(key, 0, sizeof(sgx_key_128bit_t));
	//key[2] = 'a';
	for(int i=0; i<SGX_AESCTR_KEY_SIZE; i++){
		key[i] = key_from_kms[i];
	}

	errno = 0;
	file_handle = sgx_open_cdr_file(filename, mode, &key);
	if (file_handle == NULL) {
		mbedtls_printf("sgx_open_cdr_file() : Failed to open %s file erro %d \n",
				filename, errno);
		return NULL;
	}
	//mbedtls_printf("sgx_open_cdr_file() : Opened %s file\n", filename);
	return file_handle;
}

// internal function to process file stream
int process_messages(SGX_FILE *fp, char *message) {
	char buf[2] = { 0 };
	int index;

	index = 0;
	while (!sgx_feof(fp)) {
		sgx_fread(buf, 1, 1, fp);
		if (sgx_ferror(fp) != 0) {
			mbedtls_printf("Failed to read record from protected file \n");
			return -1;
		}

		message[index++] = buf[0];
		if (buf[0] == '\n') {
			message[index] = 0;
			index = 0;
			//std::cout << "Message: " << message << std::endl;
			//mbedtls_printf("DEBUG: message from archived stream: %s\n", message);
			break;
		}
	}

	// no new line is detected at the end of the file
	if (strlen(message) <= 0 || index!=0) {
		message[0] = 0;
	}

	return 0;
}

/**
 * @Name : read_stream_from_file
 * @arguments : [In] thread_id
 * @arguments: [In] ssl
 * @arguments: [In] filename
 * @return : Returns 0 on success and -1 on error
 * @Description : Read stream data from an archived file
 */
int read_stream_from_file(long int thread_id, mbedtls_ssl_context *ssl, char *filename, char *key)
{
	SGX_FILE *handle = NULL;
	const char *mode = "r";
	int read_blocks = -1;
	char *read_buff;
	int ret;

	read_buff = (char*)malloc(1024);
	if(read_buff == NULL)
	{
		mbedtls_printf("failed to allocate memory for the buffer");
		return -1;
	}

	memset(read_buff, 0, 1024);

	// Check if filename contains MRENCLAVE value after "_" and before "extn"
	string cdrPath(filename);
	size_t pos = cdrPath.find_last_of("_");
	if (pos == string::npos) {
		mbedtls_printf("file name not valid.\n");
                free(read_buff);
                read_buff=NULL;
		return -1;
	}

	if (cdrPath.compare(pos + 1, SGX_HASH_SIZE * 2, kms_mrenclave) != 0) {
		mbedtls_printf("filename doesn't contain valid MRENCLAVE value.\n");
                free(read_buff);
                read_buff=NULL;
		return -1;
	}

	handle = open_secure_file(filename, mode, key);
	if (handle != NULL) {

		while (true) {
			// read message lines from the file
			//mbedtls_printf("processing messages ..\n");
			int r = process_messages(handle, read_buff);
			if (r != 0) {
				mbedtls_printf("read error code: %d \n", r);
				close_protected_fs(handle);
				free(read_buff);
				return -1;
			}

			// break on no message
			read_blocks = strnlen(read_buff, 1024);
			if(read_blocks == 0)
				break;

			// send data to CTF
			while ((ret = mbedtls_ssl_write(ssl, (unsigned char *) read_buff,
					read_blocks)) <= 0) {
				if (ret == MBEDTLS_ERR_NET_CONN_RESET) {
					mbedtls_printf(
							"  [ #%ld ]  failed: peer closed the connection\n",
							thread_id);
					break;
				}
				if (ret != MBEDTLS_ERR_SSL_WANT_READ
						&& ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
					mbedtls_printf(
							"  [ #%ld ]  failed: mbedtls_ssl_write returned -0x%04x\n",
							thread_id, ret);
					break;
				}
			} // while

		} // while(read_blocks > 0)
	}
	else
	{
		close_protected_fs(handle);
		free(read_buff);
		return -1;
	}

	close_protected_fs(handle);
	free(read_buff);
	return 0;
}

/**
 * @Name : write_to_protected_fs
 * @arguments : [In] file_handle
 *		[In] write_buff Data to write to Protected filesystem
 * @return : Returns number of blocks successfully written
 * @Description : Function to write data to Protected file
 */
int write_to_protected_fs(SGX_FILE *file_handle, uint8_t *write_buff, size_t len)
{
   int write_blocks = -1;

   write_blocks = sgx_write_cdr_file(write_buff, 1, len, file_handle);
   if (sgx_ferror(file_handle) != 0) { 
      mbedtls_printf("sgx_write_cdr_file() : Failed to write %s record to protected file \n", write_buff);
   } else {
#ifdef FLUSH_BUILD 
      //mbedtls_printf("FLUSH MODE\n");
      if (sgx_flush_cdr_file(file_handle) != 0) {
         mbedtls_printf("Flush failed for record : %s \n", write_buff);
      }
      //mbedtls_printf("sgx_write_cdr_file() : Number of blocks written %d \n",
      //   write_blocks);
#endif
      //mbedtls_printf("Record written to procted fs: %s \n", write_buff);
   }
   return write_blocks;
}

/**
 * @Name : close_protected_fs
 * @arguments : [In] file_handle
 * @return : Returns 0 on success and -1 on failure
 * @Description : Function to close Protected FS file
 */
int close_protected_fs(SGX_FILE *file_handle)
{
   if (!sgx_close_cdr_file(file_handle)) {
      //mbedtls_printf("sgx_close_cdr_file() : File closed successfully \n\n");
   } else {
      mbedtls_printf("sgx_fclose_cdr_file() : Failed to close file \n");
      return -1;
   }
   return 0;
}

/**
 * @Name : conn_to_in_router
 * @arguments : [In] thread_id
 *    [In] cdr_in_router_host
 *    [In] cdr_in_router_port
 *		[In] ssl dealer_identity IP address of DP
 * @return : Returns handle on success and NuLL on failure
 * @Description : Function to connect to Router-In over ZMQ protocol
 */
zmq_handle *conn_to_in_router(char *cdr_in_router_host, int cdr_in_router_port,
   char *dealer_identity)
{
   zmq_handle *handle = NULL;
   zmq_context *context = NULL;
   int8_t err = 0;
   sgx_status_t status; 
   
   status  = u_sgxcdr_connect_to_zmq_router(&handle, cdr_in_router_host,
         cdr_in_router_port, 0, dealer_identity, &err, &context);
   if (status != SGX_SUCCESS || handle == NULL || err != 0) {
      mbedtls_printf("Connecting to Router-In %s:%d failed with error %d\n",
         cdr_in_router_host, cdr_in_router_port, err);
      return NULL;
   }
   mbedtls_printf("Connected to Router-In : %s:%d \n", cdr_in_router_host,
      cdr_in_router_port);

   return handle;
}

/**
 * @Name : send_msg_to_in_router
 * @arguments : [In] handle
 *		[In] msg Data to be send
 *    [In] len Length of data
 * @return : Returns 0 on success and -1 on error
 * @Description : Function to send data to Router-In
 */
int send_msg_to_in_router(zmq_handle *handle, unsigned char *msg, int len)
{
   int send_bytes = 0;
   sgx_status_t status; 

   status = u_sgxcdr_send_zmq_router_socket((int *) &send_bytes, handle,
      (uint8_t *) msg, len);
   if (status != SGX_SUCCESS || send_bytes== -1) {
      mbedtls_printf("Send zmq router socket failed\n");
      return -1;
   }		
   //mbedtls_printf("Sent number of bytes are %d \n", send_bytes);

   return 0;
}

