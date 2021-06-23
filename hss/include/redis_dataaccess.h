/*
* Copyright (c) 2021  Great Software Laboratory Pvt. Ltd
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __REDISDATAACCESS_H_
#define __REDISDATAACCESS_H_

#include <stdexcept>
#include <list>
#include <string>

#include "dataaccess.h"
#include <sw/redis++/redis++.h>
#include <unordered_set>

class RedisDataAccess : public DataAccess
{
public:
	RedisDataAccess();
	virtual ~RedisDataAccess();

	void connect();
	void connect( const std::string &hst, const std::string &ks = "vhss" );
	void connect( const char *hst, const char *ks = "vhss" );
	void connect( const char *hst, int port, const char *ks = "vhss" );
	void disconnect();

	void splitString(std::string str, std::string delimiter, std::list<std::string> &dest_list);

	bool addEvent( DAEvent &event );
	bool getEvent( const char *scef_id, uint32_t scef_ref_id, DAEvent &event );

   	bool getEvents( const char *scef_id, std::list<uint32_t> scef_ref_ids, DAEventList &events, DataAccessCallback cb,
		void *data );

   	bool getEvents( const char *scef_id, std::list<uint32_t> scef_ref_ids, DAEventList &events, void *data );
   	bool getEventsData( std::vector<sw::redis::OptionalString> &vals, DAEventList &events );

   	void deleteEvent( const char *scef_id, uint32_t scef_ref_id );

	bool checkMSISDNExists( int64_t msisdn) ;

   	bool checkImsiExists( const char *imsi );
   	bool checkExtIdExists( const char *extid );

	bool getImsiListFromExtId( const char *extid, DAImsiList &imsilst);

   	bool getExtIdsFromImsi( const char *imsi, DAExtIdList &extids, DataAccessCallback cb, void *data );

	bool getImsiFromMsisdn( int64_t msisdn, std::string &imsi );
   	bool getImsiFromMsisdn( const char *msisdn, std::string &imsi );

	bool getMsisdnFromImsi( const char *imsi, std::string &msisdn );
   	bool getMsisdnFromImsi( const char *imsi, int64_t &msisdn );

   	bool getImsiInfoData(const char* imsi, std::vector<sw::redis::OptionalString> &result, DAImsiInfo &info );
   	bool getImsiInfo ( const char *imsi, DAImsiInfo &info, DataAccessCallback cb, void *data );

	bool getEventIdsFromMsisdnData( std::vector<sw::redis::OptionalString> &result, DAEventIdList &el );
   	bool getEventIdsFromMsisdn( int64_t msisdn, DAEventIdList &el, DataAccessCallback cb, void *data );

	void getEventIdsFromExtId( const char *extid, DAEventIdList &el );
	
	bool getEventIdsFromExtIds( DAExtIdList &extl, DAEventIdList &el, DataAccessCallback cb, void *data );
	bool getEventIdsFromExtIdsData( std::vector<sw::redis::OptionalString> &result, DAEventIdList &el);

	void getEventsFromImsi( const char *imsi, DAEventList &mcel );
	void getEventsFromImsi( DAImsiInfo &info, DAEventList &el );

	bool checkOpcKeys( const uint8_t opP[16] );
	bool updateOpc ( std::string &imsi, std::string& opc );

	bool purgeUE ( std::string &imsi );
	bool getMmeIdentityFromImsi ( std::string &imsi, DAMmeIdentity& mmeid );

	bool getMmeIdentity ( std::string &mme_id, DAMmeIdentity& mmeid );
	bool getMmeIdentity ( int32_t mme_id, DAMmeIdentity& mmeid );

	bool getLatestIdentity(const char *table_name, int64_t &id, DataAccessCallback cb, void *data);

	bool updateLatestIdentity(const char *table_name, DataAccessCallback cb, void *data);

	bool getMmeIdFromHostData( int32_t &mmeid );
	bool getMmeIdFromHost ( std::string& host, int32_t &mmeid, DataAccessCallback cb, void *data );
	
	bool addMmeIdentity(std::string &host, std::string &realm, int32_t mmeid );
	bool addMmeIdentity1(std::string &host, std::string &realm, int32_t mmeid, DataAccessCallback cb, void *data );
	bool addMmeIdentity2(std::string &host, std::string &realm, int32_t mmeid, DataAccessCallback cb, void *data );

	bool updateLocation( DAImsiInfo &location, uint32_t present_flags, int32_t idmmeidentity, DataAccessCallback cb, void *data );
	bool updateLocation ( DAImsiInfo &location, uint32_t present_flags, DataAccessCallback cb, void *data );

	bool getImsiSec ( const std::string &imsi, DAImsiSec &imsisec, DataAccessCallback cb, void *data );
	bool getImsiSecData( std::vector<sw::redis::OptionalString> &result, DAImsiSec &imsisec );


	bool updateRandSqn ( const std::string &imsi, uint8_t * rand_p, uint8_t * sqn, bool inc_sqn, DataAccessCallback cb,
		void *data );

	bool incSqn ( std::string &imsi, uint8_t * sqn );

	bool getSubDataFromImsi( const char *imsi, std::string &sub_data );

	void UpdateValidityTime( const char *imsi, std::string &validity_time );

	void UpdateNIRDestination( const char *imsi, std::string &host, std::string &realm );

	bool executeQueryUsingPipe ( const char *imsi, DAImsiInfo &info, DAExtIdList &extidl, DAEventIdList &eidl, DAEventList &el,
		std::string& host, int32_t &mmeid, int64_t &msisdn);

private:
	bool getImsiInfoExtIdMmeidUsingPipe ( const char *imsi, DAImsiInfo &info, DAExtIdList &extidl, std::string& host,
		int32_t &mmeid );

	bool getEventIdsUsingPipe ( DAExtIdList &extidl,DAEventIdList &eidl, int64_t &msisdn );
	bool getEventsUsingPipe ( DAEventIdList &eidl, DAEventList &events );
	sw::redis::Redis *mp_db;
	std::string m_keyspace;
};

#endif /* __REDISDATAACCESS_H_ */
