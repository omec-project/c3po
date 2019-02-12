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

class TSSF;

namespace st {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend TSRreq;
    friend SETRreq;
    friend TNRreq;

public:
    Application( TSSF &tssf );
    ~Application();

    TSRcmd &getTSRcmd() { return m_cmd_tsr; }
    SETRcmd &getSETRcmd() { return m_cmd_setr; }
    //TNRcmd &getTNRcmd() { return m_cmd_tnr; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendTNRreq(FDPeer &peer);

    std::string &getSessionId() { return m_sessionid; }

private:
    void registerHandlers();
    TSRcmd m_cmd_tsr;
    SETRcmd m_cmd_setr;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    TNRreq *createTNRreq(FDPeer &peer);

    TSSF &m_tssf;
    std::string m_sessionid;
};

}

#endif // __ST_IMPL_H
