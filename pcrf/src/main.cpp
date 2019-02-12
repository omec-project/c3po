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


#include <signal.h>
#include <iostream>

#include "fd.h"
#include "ssync.h"

#include "options.h"
#include "serror.h"
#include "logger.h"

#include "pcrf.h"
#include "statpcrf.h"

static PCRF pcrf;
static SEvent shutdownEvent;

void handler(int signo, siginfo_t *pinfo, void *pcontext)
{
   Logger::system().startup( "Caught signal (%d) setting shutdown event", signo );
   shutdownEvent.set();
}

void initHandler()
{
   struct sigaction sa;

   Logger::system().startup( "registering signal hander" );

   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = handler;
   sigemptyset(&sa.sa_mask);
   int signo = SIGINT;
   if (sigaction(signo, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register timer handler" );
}

int main(int argc, char **argv)
{
   int result = 0;

   if ( !Options::parse( argc, argv ) )
   {
      std::cout << "Options::parse() failed" << std::endl;
      return 1;
   }

   Logger::init( "pcrf" );
   StatsPcrf::initstats(&Logger::stat());

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   /*
   std::cout << "Options::originHost            : " << Options::originHost() << std::endl;
   std::cout << "Options::originRealm           : " << Options::originRealm() << std::endl;
   std::cout << "Options::diameterConfiguration : " << Options::diameterConfiguration() << std::endl;
   std::cout << "Options::cassandraHost         : " << Options::cassandraHost() << std::endl;
   std::cout << "Options::cassandraKeyspace     : " << Options::cassandraKeyspace() << std::endl;
   */

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   // initizlie the signal handler
   initHandler();

   // initialize and start the scef
   if ( pcrf.init() )
   {
      // wait for the shutdown trigger
      shutdownEvent.wait();
   }
   else
   {
      result = 1;
   }

   // initiate the shutdown
   Logger::system().startup( "exiting" );

   pcrf.uninit();

   // wait for pcrf to shutdown
   pcrf.waitForShutdown();

   Logger::flush();
   Logger::cleanup();
   Options::cleanup();

   return 0;
}
