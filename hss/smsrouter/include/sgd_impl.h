/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
