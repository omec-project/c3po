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
