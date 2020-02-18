/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>
#include <sutility.h>
#include "csvproc.h"
#include "statsctf.h"

#include "rf_impl.h"

namespace rf {

// Member functions that customize the individual application
Application::Application()
    : ApplicationBase()
{
   registerHandlers();
}

Application::~Application()
{
}

void Application::registerHandlers()
{
}


// Sends a ACR Request to the corresponding Peer
bool Application::sendACRreq(CSVProcessor &csvp, FDPeer &peer, CSVFile &file)
{
    ACRreq *s = createACRreq( csvp, peer, file );

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
ACRreq *Application::createACRreq(CSVProcessor &csvp, FDPeer &peer, CSVFile &file)
{

   Dictionary& dict = getDict();

   //  creates the ACRreq object
   ACRreq *req = new AccountingRequest( *this, dict, csvp, file );


   const CSVRow &data( file.row() );

   if ( !req )
   {
      std::cout << SUtility::currentTime() << " - ERROR - Unable to allocate AccountingRequest object" << std::endl;
      return req;
   }

   try
   {
      // get the SDN data from memcache for the specified IP address
      m_sdn.get( data[ cPDPAddress ] );

      req->add( dict.avpSessionId(), req->getSessionId() );

      req->addOrigin();

      req->add( dict.avpAcctApplicationId(), dict.app().getId() );

      req->add( dict.avpDestinationHost(), peer.getDestinationHost() );
      req->add( dict.avpDestinationRealm(), peer.getDestinationRealm() );

      req->add( dict.avpAccountingRecordType(), 1 );
      req->add( dict.avpAccountingRecordNumber(), 0 );

      FDAvp sid( dict.avpSubscriptionId() );
      req->add( sid );
      sid.add( dict.avpSubscriptionIdType(), 2 );
      sid.add( dict.avpSubscriptionIdData(), m_sdn.getIMSI() );

      FDAvp sdc( dict.avpServiceDataContainer() );
      req->add( sdc );

      if ( !data[cUplinkBytes].empty() )
       sdc.add( dict.avpAccountingInputOctets(), data.getUint64( cUplinkBytes ) );

      if ( !data[cDownlinkBytes].empty() )
       sdc.add( dict.avpAccountingOutputOctets(), data.getUint64( cDownlinkBytes ) );

      if ( !data[cRatingGroup].empty() )
       sdc.add( dict.avpRatingGroup(), data.getUint32( cRatingGroup ) );

      if ( !data[cServiceIdentifier].empty() )
       sdc.add( dict.avpServiceIdentifier(), data.getUint32( cServiceIdentifier ) );

      sdc.add( dict.avpSponsorIdentity(), data[ cSponsorIdentity ] );

      if ( data[ cRuleType ] == "ADC" )
       sdc.add( dict.avpAdcRuleBaseName(), data[ cRule ] );
      else
       sdc.add( dict.avpChargingRuleBaseName(), data[ cRule ] );

      sdc.add( dict.avpChangeTime(), m_sdn.getEndTime() == 0 ? m_sdn.getStartTime() : m_sdn.getEndTime() );

      FDAvp si( dict.avpServiceInformation() );
      req->add( si );

      FDAvp psi( dict.avpPsInformation() );
      si.add( psi );
      psi.add( dict.avp3gppPdpType(), "IPv4" );
      psi.add( dict.avpPdpAddress(), data[ cPDPAddress ] );

req->dump();
   }
   catch ( FDException ex )
   {
    std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
    delete req;
    req = NULL;
   }

   return req;
}

// A handler for Answers corresponding to this specific Request
void ACRreq::processAnswer( FDMessageAnswer &ans )
{

}

AccountingRequest::AccountingRequest(Application& app, Dictionary& dict, CSVProcessor &csvp, CSVFile &file)
   : ACRreq( app ),
     m_csvp( csvp ),
     m_filename( file.filename() ),
     m_recnbr( file.datarecnbr() ),
     m_offset( file.dataoffset() )
{
}

AccountingRequest::~AccountingRequest()
{
}

void AccountingRequest::processAnswer( FDMessageAnswer &ans )
{
   uint32_t rc;

ans.dump();

   bool requestSent = ans.isAnswer();

   if ( !ans.get( getApplication().getDict().avpResultCode(), rc ) )
      rc = 0;

   ACAEventMessage *e = new ACAEventMessage( filename(), recordNumber(), recordOffset(), rc );
   m_csvp.postMessage( e );
}



// ACR Command (cmd) member function

// Function invoked when a ACR Command is received
int ACRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the ACR command.
    return -1;
}
 

}
