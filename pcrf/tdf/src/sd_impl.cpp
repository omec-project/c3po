/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "sd_impl.h"
#include "options.h"

namespace sd {

// Member functions that customize the individual application
Application::Application(TDF &tdf )
   : ApplicationBase()
     , m_cmd_tsr( *this )
     , m_cmd_rear( *this )
     , m_tdf( tdf )
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
   std::cout << "Registering TSR command handler" << std::endl;
   registerHandler( m_cmd_tsr );
   std::cout << "Registering REAR command handler" << std::endl;
   registerHandler( m_cmd_rear );
}

// TSR Command (cmd) member function

// Function invoked when a TSR Command is received
int TSRcmd::process( FDMessageRequest *req )
{
   req->dump();

   TsRequestExtractor tsr( *req, getDict() );
   tsr.session_id.get( getApplication().getSessionId() );

   FDMessageAnswer ans( req );

   FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
   vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );

   ans.add( vsai );
   ans.addOrigin();
   ans.add( getDict().avpResultCode(), 2001 );

   FDAvp sf1( getDict().avpSupportedFeatures() );
   sf1.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   sf1.add( getDict().avpFeatureListId(), 1 );
   sf1.add( getDict().avpFeatureList(), 0xffffffff );
   ans.add( sf1 );

   ans.dump();
   ans.send();

   return 0;
}
 
// CRCR Request (req) Command member functions


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

// A handler for Answers corresponding to this specific Request
void CRCRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();
}

// REAR Command (cmd) member function

// Function invoked when a REAR Command is received
int REARcmd::process( FDMessageRequest *req )
{
   req->dump();

   ReAuthRequestExtractor rar( *req, getDict() );

   // send RAA
   FDMessageAnswer raa( req );

   raa.addOrigin();
   raa.add( getDict().avpResultCode(), 2001 );

   raa.dump();
   raa.send();

   if ( rar.session_release_cause.exists() )
   {
      // send CCR TERMINATION_REQUEST
      CRCRreq *ccr = new CRCRreq( getApplication() );

      ccr->add( getDict().avpSessionId(), getApplication().getSessionId() );
      ccr->add( getDict().avpAuthApplicationId(), getDict().app().getId() );
      ccr->addOrigin();
      ccr->add( getDict().avpDestinationRealm(), Options::pcrfRealm() );
      ccr->add( getDict().avpCcRequestType(), 3 );
      ccr->add( getDict().avpCcRequestNumber(), 1 );
      ccr->add( getDict().avpDestinationHost(), Options::pcrfHost() );

      ccr->dump();
      ccr->send();
   }

   return 0;
}
 
void TSRreq::processAnswer( FDMessageAnswer &ans )
{
}

void REARreq::processAnswer( FDMessageAnswer &ans )
{
}

int CRCRcmd::process( FDMessageRequest *req )
{
   return -1;
}

}
