/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __TSSF_H
#define __TSSF_H

#include "st_impl.h"

class TSSF
{
public:
   TSSF();
   ~TSSF();

   bool init();
   void uninit();

   void waitForShutdown();

   st::Application &stApp() { return *m_st; }

private:
   FDEngine m_diameter;

   st::Application *m_st;
};

#endif // #define __TSSF_H
