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

#include "sutility.h"

#include "sms.h"
#include "options.h"
#include "logger.h"

SMSRouter::SMSRouter()
   : m_s6c( NULL )
{
}

SMSRouter::~SMSRouter()
{
}

bool SMSRouter::init()
{
   // set the diameter configuration file
   m_diameter.setConfigFile( Options::diameterConfiguration() );

   // initialize diameter
   if ( !m_diameter.init() )
      return false;

   try
   {
      m_s6c = new s6c::Application();
      FDDictionaryEntryVendor vnd3gpp( m_s6c->getDict().app() );
      m_diameter.advertiseSupport( m_s6c->getDict().app(), vnd3gpp, 1, 0 );
      Logger::s6c().startup( "%s:%d - interface initialized", __FILE__, __LINE__ );
   }
   catch ( FDException &e )
   {
      Logger::s6c().startup( "%s:%d - FDException initializing interface - %s", __FILE__, __LINE__, e.what() );
      return false;
   }

   try
   {
      m_sgd = new sgd::Application();
      FDDictionaryEntryVendor vnd3gpp( m_sgd->getDict().app() );
      m_diameter.advertiseSupport( m_sgd->getDict().app(), vnd3gpp, 1, 0 );
      Logger::sgd().startup( "%s:%d - interface initialized", __FILE__, __LINE__ );
   }
   catch ( FDException &e )
   {
      Logger::sgd().startup( "%s:%d - FDException initializing interface - %s", __FILE__, __LINE__, e.what() );
      return false;
   }

//   try
//   {
//      m_scs = new SCS( *this );
//      Logger::scs_startup( SUtility::string_format( "%s:%d - interface initialized", __FILE__, __LINE__ ) );
//     // std::cout << "SCS interface sccessfully initialized" << std::endl;
//   }
//   catch ( FDException &e )
//   {
//      Logger::scs_startup( SUtility::string_format( "%s:%d - FDException initializing interface - %s", __FILE__, __LINE__, e.what() ) );
//      //std::cout << "EXCEPTION initializing s6ms6n interface - " << e.what() << std::endl;
//      return false;
//   }

   return m_diameter.start();
}

void SMSRouter::uninit()
{
   if ( m_s6c )
   {
      Logger::s6c().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      delete m_s6c;
   }

   if ( m_sgd )
   {
      Logger::sgd().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      delete m_sgd;
   }

   m_diameter.uninit();
}

void SMSRouter::waitForShutdown()
{
   m_diameter.waitForShutdown();
}
