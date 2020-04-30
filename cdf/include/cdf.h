/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __CDF_H
#define __CDF_H

#include <string>
#include "rf_impl.h"


class CDF
{
public:
   CDF();
   ~CDF();

   bool start();
   void shutdown();

   void waitForShutdown();

   static std::string currentTime();

private:
   FDEngine m_diameter;
   rf::Application *m_rf;


};

#endif // __CDF_H
