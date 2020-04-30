/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <freeDiameter/freeDiameter-host.h>

#include "options.h"

std::string Options::m_diameterconfiguration;

void Options::help()
{
   std::cout << std::endl
             << "Usage:  c3pocdf [OPTIONS]" << std::endl
             << "  -h, --help             Print help and exit" << std::endl
             << "  -f, --fdcfg filename   Read the freeDiameter configuration from this file" << std::endl
             << "                         instead of the default location (" DEFAULT_CONF_PATH "/" FD_DEFAULT_CONF_FILENAME ")." << std::endl
      ;
}

bool Options::parse( int argc, char **argv )
{
   int c;
   int option_index = 0;
   bool result = true;
   bool ffound = false;

   struct option long_options[] = {
      { "help",         no_argument,        NULL, 'h' },
      { "fdcfg",        required_argument,  NULL, 'f' },
      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "hf:", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'h': { help(); exit(0);                                         break; }
         case 'f': { m_diameterconfiguration = optarg;         ffound = true; break; }
         case '?':
         {
            switch ( optopt )
            {
               case 'f': { std::cout << "Option -f (freeDiameter configuration file) requires an argument" << std::endl; break; }
               default: { std::cout << "Unrecognized option [" << c << "]" << std::endl; break; }
            }
            result = false;
         }
         default:
         {
            std::cout << "Unrecognized option [" << c << "]" << std::endl;
            result = false;
         }
      }
   }

   if ( result )
   {
      if ( !ffound )
      {
         std::cout << "-f (diameter configuration file) option not found" << std::endl;
         result = false;
      }
   }

   return result;
}

