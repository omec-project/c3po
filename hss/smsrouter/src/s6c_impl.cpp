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

#include "options.h"
#include "s6c_impl.h"

namespace s6c {

// Member functions that customize the individual application
Application::Application()
    : ApplicationBase()
    //, m_cmd_serifsr( *this )
    //, m_cmd_alscr( *this )
    //, m_cmd_resdsr( *this )
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
    std::cout << "Registering s6c command handlers" << std::endl;
    //std::cout << "Registering SERIFSR command handler" << std::endl;
    //registerHandler( m_cmd_serifsr );
    //std::cout << "Registering ALSCR command handler" << std::endl;
    //registerHandler( m_cmd_alscr );
    //std::cout << "Registering RESDSR command handler" << std::endl;
    //registerHandler( m_cmd_resdsr );
}

// SERIFSR Request (req) Command member functions


// Sends a SERIFSR Request to the corresponding Peer
bool Application::sendSERIFSRreq( bool withMsisdn, bool withImsi )
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the SERIFSR Command
    SERIFSRreq *s = createSERIFSRreq( withMsisdn, withImsi );

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

    // DO NOT free the newly created SERIFSRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for SERIFSR reuqests
SERIFSRreq *Application::createSERIFSRreq( bool withMsisdn, bool withImsi )
{
   SERIFSRreq *s = new SERIFSRreq( *this );

   s->add( getDict().avpSessionId(), s->getSessionId() );

   FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
   vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   s->add( vsai );

   s->add( getDict().avpAuthSessionState(), 1 ); //NO_STATE_MAINTAINED

   s->addOrigin();

   s->add( getDict().avpDestinationHost(), Options::hssHost() );
   s->add( getDict().avpDestinationRealm(), Options::hssRealm() );

   if ( withMsisdn )
   {
      uint8_t buf[15];
      size_t len = FDUtility::str2tbcd( "12142150001", buf, sizeof(buf) );
      s->add( getDict().avpMsisdn(), buf, len );
   }

   if ( withImsi )
   {
      s->add( getDict().avpUserName(), "001014567891234" );
   }

   s->dump();

   return s;
}

// A handler for Answers corresponding to this specific Request
void SERIFSRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();
}

// SERIFSR Command (cmd) member function

// Function invoked when a SERIFSR Command is received
int SERIFSRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the SERIFSR command.
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
 
// RESDSR Request (req) Command member functions


// Sends a RESDSR Request to the corresponding Peer
bool Application::sendRESDSRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the RESDSR Command
    RESDSRreq *s = createRESDSRreq( peer );

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

    // DO NOT free the newly created RESDSRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for RESDSR reuqests
RESDSRreq *Application::createRESDSRreq(FDPeer &peer)
{
    //  creates the RESDSRreq object
    RESDSRreq *s = new RESDSRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the RESDSR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void RESDSRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// RESDSR Command
}

// RESDSR Command (cmd) member function

// Function invoked when a RESDSR Command is received
int RESDSRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the RESDSR command.
    return -1;
}
 

}
