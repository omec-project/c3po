/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
