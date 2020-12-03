/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <memory>
#include <iostream>
#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdproto.h"
#include "freeDiameter/libfdcore.h"

#include <pistache/endpoint.h>
#include <pistache/net.h>

#include "sutility.h"

#include "pcrf.h"

#include "../include/msg_event.h"
#include "options.h"
#include "logger.h"
#include "chronos.h"
#include "statpcrf.h"

PCRF::PCRF()
{
}

PCRF::~PCRF()
{
}

int gx_peer_validate ( struct peer_info *info, int *auth, int (**cb2) (struct peer_info *)){

    if (info == NULL) {
      return EINVAL;
    }

    *auth = 1;
    /*
     * For now we don't use security
     */
    info->config.pic_flags.sec = PI_SEC_NONE;
    info->config.pic_flags.persist = PI_PRST_NONE;
    printf ( "Accepting %s peer\n", info->pi_diamid);
    return 0;
}

bool PCRF::init()
{
   //
   // init the chronos client
   //
   Chronos::init();

   //
   // start the chronos timer callback handler
   //
   try
   {
      Pistache::Address addr( Pistache::Ipv4::any(), Pistache::Port(9081) );
      auto opts = Pistache::Http::Endpoint::options()
         .threads(1)
         .flags( Pistache::Tcp::Options::ReuseAddr );

      m_chronos = new Pistache::Http::Endpoint( addr );
      m_chronos->init( opts );
      m_chronos->setHandler( Pistache::Http::make_handler<ChronosHandler>() );
      m_chronos->serveThreaded();

      Logger::chronos().startup( "%s:%d - interface initialized", __FILE__, __LINE__ );

   }
   catch ( std::runtime_error &e )
   {
      Logger::chronos().startup( "%s:%d - Exception initializing interface - %s", __FILE__, __LINE__, e.what() );
      return false;
   }

   //
   // open the database connection
   //
   m_dataaccess.host( Options::cassandraHost() );
   m_dataaccess.keyspace( Options::cassandraKeyspace() );

   try
   {
      m_dataaccess.connect();
   }
   catch ( DAException &ex )
   {
      Logger::system().error( "%s:%d - DAException - %s", __FILE__, __LINE__, ex.what() );
      return false;
   }

   //
   // load the rules into memory
   //
   if ( !m_dataaccess.getRules( m_rules ) )
	{
      return false;
	}

	for ( auto r : m_rules )
	{
		printf ("SOHAN : RULES NAME : %s\n", (r.second)->getRuleName().c_str());	
	}

   if ( Options::enableRuleTimers() )
   {
      for ( auto r : m_rules )
      {
         RuleTimer *rt = RuleTimers::singleton().addRuleTimer( r.second );
         if ( rt ){
            rt->setNextInterval();
         }
      }
   }


   //
   // load the APN's into memory
   //
   if ( !m_dataaccess.getApns( m_apns, m_rules ) )
      return false;

   //
   // load the endpoint's into memory
   //
   if ( !m_dataaccess.getEndpoints( m_endpoints ) )
      return false;

   //
   // set the diameter configuration file
   //
   m_diameter.setConfigFile( Options::diameterConfiguration() );

   //
   // initialize diameter
   //
   if ( !m_diameter.init() )
      return false;

   //
   // initialize the Gx interface
   //
   try
   {
      m_gx = new gx::Application( *this );
      FDDictionaryEntryVendor vnd3gpp( m_gx->getDict().app() );
      m_diameter.advertiseSupport( m_gx->getDict().app(), vnd3gpp, 1, 0 );
      Logger::gx().startup( "%s:%d - interface initialized", __FILE__, __LINE__ );
      fd_peer_validate_register (gx_peer_validate);
   }
   catch ( FDException &e )
   {
      Logger::gx().startup( "%s:%d - FDException initializing interface - %s", __FILE__, __LINE__, e.what() );
      return false;
   }

#if 0
   //
   // initialize the Rx interface
   //
   try
   {
      m_rx = new rx::Application( *this );
      FDDictionaryEntryVendor vnd3gpp( m_rx->getDict().app() );
      m_diameter.advertiseSupport( m_rx->getDict().app(), vnd3gpp, 1, 0 );
      Logger::rx().startup( "%s:%d - interface initialized", __FILE__, __LINE__ );
   }
   catch ( FDException &e )
   {
      Logger::rx().startup( "%s:%d - FDException initializing interface - %s", __FILE__, __LINE__, e.what() );
      return false;
   }

   //
   // initialize the Sd interface
   //
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

   //
   // initialize the St interface
   //
   try
   {
      m_st = new st::Application( *this );
      FDDictionaryEntryVendor vnd3gpp( m_st->getDict().app() );
      m_diameter.advertiseSupport( m_st->getDict().app(), vnd3gpp, 1, 0 );
      Logger::st().startup( "%s:%d - interface initialized", __FILE__, __LINE__ );
   }
   catch ( FDException &e )
   {
      Logger::st().startup( "%s:%d - FDException initializing interface - %s", __FILE__, __LINE__, e.what() );
      return false;
   }
#endif

   try
   {
      Pistache::Address addrOss( Pistache::Ipv4::any(), Pistache::Port(Options::getossport()) );

      m_ossendpoint = new OssEndpoint<Logger>(addrOss, &StatsPcrf::singleton(), &Logger::singleton().audit(), &Logger::singleton(), Options::getossfile());
      m_ossendpoint->init();
      m_ossendpoint->start();
      Logger::system().startup( "Started OSS  server on port [%i]",   Options::getossport());
   }
   catch ( std::runtime_error &e )
   {
      Logger::system().error( "Exception starting REST server - [%s]",   e.what());
      return false;
   }

   StatsPcrf::singleton().setInterval(Options::statsFrequency());
   StatsPcrf::singleton().init( NULL );

   //
   // start freeDiameter
   //
   if ( !m_diameter.start() )
      return false;

   HookEvent::init(&StatsPcrf::singleton(), m_gx, m_rx, m_sd, m_st);

   return true;
}

void PCRF::uninit()
{
   m_diameter.uninit();

   if ( m_st )
   {
      Logger::st().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      delete m_st;
   }

   if ( m_sd )
   {
      Logger::sd().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      delete m_sd;
   }

   if ( m_rx )
   {
      Logger::rx().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      delete m_rx;
   }

   if ( m_gx )
   {
      Logger::gx().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      delete m_gx;
   }

   if ( m_chronos )
   {
      Logger::chronos().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      m_chronos->shutdown();
      delete m_chronos;
   }

   if ( m_ossendpoint )
   {
      Logger::system().startup( "%s:%d - interface shutdown", __FILE__, __LINE__ );
      m_ossendpoint->shutdown();
      delete m_ossendpoint;
   }

   Chronos::uninit();
}

void PCRF::waitForShutdown()
{
   m_diameter.waitForShutdown();
}

DataAccess &PCRF::dataaccess()
{
   return m_dataaccess;
}
