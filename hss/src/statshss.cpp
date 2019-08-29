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

#include "statshss.h"

#include <sstream>
#include <freeDiameter/freeDiameter-host.h>
#include <freeDiameter/libfdproto.h>
#include <common_def.h>


StatsHss *StatsHss::m_singleton = NULL;


StatsHss::StatsHss()
   : SStats(false, SStats::_srDerived, SStats::_srCSV),
     m_ulr_collector("ulr"),
     m_air_collector("air"),
     m_pur_collector("pur"),
     m_cir_collector("cir"),
     m_nir_collector("nir"),
     m_idr_collector("idr"),
     m_rir_collector("rir"),
     m_srr_collector("srr"),
     m_max_codes_tracked(0)
{

   m_ulr_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_ulr_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_ulr_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);
   m_ulr_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_RAT_NOT_ALLOWED);
   m_ulr_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_UNKNOWN_SERVING_NODE);
   m_ulr_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_UNKNOWN_EPS_SUBSCRIPTION);

   m_air_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_air_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_air_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_RAT_NOT_ALLOWED);
   m_air_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_ROAMING_NOT_ALLOWED);
   m_air_collector.registerCode(VENDOR_3GPP, DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE);

   m_pur_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_pur_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_pur_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);
   m_pur_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_UNKNOWN_SERVING_NODE);

   m_cir_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_cir_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);

   m_nir_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_nir_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);
   m_nir_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_USER_NO_APN_SUBSCRIPTION);

   m_idr_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_idr_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);

   m_rir_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_rir_collector.registerCode(0, ER_DIAMETER_INVALID_AVP_VALUE);
   m_rir_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);

   m_srr_collector.registerCode(0, ER_DIAMETER_SUCCESS);
   m_srr_collector.registerCode(0, ER_DIAMETER_MISSING_AVP);
   m_srr_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);
   m_srr_collector.registerCode(VENDOR_3GPP, DIAMETER_ERROR_ABSENT_USER);

   m_max_codes_tracked = m_ulr_collector.getNbCodesTracked();
   if(m_air_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_air_collector.getNbCodesTracked();
   }
   if(m_pur_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_air_collector.getNbCodesTracked();
   }
   if(m_cir_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_cir_collector.getNbCodesTracked();
   }
   if(m_nir_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_nir_collector.getNbCodesTracked();
   }
   if(m_idr_collector.getNbCodesTracked() > m_max_codes_tracked){
      m_max_codes_tracked = m_idr_collector.getNbCodesTracked();
   }
   if(m_rir_collector.getNbCodesTracked() > m_max_codes_tracked) {
      m_max_codes_tracked = m_rir_collector.getNbCodesTracked();
   }
}

StatsHss::~StatsHss() {
}

void StatsHss::getSerializedStat(std::string& stats){
   STime time_now = STime::Now();
   std::string now_str;
   time_now.Format(now_str, "%Y-%m-%dT%TZ", false);
   std::stringstream res;

   res << now_str << ",S6A,AIR," << m_air_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",S6A,ULR," << m_ulr_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",S6A,PUR," << m_pur_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",S6A,IDR," << m_idr_collector.serialize(m_max_codes_tracked) << std::endl;

   res << now_str << ",S6T,CIR," << m_cir_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",S6T,NIR," << m_nir_collector.serialize(m_max_codes_tracked) << std::endl;
   res << now_str << ",S6T,RIR," << m_rir_collector.serialize(m_max_codes_tracked) << std::endl;

   res << now_str << ",S6C,SRR," << m_rir_collector.serialize(m_max_codes_tracked);
   stats = res.str();
}

void StatsHss::dispatchDerived(SEventThreadMessage &msg){
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

void StatsHss::processStatAttemp(StatAttempMessage& stat){
   switch( stat.getType() )
   {
      case stat_hss_ulr:
         m_ulr_collector.addAttempt(stat.getAttempType());
         break;
      case stat_hss_air:
         m_air_collector.addAttempt(stat.getAttempType());
         break;
      case stat_hss_pur:
         m_pur_collector.addAttempt(stat.getAttempType());
         break;
      case stat_hss_cir:
         m_cir_collector.addAttempt(stat.getAttempType());
         break;
      case stat_hss_nir:
         m_nir_collector.addAttempt(stat.getAttempType());
         break;
      case stat_hss_idr:
         m_idr_collector.addAttempt(stat.getAttempType());
         break;
      case stat_hss_rir:
         m_rir_collector.addAttempt(stat.getAttempType());
         break;
      case stat_hss_srr:
         m_srr_collector.addAttempt(stat.getAttempType());
         break;
      default:
         break;
   }
}

void StatsHss::processStatResult(StatResultMessage& stat){
   switch( stat.getType() )
   {
      case stat_hss_ulr:
         m_ulr_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_hss_air:
         m_air_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_hss_pur:
         m_pur_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_hss_cir:
         m_cir_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_hss_nir:
         m_nir_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_hss_idr:
         m_idr_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_hss_rir:
         m_rir_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      case stat_hss_srr:
         m_srr_collector.addStat(stat.getVendor(), stat.getCode());
         break;
      default:
         break;
   }
}


void StatsHss::processStatGetLive(StatLive& msg){
   RAPIDJSON_NAMESPACE::Document document;
   document.SetObject();
   RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = document.GetAllocator();
   RAPIDJSON_NAMESPACE::Value arrayObjects(RAPIDJSON_NAMESPACE::kArrayType);

   appendStatObject(arrayObjects, allocator, m_ulr_collector);
   appendStatObject(arrayObjects, allocator, m_air_collector);
   appendStatObject(arrayObjects, allocator, m_pur_collector);
   appendStatObject(arrayObjects, allocator, m_cir_collector);
   appendStatObject(arrayObjects, allocator, m_nir_collector);
   appendStatObject(arrayObjects, allocator, m_idr_collector);
   appendStatObject(arrayObjects, allocator, m_rir_collector);
   appendStatObject(arrayObjects, allocator, m_srr_collector);

   document.AddMember("stats", arrayObjects, allocator);
   RAPIDJSON_NAMESPACE::StringBuffer strbuf;
   RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(strbuf);
   document.Accept(writer);
   msg.setlivestats(strbuf.GetString());
   msg.set();
}

void StatsHss::resetStats(){
   //HSS Stats are accumulative, nothing to do here
}
