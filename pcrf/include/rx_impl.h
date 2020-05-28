/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
