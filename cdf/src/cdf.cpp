/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <string>
#include <iostream>
#include <sstream>

#include "cdf.h"
#include "options.h"

CDF::CDF()
{
   m_rf = NULL;
}

CDF::~CDF()
{
}

bool CDF::start()
{
   bool result = true;

   try
   {
      // set the freeDiameter config file
      m_diameter.setConfigFile( Options::diameterConfiguration().c_str() );

      // start diameter
      std::cout << "Starting Diameter" << std::endl;
      result = m_diameter.init();

      if ( result )
      {
        // create the accounting application object
        std::cout << "Creating Appication object" << std::endl;
        m_rf = new rf::Application();

        // advertise support for the accounting application
        m_diameter.advertiseSupport( m_rf->getDict().app(), 0, 1 );
        result &= m_diameter.start();
       }
   }
   catch ( FDException &e )
   {
      std::cout << CDF::currentTime() << " - EXCEPTION - " << e.what() << std::endl;
      result = false;
   }

   return result;
}

void CDF::shutdown()
{
   m_diameter.uninit( false );
}

void CDF::waitForShutdown()
{
   m_diameter.waitForShutdown();
}

std::string CDF::currentTime()
{
   time_t t = time( NULL );
   struct tm *now = localtime( &t );
   std::stringstream ss;

   ss << (now->tm_year + 1900) << '-'
      << (now->tm_mon + 1) << '-'
      << (now->tm_mday) << ' '
      <<  now->tm_hour << ':'
      <<  now->tm_min << ':'
      <<  now->tm_sec;

   return ss.str();
}
