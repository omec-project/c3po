/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>
#include "logger.h"
#include "s6as6d_impl.h"
#include "fdjson.h"
#include "common_def.h"
#include "s6t.h"
#include "s6t_impl.h"
#include "dataaccess.h"
#include "fdhss.h"
#include "rapidjson/document.h"
#include "statshss.h"
#include "util.h"
#include "hssStatsPromClient.h"

#include <iomanip>
extern "C" {
#include "hss_config.h"
#include "access_restriction.h"
#include "aucpp.h"
#include <inttypes.h>
}

#include <sys/types.h>
#include <set>

#include "satomic.h"

/* ULR-Flags meaning: */
#define ULR_SINGLE_REGISTRATION_IND      (1U)
#define ULR_S6A_S6D_INDICATOR          (1U << 1)
#define ULR_SKIP_SUBSCRIBER_DATA        (1U << 2)
#define ULR_GPRS_SUBSCRIPTION_DATA_IND   (1U << 3)
#define ULR_NODE_TYPE_IND             (1U << 4)
#define ULR_INITIAL_ATTACH_IND         (1U << 5)
#define ULR_PS_LCS_SUPPORTED_BY_UE      (1U << 6)
#define ULR_PAD_VALID(x)              ((x & ~0x7f) == 0)

/* Access-restriction-data bits */
#define UTRAN_NOT_ALLOWED         (1U)
#define GERAN_NOT_ALLOWED         (1U << 1)
#define GAN_NOT_ALLOWED           (1U << 2)
#define I_HSDPA_EVO_NOT_ALLOWED     (1U << 3)
#define E_UTRAN_NOT_ALLOWED        (1U << 4)
#define HO_TO_NON_3GPP_NOT_ALLOWED   (1U << 5)

//#define FLAG_IS_SET(x, flag)       ((x) & (flag))


/* PUR-Flags */
#define PUR_UE_PURGED_IN_MME     (1U)
#define PUR_UE_PURGED_IN_SGSN    (1U << 1)
#define PUR_PAD_VALID(x)       ((x & ~0x3) == 0)

#include "timer.h"
#include "satomic.h"

#ifdef PERFORMANCE_TIMING
#define MAX_ULR_TIMERS  1048576

struct ULRPerformanceTimers
{
   stimer_t ulr1;
   stimer_t ulr2;
   stimer_t ulr3;
   stimer_t ulr4;
   stimer_t ulr5;
   stimer_t ulr6;
   stimer_t ulr7;
   stimer_t ulr8;
   stimer_t ulr9;
} ulrTimers[MAX_ULR_TIMERS];

long nextULRPerformanceTimer = 0;

#define ULR_TIMER_SET(_timer,_ofs) \
{ \
   if (_ofs >=0 && _ofs < MAX_ULR_TIMERS) \
     ulrTimers[_ofs]._timer = STIMER_GET_CURRENT_TIME; \
}

void dumpUlrTimers()
{
   STime t;

   char fn[128];

   t.Format(fn, sizeof(fn), "ulr_%Y%m%d%H%M%S.csv", true);

   FILE *fp = fopen(fn, "wt");

   fprintf(fp, "ulr1,ulr2,ulr3,ulr4,ulr5,ulr6,ulr7,ulr8,ulr9\n");

   for (long idx=0; ulrTimers[idx].ulr1 > 0; idx++)
   {
     fprintf(fp, "%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",
         ulrTimers[idx].ulr1, ulrTimers[idx].ulr2, ulrTimers[idx].ulr3,
         ulrTimers[idx].ulr4, ulrTimers[idx].ulr5, ulrTimers[idx].ulr6,
         ulrTimers[idx].ulr7, ulrTimers[idx].ulr8, ulrTimers[idx].ulr9);
   }

   fclose(fp);

   std::cout << "ULR timers written to [" << fn << "]" << std::endl;

   nextULRPerformanceTimer = 0;
}
#else
#define ULR_TIMER_SET(_timer,_ofs)
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


namespace s6as6d {

void fillscefnotify(DataAccess &dataaccess, DAEventIdList& event_list, uint32_t evt_type, std::vector< std::pair< std::string, uint32_t> > &scef_tonotify);

void fdJsonError( const char *err )
{
   printf("json error: %s", err);
}

// Member functions that customize the individual application
Application::Application( DataAccess &dbobj, bool verify_roaming)
   : ApplicationBase()
   , m_cmd_uplr( *this )
   //, m_cmd_calr( *this )
   , m_cmd_auir( *this )
   //, m_cmd_insdr( *this )
   //, m_cmd_desdr( *this )
   , m_cmd_puur( *this )
   //, m_cmd_rer( *this )
   , m_dbobj( dbobj )
{
   verify_roaming_access = verify_roaming;
   registerHandlers();
}

Application::~Application()
{
}

void Application::registerHandlers()
{
   // Remove the comments for any Command this application should
   // listen for (receive).
   Logger::s6as6d().startup("Registering s6as6d command handlers");
   Logger::s6as6d().startup("Registering UPLR command handler");
   registerHandler( m_cmd_uplr );
   //std::cout << "Registering CALR command handler" << std::endl;
   //registerHandler( m_cmd_calr );
   Logger::s6as6d().startup("Registering AUIR command handler");
   registerHandler( m_cmd_auir );
   //std::cout << "Registering INSDR command handler" << std::endl;
   //registerHandler( m_cmd_insdr );
   //std::cout << "Registering DESDR command handler" << std::endl;
   //registerHandler( m_cmd_desdr );
   Logger::s6as6d().startup("Registering PUUR command handler");
   registerHandler( m_cmd_puur );
   //std::cout << "Registering RER command handler" << std::endl;
   //registerHandler( m_cmd_rer );
}

// UPLR Request (req) Command member functions

// Sends a UPLR Request to the corresponding Peer
bool Application::sendUPLRreq(FDPeer &peer)
{
   //TODO - This code may be modified based on specific
   //       processing needs to send the UPLR Command
   UPLRreq *s = createUPLRreq( peer );

   try
   {
     if ( s )
     {
       s->send();
     }
   }
   catch ( FDException &ex )
   {
     Logger::s6as6d().error("EXCEPTION - %s", ex.what());
     delete s;
     s = NULL;
   }

   // DO NOT free the newly created UPLRreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for UPLR reuqests
UPLRreq *Application::createUPLRreq(FDPeer &peer)
{
   //  creates the UPLRreq object
   UPLRreq *s = new UPLRreq( *this );

   //TODO - Code must be added to correctly
   //       populate the UPLR request object

   // return the newly created request object
   return s;
}

// A handler for Answers corresponding to this specific Request
void UPLRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// UPLR Command
}

void display_error_message(const char *err_msg)
{

}

int UPLRcmd::process( FDMessageRequest *req )
{
   ULRProcessor *p = new ULRProcessor( *req, m_app, m_app.getDict() );
   fdHss.getWorkerQueue().addProcessor(p);
   fdHss.getWorkerQueue().startProcessor();
   return 0;
}

// CALR Request (req) Command member functions

// Sends a CALR Request to the corresponding Peer
bool Application::sendCALRreq(FDPeer &peer)
{
   //TODO - This code may be modified based on specific
   //       processing needs to send the CALR Command
   CALRreq *s = createCALRreq( peer );

   try
   {
     if ( s )
     {
       s->send();
     }
   }
   catch ( FDException &ex )
   {
     Logger::s6as6d().error("EXCEPTION - %s", ex.what());
     delete s;
     s = NULL;
   }

   // DO NOT free the newly created CALRreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for CALR reuqests
CALRreq *Application::createCALRreq(FDPeer &peer)
{
   //  creates the CALRreq object
   CALRreq *s = new CALRreq( *this );

   //TODO - Code must be added to correctly
   //       populate the CALR request object

   // return the newly created request object
   return s;
}

// A handler for Answers corresponding to this specific Request
void CALRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// CALR Command
}

// CALR Command (cmd) member function

// Function invoked when a CALR Command is received
int CALRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the CALR command.
   return -1;
}
 
// AUIR Request (req) Command member functions


// Sends a AUIR Request to the corresponding Peer
bool Application::sendAUIRreq(FDPeer &peer)
{
   AUIRreq *s = createAUIRreq( peer );

   try
   {
     if ( s )
     {
       s->send();
     }
   }
   catch ( FDException &ex )
   {
     Logger::s6as6d().error("EXCEPTION - %s", ex.what());
     delete s;
     s = NULL;
   }

   // DO NOT free the newly created AUIRreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for AUIR reuqests
AUIRreq *Application::createAUIRreq(FDPeer &peer)
{
   //  creates the AUIRreq object
   AUIRreq *s = new AUIRreq( *this );

   //TODO - Code must be added to correctly
   //       populate the AUIR request object

   // return the newly created request object
   return s;
}

// A handler for Answers corresponding to this specific Request
void AUIRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// AUIR Command
}

// AUIR Command (cmd) member function

int AUIRcmd::process( FDMessageRequest *req )
{
    // add something here in AIR Processor
   AIRProcessor *p = new AIRProcessor(*req, m_app, m_app.getDict());
   fdHss.getWorkerQueue().addProcessor(p);
   fdHss.getWorkerQueue().startProcessor();
   return 0;
}

// INSDR Request (req) Command member functions

IDRRreq::IDRRreq(Application &app, FDMessageRequest *cir_req,
             EvenStatusMap *evt_map, RIRBuilder *rirbuilder, std::string &imsi, std::string &msisdn):
     INSDRreq      ( app ),
     cir_req      (cir_req),
     evt_map      (evt_map),
     m_rirbuilder   (rirbuilder),
     m_imsi       (imsi),
     m_msisdn      (msisdn)
{
}

// A handler for Answers corresponding to this specific Request
void IDRRreq::processAnswer( FDMessageAnswer &ans )
{
   //Extract the IDA.
   InsertSubscriberDataAnswerExtractor ida(ans, getApplication().getDict() );

   //Build the CIA from the stored request.
   s6t::Application * s6tApp = fdHss.gets6tApp();

   FDMessageAnswer cia ( cir_req );

   cia.add( s6tApp->getDict().avpAuthSessionState(), 1 );
   cia.addOrigin();
   cia.add( s6tApp->getDict().avpResultCode(), ER_DIAMETER_SUCCESS);

   uint32_t vendor_code = 0;
   uint32_t ida_result_code = 0;
   //check the global status from the IDA response
   if(ida.result_code.get(ida_result_code)){
      StatsHss::singleton().registerStatResult(stat_hss_idr, 0, ida_result_code);
   }
   else{
      ida.experimental_result.vendor_id.get(vendor_code);
      ida.experimental_result.experimental_result_code.get(ida_result_code);
      StatsHss::singleton().registerStatResult(stat_hss_idr, vendor_code, ida_result_code);
   }

   if(!m_rirbuilder){

     //////////////////////////////////
     /////Single IMSI case
     //////////////////////////////////

     //Fill the long term event result status based on the insertion result on db
     for (EvenStatusMap::iterator it= evt_map->begin(); it!=evt_map->end(); ++it){
       if(it->second.isLongTermEvt()){
         FDAvp mon_evt_cfg_status( s6tApp->getDict().avpMonitoringEventConfigStatus() );
         fdHss.buildCfgStatusAvp(mon_evt_cfg_status, it->second);
         cia.add(mon_evt_cfg_status);
       }
     }

     if(ida_result_code == DIAMETER_SUCCESS){

       //////////////////////////////////////////////////////////
       ///// Response OK from MME
       ///// Build a CIA response including
       ///// + For the long term events: The config status of
       /////   reflecting the DB operation (failure/ok)
       ///// + For the one-shot events: The response from
       /////   the mme
       //////////////////////////////////////////////////////////

       //Fill the one-shot event result based on the response from the mme.
       for( std::list<MonitoringEventConfigStatusExtractor*>::iterator it = ida.monitoring_event_config_status.getList().begin();
            it != ida.monitoring_event_config_status.getList().end(); ++ it)
       {
         std::string   scef_id;
         uint32_t      scef_ref_id;
         (*it)->scef_id.get(scef_id);
         (*it)->scef_reference_id.get(scef_ref_id);

         bool service_report_found = false;
         std::list<ServiceReportExtractor*>::iterator it_service;
         if( !(*it)->service_report.getList().empty() ){
            service_report_found = true;
            it_service = (*it)->service_report.getList().begin();
         }

         EvenStatusMap::iterator map_iter = evt_map->find( std::make_pair(scef_id, scef_ref_id) );

         if(map_iter != evt_map->end()){

            if( !map_iter->second.isLongTermEvt() ){
              uint32_t  service_result_code;
              if(service_report_found && (*it_service)->service_result.service_result_code.get(service_result_code)){
                Logger::s6as6d().info("service_result found on the mme response, updating to %u", service_result_code);
                map_iter->second.result = service_result_code;
              }
              //We only add the one shot events
              FDAvp mon_evt_cfg_status( s6tApp->getDict().avpMonitoringEventConfigStatus() );
              fdHss.buildCfgStatusAvp(mon_evt_cfg_status, map_iter->second);
              cia.add(mon_evt_cfg_status);
            }
         }
       }
     }
     else{

       //////////////////////////////////////////////////////////
       ///// Response KO from MME
       ///// Build a CIA response including
       ///// + For the long term events: The config status of
       /////   reflecting the DB operation (failure/ok)
       ///// + For the one-shot events: status DIAMETER_ERROR_USER_UNKNOWN
       ///// + Include a cia flag indicating ABSENT_SUBSCRIBER
       //////////////////////////////////////////////////////////

       for (EvenStatusMap::iterator it= evt_map->begin(); it!=evt_map->end(); ++it){
         if(!it->second.isLongTermEvt()){
            it->second.result = DIAMETER_ERROR_USER_UNKNOWN;
            FDAvp mon_evt_cfg_status( s6tApp->getDict().avpMonitoringEventConfigStatus() );
            fdHss.buildCfgStatusAvp(mon_evt_cfg_status, it->second);
            cia.add(mon_evt_cfg_status);
         }
       }
       //Indicate error on mme by setting the S6T-HSS-Cause absent subscriber
       cia.add(s6tApp->getDict().avpS6tHssCause(), ABSENT_SUBSCRIBER);
     }

     delete evt_map;
     cia.send();

     //Add stats for cir result
     StatsHss::singleton().registerStatResult(stat_hss_cir, 0, ER_DIAMETER_SUCCESS);

     if(cir_req != NULL){
       delete cir_req;
       cir_req = NULL;
     }
   }
   else{

     //////////////////////////////////
     /////Group IMSI case
     //////////////////////////////////


     if(ida_result_code == DIAMETER_SUCCESS){

       //////////////////////////////////////////////////////////
       ///// Response OK from MME
       ///// Store mme response for further inclusion on RIR
       //////////////////////////////////////////////////////////

       EvenStatusMap* mme_response = new EvenStatusMap();

       for( std::list<MonitoringEventConfigStatusExtractor*>::iterator it = ida.monitoring_event_config_status.getList().begin();
            it != ida.monitoring_event_config_status.getList().end(); ++it)
       {
         std::string   scef_id;
         uint32_t      scef_ref_id;
         (*it)->scef_id.get(scef_id);
         (*it)->scef_reference_id.get(scef_ref_id);

         bool service_report_found = false;
         std::list<ServiceReportExtractor*>::iterator it_service;
         if( !(*it)->service_report.getList().empty() ){
            service_report_found = true;
            it_service = (*it)->service_report.getList().begin();
         }

         MonitoringConfEventStatus mme_status(false);
         mme_status.scef_id = scef_id;
         mme_status.scef_ref_id = scef_ref_id;

         if(service_report_found){
            uint32_t  service_result_code;

            if(service_report_found && (*it_service)->service_result.service_result_code.get(service_result_code)){
              mme_status.result = service_result_code;
            }
         }
         mme_response->insert( std::make_pair(std::make_pair(scef_id, scef_ref_id), mme_status) );
       }
       HandleMmeResponseEvtMsg *e = new HandleMmeResponseEvtMsg( mme_response, m_imsi, 0 , m_msisdn);
       m_rirbuilder->postMessage( e );
     }
     else{

       //////////////////////////////////////////////////////////
       ///// Response KO from MME
       ///// Store a fake response for further inclusion on RIR
       //////////////////////////////////////////////////////////

       HandleMmeResponseEvtMsg *e = new HandleMmeResponseEvtMsg( NULL, m_imsi, MME_DOWN,  m_msisdn);
       m_rirbuilder->postMessage( e );
     }
   }
}

// Sends a INSDR Request to the corresponding Peer
int Application::sendINSDRreq(s6t::MonitoringEventConfigurationExtractorList &cir_monevtcfg,
                        std::string& imsi, FDMessageRequest *cir_req, EvenStatusMap *evt_map,
                        RIRBuilder * rir_builder)
{

   DAImsiInfo imsi_info;
   //1.get the subscription data from database
   m_dbobj.getImsiInfo((char*)imsi.c_str(), imsi_info, NULL, NULL);
   bool imsi_attached = (imsi_info.ms_ps_status == "ATTACHED");
   FDPeer peer;
   peer.setDiameterId( (DiamId_t)imsi_info.mmehost.c_str() );
   //peer.setPort(38680);

   bool mme_reachable = ( peer.getState() == PSOpen);

   if(!imsi_attached || !mme_reachable){

      if(rir_builder == NULL){
        //simple imsi
        if(!imsi_attached){
          printf("****************Application::sendINSDRreq::IMSI_NOT_ACTIVE\n\n");
          return IMSI_NOT_ACTIVE;
        }
        printf("****************Application::sendINSDRreq::MME_DOWN: %s\n\n", imsi_info.mmehost.c_str());
        return MME_DOWN;
      }
      else{

        printf("****************Application::sendINSDRreq::MULTI IMSI\n\n");

        //multi imsi
        if(!imsi_attached){
          printf("****************Application::sendINSDRreq::POSTING fake IMSI_NOT_ACTIVE\n\n");
          HandleMmeResponseEvtMsg *e = new HandleMmeResponseEvtMsg( NULL, imsi, IMSI_NOT_ACTIVE, imsi_info.str_msisdn);
          rir_builder->postMessage( e );
        }
        else if(!mme_reachable){
          printf("****************Application::sendINSDRreq::POSTING fake MME_DOWN\n\n");
          HandleMmeResponseEvtMsg *e = new HandleMmeResponseEvtMsg( NULL, imsi, MME_DOWN,  imsi_info.str_msisdn);
          rir_builder->postMessage( e );
        }
        return true;
      }
   }


   INSDRreq *s = createINSDRreq(cir_monevtcfg, imsi, cir_req, evt_map, imsi_info, rir_builder);

   try
   {
     if ( s )
     {
       s->send();
     }
   }
   catch ( FDException &ex )
   {
     Logger::s6as6d().error("EXCEPTION - %s", ex.what());
     delete s;
     s = NULL;
   }

   // DO NOT free the newly created INSDRreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for INSDR reuqests
INSDRreq *Application::createINSDRreq(s6t::MonitoringEventConfigurationExtractorList &cir_monevtcfg,
                             std::string& imsi, FDMessageRequest *cir_req, EvenStatusMap *evt_map,
                             DAImsiInfo &imsi_info, RIRBuilder *rir_builder)
{
   //  creates the INSDRreq object
   INSDRreq *s = new IDRRreq( *this, cir_req,  evt_map, rir_builder, imsi, imsi_info.str_msisdn);

   s->add( getDict().avpSessionId(), s->getSessionId() );

   s->add( getDict().avpAuthSessionState(), 1 );

   s->addOrigin();

   s->add(getDict().avpUserName(), imsi);

   {
      s->add( getDict().avpDestinationHost(), imsi_info.mmehost );
      s->add( getDict().avpDestinationRealm(), imsi_info.mmerealm );

      printf("Subscription data: %s \n", imsi_info.subscription_data.c_str());

      //2.add the subscription data to the message
      fdJsonAddAvps( imsi_info.subscription_data.c_str() , s->getMsg(), NULL );
      //3.create a extractor to get to the subscription data avp
      InsertSubscriberDataRequestExtractor idr( *s, getDict() );
      //4. Get the pointer to the subscription data
      FDAvp subscription_data(getDict().avpSubscriptionData(), (struct avp *)idr.subscription_data.getReference());
      subscription_data.add(cir_monevtcfg);
   }
   return s;
}

// A handler for Answers corresponding to this specific Request
void INSDRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// INSDR Command
}

// INSDR Command (cmd) member function

// Function invoked when a INSDR Command is received
int INSDRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the INSDR command.
   return -1;
}
 
// DESDR Request (req) Command member functions


// Sends a DESDR Request to the corresponding Peer
bool Application::sendDESDRreq(FDPeer &peer)
{
   //TODO - This code may be modified based on specific
   //       processing needs to send the DESDR Command
   DESDRreq *s = createDESDRreq( peer );

   try
   {
     if ( s )
     {
       s->send();
     }
   }
   catch ( FDException &ex )
   {
     Logger::s6as6d().error("EXCEPTION - %s", ex.what());
     delete s;
   s = NULL;
   }

   // DO NOT free the newly created DESDRreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for DESDR reuqests
DESDRreq *Application::createDESDRreq(FDPeer &peer)
{
   //  creates the DESDRreq object
   DESDRreq *s = new DESDRreq( *this );

   //TODO - Code must be added to correctly
   //       populate the DESDR request object

   // return the newly created request object
   return s;
}

// A handler for Answers corresponding to this specific Request
void DESDRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// DESDR Command
}

// DESDR Command (cmd) member function

// Function invoked when a DESDR Command is received
int DESDRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the DESDR command.
   return -1;
}
 
// PUUR Request (req) Command member functions


// Sends a PUUR Request to the corresponding Peer
bool Application::sendPUURreq(FDPeer &peer)
{
   //TODO - This code may be modified based on specific
   //       processing needs to send the PUUR Command
   PUURreq *s = createPUURreq( peer );

   try
   {
     if ( s )
     {
       s->send();
     }
   }
   catch ( FDException &ex )
   {
     Logger::s6as6d().error("EXCEPTION - %s", ex.what());
     delete s;
     s = NULL;
   }

   // DO NOT free the newly created PUURreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for PUUR reuqests
PUURreq *Application::createPUURreq(FDPeer &peer)
{
   //  creates the PUURreq object
   PUURreq *s = new PUURreq( *this );

   //TODO - Code must be added to correctly
   //       populate the PUUR request object

   // return the newly created request object
   return s;
}

// A handler for Answers corresponding to this specific Request
void PUURreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// PUUR Command
}

// PUUR Command (cmd) member function

// Function invoked when a PUUR Command is received
int PUURcmd::process( FDMessageRequest *req )
{
   std::string          s;
   std::string          imsi;
   uint32_t             u32;
   DAMmeIdentity        mme_id;
   int                  result_code = ER_DIAMETER_SUCCESS;
   bool                 experimental = false;

   FDMessageAnswer ans( req );
   ans.addOrigin();
   s6as6d::PurgeUeRequestExtractor pur( *req, m_app.getDict() );
   hssStats::Instance()->increment(hssStatsCounter::MME_MSG_RX_S6A_PURGE_REQUEST);

   do {

      pur.auth_session_state.get(u32);
      ans.add( m_app.getDict().avpAuthSessionState(), u32 );

      pur.user_name.get(imsi);
      if(imsi.size() > IMSI_LENGTH){
         result_code = ER_DIAMETER_INVALID_AVP_VALUE;
         break;
      }

      if( pur.pur_flags.get(u32) ){
         if (FLAG_IS_SET (u32, PUR_UE_PURGED_IN_SGSN)) {
           result_code = ER_DIAMETER_INVALID_AVP_VALUE;
           break;
         }
      }

      if( !m_app.dataaccess().getMmeIdentityFromImsi(imsi,mme_id) ){
         experimental = true;
         result_code = DIAMETER_ERROR_USER_UNKNOWN;
         break;
      }

      if( !m_app.dataaccess().purgeUE(imsi) ) {
         experimental = true;
         result_code = DIAMETER_ERROR_USER_UNKNOWN;
         break;
      }

      //Get host and realm
      pur.origin_host.get(s);
      if (mme_id.mme_host != s) {
         result_code = DIAMETER_ERROR_UNKNOWN_SERVING_NODE;
         experimental = true;
      }

      pur.origin_realm.get(s);
      if (mme_id.mme_realm != s) {
         result_code = DIAMETER_ERROR_UNKNOWN_SERVING_NODE;
         experimental = true;
      }
      ans.add( m_app.getDict().avpPuaFlags(), 1 );

   } while (false);


   //Handle errors
   if (DIAMETER_ERROR_IS_VENDOR (result_code) && experimental) {
      FDAvp experimental_result ( m_app.getDict().avpExperimentalResult() );
      experimental_result.add( m_app.getDict().avpVendorId(),  VENDOR_3GPP);
      experimental_result.add( m_app.getDict().avpExperimentalResultCode(),  result_code);
      ans.add(experimental_result);
      StatsHss::singleton().registerStatResult(stat_hss_pur, VENDOR_3GPP, result_code);
      std::stringstream ss; ss<<result_code;
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_PURGE_ANSWER_FAILURE, {{"result_code",ss.str()}});
   }
   else{
      ans.add( m_app.getDict().avpResultCode(), result_code);
      StatsHss::singleton().registerStatResult(stat_hss_pur, 0, result_code);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_PURGE_ANSWER_SUCCESS);
   }

   ans.send();
   return 0;
}
 
// RER Request (req) Command member functions


// Sends a RER Request to the corresponding Peer
bool Application::sendRERreq(FDPeer &peer)
{
   //TODO - This code may be modified based on specific
   //       processing needs to send the RER Command
   RERreq *s = createRERreq( peer );

   try
   {
     if ( s )
     {
       s->send();
     }
   }
   catch ( FDException &ex )
   {
     Logger::s6as6d().error("EXCEPTION - %s", ex.what());
     delete s;
     s = NULL;
   }

   // DO NOT free the newly created RERreq object!!
   // It will be deleted by the framework after the
   // answer is received and processed.
   return s != NULL;
}

// A factory for RER reuqests
RERreq *Application::createRERreq(FDPeer &peer)
{
   //  creates the RERreq object
   RERreq *s = new RERreq( *this );

   //TODO - Code must be added to correctly
   //       populate the RER request object

   // return the newly created request object
   return s;
}

// A handler for Answers corresponding to this specific Request
void RERreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// RER Command
}

// RER Command (cmd) member function

// Function invoked when a RER Command is received
int RERcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the RER command.
   return -1;
}
 

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ULRStateProcessor::ULRStateProcessor(uint16_t state, ULRProcessor *ulrproc)
   : m_state( state ),
     m_processor( ulrproc )
{
}

ULRStateProcessor::~ULRStateProcessor()
{
}

void ULRStateProcessor::process()
{
   if (!m_processor)
      return;

   ULRProcessor::processNextPhase(m_processor);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ULRProcessor::ULRProcessor(FDMessageRequest &req, s6as6d::Application &app, s6as6d::Dictionary &dict)
   : m_ulr(req, dict),
     m_ans(&req),
     m_app(app),
     m_dict(dict)
{
   m_perf_timer = 0;
   m_present_flags = 0;
   m_plmn_len = sizeof(m_plmn_id);
   m_3count = 0;
   m_3aSuccess = false;
   m_3bSuccess = false;
   m_mmeidentity = -1;
   m_ulrflags = 0;

   m_nextphase = ULRSTATE_PHASE1;
   m_msgissued = 0;
   m_dbexecuted = 0;
   m_dbresult = -1;
   m_dbissued = 0;
   m_dbevtissued = 0;
}

ULRProcessor::~ULRProcessor()
{
}

////////////////////////////////////////////////////////////////////////////////

void ULRProcessor::on_ulr_callback(CassFuture *future, void *data)
{
   SCassFuture f(future,true);
   ULRDatabaseAction *action = (ULRDatabaseAction*)data;
#ifdef TRACK_EXECUTION
   const char *actions[] = {
         "ULRDB_GET_IMSI_INFO",
         "ULRDB_GET_EXT_IDS",
         "ULRDB_GET_EVNTIDS_MSISDN",
         "ULRDB_GET_EVNTIDS_EXTIDS",
         "ULRDB_GET_EVNTS_EVNTIDS",
         "ULRDB_GET_MMEID_HOST",
         "ULRDB_UPDATE_IMSI",
         "UNKNOWN"
   };
   int actionofs =
      action->getAction() == ULRDB_GET_IMSI_INFO ? 0 :
      action->getAction() == ULRDB_GET_EXT_IDS ? 1 :
      action->getAction() == ULRDB_GET_EVNTIDS_MSISDN ? 2 :
      action->getAction() == ULRDB_GET_EVNTIDS_EXTIDS ? 3 :
      action->getAction() == ULRDB_GET_EVNTS_EVNTIDS ? 4 :
      action->getAction() == ULRDB_GET_MMEID_HOST ? 5 :
      action->getAction() == ULRDB_UPDATE_IMSI ? 6 : 7;

printf("%lld,%s,%p,%s\n",STIMER_GET_CURRENT_TIME,__PRETTY_FUNCTION__,&action->getProcessor(),actions[actionofs]);
#endif

   switch (action->getAction())
   {
      case ULRDB_GET_IMSI_INFO:
      {
         action->getProcessor().getImsiInfo(f);
         break;
      }
      case ULRDB_GET_EXT_IDS:
      {
         action->getProcessor().getExternalIds(f);
         break;
      }
      case ULRDB_GET_EVNTIDS_MSISDN:
      {
         action->getProcessor().getEventIdsMsisdn(f);
         break;
      }
      case ULRDB_GET_EVNTIDS_EXTIDS:
      {
         action->getProcessor().getEventIdsExternalIds(f);
         break;
      }
      case ULRDB_GET_EVNTS_EVNTIDS:
      {
         action->getProcessor().getEvents(f);
         break;
      }
      case ULRDB_GET_MMEID_HOST:
      {
         action->getProcessor().getMmeIdentity(f);
         break;
      }
      case ULRDB_UPDATE_IMSI:
      {
         action->getProcessor().updateImsiInfo(f);
         break;
      }
   }

   SMutexLock l(action->getProcessor().m_mutex, false);

   if (l.acquire(false))
      action->getProcessor().triggerNextPhase();

   atomic_dec_fetch(action->getProcessor().m_dbissued);
}

void ULRProcessor::triggerNextPhase()
{
   atomic_inc_fetch(m_msgissued);
   fdHss.getWorkMgr().addWork(
         new WorkerMessage(WORKER_EVENT,
               new ULRStateProcessor(m_nextphase, this)
         )
   );
}

bool ULRProcessor::phaseReady(int phase, uint32_t adjustment)
{
   bool ready = false;
#ifdef TRACK_EXECUTION
   const char *phases[] = {
         "ULRSTATE_PHASEFINAL",
         "ULRSTATE_PHASE1",
         "ULRSTATE_PHASE2",
         "ULRSTATE_PHASE3",
         "ULRSTATE_PHASE4",
         "ULRSTATE_PHASE5",
         "UNKNOWN"
   };
#endif

   switch (phase)
   {
      case ULRSTATE_PHASE1:
      {
         ready = true;
         break;
      }
      case ULRSTATE_PHASE2:
      {
         ready = m_dbexecuted & ULRDB_GET_IMSI_INFO;
         break;
      }
      case ULRSTATE_PHASE3:
      {
         ready =
               FLAG_IS_SET(m_ulrflags, ULR_SKIP_SUBSCRIBER_DATA) ||
               (m_dbexecuted & ULRDB_GET_EVNTS_EVNTIDS);
         break;
      }
      case ULRSTATE_PHASE4:
      {
         ready = m_dbexecuted & ULRDB_GET_MMEID_HOST;
         break;
      }
      case ULRSTATE_PHASE5:
      {
         ready = m_dbexecuted & ULRDB_UPDATE_IMSI;
         break;
      }
      case ULRSTATE_PHASEFINAL:
      {
#ifdef TRACK_EXECUTION
printf("%lld,%s,%p,%s,m_dbissued=%d,m_msgissued=%d\n",STIMER_GET_CURRENT_TIME,__PRETTY_FUNCTION__,this,phases[phase-ULRSTATE_BASE],m_dbissued-adjustment,m_msgissued);
#endif
         ready = ((m_dbissued - adjustment) <= 0 && m_msgissued == 0);
         break;
      }
   }

   return ready;
}

void ULRProcessor::processNextPhase(ULRProcessor *pthis)
{
   ULRProcessor *deleteProc = NULL;
#ifdef TRACK_EXECUTION
   const char *phases[] = {
         "ULRSTATE_PHASEFINAL",
         "ULRSTATE_PHASE1",
         "ULRSTATE_PHASE2",
         "ULRSTATE_PHASE3",
         "ULRSTATE_PHASE4",
         "ULRSTATE_PHASE5",
         "UNKNOWN"
   };
#endif

   {
      SMutexLock l(pthis->m_mutex, false);

      // check to see if there is already a worker processing
      if (!l.acquire(false))
         return;

      atomic_dec_fetch(pthis->m_msgissued);

      while (pthis && pthis->phaseReady(pthis->m_nextphase))
      {
#ifdef TRACK_EXECUTION
printf("%lld,%s,%p,%s\n",STIMER_GET_CURRENT_TIME,__PRETTY_FUNCTION__,pthis,phases[pthis->m_nextphase - ULRSTATE_BASE]);
#endif
         switch (pthis->m_nextphase)
         {
            case ULRSTATE_PHASE1:
            {
               hssStats::Instance()->increment(hssStatsCounter::MME_MSG_RX_S6A_UPDATE_LOCATION_REQUEST);
               pthis->phase1();
               break;
            }
            case ULRSTATE_PHASE2:
            {
               pthis->phase2();
               break;
            }
            case ULRSTATE_PHASE3:
            {
               pthis->phase3();
               break;
            }
            case ULRSTATE_PHASE4:
            {
               pthis->phase4();
               break;
            }
            case ULRSTATE_PHASE5:
            {
               pthis->phase5();
               break;
            }
            case ULRSTATE_PHASEFINAL:
            {
               deleteProc = pthis;
               pthis = NULL;
               break;
            }
            default:
            {
               Logger::s6as6d().warn("Unrecognized ULRSTATE (%u)", pthis->m_nextphase);
               pthis = NULL;
               break;
            }
         }

      }
   }

   if (deleteProc)
   {
      delete deleteProc;
      deleteProc = NULL;
      fdHss.getWorkerQueue().finishProcessor();
      fdHss.getWorkerQueue().startProcessor();
   }
}

////////////////////////////////////////////////////////////////////////////////

void ULRProcessor::getImsiInfo(SCassFuture &future)
{
   bool success =  m_app.dataaccess().getImsiInfoData(future, m_orig_info);
   DB_OP_COMPLETE(ULRDB_GET_IMSI_INFO, m_dbexecuted, m_dbresult, success);
}

void ULRProcessor::getExternalIds(SCassFuture &future)
{
   bool success = m_app.dataaccess().getExtIdsFromImsiData( future, m_extIdLst );
   DB_OP_COMPLETE(ULRDB_GET_EXT_IDS, m_dbexecuted, m_dbresult, success);

   if (success)
   {
      bool first = true;
      std::stringstream ss;

      for (auto it = m_extIdLst.begin(); it != m_extIdLst.end(); ++it)
      {
         if (first)
            first = false;
         else
            ss << ",";
         ss << "'" << *it << "'";
      }

      atomic_inc_fetch(m_dbissued);
      success = m_app.dataaccess().getEventIdsFromExtIds(ss.str(), m_evtIdLst,
            on_ulr_callback, new ULRDatabaseAction(ULRDB_GET_EVNTIDS_EXTIDS, *this));
      if (!success)
      {
         DB_OP_COMPLETE(ULRDB_GET_EVNTIDS_EXTIDS, m_dbexecuted, m_dbresult, false);
         DB_OP_COMPLETE(ULRDB_GET_EVNTS_EVNTIDS, m_dbexecuted, m_dbresult, false);
         atomic_dec_fetch(m_dbissued);
         return;
      }

   }
   else
   {
      DB_OP_COMPLETE(ULRDB_GET_EVNTIDS_EXTIDS, m_dbexecuted, m_dbresult, success);
   }
}

void ULRProcessor::getEventIdsMsisdn(SCassFuture &future)
{
   bool getevents = false;

   {
      SMutexLock l(m_lstmutex);
      bool success = m_app.dataaccess().getEventIdsFromMsisdnData( future, m_evtIdLst );
      DB_OP_COMPLETE(ULRDB_GET_EVNTIDS_MSISDN, m_dbexecuted, m_dbresult, success);

      // if the database operation failed, mark the ULRDB_GET_EVNTS_EVNTIDS
      // as complete and failed also, otherwise of both  ULRDB_GET_EVNTIDS_MSISDN
      // and ULRDB_GET_EVNTIDS_EXTIDS are complete and successful, issue the query
      // to get the events

      if (m_dbexecuted & (ULRDB_GET_EVNTIDS_MSISDN | ULRDB_GET_EVNTIDS_EXTIDS) &&
          m_dbresult & (ULRDB_GET_EVNTIDS_MSISDN | ULRDB_GET_EVNTIDS_EXTIDS))
      {
         getevents = true;
      }
      else
      {
         getevents = false;
      }
   }

   if (getevents)
      getEvents();
}

void ULRProcessor::getEventIdsExternalIds(SCassFuture &future)
{
   bool getevents = false;

   {
      SMutexLock l(m_lstmutex);
      bool success = m_app.dataaccess().getEventIdsFromExtIdsData( future, m_evtIdLst );
      DB_OP_COMPLETE(ULRDB_GET_EVNTIDS_MSISDN, m_dbexecuted, m_dbresult, success);

      // if the database operation failed, mark the ULRDB_GET_EVNTS_EVNTIDS
      // as complete and failed also, otherwise of both  ULRDB_GET_EVNTIDS_MSISDN
      // and ULRDB_GET_EVNTIDS_EXTIDS are complete and successful, issue the query
      // to get the events

      if (m_dbexecuted & (ULRDB_GET_EVNTIDS_MSISDN | ULRDB_GET_EVNTIDS_EXTIDS) &&
          m_dbresult & (ULRDB_GET_EVNTIDS_MSISDN | ULRDB_GET_EVNTIDS_EXTIDS))
      {
         getevents = true;
      }
      else
      {
         getevents = false;
      }
   }

   if (getevents)
      getEvents();
}

void ULRProcessor::getEvents()
{
   bool success = false;
   std::string scef_id = "";
   std::list<uint32_t> scef_ref_ids;

   if (m_evtIdLst.size() == 0)
   {
      DB_OP_COMPLETE(ULRDB_GET_EVNTS_EVNTIDS, m_dbexecuted, m_dbresult, true);
      return;
   }

   // sort the event id list
   m_evtIdLst.sort(DAEventIdList::compare);

   for (auto it = m_evtIdLst.begin(); it != m_evtIdLst.end(); ++it)
   {
      if (scef_id != (*it)->scef_id && scef_ref_ids.size() > 0)
      {
         // issue the query
         atomic_inc_fetch(m_dbissued);
         success = m_app.dataaccess().getEvents(scef_id.c_str(), scef_ref_ids, m_evtLst,
               on_ulr_callback, new ULRDatabaseAction(ULRDB_GET_EVNTS_EVNTIDS, *this));
         if (!success)
         {
            DB_OP_COMPLETE(ULRDB_GET_EVNTS_EVNTIDS, m_dbexecuted, m_dbresult, false);
            atomic_dec_fetch(m_dbissued);
            return;
         }

         // set for the next scef_id
         scef_ref_ids.clear();
         scef_id = (*it)->scef_id;
      }

      scef_ref_ids.push_back((*it)->scef_ref_id);
   }

   if (scef_ref_ids.size() > 0)
   {
      atomic_inc_fetch(m_dbissued);
      success = m_app.dataaccess().getEvents(scef_id.c_str(), scef_ref_ids, m_evtLst,
            on_ulr_callback, new ULRDatabaseAction(ULRDB_GET_EVNTS_EVNTIDS, *this));
      if (!success)
      {
         DB_OP_COMPLETE(ULRDB_GET_EVNTS_EVNTIDS, m_dbexecuted, m_dbresult, false);
         atomic_dec_fetch(m_dbissued);
      }
   }
}

void ULRProcessor::getEvents(SCassFuture &future)
{
   bool success;

   {
      SMutexLock l(m_mutex);
      success = m_app.dataaccess().getEventsData(future, m_evtLst);
      if (!success)
         DB_OP_COMPLETE_RESULT(m_dbresult, ULRDB_GET_EVNTS_EVNTIDS, false);
      if (atomic_dec_fetch(m_dbevtissued) == 0)
         DB_OP_COMPLETE_EXECUTED(m_dbexecuted, ULRDB_GET_EVNTS_EVNTIDS);
   }
}

void ULRProcessor::updateImsiInfo(SCassFuture &future)
{
   bool success = future.errorCode() == CASS_OK;

   if (!success)
      Logger::s6as6d().error( "DataAccess::%s - Error %d executing updateImsiInfo()",
            __func__, future.errorCode() );

   DB_OP_COMPLETE(ULRDB_UPDATE_IMSI, m_dbexecuted, m_dbresult, success);
}

void ULRProcessor::getMmeIdentity(SCassFuture &future)
{
   bool success = m_app.dataaccess().getMmeIdFromHostData(future, m_mmeidentity);
   DB_OP_COMPLETE(ULRDB_GET_MMEID_HOST, m_dbexecuted, m_dbresult, success);
}

////////////////////////////////////////////////////////////////////////////////

void ULRProcessor::phase1()
{
   uint32_t u32;
#ifdef PERFORMANCE_TIMING
   stimer_t start_timer = STIMER_GET_CURRENT_TIME;
#endif

   m_ans.addOrigin();

   m_ulr.auth_session_state.get( u32 );
   m_ans.add( m_app.getDict().avpAuthSessionState(), u32 );

   m_ulr.user_name.get( m_new_info.imsi );
   if( m_new_info.imsi.length() > IMSI_LENGTH )
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","invalid_imsi"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }

   m_ulr.origin_host.get( m_new_info.mmehost );
   m_ulr.origin_realm.get( m_new_info.mmerealm );

#ifdef PERFORMANCE_TIMING
   {
      uint64_t uimsi;
      sscanf ( m_new_info.imsi.c_str(), "%" SCNu64, &uimsi );
      m_perf_timer = uimsi - 1014567891234ULL;
      if (m_perf_timer >= 0 && m_perf_timer < MAX_ULR_TIMERS)
         ulrTimers[m_perf_timer].ulr1 = start_timer;
   }
#endif

   ULR_TIMER_SET(ulr2, m_perf_timer);

   //
   // issue initial database queries
   //

   bool result;

   m_nextphase = ULRSTATE_PHASE2;

   result = m_app.dataaccess().getImsiInfo(m_new_info.imsi.c_str(), m_orig_info,
         on_ulr_callback, new ULRDatabaseAction(ULRDB_GET_IMSI_INFO, *this));

   if (result)
   {
      atomic_inc_fetch(m_dbissued);
      result = m_app.dataaccess().getExtIdsFromImsi(m_new_info.imsi.c_str(), m_extIdLst,
            on_ulr_callback, new ULRDatabaseAction(ULRDB_GET_EXT_IDS, *this));
      if (!result)
      {
         DB_OP_COMPLETE(ULRDB_GET_EXT_IDS, m_dbexecuted, m_dbresult, result);
         DB_OP_COMPLETE(ULRDB_GET_EVNTIDS_EXTIDS, m_dbexecuted, m_dbresult, result);
         DB_OP_COMPLETE(ULRDB_GET_EVNTS_EVNTIDS, m_dbexecuted, m_dbresult, result);
         atomic_dec_fetch(m_dbissued);
      }
   }
   else
   {
      DB_OP_COMPLETE(ULRDB_GET_IMSI_INFO, m_dbexecuted, m_dbresult, result);
   }

   if (result)
   {
      atomic_inc_fetch(m_dbissued);
      result = m_app.dataaccess().getEventIdsFromMsisdn(m_new_info.msisdn, m_evtIdLst,
            on_ulr_callback, new ULRDatabaseAction(ULRDB_GET_EVNTIDS_MSISDN, *this));
      if (!result)
      {
         DB_OP_COMPLETE(ULRDB_GET_EVNTIDS_MSISDN, m_dbexecuted, m_dbresult, result);
         DB_OP_COMPLETE(ULRDB_GET_EVNTS_EVNTIDS, m_dbexecuted, m_dbresult, result);
         atomic_dec_fetch(m_dbissued);
      }
   }

   if (result)
   {
      atomic_inc_fetch(m_dbissued);
      result = m_app.dataaccess().getMmeIdFromHost(m_new_info.mmehost, m_mmeidentity,
            on_ulr_callback, new ULRDatabaseAction(ULRDB_GET_MMEID_HOST, *this));
      if (!result)
      {
         DB_OP_COMPLETE(ULRDB_GET_MMEID_HOST, m_dbexecuted, m_dbresult, result);
         atomic_dec_fetch(m_dbissued);
      }
   }

   if (result)
   {
      atomic_inc_fetch(m_dbissued);
   }
   else
   {
      FDAvp er ( m_dict.avpExperimentalResult() );
      er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
      er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_USER_UNKNOWN);
      m_ans.add(er);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","user_unknown1"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
   }
}

////////////////////////////////////////////////////////////////////////////////

void ULRProcessor::phase2()
{
   std::string s;
   uint32_t u32;

   if (!(m_dbresult & ULRDB_GET_IMSI_INFO))
   {
      FDAvp er ( m_dict.avpExperimentalResult() );
      er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
      er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_USER_UNKNOWN);
      m_ans.add(er);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, VENDOR_3GPP, DIAMETER_ERROR_USER_UNKNOWN);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","user_unknown2"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }

   m_ulr.origin_host.get( m_new_info.mmehost );
   m_ulr.origin_realm.get( m_new_info.mmerealm );

   m_ulr.rat_type.get(u32);
   if(u32 != 1004 || FLAG_IS_SET(m_orig_info.access_restriction , E_UTRAN_NOT_ALLOWED))
   {
      m_ans.add( m_dict.avpResultCode(), DIAMETER_ERROR_RAT_NOT_ALLOWED);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, VENDOR_3GPP, DIAMETER_ERROR_RAT_NOT_ALLOWED);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code", "rat_not_allowed"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }

   m_ulr.ulr_flags.get(u32);

   if (FLAG_IS_SET (u32, ULR_SINGLE_REGISTRATION_IND))
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","invalid_avp_1"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }
   if (!FLAG_IS_SET (u32, ULR_S6A_S6D_INDICATOR))
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","invalid_avp_2"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }
   if (FLAG_IS_SET (u32, ULR_NODE_TYPE_IND))
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","invalid_avp_3"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }

   if (FLAG_IS_SET (u32, ULR_INITIAL_ATTACH_IND))
   {
      FLAGS_SET(m_present_flags, MME_IDENTITY_PRESENT);
   }
   else
   {
      if ( !m_orig_info.mmehost.empty() && !m_orig_info.mmerealm.empty() )
      {
         if(m_orig_info.mmehost != m_new_info.mmehost)
         {
            FDAvp er ( m_dict.avpExperimentalResult() );
            er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
            er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_UNKNOWN_SERVING_NODE);
            m_ans.add(er);
            m_ans.send();
            StatsHss::singleton().registerStatResult(stat_hss_ulr, VENDOR_3GPP, DIAMETER_ERROR_UNKNOWN_SERVING_NODE);
            hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","not_init_unknown_mmehost"}});
            m_nextphase = ULRSTATE_PHASEFINAL;
            return;
         }
         if (m_orig_info.mmerealm != m_new_info.mmerealm)
         {
            FDAvp er ( m_dict.avpExperimentalResult() );
            er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
            er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_UNKNOWN_SERVING_NODE);
            m_ans.add(er);
            m_ans.send();
            StatsHss::singleton().registerStatResult(stat_hss_ulr, VENDOR_3GPP, DIAMETER_ERROR_UNKNOWN_SERVING_NODE);
            hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code", "not_init_unknown_mmerealm"}});
            m_nextphase = ULRSTATE_PHASEFINAL;
            return;
         }
      }
      else
      {
         FDAvp er ( m_dict.avpExperimentalResult() );
         er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
         er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_UNKNOWN_SERVING_NODE);
         m_ans.add(er);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_ulr, VENDOR_3GPP, DIAMETER_ERROR_UNKNOWN_SERVING_NODE);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","not_init_no_mmerealm_mmehost"}});
         m_nextphase = ULRSTATE_PHASEFINAL;
         return;
      }
   }

   if (!ULR_PAD_VALID (u32))
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code", "pad_invalid"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }

   if ( m_ulr.visited_plmn_id.get(m_plmn_id, m_plmn_len) )
   {
      if ( m_plmn_len != 3 )
      {
         m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","plmn_invalid"}});
         m_nextphase = ULRSTATE_PHASEFINAL;
         return;
      }
      else
      {
         //Parse the plmn_id
         PLMNID_TO_HEX_STR( m_new_info.visited_plmnid, m_plmn_id );
      }
   }
   else
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code", "plmn_missing"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }

   if ( m_ulr.terminal_information.imei.get(m_new_info.imei) )
   {
      if ( m_new_info.imei.length() > IMEI_LENGTH )
      {
         m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code", "bad_imei"}});
         m_nextphase = ULRSTATE_PHASEFINAL;
         return;
      }
      FLAGS_SET(m_present_flags, IMEI_PRESENT);
   }
   if ( m_ulr.terminal_information.software_version.get(m_new_info.imei_sv) )
   {
      if ( m_new_info.imei_sv.size() != SV_LENGTH )
      {
         m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code", "bad_imeisv"}});
         m_nextphase = ULRSTATE_PHASEFINAL;
         return;
      }
      FLAGS_SET(m_present_flags, SV_PRESENT);
   }
   if ( m_ulr.terminal_information.tgpp2_meid.get(s) )
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code", "bad_meid"}});
      m_nextphase = ULRSTATE_PHASEFINAL;
      return;
   }

   if ( m_ulr.ue_srvcc_capability.get(u32) )
   {
      FLAGS_SET(m_present_flags, UE_SRVCC_PRESENT);
   }

   for ( std::list<s6as6d::SupportedFeaturesExtractor*>::iterator seit = m_ulr.supported_features.getList().begin();
         seit != m_ulr.supported_features.getList().end();
       ++seit )
   {
      (*seit)->vendor_id.get(u32);
      if ( u32 == VENDOR_3GPP )
      {
         (*seit)->feature_list.get( u32 );
         FLAGS_SET(m_present_flags, MME_SUPPORTED_FEATURES_PRESENT);
      }
   }

   m_ans.add( m_app.getDict().avpUlaFlags(), 1 );

   m_ulr.ulr_flags.get( m_ulrflags );
   if ( !FLAG_IS_SET(m_ulrflags, ULR_SKIP_SUBSCRIBER_DATA) )
   {
      ULR_TIMER_SET(ulr4, m_perf_timer);

      if(fdJsonAddAvps(m_orig_info.subscription_data.c_str(), m_ans.getMsg(), &s6as6d::display_error_message) != 0)
      {
         FDAvp er ( m_dict.avpExperimentalResult() );
         er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
         er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_UNKNOWN_EPS_SUBSCRIPTION);
         m_ans.add(er);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_ulr, VENDOR_3GPP, DIAMETER_ERROR_UNKNOWN_EPS_SUBSCRIPTION);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE, {{"result_code","unknown_eps_subscription"}});
         printf("%s:%d - ULRProcessor::phase2() aborting\n", __FILE__, __LINE__);
         m_nextphase = ULRSTATE_PHASEFINAL;
         return;
      }
   }

   if((m_orig_info.supported_features.length() > 0) && 
       fdJsonAddAvps(m_orig_info.supported_features.c_str(), m_ans.getMsg(), &s6as6d::display_error_message) != 0)
   {
        std::cout<<"Error in adding supported features\n";
   }

   m_nextphase = ULRSTATE_PHASE3;
}

void ULRProcessor::phase3()
{
   if (!FLAG_IS_SET(m_ulrflags, ULR_SKIP_SUBSCRIBER_DATA))
   {
      for ( DAExtIdList::iterator it = m_extIdLst.begin(); it != m_extIdLst.end(); ++it )
      {
         m_app.dataaccess().getEventIdsFromExtId( *it, m_evtIdLst );
      }

      // get the events associated with the event id's
      for ( DAEventIdList::iterator it = m_evtIdLst.begin(); it != m_evtIdLst.end(); ++it )
      {
         DAEvent *e = new DAEvent();

         if ( m_app.dataaccess().getEvent( (*it)->scef_id, (*it)->scef_ref_id, *e ) )
            m_evtLst.push_back( e );
         else
            delete e;
      }

      if ( !m_evtLst.empty() )
      {
         s6as6d::UpdateLocationAnswerExtractor ula( m_ans, m_dict );
         FDAvp sd( m_dict.avpSubscriptionData(), (struct avp *)ula.subscription_data.getReference(), false );
         for ( DAEventList::iterator it = m_evtLst.begin(); it != m_evtLst.end(); ++it )
         {
            sd.addJson( (*it)->mec_json );
         }
      }
   }

   ULR_TIMER_SET(ulr6, m_perf_timer);

   m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_SUCCESS);
   m_ans.send();

   StatsHss::singleton().registerStatResult(stat_hss_ulr, 0, ER_DIAMETER_SUCCESS);
   hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_SUCCESS);

   ULR_TIMER_SET(ulr7, m_perf_timer);

   try
   {
      //After sending the ula, we verify if we need to send a RIR reporting the change of pmnid

      //////////////////////////////////
      /// What if the db is out of sync
      //////////////////////////////////
      if( m_orig_info.ms_ps_status == "ATTACHED"  )
      {
         //If the plmnid has changed, we check if this has to be reported
         //imsi_original_info.visited_plmnid

         if( m_orig_info.visited_plmnid != m_new_info.visited_plmnid )
         {
            for( DAEventList::iterator it_evt = m_evtLst.begin() ; it_evt != m_evtLst.end(); ++ it_evt )
            {
               if ( (*it_evt)->monitoring_type == ROAMING_STATUS_EVT )
               {
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

                  FDAvp monitoring_event_report( fdHss.gets6tApp()->getDict().avpMonitoringEventReport() );

                  monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpScefId(), (*it_evt)->scef_id);
                  monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpScefReferenceId(), (*it_evt)->scef_id);
                  monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpMonitoringType(), ROAMING_STATUS_EVT);
                  monitoring_event_report.add(fdHss.gets6tApp()->getDict().avpVisitedPlmnId(), m_plmn_id, m_plmn_len );

                  s->add(monitoring_event_report);

                  try
                  {
                     if ( s )
                     {
                        s->send();
                     }
                  }
                  catch ( FDException &ex )
                  {
                     Logger::s6as6d().error("EXCEPTION while sending a RIR for ROAMING_STATUS_EVT - %s", ex.what());
                     delete s;
                     s = NULL;
                  }
               }
            }
         }
      }
   }
   catch( ... )
   {
      Logger::s6as6d().error("Error while generating evnt report");
   }

   m_nextphase = ULRSTATE_PHASE4;
}

void ULRProcessor::phase4()
{
   m_nextphase = ULRSTATE_PHASE5;

   if (!(m_dbresult & ULRDB_GET_MMEID_HOST))
   {
      Logger::s6as6d().warn(
            "The MME host [%s] was not found (even though the DIAMETER connection was authorized)",
            m_new_info.mmehost.c_str());
   }

   m_new_info.mme_id = m_mmeidentity;

   atomic_inc_fetch(m_dbissued);
   bool success = m_app.dataaccess().updateLocation(m_new_info, m_present_flags, m_mmeidentity,
         on_ulr_callback, new ULRDatabaseAction(ULRDB_UPDATE_IMSI, *this));
   if (!success)
   {
      DB_OP_COMPLETE(ULRDB_UPDATE_IMSI, m_dbexecuted, m_dbresult, false);
      atomic_dec_fetch(m_dbissued);
      m_nextphase = ULRSTATE_PHASEFINAL;
   }
}

void ULRProcessor::phase5()
{
   m_nextphase = ULRSTATE_PHASEFINAL;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

AIRStateProcessor::AIRStateProcessor(uint16_t state, AIRProcessor *airproc)
   : m_state( state ),
     m_processor( airproc )
{
}

AIRStateProcessor::~AIRStateProcessor()
{
}

void AIRStateProcessor::process()
{
   if (!m_processor)
      return;

   AIRProcessor::processNextPhase(m_processor);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

AIRProcessor::AIRProcessor(FDMessageRequest& req, s6as6d::Application &app, s6as6d::Dictionary &dict)
   : m_air(req, dict),
     m_ans(&req),
     m_app(app),
     m_dict(dict)
{
   m_uimsi = 0;
   m_num_vectors = 0;
   m_plmn_len = sizeof(m_plmn_id);
   m_auts_len = sizeof(m_auts);
   m_auts_set = false;

   m_nextphase = AIRSTATE_PHASE1;
   m_msgissued = 0;
   m_dbexecuted = 0;
   m_dbresult = -1;
   m_dbissued = 0;
   m_dbevtissued = 0;
}

AIRProcessor::~AIRProcessor()
{
}

////////////////////////////////////////////////////////////////////////////////

void AIRProcessor::on_air_callback(CassFuture *future, void *data)
{
   SCassFuture f(future,true);
   AIRDatabaseAction *action = (AIRDatabaseAction*)data;
#ifdef TRACK_EXECUTION
   const char *actions[] = {
         "AIRDB_GET_IMSI_SEC",
         "AIRDB_UPDATE_IMSI",
         "UNKNOWN"
   };
   int actionofs =
      action->getAction() == AIRDB_GET_IMSI_SEC ? 0 :
      action->getAction() == AIRDB_UPDATE_IMSI ? 1 : 7;

   printf("%lld,%s,%p,%s\n",STIMER_GET_CURRENT_TIME,__PRETTY_FUNCTION__,&action->getProcessor(),actions[actionofs]);
#endif

   switch (action->getAction())
   {
      case AIRDB_GET_IMSI_SEC:
      {
         action->getProcessor().getImsiSec(f);
         break;
      }
      case AIRDB_UPDATE_IMSI:
      {
         action->getProcessor().updateImsi(f);
         break;
      }
   }

   SMutexLock l(action->getProcessor().m_mutex, false);

   if (l.acquire(false))
      action->getProcessor().triggerNextPhase();

   atomic_dec_fetch(action->getProcessor().m_dbissued);
}

void AIRProcessor::triggerNextPhase()
{
   atomic_inc_fetch(m_msgissued);
   fdHss.getWorkMgr().addWork(
         new WorkerMessage(WORKER_EVENT,
               new AIRStateProcessor(m_nextphase, this)
         )
   );
}

bool AIRProcessor::phaseReady(int phase, uint32_t adjustment)
{
   bool ready = false;
#ifdef TRACK_EXECUTION
   static const char *phases[] = {
         "AIRSTATE_PHASEFINAL",
         "AIRSTATE_PHASE1",
         "AIRSTATE_PHASE2",
         "AIRSTATE_PHASE3",
         "UNKNOWN"
   };
#endif

   switch (phase)
   {
      case AIRSTATE_PHASE1:
      {
         ready = true;
         break;
      }
      case AIRSTATE_PHASE2:
      {
         ready = m_dbexecuted & AIRDB_GET_IMSI_SEC;
         break;
      }
      case AIRSTATE_PHASE3:
      {
         ready = m_dbexecuted & AIRDB_UPDATE_IMSI;
         break;
      }
      case AIRSTATE_PHASEFINAL:
      {
#ifdef TRACK_EXECUTION
         printf("%lld,%s,%p,%s,m_dbissued=%d,m_msgissued=%d\n",
               STIMER_GET_CURRENT_TIME,__PRETTY_FUNCTION__,this,phases[phase-AIRSTATE_BASE],
               m_dbissued-adjustment,m_msgissued);
#endif
         ready = ((m_dbissued - adjustment) <= 0 && m_msgissued == 0);
         break;
      }
   }

   return ready;
}

void AIRProcessor::processNextPhase(AIRProcessor *pthis)
{
   AIRProcessor *deleteProc = NULL;
#ifdef TRACK_EXECUTION
   static const char *phases[] = {
         "AIRSTATE_PHASEFINAL",
         "AIRSTATE_PHASE1",
         "AIRSTATE_PHASE2",
         "AIRSTATE_PHASE3",
         "UNKNOWN"
   };
#endif

   {
      SMutexLock l(pthis->m_mutex, false);

      // check to see if there is already a worker processing
      if (!l.acquire(false))
         return;

      atomic_dec_fetch(pthis->m_msgissued);

      while (pthis && pthis->phaseReady(pthis->m_nextphase))
      {
#ifdef TRACK_EXECUTION
printf("%lld,%s,%p,%s\n",STIMER_GET_CURRENT_TIME,__PRETTY_FUNCTION__,pthis,phases[pthis->m_nextphase - AIRSTATE_BASE]);
#endif
         switch (pthis->m_nextphase)
         {
            case AIRSTATE_PHASE1:
            {
               hssStats::Instance()->increment(hssStatsCounter::MME_MSG_RX_S6A_AUTH_INFO_REQUEST);
               pthis->phase1();
               break;
            }
            case AIRSTATE_PHASE2:
            {
               pthis->phase2();
               break;
            }
            case AIRSTATE_PHASE3:
            {
               pthis->phase3();
               break;
            }
            case AIRSTATE_PHASEFINAL:
            {
               deleteProc = pthis;
               pthis = NULL;
               break;
            }
            default:
            {
               Logger::s6as6d().warn("Unrecognized AIRSTATE (%u)", pthis->m_nextphase);
               pthis = NULL;
               break;
            }
         }

      }
   }

   if (deleteProc)
   {
      delete deleteProc;
      fdHss.getWorkerQueue().finishProcessor();
      fdHss.getWorkerQueue().startProcessor();
   }
}

////////////////////////////////////////////////////////////////////////////////

void AIRProcessor::getImsiSec(SCassFuture &future)
{
   bool success =  m_app.dataaccess().getImsiSecData(future, m_sec);
   DB_OP_COMPLETE(AIRDB_GET_IMSI_SEC, m_dbexecuted, m_dbresult, success);
}

void AIRProcessor::updateImsi(SCassFuture &future)
{
   bool success = future.errorCode() == CASS_OK;
   DB_OP_COMPLETE(AIRDB_UPDATE_IMSI, m_dbexecuted, m_dbresult, success);

   if (!success)
   {
      Logger::system().warn("AIRProcessor::%s - Error %d while executing updateRandSqn()",
            __func__, future.errorCode());
   }
}

////////////////////////////////////////////////////////////////////////////////

void AIRProcessor::phase1()
{
   uint32_t u32;

   m_ans.addOrigin();

   m_air.auth_session_state.get(u32);
   m_ans.add( m_dict.avpAuthSessionState(), u32 );

   m_air.user_name.get(m_imsi);
   if(m_imsi.length() > IMSI_LENGTH)
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_air, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","invalid_imsi"}});
      m_nextphase = AIRSTATE_PHASEFINAL;
      return;
   }

   sscanf(m_imsi.c_str(), "%" SCNu64, &m_uimsi);

   bool eutran_avp_found = false;

   if(m_air.requested_eutran_authentication_info.number_of_requested_vectors.get(m_num_vectors))
   {
      eutran_avp_found = true;
      if ( m_num_vectors > AUTH_MAX_EUTRAN_VECTORS )
      {
         m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_air, 0, ER_DIAMETER_INVALID_AVP_VALUE);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","invalid_max_eutran_vectors"}});
         m_nextphase = AIRSTATE_PHASEFINAL;
         return;
      }
   }

   if(m_air.requested_eutran_authentication_info.re_synchronization_info.get(m_auts, m_auts_len))
   {
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_RX_S6A_AUTH_INFO_REQUEST_RESYNC);
      eutran_avp_found = true;
      m_auts_set = true;
   }

   if(!eutran_avp_found)
   {
      if(m_air.requested_utran_geran_authentication_info.number_of_requested_vectors.get(m_num_vectors))
      {
         FDAvp er ( m_dict.avpExperimentalResult() );
         er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
         er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_RAT_NOT_ALLOWED);
         m_ans.add(er);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_air, VENDOR_3GPP, DIAMETER_ERROR_RAT_NOT_ALLOWED);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","rat_not_allowed"}});
         m_nextphase = AIRSTATE_PHASEFINAL;
         return;
      }

      if(m_air.requested_utran_geran_authentication_info.re_synchronization_info.get(m_auts, m_auts_len))
      {
         FDAvp er ( m_dict.avpExperimentalResult() );
         er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
         er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_RAT_NOT_ALLOWED);
         m_ans.add(er);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_air, VENDOR_3GPP, DIAMETER_ERROR_RAT_NOT_ALLOWED);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","rat_not_allowed2"}});
         m_nextphase = AIRSTATE_PHASEFINAL;
         return;
      }
   }

   if(m_air.visited_plmn_id.get(m_plmn_id, m_plmn_len))
   {
      if(m_plmn_len == 3 )
      {
         if ((m_app.roaming_access_control() == true) && 
            apply_access_restriction ((char*)m_imsi.c_str(), m_plmn_id) != 0)
         {
            FDAvp er ( m_dict.avpExperimentalResult() );
            er.add( m_dict.avpVendorId(),  VENDOR_3GPP);
            er.add( m_dict.avpExperimentalResultCode(),  DIAMETER_ERROR_ROAMING_NOT_ALLOWED);
            m_ans.add(er);
            m_ans.send();
            StatsHss::singleton().registerStatResult(stat_hss_air, VENDOR_3GPP, DIAMETER_ERROR_ROAMING_NOT_ALLOWED);
            hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","roaming_not_allowed"}});
            m_nextphase = AIRSTATE_PHASEFINAL;
            return;
         }
      }
      else
      {
         m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
         m_ans.send();
         StatsHss::singleton().registerStatResult(stat_hss_air, 0, ER_DIAMETER_INVALID_AVP_VALUE);
         hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","wrong_plmn_len"}});
         m_nextphase = AIRSTATE_PHASEFINAL;
         return;
      }
   }
   else
   {
      m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_INVALID_AVP_VALUE);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_air, 0, ER_DIAMETER_INVALID_AVP_VALUE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","plmn_not_found"}});
      m_nextphase = AIRSTATE_PHASEFINAL;
      return;
   }

   m_nextphase = AIRSTATE_PHASE2;

   if (m_app.dataaccess().getImsiSec(m_imsi, m_sec,
         on_air_callback, new AIRDatabaseAction(AIRDB_GET_IMSI_SEC, *this)))
   {
      atomic_inc_fetch(m_dbissued);
   }
   else
   {
      FDAvp er (m_dict.avpExperimentalResult());
      er.add(m_dict.avpVendorId(),  VENDOR_3GPP);
      er.add(m_dict.avpExperimentalResultCode(), DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE);
      m_ans.add(er);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_air, VENDOR_3GPP, DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","auth_data_not_available"}});
      m_nextphase = AIRSTATE_PHASEFINAL;
   }
}

void AIRProcessor::phase2()
{
   if (!(m_dbresult & AIRDB_GET_IMSI_SEC))
   {
      FDAvp er (m_dict.avpExperimentalResult());
      er.add(m_dict.avpVendorId(),  VENDOR_3GPP);
      er.add(m_dict.avpExperimentalResultCode(), DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE);
      m_ans.add(er);
      m_ans.send();
      StatsHss::singleton().registerStatResult(stat_hss_air, VENDOR_3GPP, DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE);
      hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE, {{"result_code","auth_data_not_available1"}});
      m_nextphase = AIRSTATE_PHASEFINAL;
      return;
   }

   if (m_auts_set)
   {
      uint8_t *sqn = sqn_ms_derive_cpp (m_sec.opc, m_sec.key, m_auts, m_sec.rand);
      if (sqn != NULL)
      {
        //We succeeded to verify SQN_MS...
        //Pick a new RAND and store SQN_MS + RAND in the HSS
        generate_random_cpp (m_vector[0].rand, RAND_LENGTH);

        // save the new rand and sqn locally
        memcpy(m_sec.rand, m_vector[0].rand, sizeof(m_sec.rand));
        /*increment sqn*/
        SqnU64Union eu;

        SQN_TO_U64(sqn, eu);

        eu.u64 += 32;

        U64_TO_SQN(eu,sqn);
        memcpy(m_sec.sqn, sqn, sizeof(m_sec.sqn));

        free (sqn);
      }
   }

   for (uint32_t i = 0; i < m_num_vectors; i++)
   {
      generate_random_cpp (m_vector[i].rand, RAND_LENGTH);
      generate_vector_cpp (m_sec.opc, m_uimsi, m_sec.key, m_plmn_id, m_sec.sqn, &m_vector[i]);
   }

   for (uint32_t i = 0; i < m_num_vectors; i++)
   {
      FDAvp authentication_info ( m_dict.avpAuthenticationInfo() );
      FDAvp eurtran_vector ( m_dict.avpEUtranVector() );

      eurtran_vector.add(m_dict.avpRand(),  m_vector[i].rand,  sizeof(m_vector[i].rand) );
      eurtran_vector.add(m_dict.avpXres(),  m_vector[i].xres,  sizeof(m_vector[i].xres) );
      eurtran_vector.add(m_dict.avpAutn(),  m_vector[i].autn,  sizeof(m_vector[i].autn) );
      eurtran_vector.add(m_dict.avpKasme(), m_vector[i].kasme, sizeof(m_vector[i].kasme));

      authentication_info.add(eurtran_vector);
      m_ans.add(authentication_info);
   }

   m_ans.add( m_dict.avpResultCode(), ER_DIAMETER_SUCCESS);
   m_ans.send();
   StatsHss::singleton().registerStatResult(stat_hss_air, 0, ER_DIAMETER_SUCCESS);
   hssStats::Instance()->increment(hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_SUCCESS);

   m_nextphase = AIRSTATE_PHASE3;

   // combine the rand and sqn updates into a single database update
   if (m_app.dataaccess().updateRandSqn(m_imsi, m_vector[m_num_vectors-1].rand, m_sec.sqn, true,
         on_air_callback, new AIRDatabaseAction(AIRDB_UPDATE_IMSI, *this)))
   {
      atomic_inc_fetch(m_dbissued);
   }
   else
   {
      m_nextphase = AIRSTATE_PHASEFINAL;
   }
}

void AIRProcessor::phase3()
{
   m_nextphase = AIRSTATE_PHASEFINAL;
}
