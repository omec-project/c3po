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
