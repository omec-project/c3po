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
		nettle-dev
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
	echo "Installing pistache"
	cd $DEPS_DIR/pistache
	mkdir build && cd build
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../
	make -j $CPUS
	$SUDO make install
}

install_build_deps() {
	install_pkg_deps
	install_freediameter
	install_c-ares
	install_cpp-driver
	install_rapidjson
	install_pistache
}

(return 2>/dev/null) && echo "Sourced" && return

set -o errexit
set -o pipefail
set -o nounset

install_build_deps
echo "Dependency install complete"
