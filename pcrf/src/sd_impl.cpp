/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "logger.h"
#include "diaconst.h"
#include "sd_impl.h"
#include "statpcrf.h"

#define DIAMETER_SUCCESS   2001

namespace sd {

// Member functions that customize the individual application
Application::Application( PCRF &pcrf )
    : ApplicationBase()
    //, m_cmd_tsr( *this )
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
    std::cout << "Registering sd command handlers" << std::endl;
    //std::cout << "Registering TSR command handler" << std::endl;
    //registerHandler( m_cmd_tsr );
    std::cout << "Registering CCR command handler" << std::endl;
    registerHandler( m_cmd_crcr );
    //std::cout << "Registering REAR command handler" << std::endl;
    //registerHandler( m_cmd_rear );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Sends a TSR Request to the corresponding Peer
bool Application::sendTSRreq( SdIpCan1EstablishSession *event )
{
   SdTSRreq *s = NULL;

   try
   {
      s = createTSRreq( event );
      if ( s )
      {
         s->dump();
         s->send();
      }
   }
   catch ( FDException &ex )
   {
      Logger::sd().error( "%s:%d - FDException creating the TSR command - %s",
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
SdTSRreq *Application::createTSRreq( SdIpCan1EstablishSession *event )
{
   SdTSRreq *req = new SdTSRreq( *this, event );
   gx::CreditControlRequestExtractor &ccr( event->getGxEvent()->getCCR() );
   SdSession &tdf( event->getGxEvent()->getGxSession()->getTdfSession() );

   req->addSessionId( getDict().avpSessionId() );

   FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
   vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->add( vsai );

   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), tdf.getEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), tdf.getEndpoint()->getHost() );
   req->add( ccr.subscription_id );
   req->add( ccr.supported_features );
   req->add( ccr.framed_ip_address );
   req->add( ccr.framed_ipv6_prefix );
   req->add( ccr.ip_can_type );
   req->add( ccr.rat_type );
   req->add( ccr.an_trusted );
   req->add( ccr.user_equipment_info );
   req->add( ccr.qos_information );
   req->add( ccr.an_gw_address );
   req->add( ccr.tgpp_sgsn_address );
   req->add( ccr.tgpp_sgsn_ipv6_address );
   req->add( ccr.tgpp_ggsn_address );
   req->add( ccr.tgpp_ggsn_ipv6_address );
   req->add( ccr.tgpp_selection_mode );
   req->add( ccr.dynamic_address_flag );
   req->add( ccr.dynamic_address_flag_extension );
   req->add( ccr.pdn_connection_charging_id );
   req->add( ccr.tgpp_sgsn_mcc_mnc );
   req->add( ccr.rai );
   req->add( ccr.tgpp_user_location_info );
   req->add( ccr.fixed_user_location_info );
   req->add( ccr.user_csg_information );
   req->add( ccr.twan_identifier );
   req->add( ccr.tgpp_ms_timezone );
   req->add( ccr.tgpp_charging_characteristics );
   req->add( ccr.called_station_id );
   //req->add( ccr.charging_information );
   req->add( ccr.online );
   req->add( ccr.offline );
   //req->add( ccr.adc_rule_install );
   //req->add( ccr.revalidation_time );
   req->add( ccr.usage_monitoring_information );
   //req->add( ccr.csg_information_reporting );
   req->add( ccr.event_trigger );
   req->add( ccr.presence_reporting_area_information );
   req->add( ccr.logical_access_id );
   req->add( ccr.physical_access_id );
   //req->add( ccr.tgpp2_bsid );

   return req;
}

SdTSRreq::SdTSRreq( Application &app, SdIpCan1EstablishSession *event )
   : TSRreq( app ),
     m_event( event )
{
}

SdTSRreq::~SdTSRreq()
{
}

void SdTSRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();

   bool result = true;
   int32_t resultCode;
   TsAnswerExtractor tsa( ans, getDict() );

   if ( !tsa.result_code.get( resultCode ) && !tsa.experimental_result.experimental_result_code.get( resultCode ) )
   {
      Logger::sd().error( "%s:%d - TDF [%s] TS-Answer Result-Code/Experimental-Result-Code missing imsi=[%s] apn=[%s]",
            __FILE__, __LINE__,
            m_event->getGxEvent()->getGxSession()->getTdfSession().getEndpoint()->getHost().c_str(),
            m_event->getGxEvent()->getGxSession()->getImsi().c_str(),
            m_event->getGxEvent()->getGxSession()->getApn().c_str() );
      result = false;
   }
   else if ( resultCode != DIAMETER_SUCCESS )
   {
      Logger::sd().error( "%s:%d - TDF [%s] TS-Answer %s did not succeed [%d] imsi=[%s] apn=[%s]",
            __FILE__, __LINE__,
            m_event->getGxEvent()->getGxSession()->getTdfSession().getEndpoint()->getHost().c_str(),
            tsa.result_code.exists() ? "Result-Code" : "Experimental-Result-Code", resultCode,
            m_event->getGxEvent()->getGxSession()->getImsi().c_str(),
            m_event->getGxEvent()->getGxSession()->getApn().c_str() );

      if(tsa.result_code.exists()){
         StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_tsr, 0, resultCode);
      }
      else{
         StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_tsr, VEND_3GPP, resultCode);
      }
      result = false;
   }
   else
   {
      StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_tsr, 0, DIAMETER_SUCCESS);

      SdSession &sd( m_event->getGxEvent()->getGxSession()->getTdfSession() );

      //
      // save the Session-Id
      //
      tsa.session_id.get( sd.getSessionId() );

      //
      // get the SupportedFeatures
      //
      uint64_t supported_features = 0;

      for ( auto sfextractor : tsa.supported_features.getList() )
      {
         uint32_t flid;

         if ( sfextractor->feature_list_id.get( flid ) )
         {
            if ( flid != 1 )
            {
               Logger::sd().error( "%s:%d - Invalid TDF Feature-List-ID [%u] for imsi=[%s] apn=[%s]",
                     __FILE__, __LINE__, flid,
                     m_event->getGxEvent()->getGxSession()->getImsi().c_str(),
                     m_event->getGxEvent()->getGxSession()->getApn().c_str() );
               result = false;
               break;
            }

            uint32_t fl;

            if ( sfextractor->feature_list.get( fl ) )
               supported_features |= (int64_t)fl;
         }
      }

      if ( result )
         sd.setSupportedFeatures( supported_features );
   }

   result = m_event->processPhase2( result );

   m_event->getGxEvent()->decrementUsage();

   if ( !result )
   {
      GxSession *gx = m_event->getGxEvent()->getGxSession();
      Logger::sd().error( "%s:%d - SdTSRreq::processAnswer calling delete on gxevent", __FILE__, __LINE__ );
      GxIpCan1::release( m_event->getGxEvent() );
      GxSession::teardownSession( "SdTSRreq::processAnswer", gx );
   }

   //
   // do not free the event object as it will be free'ed as part of
   // the GxIpCan1 event destructor
   //
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Sends a RAR Request to the corresponding Peer
bool Application::sendRARreq( SdSession &sd, RulesList &installRules, RulesList &removeRules, SessionEvent *event )
{
   SdRARreq *s = NULL;

   try
   {
      s = createRARreq( sd, installRules, removeRules, event );
      if ( s )
      {
         s->send();
      }
   }
   catch ( FDException &ex )
   {
      Logger::sd().error( "%s:%d - FDException creating the TSR command - %s",
            __FILE__, __LINE__, ex.what() );
       delete s;
       s = NULL;
   }

   // DO NOT free the newly created SdRARreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for TSR reuqests
SdRARreq *Application::createRARreq( SdSession &sd, RulesList &installRules, RulesList &removeRules, SessionEvent *event )
{
   SdRARreq *req = new SdRARreq( *this, event );

   req->add( getDict().avpSessionId(), sd.getSessionId() );
   req->add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), sd.getEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), sd.getEndpoint()->getHost() );
   req->add( getDict().avpReAuthRequestType(), 0 ); // AUTHORIZE_ONLY

   if ( !installRules.empty() )
   {
      FDAvp adcri( getDict().avpAdcRuleInstall() );

      for ( auto r : installRules )
         adcri.addJson( r->getDefinition() );

      req->add( adcri );
   }

   if ( !removeRules.empty() )
   {
      FDAvp adcrr( getDict().avpAdcRuleRemove() );

      for ( auto r : removeRules )
         adcrr.add( getDict().avpAdcRuleName(), r->getRuleName() );

      req->add( adcrr );
   }

   req->dump();

   return req;
}

SdRARreq::SdRARreq( Application &app, SessionEvent *event )
   : REARreq( app ),
     m_event( event )
{
}

SdRARreq::~SdRARreq()
{
}

void SdRARreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();

   bool result = true;
   int32_t resultCode;
   ReAuthAnswerExtractor raa( ans, getDict() );
   SdSession *sd = NULL;

   switch ( m_event->getEventType() )
   {
      case SessionEvent::etSdIpCan1ProcessRules:
      {
         sd = &((SdIpCan1ProcessRules*)m_event)->getGxEvent()->getGxSession()->getTdfSession();
         break;
      }
      case SessionEvent::etSdProcessRulesUpdate:
      {
         sd = ((SdProcessRulesUpdate*)m_event)->getSdSession();
         break;
      }
      default:
      {
         std::string s;
         raa.origin_host.get( s );
         Logger::sd().error( "%s:%d - TDF [%s] RAA unrecognized event type [%d]",
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
         Logger::sd().error( "%s:%d - TDF [%s] RAA Result-Code/Experimental-Result-Code missing imsi=[%s] apn=[%s]",
               __FILE__, __LINE__,
               sd->getEndpoint()->getHost().c_str(),
               sd->getGxSession().getImsi().c_str(),
               sd->getGxSession().getApn().c_str() );
         result = false;
      }
      else if ( resultCode != DIAMETER_SUCCESS )
      {
         std::string s;
         raa.origin_host.get( s );
         Logger::sd().error( "%s:%d - TDF [%s] RAA %s did not succeed [%d] imsi=[%s] apn=[%s]",
               __FILE__, __LINE__,
               s.c_str(),
               raa.result_code.exists() ? "Result-Code" : "Experimental-Result-Code", resultCode,
               sd->getGxSession().getImsi().c_str(),
               sd->getGxSession().getApn().c_str() );
         result = false;

         if(raa.result_code.exists()){
            StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_rar, 0, resultCode);
         }
         else{
            StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_rar, VEND_3GPP, resultCode);
         }
      }
      else{
         StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_rar, 0, DIAMETER_SUCCESS);
      }
   }
   else {
      //stats
   }

   switch ( m_event->getEventType() )
   {
      case SessionEvent::etSdIpCan1ProcessRules:
      {
         result = ((SdIpCan1ProcessRules*)m_event)->processPhase2( result );
         ((SdIpCan1ProcessRules*)m_event)->getGxEvent()->decrementUsage();
         if ( !result )
         {
            //Logger::sd().error( "%s:%d - SdRARreq::processAnswer() calling delete on gxevent", __FILE__, __LINE__ );
            GxIpCan1::release( ((SdIpCan1ProcessRules*)m_event)->getGxEvent() );
         }
         //
         // do not free the event object as it will be free'ed as part of
         // the parent event destructor
         //
         break;
      }
      case SessionEvent::etSdProcessRulesUpdate:
      {
         ((SdProcessRulesUpdate*)m_event)->processPhase2( result );
         delete m_event;
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool Application::sendRARTreq( SdSession &session, SdSession::SessionReleaseCause src, SessionEvent *event )
{
   SdRARTreq *rar = NULL;

   try
   {
      rar = createRARTreq( session, src, event );
      if ( rar ){
         rar->send();
      }
   }
   catch ( FDException &ex )
   {
      Logger::sd().error( "%s:%d - FDException creating the RAR Terminate command - %s",
            __FILE__, __LINE__, ex.what() );
       delete rar;
       rar = NULL;
   }

   // DO NOT free the newly created SdRARTreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return rar != NULL;
}

// A factory for TSR reuqests
SdRARTreq *Application::createRARTreq( SdSession &session, SdSession::SessionReleaseCause src, SessionEvent *event )
{
   SdRARTreq *req = new SdRARTreq( *this, event );

   req->add( getDict().avpSessionId(), session.getSessionId() );
   req->add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), session.getEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), session.getEndpoint()->getHost() );
   req->add( getDict().avpReAuthRequestType(), 0 /* AUTHORIZE_ONLY */ );
   req->add( getDict().avpSessionReleaseCause(), (int32_t)src );

   req->dump();

   return req;
}

SdRARTreq::SdRARTreq( Application &app, SessionEvent *event )
   : REARreq( app ),
     m_event( event )
{
}

SdRARTreq::~SdRARTreq()
{
}

void SdRARTreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();

   ReAuthAnswerExtractor raa( ans, getDict() );
   int32_t resultCode;
   bool result;

   if ( !raa.result_code.get( resultCode ) && !raa.experimental_result.experimental_result_code.get( resultCode ) )
   {
      result = false;
   }
   else if ( resultCode != DIAMETER_SUCCESS )
   {
      result = false;

      if(raa.result_code.exists()){
         StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_rar, 0, resultCode);
      }
      else{
         StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_rar, VEND_3GPP, resultCode);
      }
   }
   else{
      StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_rar, 0, DIAMETER_SUCCESS);
      result = true;
   }

   switch ( m_event->getEventType() )
   {
      case SessionEvent::etSdTerminateSession:
      {
         ((SdTerminateSession*)m_event)->processPhase2( result );
         break;
      }
   }
   delete m_event;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////








// A handler for Answers corresponding to this specific Request
void TSRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// TSR Command
}

// TSR Command (cmd) member function

// Function invoked when a TSR Command is received
int TSRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the TSR command.
    return -1;
}
 
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
#endif // #if 0

// A handler for Answers corresponding to this specific Request
void CRCRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// CRCR Command
}

// CRCR Command (cmd) member function

// Function invoked when a CCR Command is received
int CRCRcmd::process( FDMessageRequest *req )
{
   CreditControlRequestExtractor ccr( *req, getDict() );

   int32_t cc_request_type;

   ccr.cc_request_type.get( cc_request_type );

   switch ( cc_request_type )
   {
      case 1: // INITIAL_REQUEST
      {
         break;
      }
      case 2: // UPDATE_REQUEST
      {
//         ccrUpdate( ccr, cca );
         break;
      }
      case 3: // TERMINATION_REQUEST
      {
         //
         // send back a confirmation since the SdSession was torn down based on the RART
         //
         FDMessageAnswer cca( req );
         cca.add( getDict().avpAuthApplicationId(), 1 );
         cca.addOrigin();
         cca.add( getDict().avpResultCode(), DIAMETER_SUCCESS );
         cca.add( ccr.cc_request_type );
         cca.add( ccr.cc_request_number );
         cca.send();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_ccr, 0, DIAMETER_SUCCESS);

         break;
      }
      case 4: // EVENT_REQUEST
      {
         FDMessageAnswer cca( req );
         cca.add( getDict().avpAuthApplicationId(), 1 );
         cca.addOrigin();
         RESULTCODE_WITH_FAILEDAVP1( cca, DIAMETER_INVALID_AVP_VALUE, ccr.cc_request_type );
         cca.add( ccr.cc_request_type );
         cca.add( ccr.cc_request_number );
         cca.send();
         StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_ccr, 0, DIAMETER_INVALID_AVP_VALUE);
         break;
      }
   }

   delete req;

   return 0;
}
 
// REAR Request (req) Command member functions

#if 0
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
#endif // #if 0

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


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



}
