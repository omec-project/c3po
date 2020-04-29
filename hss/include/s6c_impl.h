/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __S6C_IMPL_H
#define __S6C_IMPL_H

#include "s6c.h"

class DataAccess;

namespace s6c {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend SERIFSRreq;
    friend ALSCRreq;
    friend RESDSRreq;

public:
    Application( DataAccess & dbobj );
    ~Application();

    SERIFSRcmd &getSERIFSRcmd() { return m_cmd_serifsr; }
    //ALSCRcmd &getALSCRcmd() { return m_cmd_alscr; }
    //RESDSRcmd &getRESDSRcmd() { return m_cmd_resdsr; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendSERIFSRreq(FDPeer &peer);
    bool sendALSCRreq(FDPeer &peer);
    bool sendRESDSRreq(FDPeer &peer);

    DataAccess &getDbObj() { return m_dbobj; }

private:
    void registerHandlers();
    SERIFSRcmd m_cmd_serifsr;
    //ALSCRcmd m_cmd_alscr;
    //RESDSRcmd m_cmd_resdsr;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    SERIFSRreq *createSERIFSRreq(FDPeer &peer);
    ALSCRreq *createALSCRreq(FDPeer &peer);
    RESDSRreq *createRESDSRreq(FDPeer &peer);

    DataAccess &m_dbobj;
};

}

#endif // __S6C_IMPL_H
