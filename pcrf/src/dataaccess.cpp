/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <iomanip>
#include <sstream>
#include <iostream>
#include <inttypes.h>

#include "dataaccess.h"

#include "sutility.h"
#include "serror.h"
#include "logger.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define GET_EVENT_DATA( _row, _col, _dest ) \
{ \
   SCassValue val = _row.getColumn( #_col ); \
   if ( !val.get( _dest ) ) \
      throw DAException( \
         SUtility::string_format("DataAccess::%s - ERROR - Error %d getting [%s]", \
         __func__, future.errorCode(), #_col ) \
      ); \
}

#define GET_EVENT_DATA2( _row, _col, _dest, _setter ) \
{ \
   SCassValue val = _row.getColumn( #_col ); \
   if ( !val.get( _dest ) ) \
      throw DAException( \
         SUtility::string_format("DataAccess::%s - ERROR - Error %d getting [%s]", \
         __func__, future.errorCode(), #_col ) \
      ); \
   _setter( _dest ); \
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DataAccess::DataAccess()
{
}

DataAccess::~DataAccess()
{
   disconnect();
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// PUBLIC METHODS /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void DataAccess::connect( const std::string &hst, const std::string &ks )
{
   host( hst );
   keyspace( ks );
   connect();
}

void DataAccess::connect( const char *hst, const char *ks )
{
   host( hst );
   keyspace( ks );
   connect();
}

void DataAccess::connect()
{
   SCassFuture connect_future = m_db.connect();

   connect_future.wait();

   if ( connect_future.errorCode() != CASS_OK )
      throw DAException(
         SUtility::string_format( "DataAcces::%s - Unable to connect to %s - error_code=%d",
            __func__, m_db.host().c_str(), connect_future.errorCode() )
      );
}

void DataAccess::disconnect()
{
   m_db.disconnect();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool DataAccess::getRules( RulesMap &rules )
{
   bool result = true;
   bool b;
   int64_t i64;
   std::string s;
   std::vector<std::string> crv;
   RulesMap::iterator ruleit;
   std::string sql( "SELECT * FROM rules" );

   SCassStatement stmt( sql.c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), sql.c_str() );
      return false;
   }

   SCassResult res = future.result();

   SCassIterator rows = res.rows();

   while ( result && rows.nextRow() )
   {
		int i = 0;
      SCassRow row = rows.row();
      Rule *r = new Rule();

      try
      {
         GET_EVENT_DATA2( row, rulename, s, r->setRuleName );

         if ( (ruleit = rules.find(r->getRuleName())) != rules.end() )
         {
            // found the apn so must be a duplicate
            Logger::system().error( "DataAccess::%s:%d - duplicate Rule [%s] while loading",
                  __func__, __LINE__, s.c_str() );
            result = false;
            delete r;
            break;
         }

         GET_EVENT_DATA2( row, basename, s, r->setBaseName );
         GET_EVENT_DATA2( row, type, s, r->setType );
         GET_EVENT_DATA2( row, definition_json, s, r->setDefinition );
         GET_EVENT_DATA2( row, timeofday_json, s, r->setTimeOfDay );
         GET_EVENT_DATA2( row, usage_monitoring_info, s, r->setUsageMonitoringInfo );
         GET_EVENT_DATA2( row, sy_required, b, r->setSyRequired );
         GET_EVENT_DATA2( row, timemask, i64, r->setTimeMask );
         GET_EVENT_DATA2( row, featuremask, i64, r->setFeatureMask );
         GET_EVENT_DATA2( row, defaultrule, b, r->setDefaultFlag );

         rules.insert( std::pair<std::string,Rule*>( r->getRuleName(), r ) );
			printf ("SOHAN : DATAACCESS : RULES NAME : %s\n", r->getRuleName().c_str());
      }
      catch ( DAException &ex )
      {
         Logger::system().error( "DataAccess::%s:%d - DAException loading Rule's - %s",
               __func__, __LINE__, ex.what() );
         result = false;
         delete r;
         break;
      }
      catch ( ... )
      {
         Logger::system().error( "DataAccess::%s:%d - Unrecognized exception loading Rule's",
               __func__, __LINE__ );
         result = false;
         delete r;
         break;
      }
		i++;
   }
   return result;
}

bool DataAccess::getApns( ApnMap &apns, RulesMap &rules )
{
   bool result = true;
   bool b;
   int32_t i32;
   std::string s;
   std::vector<std::string> crv;
   ApnMap::iterator apnit;
   RulesMap::iterator ruleit;
   std::string sql( "SELECT * FROM apns" );

   SCassStatement stmt( sql.c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
      throw DAException(
         SUtility::string_format( "DataAcces::%s - Error %d executing [%s]",
               __func__, future.errorCode(), sql.c_str() )
      );

   SCassResult res = future.result();

   SCassIterator rows = res.rows();

   while ( result && rows.nextRow() )
   {
      SCassRow row = rows.row();
      Apn *a = new Apn();

      try
      {
         GET_EVENT_DATA2( row, apn, s, a->setApn );

         if ( (apnit = apns.find(a->getApn())) != apns.end() )
         {
            // found the apn so must be a duplicate
				printf ("SOHAN : DUPLICATE APN in DATABASE\n");
            Logger::system().error( "DataAccess::%s:%d - duplicate APN [%s] while loading",
                  __func__, __LINE__, s.c_str() );
            result = false;
            delete a;
            break;
         }

         GET_EVENT_DATA2( row, fail_on_uninstallable_rule, b, a->setFailOnUninstallableRule );
         GET_EVENT_DATA2( row, sy_subscription_id, s, a->setSySubscriptionId );
         GET_EVENT_DATA2( row, auto_create_spr, b, a->setAutoCreateSubscriber );
         GET_EVENT_DATA2( row, membership_value, s, a->setMembershipValue );
         GET_EVENT_DATA2( row, domain, s, a->setDomain );
         GET_EVENT_DATA2( row, sy_required, b, a->setSyRequired );
         GET_EVENT_DATA2( row, default_bearer_ctl_mode, i32, a->setDefaultBearerCtlMode );
         GET_EVENT_DATA2( row, force_default_bearer_ctl_mode, b, a->setForceDefaultBearerCtlMode );

         GET_EVENT_DATA( row, computed_rules, s );

         crv = SUtility::split( s, ',' );

         for ( auto &rulename : crv )
         {
            if ( (ruleit = rules.find(rulename)) != rules.end() )
            {
               a->addComputedRule( ruleit->second );
            }
            else
            {
               // found the apn so must be a duplicate
					printf ("SOHAN DUPLICATE ENTRY of RULE NAME: %s\n", rulename.c_str());
               Logger::system().error( "DataAccess::%s:%d - rule [%s] not found while loading APN [%s]",
                     __func__, __LINE__, s.c_str(), a->getApn().c_str() );
               result = false;
               delete a;
               break;
            }
         }

         apns.insert( std::pair<std::string,Apn*>( a->getApn(), a ) );
      }
      catch ( DAException &ex )
      {
         Logger::system().error( "DataAccess::%s:%d - DAException loading APN's - %s",
               __func__, __LINE__, ex.what() );
         result = false;
         delete a;
         break;
      }
      catch ( ... )
      {
         Logger::system().error( "DataAccess::%s:%d - Unrecognized exception loading APN's",
               __func__, __LINE__ );
         result = false;
         delete a;
         break;
      }
   }

   return result;
}

bool DataAccess::getEndpoints( EndpointMap &endpoints )
{
   bool result = true;
   bool b;
   int64_t i64;
   std::string s;
   std::string sql( "SELECT * FROM endpoints" );

   SCassStatement stmt( sql.c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), sql.c_str() );
      return false;
   }

   SCassResult res = future.result();

   SCassIterator rows = res.rows();

   while ( result && rows.nextRow() )
   {
      SCassRow row = rows.row();
      Endpoint *ep = new Endpoint();

      try
      {
         GET_EVENT_DATA2( row, endpointid, s, ep->setHost );
         GET_EVENT_DATA2( row, endpointrealm, s, ep->setRealm );
         GET_EVENT_DATA2( row, endpointip, s, ep->setIp );
         GET_EVENT_DATA2( row, capabilities, i64, ep->setCapabilities );
         GET_EVENT_DATA2( row, featuremask, i64, ep->setFeatureMask );
         GET_EVENT_DATA2( row, supported_feature_persistance, b, ep->setSupportedFeaturePersistance );
         GET_EVENT_DATA2( row, assigned_tdf, s, ep->setAssignedTdf );
         GET_EVENT_DATA2( row, assigned_tssf, s, ep->setAssignedTssf );

         if ( !endpoints.addEndpoint( ep ) )
         {
            Logger::system().error( "DataAccess::%s:%d - Endpoint [%s] already exists",
                  __func__, __LINE__, ep->getHost().c_str() );
            result = false;
            delete ep;
            break;
         }
      }
      catch ( DAException &ex )
      {
         Logger::system().error( "DataAccess::%s:%d - DAException loading Rule's - %s",
               __func__, __LINE__, ex.what() );
         result = false;
         delete ep;
         break;
      }
      catch ( ... )
      {
         Logger::system().error( "DataAccess::%s:%d - Unrecognized exception loading Rule's",
               __func__, __LINE__ );
         result = false;
         delete ep;
         break;
      }
   }

   return result;
}
bool DataAccess::addEndpoint( const Endpoint &ep )
{
   bool result = true;
   std::stringstream ss;

   ss << "INSERT INTO endpoints ("
         << "endpointid, endpointrealm, endpointip, capabilities, featuremask, "
         << "supported_feature_persistance, assigned_tdf, assigned_tssf"
      << ") VALUES ("
         << "'" << ep.getHost() << "',"
         << "'" << ep.getRealm() << "',"
         << "'" << ep.getIp() << "',"
         << ep.getCapabilities() << ","
         << ep.getFeatureMask() << ","
         << ( ep.getSupportedFeaturePersistance() ? "true" : "false" ) << ","
         << "'" << ep.getAssignedTdf() << "',"
         << "'" << ep.getAssignedTssf() << "'"
      << ")";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      result = false;
   }

   return result;
}

bool DataAccess::addSession( GxSession &s )
{
   bool result = _addSession( s, s.getIPv4str(), s.getIPv6str() );

   if ( result && s.getIPv4Len() != 0 )
      result = _addSessionIP( s.getIPv4str(), s.getImsi(), s.getApn() );
   if ( result && s.getIPv6Len() != 0 )
      result = _addSessionIP( s.getIPv6str(), s.getImsi(), s.getApn() );

   if ( result )
   {
      for ( auto bearer : s.getBearers() )
      {
         result = _addSessionBearer( s.getImsi(), s.getApn(), *bearer.second );
         if ( !result )
            break;
      }
   }

   if ( !result )
      deleteSession( s );

   return result;
}

void DataAccess::deleteSession( GxSession &s )
{
   for ( auto bearer : s.getBearers() )
      _deleteSessionBearer( s.getImsi(), s.getApn(), *bearer.second );

   if ( s.getIPv4Len() != 0 )
      _deleteSessionIP( s.getIPv4str(), s.getImsi(), s.getApn() );
   if ( s.getIPv6Len() != 0 )
      _deleteSessionIP( s.getIPv6str(), s.getImsi(), s.getApn() );

   _deleteSession( s );
}

bool DataAccess::getSubscriber( const char *imsi, Subscriber &sub, RulesMap &rules )
{
   bool result = _getSubscriber( imsi, sub );

   if ( result )
      result = _getSubscriberApns( imsi, sub, rules );

   return result;
}

bool DataAccess::sessionExists( const char *imsi, const char *apn )
{
   bool result = true;
   std::stringstream ss;

   ss << "SELECT imsi, apn FROM session WHERE "
         << "imsi = '" << imsi << "' AND "
         << "apn = '" << apn << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      result = false;
   }

   SCassResult res = future.result();

   SCassRow row = res.firstRow();

   return row.valid();
}

bool DataAccess::addSubscriber( const Subscriber &s )
{
   bool result = _addSubscriber( s );

   if ( result )
   {
      for ( auto const &pair : s.getApnPolicies() )
      {
         if ( !( result = _addSubscriberApn( s.getImsi().c_str(), *pair.second ) ) )
         {
            deleteSubscriber( s );
            break;
         }
      }
   }

   return result;
}

bool DataAccess::addSubscriberApn( const char *imsi, const SubscriberApn &sa )
{
   return _addSubscriberApn( imsi, sa );
}

void DataAccess::deleteSubscriber( const Subscriber &s )
{
   _deleteSubscriber( s.getImsi().c_str() );

   for ( auto const &pair : s.getApnPolicies() )
      _deleteSubscriberApn( s.getImsi().c_str(), pair.second->getApn().c_str() );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void DataAccess::_concatenateRules( const RulesList &rules, std::string &str )
{
   bool first = true;
   std::stringstream ss;

   for ( auto rule : rules.getRules() )
      ss << ( first ? "" : "," ) << rule->getRuleName();

   str = ss.str();
}

bool DataAccess::_addSession( GxSession &s, const std::string &ipv4, const std::string &ipv6 )
{
   bool result = true;
   std::stringstream ss;

   ss << "INSERT INTO session ("
         << "imsi, apn, pcrf_endpoint, ipv4, ipv6, pcef_sessionid, pcef_endpoint, "
         << "tdf_sessionid, tdf_endpoint, tsc_sessionid, tsc_endpoint"
      << ") VALUES ("
         << "'" << s.getImsi() << "',"
         << "'" << s.getApn() << "',"
         << "'" << s.getPcrfEndpoint() << "',"
         << "'" << ipv4 << "',"
         << "'" << ipv6 << "',"
         << "'" << s.getSessionId() << "',"
         << "'" << (s.getPcefEndpoint() ? s.getPcefEndpoint()->getHost() : "") << "',"
         << "'" << s.getTdfSession().getSessionId() << "',"
         << "'" << (s.getTdfSession().getEndpoint() ? s.getTdfSession().getEndpoint()->getHost() : "") << "',"
         << "'" << s.getTssfSession().getSessionId() << "',"
         << "'" << (s.getTssfSession().getEndpoint() ? s.getTssfSession().getEndpoint()->getHost() : "") << "'"
      << ")";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      result = false;
   }

   return result;
}

bool DataAccess::_addSessionIP( const std::string &ip, const std::string &imsi, const std::string &apn )
{
   bool result = true;
   std::stringstream ss;

   ss << "INSERT INTO session_ip ("
         << "ip, imsi, apn"
      << ") VALUES ("
         << "'" << ip << "',"
         << "'" << imsi << "',"
         << "'" << apn << "'"
      << ")";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      result = false;
   }

   return result;
}

bool DataAccess::_addSessionBearer( const std::string &imsi, const std::string &apn, Bearer &b )
{
   bool result = true;
   std::stringstream ss;
   std::string computed, pcef, tdf, tsc;

   _concatenateRules( b.getComputedRules(), computed );
   _concatenateRules( b.getPcefInstalled(), pcef );
   _concatenateRules( b.getTdfInstalled(), tdf );
   _concatenateRules( b.getTscInstalled(), tsc );

   ss << "INSERT INTO session_bearer ("
         << "imsi, apn, ebi, computed_rules, pcef_installed_rules, tdf_installed_rules, tsc_installed_rules"
      << ") VALUES ("
         << "'" << imsi << "',"
         << "'" << apn << "',"
         << b.getEBI() << ","
         << "'" << computed << "',"
         << "'" << pcef << "',"
         << "'" << tdf << "',"
         << "'" << tsc << "'"
      << ")";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      result = false;
   }

   return result;
}

void DataAccess::_deleteSession( GxSession &s )
{
   std::stringstream ss;

   ss << "DELETE FROM session WHERE "
      << "imsi = '" << s.getImsi() << "' AND "
      << "apn = '" << s.getApn() << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
   }
}

void DataAccess::_deleteSessionIP( const std::string &ip, const std::string &imsi, const std::string &apn )
{
   std::stringstream ss;

   ss << "DELETE FROM session_ip WHERE "
      << "ip = '" << ip << "' AND "
      << "imsi = '" << imsi << "' AND "
      << "apn = '" << apn << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
   }
}

void DataAccess::_deleteSessionBearer( const std::string &imsi, const std::string &apn, Bearer &b )
{
   std::stringstream ss;

   ss << "DELETE FROM session_bearer WHERE "
      << "imsi = '" << imsi << "' AND "
      << "apn = '" << apn << "' AND "
      << "ebi = " << b.getEBI();

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
   }
}

bool DataAccess::_getSubscriber( const char *imsi, Subscriber &sub )
{
   bool result = true;
   bool b;
   std::string s;
   std::stringstream ss;

   ss << "SELECT * FROM subscriber WHERE imsi = '" << imsi << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      return false;
   }

   SCassResult res = future.result();

   SCassRow row = res.firstRow();

   if ( (result = row.valid()) )
   {
      GET_EVENT_DATA2( row, imsi, s, sub.setImsi );
      GET_EVENT_DATA2( row, sy_subscription_id, s, sub.setSySubscriptionId );
      GET_EVENT_DATA2( row, fail_on_uninstallable_rule, b, sub.setFailOnUninstallableRule );
   }

   return result;
}

bool DataAccess::_getSubscriberApns( const char *imsi, Subscriber &sub, RulesMap &rules )
{
   bool result = true;
   bool b;
   std::string s;
   std::vector<std::string> crv;
   RulesMap::iterator ruleit;
   std::stringstream ss;

   ss << "SELECT * FROM subscriber_apn WHERE imsi = '" << imsi << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      return false;
   }

   SCassResult res = future.result();

   SCassIterator rows = res.rows();

   while ( result && rows.nextRow() )
   {
      SCassRow row = rows.row();
      SubscriberApn *subapn = new SubscriberApn();

      try
      {
         GET_EVENT_DATA2( row, apn, s, subapn->setApn );
         GET_EVENT_DATA2( row, membership_value, s, subapn->setMembershipValue );
         GET_EVENT_DATA2( row, domain, s, subapn->setDomain );
         GET_EVENT_DATA2( row, sy_required, b, subapn->setSyRequired );
         GET_EVENT_DATA2( row, transfer_policy, s, subapn->setApn );

         GET_EVENT_DATA( row, computed_rules, s );

         crv = SUtility::split( s, ',' );

         for ( auto &rulename : crv )
         {
            if ( (ruleit = rules.find(rulename)) != rules.end() )
            {
               subapn->addComputedRule( ruleit->second );
            }
            else
            {
               // found the apn so must be a duplicate
               Logger::system().error( "DataAccess::%s:%d - rule [%s] not found while loading APN [%s]",
                     __func__, __LINE__, rulename.c_str(), rulename.c_str() );
               result = false;
               delete subapn;
               break;
            }
         }

         if ( !result )
            break;

         sub.addSubscriberApn( subapn );
//         sub.getApnPolicies().insert( std::pair<std::string,SubscriberApn*>( subapn->getApn(), subapn ) );
      }
      catch ( DAException &ex )
      {
         Logger::system().error( "DataAccess::%s:%d - DAException loading Subscriber APN's - %s",
               __func__, __LINE__, ex.what() );
         result = false;
         delete subapn;
         break;
      }
      catch ( ... )
      {
         Logger::system().error( "DataAccess::%s:%d - Unrecognized exception loading Subscriber APN's",
               __func__, __LINE__ );
         result = false;
         delete subapn;
         break;
      }
   }

   return result;
}

bool DataAccess::_addSubscriber( const Subscriber &s )
{
   bool result = true;
   std::stringstream ss;

   ss << "INSERT INTO subscriber ("
         << "imsi, sy_subscription_id, fail_on_uninstallable_rule"
      << ") VALUES ("
         << "'" << s.getImsi() << "',"
         << "'" << s.getSySubscriptionId() << "',"
         << "" << (s.getFailOnUninstallableRule() ? "true" : "false")
      << ")";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      result = false;
   }

   return result;
}


bool DataAccess::_addSubscriberApn( const char *imsi, const SubscriberApn &sa )
{
   bool result = true;
   std::stringstream ss;
   std::string cr;

   _concatenateRules( sa.getComputedRules(), cr );

   ss << "INSERT INTO subscriber_apn ("
         << "imsi, apn, membership_value, domain, computed_rules, sy_required, transfer_policy"
      << ") VALUES ("
         << "'" << imsi << "',"
         << "'" << sa.getApn() << "',"
         << "'" << sa.getMembershipValue() << "',"
         << "'" << sa.getDomain() << "',"
         << "'" << cr << "',"
         << "" << (sa.getSyRequired() ? "true" : "false") << ","
         << "'" << sa.getTransferPolicy() << "'"
      << ")";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
      result = false;
   }

   return result;
}

void DataAccess::_deleteSubscriber( const char *imsi )
{
   std::stringstream ss;

   ss << "DELETE FROM subscriber WHERE "
      << "imsi = '" << imsi << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
   }
}

void DataAccess::_deleteSubscriberApn( const char *imsi, const char *apn )
{
   std::stringstream ss;

   ss << "DELETE FROM subscriber_apn WHERE "
      << "imsi = '" << imsi << "' AND "
      << "apn = '" << apn << "'";

   SCassStatement stmt( ss.str().c_str() );

   SCassFuture future = m_db.execute( stmt );

   if ( future.errorCode() != CASS_OK )
   {
      Logger::system().error( "DataAccess::%s:%d - error %d executing [%s]",
            __func__, __LINE__, future.errorCode(), ss.str().c_str() );
   }
}

