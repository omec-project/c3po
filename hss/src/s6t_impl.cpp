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

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <utility>

#include "common_def.h"
#include "s6t_impl.h"
#include "fdhss.h"
#include "dataaccess.h"
#include <inttypes.h>

#include "fdjson.h"

#include "rapidjson/document.h"
#include "statshss.h"


#define MSISDN_LEN                        10
#define IMSI_LEN                          15

#define  GROUP_CONFIGURATION_IN_PROGRESS  (1U)
#define  ABSENT_SUBSCRIBER                (1U)

namespace s6t {

void fdJsonError( const char *err )
{
   printf("json error: %s", err);
}

void handleGlobalErrorCode(FDMessageAnswer &ans, s6t::Application& m_app,
                           int result_code, bool experimental){

   if (DIAMETER_ERROR_IS_VENDOR (result_code) && experimental) {
        FDAvp experimental_result ( m_app.getDict().avpExperimentalResult() );
        experimental_result.add( m_app.getDict().avpVendorId(),  VENDOR_3GPP);
        experimental_result.add( m_app.getDict().avpExperimentalResultCode(),  result_code);
        ans.add(experimental_result);
   }
   else{
       ans.add( m_app.getDict().avpResultCode(), result_code);
   }
}


// Member functions that customize the individual application
Application::Application( DataAccess &dbobj )
    : ApplicationBase()
    , m_cmd_coir( *this )
    //, m_cmd_reir( *this )
    , m_cmd_niir( *this )
    , m_dbobj(dbobj)
{
    registerHandlers();
}

Application::~Application()
{
}

void Application::registerHandlers()
{
   // Remove the comments for any Command this application should
   // listen for (receive).
   std::cout << "Registering s6t command handlers" << std::endl;
   std::cout << "Registering COIR command handler" << std::endl;
   registerHandler( m_cmd_coir );
   //std::cout << "Registering REIR command handler" << std::endl;
   //registerHandler( m_cmd_reir );
   std::cout << "Registering NIIR command handler" << std::endl;
   registerHandler( m_cmd_niir );
}

// COIR Request (req) Command member functions


// Sends a COIR Request to the corresponding Peer
bool Application::sendCOIRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the COIR Command
    COIRreq *s = createCOIRreq( peer );

    try
    {
         if ( s )
         {
              s->send();
         }
    }
    catch ( FDException &ex )
    {
         std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
         delete s;
         s = NULL;
    }

    // DO NOT free the newly created COIRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for COIR reuqests
COIRreq *Application::createCOIRreq(FDPeer &peer)
{
    //  creates the COIRreq object
    COIRreq *s = new COIRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the COIR request object

    // return the newly created request object
    return s;
}

DataAccess &Application::getDbObj(){
   return m_dbobj;
}

// A handler for Answers corresponding to this specific Request
void COIRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// COIR Command
}

// COIR Command (cmd) member function


int processHssDb(s6t::ConfigurationInformationRequestExtractor& cir,
                  int64_t msisdn,
                  EvenStatusMap *evt_map,
                  Application &m_app){

   ///////////////////////
   // COMON BLOCK
   ///////////////////////

   for ( std::list<MonitoringEventConfigurationExtractor*>::iterator monevt_it = cir.monitoring_event_configuration.getList().begin();
         monevt_it != cir.monitoring_event_configuration.getList().end();
         ++monevt_it )
   {
      //Remove the corresponding ids
      if((*monevt_it)->scef_reference_id_for_deletion.exists()){
         for( std::list<FDExtractorAvp*>::iterator delete_it  = (*monevt_it)->scef_reference_id_for_deletion.getList().begin();
               delete_it != (*monevt_it)->scef_reference_id_for_deletion.getList().end();
               ++delete_it)
         {
            uint32_t scef_ref_id;
            std::string scef_id;
            (*delete_it)->get(scef_ref_id);
            (*monevt_it)->scef_id.get(scef_id);

            bool delete_evt_fail = false;
            bool is_long_evt = true;
            try{
               DAEvent event;
               if(m_app.getDbObj().getEvent(scef_id, scef_ref_id, event) ){
                  m_app.getDbObj().deleteEvent(scef_id, scef_ref_id);
                  //if( !m_app.getDbObj().deleteEvent(scef_id, scef_ref_id ) ){
                  //   delete_evt_fail = true;
                  //}
               }
               else{
                  //the event does not exists
                  is_long_evt = false;
                  delete_evt_fail = true;
               }
            }
            catch(...){
               std::cout << "Error accesing db for evt removal" << std::endl;
               is_long_evt = false;
               delete_evt_fail = true;
            }
            MonitoringConfEventStatus aconf(true);
            aconf.scef_id = scef_id;
            aconf.scef_ref_id = scef_ref_id;
            aconf.forceLongTerm(is_long_evt);

            if(delete_evt_fail){
               aconf.result = DIAMETER_ERROR_CONFIGURATION_EVENT_NON_EXISTANT;
            }
            else{
               aconf.result = DIAMETER_SUCCESS;
            }
            evt_map->insert( std::make_pair(std::make_pair(aconf.scef_id, aconf.scef_ref_id), aconf) );
         }
      }

      if ( (*monevt_it)->scef_reference_id.exists() ) {
         DAEvent acfgevt;

         (*monevt_it)->scef_id.get(acfgevt.scef_id);
         (*monevt_it)->scef_reference_id.get(acfgevt.scef_ref_id);
         acfgevt.msisdn = msisdn;
         cir.user_identifier.external_identifier.get(acfgevt.extid);
         (*monevt_it)->getJson( acfgevt.mec_json );
         cir.user_identifier.getJson( acfgevt.ui_json );

         MonitoringConfEventStatus aconf(false);
         aconf.scef_id =  acfgevt.scef_id;
         aconf.scef_ref_id = acfgevt.scef_ref_id;

         (*monevt_it)->dump();

         uint32_t  max_nb_reports;
         if( (*monevt_it)->maximum_number_of_reports.get( max_nb_reports ) ){
            aconf.max_nb_reports_set = true;
            aconf.max_nb_reports = max_nb_reports;
         }

         STime time;
         if((*monevt_it)->monitoring_duration.get( time )){
            aconf.mon_duration = time;
            aconf.mon_duration_set = true;
         }

         uint32_t monitoring_type;
         if ( (*monevt_it)->monitoring_type.get(monitoring_type) ) {
            acfgevt.monitoring_type = monitoring_type;
         }

         //we only add long term event to the db, the short term
         //are consider as failing and we will wait to mme response
         //for a final resolution.
         bool add_evt_fail = false;
         try{
            if(aconf.isLongTermEvt()){
               if(!m_app.getDbObj().addEvent( acfgevt )){
                  add_evt_fail = true;
               }
            }
            else{
               add_evt_fail = true;
            }
         }
         catch ( DAException &ex ){
            printf( "%s\n", ex.what() );
            add_evt_fail = true;
         }
         catch(...){
            add_evt_fail = true;
         }

         if( aconf.isLongTermEvt() ){
            if(add_evt_fail){
               aconf.result = DIAMETER_ERROR_CONFIGURATION_EVENT_STORAGE_NOT_SUCCESSFUL;
            }
            else {
               aconf.result = DIAMETER_SUCCESS;
            }
         }

         evt_map->insert( std::make_pair(std::make_pair(aconf.scef_id, aconf.scef_ref_id), aconf) );
      }
   }
   return 0;
}


int processSimpleImsi(FDMessageRequest *req,
                       std::string &imsi,
                       int64_t msisdn,
                       s6t::ConfigurationInformationRequestExtractor& cir,
                       Application &m_app){

   //Process the hss db
   EvenStatusMap *hss_db_rst = new EvenStatusMap();
   processHssDb( cir, msisdn, hss_db_rst, m_app );

   //The CIA response will be built on the s6as6d interface once the IDA coming from the MME is
   //received
   int ret = fdHss.sendINSDRreq(cir.monitoring_event_configuration, imsi, req, hss_db_rst, NULL);

   if( ret == MME_DOWN || ret == IMSI_NOT_ACTIVE ) {

      //The idr is not sent, either because the mme is down or the imsi is not attached or the
      //session is inactive

      //Build an answer from here
      FDMessageAnswer ans( req );
      ans.addOrigin();
      ans.add( m_app.getDict().avpAuthSessionState(), 1 );
      ans.add( m_app.getDict().avpResultCode(), ER_DIAMETER_SUCCESS);

      //Build the monitoring-event-config-status including the result of the db operations
      for (EvenStatusMap::iterator it= hss_db_rst->begin(); it!=hss_db_rst->end(); ++it)
      {
         if( it->second.isLongTermEvt() ){
            FDAvp mon_evt_cfg_status( m_app.getDict().avpMonitoringEventConfigStatus() );
            fdHss.buildCfgStatusAvp(mon_evt_cfg_status, it->second);
            ans.add(mon_evt_cfg_status);
         }
         else{
            it->second.result = DIAMETER_UNABLE_TO_COMPLY; //Flag the short term as failed
            FDAvp mon_evt_cfg_status( m_app.getDict().avpMonitoringEventConfigStatus() );
            fdHss.buildCfgStatusAvp(mon_evt_cfg_status, it->second);
            ans.add(mon_evt_cfg_status);
         }
      }

      if(ret == IMSI_NOT_ACTIVE) {
         //Indicate error on mme by setting the S6T-HSS-Cause absent subscriber
         ans.add(m_app.getDict().avpS6tHssCause(), ABSENT_SUBSCRIBER);
      }


ans.dump();

      ans.send();
      delete hss_db_rst;
      delete req;
      StatsHss::singleton().registerStatResult(stat_hss_cir, 0, ER_DIAMETER_SUCCESS);
   }
   return 0;
}

int processMultiImsi(FDMessageRequest *req,
                      DAImsiList &list_imsi,
                      s6t::ConfigurationInformationRequestExtractor& cir,
                      Application &m_app) {


   //For group imsi, a CIA is sent straight away and the results will be reported on the RIR
   FDMessageAnswer ans( req );
   ans.addOrigin();
   ans.add( m_app.getDict().avpAuthSessionState(), 1 );
   ans.add( m_app.getDict().avpResultCode(), ER_DIAMETER_SUCCESS);
   uint32_t flag_cia = 0;
   FLAGS_SET(flag_cia, GROUP_CONFIGURATION_IN_PROGRESS);
   ans.add(m_app.getDict().avpCiaFlags(), flag_cia);


   std::string origin_host;
   cir.origin_host.get(origin_host);

   std::string origin_realm;
   cir.origin_realm.get(origin_realm);


   //Process the hss db
   EvenStatusMap *hss_db_rst = new EvenStatusMap();
   processHssDb( cir, 0, hss_db_rst, m_app );


   RIRBuilder *rir_builder = new RIRBuilder( list_imsi.size() , hss_db_rst, origin_host, origin_realm);

   uint32_t time_guard;
   if( cir.group_reporting_guard_timer.get(time_guard) ){
      rir_builder->setInterval( time_guard * 1000 );
   }

   rir_builder->init( NULL );


   for(std::list<std::string>::iterator it_imsi = list_imsi.begin(); it_imsi != list_imsi.end(); ++it_imsi)
   {
      //we need to build an event thread object, send it here and keep and keep it on
      //the IDRRreq, so it will be available to store the responses.
      fdHss.sendINSDRreq(cir.monitoring_event_configuration, *it_imsi, req, hss_db_rst, rir_builder);
   }

   ans.send();
   delete req;

   //Once the CIA has been sent, we indicate the RIR builder
   rir_builder->postMessage( HANDLE_CIA_SENT );

   StatsHss::singleton().registerStatResult(stat_hss_cir, 0, ER_DIAMETER_SUCCESS);

   return 0;
}


// Function invoked when a COIR Command is received
int COIRcmd::process( FDMessageRequest *req )
{

   std::string    s;
   bool           experimental;
   int            result_code = ER_DIAMETER_SUCCESS;

   uint8_t        msisdn[MSISDN_LEN];
   char           msisdnchar[MSISDN_LEN + 1];
   int64_t        msisdn64;

   DAImsiList list_imsi;

req->dump();

   s6t::ConfigurationInformationRequestExtractor cir( *req, m_app.getDict() );

   do{

      try{
         size_t amsisdn_size = sizeof(msisdn);
         if(cir.user_identifier.msisdn.get(msisdn, amsisdn_size))
         {
            // SINGLE IMSI
            FDUtility::tbcd2str( msisdn, amsisdn_size, msisdnchar, MSISDN_LEN+1 );
            //Single ue scenario
            msisdn64 = std::stoll(msisdnchar);
            std::string imsi;
            if(!m_app.getDbObj().getImsiFromMsisdn( msisdn64, imsi ))
            {
               experimental = true;
               result_code = DIAMETER_ERROR_USER_UNKNOWN;
               break;
            }
            return processSimpleImsi(req, imsi, msisdn64, cir, m_app);

         }
         else if (cir.user_identifier.external_identifier.get(s))
         {
            if( !m_app.getDbObj().checkExtIdExists( (char*)s.c_str() ) ) {
               experimental = true;
               result_code = DIAMETER_ERROR_USER_UNKNOWN;
               break;
            }

            m_app.getDbObj().getImsiListFromExtId((char*)s.c_str(), list_imsi);
            return processMultiImsi(req, list_imsi, cir, m_app);
         }
         else{
            std::cout << "Neither msisdn nor external identifier are specified: DIAMETER_ERROR_USER_UNKNOWN" << std::endl;
            experimental = true;
            result_code = DIAMETER_ERROR_USER_UNKNOWN;
            break;
         }
      }
      catch(...){
         std::cout << "Exception while processing cir users: DIAMETER_ERROR_USER_UNKNOWN" << std::endl;
         experimental = true;
         result_code = DIAMETER_ERROR_USER_UNKNOWN;
         break;
      }
   } while (false);

   //Handle global errors on CIA
   FDMessageAnswer ans( req );
   ans.addOrigin();
   ans.add( m_app.getDict().avpAuthSessionState(), 1 );
   handleGlobalErrorCode( ans, m_app, result_code, experimental);

ans.dump();

   ans.send();
   delete req;

   if(experimental){
      StatsHss::singleton().registerStatResult(stat_hss_cir, VENDOR_3GPP, result_code);
   }
   else{
      StatsHss::singleton().registerStatResult(stat_hss_cir, 0, result_code);
   }
   return 0;
}
 
// REIR Request (req) Command member functions


// Sends a REIR Request to the corresponding Peer
bool Application::sendREIRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the REIR Command
    REIRreq *s = createREIRreq( peer );

    try
    {
         if ( s )
         {
              s->send();
         }
    }
    catch ( FDException &ex )
    {
         std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
         delete s;
         s = NULL;
    }

    // DO NOT free the newly created REIRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for REIR reuqests
REIRreq *Application::createREIRreq(FDPeer &peer)
{
    //  creates the REIRreq object
    REIRreq *s = new REIRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the REIR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void REIRreq::processAnswer( FDMessageAnswer &ans )
{
   ans.dump();
   ReportingInformationAnswerExtractor ria (ans, getApplication().getDict());

   uint32_t vendor_code = 0;
   uint32_t ria_result_code = 0;
   //check the global status from the IDA response
   if(ria.result_code.get(ria_result_code)){
      StatsHss::singleton().registerStatResult(stat_hss_rir, 0, ria_result_code);
   }
   else{
      ria.experimental_result.vendor_id.get(vendor_code);
      ria.experimental_result.experimental_result_code.get(ria_result_code);
      StatsHss::singleton().registerStatResult(stat_hss_idr, vendor_code, ria_result_code);
   }
}

// REIR Command (cmd) member function

// Function invoked when a REIR Command is received
int REIRcmd::process( FDMessageRequest *req )
{
// TODO - This code must be implemented IF the application
// receives the REIR command.
    return -1;
}
 
// NIIR Request (req) Command member functions


// Sends a NIIR Request to the corresponding Peer
bool Application::sendNIIRreq(FDPeer &peer)
{
    //TODO - This code may be modified based on specific
    //         processing needs to send the NIIR Command
    NIIRreq *s = createNIIRreq( peer );

    try
    {
         if ( s )
         {
              s->send();
         }
    }
    catch ( FDException &ex )
    {
         std::cout << SUtility::currentTime() << " - EXCEPTION - " << ex.what() << std::endl;
         delete s;
         s = NULL;
    }

    // DO NOT free the newly created NIIRreq object!!
    // It will be deleted by the framework after the
    // answer is received and processed.
    return s != NULL;
}

// A factory for NIIR reuqests
NIIRreq *Application::createNIIRreq(FDPeer &peer)
{
    //  creates the NIIRreq object
    NIIRreq *s = new NIIRreq( *this );

    //TODO - Code must be added to correctly
    //         populate the NIIR request object

    // return the newly created request object
    return s;
}

// A handler for Answers corresponding to this specific Request
void NIIRreq::processAnswer( FDMessageAnswer &ans )
{
// TODO - This code must be implemented IF the application
// receives Answers for this command, i.e. it sends the
// NIIR Command
}

// Function to check if the APN is subscribed for a given IMSI

bool checkAPNSubscribed( const char *imsi, std::string &apn, Application &m_app)
{
    std::string subDataFromDB,apnFromDB;
    if( m_app.getDbObj().getSubDataFromImsi(imsi, subDataFromDB) )
    {
        if( fdJsonGetApnValueFromSubData(subDataFromDB, apnFromDB ) )
        {
            if ( apn.compare( apnFromDB ) == 0 )
            {
                return true;
            }
            std::cout << "APN is not subscribed for the provided imsi" << std::endl;
            return false;
        }
        std::cout << "APN field is not present in the subscription data" << std::endl;
        return false;
    }
    std::cout << "Subscription_data is not present for the provided imsi" << std::endl;
    return false;
}

// NIIR Command (cmd) member function

// Function invoked when a NIIR Command is received
int NIIRcmd::process( FDMessageRequest *req )
{
    std::string      s, reqValidTime, origHost, origRealm;
    uint8_t          msisdn[MSISDN_LEN];
    char             msisdnchar[MSISDN_LEN + 1];
    uint8_t          imsi[IMSI_LEN];
    char             imsichar[IMSI_LEN + 1];
    std::string      apn;
    bool             experimental =  false;
    //STime            reqValidTime;
    int              result_code = DIAMETER_SUCCESS;

    req->dump();
    s6t::NiddInformationRequestExtractor nir( *req, m_app.getDict() );
    size_t msisdn_size = sizeof( msisdn );
    size_t imsi_size = sizeof( imsi );

    //Create answer associated with the NIIR command
    FDMessageAnswer ans( req );
    ans.addOrigin();
    ans.add( m_app.getDict().avpAuthSessionState(), 1 );
    ans.add( m_app.getDict().avpResultCode(), ER_DIAMETER_SUCCESS);

    if( nir.user_identifier.user_name.get( imsi, imsi_size ) )
    {
        FDUtility::tbcd2str( imsi, imsi_size, imsichar, IMSI_LEN+1 );
        std::string msisdnFromDB,subDataFromDB, apnFromDB;
        DAExtIdList extids;

        if ( nir.nidd_authorization_request.service_selection.get( apn ) )
        {
            if ( !checkAPNSubscribed( imsichar, apn, m_app ) )
            {
                   experimental = true;
                result_code = DIAMETER_ERROR_USER_NO_APN_SUBSCRIPTION;
                //goto output;
            }
        }

        if( m_app.getDbObj().getMsisdnFromImsi( imsichar, msisdnFromDB ) &&
            m_app.getDbObj().getExtIdsFromImsi( imsichar, extids, NULL, NULL ) )
        {
             //include the MSISDN and the appropriate External Identifier assigned to the IMSI
             //in the NIDD-Authorization-Response.

            FDAvp ga( m_app.getDict().avpNiddAuthorizationResponse() );
            uint8_t msisdntbcd[MSISDN_LEN];
            FDUtility::str2tbcd( msisdnFromDB.c_str(), msisdntbcd, msisdn_size );
            ga.add( m_app.getDict().avpMsisdn(), msisdntbcd, msisdn_size );

            for( DAExtIdList::iterator it = extids.begin() ; it != extids.end(); it++ )
            {
                   ga.add( m_app.getDict().avpExternalIdentifier(), *it);
            }

            ans.add( ga );

        }

        if ( nir.nidd_authorization_request.requested_validity_time.get( reqValidTime ) )
        {
            m_app.getDbObj().UpdateValidityTime( imsichar, reqValidTime );

            //Store the origin host and realm of NIR in DB, so that in future
            //When the need to update/revoke a stored granted NIDD Authorization is detected in the HSS,
            //and the feature "NIDD Authorization Update" is commonly supported by the HSS and the SCEF,
            //the HSS shall issue an NIDD-Information Request command containing an NIDD-Authorization-Update AVP
            //towards the SCEF using this origin host and realm as destination host and realm for NIR.
            //TBD - Detection of the above mentioned method in HSS

            if( nir.origin_host.get( origHost ) && nir.origin_realm.get( origRealm ) )
            {
                m_app.getDbObj().UpdateNIRDestination( imsichar, origHost, origRealm );
            }
        }

    }

    else if ( nir.user_identifier.msisdn.get( msisdn, msisdn_size ) )
    {
        FDUtility::tbcd2str( msisdn, msisdn_size, msisdnchar, MSISDN_LEN+1 );
        std::string imsiFromDB;

        if( m_app.getDbObj().getImsiFromMsisdn( msisdnchar, imsiFromDB ) )
        {
            DAExtIdList extids;
            if ( nir.nidd_authorization_request.service_selection.get( apn ) )
            {
                if ( !checkAPNSubscribed( imsiFromDB.c_str(), apn, m_app ) )
                {
                    experimental = true;
                    result_code = DIAMETER_ERROR_USER_NO_APN_SUBSCRIPTION;
                    //goto output;
                }
            }
            //include the IMSI and if available, the appropriate External Identifier associated
            //with the MSISDN in the NIDD-Authorization-Response.

            FDAvp ga( m_app.getDict().avpNiddAuthorizationResponse() );
            ga.add( m_app.getDict().avpUserName(), imsiFromDB );

            if( m_app.getDbObj().getExtIdsFromImsi( imsiFromDB, extids, NULL, NULL ) )
            {
                for( DAExtIdList::iterator it = extids.begin() ; it != extids.end(); it++ )
                {
                      ga.add( m_app.getDict().avpExternalIdentifier(), *it);
                }
            }

            ans.add( ga );

            if ( nir.nidd_authorization_request.requested_validity_time.get( reqValidTime ) )
            {
                m_app.getDbObj().UpdateValidityTime( imsiFromDB, reqValidTime );

                //Store the origin host and realm of NIR in DB, so that in future
                //When the need to update/revoke a stored granted NIDD Authorization is detected in the HSS,
                //and the feature "NIDD Authorization Update" is commonly supported by the HSS and the SCEF,
                //the HSS shall issue an NIDD-Information Request command containing an NIDD-Authorization-Update AVP
                //towards the SCEF using this origin host and realm as destination host and realm for NIR.
                //TBD - Detection of the above mentioned method in HSS

                if( nir.origin_host.get( origHost ) && nir.origin_realm.get( origRealm ) )
                {
                    m_app.getDbObj().UpdateNIRDestination( imsichar, origHost, origRealm );
                }
            }

        }

    }

    else if ( nir.user_identifier.external_identifier.get(s) )
    {
        DAImsiList imsilst;
        std::list<std::string> msisdnlst;
        std::string msisdnFromDB;
        if ( m_app.getDbObj().getImsiListFromExtId( s, imsilst ) )
        {
            FDAvp ga( m_app.getDict().avpNiddAuthorizationResponse() );

            if( nir.nidd_authorization_request.service_selection.get( apn ) )
            {
                   for( std::list<std::string>::iterator it = imsilst.begin() ; it != imsilst.end(); it++ )
                   {
                         if ( !checkAPNSubscribed( (*it).c_str(), apn, m_app ) )
                         {
                               experimental = true;
                               result_code = DIAMETER_ERROR_USER_NO_APN_SUBSCRIPTION;
                               //goto output;
                           }
                    }

            }
            //include the IMSI and if available the MSISDN associated with the appropriate
            //External Identifier in the NIDD-Authorization-Response

            for ( std::list<std::string>::iterator it = imsilst.begin() ; it != imsilst.end(); it++ )
            {
                ga.add( m_app.getDict().avpUserName(), *it );
                if ( m_app.getDbObj().getMsisdnFromImsi( *it, msisdnFromDB ) )
                {
                      uint8_t msisdntbcd[MSISDN_LEN];
                      FDUtility::str2tbcd( msisdnFromDB.c_str(), msisdntbcd, msisdn_size );
                      ga.add( m_app.getDict().avpMsisdn(), msisdntbcd, msisdn_size );
                      //msisdnlst.push_back(msisdnFromDB);
                  }

                if ( nir.nidd_authorization_request.requested_validity_time.get( reqValidTime ) )
                {
                    //For now, we are inserting reqValidTime as a string into the DB
                    //since we can't insert future time in a column of type timestamp
                    //TODO - We can use ttl option while updating reqValidTime using
                    //some cassandra builtin functions ( available with Cassandra2.2 and later )

                    m_app.getDbObj().UpdateValidityTime( *it, reqValidTime );

                    //Store the origin host and realm of NIR in DB, so that in future
                    //When the need to update/revoke a stored granted NIDD Authorization is detected in the HSS,
                    //and the feature "NIDD Authorization Update" is commonly supported by the HSS and the SCEF,
                    //the HSS shall issue an NIDD-Information Request command containing an NIDD-Authorization-Update AVP
                    //towards the SCEF using this origin host and realm as destination host and realm for NIR.
                    //TBD - Detection of the above mentioned method in HSS

                    if( nir.origin_host.get( origHost ) && nir.origin_realm.get( origRealm ) )
                    {
                        m_app.getDbObj().UpdateNIRDestination( imsichar, origHost, origRealm );
                    }
                }
            }

        }
    }

   else
   {
        std::cout << "****** User Identifier is not present in HSS *****" << std::endl;
        experimental = true;
        result_code = DIAMETER_ERROR_USER_UNKNOWN;

   }

    handleGlobalErrorCode( ans, m_app, result_code, experimental);
    ans.dump();
    ans.send();
    delete req;

    if(experimental){
       StatsHss::singleton().registerStatResult(stat_hss_nir, VENDOR_3GPP, result_code);
    }
    else{
       StatsHss::singleton().registerStatResult(stat_hss_nir, 0, result_code);
    }

    return 0;

}
 

}



