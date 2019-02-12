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
