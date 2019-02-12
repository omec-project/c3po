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

#ifndef __S6C_IMPL_H
#define __S6C_IMPL_H

#include "s6c.h"

namespace s6c {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend SERIFSRreq;
    friend ALSCRreq;
    friend RESDSRreq;

public:
    Application();
    ~Application();

    //SERIFSRcmd &getSERIFSRcmd() { return m_cmd_serifsr; }
    //ALSCRcmd &getALSCRcmd() { return m_cmd_alscr; }
    //RESDSRcmd &getRESDSRcmd() { return m_cmd_resdsr; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendSERIFSRreq( bool withMsisdn, bool withImsi );
    bool sendALSCRreq(FDPeer &peer);
    bool sendRESDSRreq(FDPeer &peer);

private:
    void registerHandlers();
    //SERIFSRcmd m_cmd_serifsr;
    //ALSCRcmd m_cmd_alscr;
    //RESDSRcmd m_cmd_resdsr;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    SERIFSRreq *createSERIFSRreq( bool withMsisdn, bool withImsi );
    ALSCRreq *createALSCRreq(FDPeer &peer);
    RESDSRreq *createRESDSRreq(FDPeer &peer);
};

}

#endif // __S6C_IMPL_H
