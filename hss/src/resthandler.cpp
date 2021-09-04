/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include "resthandler.h"
#include "rapidjson/error/en.h"
#include "fdhss.h"

#include "logger.h"
#include "sstats.h"

#include <pistache/endpoint.h>
#include "portal_config.h"

void RestHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response)
{
   static bool needConfig = true;
   if (request.resource() == "/v2/config-check") {
     if (needConfig == true) {
        response.send(Pistache::Http::Code::Not_Found);
        needConfig = false;
     } else {
        response.send(Pistache::Http::Code::Ok);
     }
   } else if (request.resource() == "/v2/config/imsis") {
      std::cout<<"v2 config imsis resource "<<std::endl;
      RAPIDJSON_NAMESPACE::Document doc;
      doc.Parse(request.body().c_str());
      if(doc.HasParseError())
      {
              std::stringstream ss;
              ss << "Body parsing error offset="
                      << doc.GetErrorOffset()
                      << " error=" << RAPIDJSON_NAMESPACE::GetParseError_En(doc.GetParseError());
              response.send(Pistache::Http::Code::Bad_Request, ss.str());
              return;
      }
      response.send(Pistache::Http::Code::Ok, "");
      newPortalConfig* config = parse_hss_json_doc(doc);
      assert(config != NULL);
      for (uint64_t imsi = config->from_imsi; imsi <= config->to_imsi; imsi++ )
      {
              ImsiEntity data;
              std::cout<<"\n\nIMSI "<<imsi<<std::endl;
              data.imsi = std::to_string(imsi); data.imsi_pres = true;
              data.opc = config->opc; data.opc_pres = true;
              data.key = config->key; data.key_pres = true;
              data.sqn = config->sqn; data.sqn_pres = true;
              data.rand = "2683b376d1056746de3b254012908e0e"; data.rand_pres = true;
              data.msisdn = 1122334456;    data.msisdn_pres = true;
              data.access_restriction = 41; data.access_restriction_pres = true;
              data.mmeidentity_idmmeidentity = 3; data.mmeidentity_idmmeidentity_pres = true;

              data.supported_features = "{\"Supported-Features\":{\"Vendor-Id\": 10415, \"Feature-List-ID\": 2, \"Feature-List\": 134217728}}";
              data.supported_features_pres = true;

              {
                RAPIDJSON_NAMESPACE::Document jsonDoc;
                jsonDoc.SetObject();
                RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

                RAPIDJSON_NAMESPACE::Value objValue;
                objValue.SetObject();
                objValue.AddMember("Access-Restriction-Data", 41, allocator);
                objValue.AddMember("Subscriber-Status", 0, allocator);
                objValue.AddMember("Network-Access-Mode", 2, allocator);
                objValue.AddMember("Subscribed-Periodic-RAU-TAU-Timer", 0, allocator);
                objValue.AddMember("MSISDN", 0x1122334458, allocator);

                {
                    RAPIDJSON_NAMESPACE::Value objAmbr;
                    objAmbr.SetObject();
                    objAmbr.AddMember("Max-Requested-Bandwidth-UL", config->apn_ambr_ul, allocator);
                    objAmbr.AddMember("Max-Requested-Bandwidth-DL", config->apn_ambr_dl, allocator);
                    objValue.AddMember("AMBR", objAmbr, allocator);
                }

                {
                    RAPIDJSON_NAMESPACE::Value zoneCode(RAPIDJSON_NAMESPACE::kArrayType);
                    zoneCode.PushBack("0x0123", allocator);
                    zoneCode.PushBack("0x4567", allocator);
                    zoneCode.PushBack("0x89AB", allocator);
                    zoneCode.PushBack("0xCDEF", allocator);
                    zoneCode.PushBack("0x1234", allocator);
                    zoneCode.PushBack("0x5678", allocator);
                    zoneCode.PushBack("0x9ABC", allocator);
                    zoneCode.PushBack("0xDEF0", allocator);
                    zoneCode.PushBack("0x2345", allocator);
                    zoneCode.PushBack("0x6789", allocator);
                    objValue.AddMember("Regional-Subscription-Zone-Code", zoneCode, allocator);
                }

                for (std::list<apn_profile*>::iterator it=config->apn_profile_list.begin(); it!=config->apn_profile_list.end(); ++it)
                {
                    apn_profile *apn_prof = *it;
                    RAPIDJSON_NAMESPACE::Value objApnProfile;
                    objApnProfile.SetObject();
                    objApnProfile.AddMember("Context-Identifier",0, allocator);
                    objApnProfile.AddMember("All-APN-Configurations-Included-Indicator",0, allocator);
                    {
                          RAPIDJSON_NAMESPACE::Value objApnConf;
                          objApnConf.SetObject();
                          objApnConf.AddMember("Context-Identifier",0, allocator);
                          objApnConf.AddMember("PDN-Type",0, allocator);
                          objApnConf.AddMember("Service-Selection", RAPIDJSON_NAMESPACE::Value(apn_prof->apn_name.c_str(), allocator), allocator);
                          objApnConf.AddMember("PDN-GW-AllocatorType", 0, allocator);

                          RAPIDJSON_NAMESPACE::Value staticIp(RAPIDJSON_NAMESPACE::kArrayType);
                          staticIp.PushBack("0.0.0.0", allocator);
                          objApnConf.AddMember("Served-Party-IP-Address", staticIp, allocator);

                          // AMBR
                          {
                              RAPIDJSON_NAMESPACE::Value objAmbr;
                              objAmbr.SetObject();
                              objAmbr.AddMember("Max-Requested-Bandwidth-UL", config->apn_ambr_ul, allocator);
                              objAmbr.AddMember("Max-Requested-Bandwidth-DL", config->apn_ambr_dl, allocator);
                              objApnConf.AddMember("AMBR", objAmbr, allocator);
                          }

                          //EPS QoS
                          {
                              RAPIDJSON_NAMESPACE::Value objBQos;
                              objBQos.SetObject();
                              objBQos.AddMember("QoS-Class-Identifier", config->qci, allocator);
                              {
                                  RAPIDJSON_NAMESPACE::Value objArp;
                                  objArp.SetObject();
                                  objArp.AddMember("Priority-Level", 1, allocator);
                                  objArp.AddMember("Pre-emption-Capability", 1, allocator);
                                  objArp.AddMember("Pre-emption-Vulnerability", 1, allocator);
                                  objBQos.AddMember("Allocation-Retention-Priority", objArp, allocator);
                              }
                              objApnConf.AddMember("EPS-Subscribed-QoS-Profile", objBQos, allocator);
                          }

                          //MIP
                          {
                              RAPIDJSON_NAMESPACE::Value objMip;
                              objMip.SetObject();
                              RAPIDJSON_NAMESPACE::Value addr(RAPIDJSON_NAMESPACE::kArrayType);
                              addr.PushBack("0.0.0.0", allocator);
                              objMip.AddMember("MIP-Home-Agent-Address", addr, allocator);
                              objApnConf.AddMember("MIP6-Agent-Info", objMip, allocator);
                          }

                          objApnProfile.AddMember("APN-Configuration", objApnConf, allocator);
                    }
                    objValue.AddMember("APN-Configuration-Profile", objApnProfile, allocator);
                }

                jsonDoc.AddMember("Subscription-Data", objValue, allocator);
                RAPIDJSON_NAMESPACE::StringBuffer strbuf;
                RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(strbuf);
                jsonDoc.Accept(writer);
                const char *jsonString = strbuf.GetString();
                std::cout<<"jsonString "<<jsonString<<std::endl;
                data.subscription_data = std::string(jsonString);
                data.subscription_data_pres = true;
              }
              fdHss.poppulate_IMSIs(data);
      }
      delete config;
      return;
   } else if (request.resource() == "/v1/config/imsis") {
      std::cout<<"hitting config imsis resource "<<std::endl;
      RAPIDJSON_NAMESPACE::Document doc;
      doc.Parse(request.body().c_str());
      if(doc.HasParseError())
      {
              std::stringstream ss;
              ss << "Body parsing error offset="
                      << doc.GetErrorOffset()
                      << " error=" << RAPIDJSON_NAMESPACE::GetParseError_En(doc.GetParseError());
              response.send(Pistache::Http::Code::Bad_Request, ss.str());
              return;
      }
      response.send(Pistache::Http::Code::Ok, "");
      portalConfig* config = parse_json_doc(doc);
      assert(config != NULL);
      for (std::list<sub_selection_rule *> ::iterator it=config->sub_sel_rules.begin(); it != config->sub_sel_rules.end(); ++it)
      {
          sub_selection_rule *rule = *it;
          sub_selection_keys *keys = rule->keys;
          if(keys->imsi.is_valid == false) {
             continue;
          }
          security_profile *sec_prof = config->get_security_profile(rule->selected_security_profile);
          if(sec_prof == NULL) {
            continue;
          }

          qos_profile *qos_prof = config->get_qos_profile(rule->selected_qos_profile);
          if(qos_prof == NULL) {
            continue;
          }

          apn_profile *apn_prof = config->get_apn_profile(rule->selected_apn_profile);
          if(apn_prof == NULL) {
            continue;
          }

          for(uint64_t imsi = keys->imsi.from_imsi; imsi <= keys->imsi.to_imsi; imsi++)
          {
              ImsiEntity data;
              std::cout<<"\n\nIMSI "<<imsi<<std::endl;
              data.imsi = std::to_string(imsi); data.imsi_pres = true;
              data.opc = sec_prof->opc; data.opc_pres = true;
              data.key = sec_prof->key; data.key_pres = true;
              data.sqn = sec_prof->sqn; data.sqn_pres = true;
              data.rand = "2683b376d1056746de3b254012908e0e"; data.rand_pres = true;
              data.msisdn = 1122334456;    data.msisdn_pres = true;
              data.access_restriction = 41; data.access_restriction_pres = true;
              data.mmeidentity_idmmeidentity = 3; data.mmeidentity_idmmeidentity_pres = true;

              data.supported_features = "{\"Supported-Features\":{\"Vendor-Id\": 10415, \"Feature-List-ID\": 2, \"Feature-List\": 134217728}}";
              data.supported_features_pres = true;

              {
                RAPIDJSON_NAMESPACE::Document jsonDoc;
                jsonDoc.SetObject();
                RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

                RAPIDJSON_NAMESPACE::Value objValue;
                objValue.SetObject();
                objValue.AddMember("Access-Restriction-Data", 41, allocator);
                objValue.AddMember("Subscriber-Status", 0, allocator);
                objValue.AddMember("Network-Access-Mode", 2, allocator);
                objValue.AddMember("Subscribed-Periodic-RAU-TAU-Timer", 0, allocator);
                objValue.AddMember("MSISDN", 0x1122334458, allocator);

                {
                    RAPIDJSON_NAMESPACE::Value objAmbr;
                    objAmbr.SetObject();
                    objAmbr.AddMember("Max-Requested-Bandwidth-UL", qos_prof->apn_ambr_ul, allocator);
                    objAmbr.AddMember("Max-Requested-Bandwidth-DL", qos_prof->apn_ambr_dl, allocator);
                    objValue.AddMember("AMBR", objAmbr, allocator);
                }

                {
                    RAPIDJSON_NAMESPACE::Value zoneCode(RAPIDJSON_NAMESPACE::kArrayType);
                    zoneCode.PushBack("0x0123", allocator);
                    zoneCode.PushBack("0x4567", allocator);
                    zoneCode.PushBack("0x89AB", allocator);
                    zoneCode.PushBack("0xCDEF", allocator);
                    zoneCode.PushBack("0x1234", allocator);
                    zoneCode.PushBack("0x5678", allocator);
                    zoneCode.PushBack("0x9ABC", allocator);
                    zoneCode.PushBack("0xDEF0", allocator);
                    zoneCode.PushBack("0x2345", allocator);
                    zoneCode.PushBack("0x6789", allocator);
                    objValue.AddMember("Regional-Subscription-Zone-Code", zoneCode, allocator);
                }

                {
                    RAPIDJSON_NAMESPACE::Value objApnProfile;
                    objApnProfile.SetObject();
                    objApnProfile.AddMember("Context-Identifier",0, allocator);
                    objApnProfile.AddMember("All-APN-Configurations-Included-Indicator",0, allocator);
                    {
                          RAPIDJSON_NAMESPACE::Value objApnConf;
                          objApnConf.SetObject();
                          objApnConf.AddMember("Context-Identifier",0, allocator);
                          objApnConf.AddMember("PDN-Type",0, allocator);
                          objApnConf.AddMember("Service-Selection", RAPIDJSON_NAMESPACE::Value(apn_prof->apn_name.c_str(), allocator), allocator);
                          objApnConf.AddMember("PDN-GW-AllocatorType", 0, allocator);

                          RAPIDJSON_NAMESPACE::Value staticIp(RAPIDJSON_NAMESPACE::kArrayType);
                          staticIp.PushBack("0.0.0.0", allocator);
                          objApnConf.AddMember("Served-Party-IP-Address", staticIp, allocator);

                          // AMBR
                          {
                              RAPIDJSON_NAMESPACE::Value objAmbr;
                              objAmbr.SetObject();
                              objAmbr.AddMember("Max-Requested-Bandwidth-UL", qos_prof->apn_ambr_ul, allocator);
                              objAmbr.AddMember("Max-Requested-Bandwidth-DL", qos_prof->apn_ambr_dl, allocator);
                              objApnConf.AddMember("AMBR", objAmbr, allocator);
                          }

                          //EPS QoS
                          {
                              RAPIDJSON_NAMESPACE::Value objBQos;
                              objBQos.SetObject();
                              objBQos.AddMember("QoS-Class-Identifier", qos_prof->qci, allocator);
                              {
                                  RAPIDJSON_NAMESPACE::Value objArp;
                                  objArp.SetObject();
                                  objArp.AddMember("Priority-Level", qos_prof->priority, allocator);
                                  objArp.AddMember("Pre-emption-Capability", qos_prof->pre_empt_capability, allocator);
                                  objArp.AddMember("Pre-emption-Vulnerability", qos_prof->pre_empt_vulnerability, allocator);
                                  objBQos.AddMember("Allocation-Retention-Priority", objArp, allocator);
                              }
                              objApnConf.AddMember("EPS-Subscribed-QoS-Profile", objBQos, allocator);
                          }

                          //MIP
                          {
                              RAPIDJSON_NAMESPACE::Value objMip;
                              objMip.SetObject();
                              RAPIDJSON_NAMESPACE::Value addr(RAPIDJSON_NAMESPACE::kArrayType);
                              addr.PushBack("0.0.0.0", allocator);
                              objMip.AddMember("MIP-Home-Agent-Address", addr, allocator);
                              objApnConf.AddMember("MIP6-Agent-Info", objMip, allocator);
                          }

                          objApnProfile.AddMember("APN-Configuration", objApnConf, allocator);
                    }
                    objValue.AddMember("APN-Configuration-Profile", objApnProfile, allocator);
                }

                jsonDoc.AddMember("Subscription-Data", objValue, allocator);
                RAPIDJSON_NAMESPACE::StringBuffer strbuf;
                RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(strbuf);
                jsonDoc.Accept(writer);
                const char *jsonString = strbuf.GetString();
                std::cout<<"jsonString "<<jsonString<<std::endl;
                data.subscription_data = std::string(jsonString);
                data.subscription_data_pres = true;
              }
              fdHss.poppulate_IMSIs(data);
          }
      }
      delete config;
      return;
   } else if (request.resource() == "/imsis") {
      ImsiImeiData data;
      RAPIDJSON_NAMESPACE::Document doc;
      std::string keyschema("userschema");

      doc.Parse(request.body().c_str());
      if(doc.HasParseError())
      {
         std::stringstream ss;
         ss << "Body parsing error offset="
            << doc.GetErrorOffset()
            << " error=" << RAPIDJSON_NAMESPACE::GetParseError_En(doc.GetParseError());
         response.send(Pistache::Http::Code::Bad_Request, ss.str());
         return;
      }

      data.imsi = doc["imsi"].GetString();
      data.msisdn = doc["msisdn"].GetInt64();
      data.old_imei = doc["prev_imei"].GetString();
      data.old_imei_sv = doc["prev_imei_sv"].GetString();
      data.new_imei = doc["new_imei"].GetString();
      data.new_imei_sv = doc["new_imei_sv"].GetString();

      response.send(Pistache::Http::Code::Ok, "");

      if ((data.old_imei == data.new_imei) && (data.old_imei_sv == data.new_imei_sv))
         return;

      fdHss.sendRIR_ChangeImsiImeiSvAssn(data);
   } else {
      std::cout << request.resource() << std::endl;
      std::cout << request.method() << std::endl;
      std::cout << request.body() << std::endl;
      std::stringstream ss;
      ss << "Unrecognized resource [" << request.resource() << "]";
      response.send(Pistache::Http::Code::Bad_Request, ss.str() );
   }
}

