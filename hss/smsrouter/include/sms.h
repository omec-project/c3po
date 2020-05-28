/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
