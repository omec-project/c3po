/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "options.h"
#include "logger.h"
#include "diaconst.h"
#include "gx_impl.h"
#include "pcrf.h"
#include "statpcrf.h"
//#include "common_def.h"

#define EXP_RESULTCODE_WITH_FAILEDAVP2( __msg__, __vendorid__, __resultcode__, __avpde__, __value__ ) \
{ \
   FDAvp failedAvp( getDict().avpFailedAvp() ); \
   failedAvp.add( getDict().__avpde__, __value__ ); \
   EXP_RESULTCODE( __msg__, __vendorid__, __resultcode__ ); \
   __msg__.add( failedAvp ); \
}

namespace gx {

// Member functions that customize the individual application
Application::Application( PCRF &pcrf )
    : ApplicationBase()
      , m_cmd_crcr( *this )
    //, m_cmd_rear( *this )
      , m_pcrf( pcrf )
{
    registerHandlers();
}

Application::~Application()
{
}

void Application::registerHandlers()
{
   // Remove the comments for any Command this application should
   // listen for (receive).
   Logger::gx().startup( "%s:%d - Registering Gx command handlers", __FILE__, __LINE__ );
   Logger::gx().startup( "%s:%d - Registering CCR command handler", __FILE__, __LINE__ );
   registerHandler( m_cmd_crcr );
   //std::cout << "Registering REAR command handler" << std::endl;
   //registerHandler( m_cmd_rear );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// CRCR Request (req) Command member functions

#if 0
// Sends a CRCR Request to the corresponding Peer
bool Application::sendCRCRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the CRCR Command
    CRCRreq *s = createCRCRreq( peer );

    try
    {
         if ( s )
         {
              s->send();
         }
    }
    catch ( FDException &ex )
    {
         std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
         delete s;
         s = NULL;
    }

    // DO NOT free the newly created CRCRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for CRCR reuqests
CRCRreq *Application::createCRCRreq(FDPeer &peer)
{
    //  creates the CRCRreq object
    CRCRreq *s = new CRCRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the CRCR request object

    // return the newly created request object
    return s;
}
#endif

// A handler for Answers corresponding to this specific Request
void CRCRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// CRCR Command
}

// CRCR Command (cmd) member function

// Function invoked when a CRCR Command is received
int CRCRcmd::process( FDMessageRequest *req )
{

	printf ("SOHAN CALLBACK CALLED AFTER RECEIVING CCR\n");
   int cc_request_type;
   CreditControlRequestExtractor ccr( *req, getDict() );

   ccr.cc_request_type.get( cc_request_type );

   switch ( cc_request_type )
   {
      case 1: // INITIAL_REQUEST
      {
         req->dump();

         GxIpCan1 *ipcan1 = NULL;
         if ( !( ipcan1 = new GxIpCan1( getApplication().getPCRF(), req, getDict() ) ) )
         {
            Logger::gx().error( "%s:%d - Unable to allocate GxIpCan1 object", __FILE__, __LINE__ );

            FDMessageAnswer cca( req );

            cca.add( getDict().avpAuthApplicationId(), 1 );
            cca.addOrigin();
            cca.add( ccr.cc_request_type );
            cca.add( ccr.cc_request_number );
            cca.add( getDict().avpResultCode(), DIAMETER_UNABLE_TO_COMPLY );

            cca.send();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNABLE_TO_COMPLY);

            break;
         }

         if ( !ipcan1->processPhase1() )
         {
            GxSession::teardownSession( "PCRFCRCRcmd::process:INITIAL_REQUEST", ipcan1->getGxSession(), SdSession::srcUnspecifiedReason, StSession::tcDiameterAdministrative );
            GxIpCan1::release( ipcan1 );
         }

         break;
      }
      case 2: // UPDATE_REQUEST
      {
//         ccrUpdate( ccr, cca );
         break;
      }
      case 3: // TERMINATION_REQUEST
      {
         req->dump();

         std::string sid;
         ccr.session_id.get( sid );
         GxSession *session = NULL;
         FDMessageAnswer cca( req );

         cca.add( getDict().avpAuthApplicationId(), 1 );
         cca.addOrigin();
         cca.add( ccr.cc_request_type );
         cca.add( ccr.cc_request_number );

         if ( GxSessionMap::getInstance().findSession( sid, session ) )
         {
            GxSession::teardownSession( "PCRFCRCRcmd::process:TERMINATION_REQUEST", session, SdSession::srcIpCanSessionTermination, StSession::tcDiameterLogout );
            cca.add( getDict().avpResultCode(), DIAMETER_SUCCESS );
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_SUCCESS);
         }
         else
         {
            Logger::gx().error( "%s:%d - Session ID [%s] not found while processing Credit-Control-Request TERMINATION_REQUEST",
                  __FILE__, __LINE__, sid.c_str() );
            cca.add( getDict().avpResultCode(), DIAMETER_UNKNOWN_SESSION_ID );
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_UNKNOWN_SESSION_ID);
         }
         cca.dump();
         cca.send();

         break;
      }
      case 4: // EVENT_REQUEST
      {
         {
            req->dump();

            Logger::gx().error( "%s:%d - CCR EVENT_REQUEST not valid", __FILE__, __LINE__ );

            FDMessageAnswer cca( req );

            cca.add( getDict().avpAuthApplicationId(), 1 );
            cca.addOrigin();
            cca.add( ccr.cc_request_type );
            cca.add( ccr.cc_request_number );
            RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_INVALID_AVP_VALUE, ccr.cc_request_type );

            cca.dump();
            cca.send();
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_ccr, 0, DIAMETER_INVALID_AVP_VALUE);
         }
         break;
      }
   }

   delete req;

   return 0;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// REAR Request (req) Command member functions

// Sends a REAR Request to the corresponding Peer
bool Application::sendREARreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the REAR Command
    REARreq *s = createREARreq( peer );

    try
    {
         if ( s )
         {
              s->send();
         }
    }
    catch ( FDException &ex )
    {
         std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
         delete s;
         s = NULL;
    }

    // DO NOT free the newly created REARreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for REAR reuqests
REARreq *Application::createREARreq(FDPeer &peer)
{
    //  creates the REARreq object
    REARreq *s = new REARreq( *this );

    //TODO - Code must be added to correctly
    //         populate the REAR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void REARreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// REAR Command
}

// REAR Command (cmd) member function

// Function invoked when a REAR Command is received
int REARcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the REAR command.
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Sends a RAR Request to the corresponding Peer
bool Application::sendRulesRARreq( GxSession &gx, RulesList &installRules, RulesList &removeRules, SessionEvent *event )
{
   GxRulesRARreq *s = NULL;

   try
   {
      s = createRulesRARreq( gx, installRules, removeRules, event );
      if ( s )
      {
         s->send();
      }
   }
   catch ( FDException &ex )
   {
      Logger::st().error( "%s:%d - FDException creating the RAR rules command - %s",
            __FILE__, __LINE__, ex.what() );
       delete s;
       s = NULL;
   }

   // DO NOT free the newly created SdTSRreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for TSR reuqests
GxRulesRARreq *Application::createRulesRARreq( GxSession &gx, RulesList &installRules, RulesList &removeRules, SessionEvent *event )
{
   GxRulesRARreq *req = new GxRulesRARreq( *this, event );

   req->add( getDict().avpSessionId(), gx.getSessionId() );
   req->add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), gx.getPcefEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), gx.getPcefEndpoint()->getHost() );
   req->add( getDict().avpReAuthRequestType(), 0 ); // AUTHORIZE_ONLY

   if ( !installRules.empty() )
   {
      int crcnt = 0;
      int pracnt = 0;
      FDAvp cri( getDict().avpChargingRuleInstall() );
      FDAvp prai( getDict().avpPraInstall() );

      for ( auto r : installRules )
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
      }

      if ( crcnt > 0 )
         req->add( cri );
      if ( pracnt > 0 )
         req->add( prai );
   }

   if ( !removeRules.empty() )
   {
      int crcnt = 0;
      int pracnt = 0;
      FDAvp crr( getDict().avpChargingRuleRemove() );
      FDAvp prar( getDict().avpPraRemove() );

      for ( auto r : removeRules )
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
      }

      if ( crcnt > 0 )
         req->add( crr );
      if ( pracnt > 0 )
         req->add( prar );
   }

   req->dump();

   return req;
}

GxRulesRARreq::GxRulesRARreq( Application &app, SessionEvent *event )
   : REARreq( app ),
     m_event( event )
{
}

GxRulesRARreq::~GxRulesRARreq()
{
}

void GxRulesRARreq::processAnswer( FDMessageAnswer &ans )
{
	printf ("SOHAN : %s:%d CALLBACK CALLED AFTER RAA\n", __FILE__, __LINE__);
   ans.dump();

   bool result = true;
   int32_t resultCode;
   ReAuthAnswerExtractor raa( ans, getDict() );
   GxSession *gx = NULL;

   switch ( m_event->getEventType() )
   {
      case SessionEvent::etGxProcessRulesUpdate:
      {
         gx = ((GxProcessRulesUpdate*)m_event)->getGxSession();
         break;
      }
		case SessionEvent::etIpCan1:
		{
			gx = ((GxIpCan1*)m_event)->getGxSession();
			break;
		}
      default:
      {
         std::string s;
         raa.origin_host.get( s );
         Logger::st().error( "%s:%d - PCEF [%s] RAA unrecognized event type [%d]",
               __FILE__, __LINE__,
               s.c_str(),
               (int32_t)m_event->getEventType() );
         result = false;
      }
   }

   if ( result )
   {
      if ( !raa.result_code.get( resultCode ) && !raa.experimental_result.experimental_result_code.get( resultCode ) )
      {
         Logger::st().error( "%s:%d - PCEF [%s] RAA Result-Code/Experimental-Result-Code missing imsi=[%s] apn=[%s]",
               __FILE__, __LINE__,
               gx->getPcefEndpoint()->getHost().c_str(),
               gx->getImsi().c_str(),
               gx->getApn().c_str() );
         result = false;
      }
      else if ( resultCode != DIAMETER_SUCCESS )
      {
         Logger::st().error( "%s:%d - PCEF [%s] RAA %s did not succeed [%d] imsi=[%s] apn=[%s]",
               __FILE__, __LINE__,
               gx->getPcefEndpoint()->getHost().c_str(),
               raa.result_code.exists() ? "Result-Code" : "Experimental-Result-Code", resultCode,
               gx->getImsi().c_str(),
               gx->getApn().c_str() );

         if(raa.result_code.exists()){
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_rar, 0, resultCode);
         }
         else{
            StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_rar, VEND_3GPP, resultCode);
         }

         result = false;
      }
      else{
         StatsPcrf::singleton().registerStatResult(stat_pcrf_gx_rar, 0, DIAMETER_SUCCESS);
      }
   }

   switch ( m_event->getEventType() )
   {
      case SessionEvent::etGxProcessRulesUpdate:
      {
         ((GxProcessRulesUpdate*)m_event)->processPhase2( result );
         delete m_event;
         break;
      }
		case SessionEvent::etIpCan1:
		{
			printf ("SOHAN CALLING RCVD RAA\n");
			((GxIpCan1*)m_event)->rcvdRAA( ans );
			delete m_event;
			break;
		}
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if 0
PCRFCRCRcmd::PCRFCRCRcmd( Application &app )
   : CRCRcmd( app )
{
}

PCRFCRCRcmd::~PCRFCRCRcmd()
{
}

// Function invoked when a CRCR Command is received
int PCRFCRCRcmd::process( FDMessageRequest *req )
{
   int cc_request_type;
   CreditControlRequestExtractor ccr( *req, getDict() );

   ccr.cc_request_type.get( cc_request_type );

   switch ( cc_request_type )
   {
      case 1: // INITIAL_REQUEST
      {
         req->dump();

         GxIpCan1 *ipcan1 = NULL;
         if ( !( ipcan1 = new GxIpCan1( getPCRF(), req, getDict() ) ) )
         {
            Logger::gx().error( "%s:%d - Unable to allocate GxIpCan1 object", __FILE__, __LINE__ );

            FDMessageAnswer cca( req );

            cca.add( getDict().avpAuthApplicationId(), 1 );
            cca.addOrigin();
            cca.add( ccr.cc_request_type );
            cca.add( ccr.cc_request_number );
            cca.add( getDict().avpResultCode(), DIAMETER_UNABLE_TO_COMPLY );

            cca.send();

            break;
         }

         if ( !ipcan1->processPhase1() )
         {
            GxSession::teardownSession( "PCRFCRCRcmd::process:INITIAL_REQUEST", ipcan1->getGxSession(), SdSession::srcUnspecifiedReason, StSession::tcDiameterAdministrative );
            GxIpCan1::release( ipcan1 );
         }

         break;
      }
      case 2: // UPDATE_REQUEST
      {
//         ccrUpdate( ccr, cca );
         break;
      }
      case 3: // TERMINATION_REQUEST
      {
         req->dump();

         std::string sid;
         ccr.session_id.get( sid );
         GxSession *session = NULL;
         FDMessageAnswer cca( req );

         cca.add( getDict().avpAuthApplicationId(), 1 );
         cca.addOrigin();
         cca.add( ccr.cc_request_type );
         cca.add( ccr.cc_request_number );

         if ( GxSessionMap::getInstance().findSession( sid, session ) )
         {
            GxSession::teardownSession( "PCRFCRCRcmd::process:TERMINATION_REQUEST", session, SdSession::srcIpCanSessionTermination, StSession::tcDiameterLogout );
            cca.add( getDict().avpResultCode(), DIAMETER_SUCCESS );
         }
         else
         {
            Logger::gx().error( "%s:%d - Session ID [%s] not found while processing Credit-Control-Request TERMINATION_REQUEST",
                  __FILE__, __LINE__, sid.c_str() );
            cca.add( getDict().avpResultCode(), DIAMETER_UNKNOWN_SESSION_ID );
         }
         cca.dump();
         cca.send();

         break;
      }
      case 4: // EVENT_REQUEST
      {
         {
            req->dump();

            Logger::gx().error( "%s:%d - CCR EVENT_REQUEST not valid", __FILE__, __LINE__ );

            FDMessageAnswer cca( req );

            cca.add( getDict().avpAuthApplicationId(), 1 );
            cca.addOrigin();
            cca.add( ccr.cc_request_type );
            cca.add( ccr.cc_request_number );
            RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_INVALID_AVP_VALUE, ccr.cc_request_type );

            cca.dump();
            cca.send();
         }
         break;
      }
   }

   return 0;
}
#endif

#if 0
void PCRFCRCRcmd::ccrInitial( CreditControlRequestExtractor &ccr, FDMessageAnswer &cca )
{
   std::string s;
   CCRInitial ccri;

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
            Logger::gx().error( "%s:%d - Subscription-Id-Data missing for END_USER_IMSI", __FILE__, __LINE__ );
            RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_MISSING_AVP, *(*subidit) );
            return;
         }

         ccri.swapImsi( s );

         //
         // reurn if imsi is not valid
         //
         if ( ccri.getImsi().empty() || ccri.getImsi().size() != 10 )
         {
            Logger::gx().error( "%s:%d - Subscription-Id END_USER_IMSI invalid value", __FILE__, __LINE__ );
            RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_INVALID_AVP_VALUE, (*subidit)->subscription_id_data );
            return;
         }

         break;
      }
   }

   //
   // return if IMSI not found or invalid
   //
   if ( !ccri.imsiExists() )
   {
      Logger::gx().error( "%s:%d - Subscription-Id END_USER_IMSI missing", __FILE__, __LINE__ );
      RESULTCODE_WITH_FAILEDAVP2( cca, DIAMETER_MISSING_AVP, avpSubscriptionIdType(), 1 );
      return;
   }

   //
   // get the APN
   //
   if ( !ccr.called_station_id.get( s ) )
   {
      Logger::gx().error( "%s:%d - Called-Station-Id missing in INITIAL_REQUEST", __FILE__, __LINE__ );
      RESULTCODE_WITH_FAILEDAVP2( cca, DIAMETER_MISSING_AVP, avpCalledStationId(), "" );
      return;
   }

   ccri.swapApn( s );

   //
   // return if the APN is invalid
   //
   if ( ccri.getApn().empty() )
   {
      Logger::gx().error( "%s:%d - Called-Station-Id is empty", __FILE__, __LINE__ );
      EXP_RESULTCODE_WITH_FAILEDAVP1( cca, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.called_station_id );
      return;
   }

   //
   // lookup the APN and return if it does not exist
   //
   {
      Apn *apn = NULL;
      if ( !getPCRF().getApn( ccri.getApn(), apn ) )
      {
         Logger::gx().error( "%s:%d - APN [%s] not found", __FILE__, __LINE__, ccri.getApn().c_str() );
         EXP_RESULTCODE_WITH_FAILEDAVP1( cca, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.called_station_id );
         return;
      }

      ccri.setApnEntry( apn );
   }

   //
   // see if the session already exists in memory
   //
   {
      Session *session = NULL;
      if ( getPCRF().getSession( ccri.getImsi(), ccri.getApn(), session ) )
      {
         Logger::gx().error( "%s:%d - Session already exists imsi=[%s] apn=[%s]", __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
         RESULTCODE( cca, DIAMETER_UNABLE_TO_COMPLY );
         return;
      }
   }

   //
   // see if the session exists in the database
   //
   if ( getPCRF().dataaccess().sessionExists( ccri.getImsi(), ccri.getApn() ) )
   {
      if ( Options::selfRedirect() )
      {
         Logger::gx().error( "%s:%d - self redirect not implemented", __FILE__, __LINE__ );
         RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_UNABLE_TO_COMPLY, ccr.cc_request_type );
         return;
      }

      Logger::gx().error( "%s:%d - Session already exists imsi=[%s] apn=[%s]", __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
      RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_UNABLE_TO_COMPLY, ccr.cc_request_type );
      return;
   }

   //
   // lookup the subscriber
   //
   if ( !getPCRF().dataaccess().getSubscriber( ccri.getImsi(), ccri.getSubscriber(), getPCRF().getRules() ) )
   {
      //
      // add subscriber if APN is configured to
      //
      if ( ccri.getApnEntry()->getAutoCreateSubscriber() )
      {
         ccri.getSubscriber().setImsi( ccri.getImsi() );
         ccri.getSubscriber().setSySubscriptionId( ccri.getApnEntry()->getSySubscriptionId() );
         ccri.getSubscriber().setFailOnUninstallableRule( ccri.getApnEntry()->getFailOnUninstallableRule() );

         SubscriberApn *sa = new SubscriberApn();
         *sa = *ccri.getApnEntry();
         ccri.getSubscriber().addSubscriberApn( sa );

         if ( !getPCRF().dataaccess().addSubscriber( ccri.getSubscriber() ) )
         {
            Logger::gx().error( "%s:%d - Unable to add new subscriber imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
            RESULTCODE( cca, DIAMETER_UNABLE_TO_COMPLY );
            return;
         }
      }
      else
      {
         Logger::gx().error( "%s:%d - Subscriber does not exist and APN not configured to autoadd imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
         EXP_RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.called_station_id );
         return;
      }
   }

   {
      /*
       * If the subscriber is found and the APN does not exist under the subscriber, should they be added?
       */
      SubscriberApn *sa = NULL;

      if ( !ccri.getSubscriber().getApnPolicies().getSubscriberApn( ccri.getApn(), sa ) )
      {
         //
         // add the "APN" to the Subscriber
         //
         sa = new SubscriberApn();
         *sa = *ccri.getApnEntry();
         ccri.getSubscriber().addSubscriberApn( sa );

         if ( !getPCRF().dataaccess().addSubscriberApn( ccri.getImsi(), *sa ) )
         {
            Logger::gx().error( "%s:%d - Unable to associate APN with Subscriber imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
            EXP_RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.called_station_id );
            delete sa;
            return;
         }
      }

      //
      // assign the rules associated with the APN to the process list
      //
      ccri.getRules() = sa->getComputedRules();
   }

   //
   // create the session
   //
   {
      Session *session = new Session();

      session->setImsi( ccri.getImsi() );
      session->setApn( ccri.getApn() );
      session->setPcrfEndpoint( Options::originHost() );

      ccr.session_id.get( s );
      session->setPcefSessionId( s );

      {
         uint8_t ipaddr[16];
         size_t ipaddrlen = sizeof(ipaddr);

         if ( ccr.framed_ip_address.get( ipaddr, ipaddrlen ) && ipaddrlen == 4 )
         {
            if ( ipaddrlen == 4 )
            {
               char buf[16];
               snprintf( buf, sizeof(buf), "%u.%u.%u.%u", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3] );
               session->setIPv4( buf );
            }
            else
            {
               Logger::gx().error( "%s:%d - Invalid length for Framed-IP-Address, expected 4 found %d",
                     __FILE__, __LINE__, ipaddrlen );
               EXP_RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.framed_ip_address );
               delete session;
               return;
            }
         }

         ipaddrlen = sizeof(ipaddr);
         if ( ccr.framed_ipv6_prefix.get( ipaddr, ipaddrlen ) )
         {
            char buf[33];
            size_t buflen = 0;

            for ( size_t i = 0; i < ipaddrlen; i++ )
               buflen += snprintf( buf, sizeof(buf) - buflen, "%02x", ipaddr[i] );

            session->setIPv6( buf );
         }

         if ( session->getIPv4().empty() && session->getIPv6().empty() )
         {
            Logger::gx().error( "%s:%d - Either Framed-IP-Address or Framed-IPv6-Prefix must be specified",
                  __FILE__, __LINE__ );
            EXP_RESULTCODE_WITH_FAILEDAVP2( cca, DIAMETER_ERROR_INITIAL_PARAMETERS, avpFramedIpAddress(), "" );
            delete session;
            return;
         }
      }

      {
         //
         // get the PCEF endpoint
         //
         if ( ccr.origin_host( s ) )
         {
            Endpoint *ep = NULL;

            if ( getPCRF().getEndpoint( s, ep ) )
            {
               session->setPcefEndpoint( ep );
            }
            else
            {
               Logger::gx().error( "%s:%d - PCEF endpoint [%s] does not exist",
                     __FILE__, __LINE__, s.c_str() );
               EXP_RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.origin_host );
               delete session;
               return;
            }
         }
         else
         {
            Logger::gx().error( "%s:%d - Origin-Host missing imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
            RESULTCODE_WITH_FAILEDAVP2( cca, DIAMETER_MISSING_AVP, avpOriginHost(), "" );
            delete session;
            return;
         }

         //
         // get the TDF endpoint
         //
         if ( ccr.tdf_information.gdf_destination_host.get( s ) )
         {
            Endpoint *ep = NULL;

            if ( getPCRF.getEndpoint( s, ep ) )
            {
               session->setTdfEndpoint( ep );
            }
            else
            {
               Logger::gx().error( "%s:%d - TDF endpoint [%s] does not exist",
                     __FILE__, __LINE__, s.c_str() );
               EXP_RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_ERROR_INITIAL_PARAMETERS, ccr.tdf_information.tdf_destination_host );
               delete session;
               return;
            }
         }
         else
         {
            //
            // set the TDF endpoint from the PcrfEndpoint
            //
            if ( session->getPcrfEndpoint() && !session->getPcrfEndpoint()->getAssignedTdf().empty() )
            {
               Endpoint *ep = NULL;

               if ( getPCRF.getEndpoint( session->getPcrfEndpoint()->getAssignedTdf(), ep ) )
               {
                  session->setTdfEndpoint( ep );
               }
               else
               {
                  Logger::gx().error( "%s:%d - TDF endpoint [%s] specified in PCEF [%s] endoint entry does not exist",
                        __FILE__, __LINE__, session->getPcrfEndpoint()->getAssignedTdf().c_str(),
                        session->getPcrfEndpoint()->getEndpoint().c_str() );
                  EXP_RESULTCODE( cca, DIAMETER_ERROR_INITIAL_PARAMETERS );
                  delete session;
                  return;
               }
            }
         }

         //
         // get the TSC endpoint
         //
         if ( session->getPcrfEndpoint() && !session->getPcrfEndpoint()->getAssignedTssf().empty() )
         {
            Endpoint *ep = NULL;

            if ( getPCRF.getEndpoint( session->getPcrfEndpoint()->getAssignedTssf(), ep ) )
            {
               session->setTssfEndpoint( ep );
            }
            else
            {
               Logger::gx().error( "%s:%d - TSC endpoint [%s] specified in PCEF [%s] endoint entry does not exist",
                     __FILE__, __LINE__, session->getPcrfEndpoint()->getAssignedTdf().c_str(),
                     session->getPcrfEndpoint()->getEndpoint().c_str() );
               EXP_RESULTCODE( cca, DIAMETER_ERROR_INITIAL_PARAMETERS );
               delete session;
               return;
            }
         }
      }

      ccri.setSession( session );
      if ( !getPCRF().addSession( session ) )
      {
         Logger::gx().error( "%s:%d - Unable to insert session into session map imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
         RESULTCODE( cca, DIAMETER_UNABLE_TO_COMPLY );
         delete session;
         return;
      }
   }

   //
   // setup Sy interface if needed
   //
   {
      bool syRequired = ccri.getApnEntry()->getSyRequired();

      if ( !syRequired )
      {
         for ( auto rule : ccri.getRules() )
         {
            if ( ( syRequired = rule->getSyRequired() ) )
               break;
         }
      }

      if ( syRequired )
      {
         Logger::gx().error( "%s:%d - Sy interface required but not supported imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
         RESULTCODE( cca, DIAMETER_UNABLE_TO_COMPLY );
         getPCRF().deleteSession( ccri.getImsi(), ccri.getApn() );
         return;
      }
   }

   //
   // determine which rules need to be activated now
   //
   {
      RulesList::iterator ruleit = ccri.getRules().begin();
      while ( ruleit != ccri.getRules().end() )
      {
         if ( !activeNow( (*ruleit)->getTimeMask() ) )
         {
            ccri.addTimerRule( *ruleit );
            ruleit = ccri.eraseRule( ruleit );
         }
      }
   }

   //
   // Bearer Control Mode
   //
   if ( ccri.getApnEntry()->getForceDefaultBearerCtlMode() )
   {
      int32_t nrs;
      if ( ccr.network_request_support.get( nrs ) )
      {
         if ( nrs == NETWORK_REQUEST_SUPPORT_NOT_SUPPORTED &&
              ccri.getApnEntry()->getDefaultBearerCtlMode() == BEARER_CTL_MODE_UE_NETWORK )
         {
            Logger::gx().error( "%s:%d - Default bearer control mode mismatch for imsi=[%s] apn=[%s] Network-Request-Support=%sSUPPORTED DefaultBearerCtlMode=%s",
                  __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str(),
                  nrs == NETWORK_REQUEST_SUPPORT_NOT_SUPPORTED ? "NOT_" : "",
                  ccri.getApnEntry()->getDefaultBearerCtlMode() == BEARER_CTL_MODE_UE_ONLY ? "UE_ONLY" : "UE_NW" );
            RESULTCODE( cca, DIAMETER_UNABLE_TO_COMPLY );
            getPCRF().deleteSession( ccri.getImsi(), ccri.getApn() );
            return;
         }
      }

      cca.add( getDict().avpBearerControlMode(), ccri.getApnEntry()->getDefaultBearerCtlMode() );

      if ( ccri.getApnEntry()->getDefaultBearerCtlMode() == BEARER_CTL_MODE_UE_NETWORK )
      {
         //
         // need to get guidance for implementing steps 188-190
         //
      }
   }

   //
   // get the supported features
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
               Logger::gx().error( "%s:%d - Invalid Feature-List-ID [%u] for imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, flid, ccri.getImsi().c_str(), ccri.getApn().c_str() );
               EXP_RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_ERROR_INITIAL_PARAMETERS, *sfextractor );
               getPCRF().deleteSession( ccri.getImsi(), ccri.getApn() );
               return;
            }

            uint32_t fl;

            if ( sfextractor->feature_list.get( fl ) )
               supported_features |= ( flid == 1 ? (int64_t)fl : (int64_t)fl << 32 );
         }
      }

      ccri.setSupportedFeatures( supported_features );
   }

   //
   // process the rules list for Gx (PCEF) applicability
   //
   {
      auto ruleit = ccri.getRules().begin();
      Endpoint *tdf = ccri.getSession()->getTdfEndoint();
      Endpoint *tsc = ccri->getSession()->getTssfEnpoint();

      while ( ruleit != ccri.getRules().end() )
      {
         if ( ( (*ruleit)->getFeatureMask() & ccri.getSupportedFeatures() ) == (*ruleit)->getFeatureMask() )
         {
            ccri.addGxRule( *ruleit );
            ruleit = ccri.eraseRule( ruleit );
         }
         else if ( tdf && ( (*ruleit)->getFeatureMask() & ccri.getSession()->getTdfEndpoint()->)

         {
            ++ruleit;
         }
      }
   }

   //
   // process teh rules list for Sd (TDF) applicability
   //
   if ( !ccri.getRules().empty() && ccri.getSession()->getTdfEndpoint() )
   {
      auto ruleit = ccri.getRules().begin();

   }
}

#define CCR ccri->getCCR()
#define CCA ccri->getCCA()

#define ABORT() \
   result = false; \
   break;

bool PCRFCRCRcmd::parseCCRInitialRequest( FDMessageReuest *req, gx::Dictionary &dict, CCRInitial* &ccri )
{
   bool result = true;
   std::string s;

   while ( result )
   {
      //
      // create the CCRInitial object
      //
      if ( !( ccri = new CCRInitial( getPCRF(), req, dict ) ) )
      {
         Logger::gx().error( "%s:%d - Unable to allocate CCRInitial object", __FILE__, __LINE__ );

         CreditControlRequestExtractor ccr( req, dict );
         FDMessageAnswer cca( req );

         cca.add( dict.avpAuthApplicationId(), 1 );
         cca.addOrigin();
         cca.add( m_ccr.cc_request_type );
         cca.add( m_ccr.cc_request_number );
         cca.add( dict.avpResultCode(), DIAMETER_UNABLE_TO_COMPLY );

         cca.send();

         ABORT();
      }

      //
      // find the IMSI
      //
      for ( auto subidit = CCR.subscription_id.getList().begin();
            subidit != CCR.subscription_id.getList().end();
            ++subidit )
      {
         int32_t subscription_id_type;

         if ( (*subidit)->subscription_id_type.get( subscription_id_type ) && subscription_id_type == 1 ) // END_USER_IMSI
         {
            if ( !(*subidit)->subscription_id_data.get( s ) )
            {
               Logger::gx().error( "%s:%d - Subscription-Id-Data missing for END_USER_IMSI", __FILE__, __LINE__ );
               RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_MISSING_AVP, *(*subidit) );
               CCA.send();
               ABORT();
            }

            ccri->swapImsi( s );

            //
            // reurn if imsi is not valid
            //
            if ( ccri->getImsi().empty() || ccri->getImsi().size() != 10 )
            {
               Logger::gx().error( "%s:%d - Subscription-Id END_USER_IMSI invalid value", __FILE__, __LINE__ );
               RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_INVALID_AVP_VALUE, (*subidit)->subscription_id_data );
               CCA.send();
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
      if ( !ccri->imsiExists() )
      {
         Logger::gx().error( "%s:%d - Subscription-Id END_USER_IMSI missing", __FILE__, __LINE__ );
         RESULTCODE_WITH_FAILEDAVP2( CCA, DIAMETER_MISSING_AVP, avpSubscriptionIdType(), 1 );
         CCA.send();
         ABORT();
      }

      //
      // get the APN
      //
      if ( !CCR.called_station_id.get( s ) )
      {
         Logger::gx().error( "%s:%d - Called-Station-Id missing in INITIAL_REQUEST", __FILE__, __LINE__ );
         RESULTCODE_WITH_FAILEDAVP2( CCA, DIAMETER_MISSING_AVP, avpCalledStationId(), "" );
         CCA.send();
         ABORT();
      }

      ccri->swapApn( s );

      //
      // return if the APN is invalid
      //
      if ( ccri->getApn().empty() )
      {
         Logger::gx().error( "%s:%d - Called-Station-Id is empty", __FILE__, __LINE__ );
         EXP_RESULTCODE_WITH_FAILEDAVP1( CCA, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, CCR.called_station_id );
         CCA.send();
         ABORT();
      }

      //
      // lookup the APN and return if it does not exist
      //
      {
         Apn *apn = NULL;
         if ( !getPCRF().getApn( ccri->getApn(), apn ) )
         {
            Logger::gx().error( "%s:%d - APN [%s] not found", __FILE__, __LINE__, ccri.getApn().c_str() );
            EXP_RESULTCODE_WITH_FAILEDAVP1( CCA, VEND_3GPP, DIAMETER_ERROR_INITIAL_PARAMETERS, CCR.called_station_id );
            CCA.send();
            ABORT();
         }

         ccri->setApnEntry( apn );
      }

      //
      // see if the session already exists in memory
      //
      {
         Session *session = NULL;
         if ( getPCRF().getSession( ccri->getImsi(), ccri->getApn(), session ) )
         {
            Logger::gx().error( "%s:%d - Session already exists imsi=[%s] apn=[%s]", __FILE__, __LINE__, ccri->getImsi().c_str(), ccri->getApn().c_str() );
            RESULTCODE( CCA, DIAMETER_UNABLE_TO_COMPLY );
            CCA.send();
            ABORT();
         }
      }

      //
      // see if the session exists in the database
      //
      if ( getPCRF().dataaccess().sessionExists( ccri->getImsi(), ccri->getApn() ) )
      {
         if ( Options::selfRedirect() )
         {
            Logger::gx().error( "%s:%d - self redirect not implemented", __FILE__, __LINE__ );
            RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_UNABLE_TO_COMPLY, CCR.cc_request_type );
            CCA.send();
            ABORT();
         }

         Logger::gx().error( "%s:%d - Session already exists imsi=[%s] apn=[%s]", __FILE__, __LINE__, ccri->getImsi().c_str(), ccri->getApn().c_str() );
         RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_UNABLE_TO_COMPLY, CCR.cc_request_type );
         CCA.send();
         ABORT();
      }

      //
      // lookup the subscriber
      //
      if ( !getPCRF().dataaccess().getSubscriber( ccri->getImsi(), ccri->getSubscriber(), getPCRF().getRules() ) )
      {
         //
         // add subscriber if APN is configured to
         //
         if ( ccri->getApnEntry()->getAutoCreateSubscriber() )
         {
            ccri->getSubscriber().setImsi( ccri->getImsi() );
            ccri->getSubscriber().setSySubscriptionId( ccri->getApnEntry()->getSySubscriptionId() );
            ccri->getSubscriber().setFailOnUninstallableRule( ccri->getApnEntry()->getFailOnUninstallableRule() );

            SubscriberApn *sa = new SubscriberApn();
            *sa = *ccri->getApnEntry();
            ccri->getSubscriber().addSubscriberApn( sa );

            if ( !getPCRF().dataaccess().addSubscriber( ccri->getSubscriber() ) )
            {
               Logger::gx().error( "%s:%d - Unable to add new subscriber imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, ccri->getImsi().c_str(), ccri->getApn().c_str() );
               RESULTCODE( CCA, DIAMETER_UNABLE_TO_COMPLY );
               CCA.send();
               ABORT();
            }
         }
         else
         {
            Logger::gx().error( "%s:%d - Subscriber does not exist and APN not configured to autoadd imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
            EXP_RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS, CCR.called_station_id );
            CCA.send();
            ABORT();
         }
      }

      {
         /*
          * If the subscriber is found and the APN does not exist under the subscriber, should they be added?
          */
         SubscriberApn *sa = NULL;

         if ( !ccri->getSubscriber().getApnPolicies().getSubscriberApn( ccri->getApn(), sa ) )
         {
            //
            // add the "APN" to the Subscriber
            //
            sa = new SubscriberApn();
            *sa = *ccri.getApnEntry();
            ccri->getSubscriber().addSubscriberApn( sa );

            if ( !getPCRF().dataaccess().addSubscriberApn( ccri->getImsi(), *sa ) )
            {
               Logger::gx().error( "%s:%d - Unable to associate APN with Subscriber imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, ccri->getImsi().c_str(), ccri->getApn().c_str() );
               EXP_RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS, CCR.called_station_id );
               CCA.send();
               delete sa;
               ABORT();
            }
         }

         //
         // assign the rules associated with the APN to the process list
         //
         ccri->getRules() = sa->getComputedRules();
      }

      //
      // create the session
      //
      {
         Session *session = new Session();

         session->setImsi( ccri->getImsi() );
         session->setApn( ccri->getApn() );
         session->setPcrfEndpoint( Options::originHost() );

         CCR.session_id.get( s );
         session->setPcefSessionId( s );

         {
            uint8_t ipaddr[16];
            size_t ipaddrlen = sizeof(ipaddr);

            if ( CCR.framed_ip_address.get( ipaddr, ipaddrlen ) && ipaddrlen == 4 )
            {
               if ( ipaddrlen == 4 )
               {
                  char buf[16];
                  snprintf( buf, sizeof(buf), "%u.%u.%u.%u", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3] );
                  session->setIPv4( buf );
               }
               else
               {
                  Logger::gx().error( "%s:%d - Invalid length for Framed-IP-Address, expected 4 found %d",
                        __FILE__, __LINE__, ipaddrlen );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS, CCR.framed_ip_address );
                  CCA.send();
                  delete session;
                  ABORT();
               }
            }

            ipaddrlen = sizeof(ipaddr);
            if ( ccr.framed_ipv6_prefix.get( ipaddr, ipaddrlen ) )
            {
               char buf[33];
               size_t buflen = 0;

               for ( size_t i = 0; i < ipaddrlen; i++ )
                  buflen += snprintf( buf, sizeof(buf) - buflen, "%02x", ipaddr[i] );

               session->setIPv6( buf );
            }

            if ( session->getIPv4().empty() && session->getIPv6().empty() )
            {
               Logger::gx().error( "%s:%d - Either Framed-IP-Address or Framed-IPv6-Prefix must be specified",
                     __FILE__, __LINE__ );
               EXP_RESULTCODE_WITH_FAILEDAVP2( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS, avpFramedIpAddress(), "" );
               CCA.send();
               delete session;
               ABORT();
            }
         }

         {
            //
            // get the PCEF endpoint
            //
            if ( ccr.origin_host( s ) )
            {
               Endpoint *ep = NULL;

               if ( getPCRF().getEndpoint( s, ep ) )
               {
                  session->setPcefEndpoint( ep );
               }
               else
               {
                  Logger::gx().error( "%s:%d - PCEF endpoint [%s] does not exist",
                        __FILE__, __LINE__, s.c_str() );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS, CCR.origin_host );
                  CCA.send();
                  delete session;
                  ABORT();
               }
            }
            else
            {
               Logger::gx().error( "%s:%d - Origin-Host missing imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, ccri.getImsi().c_str(), ccri.getApn().c_str() );
               RESULTCODE_WITH_FAILEDAVP2( CCA, DIAMETER_MISSING_AVP, avpOriginHost(), "" );
               CCA.send();
               delete session;
               ABORT();
            }

            //
            // get the TDF endpoint
            //
            if ( CCR.tdf_information.gdf_destination_host.get( s ) )
            {
               Endpoint *ep = NULL;

               if ( getPCRF.getEndpoint( s, ep ) )
               {
                  session->setTdfEndpoint( ep );
               }
               else
               {
                  Logger::gx().error( "%s:%d - TDF endpoint [%s] does not exist",
                        __FILE__, __LINE__, s.c_str() );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS, CCR.tdf_information.tdf_destination_host );
                  CCA.send();
                  delete session;
                  ABORT();
               }
            }
            else
            {
               //
               // set the TDF endpoint from the PcrfEndpoint
               //
               if ( session->getPcrfEndpoint() && !session->getPcrfEndpoint()->getAssignedTdf().empty() )
               {
                  Endpoint *ep = NULL;

                  if ( getPCRF.getEndpoint( session->getPcrfEndpoint()->getAssignedTdf(), ep ) )
                  {
                     session->setTdfEndpoint( ep );
                  }
                  else
                  {
                     Logger::gx().error( "%s:%d - TDF endpoint [%s] specified in PCEF [%s] endoint entry does not exist",
                           __FILE__, __LINE__, session->getPcrfEndpoint()->getAssignedTdf().c_str(),
                           session->getPcrfEndpoint()->getEndpoint().c_str() );
                     EXP_RESULTCODE( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS );
                     delete session;
                     ABORT();
                  }
               }
            }

            //
            // get the TSC endpoint
            //
            if ( session->getPcrfEndpoint() && !session->getPcrfEndpoint()->getAssignedTssf().empty() )
            {
               Endpoint *ep = NULL;

               if ( getPCRF.getEndpoint( session->getPcrfEndpoint()->getAssignedTssf(), ep ) )
               {
                  session->setTssfEndpoint( ep );
               }
               else
               {
                  Logger::gx().error( "%s:%d - TSC endpoint [%s] specified in PCEF [%s] endoint entry does not exist",
                        __FILE__, __LINE__, session->getPcrfEndpoint()->getAssignedTdf().c_str(),
                        session->getPcrfEndpoint()->getEndpoint().c_str() );
                  EXP_RESULTCODE( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS );
                  CCA.send();
                  delete session;
                  ABORT();
               }
            }
         }

         ccri->setSession( session );
         if ( !getPCRF().addSession( session ) )
         {
            Logger::gx().error( "%s:%d - Unable to insert session into session map imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, ccri->getImsi().c_str(), ccri->getApn().c_str() );
            RESULTCODE( CCA, DIAMETER_UNABLE_TO_COMPLY );
            CCA.send();
            delete session;
            ABORT();
         }
      }

      //
      // setup Sy interface if needed
      //
      {
         bool syRequired = ccri.getApnEntry()->getSyRequired();

         if ( !syRequired )
         {
            for ( auto rule : ccri.getRules() )
            {
               if ( ( syRequired = rule->getSyRequired() ) )
                  break;
            }
         }

         if ( syRequired )
         {
            Logger::gx().error( "%s:%d - Sy interface required but not supported imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, ccri.getImsi().c_str(), ccri->getApn().c_str() );
            RESULTCODE( cca, DIAMETER_UNABLE_TO_COMPLY );
            CCA.send();
            getPCRF().deleteSession( ccri->getImsi(), ccri->getApn() );
            ABORT();
         }
      }

      //
      // get the PCEF supported features
      //
      {
         uint64_t supported_features = 0;

         for ( auto sfextractor : CCR.supported_features.getList() )
         {
            uint32_t flid;

            if ( sfextractor->feature_list_id.get( flid ) )
            {
               if ( flid < 1 || flid > 2 )
               {
                  Logger::gx().error( "%s:%d - Invalid Feature-List-ID [%u] for imsi=[%s] apn=[%s]",
                        __FILE__, __LINE__, flid, ccri->getImsi().c_str(), ccri->getApn().c_str() );
                  EXP_RESULTCODE_WITH_FAILEDAVP1( CCA, DIAMETER_ERROR_INITIAL_PARAMETERS, *sfextractor );
                  CCA.send();
                  getPCRF().deleteSession( ccri->getImsi(), ccri->getApn() );
                  ABORT();
               }

               uint32_t fl;

               if ( sfextractor->feature_list.get( fl ) )
                  supported_features |= ( flid == 1 ? (int64_t)fl : (int64_t)fl << 32 );
            }
         }

         if ( !result )
            break;

         ccri->getSession().setPcefSupportedFeatures( supported_features );
      }

      //
      // get the supported features from the TDF if necessary
      //
      if ( ccri->getSession()->getTdfEndpoint() && !ccri->getSession->getTdfEndpoint()->getSupportedFeaturePersistance() )
      {
         //
         // send TDF-Session-Request (TSR) command to TDF to retrieve Supported-Features
         //
         ccri->getSession()->setTdfSupportedFeaturesRetrieved( false );
      }
      else
      {
         ccri->getSession()->setTdfSupportedFeaturesRetrieved( true );
      }

      //
      // get the supported features from the TSSF if necessary
      //
      if ( ccri->getSession()->getTssfEndpoint() && !ccri->getSession->getTssfEndpoint()->getSupportedFeaturePersistance() )
      {
         //
         // send TDF-Session-Request (TSR) command to TSSF to retrieve Supported-Features
         //
         ccri->getSession()->setTssfSupportedFeaturesRetrieved( false );
      }
      else
      {
         ccri->getSession()->setTssfSupportedFeaturesRetrieved( true );
      }

      break;
   }

   if ( !result )
   {
      if ( ccri )
      {
         delete ccri;
         ccri = NULL;
      }
   }

   return result;
}

void PCRFCRCRcmd::processCCRInitialRequest( CCRInitial *ccri )
{
   bool keepGoing = true;
   if ( !ccri->getSession()->supportedFeaturesRetrieved() )
      return;

   //
   // determine which rules need to be activated now
   //
   {
      uint64_t pcefFeatures = ccri->getSession->getPcefSupportedFeatures();
      uint64_t tdfFeatures = ccri->getSession->getTdfSupportedFeatures();
      uint64_t tssfFeatures = ccri->getSession->getTssfSupportedFeatures();
      auto ruleit = ccri->getRules().begin();

      while ( keepGoing && ruleit != ccri->getRules().end() )
      {
         // check to see if the rule is active at this time
         if ( !activeNow( (*ruleit)->getTimeMask() ) )
         {
            ccri->addTimerRule( *ruleit );
            ruleit = ccri->eraseRule( ruleit );
         }
         // check to see if the rule applies to the PCEF
         else if ( ( (*ruleit)->getFeatureMask() & pcefFeatures ) == (*ruleit)->getFeatureMask() )
         {
            ccri->addGxRule( *ruleit );
            ruleit = ccri->eraseRule( ruleit );
         }
         // check to see if the rule applies to the TDF
         else if ( ( (*ruleit)->getFeatureMask() & tdfFeatures ) == (*ruleit)->getFeatureMask() )
         {
            ccri->addSdRule( *ruleit );
            ruleit = ccri->eraseRule( ruleit );
         }
         // check to see if the rule applies to the TDF
         else if ( ( (*ruleit)->getFeatureMask() & tssfFeatures ) == (*ruleit)->getFeatureMask() )
         {
            ccri->addStRule( *ruleit );
            ruleit = ccri->eraseRule( ruleit );
         }
         // check for fail on unassigned rule
         else if ( ccri->getSubscriber().getFailOnUninstallableRule() )
         {
            Logger::gx().error( "%s:%d - Aborting, unable to install rule=[%s] imsi=[%s] apn=[%s]",
                  __FILE__, __LINE__, flid, ccri->getImsi().c_str(), ccri->getApn().c_str(), (*ruleit)->getRuleName().c_str() );
            EXP_RESULTCODE( CCA, DIAMETER_PCC_RULE_EVENT );
            CCA.send();
            getPCRF().deleteSession( ccri->getImsi(), ccri->getApn() );
            keepGoing = false;
         }
      }

      if ( !keepGoing )
      {
         delete ccri;
         return;
      }
   }

   //
   // Bearer Control Mode
   //
   if ( ccri->getApnEntry()->getForceDefaultBearerCtlMode() )
   {
#if 0
      //
      // need guidance for applicability steps 173-181
      //
      int32_t nrs;
      if ( CCR.network_request_support.get( nrs ) )
      {
         if ( nrs == NETWORK_REQUEST_SUPPORT_NOT_SUPPORTED &&
              ccri->getApnEntry()->getDefaultBearerCtlMode() == BEARER_CTL_MODE_UE_NETWORK )
         {
            Logger::gx().error( "%s:%d - Default bearer control mode mismatch for imsi=[%s] apn=[%s] Network-Request-Support=%sSUPPORTED DefaultBearerCtlMode=%s",
                  __FILE__, __LINE__, ccri->getImsi().c_str(), ccri->getApn().c_str(),
                  nrs == NETWORK_REQUEST_SUPPORT_NOT_SUPPORTED ? "NOT_" : "",
                  ccri.getApnEntry()->getDefaultBearerCtlMode() == BEARER_CTL_MODE_UE_ONLY ? "UE_ONLY" : "UE_NW" );
            RESULTCODE( CCA, DIAMETER_UNABLE_TO_COMPLY );
            CCA.send();
            getPCRF().deleteSession( ccri->getImsi(), ccri->getApn() );

            ABORT();
         }
      }
#endif

      CCA.add( ccri->getDict().avpBearerControlMode(), ccri->getApnEntry()->getDefaultBearerCtlMode() );

      if ( ccri->getApnEntry()->getDefaultBearerCtlMode() == BEARER_CTL_MODE_UE_NETWORK )
      {
         //
         // need guidance for implementing steps 188-190
         //
      }
   }

   //
   // add the rules
   //
   if ( ccri)


}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void PCRFCRCRcmd::ccrUpdate( CreditControlRequestExtractor &ccr, FDMessageAnswer &cca )
{
}

void PCRFCRCRcmd::ccrTermination( CreditControlRequestExtractor &ccr, FDMessageAnswer &cca )
{
}

bool PCRFCRCRcmd::activeNow( int64_t timeMask )
{
   if ( !timeMask )
      return true;

   // need to add logic to evaluate time

   return true;
}

#endif // #if 0

}
