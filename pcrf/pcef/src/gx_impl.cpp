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
#include "gx_impl.h"
#include "pcef.h"

void displaymenu();

namespace gx {

// Member functions that customize the individual application
Application::Application( PCEF &pcef )
    : ApplicationBase()
    //, m_cmd_crcr( *this )
      , m_cmd_rear( *this )
      , m_pcef( pcef )
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
    std::cout << "Registering gx command handlers" << std::endl;
    //std::cout << "Registering CRCR command handler" << std::endl;
    //registerHandler( m_cmd_crcr );
    std::cout << "Registering REAR command handler" << std::endl;
    registerHandler( m_cmd_rear );
}

// CRCR Request (req) Command member functions


// Sends a CRCR Request to the corresponding Peer
bool Application::sendCRCRreq( int32_t ccRequestType )
{
   //TODO - This code may be modified based on specific
   //       processing needs to send the CRCR Command
   CRCRreq *s = createCRCRreq( ccRequestType );

   try
   {
      if ( s )
      {
         s->dump();
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
CRCRreq *Application::createCRCRreq( int32_t ccRequestType )
{
   CRCRreq *s = new CRCRreq( *this );

   s->add( getDict().avpSessionId(), ccRequestType == 1 ? s->getSessionId() : m_sessionid );
   s->add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   s->addOrigin();
   s->add( getDict().avpDestinationHost(), Options::pcrfHost() );
   s->add( getDict().avpDestinationRealm(), Options::pcrfRealm() );

   s->add( getDict().avpCcRequestType(), ccRequestType );
   if ( ccRequestType == 1 ) // INITIAL_REQUEST
   {
      s->add( getDict().avpCcRequestNumber(), 0 );

      FDAvp subscriptionid( getDict().avpSubscriptionId() );
      subscriptionid.add( getDict().avpSubscriptionIdType(), 1 ); // END_USER_IMSI
      subscriptionid.add( getDict().avpSubscriptionIdData(), "001014567891234" );
      s->add( subscriptionid );

      FDAvp suppfeat1( getDict().avpSupportedFeatures() );
      suppfeat1.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
      suppfeat1.add( getDict().avpFeatureListId(), 1 );
      suppfeat1.add( getDict().avpFeatureList(), 0xffffffff );
      s->add( suppfeat1 );

      FDAvp suppfeat2( getDict().avpSupportedFeatures() );
      suppfeat2.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
      suppfeat2.add( getDict().avpFeatureListId(), 2 );
      suppfeat2.add( getDict().avpFeatureList(), 0xffffffff );
      s->add( suppfeat2 );

      uint32_t ipv4 = 0x7db0e5ce;
      s->add( getDict().avpFramedIpAddress(), (uint8_t*)&ipv4, sizeof(ipv4) );

      s->add( getDict().avpCalledStationId(), "r.ispsn" );
   }
   else if ( ccRequestType == 3 ) // TERMINATION_REQUEST
   {
      s->add( getDict().avpCcRequestNumber(), 1 );
   }

   return s;
}

// A handler for Answers corresponding to this specific Request
void CRCRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();

   CreditControlAnswerExtractor cca( ans, getApplication().getDict() );
   int32_t ccRequestType, rc;

   cca.cc_request_type.get( ccRequestType );
   cca.result_code.get( rc );

   if ( rc == 2001 )
   {
      if ( ccRequestType == 1 )
      {
         cca.session_id.get( getApplication().getSessionId() );
         if ( getApplication().getPCEF().getRepetitive() )
            getApplication().sendCRCRreq( 3 );
      }
      else if ( ccRequestType == 3 )
      {
         if ( getApplication().getPCEF().getRepetitive() )
            getApplication().sendCRCRreq( 1 );
      }
   }
}

// CRCR Command (cmd) member function

// Function invoked when a CRCR Command is received
int CRCRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the CRCR command.
    return -1;
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
   req->dump();

   FDMessageAnswer raa( req );

   raa.addOrigin();
   raa.add( getDict().avpResultCode(), 2001 );

   raa.send();

   return 0;
}
 

}
