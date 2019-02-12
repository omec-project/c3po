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

#include <iostream>

#include "hssperf.h"
#include "sutility.h"

#include "options.h"
#include "logger.h"

HSSPerformance::HSSPerformance()
   : m_s6as6d( NULL )
{
}

HSSPerformance::~HSSPerformance()
{
}

bool HSSPerformance::init()
{
   // set the diameter configuration file
   m_diameter.setConfigFile( Options::diameterConfiguration() );

   // initialize diameter
   if ( !m_diameter.init() )
      return false;

   try
   {
      m_s6as6d = new s6as6d::Application();
      FDDictionaryEntryVendor vnd3gpp( m_s6as6d->getDict().app() );
      m_diameter.advertiseSupport( m_s6as6d->getDict().app(), vnd3gpp, 1, 0 );
      Logger::s6as6d().startup( "%s:%d - interface initialized", __FILE__, __LINE__ );
   }
   catch ( FDException &e )
   {
      Logger::s6as6d().startup( "%s:%d - FDException initializing interface - %s", __FILE__, __LINE__, e.what() );
      return false;
   }

   return m_diameter.start();
}

void HSSPerformance::uninit()
{
   if ( m_s6as6d )
   {
      Logger::s6as6d().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      delete m_s6as6d;
   }

   m_diameter.uninit();
}

void HSSPerformance::waitForShutdown()
{
   m_diameter.waitForShutdown();
}
