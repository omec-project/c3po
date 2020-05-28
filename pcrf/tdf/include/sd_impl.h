/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SD_IMPL_H
#define __SD_IMPL_H

#include "sd.h"

class TDF;

namespace sd {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend TSRreq;
    friend CRCRreq;
    friend REARreq;

public:
    Application( TDF &tdf );
    ~Application();

    TSRcmd &getTSRcmd() { return m_cmd_tsr; }
    REARcmd &getREARcmd() { return m_cmd_rear; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendCRCRreq(FDPeer &peer);

    std::string &getSessionId() { return m_sessionid; }

private:
    void registerHandlers();
    TSRcmd m_cmd_tsr;
    REARcmd m_cmd_rear;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    CRCRreq *createCRCRreq(FDPeer &peer);

    TDF &m_tdf;
    std::string m_sessionid;
};

}

#endif // __SD_IMPL_H
