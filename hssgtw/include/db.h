/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/



#ifndef HSSGTW_INCLUDE_DB_H_
#define HSSGTW_INCLUDE_DB_H_

#include <iostream>
#include <scassandra.h>
#include "sutility.h"
#include <stdexcept>

#define RAPIDJSON_NAMESPACE hssrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

class DAException : public std::runtime_error
{
public:
   DAException(const char *m) : std::runtime_error(m) {}
   DAException(const std::string &m) : std::runtime_error(m) {}
};

#define PARSE_STRING(_doc, _name) \
{ \
   if (_doc[#_name].IsNull()) \
   { \
      _name = ""; \
      _name ## _pres = false; \
   } \
   else if (_doc[#_name].IsString()) \
   { \
      _name = _doc[#_name].GetString(); \
      _name ## _pres = true; \
   } \
   else \
   { \
      std::cout << "Incompatible data type for " #_name << std::endl; \
   } \
}

#define PARSE_INT32(_doc, _name) \
{ \
   if (_doc[#_name].IsNull()) \
   { \
      _name = 0; \
      _name ## _pres = false; \
   } \
   else if (_doc[#_name].IsInt()) \
   { \
      _name = _doc[#_name].GetInt(); \
      _name ## _pres = true; \
   } \
   else \
   { \
      std::cout << "Incompatible data type for " #_name << std::endl; \
   } \
}

#define PARSE_INT64(_doc, _name) \
{ \
   if (_doc[#_name].IsNull()) \
   { \
      _name = 0; \
      _name ## _pres = false; \
   } \
   else if (_doc[#_name].IsInt64()) \
   { \
      _name = _doc[#_name].GetInt64(); \
      _name ## _pres = true; \
   } \
   else \
   { \
      std::cout << "Incompatible data type for " #_name << std::endl; \
   } \
}

class ImsiEntity
{
public:
   ImsiEntity()
   {
      imsi = "";
      access_restriction = 0;
      idmmeidentity = 0;
      imei = "";
      imei_sv = "";
      key = "";
      lipa_permissions = "";
      mme_cap = 0;
      mmehost = "";
      mmeidentity_idmmeidentity = 0;
      mmerealm = "";
      ms_ps_status = "";
      msisdn = 0;
      niddvalidity = "";
      nir_dest_host = "";
      nir_dest_realm = "";
      opc = "";
      pgw_id = 0;
      rand = "";
      rfsp_index = 0;
      sqn = 0;
      subscription_data = "";
      supported_features = "";
      ue_reachability = 0;
      urrp_mme = 0;
      user_identifier = "";
      visited_plmnid = "";

      imsi_pres = false;
      access_restriction_pres = false;
      idmmeidentity_pres = false;
      imei_pres = false;
      imei_sv_pres = false;
      key_pres = false;
      lipa_permissions_pres = false;
      mme_cap_pres = false;
      mmehost_pres = false;
      mmeidentity_idmmeidentity_pres = false;
      mmerealm_pres = false;
      ms_ps_status_pres = false;
      msisdn_pres = false;
      niddvalidity_pres = false;
      nir_dest_host_pres = false;
      nir_dest_realm_pres = false;
      opc_pres = false;
      pgw_id_pres = false;
      rand_pres = false;
      rfsp_index_pres = false;
      sqn_pres = false;
      subscription_data_pres = false;
      ue_reachability_pres = false;
      urrp_mme_pres = false;
      user_identifier_pres = false;
      visited_plmnid_pres = false;
   }

   bool parseJson(RAPIDJSON_NAMESPACE::Document &document)
   {
      PARSE_STRING(document, imsi);
      PARSE_INT32(document, access_restriction);
      PARSE_INT32(document, idmmeidentity);
      PARSE_STRING(document, imei);
      PARSE_STRING(document, imei_sv);
      PARSE_STRING(document, key);
      PARSE_STRING(document, lipa_permissions);
      PARSE_INT32(document, mme_cap);
      PARSE_STRING(document, mmehost);
      PARSE_INT32(document, mmeidentity_idmmeidentity);
      PARSE_STRING(document, mmerealm);
      PARSE_STRING(document, ms_ps_status);
      PARSE_INT64(document, msisdn);
      PARSE_STRING(document, niddvalidity);
      PARSE_STRING(document, nir_dest_host);
      PARSE_STRING(document, nir_dest_realm);
      PARSE_STRING(document, opc);
      PARSE_INT32(document, pgw_id);
      PARSE_STRING(document, rand);
      PARSE_INT32(document, rfsp_index);
      PARSE_INT32(document, sqn);
      PARSE_STRING(document, subscription_data);
      PARSE_STRING(document, supported_features);
      PARSE_INT32(document, ue_reachability);
      PARSE_INT32(document, urrp_mme);
      PARSE_STRING(document, user_identifier);
      PARSE_STRING(document, visited_plmnid);
      return true;
   }

   std::string imsi;                     bool imsi_pres;
   int32_t access_restriction;           bool access_restriction_pres;
   int32_t idmmeidentity;                bool idmmeidentity_pres;
   std::string imei;                     bool imei_pres;
   std::string imei_sv;                  bool imei_sv_pres;
   std::string key;                      bool key_pres;
   std::string lipa_permissions;         bool lipa_permissions_pres;
   int32_t mme_cap;                      bool mme_cap_pres;
   std::string mmehost;                  bool mmehost_pres;
   int32_t mmeidentity_idmmeidentity;    bool mmeidentity_idmmeidentity_pres;
   std::string mmerealm;                 bool mmerealm_pres;
   std::string ms_ps_status;             bool ms_ps_status_pres;
   int64_t msisdn;                       bool msisdn_pres;
   std::string niddvalidity;             bool niddvalidity_pres;
   std::string nir_dest_host;            bool nir_dest_host_pres;
   std::string nir_dest_realm;           bool nir_dest_realm_pres;
   std::string opc;                      bool opc_pres;
   int32_t pgw_id;                       bool pgw_id_pres;
   std::string rand;                     bool rand_pres;
   int32_t rfsp_index;                   bool rfsp_index_pres;
   int32_t sqn;                          bool sqn_pres;
   std::string subscription_data;        bool subscription_data_pres;
   std::string supported_features;       bool supported_features_pres;
   int32_t ue_reachability;              bool ue_reachability_pres;
   int32_t urrp_mme;                     bool urrp_mme_pres;
   std::string user_identifier;          bool user_identifier_pres;
   std::string visited_plmnid;           bool visited_plmnid_pres;
};

class DB
{
   public:
      static DB& getInstance()
      {
         static DB instance;
         return instance;
      }

      std::string &host( const char *hst ) { return m_db.host(hst); }
      std::string &host( const std::string &hst ) { return m_db.host(hst); }
      std::string &host() { return m_db.host(); }

      std::string &keyspace( const char *ks ) { return m_db.keyspace(ks); }
      std::string &keyspace( const std::string &ks ) { return m_db.keyspace(ks); }
      std::string &keyspace() { return m_db.keyspace(); }

      void connect( const std::string &hst, const std::string &ks )
      {
         host( hst );
         keyspace( ks );
         connect();
      }

      void connect( const char *hst, const char *ks )
      {
         host( hst );
         keyspace( ks );
         connect();
      }

      void connect()
      {
         SCassFuture connect_future = m_db.connect();

         connect_future.wait();

         if ( connect_future.errorCode() != CASS_OK )
         {
            throw DAException(
                  SUtility::string_format( "DataAccess::%s - Unable to connect to %s - error_code=%d",
                        __func__, m_db.host().c_str(), connect_future.errorCode() )
            );
         }
      }

      void disconnect()
      {
         m_db.disconnect();
      }

      bool checkUserImsi( const std::string &imsi );
      bool insertUserImsi( const ImsiEntity &ie );
      bool updateUserImsi( const ImsiEntity &ie );

      bool deleteUserImsi(const std::string &imsi);

      bool getImeiMsisdnFromImsi( const char *imsi, std::string &imei, std::string &imei_sv, int64_t &msisdn );

      bool checkUserMsisdn( int64_t msisdn );
      bool insertUserMsisdn( const std::string &imsi, int64_t msisdn );
      bool deleteUserMsisdn( int64_t msisdn );
      bool updateUserMsisdn( const std::string &imsi, int64_t old_msisdn, int64_t new_msisdn );

   private:
      DB(){}
      SCassandra m_db;
};



#endif /* HSSGTW_INCLUDE_DB_H_ */
