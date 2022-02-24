'''
 Copyright (c) 2021  Great Software Laboratory Pvt. Ltd
 SPDX-License-Identifier: Apache-2.0
'''


#!/usr/bin/env python

#script requires redis driver for python.
#Execute following command to install driver.
# pip install redis
#Execute below command to run the script.
#python3 redis_data_provisioning.py

import redis


imsi = 208014567891234
msisdn = 1122334455
apn = 'apn1'
key = '465B5CE8B199B49FAA5F0A2EE238A6BC'
no_of_users = 10000


def main():
    global imsi
    global msisdn


    redisClient = redis.StrictRedis(host='127.0.0.1',
                                    port=6379,
                                    ssl=False,
                                    ssl_certfile='/root/tests/tls/redis.crt',
                                    ssl_keyfile='/root/tests/tls/redis.key',
                                    ssl_ca_certs='/root/tests/tls/ca.crt' ,
                                    )


    print("****************************** Starting **************************************")


    for i in range(no_of_users):
        string_imsi = "vhss:users_imsi:"+ str(imsi)
        redisClient.hmset(string_imsi , {
	    			"imsi": imsi,
	    			"msisdn": msisdn,
	    			"access_restriction": 41,
	    			"key": key,
	    			"lipa_permissions": "LIPA_ALLOWED",
	    			"mme_cap": 4456,
	    			"ms_ps_status": "ACTIVE",
	    			"niddvalidity": "2055",
	    			"nir_dest_host": "nir.localdomain",
	    			"nir_dest_realm": "localdomain",
	    			"opc": "hss-33522",
	    			"pgw_id": 192168076,
	    			"rfsp_index": "hss-22533",
	    			"ue_reachability": 341,
	    			"urrp_mme": 221,
	    			"user_identifier": "user.localdomain.hss",
	    			"visited_plmnid":1555261,
	                        "mmehost": "mme.localdomain",
	                        "mmeidentity_idmmeidentity" : 3,
	                        "mmerealm": "localdomain",
	                        "rand": "2683b376d1056746de3b254012908e0e",
	                        "sqn": 96,
	                        "imei": "35-209900-176148-1",
	                        "imei_sv": "35-209900-176148-23",
	                        "subscription_data": "{\"Subscription-Data\":{\"Access-Restriction-Data\":41,\"Subscriber-Status\":0,\"Network-Access-Mode\":2,\"Regional-Subscription-Zone-Code\":[\"0123\",\"4567\",\"89AB\",\"CDEF\",\"1234\",\"5678\",\"9ABC\",\"DEF0\",\"2345\",\"6789\"],\"MSISDN\":\"" + str(msisdn) + "\",\"AMBR\":{\"Max-Requested-Bandwidth-UL\":50000000,\"Max-Requested-Bandwidth-DL\":100000000},\"APN-Configuration-Profile\":{\"Context-Identifier\":0,\"All-APN-Configurations-Included-Indicator\":0,\"APN-Configuration\":{\"Context-Identifier\":0,\"PDN-Type\":0,\"Served-Party-IP-Address\":[\"10.0.0.1\",\"10.0.0.2\"],\"Service-Selection\":\"apn1\",\"EPS-Subscribed-QoS-Profile\":{\"QoS-Class-Identifier\":9,\"Allocation-Retention-Priority\":{\"Priority-Level\":15,\"Pre-emption-Capability\":0,\"Pre-emption-Vulnerability\":0}},\"AMBR\":{\"Max-Requested-Bandwidth-UL\":50000000,\"Max-Requested-Bandwidth-DL\":100000000},\"PDN-GW-Allocation-Type\":0,\"MIP6-Agent-Info\":{\"MIP-Home-Agent-Address\":[\"172.26.17.183\"]}}},\"Subscribed-Periodic-RAU-TAU-Timer\":0}}"
                })

        imsi += 1
        msisdn += 1

if __name__ == "__main__":
    main()





