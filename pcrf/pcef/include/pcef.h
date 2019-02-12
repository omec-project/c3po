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

#ifndef __PCEF_H
#define __PCEF_H

#include "gx_impl.h"

class PCEF
{
public:
   PCEF();
   ~PCEF();

   bool init();
   void uninit();

   void waitForShutdown();

   gx::Application &gxApp() { return *m_gx; }

   bool getRepetitive() { return m_repetitive; }
   bool setRepetitive( bool v ) { return m_repetitive = v; }

private:
   FDEngine m_diameter;

   gx::Application *m_gx;
   bool m_repetitive;
};

#endif // #define __PCEF_H
