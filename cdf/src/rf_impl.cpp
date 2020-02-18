/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "rf_impl.h"

namespace rf {

// Member functions that customize the individual application
Application::Application()
   : ApplicationBase()
   , m_cmd_acr( getDict() )


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
    registerHandler( m_cmd_acr );
}

// ACR Request (req) Command member functions


// Sends a ACR Request to the corresponding Peer
bool Application::sendACRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //       processing needs to send the ACR Command
    ACRreq *s = createACRreq( peer );

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

    // DO NOT free the newly created ACRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for ACR reuqests
ACRreq *Application::createACRreq(FDPeer &peer)
{
    //  creates the ACRreq object
    ACRreq *s = new ACRreq( getDict() );

    //TODO - Code must be added to correctly
    //       populate the ACR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void ACRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// ACR Command
}

// ACR Command (cmd) member function

// Function invoked when a ACR Command is received
int ACRcmd::process( FDMessageRequest *req )
{

   // create the answer object
   FDMessageAnswer ans( req );

   // add the origin host/realm
   ans.addOrigin();

   int32_t rectype;
   uint32_t recnbr;

   // add the Result-Code
   ans.add( m_dict.avpResultCode(), 2001 );

   // add the Accounting-Record-Type
   if ( req->get(m_dict.avpAccountingRecordType(),rectype) )
      ans.add( m_dict.avpAccountingRecordType(), rectype );

   // add the Accounting-Record-Number
   if ( req->get(m_dict.avpAccountingRecordNumber(),recnbr) )
      ans.add( m_dict.avpAccountingRecordNumber(), recnbr );

   // dump the request & answer messages
   req->dump();

   // send the answer
   ans.send();

   // free the requst object
   delete req;

   return 0;
}
 

}
