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

#ifndef __RX_IMPL_H
#define __RX_IMPL_H

#include "rx.h"

class PCRF;

namespace rx {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend AARreq;
    friend REARreq;
    friend SETRreq;
    friend ABSRreq;

public:
    Application( PCRF &pcrf );
    ~Application();

    //AARcmd &getAARcmd() { return m_cmd_aar; }
    //REARcmd &getREARcmd() { return m_cmd_rear; }
    //SETRcmd &getSETRcmd() { return m_cmd_setr; }
    //ABSRcmd &getABSRcmd() { return m_cmd_absr; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendAARreq(FDPeer &peer);
    bool sendREARreq(FDPeer &peer);
    bool sendSETRreq(FDPeer &peer);
    bool sendABSRreq(FDPeer &peer);

private:
    Application();

    void registerHandlers();
    //AARcmd m_cmd_aar;
    //REARcmd m_cmd_rear;
    //SETRcmd m_cmd_setr;
    //ABSRcmd m_cmd_absr;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    AARreq *createAARreq(FDPeer &peer);
    REARreq *createREARreq(FDPeer &peer);
    SETRreq *createSETRreq(FDPeer &peer);
    ABSRreq *createABSRreq(FDPeer &peer);

    PCRF &m_pcrf;
};

}

#endif // __RX_IMPL_H
