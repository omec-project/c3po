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

#ifndef __S6AS6D_IMPL_H
#define __S6AS6D_IMPL_H

#include "s6as6d.h"
#include "fdhss.h"
#include "worker.h"

extern "C" {
#include "hss_config.h"
#include "access_restriction.h"
#include "aucpp.h"
#include <inttypes.h>
}

#define AUTH_MAX_EUTRAN_VECTORS 6

class DataAccess;

namespace s6t    { class MonitoringEventConfigurationExtractorList; }

namespace s6as6d {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend UPLRreq;
    friend CALRreq;
    friend AUIRreq;
    friend INSDRreq;
    friend DESDRreq;
    friend PUURreq;
    friend RERreq;

public:
    Application( DataAccess &dbobj );
    ~Application();

    UPLRcmd &getUPLRcmd() { return m_cmd_uplr; }
    //CALRcmd &getCALRcmd() { return m_cmd_calr; }
    AUIRcmd &getAUIRcmd() { return m_cmd_auir; }
    //INSDRcmd &getINSDRcmd() { return m_cmd_insdr; }
    //DESDRcmd &getDESDRcmd() { return m_cmd_desdr; }
    PUURcmd &getPUURcmd() { return m_cmd_puur; }
    //RERcmd &getRERcmd() { return m_cmd_rer; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendUPLRreq(FDPeer &peer);
    bool sendCALRreq(FDPeer &peer);
    bool sendAUIRreq(FDPeer &peer);
    int  sendINSDRreq(s6t::MonitoringEventConfigurationExtractorList &cir_monevtcfg,
                      std::string& imsi, FDMessageRequest *cir_req, EvenStatusMap *evt_map,
                      RIRBuilder * rir_builder);
    bool sendDESDRreq(FDPeer &peer);
    bool sendPUURreq(FDPeer &peer);
    bool sendRERreq(FDPeer &peer);

    DataAccess &dataaccess() { return m_dbobj; }

private:
    void registerHandlers();
    UPLRcmd m_cmd_uplr;
    //CALRcmd m_cmd_calr;
    AUIRcmd m_cmd_auir;
    //INSDRcmd m_cmd_insdr;
    //DESDRcmd m_cmd_desdr;
    PUURcmd m_cmd_puur;
    //RERcmd m_cmd_rer;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    UPLRreq *createUPLRreq(FDPeer &peer);
    CALRreq *createCALRreq(FDPeer &peer);
    AUIRreq *createAUIRreq(FDPeer &peer);
    INSDRreq *createINSDRreq(s6t::MonitoringEventConfigurationExtractorList &cir_monevtcfg,
                            std::string& imsi, FDMessageRequest *cir_req, EvenStatusMap *evt_map,
                            DAImsiInfo &imsi_info, RIRBuilder * rir_builder);
    DESDRreq *createDESDRreq(FDPeer &peer);
    PUURreq *createPUURreq(FDPeer &peer);
    RERreq *createRERreq(FDPeer &peer);

    DataAccess &m_dbobj;
};

class IDRRreq : public INSDRreq {

public:
   IDRRreq(Application &app, FDMessageRequest *cir_req,
          EvenStatusMap *evt_map, RIRBuilder *rirbuilder, std::string &imsi, std::string &msisdn);

   void processAnswer( FDMessageAnswer &ans );

private:
   FDMessageRequest *cir_req;
   EvenStatusMap *evt_map;
   RIRBuilder *m_rirbuilder;
   std::string m_imsi;
   std::string m_msisdn;
};

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define DB_OP_COMPLETE_EXECUTED(__executed,__item)       \
{                                                        \
   atomic_or_fetch(__executed, __item);                  \
}

#define DB_OP_COMPLETE_RESULT(__result,__item,__success) \
{                                                        \
   if (__success)                                        \
      atomic_or_fetch(__result, __item);                 \
   else                                                  \
      atomic_and_fetch(__result, !__item);               \
}

#define DB_OP_COMPLETE(_item,_executed,_result,_success) \
{                                                        \
   DB_OP_COMPLETE_RESULT(_result,_item,_success)         \
   DB_OP_COMPLETE_EXECUTED(_executed,_item)              \
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class DatabaseAction
{
public:
   DatabaseAction(uint32_t action)
      : m_action( action )
   {
   }

   virtual ~DatabaseAction()
   {
   }

   uint16_t getAction()
   {
      return m_action;
   }

private:
   DatabaseAction();
   uint32_t m_action;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define ULRSTATE_BASE         (WORKER_EVENT + 100)
#define ULRSTATE_PHASEFINAL   (ULRSTATE_BASE + 0)
#define ULRSTATE_PHASE1       (ULRSTATE_BASE + 1)
#define ULRSTATE_PHASE2       (ULRSTATE_BASE + 2)
#define ULRSTATE_PHASE3       (ULRSTATE_BASE + 3)
#define ULRSTATE_PHASE4       (ULRSTATE_BASE + 4)
#define ULRSTATE_PHASE5       (ULRSTATE_BASE + 5)

#define ULRDB_GET_IMSI_INFO         0x00000001
#define ULRDB_GET_EXT_IDS           0x00000002
#define ULRDB_GET_EVNTIDS_MSISDN    0x00000004
#define ULRDB_GET_EVNTIDS_EXTIDS    0x00000008
#define ULRDB_GET_EVNTS_EVNTIDS     0x00000010
#define ULRDB_GET_MMEID_HOST        0x00000020
#define ULRDB_UPDATE_IMSI           0x00000040

class ULRProcessor : public QueueProcessor
{
public:
   ULRProcessor(FDMessageRequest &req, s6as6d::Application &app, s6as6d::Dictionary &dict);
   virtual ~ULRProcessor();

   bool phaseReady(int phase, uint32_t adjustment=0);
   void triggerNextPhase();
   static void processNextPhase(ULRProcessor *pthis);

   void phase1();
   void phase2();
   void phase3();
   void phase4();
   void phase5();

   int getNextPhase() { return m_nextphase; }

   std::string &getImsi() { return m_imsi; }

private:
   static void on_ulr_callback(CassFuture *f, void *data);

   void getEvents();

   void getImsiInfo(SCassFuture &future);
   void getExternalIds(SCassFuture &future);
   void getEventIdsMsisdn(SCassFuture &future);
   void getEventIdsExternalIds(SCassFuture &future);
   void getEvents(SCassFuture &future);
   void getMmeIdentity(SCassFuture &future);
   void updateImsiInfo(SCassFuture &future);

   s6as6d::UpdateLocationRequestExtractor m_ulr;
   SMutex               m_mutex;
   SMutex               m_lstmutex;
   FDMessageAnswer      m_ans;
   s6as6d::Application &m_app;
   s6as6d::Dictionary  &m_dict;
   long                 m_perf_timer;
   std::string          m_imsi;
   DAImsiInfo           m_orig_info;
   DAImsiInfo           m_new_info;
   uint32_t             m_present_flags;
   uint8_t              m_plmn_id[4];
   size_t               m_plmn_len;
   DAExtIdList          m_extIdLst;
   DAEventIdList        m_evtIdLst;
   DAEventList          m_evtLst;
   unsigned long        m_3count;
   bool                 m_3aSuccess;
   bool                 m_3bSuccess;
   int                  m_mmeidentity;
   uint32_t             m_ulrflags;

   int                  m_nextphase;
   uint32_t             m_msgissued;
   uint32_t             m_dbexecuted;  // bit mask that shows which queries are complete
   uint32_t             m_dbresult;    // query result bit mask
   uint32_t             m_dbissued;    // # of queries in flight
   uint32_t             m_dbevtissued; // # of event queries in flight
};

////////////////////////////////////////////////////////////////////////////////

class ULRStateProcessor : public WorkProcessor
{
public:
   ULRStateProcessor(uint16_t state, ULRProcessor *ulrproc);
   virtual ~ULRStateProcessor();

   void process();

   uint16_t getState() { return m_state; }
   ULRProcessor *getProcessor() { return m_processor; }

private:
   uint16_t m_state;
   ULRProcessor *m_processor;
};

////////////////////////////////////////////////////////////////////////////////

class ULRDatabaseAction : public DatabaseAction
{
public:
   ULRDatabaseAction(uint16_t action, ULRProcessor &ulrproc)
      : DatabaseAction( action ),
        m_ulrproc( ulrproc )
   {
   }

   virtual ~ULRDatabaseAction()
   {
   }

   ULRProcessor &getProcessor()
   {
      return m_ulrproc;
   }

private:
   ULRProcessor &m_ulrproc;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AIRSTATE_BASE         (WORKER_EVENT + 200)
#define AIRSTATE_PHASEFINAL   (AIRSTATE_BASE + 0)
#define AIRSTATE_PHASE1       (AIRSTATE_BASE + 1)
#define AIRSTATE_PHASE2       (AIRSTATE_BASE + 2)
#define AIRSTATE_PHASE3       (AIRSTATE_BASE + 3)

#define AIRDB_GET_IMSI_SEC       0x00000001
#define AIRDB_UPDATE_IMSI        0x00000002

class AIRProcessor : public QueueProcessor
{
public:
   AIRProcessor(FDMessageRequest& req, s6as6d::Application &app, s6as6d::Dictionary &dict);
   virtual ~AIRProcessor();

   bool phaseReady(int phase, uint32_t adjustment=0);
   void triggerNextPhase();
   static void processNextPhase(AIRProcessor *pthis);

   void phase1();
   void phase2();
   void phase3();

   int getNextPhase() { return m_nextphase; }

private:
   static void on_air_callback(CassFuture *f, void *data);

   void getImsiSec(SCassFuture &future);
   void updateImsi(SCassFuture &future);

   s6as6d::AuthenticationInformationRequestExtractor m_air;
   SMutex               m_mutex;
   FDMessageAnswer      m_ans;
   s6as6d::Application &m_app;
   s6as6d::Dictionary  &m_dict;
   DAImsiSec            m_sec;
   std::string          m_imsi;
   uint64_t             m_uimsi;
   auc_vector_t         m_vector[AUTH_MAX_EUTRAN_VECTORS];
   uint32_t             m_num_vectors = 0;
   uint8_t              m_plmn_id[4];
   size_t               m_plmn_len;
   uint8_t              m_auts[31];
   size_t               m_auts_len;
   bool                 m_auts_set;

   int                  m_nextphase;
   uint32_t             m_msgissued;
   uint32_t             m_dbexecuted;  // bit mask that shows which queries are complete
   uint32_t             m_dbresult;    // query result bit mask
   uint32_t             m_dbissued;    // # of queries in flight
   uint32_t             m_dbevtissued; // # of event queries in flight
};

////////////////////////////////////////////////////////////////////////////////

class AIRStateProcessor : public WorkProcessor
{
public:
   AIRStateProcessor(uint16_t state, AIRProcessor *ulrproc);
   virtual ~AIRStateProcessor();

   void process();

   uint16_t getState() { return m_state; }
   AIRProcessor *getProcessor() { return m_processor; }

private:
   uint16_t m_state;
   AIRProcessor *m_processor;
};

////////////////////////////////////////////////////////////////////////////////

class AIRDatabaseAction : public DatabaseAction
{
public:
   AIRDatabaseAction(uint16_t action, AIRProcessor &airproc)
      : DatabaseAction( action ),
        m_airproc( airproc )
   {
   }

   virtual ~AIRDatabaseAction()
   {
   }

   AIRProcessor &getProcessor()
   {
      return m_airproc;
   }

private:
   AIRProcessor &m_airproc;
};

#endif // __S6AS6D_IMPL_H
