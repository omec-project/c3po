/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
