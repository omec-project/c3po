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

#ifndef __SSTATS_H
#define __SSTATS_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <map>
#include <functional>
#include <memory>

#include "sthread.h"
#include "ssyslog.h"
#include "stimer.h"
#include "stime.h"
#include "slogger.h"

#define RAPIDJSON_NAMESPACE fdrapidjson
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

const uint16_t STAT_CONSOLIDATE_EVENT = ETM_USER + 1;
const uint16_t STAT_UPDATE_INTERVAL = ETM_USER + 2;
const uint16_t STAT_GET_LIVE = ETM_USER + 3;


const uint16_t STAT_RSLT_MSG = ETM_USER + 10;
const uint16_t STAT_ATTEMPT_MSG = ETM_USER + 11;

enum StatType {
   stat_hss_ulr,
   stat_hss_air,
   stat_hss_pur,
   stat_hss_cir,
   stat_hss_nir,
   stat_hss_idr,
   stat_hss_rir,
   stat_hss_srr,
   stat_ctf_acr,
   stat_pcrf_gx_ccr,
   stat_pcrf_gx_rar,
   stat_pcrf_sd_tsr,
   stat_pcrf_sd_rar,
   stat_pcrf_sd_ccr,
   stat_pcrf_st_tsr,
   stat_pcrf_st_str
};

enum StatAttempType {
   stat_attemp_sent,
   stat_sent_ko,
   stat_attemp_received,
   stat_received_ko
};

class StatCollector{

public:
   StatCollector(const std::string& name);
   std::string serialize(uint32_t codeColumns);
   void addStat(uint32_t vendor, uint32_t statcode);
   void addAttempt(StatAttempType attempType);
   void registerCode(uint32_t vendor, uint32_t statcode);
   uint32_t getStatValue(uint32_t vendor, uint32_t statcode);
   uint32_t getStatValue(std::pair<uint32_t, uint32_t> key);
   uint32_t getNbCodesTracked();
   uint32_t getNbAttempsSent() { return m_attemps_sent; }
   uint32_t getNbSentKo() { return m_sent_ko; }
   uint32_t getNbAttempsRecv() { return m_attemps_recv; }
   uint32_t getNbRecvKo() { return m_recv_ko; }


   uint32_t getNbUnknownErrors() { return m_unknownErrors; }
   std::vector<std::pair<uint32_t, uint32_t>> &getTrackedCodes();
   std::string &getName(){ return m_name; }

private:
   uint32_t m_attemps_sent;
   uint32_t m_sent_ko;
   uint32_t m_attemps_recv;
   uint32_t m_recv_ko;

   uint32_t m_unknownErrors;
   std::map<std::pair<uint32_t, uint32_t>, uint32_t> m_cumulativeCodes;
   std::vector<std::pair<uint32_t, uint32_t>> m_trackedCodes;
   std::string m_name;
};

class StatResultMessage : public SEventThreadMessage {
public:
   StatResultMessage( StatType type, uint32_t vendor ,uint32_t code)
      : SEventThreadMessage( STAT_RSLT_MSG ),
        m_type(type),
        m_vendor(vendor),
        m_code(code) {
   }
   StatType getType() { return m_type; }
   uint32_t getVendor() { return m_vendor; }
   uint32_t getCode() { return m_code; }

private:
   StatResultMessage();
   StatType m_type;
   uint32_t m_vendor;
   uint32_t m_code;
};

class StatAttempMessage : public SEventThreadMessage {
public:
   StatAttempMessage( StatType type, StatAttempType attempType)
      : SEventThreadMessage( STAT_ATTEMPT_MSG ),
        m_type(type),
        m_attemp_type(attempType){
   }
   StatType getType() { return m_type; }
   StatAttempType getAttempType() { return m_attemp_type; }

private:
   StatAttempMessage();
   StatType m_type;
   StatAttempType m_attemp_type;
};


class SStatsSerializer {

public:
   virtual ~SStatsSerializer(){}
   virtual void serialize(std::map<std::string, std::string>& keyValues, std::string& output) = 0;
};

class SStatsSerializerJson : public SStatsSerializer  {
public:
   void serialize(std::map<std::string, std::string>& keyValues, std::string& output);
};

class SStatsSerializerCSV : public SStatsSerializer  {
public:
   void serialize(std::map<std::string, std::string>& keyValues, std::string& output);
};

class UpdateStatInterval : public SEventThreadMessage
{
public:
   UpdateStatInterval( long interval )
      : SEventThreadMessage( STAT_UPDATE_INTERVAL ),
        m_interval(interval) {
   }

   long getInterval() { return m_interval; }

private:
   long m_interval;
};


class StatLive : public SEventThreadMessage
{
public:
   StatLive(std::shared_ptr<SEvent> event,
            std::shared_ptr<std::string> livestats)
      : SEventThreadMessage( STAT_GET_LIVE ),
        m_event(event),
        m_livestats(livestats)
   {
   }
   void set() { m_event->set(); }
   std::shared_ptr<std::string> getlivestats() { return m_livestats; }
   void setlivestats(const std::string& livestats) { *m_livestats = livestats; }
   void setlivestats(const char* livestats) { m_livestats->assign(livestats); }

private:
   std::shared_ptr<SEvent> m_event;
   std::shared_ptr<std::string> m_livestats;
};

class SStats : public SEventThread
{

public :

   enum StatSerializationEngine {
      _srUndefined,
      _srJson,
      _srCSV
   };

   enum StatSerializationMode {
      _srBase,
      _srDerived
   };

   SStats(bool logElapsed, StatSerializationMode serializ_mode, StatSerializationEngine engine);
   virtual ~SStats();
   void onInit();
   void onQuit();
   void onTimer( SEventThread::Timer &t );
   void setInterval(long interval) { m_interval = interval; }
   long getInterval() { return m_interval; }
   void updateInterval (long interval);
   void setStatLogger(SLogger* statlogger) { m_statlogger = statlogger; }
   std::shared_ptr<std::string> getLive();
   void dispatch( SEventThreadMessage &msg );
   virtual void getConsolidatedPeriodStat(std::map<std::string, std::string>& keyValues) {};
   virtual void getSerializedStat(std::string& stats) {};
   virtual void dispatchDerived(SEventThreadMessage &msg) = 0;
   virtual void resetStats() = 0;
   void registerStatAttemp(StatType type, StatAttempType attempType);
   void registerStatResult(StatType type, uint32_t vendor ,uint32_t code);
   void appendStatObject(RAPIDJSON_NAMESPACE::Value &arrayObjects,
                         RAPIDJSON_NAMESPACE::Document::AllocatorType &allocator,
                         StatCollector &collector);

private:

   void addGenerationTimeStamp(std::map<std::string, std::string>& keyValues);

   long m_interval;
   SEventThread::Timer m_idletimer;
   SStatsSerializer  *m_serializer;
   STimerElapsed m_elapsedtimer;
   bool m_logElapsed;

   StatSerializationMode m_serializ_mode;

   SLogger* m_statlogger;
};



#endif /* __SSTATS_H_ */
