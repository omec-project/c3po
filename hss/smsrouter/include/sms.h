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

#ifndef __SMSROUTER_H
#define __SMSROUTER_H

#include "s6c_impl.h"
#include "sgd_impl.h"

class SMSRouter
{
public:
   SMSRouter();
   ~SMSRouter();

   bool init();
   void uninit();

   void waitForShutdown();

   s6c::Application &s6cApp() { return *m_s6c; }
   sgd::Application &sgdApp() { return *m_sgd; }

private:
   FDEngine m_diameter;

   s6c::Application *m_s6c;
   sgd::Application *m_sgd;
   bool m_repetitive;
};

#endif // #define __SMSROUTER_H
