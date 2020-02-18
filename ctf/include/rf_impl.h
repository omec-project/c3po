/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __RF_IMPL_H
#define __RF_IMPL_H

#include "rf.h"
#include "sdn.h"

class CSVProcessor;
class CSVFile;
class StatsCtf;

namespace rf {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend ACRreq;

public:
    Application();
    ~Application();

    //ACRcmd &getACRcmd() { return m_cmd_acr; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendACRreq(CSVProcessor &csvp, FDPeer &peer, CSVFile &file);

private:

   static const int cDPNTime           = 0;
   static const int cPDPAddress        = 1;
   static const int cDownlinkBytes     = 3;
   static const int cUplinkBytes       = 5;
   static const int cRuleType          = 7;
   static const int cRule              = 8;
   static const int cSponsorIdentity   = 10;
   static const int cServiceIdentifier = 11;
   static const int cRatingGroup       = 12;
   static const int cLifeCycle         = 15;

   //Application();
	void registerHandlers();


   // the parameters for createXXXreq, if present below, may be
   // changed based processing needs
   ACRreq *createACRreq(CSVProcessor &csvp, FDPeer &peer, CSVFile &file);
   SDN m_sdn;
};

class AccountingRequest : public ACRreq
{
public:
   AccountingRequest( Application& app, Dictionary& dict, CSVProcessor &csvp, CSVFile &file );
   ~AccountingRequest();

   void processAnswer( FDMessageAnswer &ans );

   CSVProcessor &processor() { return m_csvp; }
   const std::string &filename() { return m_filename; }
   uint32_t recordNumber() { return m_recnbr; }
   int64_t recordOffset() { return m_offset; }

   const std::string &getSessionId() { return m_session.getSessionId(); }
private:
   AccountingRequest();

   CSVProcessor &m_csvp;
   std::string m_filename;
   uint32_t m_recnbr;
   int64_t m_offset;
   FDSession m_session;
};

}

#endif /* __RF_IMPL_H */
