/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SD_IMPL_H
#define __SD_IMPL_H

#include "sd.h"
#include "session.h"

class PCRF;

namespace sd {

class SdTSRreq : public TSRreq
{
public:
   SdTSRreq( Application &app, SdIpCan1EstablishSession *event );
   ~SdTSRreq();

   void processAnswer( FDMessageAnswer &ans );

private:
   SdTSRreq();

   SdIpCan1EstablishSession *m_event;
};

class SdRARreq : public REARreq
{
public:
   SdRARreq( Application &app, SessionEvent *event );
   ~SdRARreq();

   void processAnswer( FDMessageAnswer &ans );

private:
   SdRARreq();

   SessionEvent *m_event;
};

class SdRARTreq : public REARreq
{
public:
   SdRARTreq( Application &app, SessionEvent *event );
   ~SdRARTreq();

   void processAnswer( FDMessageAnswer &ans );

private:
   SdRARTreq();

   SessionEvent *m_event;
};

// Member functions that customize the individual application

class CreditControlRequest;

class Application : public ApplicationBase
{
    friend TSRreq;
    friend CRCRreq;
    friend REARreq;

public:
    Application( PCRF &pcrf );
    ~Application();

    //TSRcmd &getTSRcmd() { return m_cmd_tsr; }
    CRCRcmd &getCRCRcmd() { return m_cmd_crcr; }
    //REARcmd &getREARcmd() { return m_cmd_rear; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendTSRreq( SdIpCan1EstablishSession *event );
    bool sendRARreq( SdSession &sd, RulesList &installRules, RulesList &removeRules, SessionEvent *event );
    bool sendRARTreq( SdSession &sd, SdSession::SessionReleaseCause src, SessionEvent *event );

    //bool sendCRCRreq(FDPeer &peer);
    //bool sendREARreq(FDPeer &peer);

private:
    Application();

    void registerHandlers();
    //TSRcmd m_cmd_tsr;
    CRCRcmd m_cmd_crcr;
    //REARcmd m_cmd_rear;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    SdTSRreq *createTSRreq( SdIpCan1EstablishSession *event );
    SdRARreq *createRARreq( SdSession &sd, RulesList &installRules, RulesList &removeRules, SessionEvent *event );
    SdRARTreq *createRARTreq( SdSession &sd, SdSession::SessionReleaseCause src, SessionEvent *event );

    //CRCRreq *createCRCRreq(FDPeer &peer);
    //REARreq *createREARreq(FDPeer &peer);

    PCRF &m_pcrf;
};

}

#endif // __SD_IMPL_H
