#! /bin/bash

install_intel_sgx()
{
	#install Intel(R) SGX dependencies
	sudo apt-get update
	sudo apt-get install libssl-dev libcurl4-openssl-dev libprotobuf-dev build-essential
	
	#install Intel(R) SGX Driver
	wget https://download.01.org/intel-sgx/linux-1.9/sgx_linux_x64_driver_3abcf82.bin -P /tmp/sgxcdr
	chmod +x /tmp/sgxcdr/sgx_linux_x64_driver_3abcf82.bin
	sudo /tmp/sgxcdr/sgx_linux_x64_driver_3abcf82.bin
	
	#install Intel(R) SGX Platform Software
	wget https://download.01.org/intel-sgx/linux-1.9/sgx_linux_ubuntu16.04.1_x64_psw_1.9.100.39124.bin -P /tmp/sgxcdr
	chmod +x /tmp/sgxcdr/sgx_linux_ubuntu16.04.1_x64_psw_1.9.100.39124.bin
	sudo /tmp/sgxcdr/sgx_linux_ubuntu16.04.1_x64_psw_1.9.100.39124.bin	
	
	#install Intel(R) SGX SDK
	wget https://download.01.org/intel-sgx/linux-1.9/sgx_linux_ubuntu16.04.1_x64_sdk_1.9.100.39124.bin -P /tmp/sgxcdr
	chmod +x  /tmp/sgxcdr/sgx_linux_ubuntu16.04.1_x64_sdk_1.9.100.39124.bin
	sudo /tmp/sgxcdr/sgx_linux_ubuntu16.04.1_x64_sdk_1.9.100.39124.bin

	source /opt/intel/sgxsdk/environment
}

build_kms_deps()
{
	sudo apt-get install cmake

	cd $DEALERDIR/deps

	#Clone MBEDTLS-2.15.1
	git clone https://github.com/ARMmbed/mbedtls.git -b mbedtls-2.15.1

	#Clone & build mbedtls-SGX
	git clone https://github.com/bl4ck5un/mbedtls-SGX.git
	cp mbedtls_sgx_ra_*.patch mbedtls-SGX/

	cd mbedtls-SGX
	git checkout 1529158

	rm -rf trusted/mbedtls-2.6.0/library
	rm -rf trusted/mbedtls-2.6.0/include
	cp -r ../mbedtls/library trusted/mbedtls-2.6.0/.
	cp -r ../mbedtls/include trusted/mbedtls-2.6.0/.
	rm -rf ../mbedtls

	patch -p0 < mbedtls_sgx_ra_prebuild.patch

	mkdir build
	cd build
	cmake ..
	make -j
	make install

	sleep 3

	cd ../
	patch -p0 < mbedtls_sgx_ra_postbuild.patch

	cp build/mbedtls_SGX-2.6.0/libs/libmbedtls_SGX_u.a build/mbedtls_SGX-2.6.0/lib/.
	
	#download rapidjson
	git submodule init
  	git submodule update
}

build_kms()
{
	cd $BASEDIR
	make clean
	make SGX_MODE=HW SGX_DEBUG=1
}

install_deps()
{
	install_intel_sgx
}

replace_dealer_mrenclave_mrsigner()
{
	cd $BASEDIR
	#replace MRENCLAVE value of Dealer in Enclave/ca_bundle.h file
	sed -i "0,/DEALER_MRENCLAVE/{s/DEALER_MRENCLAVE/${DEALER_MRENCLAVE}/}" Enclave/ca_bundle.h
	
	#replace MRSIGNER value of Dealer in Enclave/ca_bundle.h file
	sed -i "0,/DEALER_MRSIGNER/{s/DEALER_MRSIGNER/${DEALER_MRSIGNER}/}" Enclave/ca_bundle.h
}

BASEDIR=$PWD
DEALERDIR=$BASEDIR/../dealer

DEALER_MRENCLAVE=$1
DEALER_MRSIGNER=$2

install_deps
build_kms_deps
replace_dealer_mrenclave_mrsigner
build_kms

