/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <iostream>

#include "sutility.h"

#include "tdf.h"
#include "options.h"
#include "logger.h"

TDF::TDF()
{
}

TDF::~TDF()
{
}

bool TDF::init()
{
   // set the diameter configuration file
   m_diameter.setConfigFile( Options::diameterConfiguration() );

   // initialize diameter
   if ( !m_diameter.init() )
      return false;

   try
   {
      m_sd = new sd::Application( *this );
      FDDictionaryEntryVendor vnd3gpp( m_sd->getDict().app() );
      m_diameter.advertiseSupport( m_sd->getDict().app(), vnd3gpp, 1, 0 );
      Logger::sd().startup( "%s:%d - interface initialized", __FILE__, __LINE__ );
   }
   catch ( FDException &e )
   {
      Logger::sd().startup( "%s:%d - FDException initializing interface - %s", __FILE__, __LINE__, e.what() );
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

void TDF::uninit()
{
   if ( m_sd )
   {
      Logger::sd().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      delete m_sd;
   }

   m_diameter.uninit();
}

void TDF::waitForShutdown()
{
   m_diameter.waitForShutdown();
}
