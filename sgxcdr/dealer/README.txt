Dealer Build and Run guide

About this Guide

This document provides instructions on "How to build and run Dealer".

===================
Supported Platforms 
===================

1. OS: Ubuntu:16.04, Arch: x64.

============
Dependencies
============

Following is the list of dependencies.

1. Intel SGX for Linux.
2. ZeroMQ library(http://zeromq.org/intro:get-the-software).
3. mbedtls-SGX (https://github.com/bl4ck5un/mbedtls-SGX)
4. sgx_zmq trusted and untrusted library to communicate with ZeroMQ routers.

==================
Build Instructions
==================

* Installer Script
  Execute install.sh script to download, build and install dependencies
  and dealer.

  #./install.sh

* Manual Build
  1. Build and install Intel SGX
     Install the following dependencies for SGX SDK.

     #sudo apt-get update
     #sudo apt-get install libssl-dev libcurl4-openssl-dev libprotobuf-dev
     #sudo apt-get install build-essential

     A] Intel(R) SGX Driver, Version: Linux 1.9

        #wget https://download.01.org/intel-sgx/linux-1.9/sgx_linux_x64_driver_3abcf82.bin
        #chmod +x sgx_linux_x64_driver_3abcf82.bin
        #./sgx_linux_x64_driver_3abcf82.bin

        Expected Output:
        "uninstall.sh script generated in /opt/intel/sgxdriver"
        "Installation is successful!"

     B] Intel(R) SGX Platform Software, Version: Linux 1.9

        #wget https://download.01.org/intel-sgx/linux-1.9/sgx_linux_ubuntu16.04.1_x64_psw_1.9.100.39124.bin
        #chmod +x sgx_linux_ubuntu16.04.1_x64_psw_1.9.100.39124.bin
        #./sgx_linux_ubuntu16.04.1_x64_psw_1.9.100.39124.bin

        Expected Output:
        "uninstall.sh script generated in /opt/intel/sgxpsw"
        "Installation is successful!"

     C] Intel(R) SGX SDK, Version: Linux 1.9
        Note:Install SGXSDK in /opt/intel directory inspite of current directory.

        #wget https://download.01.org/intel-sgx/linux-1.9/sgx_linux_ubuntu16.04.1_x64_sdk_1.9.100.39124.bin
        #chmod +x  sgx_linux_ubuntu16.04.1_x64_sdk_1.9.100.39124.bin
        #./sgx_linux_ubuntu16.04.1_x64_sdk_1.9.100.39124.bin

        Expected Output:
        Installation is successful! The SDK package can be found in /opt/intel/sgxsdk.

        #source /opt/intel/sgxsdk/environment

  2. Build and Install ZeroMQ library

     #sudo apt-get install -y libtool pkg-config autoconf automake uuid-dev
     #wget https://github.com/zeromq/libzmq/releases/download/v4.2.0/zeromq-4.2.0.tar.gz
     #tar xvzf zeromq-4.2.0.tar.gz
     #cd zeromq-4.2.0
     #./configure
     #sudo make install

  3. Build mbedtsl-SGX
   
     #cd {installation_root}/sgxcdr/dealer/deps
     #git clone https://github.com/bl4ck5un/mbedtls-SGX.git
     #cp mbedtls_sgx_ra_*.patch mbedtls-SGX/
     #cd mbedtls-SGX
     #git apply mbedtls_sgx_ra_prebuild.patch
     #mkdir build
     #cd build
     #cmake ..
     #make -j
     #make install

     Expected output:
     "libmbedtls_sgx_t.a" and "libmbedtls_sgx_u.a" files must be generated in
     directory "mbedtls-SGX/mbedtls-SGX/lib"

     #cd ../
     #git apply mbedtls_sgx_ra_postbuild.patch

  4. Build sgx_zmq libraries

     #cd deps/sgx_zmq/sgx_tcdr
     #make
     #cd sgxcdr/dealer/deps/sgx_zmq/sgx_ucdr
     #make

     Expected output:
     "libsgx_tcdr.a" file must be generated in the directory "sgx_zmq/sgx_tcdr".
     "libsgx_uprotected_cdr.a" file must be generated in
     directory "sgx_zmq/sgx_ucdr".

  5. Build Dealer

     #cd {installation_root}/sgxcdr/dealer
     #make SGX_MODE=HW SGX_DEBUG=1

     SGX_MODE=HW will build dealer with SGX hardware mode.
     SGX_DEBUG=1 can be omitted for release build

==========
Run Dealer
==========

    The dealer can be run in two different modes called dealer-in and dealer-out.
    Dealer's mode and other configurations can be set via a configuration file
    or from the command line options.

    #cd {installation_root}/sgxcdr/dealer
    #./dealer

==================
Configuration File
==================

Dealer configurations are stored in a JSON file. Default configuration file 
is: **conf/dealer.json**. Please find a sample config file below:

{
    "common": {
        "maxconnections": "16",
        "kmsserver": "10.212.44.98",
        "kmsport": "8443",
        "keynamespace": "dealer_namespace"
    },
    "dealer": {
        "runmode": "OUT",
        "cdrhost": "10.212.44.100",
        "cdrport": "6790",
        "port": "443",
        "cdrpath": "./cdr",
        "cdrarchpath": "/mnt"
    }
}

Note: 
     1.All values in the config file are strings. Even though they
       are numbers.
     2.Configparser will handle string to number conversion.
     3.The Configuration file has two sections "common" and "dealer".
       "common" section will hold all common configurations for
       dealer-in and dealer-out. "dealer" section will capture 
       configurations that can change based on dealer's type.

====================
Command Line Options
====================

All the options in configurations file can be provided via command line.
Command line options will override the configurations from
the file**. E.g.

  #./dealer -m IN

This will override "runmode" from the configuration file to "IN".

Please find a detailed list of command line options here:

Usage:  dealer [OPTIONS]...
  -h, --help                    Print help and exit
  -m, --runmode mode            Dealer run mode. IN|OUT
  -j, --jsoncfg filename        Read the application configuration from
                                this file.
  -r, --cdrhost host            Hostname/IP of the CDR router host
  -q, --cdrport port            Port number of CDR router.
  -p, --port port               Port number for dealer.
  -k, --kmsserver host          Hostname/IP of KMS server.
  -l, --kmsport port            Port number for KMS server.
  -s, --keynamespace namespace  Namespace to be used for fetching key
                                from KMS.
  -n, --maxconnections number   Maximum no. of connections to be allowed
                                by the dealer.
  -c, --cdrpath                 Path to be used for storing protected file
                                temporarily.
  -d, --cdrarchpath             Path to be used for archieving protected file.

To start dealer with specific configuration file, use following options:

  #./dealer -j <custom-config.json>


===============
Dealer Security
===============

1. CDR csv files can be moved to other directories, but file name can not
   be modified once created
2. CDR csv file contents cannot be changed/added/removed outside trusted
   Dealer control.
3. Encryption key is not visible or accessible to any entity apart from
   KMS Enclave and Dealer Enclave
4. If Dealer must be modified to change contents of the CSV files
5. Dealer change will be reflected in SGX attestation with DP and CTF
   (different MRENCLAVE/MRSIGNER) 
6. 128 bit AES-GCM encryption with csv filename as AAD (Additional
   Authentication Data).
7. While auditing CSV files, auditor must do the following:
     a]. Verify KMS Enclave code for no leaks of encryption key.
         Capture MRENCLAVE and MRSIGNER.
     b]. Verify Dealer Enclave code for no leaks of encryption key.
         Match KMS MRSIGNER with MRSIGNER embeded in Dealer Enclave.
         Capture MRENCLAVE of Dealer.
     c]. Connect to Dealer, verify SGX MRENCLAVE with captured MRENCLAVE
         during attestation.
     d]. Verify MRENCLAVE of KMS with MRENCLAVE part of csv filename.
     e]. Send file name with path to read to DealerOut
         - If error, file is corrupted, if success all records are intact.
