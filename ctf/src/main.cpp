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
#include "csvproc.h"
#include "statsctf.h"
#include "serror.h"

#include "tests.h"
#include "ssyslog.h"
#include "soss.h"

#include "logger.h"
#include "sslcommhndlr.h"

CSVProcessor processor;
OssEndpoint<Logger>* ossendpoint;

void handler(int signo, siginfo_t *pinfo, void *pcontext)
{
   if ( processor.isRunning() )
   {
      std::cout << "caught signal (" << signo << ") posting quit message" << std::endl;
      processor.quit();
   }
   if ( StatsCtf::singleton().isRunning() )
   {
      std::cout << "caught signal (" << signo << ") posting quit message" << std::endl;
      StatsCtf::singleton().quit();
   }

}

void initHandler()
{
   struct sigaction sa;
   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = handler;
   sigemptyset(&sa.sa_mask);
   int signo = SIGINT;
   if (sigaction(signo, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register timer handler" );
}

int main(int argc, char **argv)
{
   if ( !Options::parse( argc, argv ) )
   {
      std::cout << "Options::parse() failed" << std::endl;
      return 1;
   }

   Logger::init( "ctf" );
   Logger::system().startup( "Starting ctf" );
   StatsCtf::initstats(&Logger::singleton().stat());

   //
   // starts the stats
   //
   StatsCtf::singleton().setInterval(Options::statsFrequency());
   StatsCtf::singleton().init( NULL );

   Pistache::Address addrOss( Pistache::Ipv4::any(), Pistache::Port(Options::getossport()) );
   ossendpoint = new OssEndpoint<Logger>(addrOss, &StatsCtf::singleton(), &Logger::singleton().audit(), &Logger::singleton(), Options::getossfile());
   ossendpoint->init();
   ossendpoint->start();

   Logger::system().startup( "Started OSS  server on port [%i]",   Options::getossport());

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////
   std::cout << "Options::jsonConfig            : " << Options::jsonConfig() << std::endl;
   std::cout << "Options::dataPrefix            : " << Options::dataPrefix() << std::endl;
   std::cout << "Options::dataPath              : " << Options::dataPath() << std::endl;
   std::cout << "Options::archivePath           : " << Options::archivePath() << std::endl;
   std::cout << "Options::trackingPath          : " << Options::trackingPath() << std::endl;
   std::cout << "Options::currentExtension      : " << Options::currentExtension() << std::endl;
   std::cout << "Options::skipRows              : " << Options::skipRows() << std::endl;
   std::cout << "Options::idleDuration          : " << Options::idleDuration() << std::endl;
   std::cout << "Options::maxACRs               : " << Options::maxACRs() << std::endl;
   std::cout << "Options::acaSaveFrequency      : " << Options::acaSaveFrequency() << std::endl;
   std::cout << "Options::trackingExtension     : " << Options::trackingExtension() << std::endl;
   std::cout << "Options::diameterConfiguration : " << Options::diameterConfiguration() << std::endl;
   std::cout << "Options::memcachedServer       : " << Options::memcachedServer() << std::endl;
   std::cout << "Options::memcachedPort         : " << Options::memcachedPort() << std::endl;
   std::cout << "Options::originHost            : " << Options::originHost() << std::endl;
   std::cout << "Options::originRealm           : " << Options::originRealm() << std::endl;
   std::cout << "Options::statsFrequency        : " << Options::statsFrequency() << std::endl;

   std::cout << "Options::runTests              : " << Options::runTests() << std::endl;

   std::cout << "Options::sgxserverIp           : " << Options::sgxserverIp() << std::endl;
   std::cout << "Options::sgxserverPort         : " << Options::sgxserverPort() << std::endl;
   std::cout << "Options::privkeyfile           : " << Options::privkeyFile() << std::endl;
   std::cout << "Options::certfile              : " << Options::certFile() << std::endl;
   std::cout << "Options::mrSigner              : " << Options::mrSigner() << std::endl;
   std::cout << "Options::mrEnclave             : " << Options::mrEnclave() << std::endl;
   std::cout << "Options::isvSvn                : " << Options::isvSvn() << std::endl;

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   // initizlie the signal handler
   initHandler();

   // initialize and start the processor thread
   processor.init( NULL );

#ifndef SGX_CDR_LIVE
   if ( Options::runTests() )
   {
      {
         FDEngine fd;
         fd.setConfigFile("conf/ctf.conf");

         if ( fd.init() )
         {
            rf::Application app;
            fd.advertiseSupport( app.getDict().app(), 0, 1 );
            Tests::fdTest1(app);
            fd.uninit();
         }
      }

      std::cout << "*****************************" << std::endl << "Tests::syslogTest()" << std::endl;
      Tests::syslogTest();
      std::cout << "*****************************" << std::endl << "Tests::dateTimeTest()" << std::endl;
      Tests::dateTimeTest();
      std::cout << "*****************************" << std::endl << "Tests::timerTest()" << std::endl;
      Tests::timerTest();
      std::cout << "*****************************" << std::endl << "Tests::trackTest()" << std::endl;
      Tests::trackTest();
      std::cout << "*****************************" << std::endl << "Tests::splitTest()" << std::endl;
      Tests::splitTest();
      std::cout << "*****************************" << std::endl << "Tests::queueTest()" << std::endl;
      Tests::queueTest();
      std::cout << "*****************************" << std::endl << "Tests::threadest()" << std::endl;
      Tests::threadTest();
      std::cout << "*****************************" << std::endl << "Tests::directoryTest()" << std::endl;
      Tests::directoryTest();
      std::cout << "*****************************" << std::endl << "Tests::pathTest()" << std::endl;
      Tests::pathTest();
      std::cout << "*****************************" << std::endl << "Tests::csvTest()" << std::endl;
      Tests::csvTest();

      return 0;
   }

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////
#else

   bool modeOffline = false;

   if (Options::streamName().c_str()[0] == '1')
      modeOffline = true;

   do {
         try
         {
            SSLCommHndlr client(Options::sgxserverIp(),
                  Options::sgxserverPort(), Options::certFile(),
                  Options::privkeyFile());

            //TODO: SGX integration red flag. Revisit live mode design.
            client.readSgxSSL();
         }
         catch (std::exception const& e)
         {
            std::cout << "Exception : " << e.what() << std::endl;
         }
   } while(modeOffline);
#endif

   StatsCtf::singleton().join();

   // wait for the thread to finish
   processor.join();

   delete ossendpoint;

   return 0;
}
