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

#ifndef SRC_OAI_HSS_FDHSS_FDHSS_H_
#define SRC_OAI_HSS_FDHSS_FDHSS_H_

#ifdef __cplusplus

#include <set>
#include "fd.h"
#include "dataaccess.h"
#include "sthread.h"
#include "stimer.h"
#include "stime.h"
#include "statshss.h"
#include "soss.h"
#include "logger.h"

#include "resthandler.h"

#include "worker.h"

const uint16_t GUARD_TIMEOUT        =    ETM_USER + 1;
const uint16_t HANDLE_MME_RESPONSE  =    ETM_USER + 2;
const uint16_t HANDLE_CIA_SENT      =    ETM_USER + 3;

const int    MME_DOWN               =    100;
const int    IMSI_NOT_ACTIVE        =    101;


namespace s6t      { class Application; class MonitoringEventConfigurationExtractorList; }
namespace s6as6d   { class Application; }
namespace s6c      { class Application; }
namespace Pistache { namespace Http { class Endpoint; } }

class MonitoringConfEventStatus;
class ImsiResult;
class RIRBuilder;
class ImsiStatus;


typedef std::map< std::pair<std::string, uint32_t>, MonitoringConfEventStatus > EvenStatusMap;

typedef std::map< std::pair<std::string, uint32_t>, std::vector<ImsiStatus> > EventImsiStatus;


typedef struct hss_config_s hss_config_t;

class HandleMmeResponseEvtMsg : public SEventThreadMessage
{
public:
   HandleMmeResponseEvtMsg( EvenStatusMap* mme_response, std::string &imsi, int imsi_reachable, std::string &msisdn);
   EvenStatusMap* m_mme_response;
   std::string m_imsi;
   int m_imsi_reachable;
   std::string m_msisdn;

private:
   HandleMmeResponseEvtMsg( );
};

class MonitoringConfEventStatus
{
public:
   MonitoringConfEventStatus(bool is_remove);
   MonitoringConfEventStatus();

   bool isLongTermEvt();
   void forceLongTerm(bool is_long_tem);

   std::string    scef_id;
   uint32_t       scef_ref_id;
   uint32_t       max_nb_reports;
   STime          mon_duration;
   bool           max_nb_reports_set;
   bool           mon_duration_set;
   uint32_t       result;
   bool           is_remove;
   bool           type_set;

private:
   bool           is_long_term;
};

class ImsiStatus
{
public:
   ImsiStatus(std::string& imsi, MonitoringConfEventStatus &status, int reachability, std::string &msisdn){
      m_imsi   = imsi;
      m_status = status;
      m_reachability = reachability;
      m_msisdn = msisdn;
   }

   std::string m_imsi;
   MonitoringConfEventStatus m_status;
   int m_reachability;
   std::string m_msisdn;
};

class ImsiImeiData
{
public:
   ImsiImeiData() : msisdn(0) {}

   std::string imsi;
   int64_t msisdn;
   std::string old_imei;
   std::string old_imei_sv;
   std::string new_imei;
   std::string new_imei_sv;
};

class HSSWorkerQueue : public QueueManager
{
public:
   HSSWorkerQueue();
   ~HSSWorkerQueue();

   void addProcessor(QueueProcessor *processor);
   void startProcessor();
   void finishProcessor();
};

class FDHss {
public:
   FDHss();
   virtual ~FDHss();
   bool initdb(hss_config_t * hss_config_p);
   bool init( hss_config_t * hss_config_p );

   void shutdown();

   void waitForShutdown();

   bool isMmeValid(std::string &mmehost);

   static int s6a_peer_validate ( struct peer_info *info, int *auth, int (**cb2) (struct peer_info *));

   void updateOpcKeys(const uint8_t opP[16]);

   int sendINSDRreq(s6t::MonitoringEventConfigurationExtractorList &cir_monevtcfg, std::string& imsi,
                     FDMessageRequest *cir_req, EvenStatusMap *evt_map, RIRBuilder * rir_builder);

   void sendRIR_ChangeImsiImeiSvAssn(ImsiImeiData &data);

   s6t::Application     *gets6tApp()      { return m_s6tapp; }
   s6as6d::Application  *gets6as6dApp()   { return m_s6aapp; }
   s6c::Application     *gets6cApp()      { return m_s6capp; }
   DataAccess           &getDb()          { return m_dbobj;  }
   WorkerManager        &getWorkMgr()     { return m_wrkmgr; }
   HSSWorkerQueue       &getWorkerQueue() { return m_workerqueue; }

   void buildCfgStatusAvp( FDAvp &mon_evt_cfg_status, MonitoringConfEventStatus& status );

   bool synchFix();

private:
   FDEngine m_diameter;
   s6t::Application *m_s6tapp;
   s6as6d::Application *m_s6aapp;
   s6c::Application *m_s6capp;
   FDPeerList m_mme_peers;
   DataAccess m_dbobj;
   Pistache::Http::Endpoint *m_endpoint;
   OssEndpoint<Logger> *m_ossendpoint;
   WorkerManager m_wrkmgr;
   HSSWorkerQueue m_workerqueue;
};

extern FDHss fdHss;

class RIRBuilder : public SEventThread
{
public :

   RIRBuilder(int nb_ida_proc, EvenStatusMap * hss_insert_status,
              std::string &destination_host, std::string &destination_realm );
   virtual ~RIRBuilder();
   void onInit();
   void onQuit();
   void onTimer( SEventThread::Timer &t );
   void setInterval(long interval) { m_interval = interval; }
   void dispatch( SEventThreadMessage &msg );

private:
   void sendRIR();

   long m_interval;
   int m_nb_ida_proc;
   SEventThread::Timer m_idletimer;

   std::string       m_destination_host;
   std::string       m_destination_realm;

   EvenStatusMap     *m_hss_insert_status;
   EventImsiStatus   imsi_status_map;
   bool m_ciasent;
};

#endif

#endif /* SRC_OAI_HSS_FDHSS_FDHSS_H_ */
