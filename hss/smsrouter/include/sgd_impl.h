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

#ifndef __SGD_IMPL_H
#define __SGD_IMPL_H

#include "sgd.h"

namespace sgd {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend MOFSMRreq;
    friend MTFSMRreq;
    friend ALSCRreq;

public:
    Application();
    ~Application();

    MOFSMRcmd &getMOFSMRcmd() { return m_cmd_mofsmr; }
    //MTFSMRcmd &getMTFSMRcmd() { return m_cmd_mtfsmr; }
    //ALSCRcmd &getALSCRcmd() { return m_cmd_alscr; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendMOFSMRreq(FDPeer &peer);
    bool sendMTFSMRreq(MoForwardShortMessageRequestExtractor &ofr);
    bool sendALSCRreq(FDPeer &peer);

private:
    void registerHandlers();
    MOFSMRcmd m_cmd_mofsmr;
    //MTFSMRcmd m_cmd_mtfsmr;
    //ALSCRcmd m_cmd_alscr;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    MOFSMRreq *createMOFSMRreq(FDPeer &peer);
    MTFSMRreq *createMTFSMRreq(MoForwardShortMessageRequestExtractor &ofr);
    ALSCRreq *createALSCRreq(FDPeer &peer);
};

}

#endif // __SGD_IMPL_H
