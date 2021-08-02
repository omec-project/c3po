// Copyright 2020-present Open Networking Foundation
//
// SPDX-License-Identifier: Apache-2.0

#ifndef __PORTAL_CONFIG_H__
#define __PORTAL_CONFIG_H__

#include "rapidjson/document.h"
#include <list>

class imsi_selection
{
    public:
        imsi_selection() {}
        ~imsi_selection() {}
        bool      is_valid;
        uint64_t  from_imsi;
        uint64_t  to_imsi;
};

class serving_plmn
{
    public:
        serving_plmn() {}
        ~serving_plmn() {}
        bool      is_valid;
        uint8_t   plmn[3];
        uint16_t  tac;
};

class apn_key
{
    public:
        apn_key() {}
        ~apn_key() {}
        bool      is_valid;
        std::string requested_apn;
};

class sub_selection_keys
{
    public:
        sub_selection_keys() {}
        ~sub_selection_keys() {}
        imsi_selection imsi;
        serving_plmn plmn;
        apn_key      apn;
};

class access_profile
{
    public:
        access_profile() {}
        ~access_profile() {}
        std::string access_profile_name;
};

class qos_profile
{
    public:
        qos_profile() {}
        ~qos_profile() {}
        std::string qos_profile_name;
        uint32_t apn_ambr_ul;
        uint32_t apn_ambr_dl;
        uint32_t  qci;
        uint32_t  priority;
        uint32_t  pre_empt_capability;
        uint32_t  pre_empt_vulnerability;
};

class user_plane_profile
{
    public:
        user_plane_profile() {}
        ~user_plane_profile() {}
        std::string user_plane_profile_name;
        std::string user_plane_service;
        uint32_t upf_addr; /* run time information */
};

class apn_profile
{
    public:
        apn_profile() {}
        ~apn_profile() {}
        std::string apn_profile_name;
        std::string apn_name;
        uint8_t apn_name_length;
        int apn_usage_type;
        std::string apn_net_cap;
        bool gx_enabled;
        uint32_t dns_primary;
        uint32_t dns_secondary;
        uint16_t mtu;
};

class  security_profile
{
    public:
        security_profile() {}
        ~security_profile() {}
        std::string name;
        std::string key;
        std::string opc;
        uint64_t    sqn;
};

class sub_selection_rule
{
    public:
        sub_selection_rule() {}
        ~sub_selection_rule()
        {
            delete keys;
        }
        uint32_t rule_priority;
        sub_selection_keys *keys;
        std::string selected_apn_profile;
        std::string selected_user_plane_profile;
        std::string selected_qos_profile;
        std::string selected_access_profile[4]; /* 4 access profiles per subscriber */
        std::string selected_security_profile;
};

class portalConfig
{
    public:
        portalConfig() {;}
        ~portalConfig()
        {
            for (std::list<sub_selection_rule *> ::iterator it=sub_sel_rules.begin(); it!=sub_sel_rules.end(); ++it)
            {
                sub_selection_rule *rule = *it;
                delete rule;
            }

            for (std::list<apn_profile*>::iterator it=apn_profile_list.begin(); it!=apn_profile_list.end(); ++it)
            {
                apn_profile *apn=*it;
                delete apn;
            }
            for (std::list<user_plane_profile*>::iterator it=user_plane_list.begin(); it!=user_plane_list.end(); ++it)
            {
                user_plane_profile *up=*it;
                delete up;
            }
            for (std::list<qos_profile*>::iterator it=qos_profile_list.begin(); it!=qos_profile_list.end(); ++it)
            {
                qos_profile *qos=*it;
                delete qos;
            }
            for (std::list<access_profile*>::iterator it=access_profile_list.begin(); it!=access_profile_list.end(); ++it)
            {
                access_profile *access=*it;
                delete access;
            }
        }

        apn_profile* get_apn_profile(const std::string name)
        {
            for (std::list<apn_profile*>::iterator it=apn_profile_list.begin(); it!=apn_profile_list.end(); ++it)
            {
                apn_profile *apn=*it;
                if(apn->apn_profile_name.compare(name) == 0)
                    return apn; 
            }
            return nullptr;
        }

        user_plane_profile* get_user_plane_profile(const std::string name) 
        {
            for (std::list<user_plane_profile*>::iterator it=user_plane_list.begin(); it!=user_plane_list.end(); ++it)
            {
                user_plane_profile *up=*it;
                if(up->user_plane_profile_name.compare(name) == 0)
                    return up; 
            }
            return nullptr;
        }

        qos_profile* get_qos_profile(const std::string name) 
        {
            for (std::list<qos_profile*>::iterator it=qos_profile_list.begin(); it!=qos_profile_list.end(); ++it)
            {
                qos_profile *qos=*it;
                if(qos->qos_profile_name.compare(name) == 0)
                    return qos; 
            }
            return nullptr;
        }

        security_profile* get_security_profile(const std::string name) 
        {
            for (std::list<security_profile*>::iterator it=security_profile_list.begin(); it!=security_profile_list.end(); ++it)
            {
                security_profile *sec=*it;
                if(sec->name.compare(name) == 0)
                    return sec; 
            }
            return nullptr;
        }

        access_profile* get_access_profile(const std::string name) 
        {
            for (std::list<access_profile*>::iterator it=access_profile_list.begin(); it!=access_profile_list.end(); ++it)
            {
                access_profile *access=*it;
                if(access->access_profile_name.compare(name) == 0)
                    return access; 
            }
            return nullptr;
        }
    public:
        std::list<sub_selection_rule *> sub_sel_rules;
        std::list<apn_profile *> apn_profile_list;
        std::list<user_plane_profile*> user_plane_list;
        std::list<access_profile*> access_profile_list;
        std::list<qos_profile*> qos_profile_list;
        std::list<security_profile*> security_profile_list;
};

class newPortalConfig
{
    public:
        newPortalConfig() {;}
        ~newPortalConfig()
        {
            for (std::list<apn_profile*>::iterator it=apn_profile_list.begin(); it!=apn_profile_list.end(); ++it)
            {
                apn_profile *apn=*it;
                delete apn;
            }
        }

        apn_profile* get_apn_profile(const std::string name)
        {
            for (std::list<apn_profile*>::iterator it=apn_profile_list.begin(); it!=apn_profile_list.end(); ++it)
            {
                apn_profile *apn=*it;
                if(apn->apn_profile_name.compare(name) == 0)
                    return apn; 
            }
            return nullptr;
        }

    public:
        std::string key;
        std::string opc;
        uint64_t    sqn;
        uint64_t    from_imsi;
        uint64_t    to_imsi;
        uint32_t    apn_ambr_ul;
        uint32_t    apn_ambr_dl;
        uint32_t    qci;
        uint32_t    arp;
        std::list<apn_profile *> apn_profile_list;
};

extern "C" {
    portalConfig *parse_json_doc(RAPIDJSON_NAMESPACE::Document &doc);
    newPortalConfig *parse_hss_json_doc(RAPIDJSON_NAMESPACE::Document &doc);
}
#endif
