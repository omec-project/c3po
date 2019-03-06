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

#include <iostream>

#include <pistache/endpoint.h>
#include <pistache/net.h>

#include "options.h"
#include "fdhss.h"
#include "hss_config.h"
#include "s6t_impl.h"
#include "s6as6d_impl.h"
#include "s6c_impl.h"
#include "dataaccess.h"
#include "common_def.h"
#include "msg_event.h"



#include "resthandler.h"

#include "util.h"

extern "C" {
   #include "hss_config.h"
   #include "aucpp.h"
}

HSSWorkerQueue::HSSWorkerQueue()
{
}

HSSWorkerQueue::~HSSWorkerQueue()
{
}

void HSSWorkerQueue::addProcessor(QueueProcessor *processor)
{
   addEntry(processor);
}

void HSSWorkerQueue::startProcessor()
{
   QueueProcessor *processor = (QueueProcessor*)startMessage();
   if (processor)
      processor->triggerNextPhase();
}

void HSSWorkerQueue::finishProcessor()
{
   finishMessage();
}

////////////////////////////////////////////////////////////////////////////////

FDHss::FDHss() :
      m_s6tapp (NULL),
      m_s6aapp (NULL),
      m_s6capp (NULL)
{

}

FDHss::~FDHss() {
   if(NULL != m_s6tapp){
      delete m_s6tapp;
      m_s6tapp = NULL;
   }
   if(NULL != m_s6aapp){
      delete m_s6aapp;
      m_s6aapp = NULL;
   }
   if(NULL != m_s6capp){
      delete m_s6capp;
      m_s6capp = NULL;
   }
}

bool FDHss::isMmeValid(std::string &mmehost) {
   int32_t mmeid(0);
   return m_dbobj.getMmeIdFromHost(mmehost, mmeid, NULL, NULL);
}

int FDHss::s6a_peer_validate ( struct peer_info *info, int *auth, int (**cb2) (struct peer_info *)){

    if (info == NULL) {
      return EINVAL;
    }

    std::string mme_host(info->pi_diamid);

    if (! fdHss.isMmeValid(mme_host) ) {
      /*
       * The MME has not been found in list of known peers -> reject it
       */
      *auth = -1;
      printf ( "Rejecting %s: either db has no knowledge of this peer " "or sql query failed\n", info->pi_diamid);
    } else {
      *auth = 1;
      /*
       * For now we don't use security
       */
      info->config.pic_flags.sec = PI_SEC_NONE;
      info->config.pic_flags.persist = PI_PRST_NONE;
      printf ( "Accepting %s peer\n", info->pi_diamid);
    }
    return 0;
}

bool FDHss::initdb(hss_config_t * hss_config_p){
   m_dbobj.connect(hss_config_p->cassandra_server);
   return true;
}

bool FDHss::init(hss_config_t * hss_config_p){
   try
   {
      // set the diameter configuration file
      m_diameter.setConfigFile( hss_config_p->freediameter_config );

      // initialize diameter
      if ( !m_diameter.init() ){
         return false;
      }

      std::cout << "Connecting to cassandra host: " << hss_config_p->cassandra_server << std::endl;
      //init the casssandra object with the parsed object

      m_s6tapp = new s6t::Application(m_dbobj);
      m_s6aapp = new s6as6d::Application(m_dbobj);
      m_s6capp = new s6c::Application(m_dbobj);

      // advertise support for the accounting application
      FDDictionaryEntryVendor vnd3gpp( m_s6tapp->getDict().app() );
      m_diameter.advertiseSupport( m_s6tapp->getDict().app(), vnd3gpp, 1, 0 );
      m_diameter.advertiseSupport( m_s6aapp->getDict().app(), vnd3gpp, 1, 0 );
      m_diameter.advertiseSupport( m_s6capp->getDict().app(), vnd3gpp, 1, 0 );

      fd_peer_validate_register (s6a_peer_validate);

      //TODO get the list of peers from the database
      char *mme = std::getenv("MME_IDENTITY");
      FDPeer *peer = new FDPeer( mme ? mme : (char*)"mme.localdomain" );
      m_mme_peers.push_back( peer );

      if(!m_diameter.start()){
         return false;
      }
   }
   catch ( FDException &e )
   {
      return false;
   }
   catch ( DAException& e)
   {
      std::cout << "error db conn: " << e.what() << std::endl;
      return false;
   }
   catch (...)
   {
      std::cout << "error initializing FDHss"  << std::endl;
      return false;
   }

   //
   // start the chronos timer callback handler
   //
   try
   {
      Pistache::Address addr( Pistache::Ipv4::any(), Pistache::Port(Options::getrestport()) );
      auto opts = Pistache::Http::Endpoint::options()
         .threads(1)
         .flags( Pistache::Tcp::Options::ReuseAddr );
//      .flags( Pistache::Tcp::Options::InstallSignalHandler | Pistache::Tcp::Options::ReuseAddr );

      m_endpoint = new Pistache::Http::Endpoint( addr );
      m_endpoint->init( opts );
      m_endpoint->setHandler( Pistache::Http::make_handler<RestHandler>() );
      m_endpoint->serveThreaded();

      Pistache::Address addrOss( Pistache::Ipv4::any(), Pistache::Port(Options::getossport()) );

      m_ossendpoint = new OssEndpoint<Logger>(addrOss, &StatsHss::singleton(), &Logger::singleton().audit(), &Logger::singleton(), Options::getossfile());
      m_ossendpoint->init();
      m_ossendpoint->start();

      Logger::system().startup( "Started REST server on port [%i]",   Options::getrestport());
      Logger::system().startup( "Started OSS  server on port [%i]",   Options::getossport());
   }
   catch ( std::runtime_error &e )
   {
      std::cout << "Exception starting REST server - " << e.what() << std::endl;
      return false;
   }

   HookEvent::init(&StatsHss::singleton(), m_s6tapp, m_s6aapp, m_s6capp);

   //
   // set the ULR queue concurrent value
   //
   m_workerqueue.setConcurrent(Options::getconcurrent());

   //
   // starts the stats
   //
   StatsHss::singleton().setInterval(Options::statsFrequency());
   StatsHss::singleton().init( NULL );

   return true;
}

void FDHss::updateOpcKeys(const uint8_t opP[16])
{
   m_dbobj.checkOpcKeys(opP);
}

void FDHss::shutdown()
{
   if ( m_endpoint )
   {
      std::cout << "REST server on port [" << Options::getrestport() << "] shutdown" << std::endl;
      m_endpoint->shutdown();
      delete m_endpoint;
      m_endpoint = NULL;
   }

   if ( m_ossendpoint ){
      m_ossendpoint->shutdown();
      delete m_ossendpoint;
      m_ossendpoint = NULL;
   }

   m_diameter.uninit( false );

   if ( StatsHss::singleton().isRunning() ){
      StatsHss::singleton().quit();
   }
}

void FDHss::waitForShutdown()
{
   m_diameter.waitForShutdown();
   StatsHss::singleton().join();
}

int FDHss::sendINSDRreq(s6t::MonitoringEventConfigurationExtractorList &cir_monevtcfg, std::string& imsi,
                        FDMessageRequest *cir_req, EvenStatusMap *evt_map, RIRBuilder * rir_builder){

   return m_s6aapp->sendINSDRreq(cir_monevtcfg, imsi, cir_req, evt_map, rir_builder);

}

void FDHss::buildCfgStatusAvp(FDAvp &mon_evt_cfg_status, MonitoringConfEventStatus& status)
{
   mon_evt_cfg_status.add(m_s6tapp->getDict().avpScefId(), status.scef_id);
   mon_evt_cfg_status.add(m_s6tapp->getDict().avpScefReferenceId(), status.scef_ref_id);

   FDAvp serv_report( m_s6tapp->getDict().avpServiceReport() );
   FDAvp serv_result( m_s6tapp->getDict().avpServiceResult() );

   serv_result.add(m_s6tapp->getDict().avpServiceResultCode(), status.result);

   if(status.result != DIAMETER_SUCCESS){
      serv_result.add(m_s6tapp->getDict().avpVendorId(), VENDOR_3GPP);
   }
   serv_report.add(serv_result);
   mon_evt_cfg_status.add(serv_report);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define VALID_HEX(c) ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define TO_HEX(c) (c >= '0' && c <= '9' ? (c - '0') : (c - (c >= 'A' && c <= 'F' ? 'A' : 'a') + 10))

static bool parsehex(const char *src, unsigned char *dst, int len)
{
   int srclen = strlen(src);

   if (len != -1 && srclen != len)
   {
      std::cout << "Expected " << len << " hex characters, received " << srclen << " hex characters [" << src << "]" << std::endl;
      return false;
   }

   if (srclen % 2 != 0)
   {
      std::cout << "Expected an even number of hex characters, received an odd number for [" << src << "]" << std::endl;
      return false;
   }

   for (int i=0; src[i]; i+=2)
   {
      if (!VALID_HEX(src[i]))
      {
         std::cout << "Invalid hex character at offset " << i << " character [" << src[i] << "]" << std::endl;
         return false;
      }
      if (!VALID_HEX(src[i+1]))
      {
         std::cout << "Invalid hex character at offset " << i+1 << " character [" << src[i+1] << "]" << std::endl;
         return false;
      }

      dst[i/2] = TO_HEX(src[i]) << 4 | TO_HEX(src[i+1]);
   }

   return true;
}

bool FDHss::synchFix()
{
   bool       result = true;
   DAImsiSec  imsisec;
   uint8_t   *sqn = NULL;
   uint8_t    rand[RAND_LENGTH];
   uint8_t    auts[31] = {0};

   if (!parsehex(Options::getsynchauts().c_str(), auts, -1))
      return false;

   if (!m_dbobj.getImsiSec(Options::getsynchimsi(), imsisec, NULL, NULL))
      return false;

   sqn = sqn_ms_derive_cpp (imsisec.opc, imsisec.key, auts, imsisec.rand);

   if (sqn != NULL)
   {
     //We succeeded to verify SQN_MS...
     //Pick a new RAND and store SQN_MS + RAND in the HSS
     generate_random_cpp(rand, RAND_LENGTH);

     SqnU64Union eu;
     SQN_TO_U64(sqn, eu);

     eu.u64 += 32;
     U64_TO_SQN(eu, sqn);

     result = m_dbobj.updateRandSqn(Options::getsynchimsi(), rand, sqn, false, NULL, NULL);

     free (sqn);
   }
   else
   {
      std::cout << "Unable to extract SQN from AUTS" << std::endl;
      result = false;
   }

   return result;
}

void FDHss::sendRIR_ChangeImsiImeiSvAssn(ImsiImeiData &data)
{
   DAEventList evt_list;
   m_dbobj.getEventsFromImsi(data.imsi, evt_list );

   for( DAEventList::iterator it_evt = evt_list.begin() ; it_evt != evt_list.end(); ++ it_evt )
   {
      if ( (*it_evt)->monitoring_type == CHANGE_IMSI_IMEI_SV_ASSN )
      {
         uint8_t msisdn[5];

         //Build the RIR for each scef to be notified
         s6t::REIRreq *s = new s6t::REIRreq( *fdHss.gets6tApp() );
         s->add( fdHss.gets6tApp()->getDict().avpSessionId(), s->getSessionId() );
         s->add( fdHss.gets6tApp()->getDict().avpAuthSessionState(), 1 );
         s->addOrigin();

         s->add ( fdHss.gets6tApp()->getDict().avpDestinationHost() , (*it_evt)->scef_id );

         size_t afound = (*it_evt)->scef_id.find(".");
         if(afound != std::string::npos)
         {
            s->add ( fdHss.gets6tApp()->getDict().avpDestinationRealm() , (*it_evt)->scef_id.substr(afound + 1) );
         }

         FDAvp user_identifier( fdHss.gets6tApp()->getDict().avpUserIdentifier() );

         user_identifier.add( fdHss.gets6tApp()->getDict().avpUserName(), data.imsi );
         std::string smsisdn = std::to_string(data.msisdn);
         FDUtility::str2tbcd( smsisdn.c_str(), msisdn, sizeof(msisdn) );
         user_identifier.add( fdHss.gets6tApp()->getDict().avpMsisdn(), msisdn, sizeof(msisdn) );
         s->add(user_identifier);

         FDAvp monitoring_event_report( fdHss.gets6tApp()->getDict().avpMonitoringEventReport() );
         monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpScefId(), (*it_evt)->scef_id);
         monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpScefReferenceId(), (*it_evt)->scef_ref_id);
         monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpMonitoringType(), CHANGE_IMSI_IMEI_SV_ASSN);
         s->add(monitoring_event_report);

s->dump();

         try
         {
              if ( s )
              {
                   s->send();
              }
         }
         catch ( FDException &ex )
         {
              std::cout << SUtility::currentTime() << " - EXCEPTION sending - " << ex.what() << std::endl;
              delete s;
              s = NULL;
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

MonitoringConfEventStatus::MonitoringConfEventStatus(bool is_remove):
            scef_ref_id(0),
            max_nb_reports(0),
            max_nb_reports_set(false),
            mon_duration_set(false),
            result(0),
            is_remove(is_remove),
            type_set(false),
            is_long_term(false)

{
}

MonitoringConfEventStatus::MonitoringConfEventStatus() :
            scef_ref_id(0),
            max_nb_reports(0),
            max_nb_reports_set(false),
            mon_duration_set(false),
            result(0),
            is_remove(false),
            type_set(false),
            is_long_term(false)
{
}

bool MonitoringConfEventStatus::isLongTermEvt(){

   if(type_set){
      return is_long_term;
   }
   else{
      if( (max_nb_reports_set && max_nb_reports < 2) || (!max_nb_reports_set && !mon_duration_set ) ){
         return false;
      }
      return true;
   }
}

void MonitoringConfEventStatus::forceLongTerm(bool is_long_tem){
   is_long_term = is_long_tem;
   type_set = true;
}

///////////////////
/// RIRBUILDER
///////////////////
RIRBuilder::RIRBuilder(int nb_ida_proc, EvenStatusMap * hss_insert_status,
                       std::string &destination_host, std::string &destination_realm)
: m_interval(0),
  m_nb_ida_proc( nb_ida_proc ),
  m_destination_host( destination_host ),
  m_destination_realm ( destination_realm ),
  m_hss_insert_status( hss_insert_status ),
  m_ciasent(false)
{

}

RIRBuilder::~RIRBuilder(){
}

void RIRBuilder::onInit(){
   m_idletimer.setInterval( m_interval );
   m_idletimer.setOneShot( false );
   initTimer( m_idletimer );
   m_idletimer.start();
}
void RIRBuilder::onQuit(){
}

void RIRBuilder::onTimer( SEventThread::Timer &t )
{
   if ( t.getId() == m_idletimer.getId() )
   {
      postMessage( GUARD_TIMEOUT );
   }
}

void RIRBuilder::sendRIR()
{
   //build the RIR
   s6t::REIRreq *s = new s6t::REIRreq( *fdHss.gets6tApp() );

   s->add( fdHss.gets6tApp()->getDict().avpSessionId(), s->getSessionId() );
   s->add( fdHss.gets6tApp()->getDict().avpAuthSessionState(), 1 );
   s->addOrigin();
   s->add ( fdHss.gets6tApp()->getDict().avpDestinationHost() , m_destination_host );
   s->add ( fdHss.gets6tApp()->getDict().avpDestinationRealm() , m_destination_realm );
   s->add( fdHss.gets6tApp()->getDict().avpResultCode(), ER_DIAMETER_SUCCESS);

   for( EventImsiStatus::iterator iter_imsist =  imsi_status_map.begin() ;
        iter_imsist != imsi_status_map.end(); ++iter_imsist)
   {
      //std::cout << "imsi_status_map->first.first" << iter_imsist->first.first << "imsi_status_map->first.second" << iter_imsist->first.second << std::endl;

      FDAvp group_monitoring_event_report( fdHss.gets6tApp()->getDict().avpGroupMonitoringEventReport() );

      group_monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpScefId(), iter_imsist->first.first);
      group_monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpScefReferenceId(), iter_imsist->first.second);

      for(std::vector<ImsiStatus>::iterator iter_imsi = iter_imsist->second.begin();
            iter_imsi != iter_imsist->second.end();
            ++iter_imsi)
      {
         FDAvp group_monitoring_event_item( fdHss.gets6tApp()->getDict().avpGroupMonitoringEventReportItem() );

         {
            FDAvp user_identifier( fdHss.gets6tApp()->getDict().avpUserIdentifier() );
            uint8_t        msisdn[5];
            FDUtility::str2tbcd( iter_imsi->m_msisdn.c_str(), msisdn, 5 );
            user_identifier.add ( fdHss.gets6tApp()->getDict().avpMsisdn(),  msisdn, 5 );

            group_monitoring_event_item.add( user_identifier );
         }

         {
            FDAvp serv_report( fdHss.gets6tApp()->getDict().avpServiceReport() );
            FDAvp serv_result( fdHss.gets6tApp()->getDict().avpServiceResult() );
            serv_result.add( fdHss.gets6tApp()->getDict().avpServiceResultCode(), iter_imsi->m_status.result );

            if(iter_imsi->m_status.result != DIAMETER_SUCCESS){
               serv_result.add( fdHss.gets6tApp()->getDict().avpVendorId(),  VENDOR_3GPP );
            }

            serv_report.add(serv_result);
            group_monitoring_event_item.add(serv_report);
         }

         if(iter_imsi->m_reachability == IMSI_NOT_ACTIVE){
            group_monitoring_event_item.add(fdHss.gets6tApp()->getDict().avpS6tHssCause(), ABSENT_SUBSCRIBER);

         }
         group_monitoring_event_report.add(group_monitoring_event_item);
      }

      s->add( group_monitoring_event_report );
   }

s->dump();

   s->send();
}

void RIRBuilder::dispatch( SEventThreadMessage &msg )
{
   if ( msg.getId() == GUARD_TIMEOUT )
   {
      sendRIR();
   }
   else if ( msg.getId() == HANDLE_MME_RESPONSE )
   {
      HandleMmeResponseEvtMsg &mme_response_msg = (HandleMmeResponseEvtMsg&)msg;

      --m_nb_ida_proc;


      ////////////////////////////////////////////////
      ////////////////////////////////////////////////
      // m_hss_insert_status contains the global result
      // of db operations for long term events see s6t_impl.cpp
      //
      // mme_response_msg.m_mme_response contains the result
      // of mme operations for one shot events for a given
      // imsi
      //
      // We need to build a structure grouped by imsi containing
      // for each imsi: the long term db operations result (coming from
      // m_hss_insert_status) and the one shot mme operations results
      // coming from mme_response_msg.m_mme_response
      ////////////////////////////////////////////////
      ////////////////////////////////////////////////

      for(EvenStatusMap::iterator it_hss_result = m_hss_insert_status->begin();
            it_hss_result != m_hss_insert_status->end();
            ++it_hss_result )
      {
         MonitoringConfEventStatus mixed_cfg = it_hss_result->second;

         //Update the shorterm with the result coming from mme
         if(!it_hss_result->second.isLongTermEvt())
         {
            if(mme_response_msg.m_mme_response != NULL){

               //There was a response coming from mme
               EvenStatusMap::iterator iter_mmeres = mme_response_msg.m_mme_response->find(it_hss_result->first);

               if(iter_mmeres != mme_response_msg.m_mme_response->end()){
                  mixed_cfg.result = iter_mmeres->second.result;
               }
            }
            else{
               //it is a simulated mme response because the mme was down or the ue was unreachable
               if(mme_response_msg.m_imsi_reachable == MME_DOWN){
                  mixed_cfg.result = DIAMETER_UNABLE_TO_COMPLY;
               }
               else {
                  mixed_cfg.result = DIAMETER_UNABLE_TO_COMPLY;
               }
            }
         }

         ////////////////////////////
         ////////////////////////////
         // Fill the final structure from where the RIR will be built
         ////////////////////////////
         ////////////////////////////
         ImsiStatus imsi_status(mme_response_msg.m_imsi, mixed_cfg, mme_response_msg.m_imsi_reachable, mme_response_msg.m_msisdn);
         std::pair<std::string, uint32_t> ascef_id = it_hss_result->first;

         EventImsiStatus::iterator imsi_statusiter= imsi_status_map.find(ascef_id);
         if( imsi_statusiter != imsi_status_map.end() ) {
            imsi_statusiter->second.push_back(imsi_status);
         }
         else{
            std::vector<ImsiStatus> list_imsi;
            list_imsi.push_back(imsi_status);
            imsi_status_map[ascef_id] = list_imsi;
         }
      }

      if(mme_response_msg.m_mme_response != NULL){
         delete mme_response_msg.m_mme_response;
         mme_response_msg.m_mme_response = NULL;
      }

      if(m_nb_ida_proc == 0 && m_ciasent ) {

         sendRIR();
         if(m_hss_insert_status != NULL) {
            delete m_hss_insert_status;
            m_hss_insert_status = NULL;
         }
         quit();
      }
   }
   else if ( msg.getId() == HANDLE_CIA_SENT )
   {
      m_ciasent = true;
      if(m_nb_ida_proc == 0){
         sendRIR();
         if(m_hss_insert_status != NULL) {
            delete m_hss_insert_status;
            m_hss_insert_status = NULL;
         }
         quit();
      }
   }
}

HandleMmeResponseEvtMsg::HandleMmeResponseEvtMsg( EvenStatusMap* mme_response, std::string &imsi, int imsi_reachable, std::string &msisdn)
 : SEventThreadMessage( HANDLE_MME_RESPONSE ),
   m_mme_response    ( mme_response ),
   m_imsi            ( imsi ),
   m_imsi_reachable  ( imsi_reachable ),
   m_msisdn          ( msisdn )
{

}
