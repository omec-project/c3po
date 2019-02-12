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

#include "dataaccess.h"
#include "s6c_impl.h"
#include "statshss.h"

namespace s6c {

// Member functions that customize the individual application
Application::Application(DataAccess &dbobj )
    : ApplicationBase()
    , m_cmd_serifsr( *this )
    //, m_cmd_alscr( *this )
    //, m_cmd_resdsr( *this )
    , m_dbobj( dbobj )
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
    std::cout << "Registering SERIFSR command handler" << std::endl;
    registerHandler( m_cmd_serifsr );
    //std::cout << "Registering ALSCR command handler" << std::endl;
    //registerHandler( m_cmd_alscr );
    //std::cout << "Registering RESDSR command handler" << std::endl;
    //registerHandler( m_cmd_resdsr );
}

// SERIFSR Request (req) Command member functions


// Sends a SERIFSR Request to the corresponding Peer
bool Application::sendSERIFSRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the SERIFSR Command
    SERIFSRreq *s = createSERIFSRreq( peer );

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
SERIFSRreq *Application::createSERIFSRreq(FDPeer &peer)
{
    //  creates the SERIFSRreq object
    SERIFSRreq *s = new SERIFSRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the SERIFSR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void SERIFSRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// SERIFSR Command
}

// SERIFSR Command (cmd) member function

// Function invoked when a SERIFSR Command is received

#define MAX_MSISDN_LENGTH 15
#define MAX_IMSI_LENGTH 15

#define SRR_FLAGS_GPRS_INDICATOR 1
#define SRR_FLAGS_SM_RP_PRI 2
#define SRR_FLAGS_SINGLE_ATTEMPT_DELIVERY 4

int SERIFSRcmd::process( FDMessageRequest *req )
{
   SendRoutingInfoForSmRequestExtractor srr( *req, getDict() );
   std::string msisdn;
   std::string imsi;
   int sm_delivery_not_intended = -1;
   DAImsiInfo info;
   DAMmeIdentity mmeid;
   uint32_t srr_flags;

   //
   // start populating the answer
   //
   FDMessageAnswer ans( req );
   FDAvp vsai( getDict().avpVendorSpecificApplicationId() );
   vsai.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
   vsai.add( getDict().avpAuthApplicationId(), getDict().app().getId() );
   ans.add( vsai );
   ans.add( srr.auth_session_state );
   ans.addOrigin();

   //
   // get the MSISDN or IMSI
   //
   if ( srr.msisdn.exists() )
   {
      uint8_t data[ MAX_MSISDN_LENGTH ];
      size_t  len = sizeof( data );

      srr.msisdn.get( data, len);

      FDUtility::tbcd2str( data, len, msisdn );
   }
   else if ( srr.user_name.exists() )
   {
      srr.user_name.get( imsi );
   }
   else
   {
      ans.add( getDict().avpResultCode(), 5005 ); // DIAMETER_MISSING_AVP
      FDAvp fa( getDict().avpFailedAvp() );
      fa.add( getDict().avpMsisdn(), "" );
      ans.add( fa );
      ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_srr, 0, 5005);
      return 0;
   }

   //
   // get SM-Delivery-Not-Intended
   //
   srr.sm_delivery_not_intended.get( sm_delivery_not_intended );

   //
   // get SRR-Flags
   //
   srr.srr_flags.get( srr_flags );

   //
   // check to see if the msisdn exists
   //
   if ( !msisdn.empty() )
   {
      if ( !getApplication().getDbObj().getImsiFromMsisdn( msisdn, imsi ) )
      {
         FDAvp er( getDict().avpExperimentalResult() );
         er.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
         er.add( getDict().avpExperimentalResultCode(), 5001 ); // DIAMETER_ERROR_USER_UNKNOWN
         ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_srr, getDict().vnd3GPP().getId(), 5001);
         return 0;
      }
   }

   //
   // lookup the imsi
   //
   if ( !getApplication().getDbObj().getImsiInfo( imsi, info, NULL, NULL ) )
   {
      FDAvp er( getDict().avpExperimentalResult() );
      er.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
      er.add( getDict().avpExperimentalResultCode(), 5001 ); // DIAMETER_ERROR_USER_UNKNOWN
      ans.add( er );
      ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_srr, getDict().vnd3GPP().getId(), 5001);
      return 0;
   }

   //
   // lookup the mme info
   //
   if ( !getApplication().getDbObj().getMmeIdentity( info.mme_id, mmeid ) )
   {
      FDAvp er( getDict().avpExperimentalResult() );
      er.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
      er.add( getDict().avpExperimentalResultCode(), 5001 ); // DIAMETER_ERROR_USER_UNKNOWN
      ans.add( er );
      ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_srr, getDict().vnd3GPP().getId(), 5001);
      return 0;
   }

   //
   // check for an attachd session
   //
   if ( info.ms_ps_status != "ATTACHED" )
   {
      FDAvp er( getDict().avpExperimentalResult() );
      er.add( getDict().avpVendorId(), getDict().vnd3GPP().getId() );
      er.add( getDict().avpExperimentalResultCode(), 5550 ); // DIAMETER_ERROR_ABSENT_USER
      ans.add( er );
      ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_srr, getDict().vnd3GPP().getId(), 5550);
      return 0;
   }

   //
   // add the Result-Code
   //
   ans.add( getDict().avpResultCode(), 2001 ); // DIAMETER_SUCCESS
   StatsHss::singleton().registerStatResult(stat_hss_srr, 0, 2001);

   //
   // add the User-Name AVP
   //
   switch ( sm_delivery_not_intended )
   {
      case 1: // ONLY_MCC_MNC_REQUESTED
      {
         //
         // THIS IS NOT CORRECT.  The MCC/MNC needs to be added to the database
         // and retrieved from there.
         //
         ans.add( getDict().avpUserName(), info.imsi.substr( 0, info.imsi.length() == 14 ? 5 : 6 ) );
         break;
      }
      case -1: // SM-Delivery-Not-Intended AVP not present in request
      case 0:  // ONLY_IMSI_REQUESTED
      {
         ans.add( getDict().avpUserName(), info.imsi );
         break;
      }
   }

   //
   // add the Serving-Node
   //
   if ( sm_delivery_not_intended == -1 )
   {
      FDAvp sn( getDict().avpServingNode() );
      sn.add( getDict().avpMmeName(), info.mmehost );
      sn.add( getDict().avpMmeRealm(), info.mmerealm );

      uint8_t buf[ MAX_MSISDN_LENGTH ];
      size_t len = FDUtility::str2tbcd( mmeid.mme_isdn, buf, sizeof( buf ) );
      sn.add( getDict().avpMmeNumberForMtSms(), buf, len );

      ans.add( sn );
   }

   //
   // add the User-Identifier if needed
   //
   if ( msisdn != info.str_msisdn )
   {
      FDAvp ui( getDict().avpUserIdentifier() );
      uint8_t buf[ MAX_MSISDN_LENGTH ];
      size_t len = FDUtility::str2tbcd( info.str_msisdn, buf, sizeof( buf ) );
      ui.add( getDict().avpMsisdn(), buf, len );

      ans.add( ui );
   }

   //
   // send the answer
   //
   ans.send();

   return 0;
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
