/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __ENDPOINT_H
#define __ENDPOINT_H

#include <string>
#include <map>

#include "ssync.h"

class Endpoint
{
public:
   Endpoint()
      : m_capabilities( 0 ),
        m_featuremask( 0 ),
        m_supported_feature_persistance( false )
   {
   }
   ~Endpoint() {}

   const std::string &getHost() const { return m_host; }
   const std::string &getRealm() const { return m_realm; }
   const std::string &getIp() const { return m_ip; }
   int64_t getCapabilities() const { return m_capabilities; }
   int64_t getFeatureMask() const { return m_featuremask; }
   bool getSupportedFeaturePersistance() const { return m_supported_feature_persistance; }
   const std::string &getAssignedTdf() const { return m_assigned_tdf; }
   const std::string &getAssignedTssf() const { return m_assigned_tssf; }

   const std::string &setHost( const char *v ) { m_host = v; return getHost(); }
   const std::string &setHost( const std::string &v ) { m_host = v; return getHost(); }
   const std::string &setRealm( const char *v ) { m_realm = v; return getRealm(); }
   const std::string &setRealm( const std::string &v ) { m_realm = v; return getRealm(); }
   const std::string &setIp( const char *v ) { m_ip = v; return getIp(); }
   const std::string &setIp( const std::string &v ) { m_ip = v; return getIp(); }
   int64_t setCapabilities( int64_t v ) { m_capabilities = v; return getCapabilities(); }
   int64_t setFeatureMask( int64_t v ) { m_featuremask = v; return getFeatureMask(); }
   bool setSupportedFeaturePersistance( bool v ) { m_supported_feature_persistance = v; return getSupportedFeaturePersistance(); }
   const std::string &setAssignedTdf( const char *v ) { m_assigned_tdf = v; return getAssignedTdf(); }
   const std::string &setAssignedTdf( const std::string &v ) { m_assigned_tdf = v; return getAssignedTdf(); }
   const std::string &setAssignedTssf( const char *v ) { m_assigned_tssf = v; return getAssignedTssf(); }
   const std::string &setAssignedTssf( const std::string &v ) { m_assigned_tssf = v; return getAssignedTssf(); }

private:
   std::string m_host;
   std::string m_realm;
   std::string m_ip;
   int64_t m_capabilities;
   int64_t m_featuremask;
   bool m_supported_feature_persistance;
   std::string m_assigned_tdf;
   std::string m_assigned_tssf;
};

class EndpointMap : public std::map<std::string,Endpoint*>
{
public:
   EndpointMap();
   ~EndpointMap();

   bool getEndpoint( const std::string &endpoint, Endpoint* &pendpoint );
   bool addEndpoint( Endpoint *pendpoint );

private:
   SMutex m_mutex;
};

#endif // __ENDPOINT_H
