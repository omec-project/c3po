/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include "serror.h"

#include <pistache/endpoint.h>

#include "ssync.h"

#include "s6t_impl.h"
#include "fdhss.h"
#include "options.h"
#include "logger.h"
#include "resthandler.h"

extern "C" {
   #include "hss_config.h"
   #include "aucpp.h"
   #include "auc.h"
}

hss_config_t hss_config;
FDHss fdHss;
static bool shutdownFlag = false;
static SEvent shutdownEvent;
#ifdef MONITOR_PENDING_MESSAGE_LEVEL
static timer_t timer;
#endif

#ifdef PERFORMANCE_TIMING
void dumpUlrTimers();
#endif

void handler(int signal)
{
   if (signal == SIGRTMIN+1)
   {
      size_t cnt = fdHss.getWorkerQueue().queueDepth();

      if (cnt > 0)
         printf("pending messages %ld\n", cnt);
   }
   else
   {
      Logger::system().startup( "Caught signal (%d)", signal );

      switch (signal)
      {
         case SIGINT:
         case SIGTERM:
         {
            Logger::system().startup( "Setting shutdown event" );
            shutdownFlag = true;
            shutdownEvent.set();
            break;
         }
#ifdef PERFORMACE_TIMING
         case SIGUSR1:
         {
            dumpUlrTimers();
            break;
         }
#endif
      }
   }
}

void initHandler()
{
   struct sigaction sa;

   /* Setup the signal handler */
   sa.sa_handler = handler;

   /* Restart the system call, if at all possible */
   sa.sa_flags = SA_RESTART;

   /* Block every signal during the handler */
   sigfillset(&sa.sa_mask);

   if (sigaction(SIGINT, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register SIGINT handler" );
   Logger::system().startup( "signal handler registered for SIGINT" );

   if (sigaction(SIGTERM, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register SIGTERM handler" );
   Logger::system().startup( "signal handler registered for SIGTERM" );

   if (sigaction(SIGRTMIN+1, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register SIGRTMIN handler" );
   Logger::system().startup( "signal handler registered for SIGRTMIN" );

#ifdef PERFORMANCE_TIMING
   if (sigaction(SIGUSR1, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register SIGUSR1 handler" );
   Logger::system().startup( "signal handler registered for SIGUSR1" );
#endif
}

#include "util.h"

int main(int argc, char **argv)
{

   if ( !Options::parse( argc, argv ) )
   {
      std::cout << "Options::parse() failed" << std::endl;
      return 1;
   }

   Logger::init( "hss" );
   StatsHss::initstats(&Logger::singleton().stat());

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   /*
   std::cout << "Options::jsonConfig            : " << Options::getjsonConfig()            << std::endl;
   std::cout << "Options::diameterconfiguration : " << Options::getdiameterconfiguration() << std::endl;
   std::cout << "Options::cassserver            : " << Options::getcassserver()            << std::endl;
   std::cout << "Options::cassuser              : " << Options::getcassuser()              << std::endl;
   std::cout << "Options::casspwd               : " << Options::getcasspwd()               << std::endl;
   std::cout << "Options::cassdb                : " << Options::getcassdb()                << std::endl;
   std::cout << "Options::randvector            : " << Options::getrandvector()            << std::endl;
   std::cout << "Options::optkey                : " << Options::getoptkey()                << std::endl;
   std::cout << "Options::reloadkey             : " << Options::getreloadkey()             << std::endl;
   std::cout << "Options::onlyloadkey           : " << Options::getonlyloadkey()           << std::endl;
   std::cout << "Options::gtwport               : " << Options::getgtwport()               << std::endl;
   std::cout << "Options::gtwhost               : " << Options::getgtwhost()               << std::endl;
   std::cout << "Options::resthost              : " << Options::getrestport()              << std::endl;
   std::cout << "Options::synchimsi             : " << Options::getsynchimsi()             << std::endl;
   std::cout << "Options::synchauts             : " << Options::getsynchauts()             << std::endl;
   */

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   initHandler();

   //Fill the hss_config to be used by c sec
   memset (&hss_config, 0, sizeof (hss_config_t));
   Options::fillhssconfig(&hss_config);

   fdHss.getWorkMgr().init(Options::getnumworkers());

   random_init();

   fdHss.initdb(&hss_config);

   if( Options::getonlyloadkey() ){
      fdHss.updateOpcKeys( (uint8_t *) hss_config.operator_key_bin );
      return 0;
   }

   if( Options::getreloadkey() ){
      fdHss.updateOpcKeys( (uint8_t *) hss_config.operator_key_bin );
   }

   if ( !Options::getsynchimsi().empty() && !Options::getsynchauts().empty() ) {
      fdHss.synchFix();
      return 0;
   }

#ifdef MONITOR_PENDING_MESSAGE_LEVEL
   {
      struct sigevent sev;
      sev.sigev_notify = SIGEV_SIGNAL;
      sev.sigev_signo = SIGRTMIN+1;
      timer_create(CLOCK_REALTIME, &sev, &timer);

      struct itimerspec its;
      its.it_value.tv_sec = 1; // seconds
      its.it_value.tv_nsec = 0; // nano-seconds
      its.it_interval.tv_sec = its.it_value.tv_sec;
      its.it_interval.tv_nsec = its.it_value.tv_nsec;
      timer_settime(timer, 0, &its, NULL);
   }
#endif

   if ( fdHss.init(&hss_config) )
      shutdownEvent.wait();

   // initiate the shutdown
   Logger::system().startup( "exiting" );

   fdHss.shutdown();

   fdHss.waitForShutdown();

   Logger::flush();
   Logger::cleanup();

   return 0;
}
