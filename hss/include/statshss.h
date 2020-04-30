/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef HSS_SRC_STATSHSS_H_
#define HSS_SRC_STATSHSS_H_

#include "sstats.h"
#include "stimer.h"


class StatsHss : public SStats {
public:

   virtual ~StatsHss();

   static void initstats(SLogger* statlogger) { singleton().setStatLogger(statlogger); };
   static StatsHss &singleton() { if (!m_singleton) m_singleton = new StatsHss(); return *m_singleton; }
   void getSerializedStat(std::string& stats);
   void dispatchDerived(SEventThreadMessage &msg);
   void resetStats();
   void processStatResult(StatResultMessage& stat);
   void processStatAttemp(StatAttempMessage& stat);
   void processStatGetLive(StatLive& msg);

private:

   StatsHss();

   static StatsHss *m_singleton;

   StatCollector m_ulr_collector;
   StatCollector m_air_collector;
   StatCollector m_pur_collector;
   StatCollector m_cir_collector;
   StatCollector m_nir_collector;
   StatCollector m_idr_collector;
   StatCollector m_rir_collector;
   StatCollector m_srr_collector;

   uint32_t m_max_codes_tracked;

};

#endif /* HSS_SRC_STATSHSS_H_ */
