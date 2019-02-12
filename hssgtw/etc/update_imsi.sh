#! /bin/bash

#Copyright (c) 2017 Sprint
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

curl -X PUT -d '{"imsi":"string","access_restriction":10,"idmmeidentity":0,"imei":"string","imei_sv":"string_up","key":"string","lipa_permissions":"string","mme_cap":0,"mmehost":"string","mmeidentity_idmmeidentity":0,"mmerealm":"string","ms_ps_status":"string","msisdn":0,"niddvalidity":"string","nir_dest_host":"string","nir_dest_realm":"string","opc":"string","pgw_id":0,"rand":"string","rfsp_index":0,"sqn":0,"subscription_data":"string","ue_reachability":0,"urrp_mme":0,"user_identifier":"string","visited_plmnid":"string"}' http://localhost:9081/imsis
