/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "logger.h"
#include "options.h"
#include "diaconst.h"

#include "pcrf.h"
#include "session.h"
#include "statpcrf.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GxSession::GxSession( PCRF &pcrf )
   : m_state( sIdle ),
     m_pcrf( pcrf ),
     m_pcrf_endpoint( NULL ),
     m_pcef_endpoint( NULL ),
     m_ipv4len( 0 ),
     m_ipv6len( 0 ),
     m_supported_features( 0 ),
     m_tdf( *this ),
     m_tssf( *this )
{
   memset( &m_ipv4, 0, sizeof( m_ipv4 ) );
   memset( &m_ipv6, 0, sizeof( m_ipv6 ) );
}

GxSession::~GxSession()
{
   m_rules.removeGxSession( this );
}

const struct in_addr &GxSession::setIPv4( struct in_addr &ia, size_t len )
{
   m_ipv4.s_addr = ia.s_addr;
   setIPv4Len( len );

   char data[INET6_ADDRSTRLEN];
   inet_ntop( AF_INET, &m_ipv4, data, sizeof( data ) );
   m_sipv4 = data;

   return getIPv4();
}

const struct in6_addr &GxSession::setIPv6( const uint8_t *ia, size_t len )
{
   memcpy( &m_ipv6.s6_addr, ia, len );
   setIPv6Len( len );

   char data[INET6_ADDRSTRLEN];
   inet_ntop( AF_INET6, &m_ipv6, data, sizeof( data ) );
   m_sipv6 = data;

   return getIPv6();
}

void GxSession::teardownSession( const char *source, GxSession *gx, SdSession::SessionReleaseCause src, StSession::TerminationCause tc, bool lock )
{
   SMutexLock l( GxSessionMap::getInstance().getMutex(), lock );
   bool isTerminating = GxSessionMap::getInstance().isTerminating( gx, false );

   if ( !GxSessionMap::getInstance().exists( gx, false ) )
   {
      if ( !isTerminating )
      {
         Logger::gx().error( "%s:%d - GxSession::teardownSession() - source=[%s] GxSession 0x%p not found", __FILE__, __LINE__, source );
         return;
      }
   }

   SdSession &tdf( gx->getTdfSession() );
   StSession &tssf( gx->getTssfSession() );
   bool sdclosed = false;
   bool stclosed = false;
   bool sdterminate = false;
   bool stterminate = false;

//   Logger::gx().warn( "%s:%d - GxSession::teardownSession - source=[%s] tdf_required=%s tdf_state=%s tssf_required=%s tssf-state=%s imsi=[%s] apn=[%s]",
//         __FILE__, __LINE__, source,
//         tdf.required()?"true":"false", tdf.required() ? tdf.getStateDescription() : tdf.getStateDescription( SdSession::sIdle ),
//         tssf.required()?"true":"false", tssf.required() ? tssf.getStateDescription() : tssf.getStateDescription( StSession::sIdle ),
//         gx->getImsi().c_str(), gx->getApn().c_str() );

   if ( tdf.required() )
   {
      switch ( tdf.getState() )
      {
         case SdSession::sOpen:
         {
            sdterminate = true;
            tdf.setState( SdSession::sClosing );
            break;
         }
         case SdSession::sIdle:
         case SdSession::sClosed:
         case SdSession::sFailed:
            sdclosed = true;
            break;
         default:
            break;
      }
   }

   if ( tssf.required() )
   {
      switch ( tssf.getState() )
      {
         case StSession::sOpen:
         {
            stterminate = true;
            tssf.setState( StSession::sClosing );
            break;
         }
         case StSession::sIdle:
         case StSession::sClosed:
         case StSession::sFailed:
            stclosed = true;
            break;
         default:
            break;
      }
   }

   if ( !isTerminating )
   {
      GxSessionMap::getInstance().terminateSession( gx );

      //
      // remove the session from the database
      //
      gx->getPCRF().dataaccess().deleteSession( *gx );

      //
      // terminate the Sd session if necessary
      //
      if ( sdterminate )
      {
         SdTerminateSession *ts = new SdTerminateSession( gx );
        printf("\n calling phase1 %s %d \n",__FUNCTION__, __LINE__);
         if ( !ts->processPhase1( src ) )
         {
            sdclosed = true;
            sdterminate = false;
            tdf.setState( SdSession::sFailed );
            delete ts;
         }
      }

      //
      // terminate the St session if necessary
      //
      if ( stterminate )
      {
         StTerminateSession *ts = new StTerminateSession( gx );
        printf("\n calling phase1 %s %d \n",__FUNCTION__, __LINE__);
         if ( !ts->processPhase1( tc ) )
         {
            stclosed = true;
            stterminate = false;
            tssf.setState( StSession::sFailed );
            delete ts;
         }
      }
   }

   if ( !sdclosed || !stclosed )
      return;

//   Logger::gx().warn( "%s:%d - GxSession::teardownSession - source=[%s] deleting GxSession sessionid=[%s] imsi=[%s] apn=[%s]",
//         __FILE__, __LINE__, source, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );

   GxSessionMap::getInstance().deleteSession( gx, false );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GxSessionMap::GxSessionMap()
{
}

GxSessionMap::~GxSessionMap()
{
   std::set<GxSession*>::iterator it;
   while ( ( it = m_sessions.begin() ) != m_sessions.end() )
      delete *it;

   while ( ( it = m_terminating.begin() ) != m_terminating.end() )
      delete *it;
}

GxSessionMap &GxSessionMap::getInstance()
{
   static GxSessionMap instance;
   return instance;
}

bool GxSessionMap::exists( GxSession *session, bool lock )
{
   SMutexLock l( m_mutex, lock );
   auto it = m_sessions.find( session );
   return m_sessions.find( session ) != m_sessions.end();
}

bool GxSessionMap::exists( const std::string &sessionid, bool lock )
{
   SMutexLock l( m_mutex, lock );
   return m_sessionids.find( sessionid ) != m_sessionids.end();
}

bool GxSessionMap::addSession( GxSession *session, bool lock )
{
   bool result = true;
   SMutexLock l( m_mutex, lock );
   GxSessionKey sk( session->getImsi(), session->getApn() );
   auto ret = insert( std::pair<GxSessionKey,GxSession*>( sk, session ) );

   if ( ret.second )
   {
      auto sesins = m_sessions.insert( session );

      if ( sesins.second )
      {
         auto sidins = m_sessionids.insert( std::pair<std::string,GxSession*>( session->getSessionId(), session ) );
         result = sidins.second;

         if ( !result )
         {
            // remove from the main map
            erase( ret.first );
            // remove from the set
            m_sessions.erase( session );
         }
      }
      else
      {
         result = false;
         // remove from the main map
         erase( ret.first );
      }
   }
   else
   {
      result = false;
   }

   return result;
}

bool GxSessionMap::findSession( const std::string &imsi, const std::string &apn, GxSession* &session, bool lock )
{
   SMutexLock l( m_mutex, lock );
   GxSessionKey k( imsi, apn );

   auto it = find( k );
   if ( it == end() )
      return false;

   session = it->second;
   return true;
}

bool GxSessionMap::findSession( const std::string &sessionid, GxSession* &session, bool lock )
{
   SMutexLock l( m_mutex, lock );

   auto it = m_sessionids.find( sessionid );
   if ( it == m_sessionids.end() )
      return false;

   session = it->second;
   return true;
}

bool GxSessionMap::isTerminating( GxSession *session, bool lock )
{
   SMutexLock l( m_mutex, lock );
   return m_terminating.find( session ) != m_terminating.end();
}

void GxSessionMap::terminateSession( GxSession *gx )
{
   if ( !exists( gx, false ) )
      return;

   GxSessionKey sk( gx->getImsi(), gx->getApn() );
   auto it = find( sk );
   if ( it != end() )
   {
//      Logger::gx().warn( "%s:%d - GxSessionMap::terminateSession() erasing map entry for sessionis=[%s] imsi=[%s] apn=[%s]",
//            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
      erase( it );
   }

   if ( m_sessions.erase( gx ) == 1 )
   {
//      Logger::gx().warn( "%s:%d - GxSessionMap::terminateSession() session erased from m_sessions sessionid=[%s] imsi=[%s] apn=[%s]",
//            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   }

   if ( m_sessionids.erase( gx->getSessionId() ) == 1 )
   {
//      Logger::gx().warn( "%s:%d - GxSessionMap::terminateSession() session erased from m_sessionids sessionid=[%s] imsi=[%s] apn=[%s]",
//            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );

   }

   auto itterm = m_terminating.find( gx );
   if ( itterm == m_terminating.end() )
   {
//      Logger::gx().warn( "%s:%d - GxSessionMap::terminateSession() adding session to terminating list sessionid=[%s] imsi=[%s] apn=[%s]",
//            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
      m_terminating.insert( gx );
   }
}

void GxSessionMap::deleteSession( GxSession *gx, bool lock )
{
   SMutexLock l( m_mutex, lock );

   GxSessionKey sk( gx->getImsi(), gx->getApn() );
   auto it = find( sk );
   if ( it != end() )
   {
//      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() erasing map entry for sessionis=[%s] imsi=[%s] apn=[%s]",
//            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
      erase( it );
   }

   if ( m_sessions.erase( gx ) == 1 )
   {
//      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_sessions sessionid=[%s] imsi=[%s] apn=[%s]",
//            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   }

   if ( m_sessionids.erase( gx->getSessionId() ) == 1 )
   {
//      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_sessionids sessionid=[%s] imsi=[%s] apn=[%s]",
//            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   }

   if ( m_terminating.erase( gx ) == 1 )
   {
//      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_terminating sessionid=[%s] imsi=[%s] apn=[%s]",
//            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   }

//   Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() deleting GxSession object for sessionid=[%s] imsi=[%s] apn=[%s]",
//         __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   delete gx;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SdSession::SdSession( GxSession &gx )
   : m_gx( gx ),
     m_ep( NULL ),
     m_state( sIdle ),
     m_supportedFeatures( 0 )
{
}

SdSession::~SdSession()
{
}

PCRF &SdSession::getPCRF()
{
   return m_gx.getPCRF();
}

const char *SdSession::getStateDescription( SdSession::State s )
{
   switch ( s )
   {
      case sIdle:       return "sIdle";
      case sOpening:    return "sOpening";
      case sOpen:       return "sOpen";
      case sClosing:    return "sClosing";
      case sClosed:     return "sClosed";
      case sFailed:     return "sFailed";
      default:          return "Unknown";
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SdSessionMap::SdSessionMap()
{
}

SdSessionMap::~SdSessionMap()
{
   SdSessionMap::iterator it;

   while ( ( it = begin() ) != end() )
   {
      delete it->second;
      erase( it );
   }
}

SdSessionMap &SdSessionMap::getInstance()
{
   static SdSessionMap instance;
   return instance;
}

bool SdSessionMap::addSession( const std::string &sessionid, SdSession *s )
{
   SMutexLock l( m_mutex );
   auto result = insert( std::pair<const std::string,SdSession*>( sessionid, s ) );
   return result.second;
}

void SdSessionMap::eraseSession( const std::string &sessionid )
{
   SMutexLock l( m_mutex );
   erase( sessionid );
}

bool SdSessionMap::findSession( const std::string &sessionid, SdSession* &s )
{
   SMutexLock l( m_mutex );

   auto it = find( sessionid );
   if ( it != end() )
   {
      s = it->second;
      return true;
   }

   return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

StSession::StSession( GxSession &gx )
   : m_gx( gx ),
     m_ep( NULL ),
     m_state( sIdle ),
     m_supportedFeatures( 0 )
{
}

StSession::~StSession()
{
}

PCRF &StSession::getPCRF()
{
   return m_gx.getPCRF();
}

const char *StSession::getStateDescription( StSession::State s )
{
   switch ( s )
   {
      case sIdle:       return "sIdle";
      case sOpening:    return "sOpening";
      case sOpen:       return "sOpen";
      case sClosing:    return "sClosing";
      case sClosed:     return "sClosed";
      case sFailed:     return "sFailed";
      default:          return "Unknown";
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

StSessionMap::StSessionMap()
{
}

StSessionMap::~StSessionMap()
{
   StSessionMap::iterator it;

   while ( ( it = begin() ) != end() )
   {
      delete it->second;
      erase( it );
   }
}

StSessionMap &StSessionMap::getInstance()
{
   static StSessionMap instance;
   return instance;
}

bool StSessionMap::addSession( const std::string &sessionid, StSession *s )
{
   SMutexLock l( m_mutex );
   auto result = insert( std::pair<std::string,StSession*>( sessionid, s ) );
   return result.second;
}

void StSessionMap::eraseSession( const std::string &sessionid )
{
   SMutexLock l( m_mutex );
   erase( sessionid );
}

bool StSessionMap::findSession( const std::string &sessionid, StSession* &s )
{
   SMutexLock l( m_mutex );

   auto it = find( sessionid );
   if ( it != end() )
   {
      s = it->second;
      return true;
   }

   return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SessionEvent::SessionEvent( PCRF &pcrf )
   : m_pcrf( pcrf ),
     m_status( esPending )
{
}

SessionEvent::~SessionEvent()
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GxIpCan1::GxIpCan1( PCRF &pcrf, FDMessageRequest *req, gx::Dictionary &dict )
   : SessionEvent( pcrf ),
     m_usagecnt( 0 ),
     m_dict( dict ),
     m_ccr( *req, dict ),
     m_cca( req ),
     m_gxsession( NULL ),
     m_delete_gxsession( true ),
     m_default_bearer_mode( 2 ), // UE_NETWORK
     m_sdEstablishSession( NULL ),
     m_stEstablishSession( NULL ),
     m_sdProcessRules( NULL ),
     m_stProcessRules( NULL )
{
   //
   // initialize the Credit-Control-Answer (CCA)
   //
   m_cca.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   m_cca.addOrigin();
   m_cca.add( m_ccr.cc_request_type );
   m_cca.add( m_ccr.cc_request_number );
}

GxIpCan1::~GxIpCan1()
{
//   Logger::gx().error( "%s:%d - inside GxIpCan1::~GxIpCan1",
//         __FILE__, __LINE__ );
   if ( m_sdEstablishSession )
      delete m_sdEstablishSession;
   if ( m_stEstablishSession )
      delete m_stEstablishSession;
   if ( m_sdProcessRules )
      delete m_sdProcessRules;
   if ( m_stProcessRules )
      delete m_stProcessRules;

   if ( getDeleteGxSession() && m_gxsession )
      delete m_gxsession;
}

void GxIpCan1::release( GxIpCan1 *gxevent )
{
   bool deleteit = false;

   {
      SMutexLock l( gxevent->m_mutex );
      deleteit = gxevent->m_usagecnt == 0;
   }

   if ( deleteit )
      delete gxevent;
}

void GxIpCan1::onInit()
{
	//m_idleTimer = new SEventThread::Timer(20, true);
   //initTimer( *m_idleTimer);
}

void GxIpCan1::onQuit()
{

}

void GxIpCan1::onTimer( SEventThread::Timer &t)
{
   if (t.getId() == m_idleTimer->getId())
   {
      postMessage( TIMEOUT );
   }
}

void GxIpCan1::dispatch( SEventThreadMessage &msg)
{
   if ( msg.getId() == TIMEOUT)
   {
      Logger::gx().debug("SOHAN TIMEOUT Occured");
      sendRAR();
   }
}

void GxIpCan1::sendRAR()
{
   Logger::gx().debug("SOHAN SENDING RAR to PEER");
   RulesList &irules( getRulesEvaluator().getGxInstallRules() );
   RulesList &rrules( getRulesEvaluator().getGxRemoveRules() );

   gx::GxRulesRARreq *req = new gx::GxRulesRARreq (((getGxSession()->getPCRF()).gxApp()), this);
   req->add( getDict().avpSessionId(), getGxSession()->getSessionId() );
   req->add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), getGxSession()->getPcefEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), getGxSession()->getPcefEndpoint()->getHost() );
   req->add( getDict().avpReAuthRequestType(), 0 );

   if ( !irules.empty() )
   {
      int crcnt = 0;
		int pracnt = 0;
		FDAvp cri ( getDict().avpChargingRuleInstall() );
		FDAvp prai ( getDict().avpPraInstall() );
		
		for (auto r : irules)
		{
			if (r->getType() == "CHARGING")
			{
				cri.addJson (r->getDefinition());
				crcnt++;
			}
			else if (r->getType() == "PRA")
			{
				prai.addJson( r->getDefinition() );
				pracnt++;
			}
		}
		
		if (crcnt > 0)
		{
			req->add(cri);
		}
		if (pracnt > 0)
		{
			req->add(prai);
		}
   }   
	if ( !rrules.empty() )
	{
		int crcnt = 0;
		int pracnt = 0;
		FDAvp crr( getDict().avpChargingRuleRemove() );
		FDAvp prar( getDict().avpPraRemove() );
		
		for (auto r : rrules)
		{
			if (r->getType() == "CHARGING")
			{
				crr.add( getDict().avpChargingRuleName(), r->getRuleName());
				crcnt++;
			}
			else if ( r->getType() == "PRA")
			{
				prar.add( getDict().avpPresenceReportingAreaIdentifier(), r->getRuleName());
				pracnt++;
			}
		}

		if (crcnt > 0)
		{
			req->add( crr );
		}
		if (pracnt > 0)
		{
			req->add( prar );
		}
	}
	FDAvp defBearerQos (getDict().avpDefaultEpsBearerQos());
	std::string json_t("{\"QoS-Class-Identifier\": 9, \"Allocation-Retention-Priority\": {\"Priority-Level\": 1, \"Pre-emption-Capability\": 2, \"Pre-emption-Vulnerability\": 20}}");
	defBearerQos.addJson(json_t);
	req->add(defBearerQos);

	req->send();
   //bool result = getPCRF().gxApp().sendRulesRARreq(*(getGxSession()), irules, rrules, this);
   //if (result)
   //{
   //   Logger::gx().debug("RAR sent successful"); 
   //}
}

void GxIpCan1::sendCCA()
{
   Logger::gx().debug( "%s:%d - Sending CCA for imsi=[%s] apn=[%s]",
         __FILE__, __LINE__,
         getGxSession() ? getGxSession()->getImsi().c_str() : "undefined",
         getGxSession() ? getGxSession()->getApn().c_str() : "undefined" );
   getCCA().dump();
   getCCA().send();
}

#define ABORT() \
   setStatus( esFailed ); \
   result = false; \
   break;

bool GxIpCan1::processPhase1()
{
   //
   // a return value of false indicates that processing should be halted
   // the destruction sequence for the object should be initiated
   //
   printf("%s %d \n",__FUNCTION__,__LINE__);
   SMutexLock l( m_mutex );
   bool result = true;
   std::string s;

   setStatus( esProcessing );

   while ( result )
   {
      setGxSession( new GxSession( getPCRF() ) );

      //
      // find the IMSI
      //
      for ( auto subidit = getCCR().subscription_id.getList().begin();
            subidit != getCCR().subscription_id.getList().end();
            ++subidit )
      {
         int32_t subscription_id_type;

         if ( (*subidit)->subscription_id_type.get( subscription_id_type ) && subscription_id_type == 1 ) // END_USER_IMSI
         {
            if ( !(*subidit)->subscription_id_data.get( s ) )
            {
               Logger::gx().error( "%s:%d - Subscription-Id-Data missing for END_USER_IMSI", __FILE__, __LINE__ );
               RESULTCODE_WITH_FAILEDAVP1( getCCA(), DIAMETER_MISSING_AVP, *(*subidit) );
               sendCCA();
               StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_MISSING_AVP);
               ABORT();
            }

            getGxSession()->setImsi( s );

            //
            // reurn if imsi is not valid
            //
            if ( getGxSession()->getImsi().length() != 15 )
            {
               Logger::gx().error( "%s:%d - Subscription-Id END_USER_IMSI invalid value", __FILE__, __LINE__ );
               RESULTCODE_WITH_FAILEDAVP1( getCCA(), DIAMETER_INVALID_AVP_VALUE, (*subidit)->subscription_id_data );
               sendCCA();
               StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_INVALID_AVP_VALUE);
               ABORT();
            }

            break;
         }
      }

      if ( !result )
         break;

      //
      // return if IMSI not found or invalid
      //
      if ( getGxSession()->getImsi().empty() )
      {
         Logger::gx().error( "%s:%d - Subscription-Id END_USER_IMSI missing", __FILE__, __LINE__ );
         RESULTCODE_WITH_FAILEDAVP2( getCCA(), DIAMETER_MISSING_AVP, avpSubscriptionIdType(), 1 );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_MISSING_AVP);
         ABORT();
      }

      //
      // get the APN
      //
      if ( !getCCR().called_station_id.get( s ) )
      {
         Logger::gx().error( "%s:%d - Called-Station-Id missing in INITIAL_REQUEST", __FILE__, __LINE__ );
         RESULTCODE_WITH_FAILEDAVP2( getCCA(), DIAMETER_MISSING_AVP, avpCalledStationId(), "" );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_MISSING_AVP);
         ABORT();
      }

      getGxSession()->setApn( s );

      //
      // return if the APN is invalid
      //
      if ( getGxSession()->getApn().empty() )
      {
         Logger::gx().error( "%s:%d - Called-Station-Id is empty", __FILE__, __LINE__ );
         EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().called_station_id );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
         ABORT();
      }

      //
      // lookup the APN and return if it does not exist
      //
      {
         Apn *apn = NULL;
         if ( !getPCRF().getApn( getGxSession()->getApn(), apn ) )
         {
            Logger::gx().error( "%s:%d - APN [%s] not found", __FILE__, __LINE__, getGxSession()->getApn().c_str() );
            EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().called_station_id );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
            ABORT();
         }

         getGxSession()->setApnEntry( apn );
      }

      //
      // get the session id
      //
      getCCR().session_id.get( s );
      getGxSession()->setSessionId( s );

      //
      // see if the session id already exists
      //
      {
         GxSession *session = NULL;
         if ( GxSessionMap::getInstance().findSession( getGxSession()->getSessionId(), session ) )
         {
            Logger::gx().error( "%s:%d - Session ID [%s] already exists imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getSessionId().c_str(),
                  getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            ABORT();
         }
      }

      //
      // see if the session already exists in memory
      //
      {
         GxSession *session = NULL;
         if ( GxSessionMap::getInstance().findSession( getGxSession()->getImsi(), getGxSession()->getApn(), session ) )
         {
            Logger::gx().error( "%s:%d - Session already exists imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            ABORT();
         }
      }

      //
      // see if the session exists in the database
      //
      if ( getPCRF().dataaccess().sessionExists( getGxSession()->getImsi(), getGxSession()->getApn() ) )
      {
         if ( Options::selfRedirect() )
         {
            Logger::gx().error( "%s:%d - self redirect not implemented", __FILE__, __LINE__ );
            RESULTCODE_WITH_FAILEDAVP1( getCCA(), DIAMETER_UNABLE_TO_COMPLY, getCCR().cc_request_type );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            ABORT();
         }

         Logger::gx().error( "%s:%d - Session already exists imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
         RESULTCODE_WITH_FAILEDAVP1( getCCA(), DIAMETER_UNABLE_TO_COMPLY, getCCR().cc_request_type );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         ABORT();
      }

      //
      // lookup the subscriber
      //
      if ( !getPCRF().dataaccess().getSubscriber( getGxSession()->getImsi(), getGxSession()->getSubscriber(), getPCRF().getRules() ) )
      {
         //
         // add subscriber if APN is configured to
         //
         if ( getGxSession()->getApnEntry()->getAutoCreateSubscriber() )
         {
            getGxSession()->getSubscriber().setImsi( getGxSession()->getImsi() );
            getGxSession()->getSubscriber().setSySubscriptionId( getGxSession()->getApnEntry()->getSySubscriptionId() );
            getGxSession()->getSubscriber().setFailOnUninstallableRule( getGxSession()->getApnEntry()->getFailOnUninstallableRule() );

//            SubscriberApn *sa = new SubscriberApn();
//            *sa = *getApnEntry();
//            getSubscriber().addSubscriberApn( sa );

            if ( !getPCRF().dataaccess().addSubscriber( getGxSession()->getSubscriber() ) )
            {
               Logger::gx().error( "%s:%d - Unable to add new subscriber imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
               RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
               sendCCA();
               StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
               ABORT();
            }
         }
         else
         {
            Logger::gx().error( "%s:%d - Subscriber does not exist and APN not configured to autoadd imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().called_station_id );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
            ABORT();
         }
      }

      {
         /*
          * If the subscriber is found and the APN does not exist under the subscriber, should they be added?
          */
         SubscriberApn *sa = NULL;

         if ( !getGxSession()->getSubscriber().getApnPolicies().getSubscriberApn( getGxSession()->getApn(), sa ) )
         {
            //
            // add the "APN" to the Subscriber
            //
            sa = new SubscriberApn();
            *sa = *getGxSession()->getApnEntry();
            getGxSession()->getSubscriber().addSubscriberApn( sa );

            if ( !getPCRF().dataaccess().addSubscriberApn( getGxSession()->getImsi(), *sa ) )
            {
               Logger::gx().error( "%s:%d - Unable to associate APN with Subscriber imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
               EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().called_station_id );
               sendCCA();
               StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
               ABORT();
            }
         }

         //
         // assign the rules associated with the APN to the process list
         //
         getGxSession()->getRules() = sa->getComputedRules();
         getGxSession()->getRules().addGxSession( getGxSession() );
      }

      //
      // get the PCRF endpoint
      //
      {
          Endpoint *ep = NULL;

          if ( !getPCRF().getEndpoint( Options::originHost(), ep ) )
          {
              ep = new Endpoint();
              ep->setHost( Options::originHost() );
              ep->setRealm( Options::originRealm() );

              if ( getPCRF().dataaccess().addEndpoint( *ep ) )
              {
                  if ( !getPCRF().addEndpoint( ep ) )
                  {
                      Logger::gx().error( "%s:%d - Unable to add PCRF endpoint [%s] to collection imsi=[%s] apn=[%s]",
                              __FILE__, __LINE__, ep->getHost().c_str(), getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
                      EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().called_station_id );
                      sendCCA();
                      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                      delete ep;
                      ABORT();
                  }
              }
              else
              {
                  Logger::gx().error( "%s:%d - Error adding PCRF endpoint [%s] to database, imsi=[%s] apn=[%s]",
                          __FILE__, __LINE__, ep->getHost().c_str(), getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().called_station_id );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  delete ep;
                  ABORT();
              }
          }

          getGxSession()->setPcrfEndpoint( ep );
      }

      {
          uint8_t ipaddr[16];
          size_t ipaddrlen = sizeof(ipaddr);

          if ( getCCR().framed_ip_address.get( ipaddr, ipaddrlen ) && ipaddrlen == 4 )
          {
              if ( ipaddrlen == 4 )
              {
                  struct in_addr ipv4;
                  ipv4.s_addr = *(uint32_t*)ipaddr;
                  getGxSession()->setIPv4( ipv4, sizeof(ipv4) );
              }
              else
              {
                  Logger::gx().error( "%s:%d - Invalid length for Framed-IP-Address, expected 4 found %d",
                          __FILE__, __LINE__, ipaddrlen );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().framed_ip_address );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  ABORT();
              }
          }

          ipaddrlen = sizeof(ipaddr);
          if ( getCCR().framed_ipv6_prefix.get( ipaddr, ipaddrlen ) )
          {
              getGxSession()->setIPv6( ipaddr, ipaddrlen );
          }

          if ( getGxSession()->getIPv4Len() == 0 && getGxSession()->getIPv6Len() == 0 )
          {
              Logger::gx().error( "%s:%d - Either Framed-IP-Address or Framed-IPv6-Prefix must be specified",
                      __FILE__, __LINE__ );
              EXP_RESULTCODE_WITH_FAILEDAVP2( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, avpFramedIpAddress(), "" );
              sendCCA();
              StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
              ABORT();
          }
      }

      //
      // get the PCEF supported features
      //
      {
          uint64_t supported_features = 0;

          for ( auto sfextractor : getCCR().supported_features.getList() )
          {
              uint32_t flid;

              if ( sfextractor->feature_list_id.get( flid ) )
              {
                  if ( flid < 1 || flid > 2 )
                  {
                      Logger::gx().error( "%s:%d - Invalid Feature-List-ID [%u] for imsi=[%s] apn=[%s]",
                              __FILE__, __LINE__, flid, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
                      RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
                      sendCCA();
                      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
                      ABORT();
                  }

                  uint32_t fl;

                  if ( sfextractor->feature_list.get( fl ) )
                      supported_features |= ( flid == 1 ? (int64_t)fl : (int64_t)fl << 32 );
              }
          }

          if ( !result )
              break;

          getGxSession()->setSupportedFeatures( supported_features );
      }

#if 0
      //
      // get the PCEF endpoint
      //
      if ( getCCR().origin_host.get( s ) )
      {
          Endpoint *ep = NULL;

          if ( !getPCRF().getEndpoint( s, ep ) )
          {
              ep = new Endpoint();
              ep->setHost( Options::originHost() );
              ep->setRealm( Options::originRealm() );

              if ( getPCRF().dataaccess().addEndpoint( *ep ) )
              {
                  if ( !getPCRF().addEndpoint( ep ) )
                  {
                      Logger::gx().error( "%s:%d - Unable to add PCEF endpoint [%s] to collection imsi=[%s] apn=[%s]",
                              __FILE__, __LINE__, ep->getHost().c_str(), getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
                      EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().origin_host );
                      sendCCA();
                      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                      delete ep;
                      ABORT();
                  }
              }
              else
              {
                  Logger::gx().error( "%s:%d - Error adding PCEF endpoint [%s] to database, imsi=[%s] apn=[%s]",
                          __FILE__, __LINE__, ep->getHost().c_str(), getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().origin_host );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  delete ep;
                  ABORT();
              }
          }

          getGxSession()->setPcefEndpoint( ep );
      }
      else
      {
          Logger::gx().error( "%s:%d - Origin-Host missing imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
          RESULTCODE_WITH_FAILEDAVP2( getCCA(), DIAMETER_MISSING_AVP, avpOriginHost(), "" );
          sendCCA();
          StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_MISSING_AVP);
          ABORT();
      }
#endif

      //
      // get the TDF endpoint
      //
      if ( getCCR().tdf_information.tdf_destination_host.get( s ) )
      {
          Endpoint *ep = NULL;

          //
          // lookup the TDF endpoint in memory
          //
          if ( getPCRF().getEndpoint( s, ep ) )
          {
              getGxSession()->getTdfSession().setEndpoint( ep );
          }
          else
          {
              //
              // initialize a new Endpoint object since it was not found
              //
              ep = new Endpoint();
              ep->setHost( s );
              if ( getCCR().tdf_information.tdf_destination_realm.get( s ) )
                  ep->setRealm( s );
              if ( getCCR().tdf_information.tdf_ip_address.get( s ) )
                  ep->setIp( s );

              try
              {
                  if ( getPCRF().dataaccess().addEndpoint( *ep ) )
                  {
                      if ( getPCRF().addEndpoint( ep ) )
                      {
                          getGxSession()->getTdfSession().setEndpoint( ep );
                      }
                      else
                      {
                          Logger::gx().error( "%s:%d - Unable to add TDF endpoint [%s] to the internal collection",
                                  __FILE__, __LINE__, ep->getHost().c_str() );
                          EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().tdf_information.tdf_destination_host );
                          sendCCA();
                          StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                          delete ep;
                          ABORT();
                      }
                  }
                  else
                  {
                      Logger::gx().error( "%s:%d - Unable to add TDF endpoint [%s] to the database",
                              __FILE__, __LINE__, ep->getHost().c_str() );
                      EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().tdf_information.tdf_destination_host );
                      sendCCA();
                      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                      delete ep;
                      ABORT();
                  }
              }
              catch ( DAException &ex )
              {
                  Logger::gx().error( "%s:%d - Exception while adding TDF endpoint [%s] to the database - %s",
                          __FILE__, __LINE__, ep->getHost().c_str(), ex.what() );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().tdf_information.tdf_destination_host );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  delete ep;
                  ABORT();
              }
          }
      }
      else
      {
          //
          // set the TDF endpoint from the PcrfEndpoint
          //
          if ( getGxSession()->getPcrfEndpoint() && !getGxSession()->getPcrfEndpoint()->getAssignedTdf().empty() )
          {
              Endpoint *ep = NULL;

              if ( getPCRF().getEndpoint( getGxSession()->getPcrfEndpoint()->getAssignedTdf(), ep ) )
              {
                  getGxSession()->getTdfSession().setEndpoint( ep );
              }
              else
              {
                  Logger::gx().error( "%s:%d - TDF endpoint [%s] specified in PCEF [%s] endoint entry does not exist",
                          __FILE__, __LINE__, getGxSession()->getPcrfEndpoint()->getAssignedTdf().c_str(),
                          getGxSession()->getPcefEndpoint()->getHost().c_str() );
                  EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  ABORT();
              }
          }
      }

      //
      // get the TSSF endpoint
      //
      if ( getGxSession()->getPcrfEndpoint() && !getGxSession()->getPcrfEndpoint()->getAssignedTssf().empty() )
      {
          Endpoint *ep = NULL;

          if ( getPCRF().getEndpoint( getGxSession()->getPcrfEndpoint()->getAssignedTssf(), ep ) )
          {
              getGxSession()->getTssfSession().setEndpoint( ep );
          }
          else
          {
              Logger::gx().error( "%s:%d - TSSF endpoint [%s] specified in PCEF [%s] endoint database entry does not exist",
                      __FILE__, __LINE__, getGxSession()->getPcrfEndpoint()->getAssignedTdf().c_str(),
                      getGxSession()->getPcefEndpoint()->getHost().c_str() );
              EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
              sendCCA();
              StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
              ABORT();
          }
      }
      //
      // setup Sy interface if needed
      //
      {
         bool syRequired = getGxSession()->getApnEntry()->getSyRequired();

         if ( !syRequired )
         {
            for ( auto rule : getGxSession()->getRules() )
            {
               if ( ( syRequired = rule->getSyRequired() ) )
                  break;
            }
         }

         if ( syRequired )
         {
            Logger::gx().error( "%s:%d - Sy interface required but not supported imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            ABORT();
         }
      }

      //
      // add the session to the database
      //
      if ( !getPCRF().dataaccess().addSession( *getGxSession() ) )
      {
         Logger::gx().error( "%s:%d - Unable to add the session to the database imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         ABORT();
      }

      //
      // all the parsing work has been completed, so add the session to the session map
      //
      if ( !GxSessionMap::getInstance().addSession( getGxSession() ) )
      {
         Logger::gx().error( "%s:%d - Unable to insert session into session map imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         ABORT();
      }

      setDeleteGxSession( false ); std::cout<<"Calling TDF functions now"<<std::endl;

      //
      // establish the TDF session if necessary
      //
      {
         m_sdEstablishSession = new SdIpCan1EstablishSession( getPCRF(), this );
         if ( !m_sdEstablishSession )
         {
            Logger::gx().error( "%s:%d - Unable to allocate SdIpCan1 object for imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            ABORT();
         }

        printf("\n calling phase1 %s %d \n",__FUNCTION__, __LINE__);
         if ( !m_sdEstablishSession->processPhase1() )
         {
            printf("\n phase1 return %s %d \n",__FUNCTION__, __LINE__);
            if ( getCCR().tdf_information.tdf_destination_host.exists() )
            {
               EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS,
                     getCCR().tdf_information.tdf_destination_host );
            }
            else
            {
               EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
            }
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
            ABORT();
         }
         printf("\n phase1 return %s %d \n",__FUNCTION__, __LINE__);
      }

      //
      // establish the TSSF session if necessary
      //
      {
         m_stEstablishSession = new StIpCan1EstablishSession( getPCRF(), this );
         if ( !m_stEstablishSession )
         {
            Logger::gx().error( "%s:%d - Unable to allocate StIpCan1EstablishSession object for imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            ABORT();
         }

        printf("\n calling phase1 %s %d \n",__FUNCTION__, __LINE__);
         if ( !m_stEstablishSession->processPhase1() )
         {
            EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
            ABORT();
         }
      }
	  if (getStatus() == esComplete)
	  {
	     // we have sent the successful CCA Initial, hence start the timer
	    Logger::gx().debug("SOHAN STARTING THE TIMER AS CCA Initial is sent");
		 m_idleTimer = new SEventThread::Timer(20, true);
   	 initTimer( *m_idleTimer);
	  	 init(NULL);
		 m_idleTimer->start();
	  }
      break;
   }

   return result;
}

#undef ABORT

bool GxIpCan1::processPhase2( bool lockit )
{
   SMutexLock l( m_mutex, lockit );

   printf("%s %d \n",__FUNCTION__,__LINE__);
   Logger::gx().debug( "%s:%d - GxIpCan1::processPhase2 - start", __FILE__, __LINE__ );

   // evaluate rules and send if good
   // when sent, then trigger sending rules to PCEF
   EventStatus sdstatus = m_sdEstablishSession ? m_sdEstablishSession->getStatus() : esComplete;
   EventStatus ststatus = m_stEstablishSession ? m_stEstablishSession->getStatus() : esComplete;

   if ( ( sdstatus == esPending || sdstatus == esProcessing ) ||
        ( ststatus == esPending || ststatus == esProcessing ) )
   {
      Logger::gx().debug( "%s:%d - GxIpCan1::processPhase2 - sdstatus=%s ststatus=%s",
            __FILE__, __LINE__,
            m_sdEstablishSession->getStatusDescription(),
            m_stEstablishSession->getStatusDescription() );
      return true;
   }

   if ( getStatus() == esFailed || sdstatus == esFailed || ststatus == esFailed )
   {
      if ( getStatus() == esProcessing )
      {
         setStatus( esFailed );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
      }
      return false;
   }

   bool result = getRulesEvaluator().evaluate(*getGxSession(), getGxSession()->getRules(),
         getGxSession()->getInstalledRules(), getGxSession()->getTdfSession().getInstalledRules(),
         getGxSession()->getTssfSession().getInstalledRules(), getGxSession()->getSubscriber().getFailOnUninstallableRule() );

   if ( result )
   {
      m_sdProcessRules = new SdIpCan1ProcessRules( getPCRF(), this );
      if ( !m_sdProcessRules )
      {
         Logger::gx().error( "%s:%d - Unable to allocate SdIpCan1ProcessRules object for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         return false;
      }

      printf("\n calling phase1 %s %d \n",__FUNCTION__, __LINE__);
      result = m_sdProcessRules->processPhase1();
      std::cout<<"phase 1 m_sdProcessRules  over "<<__FUNCTION__<<" line "<<__LINE__<<" result "<<result<<std::endl;;
      if ( !result )
      {
         setStatus( esFailed );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         return result;
      }

      m_stProcessRules = new StIpCan1ProcessRules( getPCRF(), this );
      if ( !m_stProcessRules )
      {
         Logger::gx().error( "%s:%d - Unable to allocate StIpCan1ProcessRules object for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         return false;
      }

        printf("\n calling phase1 %s %d \n",__FUNCTION__, __LINE__);
      result = m_stProcessRules->processPhase1();
      if ( !result )
      {
         setStatus( esFailed );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         return result;
      }
   }
   else
   {
      RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
      sendCCA();
      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
   }

   //
   // a false return value will result in the calling process to start the teardown process
   //
   return result;
}

bool GxIpCan1::processPhase3()
{
   printf("%s %d \n",__FUNCTION__,__LINE__);
   SMutexLock l( m_mutex );

   if ( getStatus() != esProcessing )
      return true;

   EventStatus sdstatus = m_sdProcessRules ? m_sdProcessRules->getStatus() : esComplete;
   EventStatus ststatus = m_stProcessRules ? m_stProcessRules->getStatus() : esComplete;

   if ( ( sdstatus == esPending || sdstatus == esProcessing ) ||
        ( ststatus == esPending || ststatus == esProcessing ) )
   {
      return true;
   }

   if ( sdstatus == esFailed || ststatus == esFailed )
   {
      setStatus( esFailed );
      RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
      sendCCA();
      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
      return false;
   }

   //Logger::gx().warn( "%s:%d - GxIpCan1::processPhase3() adding rules for imsi=[%s] apn=[%s]",
   //      __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );

   // add the DIAMETER_SUCCESS Result-Code AVP
   getCCA().add( getDict().avpResultCode(), DIAMETER_SUCCESS );

   FDAvp defBearerQos(getDict().avpDefaultEpsBearerQos());
   std::string json_t("{\"QoS-Class-Identifier\": 9, \"Allocation-Retention-Priority\": {\"Priority-Level\": 1, \"Pre-emption-Capability\": 1, \"Pre-emption-Vulnerability\": 1}}");
   defBearerQos.addJson(json_t);
   getCCA().add(defBearerQos);
   // add the rules
   {
      RulesList &irules( getRulesEvaluator().getGxInstallRules() );
      RulesList &rrules( getRulesEvaluator().getGxRemoveRules() );
      RulesList &ir( getGxSession()->getInstalledRules() );

      if ( !irules.empty() )
      {
         int crcnt = 0;
         int pracnt = 0;
         FDAvp cri( getDict().avpChargingRuleInstall() );
         FDAvp prai( getDict().avpPraInstall() );

         for ( auto r : irules )
         {
            if ( r->getType() == "CHARGING" )
            {
               cri.addJson( r->getDefinition() );
               crcnt++;
            }
            else if ( r->getType() == "PRA" )
            {
               prai.addJson( r->getDefinition() );
               pracnt++;
            }
            ir.push_back( r );
         }

         if ( crcnt > 0 )
            getCCA().add( cri );
         if ( pracnt > 0 )
            getCCA().add( prai );
      }

      if ( !rrules.empty() )
      {
         int crcnt = 0;
         int pracnt = 0;
         FDAvp crr( getDict().avpChargingRuleRemove() );
         FDAvp prar( getDict().avpPraRemove() );

         for ( auto r : rrules )
         {
            if ( r->getType() == "CHARGING" )
            {
               crr.add( getDict().avpChargingRuleName(), r->getRuleName() );
               crcnt++;
            }
            else if ( r->getType() == "PRA" )
            {
               prar.add( getDict().avpPresenceReportingAreaIdentifier(), r->getRuleName() );
               pracnt++;
            }
            if ( r->getRuleTimer() )
               r->getRuleTimer()->removeSession( getGxSession() );
            ir.erase( r );
         }

         if ( crcnt > 0 )
            getCCA().add( crr );
         if ( pracnt > 0 )
             getCCA().add( prar );
      }
   }

   // send the cca
   std::cout<<"Sending CCA-Initial \n"<<std::endl; 
   sendCCA();
   StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_SUCCESS);

   // flag this event as complete
   setStatus( esComplete );

   //
   // a return value of false will trigger the cleanup of the GxIpCan1 event object
   //
   return true;// false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SdIpCan1EstablishSession::SdIpCan1EstablishSession( PCRF &pcrf, GxIpCan1 *gxevent )
   : SessionEvent( pcrf ),
     m_gxevent( gxevent )
{
   m_gxevent->incrementUsage();
}

SdIpCan1EstablishSession::~SdIpCan1EstablishSession()
{
}

bool SdIpCan1EstablishSession::processPhase1()
{
   SdSession &tdf( m_gxevent->getGxSession()->getTdfSession() );

   if ( !tdf.required() )
   {
      // mark the event as complete since no session is needed
      setStatus( esComplete );
      //
      // pass false as the lockit parameter to m_gxevent->processPhase2() to
      // prevent a deadlock situation from occurring since processPhase1()
      // is called from m_gxevent with it's mutex already locked
      //
      printf("Calling phase 2 from %s %d \n",__FUNCTION__,__LINE__);
      bool result = m_gxevent->processPhase2( false );
      std::cout<<"Calling phase 2 from "<<__FUNCTION__<<" line "<<__LINE__<<" result "<<result<<std::endl;;
      return result;
   }

   bool result;

   {
      SMutexLock l( m_mutex );
      setStatus( esProcessing );
      tdf.setState( SdSession::sOpening );
      result = m_gxevent->getPCRF().sdApp().sendTSRreq( this );
      if ( !result )
      {
         setStatus( esFailed );
         tdf.setState( SdSession::sFailed );
      }
   }

   return result;
}

bool SdIpCan1EstablishSession::processPhase2( bool success )
{
   SMutexLock l( m_mutex );
   GxSession *gxsession = m_gxevent->getGxSession();
   SdSession &tdf( gxsession->getTdfSession() );

   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         setStatus( esComplete );
         SdSessionMap::getInstance().addSession( tdf.getSessionId(), &tdf );
         tdf.setState( SdSession::sOpen );
      }
      else
      {
         Logger::sd().error( "%s:%d - SdIpCan1EstablishSession - TSR processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, gxsession->getImsi().c_str(), gxsession->getApn().c_str() );
         setStatus( esFailed );
         tdf.setState( SdSession::sFailed );
      }
   }
   else
   {
      Logger::sd().error( "%s:%d - SdIpCan1EstablishSession - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription( getStatus() ),
            gxsession->getImsi().c_str(), gxsession->getApn().c_str() );
      setStatus( esFailed );
      tdf.setState( SdSession::sFailed );
   }

         printf("Calling phase 2 from %s %d \n",__FUNCTION__,__LINE__);
   bool result = m_gxevent->processPhase2();

   return result;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

StIpCan1EstablishSession::StIpCan1EstablishSession( PCRF &pcrf, GxIpCan1 *gxevent )
   : SessionEvent( pcrf ),
     m_gxevent( gxevent )
{
   m_gxevent->incrementUsage();
}

StIpCan1EstablishSession::~StIpCan1EstablishSession()
{
}

bool StIpCan1EstablishSession::processPhase1()
{
   StSession &tssf( m_gxevent->getGxSession()->getTssfSession() );

   if ( !tssf.required() )
   {
      // mark the event as complete since no session is needed
      setStatus( esComplete );
      //
      // pass false as the lockit parameter to m_gxevent->processPhase2() to
      // prevent a deadlock situation from occurring since processPhase1()
      // is called from m_gxevent with it's mutex already locked
      //
         printf("Calling phase 2 from %s %d \n",__FUNCTION__,__LINE__);
      bool result = m_gxevent->processPhase2( false );
      return result;
   }

   bool result;

   {
      SMutexLock l( m_mutex );
      setStatus( esProcessing );
      tssf.setState( StSession::sOpening );
      result = m_gxevent->getPCRF().stApp().sendTSRreq( this );
      if ( !result )
      {
         setStatus( esFailed );
         tssf.setState( StSession::sFailed );
      }
   }

   return result;
}

bool StIpCan1EstablishSession::processPhase2( bool success )
{
   SMutexLock l( m_mutex );
   GxSession *gxsession = m_gxevent->getGxSession();
   StSession &tssf( gxsession->getTssfSession() );

   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         setStatus( esComplete );
         StSessionMap::getInstance().addSession( tssf.getSessionId(), &tssf );
         tssf.setState( StSession::sOpen );
      }
      else
      {
         Logger::st().error( "%s:%d - StIpCan1EstablishSession - TSR processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, gxsession->getImsi().c_str(), gxsession->getApn().c_str() );
         setStatus( esFailed );
         tssf.setState( StSession::sFailed );
      }
   }
   else
   {
      Logger::st().error( "%s:%d - StIpCan1EstablishSession - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription(),
            gxsession->getImsi().c_str(), gxsession->getApn().c_str() );
      setStatus( esFailed );
         printf("Calling phase 2 from %s %d \n",__FUNCTION__,__LINE__);
      tssf.setState( StSession::sFailed );
   }

   bool result = m_gxevent->processPhase2();

   return result;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SdIpCan1ProcessRules::SdIpCan1ProcessRules( PCRF &pcrf, GxIpCan1 *gxevent )
   : SessionEvent( pcrf ),
     m_gxevent( gxevent )
{
   m_gxevent->incrementUsage();
}

SdIpCan1ProcessRules::~SdIpCan1ProcessRules()
{
}

bool SdIpCan1ProcessRules::processPhase1()
{
   SdSession &tdf( m_gxevent->getGxSession()->getTdfSession() );
   RulesList &irules( m_gxevent->getRulesEvaluator().getSdInstallRules() );
   RulesList &rrules( m_gxevent->getRulesEvaluator().getSdRemoveRules() );

   if ( !tdf.required() )
   {
      if ( rrules.empty() && irules.empty() )
      {
         // mark the event as complete since no session is needed
         setStatus( esComplete );
         printf("Calling phase 3 from %s %d \n",__FUNCTION__,__LINE__);
         bool result = m_gxevent->processPhase3();
         std::cout<<"phase 3 over "<<__FUNCTION__<<" line "<<__LINE__<<" result "<<result<<std::endl;;
         return result;
      }
      else
      {
         Logger::sd().error( "%s:%d - SdIpCan1ProcessRules - the TDF is flagged as not required but there are %u/%u install/remove rules to install at the TDF imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, irules.size(), rrules.size(), m_gxevent->getGxSession()->getImsi().c_str(), m_gxevent->getGxSession()->getApn().c_str() );
         setStatus( esFailed );
         return false;
      }
   }

   setStatus( esProcessing );

   bool result = m_gxevent->getPCRF().sdApp().sendRARreq( tdf, irules, rrules, this );

   if ( !result )
   setStatus( esFailed );

   return result;
}

bool SdIpCan1ProcessRules::processPhase2( bool success )
{
   GxSession *gxsession = m_gxevent->getGxSession();

   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         //SMutexLock l( gxsession->getMutex() );

         RulesList &sdrules( gxsession->getTdfSession().getInstalledRules() );
         RulesList &irules( m_gxevent->getRulesEvaluator().getSdInstallRules() );
         RulesList &rrules( m_gxevent->getRulesEvaluator().getSdRemoveRules() );

         for ( auto r : irules )
            sdrules.push_back( r );

         for ( auto r : rrules )
            sdrules.erase( r );

         setStatus( esComplete );
      }
      else
      {
         Logger::sd().error( "%s:%d - SdIpCan1ProcessRules - RAR processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, gxsession->getImsi().c_str(), gxsession->getApn().c_str() );
         setStatus( esFailed );
      }
   }
   else
   {
      Logger::sd().error( "%s:%d - SdIpCan1ProcessRules - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription( getStatus() ),
            gxsession->getImsi().c_str(), gxsession->getApn().c_str() );
      setStatus( esFailed );
   }

         printf("Calling phase 3 from %s %d \n",__FUNCTION__,__LINE__);
   return m_gxevent->processPhase3();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

StIpCan1ProcessRules::StIpCan1ProcessRules( PCRF &pcrf, GxIpCan1 *gxevent )
   : SessionEvent( pcrf ),
     m_gxevent( gxevent )
{
   m_gxevent->incrementUsage();
}

StIpCan1ProcessRules::~StIpCan1ProcessRules()
{
}

bool StIpCan1ProcessRules::processPhase1()
{
   StSession &tssf( m_gxevent->getGxSession()->getTssfSession() );
   RulesList &irules( m_gxevent->getRulesEvaluator().getStInstallRules() );
   RulesList &rrules( m_gxevent->getRulesEvaluator().getStRemoveRules() );

   if ( !tssf.required() )
   {
      if ( rrules.empty() && irules.empty() )
      {
         // mark the event as complete since no session is needed
         setStatus( esComplete );
         printf("Calling phase 3 from %s %d \n",__FUNCTION__,__LINE__);
         return m_gxevent->processPhase3();
      }
      else
      {
         Logger::st().error( "%s:%d - StIpCan1ProcessRules - the TSSF is flagged as not required but there are %u/%u install/remove rules to install at the TDF imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, irules.size(), rrules.size(), m_gxevent->getGxSession()->getImsi().c_str(), m_gxevent->getGxSession()->getApn().c_str() );
         setStatus( esFailed );
         return false;
      }
   }

   setStatus( esProcessing );

   bool result = m_gxevent->getPCRF().stApp().sendRulesTSRreq( tssf, irules, rrules, this );

   if ( !result )
      setStatus( esFailed );

   return result;
}

bool StIpCan1ProcessRules::processPhase2( bool success )
{
   GxSession *gxsession = m_gxevent->getGxSession();

   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         //SMutexLock l( gxsession->getMutex() );

         RulesList &strules( gxsession->getTssfSession().getInstalledRules() );
         RulesList &irules( m_gxevent->getRulesEvaluator().getStInstallRules() );
         RulesList &rrules( m_gxevent->getRulesEvaluator().getStRemoveRules() );

         for ( auto r : irules )
            strules.push_back( r );

         for ( auto r : rrules )
            strules.erase( r );

         setStatus( esComplete );
      }
      else
      {
         Logger::st().error( "%s:%d - StIpCan1ProcessRules - TSR processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, gxsession->getImsi().c_str(), gxsession->getApn().c_str() );
         setStatus( esFailed );
      }
   }
   else
   {
      Logger::st().error( "%s:%d - StIpCan1ProcessRules - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription( getStatus() ),
            gxsession->getImsi().c_str(), gxsession->getApn().c_str() );
      setStatus( esFailed );
   }

         printf("Calling phase 3 from %s %d \n",__FUNCTION__,__LINE__);
   return m_gxevent->processPhase3();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SdTerminateSession::SdTerminateSession( GxSession *gx )
   : SessionEvent( gx->getPCRF() ),
     m_gx( gx )
{
}

SdTerminateSession::~SdTerminateSession()
{
}

bool SdTerminateSession::processPhase1(SdSession::SessionReleaseCause src )
{
   Logger::sd().info( "%s:%d - SdTerminateSession::phase1 - this=%p status=%s for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, this, getStatusDescription(), m_gx->getImsi().c_str(), m_gx->getApn().c_str() );

   SdSession &tdf( m_gx->getTdfSession() );

   setStatus( esProcessing );

   bool result = m_gx->getPCRF().sdApp().sendRARTreq( tdf, src, this );

   if ( !result )
      setStatus( esFailed );

   return result;
}

bool SdTerminateSession::processPhase2( bool success )
{
   SMutexLock l( GxSessionMap::getInstance().getMutex() );

//   Logger::sd().error( "%s:%d - SdTerminateSession::processPhase2 - begin success=%s status=%s imsi=[%s] apn=[%s]",
//         __FILE__, __LINE__, success ? "true" : "false", getStatusDescription(), m_gx->getImsi().c_str(), m_gx->getApn().c_str() );

   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         m_gx->getTdfSession().setState( SdSession::sClosed );
         setStatus( esComplete );
      }
      else
      {
         Logger::sd().error( "%s:%d - SdTerminateSession - RART processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, m_gx->getImsi().c_str(), m_gx->getApn().c_str() );
         m_gx->getTdfSession().setState( SdSession::sFailed );
         setStatus( esFailed );
      }
   }
   else
   {
      Logger::sd().error( "%s:%d - SdTerminateSession - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription( getStatus() ),
            m_gx->getImsi().c_str(), m_gx->getApn().c_str() );
      m_gx->getTdfSession().setState( SdSession::sFailed );
      setStatus( esFailed );
   }

//   Logger::sd().error( "%s:%d - SdTerminateSession::processPhase2 - session state set to [%s] imsi=[%s] apn=[%s]",
//         __FILE__, __LINE__,  m_gx->getTdfSession().getStateDescription(), m_gx->getImsi().c_str(), m_gx->getApn().c_str() );

   SdSessionMap::getInstance().eraseSession( m_gx->getTdfSession().getSessionId() );

   GxSession::teardownSession( "SdTerminateSession::processPhase2", m_gx, false );

   return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

StTerminateSession::StTerminateSession( GxSession *gx )
   : SessionEvent( gx->getPCRF() ),
     m_gx( gx )
{
}

StTerminateSession::~StTerminateSession()
{
}

bool StTerminateSession::processPhase1( StSession::TerminationCause tc )
{
   Logger::st().info( "%s:%d - StTerminateSession::phase1 - this=%p status=%s for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, this, getStatusDescription(), m_gx->getImsi().c_str(), m_gx->getApn().c_str() );

   StSession &tssf( m_gx->getTssfSession() );

   setStatus( esProcessing );

   bool result = m_gx->getPCRF().stApp().sendSTRreq( tssf, tc, this );

   if ( !result )
      setStatus( esFailed );

   return result;
}

bool StTerminateSession::processPhase2( bool success )
{
   SMutexLock l( GxSessionMap::getInstance().getMutex() );

//   Logger::st().error( "%s:%d - StTerminateSession::processPhase2 - begin success=%s status=%s imsi=[%s] apn=[%s]",
//         __FILE__, __LINE__, success ? "true" : "false", getStatusDescription(), m_gx->getImsi().c_str(), m_gx->getApn().c_str() );

   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         m_gx->getTssfSession().setState( StSession::sClosed );
         setStatus( esComplete );
      }
      else
      {
         Logger::st().error( "%s:%d - StTerminateSession - STR processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, m_gx->getImsi().c_str(), m_gx->getApn().c_str() );
         m_gx->getTssfSession().setState( StSession::sFailed );
         setStatus( esFailed );
      }
   }
   else
   {
      Logger::st().error( "%s:%d - StTerminateSession - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription( getStatus() ),
            m_gx->getImsi().c_str(), m_gx->getApn().c_str() );
      m_gx->getTssfSession().setState( StSession::sFailed );
      setStatus( esFailed );
   }

//   Logger::st().error( "%s:%d - StTerminateSession::processPhase2 - session state set to [%s] imsi=[%s] apn=[%s]",
//         __FILE__, __LINE__,  m_gx->getTssfSession().getStateDescription(), m_gx->getImsi().c_str(), m_gx->getApn().c_str() );

   StSessionMap::getInstance().eraseSession( m_gx->getTssfSession().getSessionId() );

   GxSession::teardownSession( "StTerminateSession::processPhase2", m_gx, false );

   return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GxProcessRulesUpdate::GxProcessRulesUpdate( PCRF &pcrf, GxSession *gx )
   : SessionEvent( pcrf ),
     m_gx( gx )
{
}

GxProcessRulesUpdate::~GxProcessRulesUpdate()
{
}

bool GxProcessRulesUpdate::processPhase1( RuleEvaluator &re )
{
   GxSession &pcef( *m_gx );
   RulesList &irules( re.getGxInstallRules() );
   RulesList &rrules( re.getGxRemoveRules() );

   if ( rrules.empty() && irules.empty() )
   {
      setStatus( esComplete );
      return false; // false indicates that processing of event is to halt
   }

   setStatus( esProcessing );

   m_irules = irules;
   m_rrules = rrules;

   bool result = getPCRF().gxApp().sendRulesRARreq( pcef, irules, rrules, this );

   if ( !result )
      setStatus( esFailed );

   return result;
}

bool GxProcessRulesUpdate::processPhase2( bool success )
{
   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         RulesList &gxrules( m_gx->getInstalledRules() );

         for ( auto r : m_irules )
            gxrules.push_back( r );

         for ( auto r : m_rrules )
            gxrules.erase( r );

         setStatus( esComplete );
      }
      else
      {
         Logger::gx().error( "%s:%d - GxProcessRulesUpdate - RAR rules processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, m_gx->getImsi().c_str(), m_gx->getApn().c_str() );
         setStatus( esFailed );
      }
   }
   else
   {
      Logger::gx().error( "%s:%d - GxProcessRulesUpdate - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription( getStatus() ),
            m_gx->getImsi().c_str(), m_gx->getApn().c_str() );
      setStatus( esFailed );
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SdProcessRulesUpdate::SdProcessRulesUpdate( PCRF &pcrf, SdSession *sd )
   : SessionEvent( pcrf ),
     m_sd( sd)
{
}

SdProcessRulesUpdate::~SdProcessRulesUpdate()
{
}

bool SdProcessRulesUpdate::processPhase1( RuleEvaluator &re )
{
   SdSession &tdf( *m_sd );
   RulesList &irules( re.getSdInstallRules() );
   RulesList &rrules( re.getSdRemoveRules() );

   std::cout<<" function  "<<__FUNCTION__<<" line - "<<__LINE__<<std::endl;
   if ( rrules.empty() && irules.empty() )
   {
   std::cout<<" function  "<<__FUNCTION__<<" line - "<<__LINE__<<std::endl;
      setStatus( esComplete );
      return false; // false indicates that processing of event is to halt
   }

   if ( !tdf.required() )
   {
   std::cout<<" function  "<<__FUNCTION__<<" line - "<<__LINE__<<std::endl;
      Logger::sd().error( "%s:%d - SdProcessRulesUpdate - the TDF is flagged as not required but there are %u/%u install/remove rules to install at the TDF",
            __FILE__, __LINE__, irules.size(), rrules.size() );
      setStatus( esFailed );
      return false;
   }

   setStatus( esProcessing );

   m_irules = irules;
   m_rrules = rrules;

   bool result = getPCRF().sdApp().sendRARreq( tdf, irules, rrules, this );

   if ( !result ) {
      setStatus( esFailed );
        std::cout<<" function  "<<__FUNCTION__<<" line - "<<__LINE__<<std::endl;
    }

   std::cout<<" function  "<<__FUNCTION__<<" line - "<<__LINE__<<std::endl;
   return result;
}

bool SdProcessRulesUpdate::processPhase2( bool success )
{
   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         RulesList &sdrules( m_sd->getInstalledRules() );

         for ( auto r : m_irules )
            sdrules.push_back( r );

         for ( auto r : m_rrules )
            sdrules.erase( r );

         setStatus( esComplete );
      }
      else
      {
         Logger::sd().error( "%s:%d - SdProcessRulesUpdate - RAR processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, m_sd->getGxSession().getImsi().c_str(), m_sd->getGxSession().getApn().c_str() );
         setStatus( esFailed );
      }
   }
   else
   {
      Logger::sd().error( "%s:%d - SdProcessRulesUpdate - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription( getStatus() ),
            m_sd->getGxSession().getImsi().c_str(), m_sd->getGxSession().getApn().c_str() );
      setStatus( esFailed );
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

StProcessRulesUpdate::StProcessRulesUpdate( PCRF &pcrf, StSession *st )
   : SessionEvent( pcrf ),
     m_st( st )
{
}

StProcessRulesUpdate::~StProcessRulesUpdate()
{
}

bool StProcessRulesUpdate::processPhase1( RuleEvaluator &re )
{
   StSession &tssf( *m_st );
   RulesList &irules( re.getStInstallRules() );
   RulesList &rrules( re.getStRemoveRules() );

   if ( rrules.empty() && irules.empty() )
   {
      setStatus( esComplete );
      return false;
   }

   if ( !tssf.required() )
   {
      Logger::st().error( "%s:%d - StProcessRulesUpdate - the TSSF is flagged as not required but there are %u/%u install/remove rules to install at the TDF imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, irules.size(), rrules.size(),
            m_st->getGxSession().getImsi().c_str(), m_st->getGxSession().getApn().c_str() );
      setStatus( esFailed );
      return false;
   }

   setStatus( esProcessing );

   m_irules = irules;
   m_rrules = rrules;

   bool result = getPCRF().stApp().sendRulesTSRreq( tssf, irules, rrules, this );

   if ( !result )
      setStatus( esFailed );

   return result;
}

bool StProcessRulesUpdate::processPhase2( bool success )
{
   if ( getStatus() == esProcessing )
   {
      if ( success )
      {
         //SMutexLock l( gxsession->getMutex() );

         RulesList &strules( m_st->getInstalledRules() );

         for ( auto r : m_irules )
            strules.push_back( r );

         for ( auto r : m_rrules )
            strules.erase( r );

         setStatus( esComplete );
      }
      else
      {
         Logger::st().error( "%s:%d - StIpCan1ProcessRules - TSR processing failed for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, m_st->getGxSession().getImsi().c_str(), m_st->getGxSession().getApn().c_str() );
         setStatus( esFailed );
      }
   }
   else
   {
      Logger::st().error( "%s:%d - StIpCan1ProcessRules - expected current event status to be [%s] but found [%s] instead for imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, getStatusDescription( esProcessing ), getStatusDescription( getStatus() ),
            m_st->getGxSession().getImsi().c_str(), m_st->getGxSession().getApn().c_str() );
      setStatus( esFailed );
   }

   return true;
}
