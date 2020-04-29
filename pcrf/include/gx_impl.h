/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __GX_IMPL_H
#define __GX_IMPL_H

#include "gx.h"

class PCRF;
class GxSession;
class SessionEvent;
class RulesList;

namespace gx {

class GxRulesRARreq : public REARreq
{
public:
   GxRulesRARreq( Application &app, SessionEvent *event );
   ~GxRulesRARreq();

   void processAnswer( FDMessageAnswer &ans );

private:
   GxRulesRARreq();

   SessionEvent *m_event;
};

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend CRCRreq;
    friend REARreq;

public:
    Application( PCRF &pcrf  );
    ~Application();

    //CRCRcmd &getCRCRcmd() { return m_cmd_crcr; }
    //REARcmd &getREARcmd() { return m_cmd_rear; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendREARreq(FDPeer &peer);
    bool sendRulesRARreq( GxSession &gx, RulesList &installRules, RulesList &removeRules, SessionEvent *event );

    PCRF &getPCRF() { return m_pcrf; }

private:
    void registerHandlers();
    CRCRcmd m_cmd_crcr;
    //REARcmd m_cmd_rear;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    REARreq *createREARreq(FDPeer &peer);
    GxRulesRARreq *createRulesRARreq( GxSession &gx, RulesList &installRules, RulesList &removeRules, SessionEvent *event );

    PCRF &m_pcrf;
};

}

#endif // __GX_IMPL_H
