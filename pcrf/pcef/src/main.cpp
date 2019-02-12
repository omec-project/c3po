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

#include "pcef.h"

PCEF pcef;

void handler(int signo, siginfo_t *pinfo, void *pcontext)
{
   Logger::system().startup( "Caught signal (%d) shutting down", signo );
   pcef.uninit();
   pcef.waitForShutdown();
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
             << "  1) Send CCR Initial Request" << std::endl
             << "  2) Send CCR Termination Request" << std::endl
             << "  3) Start Repetitive CCR Initial/Termination Requests" << std::endl
             << "  4) Stop Repetitive CCR Initial/Termination Requests" << std::endl
             << " 99) Quit" << std::endl
             //<< " 3) Send 3 One-Time and 2 Long-Term MEC's in CIR" << std::endl
             //<< " 4) Send 1 One-Time and 1 Long-Term MEC's for External-Identifier" << std::endl
             //<< " 5) Send Remove last 5 Reference ID's CIR, single MEC" << std::endl
             //<< " 6) Send 1 Long-Term MEC's in CIR" << std::endl
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
            case 1: { pcef.gxApp().sendCRCRreq( 1 ); break; }
            case 2: { pcef.gxApp().sendCRCRreq( 3 ); break; }
            case 3: { pcef.setRepetitive( true ); pcef.gxApp().sendCRCRreq( 1 ); break; }
            case 4: { pcef.setRepetitive( false ); break; }
            case 99: { pcef.uninit(); break; }
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

   Logger::init( "pcef" );

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   // initizlie the signal handler
   initHandler();

   // initialize and start the scef
   if ( !pcef.init() )
      return 1;

   // wait for scef to shutdown
   menu();
   //pcef.waitForShutdown();

   Logger::system().startup( "exiting" );

   return 0;
}
