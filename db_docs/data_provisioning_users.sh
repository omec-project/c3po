#! /bin/bash

#Copyright 2019-present Open Networking Foundation
#Copyright (c) 2017 Sprint
#
#SPDX-License-Identifier: Apache-2.0

### Delete all rows in the test table ######

#cqlsh 10.78.137.141 -e "TRUNCATE vhss.test_users ;"

imsi=$1
msisdn=$2
apn=$3
key=$4
no_of_users=$5
cassandra_ip=$6
opc=$7
mmeidentity=${8:-'mme.localdomain'}
mmerealm=${9:-'localdomain'}
if [ "$*" == "" -o $# -lt 7 ] ; then
   echo -e "You must provide all of the required arguments to the script\n"
   echo -e "$0 <imsi> <msisdn> <apn> <key> <no_of_users> <cassandra_ip> <opc> [<mmeidentity> <mmerealm>]\n"
   exit
fi

for (( i=1; i<=$no_of_users; i++ ))
do
	echo "IMSI=$imsi MSISDN=$msisdn"

	cqlsh $cassandra_ip -e "INSERT INTO vhss.users_imsi (imsi, msisdn, access_restriction, key, opc, mmehost, mmeidentity_idmmeidentity, mmerealm, rand, sqn, subscription_data) VALUES ('$imsi', $msisdn, 41, '$key', '$opc', '$mmeidentity', 3, '$mmerealm', '2683b376d1056746de3b254012908e0e', 96, '{\"Subscription-Data\":{\"Access-Restriction-Data\":41,\"Subscriber-Status\":0,\"Network-Access-Mode\":2,\"Regional-Subscription-Zone-Code\":[\"0x0123\",\"0x4567\",\"0x89AB\",\"0xCDEF\",\"0x1234\",\"0x5678\",\"0x9ABC\",\"0xDEF0\",\"0x2345\",\"0x6789\"],\"MSISDN\":\"0x$msisdn\",\"AMBR\":{\"Max-Requested-Bandwidth-UL\":50000000,\"Max-Requested-Bandwidth-DL\":100000000},\"APN-Configuration-Profile\":{\"Context-Identifier\":0,\"All-APN-Configurations-Included-Indicator\":0,\"APN-Configuration\":{\"Context-Identifier\":0,\"PDN-Type\":0,\"Served-Party-IP-Address\":[\"0.0.0.0\",\"0.0.0.0\"],\"Service-Selection\":\"$apn\",\"EPS-Subscribed-QoS-Profile\":{\"QoS-Class-Identifier\":9,\"Allocation-Retention-Priority\":{\"Priority-Level\":15,\"Pre-emption-Capability\":0,\"Pre-emption-Vulnerability\":0}},\"AMBR\":{\"Max-Requested-Bandwidth-UL\":50000000,\"Max-Requested-Bandwidth-DL\":100000000},\"PDN-GW-Allocation-Type\":0,\"MIP6-Agent-Info\":{\"MIP-Home-Agent-Address\":[\"172.26.17.183\"]}}},\"Subscribed-Periodic-RAU-TAU-Timer\":0}}');"
	if [ $? -ne 0 ];then
	   echo -e "Oops! Something went wrong adding to vhss.users_imsi!\n"
	   exit
	fi

	cqlsh $cassandra_ip -e "INSERT INTO vhss.msisdn_imsi (msisdn, imsi) VALUES ($msisdn, '$imsi');"
	if [ $? -ne 0 ];then
	   echo -e "Oops! Something went wrong adding to vhss.msisdn_imsi!\n"
	   exit
	fi
	
	imsi=`expr $imsi + 1`;
	msisdn=`expr $msisdn + 1`
done

echo -e "The provisioning is successful\n"
