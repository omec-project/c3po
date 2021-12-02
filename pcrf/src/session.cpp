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

GxSession::GxSession( PCRF &pcrf)
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
    mp_currentstate = GxSessionPendingState::getInstance();
    mp_currentproc  = NULL;
    m_apnentry = NULL;
    m_pcef_endpoint = NULL;
    m_pcrf_endpoint = NULL;
}

GxSession::~GxSession()
{
   m_rules.removeGxSession( this );
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
   else
   {
        sdclosed = true;
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
   else
   {
       stclosed = true;
   }

   if ( !isTerminating )
   {
      Logger::gx().warn("\ncalling terminate session %p Imsi %s ", gx, gx->getImsi().c_str());
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
         Logger::gx().warn("\ncalling phase1 %s %d",__FUNCTION__, __LINE__);
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
         Logger::gx().warn("\ncalling phase1 %s %d",__FUNCTION__, __LINE__);
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
    {
      Logger::gx().warn("\nreturn from teardownm before calling delete session sdclosed = %d stclosed %d", sdclosed, stclosed); 
      return;
    }

   Logger::gx().warn( "%s:%d - GxSession::teardownSession - source=[%s] deleting GxSession sessionid=[%s] imsi=[%s] apn=[%s]",
         __FILE__, __LINE__, source, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );

   GxSessionMap::getInstance().deleteSession( gx, false );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


GxSessionState::GxSessionState(std::string state_name)
{
	Logger::gx().debug("GxSessionState");
    m_statename = state_name;
}

GxSessionState::~GxSessionState()
{
}

int GxSessionState::rcvdRAA( GxSessionProc* current_proc, gx::ReAuthAnswerExtractor& raa )
{
	Logger::gx().debug("GxSessionState rcvdRAA");
}

int GxSessionState::handleSubCCR( GxSessionProc* current_proc, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionState handleSubCCR");
    return ValidateErrorCode::failure;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionPendingState::GxSessionPendingState() : GxSessionState("GxSessionPendingState")
{
	Logger::gx().debug("GxSessionPendingState  ");
}

GxSessionPendingState::~GxSessionPendingState()
{
}

int GxSessionPendingState::handleSubCCR( GxSessionProc* current_proc, gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionPendingState handleSubCCR ");
    int ret;
    ret = current_proc->handleProcCCR(ccr );
    return ret;
}

GxSessionActiveState::GxSessionActiveState( ) : GxSessionState("GxSessionActiveState")
{
	Logger::gx().debug("GxSessionActiveState  ");
}

GxSessionActiveState::~GxSessionActiveState()
{
	Logger::gx().debug("~GxSessionActiveState  ");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionInactiveState::GxSessionInactiveState( ) : GxSessionState("GxSessionInactiveState" )
{
	Logger::gx().debug("GxSessionInactiveState  ");
}

GxSessionInactiveState::~GxSessionInactiveState()
{
	Logger::gx().debug("~GxSessionInactiveState  ");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionModifyPendingState::GxSessionModifyPendingState( ) : GxSessionState("GxSessionModifyPendingState")
{
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
	result = current_proc->handleProcRAA(raa );
    return result;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GxSessionProc::GxSessionProc(std::string proc_name, GxSession *gx_session, SessionEvent* current_event ) : m_procname(proc_name), mp_gxsession(gx_session),mp_sessionevent( current_event )
{
	Logger::gx().debug("GxSessionProc function ");
}

GxSessionProc::~GxSessionProc()
{
	mp_sessionevent = NULL;
}

int GxSessionProc::handleProcRAA(gx::ReAuthAnswerExtractor& raa )
{
	Logger::gx().debug("GxSessionProc accept raa ");
}

int GxSessionProc::handleProcCCR(gx::CreditControlRequestExtractor& ccr )
{
	Logger::gx().debug("GxSessionProc accept  ccr");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

RARRuleInstallProc::RARRuleInstallProc(GxSession *gx_session, SessionEvent* current_event ) : GxSessionProc("RARRuleInstallProc", gx_session, current_event )
{
	Logger::gx().debug("RARRuleInstallProc ");
}

RARRuleInstallProc::~RARRuleInstallProc()
{
	Logger::gx().debug("RARRuleInstallProc destructor ");
}

int RARRuleInstallProc::handleProcRAA(gx::ReAuthAnswerExtractor& raa )
{
    int result;
    GxIpCan1* ipcan1 = NULL;
    SessionEvent* event = getEvent();
    ipcan1 = dynamic_cast<GxIpCan1*>( event );
    if (ipcan1 != NULL )
    {
        result = ipcan1->rcvdInstallRAA( raa );
    }
	Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");
    return result;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

RARRuleRemoveProc::RARRuleRemoveProc( GxSession *gx_session, SessionEvent* current_event ) : GxSessionProc("RARRuleRemoveProc",gx_session, current_event )
{
	Logger::gx().debug("RARRuleRemoveProc ");
}

RARRuleRemoveProc::~RARRuleRemoveProc()
{
	Logger::gx().debug("~RARRuleRemoveProc ");
}

int RARRuleRemoveProc::handleProcRAA(gx::ReAuthAnswerExtractor& raa )
{
    int result;
    GxIpCan1* ipcan1 = NULL;
    SessionEvent* event = getEvent();
    ipcan1 = dynamic_cast<GxIpCan1*>( event );
    if ( ipcan1 != NULL )
    {
        result = ipcan1->rcvdRemoveRAA( raa );
    }
	// extraction of avps for remove raa here
	Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GxSessionCreateProc::GxSessionCreateProc(GxSession *gx_session, SessionEvent* current_event ) : GxSessionProc("GxSessionCreateProc",gx_session, current_event )
{
	Logger::gx().debug("GxSessionCreateProc ");
}

GxSessionCreateProc::~GxSessionCreateProc()
{
	Logger::gx().debug("~GxSessionCreateProc ");
}

int GxSessionCreateProc::handleProcCCR(gx::CreditControlRequestExtractor& ccr )
{
    int ret;
    /*
     * SessionEvent class should have the called function virtual
     * and all the Event should override these virtual methods
     * Remove the dynamic cast when this code will be changed
     */

    SessionEvent* event = getEvent();
    GxIpCan1* ipcan1 = NULL;
    ipcan1 = dynamic_cast<GxIpCan1*> ( event );
    if ( ipcan1 )
    {
       ret = ipcan1->validate( ccr );
       return ret;
    }
	Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");
    return ValidateErrorCode::failure;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

RARSessionRemoveProc::RARSessionRemoveProc(GxSession *gx_session, SessionEvent* current_event ) : GxSessionProc("RARSessionRemoveProc",gx_session, current_event )
{
	Logger::gx().debug("RARSessionRemoveProc ");
}

RARSessionRemoveProc::~RARSessionRemoveProc()
{
	Logger::gx().debug("~RARSessionRemoveProc ");
}

int RARSessionRemoveProc::handleProcRAA( gx::ReAuthAnswerExtractor& raa )
{
	int ret;
    GxIpCan1* ipcan1 = NULL;
    SessionEvent* event = getEvent();
    ipcan1 = dynamic_cast<GxIpCan1*>( event );
    if ( ipcan1 != NULL )
    {
        ret = ipcan1->rcvdRemoveRAA( raa );
    }
	// extraction of avps for remove raa here
	Logger::gx().debug("Ignoring this event as it is not GxIpCan1 type ");

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
   auto ret = m_imsiApn_session.insert( std::pair<GxSessionKey,GxSession*>( sk, session ) );

   std::cout<<"\naddSesion IMSI - "<<session->getImsi()<<" APN "<<session->getApn()<<", session "<<session<<", map size "<<m_imsiApn_session.size()<<std::endl;
   if ( ret.second )
   {
      std::cout<<"\nsuccess add imsi in gxSessionMap session IMSI - "<<session->getImsi()<<" APN "<<session->getApn()<<std::endl;
      auto sesins = m_sessions.insert( session );

      if ( sesins.second )
      {
        std::cout<<"\nsuccess1 to add imsi in session IMSI - "<<session->getImsi()<<" APN "<<session->getApn()<<std::endl;
         auto sidins = m_sessionids.insert( std::pair<std::string,GxSession*>( session->getSessionId(), session ) );
         result = sidins.second;

         if ( !result )
         {
            // remove from the main map
            m_imsiApn_session.erase( ret.first );
            // remove from the set
            m_sessions.erase( session );
            std::cout<<"failed to add imsi in sessionid IMSI - "<<session->getImsi()<<" APN "<<session->getApn()<<" sessionid "<<session->getSessionId()<<std::endl;
         } else {
            std::cout<<"success to add imsi in sessionid IMSI - "<<session->getImsi()<<" APN "<<session->getApn()<<" sessionid "<<session->getSessionId()<<std::endl;
        }
      }
      else
      {
        std::cout<<"failed to add imsi in session IMSI - "<<session->getImsi()<<" APN "<<session->getApn()<<" sessionid "<<session->getSessionId()<<std::endl;
         result = false;
         // remove from the main map
         m_imsiApn_session.erase( ret.first );
      }
   }
   else
   {
	  GxSession *temp_session = NULL;
      GxSessionMap::getInstance().findSession(session->getImsi(), session->getApn(), temp_session);
      Logger::gx().warn("\nfailed to add imsi in gxSessionMap IMSI - %s, APN = %s, existing session %p, new session %p", session->getImsi().c_str(), session->getApn().c_str(), temp_session, session);
      result = false;
   }

   return result;
}

bool GxSessionMap::findSession( const std::string &imsi, const std::string &apn, GxSession* &session, bool lock )
{
   SMutexLock l( m_mutex, lock );
   GxSessionKey k( imsi, apn );

#ifdef DEBUG_LOCAL
   std::cout<<"\nPrint map findSession";
   printImsiApnMap();
   std::cout<<"\nPrint map findSession end";
#endif

   auto it = m_imsiApn_session.find( k );
   if ( it == m_imsiApn_session.end() )
   {
      std::cout<<"failed to find imsi in session IMSI - "<<imsi<<" APN "<<apn<<std::endl;
      return false;
   }

   session = it->second;
   std::cout<<"1findSession success session for imsi "<<imsi<<" APN "<<apn<<" gx Session "<<session<<" map size "<<m_imsiApn_session.size()<<std::endl;
   return true;
}

bool GxSessionMap::findSession( const std::string &sessionid, GxSession* &session, bool lock )
{
   SMutexLock l( m_mutex, lock );

   auto it = m_sessionids.find( sessionid );
   if ( it == m_sessionids.end() )
   {
     std::cout<<"failed to find sessionid  "<<sessionid<<std::endl;
     return false;
   }

   session = it->second;
   std::cout<<"2findSession success session for sessionid "<<sessionid<<" map size "<<m_imsiApn_session.size()<<std::endl;
   return true;
}

bool GxSessionMap::eraseSession( const std::string &imsi, const std::string &apn, bool lock )
{
    SMutexLock l( m_mutex, lock );
#ifdef DEBUG_LOCAL
    std::cout<<"\nPrint map eraseSession";
    printImsiApnMap();
    std::cout<<"\nPrint map eraseSession end";
#endif

    Logger::gx().warn( "%s:%d - GxSessionMap::eraseSessio session imsi =[%s] apn=[%s] Map size = %lu ",
                __FILE__, __LINE__, imsi.c_str(), apn.c_str(), m_imsiApn_session.size());
    GxSessionKey sk( imsi, apn );
    GxSession* gx = NULL;
    auto it = m_imsiApn_session.find( sk );
    if ( it != m_imsiApn_session.end() )
    {
        gx = it->second;
        std::cout<<"\neraseSession Gx session "<<gx<<" Imsi "<<gx->getImsi()<<" APN "<<gx->getApn();
        //erase( it );
    }
    else {
        std::cout<<"\neraseSession not found session IMSI "<<imsi<<" APN "<<apn;
    }
    if ( gx != NULL )
    {
       Logger::gx().warn("\nEraseSession : Session Not Null deleting from maps :%s:%d", __FILE__, __LINE__);
       if ( m_sessions.erase( gx ) == 1 )
       {
          Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_sessions sessionid=[%s] imsi=[%s] apn=[%s]",
                __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
       }
       if ( m_sessionids.erase( gx->getSessionId() ) == 1 )
       {
          Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_sessionids sessionid=[%s] imsi=[%s] apn=[%s]",
                __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
       }
       if ( m_terminating.erase( gx ) == 1 )
       {
          Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_terminating sessionid=[%s] imsi=[%s] apn=[%s]",
                __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
       }
       m_imsiApn_session.erase( it );
    }
    Logger::gx().warn( "End %s:%d - GxSessionMap::eraseSessio session imsi =[%s] apn=[%s] Map size = %lu ",
                __FILE__, __LINE__, imsi.c_str(), apn.c_str(), m_imsiApn_session.size());

#ifdef DEBUG_LOCAL
    std::cout<<"\nPrint map eraseSession2";
    printImsiApnMap();
    std::cout<<"\nPrint map eraseSession end2";
#endif
    return ValidateErrorCode::success ;
}

bool GxSessionMap::isTerminating( GxSession *session, bool lock )
{
   SMutexLock l( m_mutex, lock );
   Logger::gx().warn("\nisTerminate Gx session %p, Imsi = %s APN = %s ", session, session->getImsi().c_str(), session->getApn().c_str());
   return m_terminating.find( session ) != m_terminating.end();
}

void GxSessionMap::terminateSession( GxSession *gx )
{
   if ( !exists( gx, false ) )
      return;

#ifdef DEBUG_LOCAL
   std::cout<<"\nPrint map terminateSession2";
   printImsiApnMap();
   std::cout<<"\nPrint map terminateSession end2";
#endif
   Logger::gx().warn("\nTerminate Gx session %p, Imsi = %s Apn = %s Map size = %lu ",gx, gx->getImsi().c_str(), gx->getApn().c_str(), m_imsiApn_session.size());

   GxSessionKey sk( gx->getImsi(), gx->getApn() );
   auto it = m_imsiApn_session.find( sk );
   if ( it != m_imsiApn_session.end() )
   {
      m_imsiApn_session.erase( it );
      Logger::gx().warn( "%s:%d - GxSessionMap::terminateSession() erasing map entry for sessionis=[%s] imsi=[%s] apn=[%s] Map size = %lu",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str(), m_imsiApn_session.size() );
   }
   else {
      Logger::gx().warn( "find failed %s:%d - GxSessionMap::terminateSession() erasing map entry for sessionis=[%s] imsi=[%s] apn=[%s] Map size = %lu",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str(), m_imsiApn_session.size() );
   }

   if ( m_sessions.erase( gx ) == 1 )
   {
      Logger::gx().warn( "%s:%d - GxSessionMap::terminateSession() session erased from m_sessions sessionid=[%s] imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   }

   if ( m_sessionids.erase( gx->getSessionId() ) == 1 )
   {
      Logger::gx().warn( "%s:%d - GxSessionMap::terminateSession() session erased from m_sessionids sessionid=[%s] imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );

   }

   auto itterm = m_terminating.find( gx );
   if ( itterm == m_terminating.end() )
   {
      Logger::gx().warn( "%s:%d - GxSessionMap::terminateSession() adding session to terminating list sessionid=[%s] imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
      m_terminating.insert( gx );
   }

#ifdef DEBUG_LOCAL
   std::cout<<"\nPrint map terminateSession3";
   printImsiApnMap();
   std::cout<<"\nPrint map terminateSession end3";
#endif
}

void GxSessionMap::deleteSession( GxSession *gx, bool lock )
{
   SMutexLock l( m_mutex, lock );

#ifdef DEBUG_LOCAL
   std::cout<<"\nPrint map deleteSession start";
   printImsiApnMap();
   std::cout<<"\nPrint map deleteSession end";
#endif

   GxSessionKey sk( gx->getImsi(), gx->getApn() );
   auto it = m_imsiApn_session.find( sk );
   if ( it != m_imsiApn_session.end() )
   {
      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() erasing map entry for sessionis=[%s] imsi=[%s] apn=[%s] Size = %lu ",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str(), m_imsiApn_session.size() );
      m_imsiApn_session.erase( it );
      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() erasing map entry for sessionis=[%s] imsi=[%s] apn=[%s] Size = %lu ",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str(), m_imsiApn_session.size() );
   }

   if ( m_sessions.erase( gx ) == 1 )
   {
      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_sessions sessionid=[%s] imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   }

   if ( m_sessionids.erase( gx->getSessionId() ) == 1 )
   {
      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_sessionids sessionid=[%s] imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   }

   if ( m_terminating.erase( gx ) == 1 )
   {
      Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() session erased from m_terminating sessionid=[%s] imsi=[%s] apn=[%s]",
            __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );
   }

   Logger::gx().warn( "%s:%d - GxSessionMap::deleteSession() deleting GxSession object for sessionid=[%s] imsi=[%s] apn=[%s]",
         __FILE__, __LINE__, gx->getSessionId().c_str(), gx->getImsi().c_str(), gx->getApn().c_str() );

#ifdef DEBUG_LOCAL
   std::cout<<"\nPrint map deleteSession start1";
   printImsiApnMap();
   std::cout<<"\nPrint map deleteSession end2";
#endif
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
			case RARTrigger::triggerActivationTimerExpire : 
			{
				postMessage( MaxCallDurationTimeout );
				break;
			}
		}
   }
}

// ajay : not clear flow 
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
			m_gxipcan1->sendRAR( RARTrigger::triggerActivationTimerExpire );
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
     m_stProcessRules( NULL ),
     m_triggertimer( NULL )
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
		delete( m_triggertimer ); // BUG - crash 
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
   RARSessionRemoveProc* gx_proc = NULL;
   GxSessionProc* current_proc = NULL;
   Logger::gx().debug("RCVD RAA FROM PEER");
   gx::ReAuthAnswerExtractor raa( ans, getDict() );
   ans.dump();
   result = getCurrentProc()->handleProcRAA(raa ); 
   if ( result == ValidateErrorCode::success )
   {
		current_proc = getCurrentProc();
		if ( current_proc != NULL )
		{
			gx_proc = dynamic_cast<RARSessionRemoveProc*>( current_proc );
      	    if ( gx_proc != NULL )
      	    {
		    	setSubCurrentState( NULL ); // FIXME
             	// this is defaultRemoveProc so we need to destroy the GxSession because the default rules have been removed in pcef
             	// PCRF initiated IP-Can Session Termination
		    	cleanupSession( false );
             	GxSession::teardownSession( "PCRFCRCRcmd::process:TERMINATION_REQUEST", getGxSession(), SdSession::srcIpCanSessionTermination, StSession::tcDiameterLogout );
             	GxIpCan1::release( this );
      	    }
		    else if ( gx_proc == NULL )
		    {
      	    	setSubCurrentState(GxSessionActiveState::getInstance());
		    	setCurrentProc( NULL );
		    }
		}
   }

	/*
	current_proc = getCurrentProc();
	if ( current_proc != NULL )
	{
		gx_proc = dynamic_cast<RARSessionRemoveProc*>( current_proc );
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

	if( triggerValue == RARTrigger::triggerActivationTimerExpire )
	{
        /*
		if( getGxSession()->getDefaultRule()->getDefaultRuleFlag() == true ) */
		{
			// 3 indicates IP_CAN_SESSION_TERMINATION
			req->add( getDict().avpSessionReleaseCause(), 3 );
		}
		/*else   // ajay : FIXME - does not look right..defaultRule always have default flag set to true 
		{
			FDAvp cri ( getDict().avpChargingRuleRemove() );
   		    cri.add( getDict().avpChargingRuleName(), getGxSession()->getDefaultRule()->getRuleName() );
			req->add( cri );
		} */
		setCurrentProc( new RARSessionRemoveProc( getGxSession(), this ) );
	}
	else if ( triggerValue == RARTrigger::triggerRARInstall )
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
		setCurrentProc( new RARSessionRemoveProc( getGxSession(), this ) );
	}
	else if ( triggerValue == RARTrigger::triggerRARRemove )
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
        setCurrentProc( new RARRuleRemoveProc( getGxSession(), this ) );
	}
	else if ( triggerValue == RARTrigger::triggerRARPending )
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
        
      setCurrentProc( new RARRuleInstallProc( getGxSession(), this ) );
	}

	
	//std::string json_t("{\"QoS-Class-Identifier\": 9, \"Allocation-Retention-Priority\": {\"Priority-Level\": 1, \"Pre-emption-Capability\": 2, \"Pre-emption-Vulnerability\": 20}}");
	//defBearerQos.addJson(json_t);
	setStatus( esProcessing );
	m_triggertimer = NULL;
	
	req->dump();
	req->send();
    setSubCurrentState( GxSessionModifyPendingState::getInstance() );
    
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
  
int GxIpCan1::cleanupSession( bool terminate )
{
    Logger::gx().warn("\n%s:%d", __FUNCTION__,__LINE__);
	// delete the Subscriber table which will internally delete SubscriberApn table.
	GxSession* prevSession = NULL;
    if ( GxSessionMap::getInstance().findSession( getGxSession()->getImsi(), getGxSession()->getApn(), prevSession ) )
    {
		
         GxSessionProc *proc = prevSession->getCurrentProc();
         
         if (proc != NULL) 
         {
             SessionEvent* current_event = proc->getEvent();
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
         }
	     Subscriber& subscriber = ( prevSession->getSubscriber() );
		 getPCRF().dataaccess().deleteSubscriber( subscriber );
	 
    	 if ( getPCRF().dataaccess().sessionExists( prevSession->getImsi(), prevSession->getApn() ) )
    	 {
            getPCRF().dataaccess().deleteSession( *prevSession );
    	 }
		 if( terminate == true )
		 {
            GxSessionMap::getInstance().eraseSession( prevSession->getImsi(), prevSession->getApn() );
		 }

         std::list<DefaultRule*>::iterator itr;
         itr = prevSession->getDefaultRuleList().begin();
         // ajay : FIXME - fix rule cleanup
         while(itr != prevSession->getDefaultRuleList().end()) {
              prevSession->getDefaultRuleList().erase(itr);
              itr = prevSession->getDefaultRuleList().begin();
         }

	 }
    return ValidateErrorCode::success;
}

 
int GxIpCan1::validate( gx::CreditControlRequestExtractor& ccr )
{
    Logger::gx().warn("\n%s:%d", __FUNCTION__,__LINE__);
    std::string s;
	std::string r;
    bool result = true;
    
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

    getGxSession()->setApn( s );
    std::cout<<"\ngxSession Apn "<<getGxSession()->getApn()<< ", s = "<<s;

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

	Options::getPolicesConfig().getDefaultRules( s, getGxSession()->getDefaultRuleList());
    std::cout<<"list of rules for GxSession "<<getGxSession()->getDefaultRuleList().size() << "\n ";


    {
        // FIXME : should we add these rules as soon as new config is available 
        Apn *apn = NULL;
        if (!getPCRF().getApn( getGxSession()->getApn(), apn ) )
        {
          std::string apnName(getGxSession()->getApn());
          std::cout<<"apName "<<apnName<< " \n";
	      ServiceGroup* service_group = 	Options::getPolicesConfig().get_service_group_map(apnName);
          if(service_group != NULL)
          {
              std::cout<<"service group not null apName "<<apnName<< " \n";
              Apn *apn = new Apn();
              apn->setApn(getGxSession()->getApn());
              apn->setFailOnUninstallableRule(true);
              apn->setAutoCreateSubscriber(true);
              apn->setMembershipValue("rule_name"); // What should be base ?
              apn->setDomain ("onf");
              apn->setSySubscriptionId("");
              apn->setSyRequired(false);
              apn->setDefaultBearerCtlMode(2);
              apn->setForceDefaultBearerCtlMode(true);
              apn->setTimerVal( 100000);
              apn->setMaxCallTimerVal(4000000);
              apn->setDedicatedBearerCreation(false);

              // list of services to be enabled for this APN (service Group)
              std::list<std::string> &service_list = service_group->get_default_service_list();
              std::cout<<"service group -- service list length "<<service_list.size()<< " \n";
              for(std::list<std::string>::iterator it=service_list.begin(); it != service_list.end(); it++) 
              {
		          std::string service_val = *it;
                  // let;s get default service 
	              Service* service_default = Options::getPolicesConfig().get_service_selection_map( service_val );
	              if( service_default != NULL )
	              {
                      apn->setApnAmbrUlVal(service_default->getAmbrUl());
                      apn->setApnAmbrDlVal(service_default->getAmbrDl());
                  }

                  auto itr = service_default->get_activation_rules_list().begin();
                  for (; itr != service_default->get_activation_rules_list().end(); itr++ ) 
                  {
                    std::cout<<"service default activation rules list  \n";
                    std::string rule_name = *itr;
                    // Now let's add rules for this APN (i.e. service) 
                    Rule *rule = nullptr;
                    auto ruleit = getPCRF().getRuleMap().find(rule_name);
                    if (ruleit  == getPCRF().getRuleMap().end())
                    {
	                  ConfigRule * default_rule = Options::getPolicesConfig().get_config_rule_map(rule_name);

                      rule = new Rule();
                      rule->setRuleName(rule_name);
                      rule->setBaseName(rule_name);
                      rule->setType("CHARGING");
                      rule->setDefinition (default_rule->getDefinition());
                      rule->setTimeOfDay("");
                      rule->setUsageMonitoringInfo("");
                      rule->setSyRequired(false);
                      rule->setTimeMask(0);
                      rule->setFeatureMask(0);
                      rule->setDefaultFlag(true);
                    } else {
                      rule = ruleit->second;
                    }
                    std::cout<<"adding rule "<<rule_name<< " in the APN\n";
                    apn->addComputedRule(rule ); // collection of default & dedicated bearer 
                  }
                  std::cout<<"service default activation rules list end \n";
                  std::list<std::string> &ondemand_service_list = service_group->get_ondemand_service_list();
                  if(ondemand_service_list.size() > 0)
                  { 
                      apn->setDedicatedBearerCreation(true);
                  }
              }
              getPCRF().getApnMap().insert( std::pair<std::string,Apn*>( apn->getApn(), apn ) );
          } else {
              std::cout<<"service group null apName "<<apnName<< " \n";
          }
       }
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
          std::cout<<"subscriber apn not found create new one "<<getGxSession()->getApn()<<"\n";
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
       std::cout<<"rules list size "<<getGxSession()->getRules().size()<<"\n";
       getGxSession()->getRules().addGxSession( getGxSession() ); // add gx session in each rule
       
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
        } else {
           Logger::gx().warn("\nSkipping TDF %s %d ",__FUNCTION__, __LINE__);
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
    }  else {
           Logger::gx().warn("\nSkipping TSSF %s %d ",__FUNCTION__, __LINE__);
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
       } else {
           Logger::gx().warn("\nNo syRequired %s %d ",__FUNCTION__, __LINE__);
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

    setDeleteGxSession( false ); 
    std::cout<<"\nCalling TDF functions now";

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

       Logger::gx().warn("\ncalling phase1 m_sdEstablishSession %s %d ",__FUNCTION__, __LINE__);
       if ( !m_sdEstablishSession->processPhase1() )
       {
          Logger::gx().warn("\n phase1 return %s %d ",__FUNCTION__, __LINE__);
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
       Logger::gx().warn("\nfinished - phase1 m_sdEstablishSession %s %d ",__FUNCTION__, __LINE__);
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

       Logger::gx().warn("\ncalling - phase1 m_stEstablishSession %s %d ",__FUNCTION__, __LINE__);
       if ( !m_stEstablishSession->processPhase1() )
       {
          // stEstablishSessionFailed
          EXP_RESULTCODE( getCCA(), VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS );
          sendCCA();
          StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS);
          return ValidateErrorCode::stEstablishSessionFailed ;
       }
       Logger::gx().warn("\nfinished - phase1 m_stEstablishSession %s %d ",__FUNCTION__, __LINE__);
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
   setGxSession( new GxSession( getPCRF()) );
   GxSessionProc *proc = new GxSessionCreateProc(getGxSession(), this);
   getGxSession()->setCurrentProc(proc); // CRASH 
   
   ret = getSubCurrentState()->handleSubCCR( getCurrentProc(), getCCR() );
	//int max_call_timer = getGxSession()->getApnEntry()->getMaxCallTimerVal();
   switch( ret )
   {
      case ValidateErrorCode::contextExists :
      {
      	// set current state to ActivePending and again validate the req
         setSubCurrentState( GxSessionPendingState::getInstance( ) );
         // Assumption is IMSI set in the session 
         std::cout<<"Cleanup existing session \n";
		 ret = cleanupSession( true );
         ret = getSubCurrentState()->handleSubCCR( getCurrentProc(), getCCR() );
         if ( ret == ValidateErrorCode::success )
         {
            setSubCurrentState(GxSessionActiveState::getInstance() );
            setCurrentProc( NULL );
            result = true;
            if ( getGxSession()->getApnEntry()->getDedicatedBearerCreation() )
            {
                     // we have sent the successful CCA Initial, hence start the timer
                     Logger::gx().debug("STARTING THE TIMER AS CCA Initial is sent");
                     int timer = getGxSession()->getApnEntry()->getTimerVal();
                     //m_triggertimer = new TriggerTimer( this, RARTrigger::triggerRARPending, timer );
            }
         }
         else 
         {
            result = false;
         }
         //new TriggerTimer( this, RARTrigger::triggerCallDisconnect, max_call_timer );  
         break;
       }
       case ValidateErrorCode::success :
       {	
			if ( getStatus() == esComplete )
            {
                // check whether dedicated bearer creation flag is true or false
                setSubCurrentState( GxSessionActiveState::getInstance());
                setCurrentProc( NULL );
                if ( getGxSession()->getApnEntry()->getDedicatedBearerCreation() )
                {
                        // we have sent the successful CCA Initial, hence start the timer
                        Logger::gx().debug("STARTING THE TIMER AS CCA Initial is sent");
                        int timer = getGxSession()->getApnEntry()->getTimerVal();
                        //m_triggertimer = new TriggerTimer( this, RARTrigger::triggerRARPending, timer );
                }
            }
            result = true;
		       //new TriggerTimer( this, RARTrigger::triggerCallDisconnect, max_call_timer );  
            break;
       }
       default :
       {
          setSubCurrentState( NULL );
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

   Logger::gx().warn( "%s:%d - GxIpCan1::processPhase2 - start", __FILE__, __LINE__ );

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
   Logger::gx().warn("\nsd st establish session skipped ");
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

   Logger::gx().warn("\ncalling rule evaluator ");
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

      Logger::gx().warn("\ncalling phase1 %s %d ",__FUNCTION__, __LINE__);
      result = m_sdProcessRules->processPhase1();
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

      Logger::gx().warn("\ncreated m_stProcessRules calling phase1 %s %d ",__FUNCTION__, __LINE__);
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
   Logger::gx().warn("\n%s %d ",__FUNCTION__,__LINE__);
   SMutexLock l( m_mutex );

   if ( getStatus() != esProcessing )
      return true;

   Logger::gx().warn("\n%s %d ",__FUNCTION__,__LINE__);
   EventStatus sdstatus = m_sdProcessRules ? m_sdProcessRules->getStatus() : esComplete;
   EventStatus ststatus = m_stProcessRules ? m_stProcessRules->getStatus() : esComplete;

   if ( ( sdstatus == esPending || sdstatus == esProcessing ) ||
        ( ststatus == esPending || ststatus == esProcessing ) )
   {
      Logger::gx().warn("\n SD ST is either pending or processing %s %d \n",__FUNCTION__,__LINE__);
      return true;
   }

   if ( sdstatus == esFailed || ststatus == esFailed )
   {
      Logger::gx().warn("\n SD ST is failed %s %d \n",__FUNCTION__,__LINE__);
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
   
   auto itr = getGxSession()->getDefaultRuleList().begin(); 
   DefaultRule *firstRule = NULL;
   while(itr != getGxSession()->getDefaultRuleList().end()) {
        firstRule =  *itr;
        break;
   }
   arp.add( getDict().avpPriorityLevel(), firstRule->getPriorityLevel() );
   arp.add( getDict().avpPreEmptionCapability(), firstRule->getPreemptionCapability() );
   arp.add( getDict().avpPreEmptionVulnerability(), firstRule->getPreemptionVulnerability() );

   FDAvp defBearerQos( getDict().avpDefaultEpsBearerQos() );
   defBearerQos.add( getDict().avpQosClassIdentifier(), firstRule->getQci() );
   defBearerQos.add( arp );
   getCCA().add( defBearerQos );

   // ajay : this is APN AMBR ..
   FDAvp qos_info( getDict().avpQosInformation() );

   qos_info.add( getDict().avpQosClassIdentifier(), firstRule->getQci() );

   FDAvp def_arp( getDict().avpAllocationRetentionPriority() );
   def_arp.add( getDict().avpPriorityLevel(), firstRule->getPriorityLevel() );
   def_arp.add( getDict().avpPreEmptionCapability(), firstRule->getPreemptionCapability() );
   def_arp.add( getDict().avpPreEmptionVulnerability(), firstRule->getPreemptionVulnerability() );
   qos_info.add( def_arp );

   qos_info.add( getDict().avpApnAggregateMaxBitrateUl(), firstRule->getApnAggregateMaxBitrateUl() );
   qos_info.add( getDict().avpApnAggregateMaxBitrateDl(), firstRule->getApnAggregateMaxBitrateDl() );


   getCCA().add( qos_info );


#if 0
   FDAvp def_qos_info( getDict().avpQosInformation() );
   def_qos_info.add( getDict().avpQosClassIdentifier(), getGxSession()->getDefaultRule()->getQci() );
   def_qos_info.add( getDict().avpApnAggregateMaxBitrateUl(), getGxSession()->getDefaultRule()->getApnAggregateMaxBitrateUl() );
   def_qos_info.add( getDict().avpApnAggregateMaxBitrateDl(), getGxSession()->getDefaultRule()->getApnAggregateMaxBitrateDl() );
#endif

   std::list<DefaultRule*>::iterator itr1;
   std::cout<<"Default rules list  size "<<getGxSession()->getDefaultRuleList().size()<<"\n";
   for(itr1 = getGxSession()->getDefaultRuleList().begin();  itr1 != getGxSession()->getDefaultRuleList().end(); itr1++) {
        DefaultRule *rule = NULL;
        rule =  *itr1;
        FDAvp crdef( getDict().avpChargingRuleDefinition() );
        crdef.addJson( rule->getDefinition() );

        FDAvp cri( getDict().avpChargingRuleInstall() );
        cri.add( getDict().avpChargingRuleName(), rule->getRuleName() );
        cri.add( crdef );
        getCCA().add( cri );
        std::cout<<"Adding rule in CRI "<<rule->getRuleName()<<"\n";
   } 


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
   // not just one rule..may be other as well?
   int deactivation_timer = firstRule->getDeactivationTimer();
   if (deactivation_timer != 0)
   {
           new TriggerTimer( this, RARTrigger::triggerActivationTimerExpire, deactivation_timer*1000 );	
   }

   std::cout<<"\nSending CCA-Initial "; 
   sendCCA();
   StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_SUCCESS);
   setCurrentProc( NULL );
   setSubCurrentState( GxSessionActiveState::getInstance( ) );

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
      Logger::gx().warn("\ntdf not required start gxEvent phase2 %s %d ",__FUNCTION__,__LINE__);
      bool result = m_gxevent->processPhase2( false );
      std::cout<<"\ngxEvent phase2 over "<<__FUNCTION__<<" line "<<__LINE__<<" result "<<result;
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

   Logger::gx().warn("\ncalling phase2 from %s %d ",__FUNCTION__,__LINE__);
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
         Logger::gx().warn("\ncalling phase2 from %s %d ",__FUNCTION__,__LINE__);
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
         Logger::gx().warn("\ncalling phase2 from %s %d ",__FUNCTION__,__LINE__);
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
         Logger::gx().warn("\ncalling gxEvent phase 3 from %s %d ",__FUNCTION__,__LINE__);
         bool result = m_gxevent->processPhase3();
         Logger::gx().warn("\nfinished gxEvent phase 3 from %s %d ",__FUNCTION__,__LINE__);
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

   Logger::gx().warn("\ncalling phase3 from %s %d ",__FUNCTION__,__LINE__);
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
         Logger::gx().warn("\ncalling phase3 from %s %d ",__FUNCTION__,__LINE__);
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

   Logger::gx().warn("\ncalling phase3 from %s %d ",__FUNCTION__,__LINE__);
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

   std::cout<<"\nfunction  "<<__FUNCTION__<<" line - "<<__LINE__;
   if ( rrules.empty() && irules.empty() )
   {
      std::cout<<"\nno rule to install sdProcess function  "<<__FUNCTION__<<" line - "<<__LINE__;
      setStatus( esComplete );
      return false; // false indicates that processing of event is to halt
   }

   if ( !tdf.required() )
   {
      std::cout<<"\nTDF not required function  "<<__FUNCTION__<<" line - "<<__LINE__;
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
        std::cout<<"\nfunction  "<<__FUNCTION__<<" line - "<<__LINE__;
    }

   std::cout<<"\nfunction  "<<__FUNCTION__<<" line - "<<__LINE__;
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
      std::cout<<"\nno rules to process on st interface ";
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
