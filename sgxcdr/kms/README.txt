Key Manager Service(KMS) Build and Run guide

Key Manager Service is an SGX based program to serve keys to SGX dealer.
KMS uses SGX to secure the key infra structure.

About this guide

This document provides instructions on "How to build and run KMS".

====================
Design Consideration
====================
KMS will have two types of keys: master key and namespace key.

  A] Master Key
    1. Master key is used to encrypt all namespace keys.
    2. KMS will generate master key for the very first time it started
       or if existing master key is removed by administrator.
    3. Master key will be sealed and stored in the key store as
       file(kms_fs_key.key).
    4. Master key will have double encryption as it is sealed and
       stored on the disk using sgx protected filesystem.
    5. Master key is encrypted using signing identity and ISVSVN
       value with protected filesystem (using autokey as
       filesystem encryption key).
    6. Only KMS can access the master key, it will never be
       delivered to KMS client.

  B] Namespace Key
    1. KMS client(here it is Dealer) can request for any namespace key.
    2. Namespace is a unique string to indentify key.
    3. KMS will create new namespace key if it does not exists.
    4. Namespace key will be stored on the disk using sgx protected
       filesystem. Master key will be used as filesystem key for
       namespace keys.

==================
KMS Authentication
==================
1. SGX Remote Attestation(TLS based) will be used to authenticate
   all KMS clients.
2. KMS will verify both MRENCLAVE and MRSIGNER against the SGX report
   provided with Dealer certificate.

===================
Supported Platforms
===================

1. OS: Ubuntu:16.04, Arch: x64

============
Dependencies
============

Following is the list of dependency.

1. Intel SGX for Linux.
2. mbedtls-SGX (https://github.com/bl4ck5un/mbedtls-SGX)

==================
Build Instructions
==================

* Installer Script
  Execute install.sh script to download, build and install dependencies
  and kms.

  #./install.sh

* Manual Build
  1. Build and install Intel SGX
     Install the following dependencies for SGX SDK.

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

  2. Build mbedtsl-SGX

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

  3. Build KMS

     #cd {installation_root}/sgxcdr/kms
     #make SGX_MODE=HW SGX_DEBUG=1

     SGX_MODE=HW will build kms with SGX hardware mode.
     SGX_DEBUG=1 can be omitted for release build

=======
Run KMS
=======

    Configurations can be set via a configuration file or from the
    command line options.

    #cd {installation_root}/sgxcdr/kms
    #./kms

==================
Configuration File
==================
KMS configurations are stored in a JSON file. Default configuration file
is: **conf/kms.json**. Please find a sample config file below:

{
  "kms": {
      "port": "443",
      "maxconnections": "16",
      "storepath": "./store",
    }
}

Note: 
     1. All values in the config file are strings. Even though they are numbers.
     2. Configparser will handle string to number conversion.
     3. Storpath directory must exists. KMS will not create any such directory.

====================
Command Line Options
====================

All the options in configurations file can be provided via command line.
Command line options will override the configurations from
the file**. E.g.

 #./kms -p 8443

This will override "port" from the configuration file to "8443".

Please find a detailed list of command line options here:

Usage:  kms [OPTIONS]...
  -h, --help                   Print help and exit.
  -j, --jsoncfg filename       Read the application configuration from this file.
  -p, --port port              Port number for kms.
  -s, --storepath path         Path to store sealed key files.
  -n, --maxconnections number  Maximum no. of connections to be allowed by the kms.


To start KMS with specific configuration file, use following options:

 #./kms -j <custom-config.json>
