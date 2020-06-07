/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "st_impl.h"
#include "tssf.h"

namespace st {

// Member functions that customize the individual application
Application::Application( TSSF &tssf )
   : ApplicationBase()
     , m_cmd_tsr( *this )
     , m_cmd_setr( *this )
     , m_tssf( tssf )
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
    std::cout << "Registering TSR command handler" << std::endl;
    registerHandler( m_cmd_tsr );
    std::cout << "Registering SETR command handler" << std::endl;
    registerHandler( m_cmd_setr );
}

// TSR Request (req) Command member functions

// A handler for Answers corresponding to this specific Request
void TSRreq::processAnswer( FDMessageAnswer &ans )
{
}

// TSR Command (cmd) member function

// Function invoked when a TSR Command is received
int TSRcmd::process( FDMessageRequest *req )
{
   req->dump();

   TsRequestExtractor tsr( *req, getDict() );

   tsr.session_id.get( getApplication().getSessionId() );

   FDMessageAnswer tsa( req );

   FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
   vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   tsa.add( vsai );
   tsa.addOrigin();
   tsa.add( getDict().avpResultCode(), 2001 );

   tsa.dump();
   tsa.send();

   return 0;
}
 
// SETR Request (req) Command member functions

// A handler for Answers corresponding to this specific Request
void SETRreq::processAnswer( FDMessageAnswer &ans )
{
}

// SETR Command (cmd) member function

// Function invoked when a SETR Command is received
int SETRcmd::process( FDMessageRequest *req )
{
   req->dump();

   SessionTerminationRequestExtractor str( *req, getDict() );

   FDMessageAnswer sta( req );

   sta.addOrigin();
   sta.add( getDict().avpResultCode(), 2001 );

   sta.dump();
   sta.send();

   return 0;
}
 
// TNR Request (req) Command member functions

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
   return -1;
}
 

}
