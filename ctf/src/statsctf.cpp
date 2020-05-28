/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/
#include "statsctf.h"

#include <sstream>

#include <freeDiameter/freeDiameter-host.h>
#include <freeDiameter/libfdproto.h>
#include <common_def.h>

StatsCtf *StatsCtf::m_singleton = NULL;

StatsCtf::StatsCtf() :
   SStats(false, SStats::_srDerived , SStats::_srCSV),
   m_acr_collector("acr")
{
   m_acr_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_max_codes_tracked = m_acr_collector.getNbCodesTracked();
}

StatsCtf::~StatsCtf(){

}

void StatsCtf::getSerializedStat(std::string& stats){
   STime time_now = STime::Now();
   std::string now_str;
   time_now.Format(now_str, "%Y-%m-%dT%TZ", false);
   std::stringstream res;
   res << now_str << ",RF,ACR," << m_acr_collector.serialize(m_max_codes_tracked);
   stats = res.str();
}

void StatsCtf::dispatchDerived(SEventThreadMessage &msg){
   switch ( msg.getId() )
   {
      case STAT_ATTEMPT_MSG:
         processStatAttemp((StatAttempMessage&)msg);
         break;
      case STAT_RSLT_MSG:
         processStatResult((StatResultMessage&)msg);
         break;
      case STAT_GET_LIVE:
         processStatGetLive((StatLive&)msg);
         break;
      default:
         break;
   }
}

void StatsCtf::processStatAttemp(StatAttempMessage& stat){
   switch( stat.getType() )
   {
      case stat_ctf_acr:
         m_acr_collector.addAttempt( stat.getAttempType() );
         break;
      default:
         break;
   }
}

void StatsCtf::processStatResult(StatResultMessage& stat){
   switch( stat.getType() )
   {
      case stat_ctf_acr:
         m_acr_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      default:
         break;
   }
}

void StatsCtf::processStatGetLive(StatLive& msg){
   RAPIDJSON_NAMESPACE::Document document;
   document.SetObject();
   RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = document.GetAllocator();
   RAPIDJSON_NAMESPACE::Value arrayObjects(RAPIDJSON_NAMESPACE::kArrayType);

   appendStatObject(arrayObjects, allocator, m_acr_collector);

   document.AddMember("stats", arrayObjects, allocator);
   RAPIDJSON_NAMESPACE::StringBuffer strbuf;
   RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(strbuf);
   document.Accept(writer);
   msg.setlivestats(strbuf.GetString());
   msg.set();
}

void StatsCtf::resetStats(){
   //
}
