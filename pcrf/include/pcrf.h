/*
* Copyright (c) 2017 Sprint
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
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
