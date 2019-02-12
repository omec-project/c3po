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
