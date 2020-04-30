#! /bin/bash
# Copyright 2019-present Open Networking Foundation

#Copyright (c) 2017 Sprint
#
# SPDX-License-Identifier: Apache-2.0

if [ $# -ne 2 ]
then
   echo usage: make_certs.sh diameter_host_name diameter_realm_name
   exit
fi

rm -rf demoCA
mkdir demoCA
echo 01 > demoCA/serial
touch demoCA/index.txt

HOST=$1
DOMAIN=$2

# CA self certificate
openssl req  -new -batch -x509 -days 3650 -nodes -newkey rsa:1024 -out cacert.pem -keyout cakey.pem -subj /CN=ca.localdomain/C=FR/ST=BdR/L=Aix/O=fD/OU=Tests

#
openssl genrsa -out $HOST.key.pem 1024
openssl req -new -batch -out $HOST.csr.pem -key $HOST.key.pem -subj /CN=$HOST.$DOMAIN/C=FR/ST=BdR/L=Aix/O=fD/OU=Tests
openssl ca -cert cacert.pem -keyfile cakey.pem -in $HOST.csr.pem -out $HOST.cert.pem -outdir . -batch

