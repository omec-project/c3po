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
