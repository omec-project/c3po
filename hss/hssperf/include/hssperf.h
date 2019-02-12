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

#ifndef __HSSPERF_H
#define __HSSPERF_H

#include "s6as6d_impl.h"

class HSSPerformance
{
public:
   HSSPerformance();
   ~HSSPerformance();

   bool init();
   void uninit();

   void waitForShutdown();

   s6as6d::Application &s6as6dApp() { return *m_s6as6d; }

private:
   FDEngine m_diameter;

   s6as6d::Application *m_s6as6d;
   bool m_repetitive;
};

#endif // #define __HSSPERF_H
