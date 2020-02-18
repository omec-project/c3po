/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "chronos.h"
#include "logger.h"
#include "rule.h"

void ChronosHandler::onRequest( const Pistache::Http::Request &request, Pistache::Http::ResponseWriter response )
{
   Logger::chronos().debug( "%s:%d - ChronosHandler::onRequest() - received request [%s] body [%s]",
         __FILE__, __LINE__, request.resource().c_str(), request.body().c_str() );

   if ( request.resource() == "/ruleTimer")
   {
      response.send( Pistache::Http::Code::Ok, "" );

      RuleTimer *rt = RuleTimers::singleton().getRuleTimer( request.body() );

      if ( rt )
      {
         try
         {
            rt->setNextInterval();
         }
         catch ( std::runtime_error &e )
         {
            Logger::chronos().error( "%s:%d - ChronosHandler::onRequest() - RuleTimer for [%s] setNextInterval() threw exception - %s"
                  __FILE__, __LINE__, rt->getRule()->getRuleName().c_str(), e.what() );
         }

         try
         {
            rt->processIntervalExpiration();
         }
         catch ( std::runtime_error &e )
         {
            Logger::chronos().error( "%s:%d - ChronosHandler::onRequest() - RuleTimer for [%s] processIntervalExpiration() threw exception - %s"
                  __FILE__, __LINE__, rt->getRule()->getRuleName().c_str(), e.what() );
         }
      }
      else
      {
         Logger::chronos().warn( "%s:%d - ChronosHandler::onRequest() - unable to find rule [%s]",
               __FILE__, __LINE__, request.body().c_str() );
      }
   }
   else
   {
      response.send( Pistache::Http::Code::Bad_Request );
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Pistache::Http::Client Chronos::m_client;

//
// It seems that the Pistache::Http::Client is buggy.  If problems occur,
// it may be necessary to move to a different http client library.
//

void Chronos::init()
{
   auto opts = Pistache::Http::Client::options()
       .threads(1)
       .maxConnectionsPerHost(8);

   client().init( opts );
}

void Chronos::uninit()
{
   client().shutdown();
}
