#! /bin/bash
# Copyright 2019-present Open Networking Foundation

#Copyright (c) 2017 Sprint
#
# SPDX-License-Identifier: Apache-2.0

### Delete all rows in the test table ######

#cqlsh 10.78.137.141 -e "TRUNCATE vhss.test_users ;"

imsi=$1
apn=$2
key=$3
opc=$4
no_of_users=$5
cassandra_ip=$6
if [ "$*" == "" -o $# -ne 6 ] ; then
   echo -e "You must provide all of the arguments to the script\n"
   echo -e "$0 <imsi_value> <apn_value> <key_value> <opc_value> <no_of_users> <cassandra_ip>\n"
   exit
fi


for (( i=1; i<=$no_of_users; i++ ))
do

cqlsh $cassandra_ip -e "INSERT INTO vhss.users_imsi (imsi, access_restriction, key, mmehost, mmeidentity_idmmeidentity, mmerealm, opc, rand, sqn, subscription_data) VALUES ('$imsi', 41, '$key', 'mme.localdomain', 3, 'localdomain', '$opc', '2683b376d1056746de3b254012908e0e', 96, '{\"Subscription-Data\":{\"Access-Restriction-Data\":41,\"Subscriber-Status\":0,\"Network-Access-Mode\":2,\"Regional-Subscription-Zone-Code\":[\"0123\",\"4567\",\"89AB\",\"CDEF\",\"1234\",\"5678\",\"9ABC\",\"DEF0\",\"2345\",\"6789\"],\"MSISDN\":\"1122334455\",\"AMBR\":{\"Max-Requested-Bandwidth-UL\":50000000,\"Max-Requested-Bandwidth-DL\":100000000},\"APN-Configuration-Profile\":{\"Context-Identifier\":0,\"All-APN-Configurations-Included-Indicator\":0,\"APN-Configuration\":{\"Context-Identifier\":0,\"PDN-Type\":0,\"Served-Party-IP-Address\":[\"10.0.0.1\",\"10.0.0.2\"],\"Service-Selection\":\"$apn\",\"EPS-Subscribed-QoS-Profile\":{\"QoS-Class-Identifier\":9,\"Allocation-Retention-Priority\":{\"Priority-Level\":15,\"Pre-emption-Capability\":0,\"Pre-emption-Vulnerability\":0}},\"AMBR\":{\"Max-Requested-Bandwidth-UL\":50000000,\"Max-Requested-Bandwidth-DL\":100000000},\"PDN-GW-Allocation-Type\":0,\"MIP6-Agent-Info\":{\"MIP-Home-Agent-Address\":[\"172.26.17.183\"]}}},\"Subscribed-Periodic-RAU-TAU-Timer\":0}}');"
if [ $? -ne 0 ];then
   echo -e "Oops! Something went wrong!\n"
   exit
fi
imsi=`expr $imsi + 1`;
printf -v imsi "%015d" $imsi
#echo $imsi;
done

echo -e "The provisioning is successfull\n"
