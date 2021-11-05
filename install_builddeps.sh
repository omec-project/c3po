#!/usr/bin/env bash
# Copyright 2019-present Open Networking Foundation
# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2019 Intel Corporation

SUDO=''
[[ $EUID -ne 0 ]] && SUDO=sudo

CUR_DIR=$PWD
function finish() {
	cd $CUR_DIR
}
trap finish EXIT

install_pkg_deps() {
	$SUDO apt-get update && $SUDO apt-get -y install \
		automake \
		autotools-dev \
		bison \
		cmake \
		flex \
		g++ \
		gcc \
		libgcrypt-dev \
		libgnutls-dev \
		libidn11-dev \
		libmemcached-dev \
		libsctp-dev \
		libssl-dev \
		libtool-bin \
		libuv1-dev \
		m4 \
		make \
		memcached \
		nettle-dev \
 		curl \
 		libcurl4-openssl-dev \
 		wget
}

DEPS_DIR=${DEPS_DIR:-"$PWD/modules"}
CPUS=${CPUS:-''}

install_freediameter() {
	echo "Installing freeDiameter"
	cd $DEPS_DIR/freeDiameter
	mkdir build && cd build
	cmake -DDISABLE_SCTP:BOOL=ON ../
	make -j $CPUS
	$SUDO make install
}

install_c-ares() {
	echo "Installing c-ares"
	cd $DEPS_DIR/c-ares
	./buildconf
	./configure
	make -j $CPUS
	$SUDO make install
}

install_cpp-driver() {
	echo "Installing cpp-driver"
	cd $DEPS_DIR/cpp-driver
	mkdir build && cd build
	cmake ../
	make -j $CPUS
	$SUDO make install
}

install_rapidjson() {
	echo "Installing rapidjson"
	cd $DEPS_DIR/rapidjson
	mkdir build && cd build
	cmake ../
	make -j $CPUS
	$SUDO make install
}

install_pistache() {
	cd /tmp
	wget https://github.com/Kitware/CMake/releases/download/v3.18.0/cmake-3.18.0-Linux-x86_64.tar.gz 
	tar -zxvf cmake-3.18.0-Linux-x86_64.tar.gz
	echo "Installing pistache"
	cd $DEPS_DIR/pistache
  echo $PWD
	patch -p1 < ../../patches/pistache.patch.1.txt
	mkdir build && cd build
	/tmp/cmake-3.18.0-Linux-x86_64/bin/cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../
	make -j $CPUS
	$SUDO make install
}

install_prometheus() {
    set -xe 
    pushd /tmp
    pushd $DEPS_DIR/prometheus-cpp
    mkdir -p _build && cd _build
    /tmp/cmake-3.18.0-Linux-x86_64/bin/cmake .. -DBUILD_SHARED_LIBS=ON && make -j 4 && $SUDO make install && $SUDO make DESTDIR=`pwd`/deploy install
    echo $PWD
    popd
    popd
}

install_build_deps() {
	install_pkg_deps
	install_freediameter
	install_c-ares
	install_cpp-driver
	install_rapidjson
	install_pistache
	install_prometheus
}

(return 2>/dev/null) && echo "Sourced" && return

set -o errexit
set -o pipefail
set -o nounset

install_build_deps
echo "Dependency install complete"
