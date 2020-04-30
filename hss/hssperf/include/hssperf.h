/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
