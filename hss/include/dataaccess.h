/*
* Copyright (c) 2021  Great Software Laboratory Pvt. Ltd
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __DATAACCESS_H_
#define __DATAACCESS_H_

#include <stdexcept>
#include <list>
#include <string>

#include "scassandra.h"

#define MME_IDENTITY_PRESENT              (1U)
#define MME_SUPPORTED_FEATURES_PRESENT    (1U << 1)
#define IMEI_PRESENT                      (1U << 2)
#define SV_PRESENT                        (1U << 3)
#define UE_SRVCC_PRESENT                  (1U << 4)

#define KEY_LENGTH  (16)
#define SQN_LENGTH  (6)
#define RAND_LENGTH (16)
#define OPC_LENGTH (16)


#define ASCII_TO_BINARY(c) \
( \
    c >= '0' && c <= '9' ? c - '0' : \
    c >= 'a' && c <= 'f' ? c - 'a' + 10 : \
    c >= 'A' && c <= 'F' ? c - 'A' + 10 : 0 \
)


#define GET_EVENT_DATA( _row, _col, _dest ) \
{ \
    SCassValue val = _row.getColumn( #_col ); \
    if ( !val.isNull() && !val.get( _dest ) ) \
        throw DAException( \
                SUtility::string_format("DataAccess::%s - ERROR - Error %d getting [%s]", \
                        __func__, future.errorCode(), #_col ) \
        ); \
}

void convert_ascii_to_binary(unsigned char *dest, unsigned char *src, int length);

class DAException : public std::runtime_error
{
public:
    DAException(const char *m) : std::runtime_error(m) {}
    DAException(const std::string &m) : std::runtime_error(m) {}
};

class DAImsiList : public std::list<std::string>
{
public:
    DAImsiList() {}
    ~DAImsiList() {}
};

class DAExtIdList : public std::list<std::string>
{
public:
    DAExtIdList() {}
    ~DAExtIdList() {}
};

class DAEvent
{
public:
    DAEvent()
    {
        init();
    }

    void init()
    {
        scef_id.clear();
        scef_ref_id = 0;
        msisdn = 0;
        extid.clear();
        ui_json.clear();
        mec_json.clear();
    }

    std::string scef_id;
    uint32_t scef_ref_id;
    int64_t msisdn;
    std::string extid;
    std::string ui_json;
    std::string mec_json;
    int32_t monitoring_type;
};

class DAEventList : public std::list<DAEvent*>
{
public:
    DAEventList();
    ~DAEventList();
};

struct DAEventId
{
    std::string scef_id;
    uint32_t scef_ref_id;
};

struct DAMmeIdentity
{
    std::string mme_host;
    std::string mme_realm;
    std::string mme_isdn;
};

class DAEventIdList : public std::list<DAEventId*>
{
public:
    DAEventIdList();
    ~DAEventIdList();

    static bool compare(DAEventId* l, DAEventId* r)
    {
        if (l->scef_id < r->scef_id)
            return true;
        if (l->scef_id > r->scef_id)
            return false;
        return l->scef_ref_id < r->scef_ref_id;
    }
};

class ImsiEntity
{
public:
    ImsiEntity()
    {
        imsi = "";                        imsi_pres = false;
        idmmeidentity = 0;                idmmeidentity_pres = false;
        imei = "";                        imei_pres = false;
        imei_sv = "";                     imei_sv_pres = false;
        key = "";                         key_pres = false;
        lipa_permissions = "";            lipa_permissions_pres = false;
        mmeidentity_idmmeidentity = 0;    mmeidentity_idmmeidentity_pres = false;
        mme_cap = 0;                      mme_cap_pres = false;
        mmehost = "";                     mmehost_pres = false;
        mmerealm = "";                    mmerealm_pres = false;
        ms_ps_status = "";                ms_ps_status_pres = false;
        msisdn = 0;                       msisdn_pres = false;
        niddvalidity = "";                niddvalidity_pres = false;
        nir_dest_host = "";               nir_dest_host_pres = false;
        nir_dest_realm = "";              nir_dest_realm_pres = false;
        opc = "";                         opc_pres = false;
        pgw_id = 0;                       pgw_id_pres = false;
        rand = "";                        rand_pres = false;
        rfsp_index = 0;                   rfsp_index_pres = false;
        sqn = 0;                          sqn_pres = false;
        subscription_data = "";           subscription_data_pres = false;
        supported_features = "";          supported_features_pres = false;
        ue_reachability = 0;              ue_reachability_pres = false;
        urrp_mme = 0;                     urrp_mme_pres = false;
        user_identifier = "";             user_identifier_pres = false;
        visited_plmnid = "";              visited_plmnid_pres = false;
        access_restriction = 0;           access_restriction_pres = false;
    }
    std::string imsi;                     bool imsi_pres;
    int32_t access_restriction;           bool access_restriction_pres;
    int32_t idmmeidentity;                bool idmmeidentity_pres;
    std::string imei;                     bool imei_pres;
    std::string imei_sv;                  bool imei_sv_pres;
    std::string key;                      bool key_pres;
    std::string lipa_permissions;         bool lipa_permissions_pres;
    int32_t mme_cap;                      bool mme_cap_pres;
    std::string mmehost;                  bool mmehost_pres;
    int32_t mmeidentity_idmmeidentity;    bool mmeidentity_idmmeidentity_pres;
    std::string mmerealm;                 bool mmerealm_pres;
    std::string ms_ps_status;             bool ms_ps_status_pres;
    int64_t msisdn;                       bool msisdn_pres;
    std::string niddvalidity;             bool niddvalidity_pres;
    std::string nir_dest_host;            bool nir_dest_host_pres;
    std::string nir_dest_realm;           bool nir_dest_realm_pres;
    std::string opc;                      bool opc_pres;
    int32_t pgw_id;                       bool pgw_id_pres;
    std::string rand;                     bool rand_pres;
    int32_t rfsp_index;                   bool rfsp_index_pres;
    int32_t sqn;                          bool sqn_pres;
    std::string subscription_data;        bool subscription_data_pres;
    std::string supported_features;       bool supported_features_pres;
    int32_t ue_reachability;              bool ue_reachability_pres;
    int32_t urrp_mme;                     bool urrp_mme_pres;
    std::string user_identifier;          bool user_identifier_pres;
    std::string visited_plmnid;           bool visited_plmnid_pres;
};


struct DAImsiInfo 
{
    std::string imsi;
    std::string mmehost;
    std::string mmerealm;
    std::string ms_ps_status;
    std::string subscription_data;
    std::string supported_features;
    int64_t msisdn = 0;
    std::string str_msisdn;
    std::string visited_plmnid;
    int32_t access_restriction = 0;
    std::string imei;
    std::string imei_sv;
    int32_t mme_id = 0;
};

struct DAImsiSec 
{
    uint8_t key[KEY_LENGTH];
    uint8_t sqn[SQN_LENGTH];
    uint8_t rand[RAND_LENGTH];
    uint8_t opc[OPC_LENGTH];
};

typedef void(*DataAccessCallback)(void);

class DataAccess
{
public:
    DataAccess(){}
    virtual ~DataAccess(){}

    static DataAccess *createDataAccessObj(std::string dbms_type);

    //std::string &host( const char *hst ) { return m_db.host(hst); }
    //std::string &host( const std::string &hst ) { return m_db.host(hst); }
    //std::string &host() { return m_db.host(); }

    //std::string &keyspace( const char *ks ) { return m_db.keyspace(ks); }
    //std::string &keyspace( const std::string &ks ) { return m_db.keyspace(ks); }
    //std::string &keyspace() { return m_db.keyspace(); }

    //int protocolVersion( int pv ) { return m_db.protocolVersion(pv); }
    //int protocolVersion() { return m_db.protocolVersion(); }

    //void connect() {}

    virtual void connect( const std::string &hst, const std::string &ks = "vhss" ) {}
    virtual void connect( const char *hst, const char *ks = "vhss" ) {}
    virtual void connect( const char *hst, int port, const char *ks = "vhss" ) {}

    virtual void disconnect() {}

    virtual bool addEvent( DAEvent &event ) {}

    virtual bool getEvent( const char *scef_id, uint32_t scef_ref_id, DAEvent &event ) {}
    bool getEvent( const std::string &scef_id, uint32_t scef_ref_id, DAEvent &event ) 
    { 
        return getEvent( scef_id.c_str(), scef_ref_id, event ); 
    }

    virtual bool getEvents( const char *scef_id, std::list<uint32_t> scef_ref_ids, DAEventList &events, DataAccessCallback cb,
            void *data ) {}
    virtual bool getEventsData( SCassFuture &future, DAEventList &events ) {}

    virtual void deleteEvent( const char *scef_id, uint32_t scef_ref_id ) {}
    void deleteEvent( const std::string &scef_id, uint32_t scef_ref_id ) { deleteEvent( scef_id.c_str(), scef_ref_id ); }

    virtual bool checkMSISDNExists( int64_t msisdn) {}

    virtual bool checkImsiExists( const char *imsi ){}
    bool checkImsiExists( const std::string &imsi ) { return checkImsiExists( imsi.c_str() ); }

    virtual bool checkExtIdExists( const char *extid ){}
    bool checkExtIdExists( const std::string &extid ) { return checkExtIdExists( extid.c_str() ); }

    virtual bool getImsiListFromExtId( const char *extid, DAImsiList &imsilst){}
    bool getImsiListFromExtId( std::string &extid, DAImsiList &imsilst) { return getImsiListFromExtId( extid.c_str(), imsilst ); }

    virtual bool getExtIdsFromImsiData( SCassFuture &future, DAExtIdList &extids ){}
    virtual bool getExtIdsFromImsi( const char *imsi, DAExtIdList &extids, DataAccessCallback cb, void *data ){}
    bool getExtIdsFromImsi( const std::string &imsi, DAExtIdList &extids, DataAccessCallback cb, void *data ) 
    { 
        return getExtIdsFromImsi( imsi.c_str(), extids, cb, data ); 
    }

    virtual bool getImsiFromMsisdn( int64_t msisdn, std::string &imsi ){}
    virtual bool getImsiFromMsisdn( const char *msisdn, std::string &imsi ){}
    bool getImsiFromMsisdn( const std::string &msisdn, std::string &imsi ) { return getImsiFromMsisdn( msisdn.c_str(), imsi ); }

    virtual bool getMsisdnFromImsi( const char *imsi, std::string &msisdn ){}
    bool getMsisdnFromImsi( const std::string &imsi, std::string &msisdn ) { return getMsisdnFromImsi( imsi.c_str(), msisdn ); }
    virtual bool getMsisdnFromImsi( const char *imsi, int64_t &msisdn ){}

    virtual bool executeQueryUsingPipe ( const char *imsi, DAImsiInfo &info, DAExtIdList &extidl, DAEventIdList &eidl,
            DAEventList &el, std::string& host, int32_t &mmeid, int64_t &msisdn){}

    virtual bool getImsiInfoData( SCassFuture &future, DAImsiInfo &info ){}
    virtual bool getImsiInfo ( const char *imsi, DAImsiInfo &info, DataAccessCallback cb, void *data ){}
    bool getImsiInfo ( const std::string &imsi, DAImsiInfo &info, DataAccessCallback cb, void *data ) 
    { 
        return getImsiInfo( imsi.c_str(), info, cb, data ); 
    }

    virtual bool insertUserImsi(const ImsiEntity &ie, DataAccessCallback cb, void *data){}
    virtual bool getEventIdsFromMsisdnData( SCassFuture &future, DAEventIdList &el ){}
    virtual bool getEventIdsFromMsisdn( int64_t msisdn, DAEventIdList &el, DataAccessCallback cb, void *data ){}

    virtual void getEventIdsFromExtId( const char *extid, DAEventIdList &el ){}
    void getEventIdsFromExtId( const std::string &extid, DAEventIdList &el ) { getEventIdsFromExtId( extid.c_str(), el ); }

    virtual bool getEventIdsFromExtIds( DAExtIdList &extl, DAEventIdList &el, DataAccessCallback cb, void *data ){}
    virtual bool getEventIdsFromExtIds( const char *extid, DAEventIdList &el, DataAccessCallback cb, void *data ){}
    bool getEventIdsFromExtIds( const std::string &extid, DAEventIdList &el, DataAccessCallback cb, void *data ) 
    { 
        return getEventIdsFromExtIds( extid.c_str(), el, cb, data ); 
    }
    virtual bool getEventIdsFromExtIdsData( SCassFuture &future, DAEventIdList &el){}

    virtual void getEventsFromImsi( const char *imsi, DAEventList &mcel ){}
    void getEventsFromImsi( const std::string &imsi, DAEventList &mcel ) { getEventsFromImsi( imsi.c_str(), mcel ); }
    virtual void getEventsFromImsi( DAImsiInfo &info, DAEventList &el ){}

    virtual bool checkOpcKeys( const uint8_t opP[16] ){}
    virtual bool updateOpc ( std::string &imsi, std::string& opc ){}

    virtual bool purgeUE ( std::string &imsi ){}

    virtual bool getMmeIdentityFromImsi ( std::string &imsi, DAMmeIdentity& mmeid ){}

    virtual bool getMmeIdentity ( std::string &mme_id, DAMmeIdentity& mmeid ){}
    virtual bool getMmeIdentity ( int32_t mme_id, DAMmeIdentity& mmeid ){}

    virtual bool getLatestIdentity(const char *table_name, int64_t &id, DataAccessCallback cb, void *data){}
    virtual bool getLatestIdentityData(SCassFuture &future, int64_t &id){}

    virtual bool updateLatestIdentity(const char *table_name, DataAccessCallback cb, void *data){}

    virtual bool getMmeIdFromHostData( SCassFuture &future, int32_t &mmeid ){}
    virtual bool getMmeIdFromHost ( std::string& host, int32_t &mmeid, DataAccessCallback cb, void *data ){}

    virtual bool addMmeIdentity(std::string &host, std::string &realm, int32_t mmeid ){}
    virtual bool addMmeIdentity1(std::string &host, std::string &realm, int32_t mmeid, DataAccessCallback cb, void *data ){}
    virtual bool addMmeIdentity2(std::string &host, std::string &realm, int32_t mmeid, DataAccessCallback cb, void *data ){}

    virtual bool updateLocation( DAImsiInfo &location, uint32_t present_flags, int32_t idmmeidentity, DataAccessCallback cb,
            void *data ){}
    virtual bool updateLocation ( DAImsiInfo &location, uint32_t present_flags, DataAccessCallback cb, void *data ){}

    virtual bool getImsiSec ( const std::string &imsi, DAImsiSec &imsisec, DataAccessCallback cb, void *data ){}
    virtual bool getImsiSecData(SCassFuture &future, DAImsiSec &imsisec){}


    virtual bool updateRandSqn ( const std::string &imsi, uint8_t * rand_p, uint8_t * sqn, bool inc_sqn, DataAccessCallback cb,
            void *data ){}

    virtual bool incSqn ( std::string &imsi, uint8_t * sqn ){}

    virtual bool getSubDataFromImsi( const char *imsi, std::string &sub_data ){}

    bool getSubDataFromImsi( const std::string &imsi, std::string &sub_data ) 
    {
        return getSubDataFromImsi( imsi.c_str(), sub_data);
    }

    virtual void UpdateValidityTime( const char *imsi, std::string &validity_time ){}
    void UpdateValidityTime( const std::string &imsi, std::string &validity_time ) 
    {
        UpdateValidityTime( imsi.c_str(), validity_time);
    }

    virtual void UpdateNIRDestination( const char *imsi, std::string &host, std::string &realm ){}
    void UpdateNIRDestination( const std::string &imsi, std::string &host, std::string &realm ) 
    {
        UpdateNIRDestination( imsi.c_str(), host, realm );
    }


//	bool getImsiFromScefIdScefRefId( char *scef_id, uint32_t scef_ref_id, std::string &imsi );

//	bool getScefIdScefRefIdFromImsi( char *imsi, std::string &scef_id, uint32_t &scef_ref_id );

//	bool delScefIds( const char *imsi);

//	bool delImsiFromScefIds ( const char *scef_id, uint32_t scef_ref_id );

// bool isImsiAttached ( const char *imsi );
// bool isImsiAttached ( const std::string &imsi ) { return isImsiAttached( imsi.c_str() ); }

};

#endif /* __DATAACCESS_H */
