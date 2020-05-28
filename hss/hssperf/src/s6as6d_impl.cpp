/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "s6as6d_impl.h"
#include "ulrtest.h"
#include "airtest.h"
#include "attachtest.h"

#define S6A_ULR_FLAGS_SINGLE_REGSISTRATION_INDICATION (1 << 0)
#define S6A_ULR_FLAGS_S6AS6D_INDICATOR                  (1 << 1)
#define S6A_ULR_FLAGS_SKIP_SUBSCRIBER_DATA              (1 << 2)
#define S6A_ULR_FLAGS_GPRS_SUBSCRIPTION_DATA_INDICATOR  (1 << 3)
#define S6A_ULR_FLAGS_NODE_TYPE_INDICATOR               (1 << 4)
#define S6A_ULR_FLAGS_INITIAL_ATTACH_INDICATOR          (1 << 5)
#define S6A_ULR_FLAGS_PS_LCS_NOT_SUPPORTED_BY_UE        (1 << 6)
#define S6A_ULR_FLAGS_SMS_ONLY_INDICATION               (1 << 7)

#define S6A_RAT_TYPE_WLAN           (0) /* This value shall be used to indicate that the RAT is WLAN. */
#define S6A_RAT_TYPE_VIRTUAL        (1) /* This value shall be used to indicate that the RAT is unknown. */
#define S6A_RAT_TYPE_UTRAN          (1000) /* This value shall be used to indicate that the RAT is UTRAN. */
#define S6A_RAT_TYPE_GERAN          (1001) /* This value shall be used to indicate that the RAT is GERAN. */
#define S6A_RAT_TYPE_GAN            (1002) /* This value shall be used to indicate that the RAT is GAN. */
#define S6A_RAT_TYPE_HSPA_EVOLUTION (1003) /* This value shall be used to indicate that the RAT is HSPA Evolution. */
#define S6A_RAT_TYPE_EUTRAN         (1004) /* This value shall be used to indicate that the RAT is EUTRAN (WB-EUTRAN). */
#define S6A_RAT_TYPE_EUTRAN_NB_IOT  (1005) /* This value shall be used to indicate that the RAT is NB-IoT. */
#define S6A_RAT_TYPE_CDMA2000_1X    (2000) /* This value shall be used to indicate that the RAT is CDMA2000 1X. */
#define S6A_RAT_TYPE_HRPD           (2001) /* This value shall be used to indicate that the RAT is HRPD. */
#define S6A_RAT_TYPE_UMB            (2002) /* This value shall be used to indicate that the RAT is UMB. */
#define S6A_RAT_TYPE_EHRPD          (2003) /* This value shall be used to indicate that the RAT is eHRPD. */

#define S6A_SUBSCRIPTION_DATA_FLAGS_PS_AND_SMS_ONLY_SERVICE_PROVISION_INDICATION (1 << 0)
#define S6A_SUBSCRIPTION_DATA_FLAGS_SMS_IN_SGSN_ALLOWED_INDICATION               (1 << 1)
#define S6A_SUBSCRIPTION_DATA_FLAGS_USER_PLANE_INTEGRITY_PROTECTION              (1 << 2)
#define S6A_SUBSCRIPTION_DATA_FLAGS_PDN_CONNECTION_RESTRICTED                    (1 << 3)

namespace s6as6d {

// Member functions that customize the individual application
Application::Application()
    : ApplicationBase()
    //, m_cmd_uplr( *this )
    //, m_cmd_calr( *this )
    //, m_cmd_auir( *this )
    //, m_cmd_insdr( *this )
    //, m_cmd_desdr( *this )
    //, m_cmd_puur( *this )
    //, m_cmd_rer( *this )
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
    //std::cout << "Registering s6as6d command handlers" << std::endl;
    //std::cout << "Registering UPLR command handler" << std::endl;
    //registerHandler( m_cmd_uplr );
    //std::cout << "Registering CALR command handler" << std::endl;
    //registerHandler( m_cmd_calr );
    //std::cout << "Registering AUIR command handler" << std::endl;
    //registerHandler( m_cmd_auir );
    //std::cout << "Registering INSDR command handler" << std::endl;
    //registerHandler( m_cmd_insdr );
    //std::cout << "Registering DESDR command handler" << std::endl;
    //registerHandler( m_cmd_desdr );
    //std::cout << "Registering PUUR command handler" << std::endl;
    //registerHandler( m_cmd_puur );
    //std::cout << "Registering RER command handler" << std::endl;
    //registerHandler( m_cmd_rer );
}

// UPLR Request (req) Command member functions

bool Application::sendULRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, ULRTest &ulrtest, ULRTracker &result)
{
   TestULRreq *ulr = new TestULRreq( *this, ulrtest, result );

   ulr->addSessionId( getDict().avpSessionId() );
   ulr->add( getDict().avpAuthSessionState(), 1 );
   ulr->addOrigin();
   ulr->add( getDict().avpDestinationHost(), host );
   ulr->add( getDict().avpDestinationRealm(), realm );
   ulr->add( getDict().avpUserName(), imsi );
   ulr->add( getDict().avpRatType(), S6A_RAT_TYPE_EUTRAN );
   ulr->add( getDict().avpUlrFlags(), S6A_ULR_FLAGS_S6AS6D_INDICATOR | S6A_ULR_FLAGS_INITIAL_ATTACH_INDICATOR );
   ulr->add( getDict().avpVisitedPlmnId(), vplmnid, 3 );

   try
   {
      ulr->send();
   }
   catch ( FDException &ex )
   {
        std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
        delete ulr;
        ulr = NULL;
   }

   return ulr != NULL;
}

bool Application::sendULRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, ULRTestThread &ulrtest, ULRTracker &result)
{
   TestULRreq2 *ulr = new TestULRreq2( *this, ulrtest, result );

   ulr->addSessionId( getDict().avpSessionId() );
   ulr->add( getDict().avpAuthSessionState(), 1 );
   ulr->addOrigin();
   ulr->add( getDict().avpDestinationHost(), host );
   ulr->add( getDict().avpDestinationRealm(), realm );
   ulr->add( getDict().avpUserName(), imsi );
   ulr->add( getDict().avpRatType(), S6A_RAT_TYPE_EUTRAN );
   ulr->add( getDict().avpUlrFlags(), S6A_ULR_FLAGS_S6AS6D_INDICATOR | S6A_ULR_FLAGS_INITIAL_ATTACH_INDICATOR );
   ulr->add( getDict().avpVisitedPlmnId(), vplmnid, 3 );

   try
   {
      ulr->send();
   }
   catch ( FDException &ex )
   {
        std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
        delete ulr;
        ulr = NULL;
   }

   return ulr != NULL;
}

bool Application::sendULRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, AttachTestThread &attachtest, AttachTracker &result)
{
   TestULRAttachreq *ulr = new TestULRAttachreq( *this, attachtest, result );

   ulr->addSessionId( getDict().avpSessionId() );
   ulr->add( getDict().avpAuthSessionState(), 1 );
   ulr->addOrigin();
   ulr->add( getDict().avpDestinationHost(), host );
   ulr->add( getDict().avpDestinationRealm(), realm );
   ulr->add( getDict().avpUserName(), imsi );
   ulr->add( getDict().avpRatType(), S6A_RAT_TYPE_EUTRAN );
   ulr->add( getDict().avpUlrFlags(), S6A_ULR_FLAGS_S6AS6D_INDICATOR | S6A_ULR_FLAGS_INITIAL_ATTACH_INDICATOR );
   ulr->add( getDict().avpVisitedPlmnId(), vplmnid, 3 );

   try
   {
      ulr->send();
   }
   catch ( FDException &ex )
   {
        std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
        delete ulr;
        ulr = NULL;
   }

   return ulr != NULL;
}

bool Application::sendAIRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, AIRTestThread &airtest, AIRTracker &result)
{
   TestAIRreq *air = new TestAIRreq( *this, airtest, result );

   air->addSessionId( getDict().avpSessionId() );
   air->add( getDict().avpAuthSessionState(), 1 );
   air->addOrigin();
   air->add( getDict().avpDestinationHost(), host );
   air->add( getDict().avpDestinationRealm(), realm );
   air->add( getDict().avpUserName(), imsi );
   air->add( getDict().avpVisitedPlmnId(), vplmnid, 3 );

   {
      FDAvp requested_eutran_authentication_info( getDict().avpRequestedEutranAuthenticationInfo() );
      requested_eutran_authentication_info.add( getDict().avpNumberOfRequestedVectors(), 1 );
      requested_eutran_authentication_info.add( getDict().avpImmediateResponsePreferred(), 0 );
      air->add( requested_eutran_authentication_info );
   }

   try
   {
      air->send();
   }
   catch ( FDException &ex )
   {
        std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
        delete air;
        air = NULL;
   }

   return air != NULL;
}

bool Application::sendAIRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, AttachTestThread &attachtest, AttachTracker &result)
{
   TestAIRAttachreq *air = new TestAIRAttachreq( *this, attachtest, result );

   air->addSessionId( getDict().avpSessionId() );
   air->add( getDict().avpAuthSessionState(), 1 );
   air->addOrigin();
   air->add( getDict().avpDestinationHost(), host );
   air->add( getDict().avpDestinationRealm(), realm );
   air->add( getDict().avpUserName(), imsi );
   air->add( getDict().avpVisitedPlmnId(), vplmnid, 3 );

   {
      FDAvp requested_eutran_authentication_info( getDict().avpRequestedEutranAuthenticationInfo() );
      requested_eutran_authentication_info.add( getDict().avpNumberOfRequestedVectors(), 1 );
      requested_eutran_authentication_info.add( getDict().avpImmediateResponsePreferred(), 0 );
      air->add( requested_eutran_authentication_info );
   }

   try
   {
      air->send();
   }
   catch ( FDException &ex )
   {
        std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
        delete air;
        air = NULL;
   }

   return air != NULL;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

TestULRreq::TestULRreq( Application &app, ULRTest &ulrtest, ULRTracker &result )
   : UPLRreq( app ),
     m_ulrtest( ulrtest ),
     m_result( result )
{
}

TestULRreq::~TestULRreq()
{
}

void TestULRreq::processAnswer( FDMessageAnswer &ans )
{
   UpdateLocationAnswerExtractor ula(ans, getApplication().getDict());

   int32_t result_code;

   if ( !ula.result_code.get(result_code) )
      result_code = 0;

   m_ulrtest.recvULA(m_result, result_code == 2001);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

TestULRreq2::TestULRreq2( Application &app, ULRTestThread &ulrtest, ULRTracker &result )
   : UPLRreq( app ),
     m_ulrtest( ulrtest ),
     m_result( result )
{
}

TestULRreq2::~TestULRreq2()
{
}

void TestULRreq2::processAnswer( FDMessageAnswer &ans )
{
   UpdateLocationAnswerExtractor ula(ans, getApplication().getDict());

   int32_t result_code;

   if ( !ula.result_code.get(result_code) )
      result_code = 0;

   m_ulrtest.receive(m_result);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

TestAIRreq::TestAIRreq( Application &app, AIRTestThread &airtest, AIRTracker &result )
   : AUIRreq( app ),
     m_airtest( airtest ),
     m_result( result )
{
}

TestAIRreq::~TestAIRreq()
{
}

void TestAIRreq::processAnswer( FDMessageAnswer &ans )
{
   AuthenticationInformationAnswerExtractor aia(ans, getApplication().getDict());

   int32_t result_code;

   if ( !aia.result_code.get(result_code) )
      result_code = 0;

   m_airtest.receive(m_result);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

TestAIRAttachreq::TestAIRAttachreq( Application &app, AttachTestThread &attachtest, AttachTracker &result )
   : AUIRreq( app ),
     m_attachtest( attachtest ),
     m_result( result )
{
}

TestAIRAttachreq::~TestAIRAttachreq()
{
}

void TestAIRAttachreq::processAnswer( FDMessageAnswer &ans )
{
   AuthenticationInformationAnswerExtractor aia(ans, getApplication().getDict());

   int32_t result_code;

   if ( !aia.result_code.get(result_code) )
      result_code = 0;

   m_attachtest.receive(m_result);
}

TestULRAttachreq::TestULRAttachreq( Application &app, AttachTestThread &attachtest, AttachTracker &result )
   : UPLRreq( app ),
     m_attachtest( attachtest ),
     m_result( result )
{
}

TestULRAttachreq::~TestULRAttachreq()
{
}

void TestULRAttachreq::processAnswer( FDMessageAnswer &ans )
{
   UpdateLocationAnswerExtractor ulr(ans, getApplication().getDict());

   int32_t result_code;

   if ( !ulr.result_code.get(result_code) )
      result_code = 0;

   m_attachtest.receive(m_result);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Sends a UPLR Request to the corresponding Peer
bool Application::sendUPLRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the UPLR Command
    UPLRreq *s = createUPLRreq( peer );

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

    // DO NOT free the newly created UPLRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for UPLR reuqests
UPLRreq *Application::createUPLRreq(FDPeer &peer)
{
    //  creates the UPLRreq object
    UPLRreq *s = new UPLRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the UPLR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void UPLRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// UPLR Command
}

// UPLR Command (cmd) member function

// Function invoked when a UPLR Command is received
int UPLRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the UPLR command.
    return -1;
}
 
// CALR Request (req) Command member functions


// Sends a CALR Request to the corresponding Peer
bool Application::sendCALRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the CALR Command
    CALRreq *s = createCALRreq( peer );

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

    // DO NOT free the newly created CALRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for CALR reuqests
CALRreq *Application::createCALRreq(FDPeer &peer)
{
    //  creates the CALRreq object
    CALRreq *s = new CALRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the CALR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void CALRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// CALR Command
}

// CALR Command (cmd) member function

// Function invoked when a CALR Command is received
int CALRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the CALR command.
    return -1;
}
 
// AUIR Request (req) Command member functions


// Sends a AUIR Request to the corresponding Peer
bool Application::sendAUIRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the AUIR Command
    AUIRreq *s = createAUIRreq( peer );

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

    // DO NOT free the newly created AUIRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for AUIR reuqests
AUIRreq *Application::createAUIRreq(FDPeer &peer)
{
    //  creates the AUIRreq object
    AUIRreq *s = new AUIRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the AUIR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void AUIRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// AUIR Command
}

// AUIR Command (cmd) member function

// Function invoked when a AUIR Command is received
int AUIRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the AUIR command.
    return -1;
}
 
// INSDR Request (req) Command member functions


// Sends a INSDR Request to the corresponding Peer
bool Application::sendINSDRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the INSDR Command
    INSDRreq *s = createINSDRreq( peer );

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

    // DO NOT free the newly created INSDRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for INSDR reuqests
INSDRreq *Application::createINSDRreq(FDPeer &peer)
{
    //  creates the INSDRreq object
    INSDRreq *s = new INSDRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the INSDR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void INSDRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// INSDR Command
}

// INSDR Command (cmd) member function

// Function invoked when a INSDR Command is received
int INSDRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the INSDR command.
    return -1;
}
 
// DESDR Request (req) Command member functions


// Sends a DESDR Request to the corresponding Peer
bool Application::sendDESDRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the DESDR Command
    DESDRreq *s = createDESDRreq( peer );

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

    // DO NOT free the newly created DESDRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for DESDR reuqests
DESDRreq *Application::createDESDRreq(FDPeer &peer)
{
    //  creates the DESDRreq object
    DESDRreq *s = new DESDRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the DESDR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void DESDRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// DESDR Command
}

// DESDR Command (cmd) member function

// Function invoked when a DESDR Command is received
int DESDRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the DESDR command.
    return -1;
}
 
// PUUR Request (req) Command member functions


// Sends a PUUR Request to the corresponding Peer
bool Application::sendPUURreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the PUUR Command
    PUURreq *s = createPUURreq( peer );

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

    // DO NOT free the newly created PUURreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for PUUR reuqests
PUURreq *Application::createPUURreq(FDPeer &peer)
{
    //  creates the PUURreq object
    PUURreq *s = new PUURreq( *this );

    //TODO - Code must be added to correctly
    //         populate the PUUR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void PUURreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// PUUR Command
}

// PUUR Command (cmd) member function

// Function invoked when a PUUR Command is received
int PUURcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the PUUR command.
    return -1;
}
 
// RER Request (req) Command member functions


// Sends a RER Request to the corresponding Peer
bool Application::sendRERreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the RER Command
    RERreq *s = createRERreq( peer );

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

    // DO NOT free the newly created RERreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for RER reuqests
RERreq *Application::createRERreq(FDPeer &peer)
{
    //  creates the RERreq object
    RERreq *s = new RERreq( *this );

    //TODO - Code must be added to correctly
    //         populate the RER request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void RERreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// RER Command
}

// RER Command (cmd) member function

// Function invoked when a RER Command is received
int RERcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the RER command.
    return -1;
}
 

}
