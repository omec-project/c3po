About : 
   Provides ZMQ socket support and protected fs wrapper for secure CDR
   processing.

Source code structure:
 - sgx_tcdr : Trusted static library to be linked with an enclave. Exposes APIs
   to connect to ZMQ in PULL/PUSH config, connect to ZMQ router and read/write
   messages. Also exposes wrapper API over protected FS. 
 - sgx_ucdr : Untrusted static library to be linked with an untrusted
   Application. Actual implementation of ZMQ mechanism.
 
Prerequisites:
   - Ubuntu 16.04
   - Intel SGX SDK 1.9
   - ZeroMQ 4.0.* (http://download.zeromq.org/#ZeroMQ_4)
   
Build Library :
   - Inside sgx_tcdr directory, run "make" to build libsgx_tcdr.a library.
   - Inside sgx_ucdr directory, run "make" to build libsgx_ucdr.a library.
   
Usage : 
   Makefile :  
      - Define a variable CDR_LIB_PATH with value of parent directory of
        sgx_tcdr and sgx_ucdr.
      - In App Include Path add "-I$(CDR_LIB_PATH)/sgx_ucdr"
      - In App Link Flags add "-lsgx_uprotected_fs -L$(CDR_LIB_PATH)/sgx_ucdr
        -lsgx_uprotected_cdr -lzmq"
      - In Enclave Include Path add "-I$(CDR_LIB_PATH)/sgx_tcdr"
      - In Enclave Link Flags add "-L$(CDR_LIB_PATH)/sgx_tcdr" after
        "-L$(SGX_LIBRARY_PATH)". Add " -lsgx_tcxx -lsgx_tcdr
        -lsgx_tprotected_fs" and between --start-group and --end-group.
   
   Enclave : 
      - Include "include/sgx_tcdr.h" from Enclave code to call trusted
        functions. Also import all the functions from
        sgx_tcdr/trusted/sgx_tcdr.edl in Enclave edl file.
      
   Untrusted App:
      - Include "uprotected_cdr.h from Untrusted App code.
   
Limitations:
   - Platform dependednt. Tested only on Ubuntu 16.04.

TODO :
   - Add custom error codes.

