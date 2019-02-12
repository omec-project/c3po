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
