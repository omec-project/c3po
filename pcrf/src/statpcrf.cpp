/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "statpcrf.h"

#include <sstream>
#include <freeDiameter/freeDiameter-host.h>
#include <freeDiameter/libfdproto.h>
#include <common_def.h>


StatsPcrf *StatsPcrf::m_singleton = NULL;


StatsPcrf::StatsPcrf()
   : SStats(false, SStats::_srDerived, SStats::_srCSV),
     m_max_codes_tracked(0),
     m_gx_ccr_collector("gx_ccr"),
     m_gx_rar_collector("gx_rar"),
     m_sd_tsr_collector("sd_tsr"),
     m_sd_rar_collector("sd_rar"),
     m_sd_ccr_collector("sd_ccr"),
     m_st_tsr_collector("st_tsr"),
     m_st_str_collector("st_str")

{

   m_gx_ccr_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_gx_ccr_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_gx_rar_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_gx_rar_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_sd_tsr_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_sd_tsr_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_sd_rar_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_sd_rar_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_sd_ccr_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_sd_ccr_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_st_tsr_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_st_tsr_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_st_str_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_st_str_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);

   m_max_codes_tracked = m_gx_ccr_collector.getNbCodesTracked();

   if(m_gx_rar_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_gx_rar_collector.getNbCodesTracked();
   }
   if(m_sd_tsr_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_sd_tsr_collector.getNbCodesTracked();
   }
   if(m_sd_rar_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_sd_rar_collector.getNbCodesTracked();
   }
   if(m_sd_ccr_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_sd_ccr_collector.getNbCodesTracked();
   }
   if(m_st_tsr_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_st_tsr_collector.getNbCodesTracked();
   }
   if(m_st_str_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_st_str_collector.getNbCodesTracked();
   }
}

StatsPcrf::~StatsPcrf() {
}

void StatsPcrf::getSerializedStat(std::string& stats){
   STime time_now = STime::Now();
   std::string now_str;
   time_now.Format(now_str, "%Y-%m-%dT%TZ", false);
   std::stringstream res;
   res << now_str << ",GX,CCR," << m_gx_ccr_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",GX,RAR," << m_gx_rar_collector.serialize(m_max_codes_tracked) << std::endl;

   res << now_str << ",SD,TSR," << m_sd_tsr_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",SD,RAR," << m_sd_rar_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",SD,CCR," << m_sd_ccr_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",ST,TSR," << m_st_tsr_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",ST,STR," << m_st_str_collector.serialize(m_max_codes_tracked);

   stats = res.str();
}

void StatsPcrf::dispatchDerived(SEventThreadMessage &msg){
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

void StatsPcrf::processStatAttemp(StatAttempMessage& stat){
   switch( stat.getType() )
   {
      case stat_pcrf_gx_ccr:
         m_gx_ccr_collector.addAttempt(stat.getAttempType());
         break;
      case stat_pcrf_gx_rar:
         m_gx_rar_collector.addAttempt(stat.getAttempType());
         break;
      case stat_pcrf_sd_tsr:
         m_sd_tsr_collector.addAttempt(stat.getAttempType());
         break;
      case stat_pcrf_sd_rar:
         m_sd_rar_collector.addAttempt(stat.getAttempType());
         break;
      case stat_pcrf_sd_ccr:
         m_sd_ccr_collector.addAttempt(stat.getAttempType());
         break;
      case stat_pcrf_st_tsr:
         m_st_tsr_collector.addAttempt(stat.getAttempType());
         break;
      case stat_pcrf_st_str:
         m_st_str_collector.addAttempt(stat.getAttempType());
         break;
      default:
         break;
   }
}

void StatsPcrf::processStatResult(StatResultMessage& stat){

   switch( stat.getType() )
   {
      case stat_pcrf_gx_ccr:
         m_gx_ccr_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_pcrf_gx_rar:
         m_gx_rar_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_pcrf_sd_tsr:
         m_sd_tsr_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_pcrf_sd_rar:
         m_sd_rar_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_pcrf_sd_ccr:
         m_sd_ccr_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_pcrf_st_tsr:
         m_st_tsr_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_pcrf_st_str:
         m_st_str_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      default:
         break;
   }

}


void StatsPcrf::processStatGetLive(StatLive& msg){
   RAPIDJSON_NAMESPACE::Document document;
   document.SetObject();
   RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = document.GetAllocator();
   RAPIDJSON_NAMESPACE::Value arrayObjects(RAPIDJSON_NAMESPACE::kArrayType);

   appendStatObject(arrayObjects, allocator, m_gx_ccr_collector);
   appendStatObject(arrayObjects, allocator, m_gx_rar_collector);
   appendStatObject(arrayObjects, allocator, m_sd_tsr_collector);
   appendStatObject(arrayObjects, allocator, m_sd_rar_collector);
   appendStatObject(arrayObjects, allocator, m_sd_ccr_collector);
   appendStatObject(arrayObjects, allocator, m_st_tsr_collector);
   appendStatObject(arrayObjects, allocator, m_st_str_collector);

   document.AddMember("stats", arrayObjects, allocator);
   RAPIDJSON_NAMESPACE::StringBuffer strbuf;
   RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(strbuf);
   document.Accept(writer);
   msg.setlivestats(strbuf.GetString());
   msg.set();
}

void StatsPcrf::resetStats(){
   //HSS Stats are accumulative, nothing to do here
}
