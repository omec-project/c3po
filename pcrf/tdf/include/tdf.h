/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
