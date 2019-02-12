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

#include <sstream>
#include "sstats.h"

#include <ctime>
#include <memory>



StatCollector::StatCollector(const std::string& name):
   m_attemps_sent(0),
   m_sent_ko(0),
   m_attemps_recv(0),
   m_recv_ko(0),
   m_unknownErrors(0),
   m_name(name)
{}

std::string StatCollector::serialize(uint32_t codeColumns){
   std::stringstream res;
   res << std::to_string(m_attemps_sent) << ",";
   res << std::to_string(m_sent_ko) << ",";
   res << std::to_string(m_attemps_recv) << ",";
   res << std::to_string(m_recv_ko) << ",";

   for(auto& val : m_trackedCodes){
      res <<  std::to_string(m_cumulativeCodes[val]) << ",";
   }
   res << std::to_string(m_unknownErrors);
   uint32_t extraColumns = 0;
   if(codeColumns > m_trackedCodes.size()){
      extraColumns = codeColumns - m_trackedCodes.size();
   }
   for(uint32_t iter = 0; iter < extraColumns; ++iter){
      res << ",";
   }
   return res.str();
}
void StatCollector::addStat(uint32_t vendor, uint32_t statcode){
   auto aKey = std::make_pair(vendor, statcode);
   if(m_cumulativeCodes.find(aKey) != m_cumulativeCodes.end()){
      m_cumulativeCodes[aKey] = m_cumulativeCodes[aKey] + 1;
   }
   else{
      m_unknownErrors++;
   }
}
void StatCollector::addAttempt(StatAttempType attempType){
   switch( attempType )
   {
      case stat_attemp_received:
         m_attemps_recv++;
         break;
      case stat_received_ko:
         m_recv_ko++;
         break;
      case stat_attemp_sent:
         m_attemps_sent++;
         break;
      case stat_sent_ko:
         m_sent_ko++;
         break;
      default:
         break;
   }
}
void StatCollector::registerCode(uint32_t vendor, uint32_t statcode){
   auto aKey = std::make_pair(vendor, statcode);
   m_trackedCodes.push_back(aKey);
   m_cumulativeCodes[aKey] = 0;
}

uint32_t StatCollector::getStatValue(uint32_t vendor, uint32_t statcode){
   auto aKey = std::make_pair(vendor, statcode);
   return m_cumulativeCodes[aKey];
}

uint32_t StatCollector::getStatValue(std::pair<uint32_t, uint32_t> key){
   return m_cumulativeCodes[key];
}

std::vector<std::pair<uint32_t, uint32_t>> &StatCollector::getTrackedCodes() {
   return m_trackedCodes;
}

uint32_t StatCollector::getNbCodesTracked(){
   return m_trackedCodes.size();
}

///////////////////////


SStats::SStats(bool logElapsed, StatSerializationMode serializ_mode, StatSerializationEngine engine):
  m_interval(0),
  m_logElapsed(logElapsed),
  m_serializ_mode(serializ_mode),
  m_statlogger(NULL)
{
   switch(engine){
      case _srJson:
         m_serializer = new SStatsSerializerJson();
         break;
      case _srCSV:
         m_serializer = new SStatsSerializerCSV();
         break;
      default:
         break;
   }
}

SStats::~SStats(){

   if(m_serializer != NULL){
      delete m_serializer;
      m_serializer = NULL;
   }
}

void SStats::onInit(){
   m_idletimer.setInterval( m_interval );
   m_idletimer.setOneShot( false );
   initTimer( m_idletimer );
   m_idletimer.start();
   m_elapsedtimer.Start();

}
void SStats::onQuit(){
   if(m_logElapsed){
      stime_t elapsed = m_elapsedtimer.MilliSeconds();
      std::stringstream sstm;
      sstm << elapsed / 1000;
      std::map<std::string, std::string> keyValues;
      keyValues["uptime"]  = sstm.str();
      addGenerationTimeStamp(keyValues);
      std::string serializedStast;
      m_serializer->serialize(keyValues, serializedStast);
      if(m_statlogger) {
         m_statlogger->info(serializedStast);
      }
   }
}

void SStats::onTimer( SEventThread::Timer &t )
{
   if ( t.getId() == m_idletimer.getId() ){
      postMessage( STAT_CONSOLIDATE_EVENT );
   }
}

void SStats::updateInterval (long interval){
   UpdateStatInterval *e = new UpdateStatInterval( interval );
   postMessage( e );
}

std::shared_ptr<std::string> SStats::getLive(){
   auto stats = std::make_shared<std::string>();
   auto event = std::make_shared<SEvent>();
   StatLive *e = new StatLive(event, stats);
   postMessage( e );
   event->wait( 5000 );
   return stats;
}

void SStats::dispatch( SEventThreadMessage &msg )
{
   if ( msg.getId() == STAT_CONSOLIDATE_EVENT ){
      std::string serializedStast;
      if(m_serializ_mode == _srBase){
         std::map<std::string, std::string> keyValues;
         getConsolidatedPeriodStat(keyValues);
         addGenerationTimeStamp(keyValues);
         m_serializer->serialize(keyValues, serializedStast);
      }
      else{
         getSerializedStat(serializedStast);
      }

      if(m_statlogger){
         m_statlogger->info(serializedStast);
      }
      resetStats();
   }
   else if ( msg.getId() == STAT_UPDATE_INTERVAL ) {
      m_idletimer.stop();
      m_interval = ((UpdateStatInterval&)msg).getInterval();
      m_idletimer.setInterval( ((UpdateStatInterval&)msg).getInterval() );
      m_idletimer.start();
   }
   else{
      dispatchDerived(msg);
   }
}

void SStats::addGenerationTimeStamp(std::map<std::string, std::string>& keyValues){
   STime time_now = STime::Now();
   std::string now_str;
   time_now.Format(now_str, "%Y-%m-%d %H:%M:%S.%0", false);
   keyValues["time_utc"] = now_str;
}

void SStats::registerStatAttemp(StatType type, StatAttempType attempType){
   StatAttempMessage *statmsg = new StatAttempMessage(type, attempType);
   this->postMessage(statmsg);
}

void SStats::registerStatResult(StatType type, uint32_t vendor ,uint32_t code){
   StatResultMessage *statmsg = new StatResultMessage(type, vendor, code);
   postMessage(statmsg);
}


void SStats::appendStatObject(RAPIDJSON_NAMESPACE::Value &arrayObjects,
                                RAPIDJSON_NAMESPACE::Document::AllocatorType &allocator,
                                StatCollector &collector){
   RAPIDJSON_NAMESPACE::Value statsObject(RAPIDJSON_NAMESPACE::kObjectType);

   statsObject.AddMember("type", RAPIDJSON_NAMESPACE::StringRef(collector.getName().c_str()), allocator);
   statsObject.AddMember("sent_att", collector.getNbAttempsSent(), allocator);
   statsObject.AddMember("sent_ko", collector.getNbSentKo(), allocator);
   statsObject.AddMember("recv_att", collector.getNbAttempsRecv(), allocator);
   statsObject.AddMember("recv_ko", collector.getNbRecvKo(), allocator);

   statsObject.AddMember("unknown", collector.getNbUnknownErrors(), allocator);

   RAPIDJSON_NAMESPACE::Value arrayValues(RAPIDJSON_NAMESPACE::kArrayType);
   std::vector<std::pair<uint32_t, uint32_t>> trackedcode = collector.getTrackedCodes();
   for(auto val : trackedcode){
      uint32_t nbHits = collector.getStatValue(val);
      RAPIDJSON_NAMESPACE::Value valueObject(RAPIDJSON_NAMESPACE::kObjectType);
      valueObject.AddMember("vendor", val.first , allocator);
      valueObject.AddMember("code", val.second, allocator);
      valueObject.AddMember("nb", nbHits, allocator);
      arrayValues.PushBack(valueObject, allocator);
   }
   statsObject.AddMember("values", arrayValues, allocator);
   arrayObjects.PushBack(statsObject, allocator);
}

///////////////////////////////////////////////////////////////////////////////////

void SStatsSerializerJson::serialize(std::map<std::string, std::string>& keyValues, std::string& output){

   fdrapidjson::Document document;
   document.SetObject();
   fdrapidjson::Document::AllocatorType& allocator = document.GetAllocator();

   for (std::map<std::string, std::string>::const_iterator it=keyValues.begin(); it!=keyValues.end(); ++it){

      document.AddMember(fdrapidjson::StringRef(it->first.c_str()), fdrapidjson::StringRef(it->second.c_str()), allocator);
   }

   fdrapidjson::StringBuffer strbuf;
   fdrapidjson::Writer<fdrapidjson::StringBuffer> writer(strbuf);
   document.Accept(writer);
   output =  strbuf.GetString();
}

void SStatsSerializerCSV::serialize(std::map<std::string, std::string>& keyValues, std::string& output){
   std::stringstream ss;
   ss << keyValues["time_utc"];
   for(auto& pair : keyValues){
      if(pair.first != "time_utc"){
         ss << "," << pair.second;
      }
   }
   output = ss.str();
}


