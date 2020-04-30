/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <signal.h>
#include <iostream>

#include "fd.h"

#include "options.h"
#include "serror.h"

#include "cdf.h"

CDF g_cdf;

void handler(int signo, siginfo_t *pinfo, void *pcontext)
{
   //if ( processor.isRunning() )
   //{
      std::cout << "caught signal (" << signo << ") initiating shutdown" << std::endl;
      g_cdf.shutdown();
   //}
}

void initHandler()
{
   struct sigaction sa;
   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = handler;
   sigemptyset(&sa.sa_mask);
   int signo = SIGINT;
   if (sigaction(signo, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register SIGINT handler" );
   signo = SIGTERM;
   if (sigaction(signo, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register SIGTERM handler" );
}

int main(int argc, char **argv)
{
   if ( !Options::parse( argc, argv ) )
   {
      std::cout << "Options::parse() failed" << std::endl;
      return 1;
   }

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   std::cout << "Options::diameterConfiguration : " << Options::diameterConfiguration() << std::endl;

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   // initizlie the signal handler
   initHandler();

   // initialize and start CDF processing
   if ( !g_cdf.start() )
      return 1;

   std::cout << "Ready to process requests" << std::endl;

   // wait for the thread to finish
   g_cdf.waitForShutdown();

   return 0;
}
