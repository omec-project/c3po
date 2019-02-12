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

#ifndef __TDF_H
#define __TDF_H

#include "sd_impl.h"

class TDF
{
public:
   TDF();
   ~TDF();

   bool init();
   void uninit();

   void waitForShutdown();

   sd::Application &sdApp() { return *m_sd; }

private:
   FDEngine m_diameter;

   sd::Application *m_sd;
};

#endif // #define __TDF_H
