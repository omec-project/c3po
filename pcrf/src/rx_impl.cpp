/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "rx_impl.h"

namespace rx {

// Member functions that customize the individual application
Application::Application( PCRF &pcrf )
    : ApplicationBase()
    //, m_cmd_aar( *this )
    //, m_cmd_rear( *this )
    //, m_cmd_setr( *this )
    //, m_cmd_absr( *this )
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
    std::cout << "Registering rx command handlers" << std::endl;
    //std::cout << "Registering AAR command handler" << std::endl;
    //registerHandler( m_cmd_aar );
    //std::cout << "Registering REAR command handler" << std::endl;
    //registerHandler( m_cmd_rear );
    //std::cout << "Registering SETR command handler" << std::endl;
    //registerHandler( m_cmd_setr );
    //std::cout << "Registering ABSR command handler" << std::endl;
    //registerHandler( m_cmd_absr );
}

// AAR Request (req) Command member functions


// Sends a AAR Request to the corresponding Peer
bool Application::sendAARreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the AAR Command
    AARreq *s = createAARreq( peer );

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

    // DO NOT free the newly created AARreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for AAR reuqests
AARreq *Application::createAARreq(FDPeer &peer)
{
    //  creates the AARreq object
    AARreq *s = new AARreq( *this );

    //TODO - Code must be added to correctly
    //         populate the AAR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void AARreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// AAR Command
}

// AAR Command (cmd) member function

// Function invoked when a AAR Command is received
int AARcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the AAR command.
    return -1;
}
 
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
 
// SETR Request (req) Command member functions


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
 
// ABSR Request (req) Command member functions


// Sends a ABSR Request to the corresponding Peer
bool Application::sendABSRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the ABSR Command
    ABSRreq *s = createABSRreq( peer );

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

    // DO NOT free the newly created ABSRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for ABSR reuqests
ABSRreq *Application::createABSRreq(FDPeer &peer)
{
    //  creates the ABSRreq object
    ABSRreq *s = new ABSRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the ABSR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void ABSRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// ABSR Command
}

// ABSR Command (cmd) member function

// Function invoked when a ABSR Command is received
int ABSRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the ABSR command.
    return -1;
}
 

}
