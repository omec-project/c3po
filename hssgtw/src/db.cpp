/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "db.h"
#include <iostream>
#include <sstream>

#define GET_EVENT_DATA( _row, _col, _dest ) \
{ \
   SCassValue val = _row.getColumn( #_col ); \
   if ( !val.get( _dest ) ) \
      throw DAException( \
            SUtility::string_format("DataAccess::%s - ERROR - Error %d getting [%s]", \
                  __func__, future.errorCode(), #_col ) \
      ); \
}

#define ADD_VALUE_STRING(_ss, _ie, _name, _comma) \
{ \
   if (_ie._name ## _pres) \
      _ss << "'" << _ie._name << "'" _comma; \
   else \
      _ss << "null" _comma; \
}

#define ADD_VALUE_INT(_ss, _ie, _name, _comma) \
{ \
   if (_ie._name ## _pres) \
      _ss << _ie._name << _comma; \
   else \
      _ss << "null" _comma; \
}

#define ADD_UPDATE_STRING(_ss, _ie, _name, _cnt) \
{ \
   if (_ie._name ## _pres) \
      _ss << (_cnt++?",":"") << #_name << "='" << _ie._name << "'"; \
}

#define ADD_UPDATE_INT(_ss, _ie, _name, _cnt) \
{ \
   if (_ie._name ## _pres) \
      _ss << (_cnt++?",":"") << #_name << "=" << _ie._name; \
}

bool DB::checkUserImsi( const std::string &imsi )
{
   std::stringstream ss;

   ss << "SELECT imsi FROM users_imsi WHERE imsi = '" << imsi << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      throw DAException(
         SUtility::string_format( "DataAccess::%s - Error %d executing [%s]",
         __func__, future.errorCode(), ss.str().c_str() )
      );
   }

   SCassResult res = future.result();
   SCassRow row = res.firstRow();

   return row.valid();
}

bool DB::insertUserImsi( const ImsiEntity &ie )
{
   std::stringstream ss;

   ss << "INSERT INTO users_imsi ("
         << "imsi,access_restriction,idmmeidentity,imei,imei_sv,key,"
         << "lipa_permissions,mme_cap,mmehost,mmeidentity_idmmeidentity,"
         << "mmerealm,ms_ps_status,msisdn,niddvalidity,nir_dest_host,"
         << "nir_dest_realm,opc,pgw_id,rand,rfsp_index,sqn,subscription_data,supported_features"
         << "ue_reachability,urrp_mme,user_identifier,visited_plmnid"
         << ") VALUES (";
   ADD_VALUE_STRING(ss, ie, imsi, ",");
   ADD_VALUE_INT(ss, ie, access_restriction, ",");
   ADD_VALUE_INT(ss, ie, idmmeidentity, ",");
   ADD_VALUE_STRING(ss, ie, imei, ",");
   ADD_VALUE_STRING(ss, ie, imei_sv, ",");
   ADD_VALUE_STRING(ss, ie, key, ",");
   ADD_VALUE_STRING(ss, ie, lipa_permissions, ",");
   ADD_VALUE_INT(ss, ie, mme_cap, ",");
   ADD_VALUE_STRING(ss, ie, mmehost, ",");
   ADD_VALUE_INT(ss, ie, mmeidentity_idmmeidentity, ",");
   ADD_VALUE_STRING(ss, ie, mmerealm, ",");
   ADD_VALUE_STRING(ss, ie, ms_ps_status, ",");
   ADD_VALUE_INT(ss, ie, msisdn, ",");
   ADD_VALUE_STRING(ss, ie, niddvalidity, ",");
   ADD_VALUE_STRING(ss, ie, nir_dest_host, ",");
   ADD_VALUE_STRING(ss, ie, nir_dest_realm, ",");
   ADD_VALUE_STRING(ss, ie, opc, ",");
   ADD_VALUE_INT(ss, ie, pgw_id, ",");
   ADD_VALUE_STRING(ss, ie, rand, ",");
   ADD_VALUE_INT(ss, ie, rfsp_index, ",");
   ADD_VALUE_INT(ss, ie, sqn, ",");
   ADD_VALUE_STRING(ss, ie, subscription_data, ",");
   ADD_VALUE_STRING(ss, ie, supported_features, ",");
   ADD_VALUE_INT(ss, ie, ue_reachability, ",");
   ADD_VALUE_INT(ss, ie, urrp_mme, ",");
   ADD_VALUE_STRING(ss, ie, user_identifier, ",");
   ADD_VALUE_STRING(ss, ie, visited_plmnid, "");
   ss << ")";

   std::cout << ss.str() << std::endl;

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      std::cout << "DataAccess::" << __func__ << " - Error " << future.errorCode()
                << " executing [" << ss.str() << "]" << std::endl;
      return false;
   }

   if ( !insertUserMsisdn( ie.imsi, ie.msisdn ) )
   {
      deleteUserImsi( ie.imsi );
      return false;
   }

   return true;
}

bool DB::updateUserImsi( const ImsiEntity &ie )
{
   int cnt = 0;
   std::stringstream ss;

   ss << "UPDATE users_imsi SET ";
   ADD_UPDATE_INT(ss, ie, access_restriction, cnt);
   ADD_UPDATE_INT(ss, ie, idmmeidentity, cnt);
   ADD_UPDATE_STRING(ss, ie, imei, cnt);
   ADD_UPDATE_STRING(ss, ie, imei_sv, cnt);
   ADD_UPDATE_STRING(ss, ie, key, cnt);
   ADD_UPDATE_STRING(ss, ie, lipa_permissions, cnt);
   ADD_UPDATE_INT(ss, ie, mme_cap, cnt);
   ADD_UPDATE_STRING(ss, ie, mmehost, cnt);
   ADD_UPDATE_INT(ss, ie, mmeidentity_idmmeidentity, cnt);
   ADD_UPDATE_STRING(ss, ie, mmerealm, cnt);
   ADD_UPDATE_STRING(ss, ie, ms_ps_status, cnt);
   ADD_UPDATE_INT(ss, ie, msisdn, cnt);
   ADD_UPDATE_STRING(ss, ie, niddvalidity, cnt);
   ADD_UPDATE_STRING(ss, ie, nir_dest_host, cnt);
   ADD_UPDATE_STRING(ss, ie, nir_dest_realm, cnt);
   ADD_UPDATE_STRING(ss, ie, opc, cnt);
   ADD_UPDATE_INT(ss, ie, pgw_id, cnt);
   ADD_UPDATE_STRING(ss, ie, rand, cnt);
   ADD_UPDATE_INT(ss, ie, rfsp_index, cnt);
   ADD_UPDATE_INT(ss, ie, sqn, cnt);
   ADD_UPDATE_STRING(ss, ie, subscription_data, cnt);
   ADD_UPDATE_STRING(ss, ie, supported_features, cnt);
   ADD_UPDATE_INT(ss, ie, ue_reachability, cnt);
   ADD_UPDATE_INT(ss, ie, urrp_mme, cnt);
   ADD_UPDATE_STRING(ss, ie, user_identifier, cnt);
   ADD_UPDATE_STRING(ss, ie, visited_plmnid, cnt);
   ss << " WHERE imsi = '" << ie.imsi << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      throw DAException(
         SUtility::string_format( "DataAccess::%s - Error %d executing [%s]",
         __func__, future.errorCode(), ss.str().c_str() )
      );
   }
   return true;
}

bool DB::deleteUserImsi( const std::string &id )
{
   std::stringstream ss;

   ss << "DELETE FROM users_imsi where imsi='" << id << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      throw DAException(
         SUtility::string_format( "DataAccess::%s - Error %d executing [%s]",
         __func__, future.errorCode(), ss.str().c_str() )
      );
   }

   return true;
}

bool DB::checkUserMsisdn( int64_t msisdn )
{
   std::stringstream ss;

   ss << "SELECT msisdn FROM msisdn_imsi WHERE msisdn = " << msisdn;

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      throw DAException(
         SUtility::string_format( "DataAccess::%s - Error %d executing [%s]",
         __func__, future.errorCode(), ss.str().c_str() )
      );
   }

   SCassResult res = future.result();
   SCassRow row = res.firstRow();

   return row.valid();
}

bool DB::insertUserMsisdn( const std::string &imsi, int64_t msisdn )
{
   std::stringstream ss;

    ss << "INSERT INTO msisdn_imsi (msisdn,imsi) VALUES (" << msisdn << ",'" << imsi << "')";

   std::cout << ss.str() << std::endl;

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      std::cout << "DataAccess::" << __func__ << " - Error " << future.errorCode()
                << " executing [" << ss.str() << "]" << std::endl;
      return false;
   }

   return true;
}

bool DB::deleteUserMsisdn( int64_t msisdn )
{
   std::stringstream ss;

   ss << "DELETE FROM msisdn_imsi WHERE msisdn=" << msisdn;

   std::cout << ss.str() << std::endl;

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      std::cout << "DataAccess::" << __func__ << " - Error " << future.errorCode()
                << " executing [" << ss.str() << "]" << std::endl;
      return false;
   }

   return true;
}

bool DB::updateUserMsisdn( const std::string &imsi, int64_t old_msisdn, int64_t new_msisdn )
{
   if (!deleteUserMsisdn(old_msisdn))
      return false;
   return insertUserMsisdn(imsi, new_msisdn);
}

bool DB::getImeiMsisdnFromImsi( const char *imsi, std::string &imei, std::string &imei_sv, int64_t &msisdn )
{
   std::stringstream ss;

   ss << "SELECT imei, imei_sv, msisdn FROM users_imsi where imsi = '"
      << imsi << "' ;" ;

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK ){
      throw DAException(
           SUtility::string_format( "DataAccess::%s - Error %d executing [%s]",
                 __func__, future.errorCode(), ss.str().c_str() )
      );
   }

   SCassResult res = future.result();
   SCassRow row = res.firstRow();

   if ( row.valid() )
   {
      GET_EVENT_DATA( row, imei, imei );
      GET_EVENT_DATA( row, imei_sv, imei_sv );
      GET_EVENT_DATA( row, msisdn, msisdn );
      return true;
   }
   return false;
}
