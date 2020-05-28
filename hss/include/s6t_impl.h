/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __S6T_IMPL_H
#define __S6T_IMPL_H

#ifdef __cplusplus

#include "s6t.h"

class DataAccess;

namespace s6t {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend COIRreq;
    friend REIRreq;
    friend NIIRreq;

public:
    Application( DataAccess & dbobj );
    ~Application();

    COIRcmd &getCOIRcmd() { return m_cmd_coir; }
    //REIRcmd &getREIRcmd() { return m_cmd_reir; }
    NIIRcmd &getNIIRcmd() { return m_cmd_niir; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendCOIRreq(FDPeer &peer);
    bool sendREIRreq(FDPeer &peer);
    bool sendNIIRreq(FDPeer &peer);

    DataAccess &getDbObj();

private:
    void registerHandlers();
    COIRcmd m_cmd_coir;
    //REIRcmd m_cmd_reir;
    NIIRcmd m_cmd_niir;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    COIRreq *createCOIRreq(FDPeer &peer);
    REIRreq *createREIRreq(FDPeer &peer);
    NIIRreq *createNIIRreq(FDPeer &peer);

    DataAccess &m_dbobj;
};

}

#endif
#endif // __S6T_IMPL_H
