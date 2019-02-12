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

install_zmq_lib()
{
	#build and install ZeroMQ library
	sudo apt-get install -y libtool pkg-config autoconf automake uuid-dev
	
	wget https://github.com/zeromq/libzmq/releases/download/v4.2.0/zeromq-4.2.0.tar.gz -P /tmp/sgxcdr
	cd /tmp/sgxcdr
	tar xvzf zeromq-4.2.0.tar.gz
	cd zeromq-4.2.0
	./configure
	make
	sudo make install
	ldconfig
}

build_dealer_deps()
{
	sudo apt-get install cmake
	cd $BASEDIR/deps
	
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

	#build sgx_tcdr
	cd ../sgx_zmq/sgx_tcdr/
	make
	
	#build sgx_ucdr
	cd ../sgx_ucdr/
	make
	
	#download rapidjson
	git submodule init 
  	git submodule update
}

build_dealer()
{
	cd $BASEDIR
	make clean
	make SGX_MODE=HW SGX_DEBUG=1
}

install_deps()
{
	install_intel_sgx
	install_zmq_lib
}

get_dealer_hash()
{	
	output=($(./dealer -j conf/dealer.json -x | sed -n 's/MR.* ://p'))
	
	MRENCLAVE=${output[0]}
	MRSIGNER=${output[1]}
}

replace_kms_mrsigner()
{	
	#replace MRSIGNER value of KMS in Enclave/ca_bundle.h file
	sed -i "/#define KMS_MRSIGNER/c\#define KMS_MRSIGNER \"${MRSIGNER}\"" Enclave/ca_bundle.h
}

print_hash_message()
{
	echo "---------------------------------------------------------------------------"
	echo "Use MRENCLAVE and MRSIGNER values while building KMS."
	echo "./install.sh <MRENCLAVE> <MRSIGNER>"
	echo "MRENCLAVE : $MRENCLAVE"
	echo "MRSIGNER  : $MRSIGNER"
	echo "---------------------------------------------------------------------------" 
}

BASEDIR=$PWD

install_deps
build_dealer_deps
build_dealer
get_dealer_hash
replace_kms_mrsigner
build_dealer
get_dealer_hash
print_hash_message

