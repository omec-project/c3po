/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __GX_IMPL_H
#define __GX_IMPL_H

#include "gx.h"

class PCEF;

namespace gx {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend CRCRreq;
    friend REARreq;

public:
    Application( PCEF &pcef );
    ~Application();

    //CRCRcmd &getCRCRcmd() { return m_cmd_crcr; }
    REARcmd &getREARcmd() { return m_cmd_rear; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendCRCRreq( int32_t ccRequestType );
    //bool sendREARreq(FDPeer &peer);

    PCEF &getPCEF() { return m_pcef; }
    std::string &getSessionId() { return m_sessionid; }

private:
    Application();

    void registerHandlers();
    //CRCRcmd m_cmd_crcr;
    REARcmd m_cmd_rear;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    CRCRreq *createCRCRreq( int32_t ccRequestType );
    //REARreq *createREARreq(FDPeer &peer);

    PCEF &m_pcef;
    std::string m_sessionid;
};

}

#endif // __GX_IMPL_H
