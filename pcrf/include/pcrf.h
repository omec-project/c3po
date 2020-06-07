/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __PCRF_H
#define __PCRF_H

#include <pistache/endpoint.h>

#include "gx_impl.h"
#include "rx_impl.h"
#include "sd_impl.h"
#include "st_impl.h"

#include "dataaccess.h"

#include "rule.h"
#include "apn.h"
#include "endpoint.h"
#include "session.h"
#include "soss.h"
#include "logger.h"

class PCRF
{
public:
   PCRF();
   ~PCRF();

   bool init();
   void uninit();

   void waitForShutdown();

   DataAccess &dataaccess();

   gx::Application &gxApp() { return *m_gx; }
   rx::Application &rxApp() { return *m_rx; }
   sd::Application &sdApp() { return *m_sd; }
   st::Application &stApp() { return *m_st; }

   bool getRule( const std::string &rulename, Rule* &rule ) { return m_rules.getRule( rulename, rule ); }
   bool getRule( const char *rulename, Rule* &rule ) { std::string s(rulename); return m_rules.getRule( s, rule ); }

   bool getApn( const std::string &apn, Apn* &papn ) { return m_apns.getApn( apn, papn ); }
   bool getApn( const char *apn, Apn* &papn ) { std::string s(apn); return m_apns.getApn( s, papn ); }

   bool getEndpoint( const std::string &endpoint, Endpoint* &pendpoint ) { return m_endpoints.getEndpoint( endpoint, pendpoint ); }
   bool getEndpoint( const char *endpoint, Endpoint* &pendpoint ) { std::string s(endpoint); return m_endpoints.getEndpoint( s, pendpoint ); }
   bool addEndpoint( Endpoint *pendpoint ) { return m_endpoints.addEndpoint( pendpoint ); }

   /*
   bool sessionExists( GxSession *session ) { return m_sessions.exists( session ); }
   bool addSession( GxSession *session );
   bool getSession( const std::string &imsi, const std::string &apn, GxSession* &session ) { return m_sessions.findSession( imsi, apn, session ); }
   bool getSession( const char *imsi, const char *apn, GxSession* &session ) { std::string i(imsi), a(apn); return m_sessions.findSession( i, a, session ); }
   void deleteSession( const std::string &imsi, const std::string &apn );
   */

   RulesMap &getRules() { return m_rules; }

private:
   FDEngine m_diameter;

   gx::Application *m_gx;
   rx::Application *m_rx;
   sd::Application *m_sd;
   st::Application *m_st;
   Pistache::Http::Endpoint *m_chronos;

   DataAccess m_dataaccess;

   RulesMap m_rules;
   ApnMap m_apns;
   EndpointMap m_endpoints;
   OssEndpoint<Logger> *m_ossendpoint;
};

#endif // #define __PCRF_H
