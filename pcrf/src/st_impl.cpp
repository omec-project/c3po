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
#include "st_impl.h"
#include "statpcrf.h"

namespace st {

// Member functions that customize the individual application
Application::Application( PCRF &pcrf )
    : ApplicationBase()
    //, m_cmd_tsr( *this )
    //, m_cmd_setr( *this )
    //, m_cmd_tnr( *this )
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
    std::cout << "Registering st command handlers" << std::endl;
    //std::cout << "Registering TSR command handler" << std::endl;
    //registerHandler( m_cmd_tsr );
    //std::cout << "Registering SETR command handler" << std::endl;
    //registerHandler( m_cmd_setr );
    //std::cout << "Registering TNR command handler" << std::endl;
    //registerHandler( m_cmd_tnr );
}

// TSR Request (req) Command member functions


// Sends a TSR Request to the corresponding Peer
bool Application::sendTSRreq( StIpCan1EstablishSession *event )
{
   StTSRreq *s = NULL;

   try
   {
      s = createTSRreq( event );
      if ( s )
      {
         s->send();
      }
   }
   catch ( FDException &ex )
   {
      Logger::st().error( "%s:%d - FDException creating the TSR command - %s",
            __FILE__, __LINE__, ex.what() );
       delete s;
       s = NULL;
   }

   // DO NOT free the newly created StTSRreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for TSR reuqests
StTSRreq *Application::createTSRreq( StIpCan1EstablishSession *event )
{
   StTSRreq *req = new StTSRreq( *this, event );
   gx::CreditControlRequestExtractor &ccr( event->getGxEvent()->getCCR() );

   req->addSessionId( getDict().avpSessionId() );

   FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
   vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->add( vsai );

   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), event->getGxEvent()->getGxSession()->getTssfSession().getEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), event->getGxEvent()->getGxSession()->getTssfSession().getEndpoint()->getHost() );
   req->add( getDict().avpRequestType(), 0 ); // initial request
   req->add( ccr.framed_ip_address );
   req->add( ccr.framed_ipv6_prefix );
   req->add( ccr.called_station_id );

   req->dump();

   return req;
}

StTSRreq::StTSRreq( Application &app, StIpCan1EstablishSession *event )
   : TSRreq( app ),
     m_event( event )
{
}

StTSRreq::~StTSRreq()
{
}

void StTSRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();

   bool result = true;
   int32_t resultCode;
   int32_t exresultCode;
   TsAnswerExtractor tsa( ans, getDict() );

   if ( !tsa.result_code.get( resultCode ) && !tsa.experimental_result.experimental_result_code.get( resultCode ) )
   {
      Logger::st().error( "%s:%d - TSSF [%s] TS-Answer Result-Code/Experimental-Result-Code missing imsi=[%s] apn=[%s]",
            __FILE__, __LINE__,
            m_event->getGxEvent()->getGxSession()->getTssfSession().getEndpoint()->getHost().c_str(),
            m_event->getGxEvent()->getGxSession()->getImsi().c_str(),
            m_event->getGxEvent()->getGxSession()->getApn().c_str() );
      result = false;
   }
   else if ( resultCode != DIAMETER_SUCCESS )
   {
      Logger::st().error( "%s:%d - TSSF [%s] TS-Answer %s did not succeed [%d] imsi=[%s] apn=[%s]",
            __FILE__, __LINE__,
            m_event->getGxEvent()->getGxSession()->getTssfSession().getEndpoint()->getHost().c_str(),
            tsa.result_code.exists() ? "Result-Code" : "Experimental-Result-Code", resultCode,
            m_event->getGxEvent()->getGxSession()->getImsi().c_str(),
            m_event->getGxEvent()->getGxSession()->getApn().c_str() );
      result = false;

      if(tsa.result_code.exists()){
         StatsPcrf::singleton().registerStatResult(stat_pcrf_st_tsr, 0, resultCode);
      }
      else{
         StatsPcrf::singleton().registerStatResult(stat_pcrf_st_tsr, VEND_3GPP, resultCode);
      }

   }
   else
   {
      StatsPcrf::singleton().registerStatResult(stat_pcrf_st_tsr, VEND_3GPP, DIAMETER_SUCCESS);

      StSession &st( m_event->getGxEvent()->getGxSession()->getTssfSession() );

      //
      // save the Session-Id
      //
      tsa.session_id.get( st.getSessionId() );

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
               Logger::st().error( "%s:%d - Invalid TSSF Feature-List-ID [%u] for imsi=[%s] apn=[%s]",
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
         st.setSupportedFeatures( supported_features );
   }

   result = m_event->processPhase2( result );

   m_event->getGxEvent()->decrementUsage();

   if ( !result )
   {
      GxSession *gx = m_event->getGxEvent()->getGxSession();
      Logger::st().error( "%s:%d - StTSRreq::processAnswer calling delete on gxevent", __FILE__, __LINE__ );
      GxIpCan1::release( m_event->getGxEvent() );
      GxSession::teardownSession( "StTSRreq::processAnswer", gx );
   }

   //
   // do not free the event object as it will be free'ed as part of
   // the GxIpCan1 event destructor
   //
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Sends a RAR Request to the corresponding Peer
bool Application::sendRulesTSRreq( StSession &st, RulesList &installRules, RulesList &removeRules, SessionEvent *event )
{
   StRulesTSRreq *s = NULL;

   try
   {
      s = createRulesTSRreq( st, installRules, removeRules, event );
      if ( s )
      {
         s->send();
      }
   }
   catch ( FDException &ex )
   {
      Logger::st().error( "%s:%d - FDException creating the TSR rules command - %s",
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
StRulesTSRreq *Application::createRulesTSRreq( StSession &st, RulesList &installRules, RulesList &removeRules, SessionEvent *event )
{
   StRulesTSRreq *req = new StRulesTSRreq( *this, event );

   req->add( getDict().avpSessionId(), st.getSessionId() );

   FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
   vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->add( vsai );

   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), st.getEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), st.getEndpoint()->getHost() );
   req->add( getDict().avpRequestType(), 1 ); // update request

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

StRulesTSRreq::StRulesTSRreq( Application &app, SessionEvent *event )
   : TSRreq( app ),
     m_event( event )
{
}

StRulesTSRreq::~StRulesTSRreq()
{
}

void StRulesTSRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();

   bool result = true;
   int32_t resultCode;
   TsAnswerExtractor tsa( ans, getDict() );
   StSession *st = NULL;

   switch ( m_event->getEventType() )
   {
      case SessionEvent::etStIpCan1ProcessRules:
      {
         st = &((StIpCan1ProcessRules*)m_event)->getGxEvent()->getGxSession()->getTssfSession();
         break;
      }
      case SessionEvent::etStProcessRulesUpdate:
      {
         st = ((StProcessRulesUpdate*)m_event)->getStSession();
         break;
      }
      default:
      {
         std::string s;
         tsa.origin_host.get( s );
         Logger::st().error( "%s:%d - TSSF [%s] TSA unrecognized event type [%d]",
               __FILE__, __LINE__,
               s.c_str(),
               (int32_t)m_event->getEventType() );
         result = false;
      }
   }

   if ( result )
   {
      if ( !tsa.result_code.get( resultCode ) && !tsa.experimental_result.experimental_result_code.get( resultCode ) )
      {
         Logger::st().error( "%s:%d - TDF [%s] TSA Result-Code/Experimental-Result-Code missing imsi=[%s] apn=[%s]",
               __FILE__, __LINE__,
               st->getEndpoint()->getHost().c_str(),
               st->getGxSession().getImsi().c_str(),
               st->getGxSession().getApn().c_str() );
         result = false;
      }
      else if ( resultCode != DIAMETER_SUCCESS )
      {
         Logger::st().error( "%s:%d - TDF [%s] TSA %s did not succeed [%d] imsi=[%s] apn=[%s]",
               __FILE__, __LINE__,
               st->getEndpoint()->getHost().c_str(),
               tsa.result_code.exists() ? "Result-Code" : "Experimental-Result-Code", resultCode,
               st->getGxSession().getImsi().c_str(),
               st->getGxSession().getApn().c_str() );
         result = false;
         if(tsa.result_code.exists()){
            StatsPcrf::singleton().registerStatResult(stat_pcrf_st_tsr, 0, resultCode);
         }
         else{
            StatsPcrf::singleton().registerStatResult(stat_pcrf_st_tsr, VEND_3GPP, resultCode);
         }
      }
      else{
         StatsPcrf::singleton().registerStatResult(stat_pcrf_sd_tsr, 0, DIAMETER_SUCCESS);
      }
   }

   switch ( m_event->getEventType() )
   {
      case SessionEvent::etStIpCan1ProcessRules:
      {
         result = ((StIpCan1ProcessRules*)m_event)->processPhase2( result );
         ((StIpCan1ProcessRules*)m_event)->getGxEvent()->decrementUsage();
         if ( !result )
         {
            //Logger::st().error( "%s:%d - StRulesTSRreq::processAnswer calling delete on gxevent", __FILE__, __LINE__ );
            GxIpCan1::release( ((StIpCan1ProcessRules*)m_event)->getGxEvent() );
         }
         //
         // do not free the event object as it will be free'ed as part of
         // the parent event destructor
         //
         break;
      }
      case SessionEvent::etStProcessRulesUpdate:
      {
         ((StProcessRulesUpdate*)m_event)->processPhase2( result );
         delete m_event;
         break;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool Application::sendSTRreq( StSession &st, StSession::TerminationCause tc, SessionEvent *event )
{
   StSTRreq *str = NULL;

   try
   {
      str = createSTRreq( st, tc, event );
      if ( str ){
         str->send();
      }
   }
   catch ( FDException &ex )
   {
      Logger::st().error( "%s:%d - FDException creating the RAR Terminate command - %s",
            __FILE__, __LINE__, ex.what() );
       delete str;
       str = NULL;
   }

   // DO NOT free the newly created SdRARTreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return str != NULL;
}

// A factory for TSR reuqests
StSTRreq *Application::createSTRreq( StSession &st, StSession::TerminationCause tc, SessionEvent *event )
{
   StSTRreq *req = new StSTRreq( *this, event );

   req->add( getDict().avpSessionId(), st.getSessionId() );
   req->addOrigin();
   req->add( getDict().avpDestinationRealm(), st.getEndpoint()->getRealm() );
   req->add( getDict().avpDestinationHost(), st.getEndpoint()->getHost() );
   req->add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   req->add( getDict().avpTerminationCause(), (int32_t)tc );

   req->dump();

   return req;
}

StSTRreq::StSTRreq( Application &app, SessionEvent *event )
   : SETRreq( app ),
     m_event( event )
{
}

StSTRreq::~StSTRreq()
{
}

void StSTRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();

   int32_t rc;
   SessionTerminationAnswerExtractor sta( ans, getDict() );

   if ( sta.result_code.get( rc ) )
   {
      bool success = ( rc == DIAMETER_SUCCESS );

      StatsPcrf::singleton().registerStatResult(stat_pcrf_st_str, 0, rc);

      switch ( m_event->getEventType() )
      {
         case SessionEvent::etStTerminateSession:
         {
            ((StTerminateSession*)m_event)->processPhase2( success );
            break;
         }
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
 
// SETR Request (req) Command member functions

#if 0
// Sends a SETR Request to the corresponding Peer
bool Application::sendSETRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the SETR Command
    SETRreq *s = createSETRreq( peer );

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

    // DO NOT free the newly created SETRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for SETR reuqests
SETRreq *Application::createSETRreq(FDPeer &peer)
{
    //  creates the SETRreq object
    SETRreq *s = new SETRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the SETR request object

    // return the newly created request object
    return s;
}
#endif // #if 0

// A handler for Answers corresponding to this specific Request
void SETRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// SETR Command
}

// SETR Command (cmd) member function

// Function invoked when a SETR Command is received
int SETRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the SETR command.
    return -1;
}
 
// TNR Request (req) Command member functions

#if 0
// Sends a TNR Request to the corresponding Peer
bool Application::sendTNRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the TNR Command
    TNRreq *s = createTNRreq( peer );

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

    // DO NOT free the newly created TNRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for TNR reuqests
TNRreq *Application::createTNRreq(FDPeer &peer)
{
    //  creates the TNRreq object
    TNRreq *s = new TNRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the TNR request object

    // return the newly created request object
    return s;
}
#endif // #if 0

// A handler for Answers corresponding to this specific Request
void TNRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// TNR Command
}

// TNR Command (cmd) member function

// Function invoked when a TNR Command is received
int TNRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the TNR command.
    return -1;
}
 
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

}
