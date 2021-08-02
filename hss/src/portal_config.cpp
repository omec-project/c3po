// Copyright 2020-present Open Networking Foundation
//
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "portal_config.h"


bool 
local_compare_sub_rules(const sub_selection_rule *rule1, const sub_selection_rule *rule2)
{
    if(rule1->rule_priority < rule2->rule_priority)
        return true;
    return false;
}

portalConfig *
parse_json_doc(RAPIDJSON_NAMESPACE::Document &doc)
{
    portalConfig *config = new (portalConfig);

    if(doc.HasMember("subscriber-selection-rules"))
    {
        for(uint32_t i=0; i< doc["subscriber-selection-rules"].Size();i++) 
        {
            sub_selection_rule *sub_rule = new (sub_selection_rule);
            std::cout<<"\tSubscriber selection rule "<<i<<std::endl;
            sub_rule->keys = nullptr;
            const RAPIDJSON_NAMESPACE::Value& subRuleSection = doc["subscriber-selection-rules"][i];
            if(subRuleSection.HasMember("priority"))
            {
                sub_rule->rule_priority = subRuleSection["priority"].GetInt();
                std::cout<<"\t\tkeys has priority "<<sub_rule->rule_priority<<std::endl;
            }
            if(subRuleSection.HasMember("keys"))
            {
                sub_selection_keys *key = new (sub_selection_keys); 
                key->imsi.is_valid = false;
                key->plmn.is_valid = false;
                key->apn.is_valid = false;
                sub_rule->keys = key;
                const RAPIDJSON_NAMESPACE::Value& ruleKeys = subRuleSection["keys"];
                if(ruleKeys.HasMember("imsi-range") && ruleKeys["imsi-range"].IsObject())
                {
                    std::cout<<"\t\t\tkeys has imsi-range Object "<<std::endl;
                    const RAPIDJSON_NAMESPACE::Value& imsiKeys = ruleKeys["imsi-range"];
                    if(imsiKeys.HasMember("from"))
                    {
                        std::cout<<"\t\t\t\tIMSI range from present "<<imsiKeys["from"].GetInt64()<<std::endl;
                    }
                    if(imsiKeys.HasMember("to"))
                    {
                        std::cout<<"\t\t\t\tIMSI range to present "<<imsiKeys["to"].GetInt64()<<std::endl;
                    }
                    key->imsi.is_valid = true;
                    key->imsi.from_imsi= imsiKeys["from"].GetInt64(); 
                    key->imsi.to_imsi= imsiKeys["to"].GetInt64(); 
                }
                if(ruleKeys.HasMember("serving-plmn") && ruleKeys["serving-plmn"].IsObject())
                {
                    std::cout<<"\t\t\tkeys has serving-plmn Object "<<std::endl;
                    const RAPIDJSON_NAMESPACE::Value& plmnKeys = ruleKeys["serving-plmn"];
                    uint16_t mcc, mnc;
                    if(plmnKeys.HasMember("mcc"))
                    {
                        mcc = plmnKeys["mcc"].GetInt();
                        std::cout<<"\t\t\t\tmcc "<<mcc<<std::endl;
                    }
                    if(plmnKeys.HasMember("mnc"))
                    {
                        mnc = plmnKeys["mnc"].GetInt();
                        std::cout<<"\t\t\t\tmnc "<<mnc<<std::endl;
                    }
                    if(plmnKeys.HasMember("tac"))
                    {
                        std::cout<<"\t\t\t\ttac "<<plmnKeys["tac"].GetInt()<<std::endl;
                    }
                    key->plmn.is_valid = true;
                    uint16_t mcc_dig_1, mcc_dig_2, mcc_dig_3;
                    uint16_t mnc_dig_1, mnc_dig_2, mnc_dig_3;
                    mcc_dig_1 = mcc/100; mcc = mcc % 100;
                    mcc_dig_2 = mcc/10; mcc = mcc %10;
                    mcc_dig_3 = mcc;
                    if(mnc > 99) 
                    {
                        mnc_dig_1 = (mnc/100);
                        mnc_dig_2 = (mnc%100)/10;
                        mnc_dig_3 = mnc % 10;
                    }
                    else
                    {
                        mnc_dig_3 = 0xf; 
                        mnc_dig_1 = mnc/10;
                        mnc_dig_2 = mnc % 10;
                    }

                    key->plmn.plmn[0] = (mcc_dig_2 << 4) | (mcc_dig_1); 
                    key->plmn.plmn[1] = (mnc_dig_1 << 4) | (mcc_dig_3);
                    key->plmn.plmn[2] = (mnc_dig_3 << 4) | (mnc_dig_2);
                    key->plmn.tac = plmnKeys["tac"].GetInt();
                }
                if(ruleKeys.HasMember("requested-apn"))
                {
                    key->apn.requested_apn = ruleKeys["requested-apn"].GetString();
                    std::cout<<"\t\t\tkeys has requested-apn "<<key->apn.requested_apn<<std::endl;
                    key->apn.is_valid = true;
                }
            }
            std::cout<<"\t\tSelected Profiles"<<std::endl;
            if(subRuleSection.HasMember("selected-apn-profile"))
            {
                sub_rule->selected_apn_profile = subRuleSection["selected-apn-profile"].GetString();
                std::cout<<"\t\t\tselected-apn-profile found - "<<sub_rule->selected_apn_profile<<std::endl;
            }
            if(subRuleSection.HasMember("selected-user-plane-profile"))
            {
                sub_rule->selected_user_plane_profile = subRuleSection["selected-user-plane-profile"].GetString();
                std::cout<<"\t\t\tselected-user-plane-profile found "<<sub_rule->selected_user_plane_profile<<std::endl;
            }
            if(subRuleSection.HasMember("selected-qos-profile"))
            {
                sub_rule->selected_qos_profile = subRuleSection["selected-qos-profile"].GetString();
                std::cout<<"\t\t\tselected-qos-profile found - "<<sub_rule->selected_qos_profile<<std::endl;
            }
            if(subRuleSection.HasMember("selected-security-profile"))
            {
                sub_rule->selected_security_profile = subRuleSection["selected-security-profile"].GetString();
                std::cout<<"\t\t\tselected-security-profile found - "<<sub_rule->selected_security_profile<<std::endl;
            }
            if(subRuleSection.HasMember("selected-access-profile"))
            {
                for(uint32_t acc = 0; acc<subRuleSection["selected-access-profile"].Size() && (acc < 4) ; acc++)
                {
                    const RAPIDJSON_NAMESPACE::Value& accessProfile= subRuleSection["selected-access-profile"][acc];
                    sub_rule->selected_access_profile[acc] = accessProfile.GetString(); 
                    std::cout<<"\t\t\tselected-access-profile - "<<sub_rule->selected_access_profile[acc]<<std::endl;
                }
            }            
            config->sub_sel_rules.push_back(sub_rule);
        }
        config->sub_sel_rules.sort(local_compare_sub_rules);
    }
    if(doc.HasMember("apn-profiles"))
    {
        const RAPIDJSON_NAMESPACE::Value& apnProfileSection = doc["apn-profiles"];
        for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator itr = apnProfileSection.MemberBegin(); itr != apnProfileSection.MemberEnd(); ++itr)
        {
            std::string key = itr->name.GetString();
            if(itr->value.IsObject())
            {
                std::cout<<"\tAPN profile "<< key.c_str()<<" is Object"<<std::endl; 
                apn_profile *apn_prof = new (apn_profile);
                apn_prof->apn_profile_name = key;
                const RAPIDJSON_NAMESPACE::Value& apnSection = itr->value; 
                if(apnSection.HasMember("apn-name")) {
                    apn_prof->apn_name = apnSection["apn-name"].GetString();
                    std::cout<<"\t\tAPN name["<<apn_prof->apn_name<<"]"<<std::endl;
                    apn_prof->apn_name_length = apn_prof->apn_name.length();
                    std::cout<<"\t\tAPN name after encode ["<<apn_prof->apn_name<<"]"<<std::endl;
                }
                if(apnSection.HasMember("usage")) {
                    int usage = apnSection["usage"].GetInt();
                    std::cout<<"\t\tUsage type  "<<usage<<std::endl;
                    apn_prof->apn_usage_type = usage;
                }
                if(apnSection.HasMember("network")) {
                    apn_prof->apn_net_cap = apnSection["network"].GetString();
                    std::cout<<"\t\tNetwork type "<<apn_prof->apn_net_cap<<std::endl;
                }
                if(apnSection.HasMember("gx_enabled")) {
                    bool gx_enabled = apnSection["gx_enabled"].GetBool();
                    std::cout<<"\t\tGx enabled "<<gx_enabled<<std::endl;
                    apn_prof->gx_enabled = gx_enabled;
                }
                if(apnSection.HasMember("mtu")) {
                    uint16_t mtu = apnSection["mtu"].GetInt();
                    std::cout<<"\t\tAPN has mtu "<<mtu<<std::endl;
                    apn_prof->mtu = mtu;
                }
                if(apnSection.HasMember("dns_primary")) {
                    const char *temp = apnSection["dns_primary"].GetString();
                    std::cout<<"\t\tAPN has dns_primary "<<temp<<std::endl;
                    struct in_addr temp_i;
                    inet_aton(temp, &temp_i);
                    apn_prof->dns_primary = temp_i.s_addr;
                }
                if(apnSection.HasMember("dns_secondary")) {
                    const char *temp = apnSection["dns_secondary"].GetString();
                    std::cout<<"\t\tAPN has dns_secondary "<<temp<<std::endl;
                    struct in_addr temp_i;
                    inet_aton(temp, &temp_i);
                    apn_prof->dns_secondary = temp_i.s_addr;
                }
                config->apn_profile_list.push_back(apn_prof);
                std::cout<<std::endl;
            }
        }
    }
     
    if(doc.HasMember("user-plane-profiles"))
    {
        const RAPIDJSON_NAMESPACE::Value& userProfileSection = doc["user-plane-profiles"];
        for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator itr = userProfileSection.MemberBegin(); itr != userProfileSection.MemberEnd(); ++itr)
        {
            std::string key = itr->name.GetString();
            user_plane_profile *user_plane = new (user_plane_profile);
            user_plane->upf_addr = 0;
            const RAPIDJSON_NAMESPACE::Value& userPlaneSection = itr->value; 
            user_plane->user_plane_profile_name = key;
            if(userPlaneSection.HasMember("user-plane"))
            {
                user_plane->user_plane_service = userPlaneSection["user-plane"].GetString();
                std::cout<<"\tUser Plane - "<<user_plane->user_plane_service<<std::endl;
            }
            if(userPlaneSection.HasMember("qos-tags"))
            {
                std::cout<<"\t\tQoS Tags specified "<<std::endl;
            }
            if(userPlaneSection.HasMember("access-tags"))
            {
                std::cout<<"\t\tAccess Tags specified "<<std::endl;
            }
            config->user_plane_list.push_back(user_plane);
        }
    }
    if(doc.HasMember("qos-profiles"))
    {
        const RAPIDJSON_NAMESPACE::Value& qosProfileSection = doc["qos-profiles"];
        for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator itr = qosProfileSection.MemberBegin(); itr != qosProfileSection.MemberEnd(); ++itr)
        {
            qos_profile *qos_prof = new (qos_profile);
            std::string key = itr->name.GetString();
            qos_prof->qos_profile_name = key;
            std::cout<<"\tQoS profile - "<<key<<std::endl;
            const RAPIDJSON_NAMESPACE::Value& qosPlaneSection = itr->value; 
            qos_prof->apn_ambr_ul = qosPlaneSection["apn-ambr"][0].GetInt64();
            qos_prof->apn_ambr_dl = qosPlaneSection["apn-ambr"][1].GetInt64();
            qos_prof->qci = qosPlaneSection["qci"].GetInt64();
            std::cout<<" qci "<<qos_prof->qci<<std::endl;

            const RAPIDJSON_NAMESPACE::Value& arpProfileSection = qosPlaneSection["arp"];
            qos_prof->priority = arpProfileSection["priority"].GetInt64();
            qos_prof->pre_empt_capability = arpProfileSection["pre-emption-capability"].GetInt64();
            qos_prof->pre_empt_vulnerability = arpProfileSection["pre-emption-vulnerability"].GetInt64();
            std::cout<<"\t\tQoS apn ambr uplink - "<<qos_prof->apn_ambr_ul<<std::endl
                     <<" downlink - "<<qos_prof->apn_ambr_dl<<std::endl
                     <<" qci "<<qos_prof->qci<<std::endl
                     <<" Priority "<<qos_prof->priority<<std::endl
                     <<" Pre Empt Cap "<<qos_prof->pre_empt_capability<<std::endl
                     <<" Pre Empt Vul "<<qos_prof->pre_empt_vulnerability<<std::endl;
            config->qos_profile_list.push_back(qos_prof);
        }
    }
    if(doc.HasMember("security-profiles"))
    {
        const RAPIDJSON_NAMESPACE::Value& securityProfileSection = doc["security-profiles"];
        for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator itr = securityProfileSection.MemberBegin(); itr != securityProfileSection.MemberEnd(); ++itr)
        {
            security_profile *profile = new (security_profile);
            profile->name = itr->name.GetString();
            std::cout<<"\tSecurity profile - "<<profile->name<<std::endl;
            const RAPIDJSON_NAMESPACE::Value& secSection = itr->value; 
            profile->key = secSection["key"].GetString();
            profile->opc = secSection["opc"].GetString();
            profile->sqn = secSection["sqn"].GetInt64();
            std::cout<<"Security Profile Name - "<<profile->name
                     <<" Opc - "<<profile->opc <<" key - "<<profile->key
                     <<" sqn - "<<profile->sqn<<std::endl;
            config->security_profile_list.push_back(profile);
        }
    }
    if(doc.HasMember("access-profiles"))
    {
        const RAPIDJSON_NAMESPACE::Value& accessProfileSection = doc["access-profiles"];
        for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator itr = accessProfileSection.MemberBegin(); itr != accessProfileSection.MemberEnd(); ++itr)
        {
            std::string key = itr->name.GetString();
            std::cout<<"\tAccess profile - "<<key<<std::endl;
            access_profile *access_prof = new (access_profile);
            access_prof->access_profile_name = key;
            config->access_profile_list.push_back(access_prof);
        }
    }    
    return config;
}

newPortalConfig *
parse_hss_json_doc(RAPIDJSON_NAMESPACE::Document &doc)
{
    newPortalConfig *config = new (newPortalConfig);

    if(doc.HasMember("start-imsi")) 
    {
        config->from_imsi= doc["start-imsi"].GetInt64(); 
    }

    if(doc.HasMember("end-imsi")) 
    {
        config->to_imsi= doc["start-imsi"].GetInt64(); 
    }

    if(doc.HasMember("Opc"))
    {
        config->opc = doc["Opc"].GetString();
    }

    if(doc.HasMember("Key"))
    {
        config->key = doc["Key"].GetString();
    }

    if(doc.HasMember("Sqn"))
    {
        config->sqn = doc["Sqn"].GetInt64();
    }


    if(doc.HasMember("ambr-down"))
    {
        config->apn_ambr_dl = doc["ambr-down"].GetInt();
    }

    if(doc.HasMember("ambr-up"))
    {
        config->apn_ambr_ul = doc["ambr-up"].GetInt();
    }

    std::cout<<" Opc - "<<config->opc <<" key - "<<config->key
        <<" sqn - "<<config->sqn<<std::endl;

    if(doc.HasMember("apn-profiles"))
    {
        const RAPIDJSON_NAMESPACE::Value& apnProfileSection = doc["apn-profiles"];
        for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator itr = apnProfileSection.MemberBegin(); itr != apnProfileSection.MemberEnd(); ++itr)
        {
            std::string key = itr->name.GetString();
            if(itr->value.IsObject())
            {
                std::cout<<"\tAPN profile "<< key.c_str()<<" is Object"<<std::endl; 
                apn_profile *apn_prof = new (apn_profile);
                apn_prof->apn_profile_name = key;
                const RAPIDJSON_NAMESPACE::Value& apnSection = itr->value; 
                if(apnSection.HasMember("apn-name")) {
                    apn_prof->apn_name = apnSection["apn-name"].GetString();
                    std::cout<<"\t\tAPN name["<<apn_prof->apn_name<<"]"<<std::endl;
                    apn_prof->apn_name_length = apn_prof->apn_name.length();
                    std::cout<<"\t\tAPN name after encode ["<<apn_prof->apn_name<<"]"<<std::endl;
                }
                config->apn_profile_list.push_back(apn_prof);
                std::cout<<std::endl;
            }
        }
    }
     
    return config;
}
