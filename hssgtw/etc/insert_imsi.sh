#! /bin/bash
# Copyright 2019-present Open Networking Foundation

#Copyright (c) 2017 Sprint
#
# SPDX-License-Identifier: Apache-2.0

curl -d '{"imsi":"string","access_restriction":0,"idmmeidentity":0,"imei":"string","imei_sv":"string","key":"string","lipa_permissions":"string","mme_cap":0,"mmehost":"string","mmeidentity_idmmeidentity":0,"mmerealm":"string","ms_ps_status":"string","msisdn":0,"niddvalidity":"string","nir_dest_host":"string","nir_dest_realm":"string","opc":"string","pgw_id":0,"rand":"string","rfsp_index":0,"sqn":0,"subscription_data":"string","ue_reachability":0,"urrp_mme":0,"user_identifier":"string","visited_plmnid":"string"}' http://localhost:9081/imsis
