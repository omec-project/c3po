/*
* Copyright (c) 2017 Sprint
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <string>
#include <iostream>
#include <sstream>

#include "sgd_impl.h"

namespace sgd {

// Member functions that customize the individual application
Application::Application()
    : ApplicationBase()
    , m_cmd_mofsmr( *this )
    //, m_cmd_mtfsmr( *this )
    //, m_cmd_alscr( *this )
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
    std::cout << "Registering sgd command handlers" << std::endl;
    std::cout << "Registering MOFSMR command handler" << std::endl;
    registerHandler( m_cmd_mofsmr );
    //std::cout << "Registering MTFSMR command handler" << std::endl;
    //registerHandler( m_cmd_mtfsmr );
    //std::cout << "Registering ALSCR command handler" << std::endl;
    //registerHandler( m_cmd_alscr );
}

// MOFSMR Request (req) Command member functions


// Sends a MOFSMR Request to the corresponding Peer
bool Application::sendMOFSMRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the MOFSMR Command
    MOFSMRreq *s = createMOFSMRreq( peer );

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

    // DO NOT free the newly created MOFSMRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for MOFSMR reuqests
MOFSMRreq *Application::createMOFSMRreq(FDPeer &peer)
{
    //  creates the MOFSMRreq object
    MOFSMRreq *s = new MOFSMRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the MOFSMR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void MOFSMRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// MOFSMR Command
}

// MOFSMR Command (cmd) member function

// Function invoked when a MOFSMR Command is received
int MOFSMRcmd::process( FDMessageRequest *req )
{
   MoForwardShortMessageRequestExtractor ofr( *req, getDict() );
   FDMessageAnswer ans( req );

   FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
   vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   ans.add( vsai );

   ans.add( getDict().avpResultCode(), 2001 );
   ans.add( getDict().avpAuthSessionState(), 1 );
   ans.addOrigin();

   ans.send();

   getApplication().sendMTFSMRreq(ofr);

   return 0;
}
 
// MTFSMR Request (req) Command member functions


// Sends a MTFSMR Request to the corresponding Peer
bool Application::sendMTFSMRreq( MoForwardShortMessageRequestExtractor &ofr )
{
    MTFSMRreq *s = createMTFSMRreq( ofr );

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

    // DO NOT free the newly created MTFSMRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for MTFSMR reuqests
MTFSMRreq *Application::createMTFSMRreq( MoForwardShortMessageRequestExtractor &ofr )
{
    std::string str;
    MTFSMRreq *s = new MTFSMRreq( *this );

    s->add( getDict().avpSessionId(), s->getSessionId() );

    FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
    vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
    vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
    s->add( vsai );

    s->add( getDict().avpAuthSessionState(), 1 );
    s->addOrigin();

    ofr.origin_host.get( str );
    s->add( getDict().avpDestinationHost(), str );
    ofr.origin_realm.get( str );
    s->add( getDict().avpDestinationRealm(), str );

    s->add( ofr.user_identifier.user_name );
    s->add( ofr.sc_address );

    uint8_t tpdu[] = {
          0x04, 0x08, 0x81, 0x11, 0x22,
          0x33, 0x44, 0x00, 0x00, 0x81, 0x10, 0x30, 0x31, 0x60, 0x00, 0x4a, 0x11, 0xc8, 0x30, 0xf2, 0x8d,
          0x2c, 0x83, 0x90, 0x65, 0x39, 0x1d, 0x1d, 0x06, 0x09, 0xa7, 0x43
    };

    s->add( getDict().avpSmRpUi(), tpdu, 32 );

    s->dump();

    return s;
}

// A handler for Answers corresponding to this specific Request
void MTFSMRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();
}

// MTFSMR Command (cmd) member function

// Function invoked when a MTFSMR Command is received
int MTFSMRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the MTFSMR command.
    return -1;
}
 
// ALSCR Request (req) Command member functions


// Sends a ALSCR Request to the corresponding Peer
bool Application::sendALSCRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the ALSCR Command
    ALSCRreq *s = createALSCRreq( peer );

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

    // DO NOT free the newly created ALSCRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for ALSCR reuqests
ALSCRreq *Application::createALSCRreq(FDPeer &peer)
{
    //  creates the ALSCRreq object
    ALSCRreq *s = new ALSCRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the ALSCR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void ALSCRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// ALSCR Command
}

// ALSCR Command (cmd) member function

// Function invoked when a ALSCR Command is received
int ALSCRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the ALSCR command.
    return -1;
}
 

}
