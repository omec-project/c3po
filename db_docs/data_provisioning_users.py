'''
 Copyright 2019-present Open Networking Foundation
 SPDX-License-Identifier: Apache-2.0
'''
#!/usr/bin/env python

#script requires cassandra driver for python.
#Execute following command to install driver.
# pip install cassandra-driver

import logging

log = logging.getLogger('test')
log.setLevel('ERROR')
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter("%(asctime)s [%(levelname)s] %(name)s: %(message)s"))
log.addHandler(handler)

from cassandra import ConsistencyLevel
from cassandra.cluster import Cluster
from cassandra.query import SimpleStatement

KEYSPACE = "vhss"
imsi = 208014567891234
msisdn = 1122334455
apn = 'apn1'
key = '465B5CE8B199B49FAA5F0A2EE238A6BC'
no_of_users = 8000
cassandra_ip = '192.168.112.60'


def main():
    global imsi
    global msisdn
    cluster = Cluster([cassandra_ip])
    session = cluster.connect()
	
    for i in range(no_of_users):
        query = "INSERT INTO vhss.users_imsi (imsi, msisdn, access_restriction, key, mmehost, mmeidentity_idmmeidentity, mmerealm, rand, sqn, subscription_data) VALUES ('" + str(imsi) + "'," + str(msisdn) + ", 41, '" + key + "', 'mme.localdomain', 3, 'localdomain', '2683b376d1056746de3b254012908e0e', 96, '{\"Subscription-Data\":{\"Access-Restriction-Data\":41,\"Subscriber-Status\":0,\"Network-Access-Mode\":2,\"Regional-Subscription-Zone-Code\":[\"0123\",\"4567\",\"89AB\",\"CDEF\",\"1234\",\"5678\",\"9ABC\",\"DEF0\",\"2345\",\"6789\"],\"MSISDN\":\"" + str(msisdn) + "\",\"AMBR\":{\"Max-Requested-Bandwidth-UL\":50000000,\"Max-Requested-Bandwidth-DL\":100000000},\"APN-Configuration-Profile\":{\"Context-Identifier\":0,\"All-APN-Configurations-Included-Indicator\":0,\"APN-Configuration\":{\"Context-Identifier\":0,\"PDN-Type\":0,\"Served-Party-IP-Address\":[\"10.0.0.1\",\"10.0.0.2\"],\"Service-Selection\":\"apn1\",\"EPS-Subscribed-QoS-Profile\":{\"QoS-Class-Identifier\":9,\"Allocation-Retention-Priority\":{\"Priority-Level\":15,\"Pre-emption-Capability\":0,\"Pre-emption-Vulnerability\":0}},\"AMBR\":{\"Max-Requested-Bandwidth-UL\":50000000,\"Max-Requested-Bandwidth-DL\":100000000},\"PDN-GW-Allocation-Type\":0,\"MIP6-Agent-Info\":{\"MIP-Home-Agent-Address\":[\"172.26.17.183\"]}}},\"Subscribed-Periodic-RAU-TAU-Timer\":0}}');"
        session.execute(query)

        query1 = "INSERT INTO vhss.msisdn_imsi (msisdn, imsi) VALUES (" + str(msisdn) + ", '" + str(imsi) + "');"
        session.execute(query1)
        imsi += 1
        msisdn += 1


if __name__ == "__main__":
    main()


