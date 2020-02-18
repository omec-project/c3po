/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __STATSCTF_H
#define __STATSCTF_H

#include <sstream>
#include "sstats.h"
#include "stimer.h"

class StatsCtf : public SStats {
public:
   static void initstats(SLogger* statlogger) { singleton().setStatLogger(statlogger); };
   static StatsCtf &singleton() { if (!m_singleton) m_singleton = new StatsCtf(); return *m_singleton; }
   void getSerializedStat(std::string& stats);
   void dispatchDerived(SEventThreadMessage &msg);
   void resetStats();
   void processStatResult(StatResultMessage& stat);
   void processStatAttemp(StatAttempMessage& stat);
   void processStatGetLive(StatLive& msg);

private:
   StatsCtf();
   ~StatsCtf();

   static StatsCtf *m_singleton;
   StatCollector m_acr_collector;
   uint32_t m_max_codes_tracked;
};


#endif // #define __TESTS_H
