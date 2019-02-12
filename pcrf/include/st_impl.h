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

#ifndef __ST_IMPL_H
#define __ST_IMPL_H

#include "st.h"
#include "rule.h"
#include "session.h"

class PCRF;
class StSession;
class SessionEvent;
class StIpCan1EstablishSession;

namespace st {

class StTSRreq : public TSRreq
{
public:
   StTSRreq( Application &app, StIpCan1EstablishSession *event );
   ~StTSRreq();

   void processAnswer( FDMessageAnswer &ans );

private:
   StTSRreq();

   StIpCan1EstablishSession *m_event;
};

class StRulesTSRreq : public TSRreq
{
public:
   StRulesTSRreq( Application &app, SessionEvent *event );
   ~StRulesTSRreq();

   void processAnswer( FDMessageAnswer &ans );

private:
   StRulesTSRreq();

   SessionEvent *m_event;
};

class StSTRreq : public SETRreq
{
public:
   StSTRreq( Application &app, SessionEvent *event );
   ~StSTRreq();

   void processAnswer( FDMessageAnswer &ans );

private:
   StSTRreq();

   SessionEvent *m_event;
};


// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend TSRreq;
    friend SETRreq;
    friend TNRreq;

public:
    Application( PCRF &pcrf );
    ~Application();

    //TSRcmd &getTSRcmd() { return m_cmd_tsr; }
    //SETRcmd &getSETRcmd() { return m_cmd_setr; }
    //TNRcmd &getTNRcmd() { return m_cmd_tnr; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendTSRreq( StIpCan1EstablishSession *event );
    bool sendRulesTSRreq( StSession &st, RulesList &installRules, RulesList &removeRules, SessionEvent *event );
    bool sendSTRreq( StSession &st, StSession::TerminationCause tc, SessionEvent *event );
    //bool sendTNRreq(FDPeer &peer);

private:
    Application();

    void registerHandlers();
    //TSRcmd m_cmd_tsr;
    //SETRcmd m_cmd_setr;
    //TNRcmd m_cmd_tnr;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    StTSRreq *createTSRreq( StIpCan1EstablishSession *event );
    StRulesTSRreq *createRulesTSRreq( StSession &st, RulesList &installRules, RulesList &removeRules, SessionEvent *event );
    StSTRreq *createSTRreq( StSession &st, StSession::TerminationCause tc, SessionEvent *event );
    //TNRreq *createTNRreq(FDPeer &peer);

    PCRF &m_pcrf;
};

}

#endif // __ST_IMPL_H
