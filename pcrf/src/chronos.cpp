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
