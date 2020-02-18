/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef HSS_SRC_STATPCRF_H_
#define HSS_SRC_STATPCRF_H_

#include "sstats.h"
#include "stimer.h"


class StatsPcrf : public SStats {
public:

   virtual ~StatsPcrf();

   static void initstats(SLogger* statlogger) { singleton().setStatLogger(statlogger); };
   static StatsPcrf &singleton() { if (!m_singleton) m_singleton = new StatsPcrf(); return *m_singleton; }
   void getSerializedStat(std::string& stats);
   void dispatchDerived(SEventThreadMessage &msg);
   void resetStats();
   void processStatResult(StatResultMessage& stat);
   void processStatAttemp(StatAttempMessage& stat);
   void processStatGetLive(StatLive& msg);

private:

   StatsPcrf();

   static StatsPcrf *m_singleton;

   StatCollector m_gx_ccr_collector;
   StatCollector m_gx_rar_collector;
   StatCollector m_sd_tsr_collector;
   StatCollector m_sd_rar_collector;
   StatCollector m_sd_ccr_collector;
   StatCollector m_st_tsr_collector;
   StatCollector m_st_str_collector;

   uint32_t m_max_codes_tracked;

};

#endif /* HSS_SRC_STATPCRF_H_ */
