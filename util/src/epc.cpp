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

#include <stdlib.h>
#include <iostream>

#include "epc.h"

using namespace EPC;

std::string Utility::home_network( const char *mnc, const char *mcc )
{
   std::string s;

   // '^mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::home_network( const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return home_network( mnc, mcc );
}

std::string Utility::home_network_gprs( const char *mnc, const char *mcc )
{
   std::string s;

   // '^mnc(\d{3})\.mcc(\d{3})\.gprs'
   s.APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .append( "gprs" );

   return s;
}

std::string Utility::home_network_gprs( const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return home_network_gprs( mnc, mcc );
}

std::string Utility::tai_fqdn( const char *lb, const char *hb, const char *mnc, const char *mcc )
{
   std::string s;

   // '^tac-lb([0-9a-fA-F]{2})\.tac-hb([0-9a-fA-F]{2})\.tac\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.append( "tac-lb" )
    .append( lb )
    .append( ".tac-hb" )
    .append( hb )
    .append( ".tac.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::tai_fqdn( const char *lb, const char *hb, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return tai_fqdn( lb, hb, mnc, mcc );
}

std::string Utility::mme_fqdn( const char *mmec, const char *mmegi, const char *mnc, const char *mcc )
{
   std::string s;

   // '^mmec([0-9a-fA-F]{2})\.mmegi([0-9a-fA-F]{4})\.mme.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.append( "mmec" )
    .append( mmec )
    .append( ".mmegi" )
    .append( mmegi )
    .append( ".mme.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::mme_fqdn( const char *mmec, const char *mmegi, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return mme_fqdn( mmec, mmegi, mnc, mcc );
}

std::string Utility::mme_pool_fqdn( const char *mmegi, const char *mnc, const char *mcc )
{
   std::string s;

   // '^mmegi([0-9a-fA-F]{4})\.mme\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.append( "mmegi" )
    .append( mmegi )
    .append( ".mme.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::mme_pool_fqdn( const char *mmegi, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return mme_pool_fqdn( mmegi, mnc, mcc );
}

std::string Utility::rai_fqdn( const char *rac, const char *lac, const char *mnc, const char *mcc )
{
   std::string s;

   // '^rac([0-9a-fA-F]{4})\.lac([0-9a-fA-F]{4})\.rac\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.append( "rac" )
    .append( rac )
    .append( ".lac" )
    .append( lac )
    .append( ".rac.epc." )
       .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::rai_fqdn( const char *rac, const char *lac, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return rai_fqdn( rac, lac, mnc, mcc );
}

std::string Utility::rnc_fqdn( const char *rnc, const char *mnc, const char *mcc )
{
   std::string s;

   // '^rnc([0-9a-fA-F]{4})\.rnc\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.append( "rnc" )
    .append( rnc )
    .append( ".rnc.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::rnc_fqdn( const char *rnc, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return rnc_fqdn( rnc, mnc, mcc );
}

std::string Utility::sgsn_fqdn( const char *nri, const char *rac, const char *lac, const char *mnc, const char *mcc )
{
   std::string s;

   // '^nri([0-9a-fA-F]{4})\.rac([0-9a-fA-F]{4})\.lac([0-9a-fA-F]{4})\.rac\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.append( "nri" )
    .append( nri )
    .append( ".rac" )
    .append( rac )
    .append( ".lac" )
    .append( lac )
    .append( ".rac.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::sgsn_fqdn( const char *nri, const char *rac, const char *lac, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return sgsn_fqdn( nri, rac, lac, mnc, mcc );
}

std::string Utility::epc_nodes_domain_fqdn( const char *mnc, const char *mcc )
{
   std::string s;

   // '^node\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.append( "node.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::epc_nodes_domain_fqdn( const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return epc_nodes_domain_fqdn( mnc, mcc );
}

std::string Utility::epc_node_fqdn( const char *node, const char *mnc, const char *mcc )
{
   std::string s;

   // '^(.+)\.node\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org$'
   s.append( node )
    .append( ".node.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::epc_node_fqdn( const char *node, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return epc_node_fqdn( node, mnc, mcc );
}

std::string Utility::nonemergency_epdg_oi_fqdn( const char *mnc, const char *mcc )
{
   std::string s;

   // '^epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
   s.append( "epdg.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .append( "pub." )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::nonemergency_epdg_oi_fqdn( const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return nonemergency_epdg_oi_fqdn( mnc, mcc );
}

std::string Utility::nonemergency_epdg_tai_fqdn( const char *lb, const char *hb, const char *mnc, const char *mcc )
{
   std::string s;

   // '^tac-lb([0-9a-fA-F]{2})\.tac-hb([0-9a-fA-F]{2})\.tac\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
   s.append( "tac-lb" )
    .append( lb )
    .append( ".tac-hb" )
    .append( hb )
    .append( ".tac.epdg.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::nonemergency_epdg_tai_fqdn( const char *lb, const char *hb, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return nonemergency_epdg_tai_fqdn( lb, hb, mnc, mcc );
}

std::string Utility::nonemergency_epdg_lac_fqdn( const char *lac, const char *mnc, const char *mcc )
{
   std::string s;

   // '^lac([0-9a-fA-F]{4})\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
   s.append( "lac" )
    .append( lac )
    .append( ".epdg.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .append( "pub." )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::nonemergency_epdg_lac_fqdn( const char *lac, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return nonemergency_epdg_lac_fqdn( lac, mnc, mcc );
}

std::string Utility::nonemergency_epdg_visitedcountry_fqdn( const char *mcc )
{
   std::string s;

   // '^epdg\.epc\.mcc(\d{3})\.visited-country\.pub\.3gppnetworks\.org'
   s.append( "epdg.epc." )
    .APPEND_MCC( mcc )
    .append( "visited-country.pub." )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::nonemergency_epdg_visitedcountry_fqdn( const unsigned char *plmnid )
{
   PARSE_PLMNID_MCC( plmnid );
   return nonemergency_epdg_visitedcountry_fqdn( mcc );
}

std::string Utility::emergency_epdg_oi_fqdn( const char *mnc, const char *mcc )
{
   std::string s;

   // '^sos\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
   s.append( "sos.epdg.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .append( "pub." )
    .APPEND_3GPPNETWORK;

   return s;
}

std::string Utility::emergency_epdg_oi_fqdn( const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return emergency_epdg_oi_fqdn( mnc, mcc );
}

std::string Utility::emergency_epdg_tai_fqdn( const char *lb, const char *hb, const char *mnc, const char *mcc )
{
   std::string s;

   // '^tac-lb([0-9a-fA-F]{2})\.tac-hb([0-9a-fA-F]{2})\.tac\.sos\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org',
   s.append( "tac-lb" )
    .append( lb )
    .append( ".tac-hb" )
    .append( hb )
    .append( ".tac.sos.epdg.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .append( "pub." )
    .APPEND_3GPPNETWORK; 

   return s;
}

std::string Utility::emergency_epdg_tai_fqdn( const char *lb, const char *hb, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return emergency_epdg_tai_fqdn( lb, hb, mnc, mcc );
}

std::string Utility::emergency_epdg_lac_fqdn( const char *lac, const char *mnc, const char *mcc )
{
   std::string s;

   // '^lac([0-9a-fA-F]{4})\.sos\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
   s.append( "lac" )
    .append( lac )
    .append( ".sos.epdg.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .append( "pub." )
    .APPEND_3GPPNETWORK; 

   return s;
}

std::string Utility::emergency_epdg_lac_fqdn( const char *lac, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return emergency_epdg_lac_fqdn( lac, mnc, mcc );
}

std::string Utility::emergency_epdg_visitedcountry_fqdn( const char *mcc )
{
   std::string s;

   // '^sos\.epdg\.epc\.mcc(\d{3})\.visited-country\.pub\.3gppnetworks\.org'
   s.append( "sos.epdg.epc." )
    .APPEND_MCC( mcc )
    .append( "visited-country.pub." )
    .APPEND_3GPPNETWORK; 

   return s;
}

std::string Utility::emergency_epdg_visitedcountry_fqdn( const unsigned char *plmnid )
{
   PARSE_PLMNID_MCC( plmnid );
   return emergency_epdg_visitedcountry_fqdn( mcc );
}

std::string Utility::global_enodeb_id_fqdn( const char *enb, const char *mcc )
{
   std::string s;

   // '^enb([0-9a-fA-F]{4})\.enb\.epc\.mcc(\d{3})\.visited-country\.pub\.3gppnetworks\.org'
   s.append( "enb" )
    .append( enb )
    .append( ".enb.epc." )
    .APPEND_MCC( mcc )
    .append( "visited-country.pub." )
    .APPEND_3GPPNETWORK; 

   return s;
}

std::string Utility::global_enodeb_id_fqdn( const char *enb, const unsigned char *plmnid )
{
   PARSE_PLMNID_MCC( plmnid );
   return global_enodeb_id_fqdn( enb, mcc );
}

std::string Utility::local_homenetwork_fqdn( const char *lhn, const char *mcc )
{
   std::string s;

   // '^lhn(.+)\.lhn\.epc\.mcc(\d{3})\.visited-country\.pub\.3gppnetworks\.org$'
   s.append( "lhn" )
    .append( lhn )
    .append( ".lhn.epc." )
    .APPEND_MCC( mcc )
    .append( "visited-country.pub." )
    .APPEND_3GPPNETWORK; 

   return s;
}

std::string Utility::local_homenetwork_fqdn( const char *lhn, const unsigned char *plmnid )
{
   PARSE_PLMNID_MCC( plmnid );
   return local_homenetwork_fqdn( lhn, mcc );
}

std::string Utility::epc( const char *mnc, const char *mcc )
{
   std::string s;

   // '^epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
   s.append( "epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK; 

   return s;
}

std::string Utility::epc( const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return epc( mnc, mcc );
}

std::string Utility::apn_fqdn( const char *apnoi, const char *mnc, const char *mcc )
{
   std::string s;

   // '(.+)\.apn\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org$'
   s.append( apnoi )
    .append( ".apn.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK; 

   return s;
}

std::string Utility::apn_fqdn( const char *apnoi, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return apn_fqdn( apnoi, mnc, mcc );
}

std::string Utility::apn( const char *apnoi, const char *mnc, const char *mcc )
{
   std::string s;

   // '(.+)\.apn\.mnc(\d{3})\.mcc(\d{3})\.gprs$'} 
   s.append( apnoi )
    .append( ".apn." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .append( "gprs" ); 

   return s;
}

std::string Utility::apn( const char *apnoi, const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return apn( apnoi, mnc, mcc );
}

AppServiceEnum Utility::getAppService( const std::string &s )
{
   return
      s == "x-3gpp-pgw"  ? x_3gpp_pgw  :
      s == "x-3gpp-sgw"  ? x_3gpp_sgw  :
      s == "x-3gpp-ggsn" ? x_3gpp_ggsn :
      s == "x-3gpp-sgsn" ? x_3gpp_sgsn :
      s == "x-3gpp-mme"  ? x_3gpp_mme  :
      s == "x-3gpp-msc"  ? x_3gpp_msc  : x_3gpp_unknown;
}

AppProtocolEnum Utility::getAppProtocol( const std::string &p )
{
   return
      p == "x-gn"        ? x_gn        :
      p == "x-gp"        ? x_gp        :
      p == "x-nq"        ? x_nq        :
      p == "x-nqprime"   ? x_nqprime   :
      p == "x-s10"       ? x_s10       :
      p == "x-s11"       ? x_s11       :
      p == "x-s12"       ? x_s12       :
      p == "x-s1-mme"    ? x_s1_mme    :
      p == "x-s1-u"      ? x_s1_u      :
      p == "x-s16"       ? x_s16       :
      p == "x-s2a-gtp"   ? x_s2a_gtp   :
      p == "x-s2a-mipv4" ? x_s2a_mipv4 :
      p == "x-s2a-pmip"  ? x_s2a_pmip  :
      p == "x-s2b-gtp"   ? x_s2b_gtp   :
      p == "x-s2b-pmip"  ? x_s2b_pmip  :
      p == "x-s2c-dsmip" ? x_s2c_dsmip :
      p == "x-s3"        ? x_s3        :
      p == "x-s4"        ? x_s4        :
      p == "x-s5-gtp"    ? x_s5_gtp    :
      p == "x-s5-pmip"   ? x_s5_pmip   :
      p == "x-s6a"       ? x_s6a       :
      p == "x-s8-gtp"    ? x_s8_gtp    :
      p == "x-s8-pmip"   ? x_s8_pmip   :
      p == "x-sv"        ? x_sv        : x_unknown;
}

const char *Utility::getAppService( AppServiceEnum s )
{
   return
      s == x_3gpp_pgw  ? "x-3gpp-pgw"  :
      s == x_3gpp_sgw  ? "x-3gpp-sgw"  :
      s == x_3gpp_ggsn ? "x-3gpp-ggsn" :
      s == x_3gpp_sgsn ? "x-3gpp-sgsn" :
      s == x_3gpp_mme  ? "x-3gpp-mme"  :
      s == x_3gpp_msc  ? "x-3gpp-msc"  : "x-3gpp-unknown";
}

const char *Utility::getAppProtocol( AppProtocolEnum proto )
{
   return
      proto == x_gn        ? "x-gn"        :
      proto == x_gp        ? "x-gp"        :
      proto == x_nq        ? "x-nq"        :
      proto == x_nqprime   ? "x-nqprime"   :
      proto == x_s10       ? "x-s10"       :
      proto == x_s11       ? "x-s11"       :
      proto == x_s12       ? "x-s12"       :
      proto == x_s1_mme    ? "x-s1-mme"    :
      proto == x_s1_u      ? "x-s1-u"      :
      proto == x_s16       ? "x-s16"       :
      proto == x_s2a_gtp   ? "x-s2a-gtp"   :
      proto == x_s2a_mipv4 ? "x-s2a-mipv4" :
      proto == x_s2a_pmip  ? "x-s2a-pmip"  :
      proto == x_s2b_gtp   ? "x-s2b-gtp"   :
      proto == x_s2b_pmip  ? "x-s2b-pmip"  :
      proto == x_s2c_dsmip ? "x-s2c-dsmip" :
      proto == x_s3        ? "x-s3"        :
      proto == x_s4        ? "x-s4"        :
      proto == x_s5_gtp    ? "x-s5-gtp"    :
      proto == x_s5_pmip   ? "x-s5-pmip"   :
      proto == x_s6a       ? "x-s6a"       :
      proto == x_s8_gtp    ? "x-s8-gtp"    :
      proto == x_s8_pmip   ? "x-s8-pmip"   :
      proto == x_sv        ? "x-sv"        : "x-unknown";
}

AppProtocolEnum Utility::getAppProtocol( PGWAppProtocolEnum proto )
{
   return
      proto == pgw_x_gn        ? x_gn        :
      proto == pgw_x_gp        ? x_gp        :
      proto == pgw_x_s2a_gtp   ? x_s2a_gtp   :
      proto == pgw_x_s2a_mipv4 ? x_s2a_mipv4 :
      proto == pgw_x_s2a_pmip  ? x_s2a_pmip  :
      proto == pgw_x_s2b_gtp   ? x_s2b_gtp   :
      proto == pgw_x_s2b_pmip  ? x_s2b_pmip  :
      proto == pgw_x_s2c_dsmip ? x_s2c_dsmip :
      proto == pgw_x_s5_gtp    ? x_s5_gtp    :
      proto == pgw_x_s5_pmip   ? x_s5_pmip   :
      proto == pgw_x_s8_gtp    ? x_s8_gtp    :
      proto == pgw_x_s8_pmip   ? x_s8_pmip   : x_unknown;
}

AppProtocolEnum Utility::getAppProtocol( SGWAppProtocolEnum proto )
{
   return
      proto == sgw_x_s11      ? x_s11      :
      proto == sgw_x_s12      ? x_s12      :
      proto == sgw_x_s1_u     ? x_s1_u     :
      proto == sgw_x_s2a_pmip ? x_s2a_pmip :
      proto == sgw_x_s2b_pmip ? x_s2b_pmip :
      proto == sgw_x_s4       ? x_s4       :
      proto == sgw_x_s5_gtp   ? x_s5_gtp   :
      proto == sgw_x_s5_pmip  ? x_s5_pmip  :
      proto == sgw_x_s8_gtp   ? x_s8_gtp   :
      proto == sgw_x_s8_pmip  ? x_s8_pmip  : x_unknown;
}

AppProtocolEnum Utility::getAppProtocol( GGSNAppProtocolEnum proto )
{
   return
      proto == ggsn_x_gn ? x_gn :
      proto == ggsn_x_gp ? x_gp : x_unknown;
}

AppProtocolEnum Utility::getAppProtocol( SGSNAppProtocolEnum proto )
{
   return
      proto == sgsn_x_gn      ? x_gn      :
      proto == sgsn_x_gp      ? x_gp      :
      proto == sgsn_x_nqprime ? x_nqprime :
      proto == sgsn_x_s16     ? x_s16     :
      proto == sgsn_x_s3      ? x_s3      :
      proto == sgsn_x_s4      ? x_s4      :
      proto == sgsn_x_sv      ? x_sv      : x_unknown;
}

AppProtocolEnum Utility::getAppProtocol( MMEAppProtocolEnum proto )
{
   return
      proto == mme_x_gn     ? x_gn     :
      proto == mme_x_gp     ? x_gp     :
      proto == mme_x_nq     ? x_nq     :
      proto == mme_x_s10    ? x_s10    :
      proto == mme_x_s11    ? x_s11    :
      proto == mme_x_s1_mme ? x_s1_mme :
      proto == mme_x_s3     ? x_s3     :
      proto == mme_x_s6a    ? x_s6a    :
      proto == mme_x_sv     ? x_sv     : x_unknown;
}

AppProtocolEnum Utility::getAppProtocol( MSCAppProtocolEnum proto )
{
   return
      proto == msc_x_sv ? x_sv : x_unknown;
}

std::string Utility::diameter_fqdn( const char *mnc, const char *mcc )
{
   std::string s;

   // 'diameter\.epc.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org$'
   s.append( "diameter.epc." )
    .APPEND_MNC( mnc )
    .APPEND_MCC( mcc )
    .APPEND_3GPPNETWORK; 

   return s;
}

std::string Utility::diameter_fqdn( const unsigned char *plmnid )
{
   PARSE_PLMNID( plmnid );
   return diameter_fqdn( mnc, mcc );
}

std::string Utility::getDiameterService( DiameterApplicationEnum app, DiameterProtocolEnum protocol )
{
   std::ostringstream buffer; 
   buffer << "aaa+ap" << getDiameterApplication( app ) << ":" << getDiameterProtocol( protocol );
   return buffer.str();
}

uint32_t Utility::getDiameterApplication( DiameterApplicationEnum app )
{
   return
      /*
      +------------------+----------------------------+
      | Tag              | Diameter Application       |
      +------------------+----------------------------+
      | aaa+ap1          | NASREQ [RFC3588]           |
      | aaa+ap2          | Mobile IPv4 [RFC4004]      |
      | aaa+ap3          | Base Accounting [RFC3588]  |
      | aaa+ap4          | Credit Control [RFC4006]   |
      | aaa+ap5          | EAP [RFC4072]              |
      | aaa+ap6          | SIP [RFC4740]              |
      | aaa+ap7          | Mobile IPv6 IKE [RFC5778]  |
      | aaa+ap8          | Mobile IPv6 Auth [RFC5778] |
      | aaa+ap9          | QoS [RFC5866]              |
      | aaa+ap4294967295 | Relay [RFC3588]            |
      +------------------+----------------------------+
      */
      app == dia_app_nasreq           ? 1 :
      app == dia_app_mobile_ipv4      ? 2 :
      app == dia_app_base_accounting  ? 3 :
      app == dia_app_credit_control   ? 4 :
      app == dia_app_eap              ? 5 :
      app == dia_app_sip6             ? 6 :
      app == dia_app_mobile_ipv6_ike  ? 7 :
      app == dia_app_mobile_ipv6_auth ? 8 :
      app == dia_app_qos              ? 9 :
      app == dia_app_relay            ? 4294967295 :
      /*
      +----------------+----------------------+
      | Tag            | Diameter Application |
      +----------------+----------------------+
      | aaa+ap16777250 | 3GPP STa [TS29.273]  |
      | aaa+ap16777251 | 3GPP S6a [TS29.272]  |
      | aaa+ap16777264 | 3GPP SWm [TS29.273]  |
      | aaa+ap16777267 | 3GPP S9 [TS29.215]   |
      +----------------+----------------------+
      */
      app == dia_app_3gpp_sta ? 16777250 :
      app == dia_app_3gpp_s6a ? 16777251 :
      app == dia_app_3gpp_swm ? 16777264 :
      app == dia_app_3gpp_s9  ? 16777267 :
      /*
      +----------------+--------------------------------------------------+
      | Tag            | Diameter Application                             |
      +----------------+--------------------------------------------------+
      | aaa+ap16777281 | WiMAX Network Access Authentication and          |
      |                | Authorization Diameter Application (WNAAADA)     |
      |                | [WiMAX-BASE]                                     |
      | aaa+ap16777282 | WiMAX Network Accounting Diameter Application    |
      |                | (WNADA) [WiMAX-BASE]                             |
      | aaa+ap16777283 | WiMAX MIP4 Diameter Application (WM4DA)          |
      |                | [WiMAX-BASE]                                     |
      | aaa+ap16777284 | WiMAX MIP6 Diameter Application (WM6DA)          |
      |                | [WiMAX-BASE]                                     |
      | aaa+ap16777285 | WiMAX DHCP Diameter Application (WDDA)           |
      |                | [WiMAX-BASE]                                     |
      | aaa+ap16777286 | WiMAX Location Authentication Authorization      |
      |                | Diameter Application (WLAADA) [WiMAX-LBS]        |
      | aaa+ap16777287 | WiMAX Policy and Charging Control R3 Policies    |
      |                | Diameter Application (WiMAX PCC-R3-P)            |
      |                | [WiMAX-PCC]                                      |
      | aaa+ap16777288 | WiMAX Policy and Charging Control R3 Offline     |
      |                | Charging Diameter Application (WiMAX PCC-R3-OFC) |
      |                | [WiMAX-PCC]                                      |
      | aaa+ap16777289 | WiMAX Policy and Charging Control R3 Offline     |
      |                | Charging Prime Diameter Application (WiMAX       |
      |                | PCC-R3-OFC-PRIME) [WiMAX-PCC]                    |
      | aaa+ap16777290 | WiMAX Policy and Charging Control R3 Online      |
      |                | Charging Diameter Application (WiMAX PCC-R3-OC)  |
      |                | [WiMAX-PCC]                                      |
      +----------------+--------------------------------------------------+
      */
      app == dia_app_wimax_wnaaada          ? 16777281 :
      app == dia_app_wimax_wnada            ? 16777282 :
      app == dia_app_wimax_wm4da            ? 16777283 :
      app == dia_app_wimax_wm6da            ? 16777284 :
      app == dia_app_wimax_wdda             ? 16777285 :
      app == dia_app_wimax_wlaada           ? 16777286 :
      app == dia_app_wimax_pcc_r3_p         ? 16777287 :
      app == dia_app_wimax_pcc_r3_ofc       ? 16777288 :
      app == dia_app_wimax_pcc_r3_ofc_prime ? 16777289 :
      app == dia_app_wimax_pcc_r3_oc        ? 16777290 : 0;
}

const char *Utility::getDiameterProtocol( DiameterProtocolEnum protocol )
{
   return
      protocol == dia_protocol_tcp ?     "diameter.tcp"     :
      protocol == dia_protocol_sctp ?    "diameter.sctp"    :
      protocol == dia_protocol_tls_tcp ? "diameter.tls.tcp" : "unknown";
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

CanonicalNodeName::CanonicalNodeName()
   : m_topon( false )
{
}

CanonicalNodeName::CanonicalNodeName( const std::string &n )
   : m_topon( false )
{
   setName( n );
}

void CanonicalNodeName::setName( const std::string &n )
{
   // reset the object
   clear();
   m_topon = false;
   m_name = "";
   
   // parse the name into labels
   std::string arg;
   std::istringstream ss( n );

   // get the first label
   std::getline( ss, arg, '.' );

   // check for "topon"
   m_topon = arg == "topon";

   // check for not "topoff"
   if ( !m_topon && arg != "topoff" )
   {
      // set the name to what was passed in and stop processing
      m_name = n;
      return;
   }

   // skip the interface
   std::getline( ss, arg, '.' );

   // save the canonical name
   if ( !std::getline( ss, arg ) )
      return;  // no need to continue if there is no more data
   m_name = arg;

   // only get the labels of topon is specified
   if ( m_topon )
   {
      // set ss to the conanical name to split
      ss.str( m_name );
      ss.clear();

      // add the labels to the list
      while ( std::getline( ss, arg, '.' ) )
         push_back( arg );

      // reverse the list for use in topological matching
      reverse();
   }
}

int CanonicalNodeName::topologicalCompare( const CanonicalNodeName &right )
{
   int matchingLabels = 0;
   CanonicalNodeName::const_iterator itl = begin();
   CanonicalNodeName::const_iterator itr = right.begin();
   
   while ( itl != end() && itr != right.end() )
   {
      if ( *itl != *itr )
         break;
      ++matchingLabels;
      ++itl;
      ++itr;
   }

   return matchingLabels;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

NodeSelectorResultList &NodeSelector::process()
{
   std::list<AppProtocolEnum> supportedProtocols;

   // perform dns query
   bool cacheHit = false;
   m_query = CachedDNS::Cache::getInstance().query( ns_t_naptr, m_domain, cacheHit );

   // evaluate each answer to see if it matches the service/protocol requirements
   for (std::list<CachedDNS::ResourceRecord*>::const_iterator rrit = m_query->getAnswers().begin();
        rrit != m_query->getAnswers().end();
        ++rrit )
   {
      CachedDNS::RRecordNAPTR* naptr = (CachedDNS::RRecordNAPTR*)*rrit;

      // parse the service field
      AppService service;

      service.parse( naptr->getService() );

      // check for service match
      if ( service.getService() == m_desiredService )
      {
         NodeSelectorResult *nsr = new NodeSelectorResult();

         nsr->setHostname( naptr->getReplacement() );
         nsr->setOrder( naptr->getOrder() );
         nsr->setPreference( naptr->getPreference() );

         // identify all of the desired protocols supported by the service 
         for (AppProtocolList::const_iterator dpit = m_desiredProtocols.begin();
              dpit != m_desiredProtocols.end();
              ++dpit)
         {
            // is the protocol supported by the naptr service
            AppProtocol *sp = service.findProtocol( (*dpit)->getProtocol() );
            if ( sp )
            {
               AppProtocol *nsrap = new AppProtocol();

               nsrap->setProtocol( sp->getProtocol() );

               // the protocol has to support all of the desired usage types
               if ( sp->getUsageTypes().empty() )
               {
                  // the naptr app protocol is not ue specific which means that it supports all usage types
                  nsr->addSupportedProtocol( nsrap );
               }
               else
               {
                  // the naptr app protocol only supports specific usage types
                  // need to check to see if one of the desired usage types matches
                  for ( UsageTypeList::const_iterator utit = m_desiredUsageTypes.begin();
                        utit != m_desiredUsageTypes.end();
                        ++utit )
                  {
                     if ( sp->findUsageType( *utit ) )
                        nsrap->addUsageType( *utit );
                  }

                  if ( !nsrap->getUsageTypes().empty() )
                  {
                     // at least 1 usage type matched, so consider the protocol a match
                     nsr->addSupportedProtocol( nsrap );
                  }
                  else
                  {
                     // the protocol was not a match
                     delete nsrap;
                  }
               }
            }
         }

         if ( !nsr->getSupportedProtocols().empty() )
         {
            // iterate through the dns query additonal records adding the ip addresses for the host to the result
            for ( CachedDNS::ResourceRecordList::const_iterator it = m_query->getAdditional().begin();
                  it != m_query->getAdditional().end();
                  ++it )
            {
               if ( nsr->getHostname() == (*it)->getName() )
               {
                  switch ( (*it)->getType() )
                  {
                     case ns_t_a:
                     {
                        nsr->addIPv4Host( ((CachedDNS::RRecordA*)*it)->getAddressString() );
                        break;
                     }
                     case ns_t_aaaa:
                     {
                        nsr->addIPv6Host( ((CachedDNS::RRecordAAAA*)*it)->getAddressString() );
                        break;
                     }
                     default:
                     {
                        break;
                     }
                  }
               }
            }

            // shuffle the ip addresses
            nsr->getIPv4Hosts().shuffle();
            nsr->getIPv6Hosts().shuffle();

            // add the nsr pointer to the list since at least 1 protocol matched
            m_results.push_back( nsr );
         }
         else
         {
            // delete the nsr pointer since no protocols matched
            delete nsr;
         }
      }
   }

   // sort the naptr list
   m_results.sort( NodeSelectorResultList::sort_compare );
      
   return m_results;
}

NodeSelector::NodeSelector()
{
   m_query = NULL;
}

NodeSelector::~NodeSelector()
{
   // do not delete m_query, it will be deleted by CachedDNS::~Cache

   while ( !m_desiredProtocols.empty() )
   {
      AppProtocol *ap = *m_desiredProtocols.begin();
      m_desiredProtocols.pop_front();
      delete ap;
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool NodeSelectorResultList::sort_compare( NodeSelectorResult*& first, NodeSelectorResult*& second )
{
   if ( first->getOrder() < second->getOrder() )
      return true;

   if ( first->getOrder() > second->getOrder() )
      return false;

   return first->getPreference() < second->getPreference();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void AppService::parse( const std::string &rs )
{
   m_rawService = rs;
   
   std::string arg;
   std::istringstream ss( m_rawService );

   if ( !std::getline(ss, arg, ':') )
      return;

   m_service = Utility::getAppService( arg );
   
   while ( getline( ss, arg, ':' ) )
   {
      AppProtocol *ap = new AppProtocol();
      ap->parse( arg );
      m_protocols.push_back( ap );
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void AppProtocol::parse( const std::string &rp )
{
   m_rawProtocol = rp;

   std::string arg;
   std::istringstream ss( m_rawProtocol );

   if ( !std::getline( ss, arg, '+' ) )
      return;

   m_protocol = Utility::getAppProtocol( arg );

   if ( std::getline( ss, arg, '-' ) && arg == "ue" )
   {
      while ( std::getline( ss, arg, '.' ) )
         m_usageTypes.push_back( atoi( arg.c_str() ) );
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ColocatedCandidate::ColocatedCandidate( NodeSelectorResult &candidate1, NodeSelectorResult &candidate2 )
   : m_candidate1( candidate1 ),
     m_candidate2( candidate2 )
{
   m_cnn1.setName( m_candidate1.getHostname() );
   m_cnn2.setName( m_candidate2.getHostname() );

   m_pairtype =
      m_cnn1.getName() == m_cnn2.getName() ? ptColocated :
      m_cnn1.getTopon() && m_cnn2.getTopon() ? ptTopologicalDistance : ptDNSPriority;

   m_topologicalMatches =
      m_pairtype == ptTopologicalDistance ?  m_cnn1.topologicalCompare( m_cnn2 ) : 0;
}

ColocatedCandidateList::ColocatedCandidateList( NodeSelectorResultList &nodelist1, NodeSelectorResultList &nodelist2 )
   : m_nodelist1( nodelist1 ),
     m_nodelist2( nodelist2 )
{
   // create all of the colocated candidate pairs (which also classifies them)
   for ( NodeSelectorResultList::const_iterator it1 = m_nodelist1.begin();
         it1 != m_nodelist1.end();
         ++it1 )
   {
      for( NodeSelectorResultList::const_iterator it2 = m_nodelist2.begin();
           it2 != m_nodelist2.end();
           it2++ )
      {
         ColocatedCandidate *cc = new ColocatedCandidate( *(*it1), *(*it2) );
         push_back( cc );
      }
   }

   sort( ColocatedCandidateList::sort_compare );
}

ColocatedCandidateList::~ColocatedCandidateList()
{
   while ( !empty() )
   {
      ColocatedCandidate *cc = *begin();
      erase( begin() );
      delete cc;
   }
}

bool ColocatedCandidateList::sort_compare( ColocatedCandidate*& first, ColocatedCandidate*& second )
{
   // sort by pairtype ascending
   if ( (int)first->getPairType() < (int)second->getPairType() )
      return true;
   if ( (int)first->getPairType() > (int)second->getPairType() )
      return false;

   // sort by the candidate1 order
   if ( first->getCandidate1().getOrder() < second->getCandidate1().getOrder() )
      return true;
   if ( first->getCandidate1().getOrder() > second->getCandidate1().getOrder() )
      return false;

   // sort by the candidate1 preference
   return first->getCandidate1().getPreference() < second->getCandidate1().getPreference();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DiameterSelector::DiameterSelector()
   : m_application( dia_app_unknown ),
     m_protocol( dia_protocol_unknown ),
     m_query( NULL )
{
}

DiameterNaptrList &DiameterSelector::process()
{
   // validate m_applciation
   if ( m_application == dia_app_unknown )
      return m_results;

   // validate m_protocol
   if ( m_protocol == dia_protocol_unknown )
      return m_results;

   // validate realm
   if ( m_realm.empty() )
      return m_results;

   // construct the service string
   std::string service( Utility::getDiameterService( m_application, m_protocol ) );

   // perform dns query
   bool cacheHit = false;
   m_query = CachedDNS::Cache::getInstance().query( ns_t_naptr, m_realm, cacheHit );

   // evaluate each answer to see if it matches the service/protocol requirements
   for ( std::list<CachedDNS::ResourceRecord*>::const_iterator rrit = m_query->getAnswers().begin();
         rrit != m_query->getAnswers().end();
         ++rrit )
   {
      CachedDNS::RRecordNAPTR* naptr = (CachedDNS::RRecordNAPTR*)*rrit;
      
      // does the naptr service field match the app/protocol that we are looking for
      if ( naptr->getService() == service )
      {
         DiameterNaptr *n = NULL;

         if ( naptr->getFlags() == "a" )
            n = new DiameterNaptrA();
         else if ( naptr->getFlags() == "s" )
            n = new DiameterNaptrS();

         // check for valid "flags" value
         if ( !n )
            continue;

         n->setOrder( naptr->getOrder() );
         n->setPreference( naptr->getPreference() );
         n->setService( naptr->getService() );
         n->setReplacement( naptr->getReplacement() );

         if ( n->getType() == dnt_hostname )
         {
            DiameterNaptrA *a = (DiameterNaptrA*)n;

            // set the host name
            a->getHost().setName( n->getReplacement() );

            // add all of the A/AAAA records for the host
            for ( CachedDNS::ResourceRecordList::const_iterator rr = m_query->getAdditional().begin();
                  rr != m_query->getAdditional().end();
                  ++rr)
            {
               if ( (*rr)->getName() == a->getHost().getName() )
               {
                  switch ( (*rr)->getType() )
                  {
                     case ns_t_a:
                     {
                        a->getHost().addIPv4Address( ((CachedDNS::RRecordA*)*rr)->getAddressString() );
                        break;
                     }
                     case ns_t_aaaa:
                     {
                        a->getHost().addIPv6Address( ((CachedDNS::RRecordAAAA*)*rr)->getAddressString() );
                        break;
                     }
                     default:
                     {
                        break;
                     }
                  }
               }
            }

            // randomize the ip addresses
            a->getHost().getIPv4Addresses().shuffle();
            a->getHost().getIPv6Addresses().shuffle();
         }
         else if ( n->getType() == dnt_service )
         {
            DiameterNaptrS *s = (DiameterNaptrS*)n;

            // add all of the matching SRV records
            for ( CachedDNS::ResourceRecordList::const_iterator rr = m_query->getAdditional().begin();
                  rr != m_query->getAdditional().end();
                  ++rr)
            {
               if ( (*rr)->getType() == ns_t_srv &&  (*rr)->getName() == s->getReplacement() )
               {
                  CachedDNS::RRecordSRV *rrs = (CachedDNS::RRecordSRV*)*rr;

                  DiameterSrv *ds = new DiameterSrv();

                  // set the SRV properties
                  ds->setPriority( rrs->getPriority() );
                  ds->setWeight( rrs->getWeight() );
                  ds->setPort( rrs->getPort() );

                  // loop through the additional records adding the hostname entries that match the SRV hostname
                  for ( CachedDNS::ResourceRecordList::const_iterator rr2 = m_query->getAdditional().begin();
                        rr2 != m_query->getAdditional().end();
                        ++rr2 )
                  {
                     if ( (*rr2)->getName() == rrs->getTarget() )
                     {
                        ds->getHost().setName( (*rr2)->getName() );
                        switch ( (*rr2)->getType() )
                        {
                           case ns_t_a:
                           {
                              ds->getHost().addIPv4Address( ((CachedDNS::RRecordA*)*rr2)->getAddressString() );
                              break;
                           }
                           case ns_t_aaaa:
                           {
                              ds->getHost().addIPv6Address( ((CachedDNS::RRecordAAAA*)*rr2)->getAddressString() );
                              break;
                           }
                           default:
                           {
                              break;
                           }
                        }
                     }
                  }

                  // randomize the ip addresses
                  ds->getHost().getIPv4Addresses().shuffle();
                  ds->getHost().getIPv6Addresses().shuffle();

                  // add to the SRV collection
                  s->getSrvs().push_back( ds );
               }
            }

            // sort the srv records
            s->getSrvs().sort_vector();
         }
         else
         {
            delete n;
            continue;
         }

         // add the record to the results
         m_results.push_back( n );
      }
   }
   

   return m_results;
}

void DiameterSrvVector::sort_vector()
{
   // sort the SRV records according to RFC 2782

   // sort the list ascending on priority and weight
   std::sort( this->begin(), this->end(), DiameterSrvVector::sort_compare );

   int priority = -1;
   int first = -1;
   int last = -1;

   srand( time(NULL) );              /* initialize random seed: */

   for ( uint32_t i = 0; i <= size(); i++ )
   {
      if ( ( i == size() || at(i)->getPriority() != priority ) && priority != -1)
      {
         if ( priority != -1 )
         {
            // the sorted vector contains indices in the specified range in sorted order
            std::vector<int> sorted;

            // execute selection algo for each item in range
            for ( int j = 0; j < last - first + 1; j++ )
            {
               int runningTotal = 0;
               std::vector<int> unsorted;
               std::vector<int> runningTotals;

               // generate the running totals
               for ( int k = first; k <= last; k++ )
               {
                  // skip if entry is already in sorted list
                  if ( std::find( sorted.begin(), sorted.end(), k ) == sorted.end() )
                  {
                     runningTotal += at(k)->getWeight();
                     unsorted.push_back( k );
                     runningTotals.push_back( runningTotal );
                  }
               }

               // generate value between 0 and runningTotal
               int val = rand() & runningTotal;

               // identify the entry and place it in the sorted list
               for ( uint32_t l = 0; l < runningTotals.size(); l++ )
               {
                  if ( runningTotals[l] >= val )
                  {
                     sorted.push_back( unsorted[l] );
                     break;
                  }
               }
            }

            // save the pointers in sorted order
            DiameterSrvVector newsorted;
            for ( uint32_t j = 0; j < sorted.size(); j++ )
               newsorted.push_back( at( sorted[j] ) );

            // update the original array with the sorted values
            for ( int j = first; j <= last; j++ )
               (*this)[ j ] = newsorted[ j - first ];
         }

         if ( i < size() )
         {
            // init for the next priority
            priority = at(i)->getPriority();
            first = i;
            last = i;
         }
      }
      else
      {
         last = i;
      }
   }
}

bool DiameterSrvVector::sort_compare( DiameterSrv* first, DiameterSrv* second )
{
   return
      first->getPriority() < second->getPriority() ? true :
      first->getPriority() > second->getPriority() ? false : first->getWeight() < second->getWeight();
}
