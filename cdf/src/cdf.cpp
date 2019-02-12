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
