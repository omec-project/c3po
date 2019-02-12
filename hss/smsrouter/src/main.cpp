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

#include "options.h"
#include "serror.h"
#include "logger.h"

#include "sms.h"

SMSRouter smsrouter;

void handler(int signo, siginfo_t *pinfo, void *pcontext)
{
   Logger::system().startup( "Caught signal (%d) shutting down", signo );
   smsrouter.uninit();
   smsrouter.waitForShutdown();
   exit(0);
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

void displaymenu()
{
   std::cout << std::endl << std::endl
             << "**********  PCEF MENU  **********" << std::endl
             << "  1) Send SRR with MSISDN only" << std::endl
             << "  2) Send SRR with IMSI only" << std::endl
             << "  3) Send SRR with MSISDN & IMSI" << std::endl
             << " 99) Quit" << std::endl
             ;
}

void menu()
{
   int choice;

   for(;;)
   {
      displaymenu();

      std::string line;

      std::cin.clear();
      if ( std::cin >> choice )
      {
         switch ( choice )
         {
            case 1: { smsrouter.s6cApp().sendSERIFSRreq( true, false ); break; }
            case 2: { smsrouter.s6cApp().sendSERIFSRreq( false, true ); break; }
            case 3: { smsrouter.s6cApp().sendSERIFSRreq( true, true ); break; }
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

   Logger::init( "smsrouter" );

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   // initizlie the signal handler
   initHandler();

   // initialize and start the scef
   if ( !smsrouter.init() )
      return 1;

   // wait for scef to shutdown
   menu();

   smsrouter.uninit();
   smsrouter.waitForShutdown();

   Logger::system().startup( "exiting" );

   return 0;
}
