/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
