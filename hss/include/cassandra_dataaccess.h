/*
* Copyright (c) 2021  Great Software Laboratory Pvt. Ltd
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __CASSANDRADATAACCESS_H_
#define __CASSANDRADATAACCESS_H_

#include "dataaccess.h"

class CassandraDataAccess : public DataAccess
{
public:
    CassandraDataAccess();
    virtual ~CassandraDataAccess();

    std::string &host( const char *hst ) { return m_db.host(hst); }
    std::string &host( const std::string &hst ) { return m_db.host(hst); }
    std::string &host() { return m_db.host(); }

    std::string &keyspace( const char *ks ) { return m_db.keyspace(ks); }
    std::string &keyspace( const std::string &ks ) { return m_db.keyspace(ks); }
    std::string &keyspace() { return m_db.keyspace(); }

    int protocolVersion( int pv ) { return m_db.protocolVersion(pv); }
    int protocolVersion() { return m_db.protocolVersion(); }

    void connect();
    void connect( const std::string &hst, const std::string &ks = "vhss" );
    void connect( const char *hst, const char *ks = "vhss" );

    void disconnect();

    bool addEvent( DAEvent &event );

    bool getEvent( const char *scef_id, uint32_t scef_ref_id, DAEvent &event );

    bool getEvents( const char *scef_id, std::list<uint32_t> scef_ref_ids, DAEventList &events, DataAccessCallback cb,
        void *data );
    bool getEventsData( SCassFuture &future, DAEventList &events );

    void deleteEvent( const char *scef_id, uint32_t scef_ref_id );

    bool checkMSISDNExists( int64_t msisdn) ;

    bool checkImsiExists( const char *imsi );

    bool checkExtIdExists( const char *extid );

    bool getImsiListFromExtId( const char *extid, DAImsiList &imsilst);

    bool getExtIdsFromImsiData( SCassFuture &future, DAExtIdList &extids );
    bool getExtIdsFromImsi( const char *imsi, DAExtIdList &extids, DataAccessCallback cb, void *data );

    bool getImsiFromMsisdn( int64_t msisdn, std::string &imsi );
    bool getImsiFromMsisdn( const char *msisdn, std::string &imsi );

    bool getMsisdnFromImsi( const char *imsi, std::string &msisdn );
    bool getMsisdnFromImsi( const char *imsi, int64_t &msisdn );

    bool getImsiInfoData( SCassFuture &future, DAImsiInfo &info );
    bool getImsiInfo ( const char *imsi, DAImsiInfo &info, DataAccessCallback cb, void *data );

    bool insertUserImsi(const ImsiEntity &ie, DataAccessCallback cb, void *data);
    bool getEventIdsFromMsisdnData( SCassFuture &future, DAEventIdList &el );
    bool getEventIdsFromMsisdn( int64_t msisdn, DAEventIdList &el, DataAccessCallback cb, void *data );

    void getEventIdsFromExtId( const char *extid, DAEventIdList &el );

    bool getEventIdsFromExtIds( const char *extid, DAEventIdList &el, DataAccessCallback cb, void *data );
    bool getEventIdsFromExtIdsData( SCassFuture &future, DAEventIdList &el);

    void getEventsFromImsi( const char *imsi, DAEventList &mcel );
    void getEventsFromImsi( DAImsiInfo &info, DAEventList &el );

    bool checkOpcKeys( const uint8_t opP[16] );
    bool updateOpc ( std::string &imsi, std::string& opc );

    bool purgeUE ( std::string &imsi );

    bool getMmeIdentityFromImsi ( std::string &imsi, DAMmeIdentity& mmeid );

    bool getMmeIdentity ( std::string &mme_id, DAMmeIdentity& mmeid );
    bool getMmeIdentity ( int32_t mme_id, DAMmeIdentity& mmeid );

    bool getLatestIdentity(const char *table_name, int64_t &id, DataAccessCallback cb, void *data);
    bool getLatestIdentityData(SCassFuture &future, int64_t &id);

    bool updateLatestIdentity(const char *table_name, DataAccessCallback cb, void *data);

    bool getMmeIdFromHostData( SCassFuture &future, int32_t &mmeid );
    bool getMmeIdFromHost ( std::string& host, int32_t &mmeid, DataAccessCallback cb, void *data );

    bool addMmeIdentity(std::string &host, std::string &realm, int32_t mmeid );
    bool addMmeIdentity1(std::string &host, std::string &realm, int32_t mmeid, DataAccessCallback cb, void *data );
    bool addMmeIdentity2(std::string &host, std::string &realm, int32_t mmeid, DataAccessCallback cb, void *data );

    bool updateLocation( DAImsiInfo &location, uint32_t present_flags, int32_t idmmeidentity, DataAccessCallback cb, void *data );
    bool updateLocation ( DAImsiInfo &location, uint32_t present_flags, DataAccessCallback cb, void *data );

    bool getImsiSec ( const std::string &imsi, DAImsiSec &imsisec, DataAccessCallback cb, void *data );
    bool getImsiSecData(SCassFuture &future, DAImsiSec &imsisec);

    bool updateRandSqn ( const std::string &imsi, uint8_t * rand_p, uint8_t * sqn, bool inc_sqn, DataAccessCallback cb,
        void *data );

    bool incSqn ( std::string &imsi, uint8_t * sqn );

    bool getSubDataFromImsi( const char *imsi, std::string &sub_data );

    void UpdateValidityTime( const char *imsi, std::string &validity_time );

    void UpdateNIRDestination( const char *imsi, std::string &host, std::string &realm );


//  bool getImsiFromScefIdScefRefId( char *scef_id, uint32_t scef_ref_id, std::string &imsi );

//  bool getScefIdScefRefIdFromImsi( char *imsi, std::string &scef_id, uint32_t &scef_ref_id );

//  bool delScefIds( const char *imsi);

//  bool delImsiFromScefIds ( const char *scef_id, uint32_t scef_ref_id );

// bool isImsiAttached ( const char *imsi );
// bool isImsiAttached ( const std::string &imsi ) { return isImsiAttached( imsi.c_str() ); }


private:

    SCassandra m_db;
};

#endif /* __CASSANDRADATAACCESS_H */
