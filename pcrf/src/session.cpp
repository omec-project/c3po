/*
* Copyright (c) 2003-2020, Great Software Laboratory Pvt. Ltd.
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
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GxSession::GxSession( PCRF &pcrf, SessionEvent* current_event )
   : m_state( sIdle ),
     m_pcrf( pcrf ),
     m_pcrf_endpoint( NULL ),
     m_pcef_endpoint( NULL ),
     m_ipv4len( 0 ),
     m_ipv6len( 0 ),
     m_supported_features( 0 ),
     m_tdf( *this ),
     m_tssf( *this ),
	  m_default_rule( NULL )
{
   memset( &m_ipv4, 0, sizeof( m_ipv4 ) );
   memset( &m_ipv6, 0, sizeof( m_ipv6 ) );

   mp_currentstate = new GxSessionPendingState( pcrf, current_event );
   mp_currentproc = new GxSessionValidateProc( pcrf, current_event );

	mp_currentevent = current_event; 
}

GxSession::~GxSession()
{
   m_rules.removeGxSession( this );
	if ( mp_currentstate != NULL )
	{
		delete( mp_currentstate );
	}
	if ( mp_currentproc != NULL )
	{
		delete( mp_currentproc );
	}
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


GxSessionState::GxSessionState( PCRF& pcrf, SessionEvent* current_event) : m_pcrf( pcrf ), mp_sessionevent( current_event )
{
	Logger::gx().debug("GxSessionState");
}

GxSessionState::~GxSessionState()
{
	mp_sessionevent = NULL;
}

int GxSessionState::rcvdRAA( GxSessionProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
	Logger::gx().debug("GxSessionState rcvdRAA");
}

int GxSessionState::validateReq( GxSessionProc* current_proc, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionState validateReq");
}

int GxSessionState::visit( GxSessionInstallProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
	Logger::gx().debug("GxSessionState visit install function");
}

int GxSessionState::visit( GxSessionRemoveProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
	Logger::gx().debug("GxSessionState visit remove function ");
}

int GxSessionState::visit( GxSessionValidateProc* current_proc, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionState visit validate function ");
}

int GxSessionState::visit( GxSessionDefaultRemoveProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
	Logger::gx().debug("GxSessionState visit default remove function ");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionPendingState::GxSessionPendingState( PCRF& pcrf, SessionEvent* current_event ) : GxSessionState( pcrf, current_event )
{
	GxSessionState::setStateName( "GxSessionPendingState" );
	Logger::gx().debug("GxSessionPendingState  ");
}

GxSessionPendingState::~GxSessionPendingState()
{
}

int GxSessionPendingState::validateReq( GxSessionProc* current_proc, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionPendingState validateReq ");
    int ret;
    ret = current_proc->accept( this, ccr );
    return ret;
}

int GxSessionPendingState::visit( GxSessionValidateProc* current_proc, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionPendingState visit validate function  ");
    /*
     * SessionEvent class should have the called function virtual
     * and all the Event should override these virtual methods
     * Remove the dynamic cast when this code will be changed
     */
    int ret;
    SessionEvent* event = getCurrentEvent();
    GxIpCan1* ipcan1 = NULL;
    ipcan1 = dynamic_cast<GxIpCan1*> ( event );
    if ( ipcan1 )
    {
       ret = ipcan1->validate( ccr );
       return ret;
    }
	Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionActivePendingState::GxSessionActivePendingState( PCRF& pcrf, SessionEvent* current_event ) : GxSessionState( pcrf, current_event )
{
	GxSessionState::setStateName( "GxSessionActivePendingState" );
   Logger::gx().debug("GxSessionActivePendingState  ");
}

GxSessionActivePendingState::~GxSessionActivePendingState()
{
    Logger::gx().debug("~GxSessionActivePendingState  ");
}

int GxSessionActivePendingState::validateReq( GxSessionProc* current_proc, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionActivePendingState validateReq ");
    int ret;
    ret = current_proc->accept( this, ccr );
    return ret;
}

int GxSessionActivePendingState::visit( GxSessionValidateProc* current_proc, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionActivePendingState visit validate function  ");
    /*
     * SessionEvent class should have the called function virtual
     * and all the Event should override these virtual methods
     * Remove the dynamic cast when this code will be changed
     */
    int ret;
    SessionEvent* event = getCurrentEvent();
    GxIpCan1* ipcan1 = NULL;
    ipcan1 = dynamic_cast<GxIpCan1*> ( event );
    if ( ipcan1 )
    {
		 ret = ipcan1->cleanupSession();
       ret = ipcan1->validate( ccr );
    }
	Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");
    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionActiveState::GxSessionActiveState( PCRF& pcrf, SessionEvent* current_event ) : GxSessionState( pcrf, current_event )
{
	GxSessionState::setStateName( "GxSessionActiveState" );
	Logger::gx().debug("GxSessionActiveState  ");
}

GxSessionActiveState::~GxSessionActiveState()
{
	Logger::gx().debug("~GxSessionActiveState  ");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionInactiveState::GxSessionInactiveState( PCRF& pcrf, SessionEvent* current_event ) : GxSessionState( pcrf, current_event )
{
	GxSessionState::setStateName( "GxSessionInactiveState" );
	Logger::gx().debug("GxSessionInactiveState  ");
}

GxSessionInactiveState::~GxSessionInactiveState()
{
	Logger::gx().debug("~GxSessionInactiveState  ");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionModifyPendingState::GxSessionModifyPendingState( PCRF& pcrf, SessionEvent* current_event ) : GxSessionState( pcrf, current_event )
{
	GxSessionState::setStateName( "GxSessionModifyPendingState" );
	Logger::gx().debug("GxSessionModifyPendingState  ");
}

GxSessionModifyPendingState::~GxSessionModifyPendingState()
{
	Logger::gx().debug("~GxSessionModifyPendingState  ");
}

int GxSessionModifyPendingState::rcvdRAA( GxSessionProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
	Logger::gx().debug("GxSessionModifyPendingState::rcvdRAA  ");
   int result;
	result = current_proc->accept( this, raa );
   return result;
}

int GxSessionModifyPendingState::visit( GxSessionInstallProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
    int result = false;
    /*
     * SessionEvent class should have the called function virtual
     * and all the Event should override these virtual methods
     * Remove the dynamic cast when this code will be changed
     */
    GxIpCan1* ipcan1 = NULL;
    SessionEvent* event = getCurrentEvent();
    ipcan1 = dynamic_cast<GxIpCan1*>( event );
    if (ipcan1 != NULL )
    {
        result = ipcan1->rcvdInstallRAA( raa );
    }
	 Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");
    return result;
}

int GxSessionModifyPendingState::visit( GxSessionRemoveProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
    int result;
    GxIpCan1* ipcan1 = NULL;
    SessionEvent* event = getCurrentEvent();
    ipcan1 = dynamic_cast<GxIpCan1*>( event );
    if ( ipcan1 != NULL )
    {
        result = ipcan1->rcvdRemoveRAA( raa );
    }
	// extraction of avps for remove raa here
	Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");
    return result;
}

int GxSessionModifyPendingState::visit( GxSessionDefaultRemoveProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
	int result;
	GxIpCan1* ipcan1 = NULL;
	SessionEvent* event = getCurrentEvent();
	ipcan1 = dynamic_cast<GxIpCan1*>( event );
	if ( ipcan1 != NULL )
	{
		result = ipcan1->rcvdDefaultRemoveRAA( raa );
		// remove the session as we have rcvd the RAA for removing default rules.
		result = ipcan1->cleanupSession();
	}
	Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");
	return result;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GxSessionProc::GxSessionProc( PCRF& pcrf, SessionEvent* current_event ) : m_pcrf( pcrf ), mp_sessionevent( current_event )
{
	Logger::gx().debug("GxSessionProc function ");
}

GxSessionProc::~GxSessionProc()
{
	mp_sessionevent = NULL;
}

int GxSessionProc::accept( GxSessionState* current_state, gx::ReAuthAnswerExtractor& raa )
{
	Logger::gx().debug("GxSessionProc accept raa ");
}

int GxSessionProc::accept( GxSessionState* current_state, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionProc accept  ccr");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionInstallProc::GxSessionInstallProc( PCRF& pcrf, SessionEvent* current_event ) : GxSessionProc( pcrf, current_event )
{
	GxSessionProc::setProcName( "GxSessionInstallProc" );
	Logger::gx().debug("GxSessionInstallProc ");
}

GxSessionInstallProc::~GxSessionInstallProc()
{
	Logger::gx().debug("GxSessionInstallProc destructor ");
}

int GxSessionInstallProc::accept( GxSessionState* current_state, gx::ReAuthAnswerExtractor& raa )
{
    int result;
	result = current_state->visit( this, raa );
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionRemoveProc::GxSessionRemoveProc( PCRF& pcrf, SessionEvent* current_event ) : GxSessionProc( pcrf, current_event )
{
	GxSessionProc::setProcName( "GxSessionRemoveProc" );
	Logger::gx().debug("GxSessionRemoveProc ");
}

GxSessionRemoveProc::~GxSessionRemoveProc()
{
	Logger::gx().debug("~GxSessionRemoveProc ");
}

int GxSessionRemoveProc::accept( GxSessionState* current_state, gx::ReAuthAnswerExtractor& raa )
{
    int result;
	result = current_state->visit( this, raa );
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionValidateProc::GxSessionValidateProc( PCRF& pcrf, SessionEvent* current_event ) : GxSessionProc( pcrf, current_event )
{
	GxSessionProc::setProcName( "GxSessionValidateProc" );
	Logger::gx().debug("GxSessionValidateProc ");
}

GxSessionValidateProc::~GxSessionValidateProc()
{
	Logger::gx().debug("~GxSessionValidateProc ");
}

int GxSessionValidateProc::accept( GxSessionState* current_state, gx::CreditControlRequestExtractor& ccr )
{
   int ret;
	ret = current_state->visit( this, ccr );
   return ret;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GxSessionDefaultRemoveProc::GxSessionDefaultRemoveProc( PCRF& pcrf, SessionEvent* current_event ) : GxSessionProc( pcrf, current_event )
{
	 GxSessionProc::setProcName( "GxSessionDefaultRemoveProc" );
	Logger::gx().debug("GxSessionDefaultRemoveProc ");
}

GxSessionDefaultRemoveProc::~GxSessionDefaultRemoveProc()
{
	Logger::gx().debug("~GxSessionDefaultRemoveProc ");
}

int GxSessionDefaultRemoveProc::accept( GxSessionState* current_state, gx::ReAuthAnswerExtractor& raa )
{
	int ret;
	ret = current_state->visit( this, raa );
	return ret;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DefaultRule::DefaultRule()
{
}

DefaultRule::~DefaultRule()
{
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

bool GxSessionMap::eraseSession( const std::string &imsi, const std::string &apn, bool lock )
{
    SMutexLock l( m_mutex, lock );
    GxSessionKey sk( imsi, apn );
    GxSession* gx = NULL;
    auto it = find( sk );
    if ( it != end() )
    {
        gx = it->second;
        //erase( it );
    }
    if ( gx != NULL )
    {
       printf ("EraseSession : Session Not Null deleting from maps :%s:%d\n", __FILE__, __LINE__);
       if ( m_sessions.erase( gx ) == 1 )
       {
          Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_sessions sessionid=[%s] imsi=[%s] apn=[%s]",
                __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
          printf (" EraseSession : %s:%d\n", __FILE__, __LINE__);
       }
       if ( m_sessionids.erase( gx->getSessionId() ) == 1 )
       {
          Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_sessionids sessionid=[%s] imsi=[%s] apn=[%s]",
                __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
          printf(" EraseSession : %s:%d\n", __FILE__, __LINE__);
       }
       if ( m_terminating.erase( gx ) == 1 )
       {
          Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_terminating sessionid=[%s] imsi=[%s] apn=[%s]",
                __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
          printf(" EraseSession : %s:%d\n", __FILE__, __LINE__);
       }
       erase( it );
    }
    return ValidateErrorCode::success ;
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

TriggerTimer::TriggerTimer( GxIpCan1* gxIpCan1, int triggerRARValue, int timer )
{
	m_gxipcan1 = gxIpCan1;
	m_triggerRARValue = triggerRARValue;
	m_timer = timer;
	init( NULL );
}

TriggerTimer::~TriggerTimer()
{
	m_gxipcan1 = NULL;
}

void TriggerTimer::onInit()
{
   m_reqTimer = new SEventThread::Timer(m_timer, true);
   initTimer( *m_reqTimer );
	m_reqTimer->start();
}

void TriggerTimer::onQuit()
{
	delete this;
}

void TriggerTimer::onTimer( SEventThread::Timer &t)
{
	if ( t.getId() == m_reqTimer->getId() )
   {
		switch ( m_triggerRARValue )
		{
			case RARTrigger::triggerRARPending :
			{
				postMessage( RARPendingRuleInstallTimeout );
				break;
			}
			case RARTrigger::triggerRARRemove :
			{
			   postMessage( RARPendingRuleRemoveTimeout );
				break;
			}
			case RARTrigger::triggerRARInstall :
			{
				postMessage( RARDefaultRuleRemoveTimeout );
				break;
			}
			case RARTrigger::triggerCallDisconnect : 
			{
				postMessage( MaxCallDurationTimeout );
				break;
			}
		}
   }
}

void TriggerTimer::dispatch( SEventThreadMessage& msg )
{

	switch( msg.getId() )
	{
		case RARPendingRuleInstallTimeout : 
		{
			Logger::gx().debug("RAR TIMEOUT Occured");
      	m_reqTimer->stop();
      	//m_gxipcan1->sendRAR( true );
      	m_gxipcan1->sendRAR( RARTrigger::triggerRARPending );
			break;
		}
		case RARPendingRuleRemoveTimeout :
		{
			m_reqTimer->stop();
      	m_gxipcan1->sendRAR( RARTrigger::triggerRARRemove );
			break;
		}
		case RARDefaultRuleRemoveTimeout : 
		{
			m_reqTimer->stop();
			m_gxipcan1->sendRAR( RARTrigger::triggerRARInstall );
			break;
		}
		case MaxCallDurationTimeout : 
		{
			m_reqTimer->stop();
			//m_gxipcan1->cleanupSession();
			break;
		}
	}
	join();
	quit();
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
	{
      delete m_sdEstablishSession;
	}
   if ( m_stEstablishSession )
	{
      delete m_stEstablishSession;
	}
   if ( m_sdProcessRules )
	{
      delete m_sdProcessRules;
	}
   if ( m_stProcessRules )
	{
      delete m_stProcessRules;
	}

   if ( getDeleteGxSession() && m_gxsession )
	{
      delete m_gxsession;
	}
	
	if ( m_triggertimer != NULL )
	{
		delete( m_triggertimer );
	}
	
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

int GxIpCan1::rcvdDefaultRemoveRAA( gx::ReAuthAnswerExtractor& raa )
{
	GxSession *session = NULL;
	Rule *rule;
	RulesList &irules( getRulesEvaluator().getGxInstallRules() );
	RulesList &rrules( getRulesEvaluator().getGxRemoveRules() );
	std::string session_id;
	std::string rule_name;
	int pcc_status;
   bool result = ValidateErrorCode::success ;
	std::list<gx::ChargingRuleReportExtractor*> l_fd_extractor_list;
	std::list<FDExtractorAvp*> l_rule_name_list;
	setStatus( esComplete );
	raa.session_id.get( session_id );
   if ( !GxSessionMap::getInstance().findSession( getGxSession()->getSessionId(), session ) )
   {
		return ValidateErrorCode::failure;
   }
   l_fd_extractor_list = raa.charging_rule_report.getList();

	for(std::list<gx::ChargingRuleReportExtractor*>::iterator iter= l_fd_extractor_list . begin(); iter != l_fd_extractor_list . end(); iter++)
   {
      GxChargingRuleReport* rule_report = new GxChargingRuleReport();
      l_rule_name_list = (*iter)->charging_rule_name.getList();
      for(std::list<FDExtractorAvp *>::iterator iterAvp= l_rule_name_list.begin(); iterAvp != l_rule_name_list.end(); iterAvp++)
      {
         (*iterAvp)->get( rule_name );
			/*
         rule = irules.getRule( rule_name );
         if ( irules.erase( rule ) == false )
         {
            irules.clear();
         }
         rrules.push_back( rule );
			*/
      }
      rule_report->setRuleName( rule_name ); // this will be the list of rule names
      (*iter)->pcc_rule_status.get( pcc_status );
      rule_report->setPccStatus( pcc_status );
      getGxSession()->getRulesReport().push_back( rule_report );
   }
	RulesReportList& rlist( getGxSession()->getRulesReport() );
   bool status = true;
   if (!rlist.empty())
   {
      for (auto r : rlist)
      {
         if ( r->getPccStatus() != 1 )
         {
            status = false;
         }
      }
   }
	if ( status == true )
   {
      // remove the rules from the remove list as we donot need to send rar remove again.
		irules.clear();
   }
   return result;	
}

int GxIpCan1::rcvdRemoveRAA( gx::ReAuthAnswerExtractor& raa )
{
   GxSession *session = NULL;
	Rule *rule;
	RulesList &irules( getRulesEvaluator().getGxInstallRules() );
	RulesList &rrules( getRulesEvaluator().getGxRemoveRules() );
	std::string session_id;
	std::string rule_name;
	int pcc_status;
   bool result = ValidateErrorCode::success ;
	std::list<gx::ChargingRuleReportExtractor*> l_fd_extractor_list;
	std::list<FDExtractorAvp*> l_rule_name_list;
	setStatus( esComplete );
	raa.session_id.get( session_id );
   if ( !GxSessionMap::getInstance().findSession( getGxSession()->getSessionId(), session ) )
   {
		return ValidateErrorCode::failure ;
   }
   l_fd_extractor_list = raa.charging_rule_report.getList();

   for(std::list<gx::ChargingRuleReportExtractor*>::iterator iter= l_fd_extractor_list . begin(); iter != l_fd_extractor_list . end(); iter++)
   {
		GxChargingRuleReport* rule_report = new GxChargingRuleReport();
		l_rule_name_list = (*iter)->charging_rule_name.getList();
		for(std::list<FDExtractorAvp *>::iterator iterAvp= l_rule_name_list.begin(); iterAvp != l_rule_name_list.end(); iterAvp++)
    	{
      	(*iterAvp)->get( rule_name );
			rule = irules.getRule( rule_name );
         if ( irules.erase( rule ) == false )
         {
         	irules.clear();
         }
         rrules.push_back( rule );
    	}
		rule_report->setRuleName( rule_name ); // this will be the list of rule names
		(*iter)->pcc_rule_status.get( pcc_status );
		rule_report->setPccStatus( pcc_status );	
		getGxSession()->getRulesReport().push_back( rule_report );
   }
   RulesReportList& rlist( getGxSession()->getRulesReport() );
	bool status = true;
	if (!rlist.empty())
	{
		for (auto r : rlist)
		{
			if ( r->getPccStatus() != 1)
			{
				status = false;
			}
		}
	}
   if ( status == true )
   {
      // remove the rules from the remove list as we donot need to send rar remove again.
      rrules.clear();
		// start the timer of 10 mins to send RAR for removal of all the default rules.
		int timer = getGxSession()->getApnEntry()->getTimerVal();
		m_triggertimer = new TriggerTimer( this, RARTrigger::triggerRARInstall, timer );
   }
	return result;
}

int GxIpCan1::rcvdInstallRAA( gx::ReAuthAnswerExtractor& raa )
{
    GxSession *session = NULL;
	Rule *rule;
	RulesList &irules( getRulesEvaluator().getGxInstallRules() );
	RulesList &rrules( getRulesEvaluator().getGxRemoveRules() );
	std::string session_id;
	std::string rule_name;
	int pcc_status;
   bool result = ValidateErrorCode::success ;
	std::list<gx::ChargingRuleReportExtractor*> l_fd_extractor_list;
	std::list<FDExtractorAvp*> l_rule_name_list;
	setStatus( esComplete );
	raa.session_id.get( session_id );	
   if ( !GxSessionMap::getInstance().findSession( getGxSession()->getSessionId(), session ) )
   {
		return ValidateErrorCode::failure ;
   }
	
	l_fd_extractor_list = raa.charging_rule_report.getList();

   for(std::list<gx::ChargingRuleReportExtractor*>::iterator iter= l_fd_extractor_list . begin(); iter != l_fd_extractor_list . end(); iter++)
   {
		GxChargingRuleReport* rule_report = new GxChargingRuleReport();
		l_rule_name_list = (*iter)->charging_rule_name.getList();
		for(std::list<FDExtractorAvp *>::iterator iterAvp= l_rule_name_list.begin(); iterAvp != l_rule_name_list.end(); iterAvp++)
    	{
      	(*iterAvp)->get( rule_name );
			// add the logic to identify the RAA whether this for initiating the remove rules or when the default rule is deleted.
			// in case where the default rule is also deleted then we donot need to send the rar again for removal of rules.
			rule = irules.getRule( rule_name );
         if ( irules.erase(rule) == false )
         {
         	irules.clear();
         }
         rrules.push_back( rule );
    	}
		rule_report->setRuleName( rule_name );
		(*iter)->pcc_rule_status.get( pcc_status );
		rule_report->setPccStatus( pcc_status );	
		getGxSession()->getRulesReport().push_back( rule_report );
   }
   RulesReportList& rlist( getGxSession()->getRulesReport() );
	bool status = true;
	if (!rlist.empty())
	{
		for (auto r : rlist)
		{
			if ( r->getPccStatus() != 0)
			{
				status = false;
			}
		}
	}
	if ( status == true )
	{
        // if the pending rules are installed successfully at pcef, sendrar after 20 sec to remove the rules
		Logger::gx().debug("STARTING THE TIMER AS RAA is rcvd");
		int timer = getGxSession()->getApnEntry()->getTimerVal();
		m_triggertimer = new TriggerTimer( this, RARTrigger::triggerRARRemove, timer );
	}
	/*
   else
   {
      // again send the rar to install the pending rule, because the pending were not installed successfully at pcef
   	Logger::gx().debug("STARTING THE TIMER AS RAA is rcvd");
      m_triggertimer = new TriggerTimer( this, RARTrigger::triggerRARPending, 20000 );
   }
	*/
   return result;
}

void GxIpCan1::rcvdRAA(FDMessageAnswer& ans)
{
   int result;
	GxSessionDefaultRemoveProc* gx_proc = NULL;
	GxSessionProc* current_proc = NULL;
	Logger::gx().debug("RCVD RAA FROM PEER");
	gx::ReAuthAnswerExtractor raa( ans, getDict() );
	ans.dump();
   result = getCurrentState()->rcvdRAA( getCurrentProc(), raa ); 
   if ( result == ValidateErrorCode::success )
   {
		current_proc = getCurrentProc();
		if ( current_proc != NULL )
		{
			gx_proc = dynamic_cast<GxSessionDefaultRemoveProc*>( current_proc );
      	if ( gx_proc != NULL )
      	{
				setCurrentState( NULL );
         	// this is defaultRemoveProc so we need to destroy the GxSession because the default rules have been removed in pcef
         	// PCRF initiated IP-Can Session Termination
				cleanupSession();
         	GxSession::teardownSession( "PCRFCRCRcmd::process:TERMINATION_REQUEST", getGxSession(), SdSession::srcIpCanSessionTermination, StSession::tcDiameterLogout );
         	GxIpCan1::release( this );
      	}
			else
			if ( gx_proc == NULL )
			{
      		setCurrentState( new GxSessionActiveState( getPCRF(), this ) );
				setCurrentProc( NULL );
			}
		}
   }

	/*
	current_proc = getCurrentProc();
	if ( current_proc != NULL )
	{
		gx_proc = dynamic_cast<GxSessionDefaultRemoveProc*>( current_proc );
		if ( gx_proc != NULL )
		{
			// this is defaultRemoveProc so we need to destroy the GxSession because the default rules have been removed in pcef 
			// PCRF initiated IP-Can Session Termination
			GxSession::teardownSession( "PCRFCRCRcmd::process:TERMINATION_REQUEST", getGxSession(), SdSession::srcIpCanSessionTermination, StSession::tcDiameterLogout );
			GxSession::release( this );	
		}
		else
		if ( gx_proc == NULL ) // means its not defaultRemove proc so in this we need set the new current proc to NULL.
		{
			setCurrentProc( NULL );
		}
	}
	*/
}

void GxIpCan1::sendRAR( int triggerValue )
{
   Logger::gx().debug("SENDING RAR to PEER");
	int qci, pl, pec, pev;
   RAPIDJSON_NAMESPACE::Document doc;
   RulesList &irules( getRulesEvaluator().getGxInstallRules() );
   RulesList &rrules( getRulesEvaluator().getGxRemoveRules() );
	RulesList &prules( getRulesEvaluator().getGxPendingRules() );

   gx::GxRulesRARreq *req = new gx::GxRulesRARreq (((getGxSession()->getPCRF()).gxApp()), this);
   req->add( getDict().avpSessionId(), getGxSession()->getSessionId() );
   req->add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), getGxSession()->getPcefEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), getGxSession()->getPcefEndpoint()->getHost() );
   req->add( getDict().avpReAuthRequestType(), 0 );
	if ( triggerValue == RARTrigger::triggerRARInstall )
	{
		RulesList &arules( getGxSession()->getRules() );
		if ( !arules.empty() )
		{
			int crcnt = 0;
			int pracnt = 0;
			FDAvp cri ( getDict().avpChargingRuleRemove() );
			FDAvp prai ( getDict().avpPraInstall() );
			
			for (auto r : arules)
			{
				if (r->getType() == "CHARGING" && r->getDefaultFlag() == true )
				{
					cri.add( getDict().avpChargingRuleName(), r->getRuleName() );
					cri.addJson( r->getDefinition() );
					crcnt++;
				}
				else if (r->getType() == "PRA" && r->getDefaultFlag() == true )
				{
					prai.addJson( r->getDefinition() );
					prai.add( getDict().avpPresenceReportingAreaIdentifier(), r->getRuleName() );
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
		// set current procedure for handling the remove RAA of the default rule
		setCurrentProc( new GxSessionDefaultRemoveProc( getPCRF(), this ) );
	}
	else
	if ( triggerValue == RARTrigger::triggerRARRemove )
	{ 
		// add the pending rule in remove list from install list
		RulesList &arules( getGxSession()->getRules() );
		if ( !arules.empty() )
		{
			int crcnt = 0;
			int pracnt = 0;
			FDAvp crr( getDict().avpChargingRuleRemove() );
			FDAvp prar( getDict().avpPraRemove() );
			
			for (auto r : arules)
			{
				if ( r->getType() == "CHARGING" && r->getDefaultFlag() == false )
				{
					crr.add( getDict().avpChargingRuleName(), r->getRuleName() );
					crr.addJson( r->getDefinition() );
					crcnt++;
				}
				else if ( r->getType() == "PRA" && r->getDefaultFlag() == false)
				{
					prar.add( getDict().avpPresenceReportingAreaIdentifier(), r->getRuleName() );
					prar.addJson( r->getDefinition() );
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
      setCurrentProc( new GxSessionRemoveProc( getPCRF(), this ) );
	}
	else
	if ( triggerValue == RARTrigger::triggerRARPending )
	{
		if ( !prules.empty() )
		{
			int crcnt = 0;
			int pracnt = 0;
			std::string s_pl;
			FDAvp crp( getDict().avpChargingRuleInstall() );
			FDAvp prap( getDict().avpPraInstall() );
			FDAvp qos_info( getDict().avpQosInformation() ); 
			
			for (auto r : prules)
			{
				if (r->getType() == "CHARGING")
				{
					crp.add( getDict().avpChargingRuleName(), r->getRuleName() );
					crp.addJson( r->getDefinition() );
					qos_info.addJson(  r->getDefinition() );
					doc.Parse( r->getDefinition().c_str() );
					const RAPIDJSON_NAMESPACE::Value& crditem = doc["Charging-Rule-Definition"];
					for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator crditr = crditem.MemberBegin(); crditr != crditem.MemberEnd(); ++crditr)
					{
						if ( strcmp(crditr->name.GetString(), "QoS-Information") == 0)
						{
							qci = crditr->value["QoS-Class-Identifier"].GetInt();
							const RAPIDJSON_NAMESPACE::Value& arpitem = crditr->value["Allocation-Retention-Priority"];
							pl = arpitem["Priority-Level"].GetInt();
							pec = arpitem["Pre-emption-Capability"].GetInt();
							pev = arpitem["Pre-emption-Vulnerability"].GetInt();
						}
					}
					crcnt++;
				}
				else if ( r->getType() == "PRA")
				{
					prap.add( getDict().avpPresenceReportingAreaIdentifier(), r->getRuleName() );
					prap.addJson( r->getDefinition() );
					pracnt++;
				}
			}

			if (crcnt > 0)
			{
				req->add( crp );
				req->add( qos_info );
			}
			if (pracnt > 0)
			{
				req->add( prap );
			}

			prules.clear();
		}
		FDAvp avp_qci( getDict().avpQosClassIdentifier() );
		avp_qci.set( qci );

		FDAvp avp_arp( getDict().avpAllocationRetentionPriority() );
		avp_arp.add( getDict().avpPriorityLevel(), pl );
		avp_arp.add( getDict().avpPreEmptionCapability(), pec );
		avp_arp.add( getDict().avpPreEmptionVulnerability(), pev );

		
		FDAvp defBearerQos ( getDict().avpDefaultEpsBearerQos());
		defBearerQos.add( avp_qci );
		defBearerQos.add( avp_arp );
		req->add(defBearerQos);
        
      setCurrentProc( new GxSessionInstallProc( getPCRF(), this ) );
	}

	
	//std::string json_t("{\"QoS-Class-Identifier\": 9, \"Allocation-Retention-Priority\": {\"Priority-Level\": 1, \"Pre-emption-Capability\": 2, \"Pre-emption-Vulnerability\": 20}}");
	//defBearerQos.addJson(json_t);
	setStatus( esProcessing );
	m_triggertimer = NULL;
	
	req->dump();
	req->send();
   setCurrentState( new GxSessionModifyPendingState(getPCRF(), this ) );
    
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
  
int GxIpCan1::cleanupSession()
{
    printf("%s:%d\n", __FUNCTION__,__LINE__);
	 // delete the Subscriber table which will internally delete SubscriberApn table.
	 GxSession* prevSession = NULL;
    if ( GxSessionMap::getInstance().findSession( getGxSession()->getImsi(), getGxSession()->getApn(), prevSession ) )
    {
		
		 SessionEvent* current_event = prevSession->getCurrentEvent();
    	 GxIpCan1* ipcan1 = NULL;
    	 ipcan1 = dynamic_cast<GxIpCan1*> ( current_event );
		 if ( ipcan1 != NULL )
		 {
			 if ( ipcan1->getTriggerTimer() != NULL )
			 {
			    ipcan1->getTriggerTimer()->getTimer()->stop();
				 delete( ipcan1->getTriggerTimer() );
			 }
		 }
	    Subscriber& subscriber = ( prevSession->getSubscriber() );
		 getPCRF().dataaccess().deleteSubscriber( subscriber );
	 
    	 if ( getPCRF().dataaccess().sessionExists( prevSession->getImsi(), prevSession->getApn() ) )
    	 {
          getPCRF().dataaccess().deleteSession( *prevSession );
    	 }
       GxSessionMap::getInstance().eraseSession( prevSession->getImsi(), prevSession->getApn() );
	 }
    return ValidateErrorCode::success;
}

 
int GxIpCan1::validate( gx::CreditControlRequestExtractor& ccr )
{
    printf("%s %d \n",__FUNCTION__,__LINE__);
   std::string s;
	std::string r;
    bool result = true;
    
      size_t session_list_size = getGxSession()->getRules().size();

      //
      // find the IMSI
      //
      for ( auto subidit = ccr.subscription_id.getList().begin();
            subidit != ccr.subscription_id.getList().end();
            ++subidit )
      {
         int32_t subscription_id_type;

         if ( (*subidit)->subscription_id_type.get( subscription_id_type ) && subscription_id_type == 1 ) // END_USER_IMSI
         {
            if ( !(*subidit)->subscription_id_data.get( s ) )
            {
               // subscriptionIdMissing error code
               Logger::gx().error( "%s:%d - Subscription-Id-Data missing for END_USER_IMSI", __FILE__, __LINE__ );
               RESULTCODE_WITH_FAILEDAVP1( getCCA(), DIAMETER_MISSING_AVP, *(*subidit) );
               sendCCA();
               StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_MISSING_AVP);
               return ValidateErrorCode::subscriptionIdMissing;
            }

            getGxSession()->setImsi( s );

            //
            // reurn if imsi is not valid
            //
            if ( getGxSession()->getImsi().length() != 15 )
            {
               //imsi invalid error code
               Logger::gx().error( "%s:%d - Subscription-Id END_USER_IMSI invalid value", __FILE__, __LINE__ );
               RESULTCODE_WITH_FAILEDAVP1( getCCA(), DIAMETER_INVALID_AVP_VALUE, (*subidit)->subscription_id_data );
               sendCCA();
               StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_INVALID_AVP_VALUE);
               return ValidateErrorCode::imsiInvalid ;
            }
            break;
         }
      }

      if ( !result )
      {
          return false;
      }

      //
      // return if IMSI not found or invalid
      //
      if ( getGxSession()->getImsi().empty() )
      {
         // imsi empty return code
         Logger::gx().error( "%s:%d - Subscription-Id END_USER_IMSI missing", __FILE__, __LINE__ );
         RESULTCODE_WITH_FAILEDAVP2( getCCA(), DIAMETER_MISSING_AVP, avpSubscriptionIdType(), 1 );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_MISSING_AVP);
         return ValidateErrorCode::imsiEmpty ;
      }

      //
      // get the APN
      //
      if ( !ccr.called_station_id.get( s ) )
      {
         // called station id missing error code
         Logger::gx().error( "%s:%d - Called-Station-Id missing in INITIAL_REQUEST", __FILE__, __LINE__ );
         RESULTCODE_WITH_FAILEDAVP2( getCCA(), DIAMETER_MISSING_AVP, avpCalledStationId(), "" );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_MISSING_AVP);
         return ValidateErrorCode::calledStationIdMissing ;
      }

		DefaultRule* default_rule = new DefaultRule();
		Options::getPolicesConfig().getDefaultRule( s, default_rule );
		//printf( "Default Rule Qci : %d\n", default_rule->getQci() );	
		getGxSession()->setDefaultRule( default_rule );

      getGxSession()->setApn( s );

      //
      // return if the APN is invalid
      //
      if ( getGxSession()->getApn().empty() )
      {
         //called-station-id empty error code
         Logger::gx().error( "%s:%d - Called-Station-Id is empty", __FILE__, __LINE__ );
         EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().called_station_id );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
         return ValidateErrorCode::calledStationIdEmpty ;
      }


      //
      // lookup the APN and return if it does not exist
      //
      {
         Apn *apn = NULL;
         if ( !getPCRF().getApn( getGxSession()->getApn(), apn ) )
         {
            // apn missing error code
            Logger::gx().error( "%s:%d - APN [%s] not found", __FILE__, __LINE__, getGxSession()->getApn().c_str() );
            EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, getCCR().called_station_id );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
            return ValidateErrorCode::apnMissing ;
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
            // contextExists error code
            Logger::gx().error( "%s:%d - Session ID [%s] already exists imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getSessionId().c_str(),
                  getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            return ValidateErrorCode::contextExists ;
         }
      }

      //
      // see if the session already exists in memory
      //
      {
         GxSession *session = NULL;
         if ( GxSessionMap::getInstance().findSession( getGxSession()->getImsi(), getGxSession()->getApn(), session ) )
         {
            // contextExists error code
            Logger::gx().error( "%s:%d - Session already exists imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            return ValidateErrorCode::contextExists ;
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
            return ValidateErrorCode::failure ;
         }
         // context exists error code
         Logger::gx().error( "%s:%d - Session already exists imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
         return ValidateErrorCode::contextExists ;
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
               // unableToAddNewSubscriber error code
               Logger::gx().error( "%s:%d - Unable to add new subscriber imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
               RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
               sendCCA();
               StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
               return ValidateErrorCode::unableToAddNewSubscriber ;
            }
         }
         else
         {
            // apnNotConfigured error code.
            Logger::gx().error( "%s:%d - Subscriber does not exist and APN not configured to autoadd imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.called_station_id );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
            return ValidateErrorCode::apnNotConfigured ;
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
               //unableToAssociateApnWithSubscriber
               Logger::gx().error( "%s:%d - Unable to associate APN with Subscriber imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
               EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.called_station_id );
               sendCCA();
               StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
               return ValidateErrorCode::unableToAssociateApnWithSubscriber ;
            }
         }

         //
         // assign the rules associated with the APN to the process list
         //

         getGxSession()->getRules() = sa->getComputedRules();
         getGxSession()->getRules().addGxSession( getGxSession() );
      }
#if 0

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
#endif
      {
          uint8_t ipaddr[16];
          size_t ipaddrlen = sizeof(ipaddr);

          if ( ccr.framed_ip_address.get( ipaddr, ipaddrlen ) && ipaddrlen == 4 )
          {
              if ( ipaddrlen == 4 )
              {
                  struct in_addr ipv4;
                  ipv4.s_addr = *(uint32_t*)ipaddr;
                  getGxSession()->setIPv4( ipv4, sizeof(ipv4) );
              }
              else
              {
                  //invalidIPForFrammedIPAddress
                  Logger::gx().error( "%s:%d - Invalid length for Framed-IP-Address, expected 4 found %d",
                          __FILE__, __LINE__, ipaddrlen );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.framed_ip_address );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  return ValidateErrorCode::invalidIPForFrammedIPAddress ;
              }
          }

          ipaddrlen = sizeof(ipaddr);
          if ( ccr.framed_ipv6_prefix.get( ipaddr, ipaddrlen ) )
          {
              getGxSession()->setIPv6( ipaddr, ipaddrlen );
          }

          if ( getGxSession()->getIPv4Len() == 0 && getGxSession()->getIPv6Len() == 0 )
          {
              //ipv4OrIpv6 missing
              Logger::gx().error( "%s:%d - Either Framed-IP-Address or Framed-IPv6-Prefix must be specified",
                      __FILE__, __LINE__ );
              EXP_RESULTCODE_WITH_FAILEDAVP2( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, avpFramedIpAddress(), "" );
              sendCCA();
              StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
              return ValidateErrorCode::ipv4OrIpv6Missing ;
          }
      }
      //
      // get the PCEF supported features
      //
      {
          uint64_t supported_features = 0;

          for ( auto sfextractor : ccr.supported_features.getList() )
          {
              uint32_t flid;

              if ( sfextractor->feature_list_id.get( flid ) )
              {
                  if ( flid < 1 || flid > 2 )
                  {
                      //invalidFeatureListId error code
                      Logger::gx().error( "%s:%d - Invalid Feature-List-ID [%u] for imsi=[%s] apn=[%s]",
                              __FILE__, __LINE__, flid, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
                      RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
                      sendCCA();
                      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
                      return ValidateErrorCode::invalidFeatureListId ;
                  }

                  uint32_t fl;

                  if ( sfextractor->feature_list.get( fl ) )
                      supported_features |= ( flid == 1 ? (int64_t)fl : (int64_t)fl << 32 );
              }
          }

          if ( !result )
          {
              return false;
          }

          getGxSession()->setSupportedFeatures( supported_features );
      }
#if 1
      //
      // get the PCEF endpoint
      //
      if ( ccr.origin_host.get( s ) )
      {
          Endpoint *ep = NULL;

          if ( !getPCRF().getEndpoint( s, ep ) )
          {
              ep = new Endpoint();
              ep->setHost( s );
              std::string pcef_realm;
              if ( ccr.origin_realm.get( pcef_realm ) )
              {
                 ep->setRealm( pcef_realm );
              }
              else
              {
                 ep->setRealm( Options::originRealm() );
              }

              if ( getPCRF().dataaccess().addEndpoint( *ep ) )
              {
                  if ( !getPCRF().addEndpoint( ep ) )
                  {
                      //unableToAddPcef error code
                      Logger::gx().error( "%s:%d - Unable to add PCEF endpoint [%s] to collection imsi=[%s] apn=[%s]",
                              __FILE__, __LINE__, ep->getHost().c_str(), getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
                      EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.origin_host );
                      sendCCA();
                      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                      delete ep;
                      return ValidateErrorCode::unableToAddPcef ;
                  }
              }
              else
              {
                  // unableToAddPcefInDatabase
                  Logger::gx().error( "%s:%d - Error adding PCEF endpoint [%s] to database, imsi=[%s] apn=[%s]",
                          __FILE__, __LINE__, ep->getHost().c_str(), getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.origin_host );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  delete ep;
                  return ValidateErrorCode::unableToAddPcefInDatabase ;
              }
          }

          getGxSession()->setPcefEndpoint( ep );
      }
      else
      {
          // originHostMissing
          Logger::gx().error( "%s:%d - Origin-Host missing imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
          RESULTCODE_WITH_FAILEDAVP2( getCCA(), DIAMETER_MISSING_AVP, avpOriginHost(), "" );
          sendCCA();
          StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_MISSING_AVP);
          return ValidateErrorCode::originHostMissing ;
      }
#endif
      //
      // get the TDF endpoint
      //
      if ( ccr.tdf_information.tdf_destination_host.get( s ) )
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
              if ( ccr.tdf_information.tdf_destination_realm.get( s ) )
                  ep->setRealm( s );
              if ( ccr.tdf_information.tdf_ip_address.get( s ) )
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
                          //unableToAddTdfEndpoint
                          Logger::gx().error( "%s:%d - Unable to add TDF endpoint [%s] to the internal collection",
                                  __FILE__, __LINE__, ep->getHost().c_str() );
                          EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.tdf_information.tdf_destination_host );
                          sendCCA();
                          StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                          delete ep;
                          return ValidateErrorCode::unableToAddTdfEndpoint ;
                      }
                  }
                  else
                  {
                      //unableToAddTdfEndpointToDatabase
                      Logger::gx().error( "%s:%d - Unable to add TDF endpoint [%s] to the database",
                              __FILE__, __LINE__, ep->getHost().c_str() );
                      EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.tdf_information.tdf_destination_host );
                      sendCCA();
                      StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                      delete ep;
                      return ValidateErrorCode::unableToAddTdfEndpointToDatabase ;
                  }
              }
              catch ( DAException &ex )
              {
                  // exceptionWhileAddingTdfEndpoint
                  Logger::gx().error( "%s:%d - Exception while adding TDF endpoint [%s] to the database - %s",
                          __FILE__, __LINE__, ep->getHost().c_str(), ex.what() );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.tdf_information.tdf_destination_host );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  delete ep;
                  return ValidateErrorCode::exceptionWhileAddingTdfEndpoint ;
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
                  //tdfEndpointEntryInvalid
                  Logger::gx().error( "%s:%d - TDF endpoint [%s] specified in PCEF [%s] endoint entry does not exist",
                          __FILE__, __LINE__, getGxSession()->getPcrfEndpoint()->getAssignedTdf().c_str(),
                          getGxSession()->getPcefEndpoint()->getHost().c_str() );
                  EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
                  sendCCA();
                  StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
                  return ValidateErrorCode::tdfEndpointEntryInvalid ;
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
              // tssfEndpointEntryInvalid
              Logger::gx().error( "%s:%d - TSSF endpoint [%s] specified in PCEF [%s] endoint database entry does not exist",
                      __FILE__, __LINE__, getGxSession()->getPcrfEndpoint()->getAssignedTdf().c_str(),
                      getGxSession()->getPcefEndpoint()->getHost().c_str() );
              EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
              sendCCA();
              StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
              return ValidateErrorCode::tssfEndpointEntryInvalid ;
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
               {
                  break;
               }
            }
         }

         if ( syRequired )
         {
            // syInterfaceNotSuported
            Logger::gx().error( "%s:%d - Sy interface required but not supported imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            return ValidateErrorCode::syInterfaceNotSuported ;
         }
      }

      //
      // add the session to the database
      //
      if ( !getPCRF().dataaccess().addSession( *getGxSession() ) )
      {
         // unableToAddSessionToDatabase
         Logger::gx().error( "%s:%d - Unable to add the session to the database imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         return ValidateErrorCode::unableToAddSessionToDatabase ;
      }

      //
      // all the parsing work has been completed, so add the session to the session map
      //
      if ( !GxSessionMap::getInstance().addSession( getGxSession() ) )
      {
         // unableToAddSessionInSessionMap
         Logger::gx().error( "%s:%d - Unable to insert session into session map imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
         RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
         sendCCA();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
         return ValidateErrorCode::unableToAddSessionInSessionMap ;
      }

      setDeleteGxSession( false ); std::cout<<"Calling TDF functions now"<<std::endl;

      //
      // establish the TDF session if necessary
      //
      {
         m_sdEstablishSession = new SdIpCan1EstablishSession( getPCRF(), this );
         if ( !m_sdEstablishSession )
         {
            // unableToAllocateSdIpCan1Object
            Logger::gx().error( "%s:%d - Unable to allocate SdIpCan1 object for imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            return ValidateErrorCode::unableToAllocateSdIpCan1Object ;
         }

        printf("\n calling phase1 %s %d \n",__FUNCTION__, __LINE__);
         if ( !m_sdEstablishSession->processPhase1() )
         {
            printf("\n phase1 return %s %d \n",__FUNCTION__, __LINE__);
            if ( ccr.tdf_information.tdf_destination_host.exists() )
            {
               EXP_RESULTCODE_WITH_FAILEDAVP1( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS,
                     ccr.tdf_information.tdf_destination_host );
            }
            else
            {
               EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
            }
            // sdEstablishSessionFailed
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
            return ValidateErrorCode::sdEstablishSessionFailed ;
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
            // unableToAllocateStIpCan1EstablishSessionObject
            Logger::gx().error( "%s:%d - Unable to allocate StIpCan1EstablishSession object for imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, getGxSession()->getImsi().c_str(), getGxSession()->getApn().c_str() );
            RESULTCODE( getCCA(), DIAMETER_UNABLE_TO_COMPLY );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);
            return ValidateErrorCode::unableToAllocateStIpCan1EstablishSessionObject ;
         }

        printf("\n calling phase1 %s %d \n",__FUNCTION__, __LINE__);
         if ( !m_stEstablishSession->processPhase1() )
         {
            // stEstablishSessionFailed
            EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
            sendCCA();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
            return ValidateErrorCode::stEstablishSessionFailed ;
         }
      }
      return ValidateErrorCode::success ;
}

bool GxIpCan1::processPhase1()
{
   //
   // a return value of false indicates that processing should be halted
   // the destruction sequence for the object should be initiated
   //
   SMutexLock l( m_mutex );
   bool result = true;
   int ret;

   setStatus( esProcessing );
   setGxSession( new GxSession( getPCRF(), this ) );


   ret = getCurrentState()->validateReq( getCurrentProc(), getCCR() );
	int max_call_timer = getGxSession()->getApnEntry()->getMaxCallTimerVal(); 
   switch( ret )
   {
      case ValidateErrorCode::contextExists :
      {
      	// set current state to ActivePending and again validate the req
         setCurrentState( new GxSessionActivePendingState( getPCRF(), this ) );
         ret = getCurrentState()->validateReq( getCurrentProc(), getCCR() );
			if ( ret == ValidateErrorCode::success )
			{
				setCurrentState( new GxSessionActiveState( getPCRF(), this ) );
				setCurrentProc( NULL );
			}
         result = true;
			new TriggerTimer( this, RARTrigger::triggerCallDisconnect, max_call_timer );  
         break;
       }
       case ValidateErrorCode::success :
       {	
			if ( getStatus() == esComplete )
			{
				// check whether dedicated bearer creation flag is true or false
          	 setCurrentState( new GxSessionActiveState(getPCRF(), this ) );
          	 setCurrentProc( NULL );
				 if ( getGxSession()->getApnEntry()->getDedicatedBearerCreation() )
				 {
	          // we have sent the successful CCA Initial, hence start the timer
             	Logger::gx().debug("STARTING THE TIMER AS CCA Initial is sent");
				 	int timer = getGxSession()->getApnEntry()->getTimerVal();
		       	m_triggertimer = new TriggerTimer( this, RARTrigger::triggerRARPending, timer );
				 }
	       }
          result = true;
			 new TriggerTimer( this, RARTrigger::triggerCallDisconnect, max_call_timer );  
          break;
       }
       default :
       {
          setCurrentState( NULL );
          setCurrentProc( NULL );
          result = false;
          break;
       }
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

	getCCA().add( getDict().avpEventTrigger(), EventTriggerValues :: USER_LOCATION_CHANGE );
	getCCA().add( getDict().avpEventTrigger(), EventTriggerValues :: UE_TIMEZONE_CHANGE );
	getCCA().add( getDict().avpEventTrigger(), EventTriggerValues :: USAGE_REPORT );

	FDAvp arp( getDict().avpAllocationRetentionPriority() );
	arp.add( getDict().avpPriorityLevel(), getGxSession()->getDefaultRule()->getPriorityLevel() );
   arp.add( getDict().avpPreEmptionCapability(), getGxSession()->getDefaultRule()->getPreemptionCapability() );
   arp.add( getDict().avpPreEmptionVulnerability(), getGxSession()->getDefaultRule()->getPreemptionVulnerability() );

   FDAvp defBearerQos( getDict().avpDefaultEpsBearerQos() );
	defBearerQos.add( getDict().avpQosClassIdentifier(), getGxSession()->getDefaultRule()->getQci() );
	defBearerQos.add( arp );
  // std::string json_t("{\"QoS-Class-Identifier\": 9, \"Allocation-Retention-Priority\": {\"Priority-Level\": 1, \"Pre-emption-Capability\": 1, \"Pre-emption-Vulnerability\": 1}}");
   //defBearerQos.addJson(json_t);
   getCCA().add( defBearerQos );

	FDAvp qos_info( getDict().avpQosInformation() );
	FDAvp def_arp( getDict().avpAllocationRetentionPriority() );
	//FDAvp caamb( getDict().avpConditionalApnAggregateMaxBitrate() );

	qos_info.add( getDict().avpQosClassIdentifier(), getGxSession()->getDefaultRule()->getQci() );
	
	def_arp.add( getDict().avpPriorityLevel(), getGxSession()->getDefaultRule()->getPriorityLevel() );
	def_arp.add( getDict().avpPreEmptionCapability(), getGxSession()->getDefaultRule()->getPreemptionCapability() );
	def_arp.add( getDict().avpPreEmptionVulnerability(), getGxSession()->getDefaultRule()->getPreemptionVulnerability() );

	qos_info.add( getDict().avpApnAggregateMaxBitrateUl(), getGxSession()->getDefaultRule()->getApnAggregateMaxBitrateUl() );
	qos_info.add( getDict().avpApnAggregateMaxBitrateDl(), getGxSession()->getDefaultRule()->getApnAggregateMaxBitrateDl() );

	qos_info.add( def_arp );

	getCCA().add( qos_info );


	FDAvp def_qos_info( getDict().avpQosInformation() );
	def_qos_info.add( getDict().avpQosClassIdentifier(), getGxSession()->getDefaultRule()->getQci() );
	def_qos_info.add( getDict().avpApnAggregateMaxBitrateUl(), getGxSession()->getDefaultRule()->getApnAggregateMaxBitrateUl() );
	def_qos_info.add( getDict().avpApnAggregateMaxBitrateDl(), getGxSession()->getDefaultRule()->getApnAggregateMaxBitrateDl() );
	FDAvp cri( getDict().avpChargingRuleInstall() );
	FDAvp crdef( getDict().avpChargingRuleDefinition() );
	crdef.addJson( getGxSession()->getDefaultRule()->getDefinition() );
	cri.add( getDict().avpChargingRuleName(), getGxSession()->getDefaultRule()->getRuleName() );
	cri.add( crdef );
	//cri.add( getDict().avpChargingRuleDefinition(), getGxSession()->getDefaultRule()->getDefinition() );
	cri.add( def_qos_info );
	FDAvp cri_arp( getDict().avpAllocationRetentionPriority() );

	cri_arp.add( getDict().avpPriorityLevel(), getGxSession()->getDefaultRule()->getPriorityLevel() );
   cri_arp.add( getDict().avpPreEmptionCapability(), getGxSession()->getDefaultRule()->getPreemptionCapability() );
   cri_arp.add( getDict().avpPreEmptionVulnerability(), getGxSession()->getDefaultRule()->getPreemptionVulnerability() );
	cri.add( cri_arp );
	getCCA().add( cri );

   // add the rules
	/*	
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
			{
            //getCCA().add( cri );
			}
         if ( pracnt > 0 )
			{
            //getCCA().add( prai );
			}
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
			{
            //getCCA().add( crr );
			}
         if ( pracnt > 0 )
			{
             //getCCA().add( prar );
			}
      }
   }
	*/
	

   // send the cca
   std::cout<<"Sending CCA-Initial \n"<<std::endl; 
   sendCCA();
   StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_SUCCESS);
   setCurrentProc( NULL );
   setCurrentState( new GxSessionActiveState( getPCRF(), this ) );

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
