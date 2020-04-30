/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <signal.h>
#include <iostream>

#include "hssperf.h"
#include "fd.h"

#include "options.h"
#include "serror.h"
#include "logger.h"

#include "ulrtest.h"
#include "airtest.h"
#include "attachtest.h"

HSSPerformance hssperf;
ULRTest ulrtest(hssperf);

ULRTestThread ulrtestthread(hssperf);
AIRTestThread airtestthread(hssperf);
AttachTestThread attachtestthread(hssperf);

std::string gHost = "hss.openair4G.eur";
std::string gRealm = "openair4G.eur";
std::string gImsi = "001014567891234";
std::string gVPlmnId = "00101";
uint8_t vplmnid[3];
int gCount = 10000;
int gOutstanding = 200;
int gSendCount = 10;
long gSendMs = 10;
bool gShutdown = false;

#define ENC_PLMN_ID(_plmnid, _chrbuf, _len)                       \
{                                                                 \
   _plmnid[0] = ((_chrbuf[1] - '0') << 4) | (_chrbuf[0] - '0');   \
   if (_len == 5)                                                 \
   {                                                              \
      _plmnid[1] = ((0xf0) | (_chrbuf[2] - '0'));                 \
   }                                                              \
   else                                                           \
   {                                                              \
      _plmnid[1] = ((_chrbuf[5] - '0') << 4) | (_chrbuf[2] - '0');\
   }                                                              \
   _plmnid[2] = ((_chrbuf[4] - '0') << 4) | (_chrbuf[3] - '0');   \
}

void handler(int signo, siginfo_t *pinfo, void *pcontext)
{
   Logger::system().startup( "Caught signal (%d) shutting down", signo );
   gShutdown= true;
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
      SError::throwRuntimeExceptionWithErrno( "Unable to register SIGINT handler" );

   signo = SIGTERM;
   if (sigaction(signo, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register SIGTERM handler" );
}

void displaymenu()
{
   std::cout << std::endl << std::endl
             << "**********  HSSPERF MENU  **********" << std::endl
             << "  1) Start ULR test" << std::endl
             << "  2) Start AIR test" << std::endl
             << "  3) Start Attach test" << std::endl
             << " 99) Quit" << std::endl
             ;
}

void startULRtest()
{
   std::string input;

   std::cin.ignore();

   std::cout << "HSS Diameter Host [" << gHost << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gHost = input;

   std::cout << "HSS Diameter Realm [" << gRealm << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gRealm = input;

   std::cout << "First IMSI [" << gImsi << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gImsi = input;

   std::cout << "Visited PLMN Id [" << gVPlmnId << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gVPlmnId = input;
   ENC_PLMN_ID(vplmnid, gVPlmnId.c_str(), gVPlmnId.length());

   while (1)
   {
      std::cout << "Total Number of Transactions [" << gCount << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         int val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gCount = val;
      }
      break;
   }

   while (1)
   {
      std::cout << "Max Outstanding Transactions [" << gOutstanding << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         int val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gOutstanding = val;
      }
      break;
   }

   ulrtest.startTest(gImsi.c_str(), vplmnid, gHost.c_str(), gRealm.c_str(), gOutstanding, gCount);
}

void startULRtest2()
{
   std::string input;

   std::cin.ignore();

   std::cout << "HSS Diameter Host [" << gHost << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gHost = input;

   std::cout << "HSS Diameter Realm [" << gRealm << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gRealm = input;

   std::cout << "First IMSI [" << gImsi << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gImsi = input;

   std::cout << "Visited PLMN Id [" << gVPlmnId << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gVPlmnId = input;
   ENC_PLMN_ID(vplmnid, gVPlmnId.c_str(), gVPlmnId.length());

   while (1)
   {
      std::cout << "Total Number of Transactions [" << gCount << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         int val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gCount = val;
      }
      break;
   }

   while (1)
   {
      std::cout << "Batch send size [" << gSendCount << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         int val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gSendCount = val;
      }
      break;
   }

   while (1)
   {
      std::cout << "Batch send frequency (ms) [" << gSendMs << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         long val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gSendMs = val;
      }
      break;
   }

   SSemaphore testsem(0,1);
   ulrtestthread.postMessage(new ULRTestStartMessage(gImsi, vplmnid, gHost, gRealm, gCount, gSendCount, gSendMs, &testsem));
   testsem.decrement();
}

void startAIRtest()
{
   std::string input;

   std::cin.ignore();

   std::cout << "HSS Diameter Host [" << gHost << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gHost = input;

   std::cout << "HSS Diameter Realm [" << gRealm << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gRealm = input;

   std::cout << "First IMSI [" << gImsi << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gImsi = input;

   std::cout << "Visited PLMN Id [" << gVPlmnId << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gVPlmnId = input;
   ENC_PLMN_ID(vplmnid, gVPlmnId.c_str(), gVPlmnId.length());

   while (1)
   {
      std::cout << "Total Number of Transactions [" << gCount << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         int val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gCount = val;
      }
      break;
   }

   while (1)
   {
      std::cout << "Batch send size [" << gSendCount << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         int val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gSendCount = val;
      }
      break;
   }

   while (1)
   {
      std::cout << "Batch send frequency (ms) [" << gSendMs << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         long val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gSendMs = val;
      }
      break;
   }

   SSemaphore testsem(0,1);
   airtestthread.postMessage(new AIRTestStartMessage(gImsi, vplmnid, gHost, gRealm, gCount, gSendCount, gSendMs, &testsem));
   testsem.decrement();
}

void startAttachTest()
{
   std::string input;

   std::cin.ignore();

   std::cout << "HSS Diameter Host [" << gHost << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gHost = input;

   std::cout << "HSS Diameter Realm [" << gRealm << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gRealm = input;

   std::cout << "First IMSI [" << gImsi << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gImsi = input;

   std::cout << "Visited PLMN Id [" << gVPlmnId << "] - ";
   std::getline(std::cin, input);
   if (!input.empty())
      gVPlmnId = input;
   ENC_PLMN_ID(vplmnid, gVPlmnId.c_str(), gVPlmnId.length());

   while (1)
   {
      std::cout << "Total Number of Transactions [" << gCount << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         int val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gCount = val;
      }
      break;
   }

   while (1)
   {
      std::cout << "Batch send size [" << gSendCount << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         int val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gSendCount = val;
      }
      break;
   }

   while (1)
   {
      std::cout << "Batch send frequency (ms) [" << gSendMs << "] - ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
         long val;
         std::stringstream ms(input);
         if (!(ms >> val))
         {
            std::cout << "*** Invalid number, please try again ***" << std::endl;
            continue;
         }
         gSendMs = val;
      }
      break;
   }

   SSemaphore testsem(0,1);
   attachtestthread.postMessage(new AttachTestStartMessage(gImsi, vplmnid, gHost, gRealm, gCount, gSendCount, gSendMs, &testsem));
   testsem.decrement();
}

void menu()
{
   int choice;

   for(;;)
   {
      if (gShutdown)
         break;

      displaymenu();

      std::string line;

      std::cin.clear();
      if ( std::cin >> choice )
      {
         switch ( choice )
         {
            case 1: { startULRtest2(); break; }
            case 2: { startAIRtest(); break; }
            case 3: { startAttachTest(); break; }
            case 99: { break; }
            default:
            {
               std::cout << "Unrecognized menu option" << std::endl;
            }
         }

         if ( choice == 99 )
            break;
      }
   }
}

int main(int argc, char **argv)
{
   if ( !Options::parse( argc, argv ) )
   {
      std::cout << "Options::parse() failed" << std::endl;
      return 1;
   }

   Logger::init( "hssperf" );

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   // initizlie the signal handler
   initHandler();

   // initialize and start hssperf
   if ( !hssperf.init() )
      return 1;

   ulrtestthread.init(NULL);
   airtestthread.init(NULL);
   attachtestthread.init(NULL);

   // wait for hssperf to shutdown
   menu();

   hssperf.uninit();
   hssperf.waitForShutdown();

   ulrtestthread.quit();
   airtestthread.quit();
   attachtestthread.quit();

   ulrtestthread.join();
   airtestthread.join();
   attachtestthread.join();

   Logger::system().startup( "exiting" );

   return 0;
}
