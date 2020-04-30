#! /bin/bash
# Copyright 2019-present Open Networking Foundation

#Copyright (c) 2017 Sprint
#
# SPDX-License-Identifier: Apache-2.0

### Add an entry to the mme_identity table ######

id=$1
isdn=$2
host=$3
realm=$4
uereachability=$5
cassandra_ip=$6
if [ "$*" == "" -o $# -ne 6 ] ; then
   echo -e "You must provide all of the arguments to the script\n"
   echo -e "$0 <id> <isdn> <host> <realm> <uereachability> <cassandra_ip>\n"
   exit
fi

cqlsh $cassandra_ip -e "INSERT INTO vhss.mmeidentity (idmmeidentity, mmeisdn, mmehost, mmerealm, ue_reachability) VALUES ($id, '$isdn', '$host', '$realm', $uereachability);"
if [ $? -ne 0 ];then
   echo -e "Oops! Something went wrong adding to vhss.mmeidentity!\n"
   exit
fi

cqlsh $cassandra_ip -e "INSERT INTO vhss.mmeidentity_host (idmmeidentity, mmeisdn, mmehost, mmerealm, ue_reachability) VALUES ($id, '$isdn', '$host', '$realm', $uereachability);"
if [ $? -ne 0 ];then
   echo -e "Oops! Something went wrong adding to vhss.mmeidentity_host!\n"
   exit
fi

echo -e "The mmeidentity provisioning is successfull\n"
