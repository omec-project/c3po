/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "rf.h"

namespace rf {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend ACRreq;

public:
    Application();
    ~Application();

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendACRreq(FDPeer &peer);

private:
	void registerHandlers();

	ACRcmd m_cmd_acr;

	// the parameters for createXXXreq, if present below, may be
	// changed based processing needs
    ACRreq *createACRreq(FDPeer &peer);


};

}
