#!/usr/bin/env bash
# Copyright 2019-present Open Networking Foundation
# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2019 Intel Corporation

SUDO=''
[[ $EUID -ne 0 ]] && SUDO=sudo

update_pkg_cache() {
	$SUDO apt-get update
}

install_run_cdf_deps() {
	$SUDO apt-get -y install \
		libgnutls30 \
		libidn11
}

install_run_ctf_deps() {
	$SUDO apt-get -y install \
		libgnutls30 \
		libidn11 \
		libmemcached11
}

install_run_hss_deps() {
	$SUDO apt-get -y install \
		libgmp10 \
		libgnutls30 \
		libidn11 \
		libnettle6 \
		libssl1.0.0 \
		libuv1 \
		openssl
}

install_run_hssgtw_deps() {
	$SUDO apt-get -y install \
		libssl1.0.0 \
		libuv1
}

install_run_pcrf_deps() {
	$SUDO apt-get -y install \
		libgmp10 \
		libgnutls30 \
		libidn11 \
		libnettle6 \
		libssl1.0.0 \
		libuv1
}

install_run_pcef_deps() {
	$SUDO apt-get -y install \
		libgnutls30 \
		libidn11
}

install_run_tdf_deps() {
	$SUDO apt-get -y install \
		libgnutls30 \
		libidn11
}

install_run_tssf_deps() {
	$SUDO apt-get -y install \
		libgnutls30 \
		libidn11
}

install_run_utils() {
	$SUDO apt-get -y install \
		dnsutils \
		iproute2 \
		iputils-ping \
		tcpdump
}

cleanup_image() {
	$SUDO rm -rf /var/lib/apt/lists/*
}

(return 2>/dev/null) && echo "Sourced" && return
