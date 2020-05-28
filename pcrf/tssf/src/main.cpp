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
#include "logger.h"

#include "tssf.h"

TSSF tssf;

void handler(int signo, siginfo_t *pinfo, void *pcontext)
{
   Logger::system().startup( "Caught signal (%d) shutting down", signo );
   tssf.uninit();
   tssf.waitForShutdown();
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
             << "**********  TSSF MENU  **********" << std::endl
             //<< "  1) Send CCR Initial Request" << std::endl
             //<< "  2) Send CCR Termination Request" << std::endl
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
            //case 1: { pcef.gxApp().sendCRCRreq( 1 ); break; }
            //case 2: { pcef.gxApp().sendCRCRreq( 3 ); break; }
            case 99: { tssf.uninit(); break; }
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

   Logger::init( "tssf" );

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   // initizlie the signal handler
   initHandler();

   // initialize and start the scef
   if ( !tssf.init() )
      return 1;

   // wait for scef to shutdown
   menu();
   //tssf.waitForShutdown();

   Logger::system().startup( "exiting" );

   return 0;
}
