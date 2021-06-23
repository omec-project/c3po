/*
* Copyright (c) 2021  Great Software Laboratory Pvt. Ltd
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <iostream>
#include <inttypes.h>
#include <iomanip>

#include "redis_dataaccess.h"
#include "sutility.h"
#include "serror.h"
#include "common_def.h"

#include "util.h"
#include "logger.h"
#include "options.h"

extern "C" {
   #include "auc.h"
}

// Database keyspace
#define KEYSPACE "vhss:"

// Database table names
#define USERS_IMSI_HASH 	"users_imsi:"
#define MSISDN_IMSI 		"msisdn_imsi"
#define GLOBAL_IDS 		"global_ids"
#define MME_IDENTITY 		"mmeidentity:"
#define MME_IDENTITY_HOST 	"mmeidentity_host:"
#define EVENTS			"events:"
#define EVENTS_MSISDN 		"events_msisdn"
#define EVENTS_EXTID 		"events_extid"
#define EXTID 			"extid"
#define EXTID_IMSI_XREF 	"extid_imsi_xref"
#define EXTID_IMSI 		"extid_imsi"

using namespace sw::redis;

RedisDataAccess::RedisDataAccess() : mp_db(nullptr)
{
}

RedisDataAccess::~RedisDataAccess()
{
	disconnect();
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// PUBLIC METHODS /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void RedisDataAccess::connect( const std::string &hst, const std::string &ks )
{
	std::cout << "Connecting to Redis DB" << std::endl;
	mp_db = new Redis(hst);
	m_keyspace = ks;
}

void RedisDataAccess::connect( const char *hst, int port, const char *ks )
{
	std::cout << "Connecting to Redis DB" << std::endl;

	ConnectionOptions connection_options;
	connection_options.host = hst;
	connection_options.port = port;
	ConnectionPoolOptions pool_options;
	if(Options::getredismaxconnections() > 0) {
		pool_options.size = Options::getredismaxconnections() ;
	} else {
		std::cout << "redismaxconnections can not be zero"  << std::endl;
		return ;
	}
	if(Options::getredistls()) {
		/* Tls setting */
		connection_options.tls.enabled = Options::getredistls();
		connection_options.tls.cert = Options::getredistlscert();
		connection_options.tls.key = Options::getredistlskey();
		connection_options.tls.cacert = Options::getredistlscacert();
	}
	mp_db = new Redis(connection_options, pool_options);

	m_keyspace = ks;
}
void RedisDataAccess::connect( const char *hst, const char *ks )
{
	std::cout << "Connecting to Redis DB" << std::endl;

	mp_db = new Redis(hst);
	m_keyspace = ks;
}
void RedisDataAccess::connect()
{
}

void RedisDataAccess::disconnect()
{
	if(mp_db) {
		delete mp_db;
		mp_db = nullptr;
	}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void RedisDataAccess::splitString(std::string str, std::string delimiter, std::list<std::string> &dest_list)
{
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {

        token = str.substr(0, pos);
        dest_list.push_back(token);
        str.erase(0, pos + delimiter.length());

      	}
      	dest_list.push_back(str);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::addEvent( DAEvent &event )
{
	std::stringstream ss;

	// insert the event
	{
		ss << KEYSPACE << EVENTS << event.scef_id <<":" << event.scef_ref_id;
		std::unordered_map<std::string,std::string> fld_val;

		fld_val.emplace("scef_id", event.scef_id);
		fld_val.emplace("scef_ref_id", std::to_string(event.scef_ref_id));
		fld_val.emplace("msisdn", std::to_string(event.msisdn));
		fld_val.emplace("extid", event.extid);
		fld_val.emplace("ui_json", event.ui_json);
		fld_val.emplace("mec_json", event.mec_json);
		fld_val.emplace("monitoring_type", std::to_string(event.monitoring_type));

		try
		{
			mp_db->hset(ss.str(), fld_val.begin(), fld_val.end());
		}
		catch(...)
		{
			return false;
		}

	}

	// insert into events_msisdn

	if ( event.msisdn != 0 )
	{
		ss.str( std::string() );

		ss << KEYSPACE << EVENTS_MSISDN;

		std::unordered_map<std::string,std::string> fld_val;

		std:: string str_member;
		std:: string str_scef_ref = event.scef_id + ":" + std::to_string(event.scef_ref_id);

		auto hget_res = mp_db->hget(ss.str(), std::to_string(event.msisdn));

		if(hget_res) {
			std::size_t pos = (*hget_res).find(str_scef_ref);

			if(pos != std::string::npos) {
				return true;
			}

			str_member = hget_res.value() + ",";
		}

		str_member += str_scef_ref;
		mp_db->hset(ss.str(), std::to_string(event.msisdn) , str_member);
	}

	// insert into events_extid
	if ( !event.extid.empty() )
	{
		ss.str( std::string() );

		ss << KEYSPACE << EVENTS_EXTID;

		std::unordered_map<std::string,std::string> fld_val;

		std:: string str_member;
		std:: string str_scef_ref = event.scef_id + ":" + std::to_string(event.scef_ref_id);

		auto hget_res = mp_db->hget(ss.str(), event.extid);

		if(hget_res) {
			std::size_t pos = (*hget_res).find(str_scef_ref);

			if(pos != std::string::npos) {
				return true;
			}

			str_member = hget_res.value() + ",";
		}

		str_member += str_scef_ref;
		mp_db->hset(ss.str(), event.extid, str_member);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getEvent( const char *scef_id, uint32_t scef_ref_id, DAEvent &event )
{
	std::stringstream ss;
	ss << KEYSPACE << EVENTS << scef_id <<":" << scef_ref_id;



	std::vector<std::string> fields = {"scef_id", "scef_ref_id", "msisdn", "extid", "ui_json", "mec_json", "monitoring_type"};

	std::vector<sw::redis::OptionalString> vals;
	try {
		uint8_t count = 0;

		mp_db->hmget(ss.str(), fields.begin(), fields.end(), std::back_inserter(vals));
		if(vals[0]) {
			event.scef_id = vals[0].value();
			count++;
		}
		if(vals[1]) {
			event.scef_ref_id = std::stoll(vals[1].value());
			count++;
		}
		if(vals[2]) {
			event.msisdn = std::stoll(vals[2].value());
			count++;
		}
		if(vals[3]) {
			event.extid = vals[3].value();
			count++;
		}

		if(vals[4]) {
			event.ui_json = vals[4].value();
			count++;
		}

		if(vals[5]) {
			event.mec_json = vals[5].value();
			count++;
		}

		if(vals[6]) {
			event.monitoring_type = std::stoll(vals[6].value());
			count++;
		}

		if(count < vals.size()) {
			// key or field does not exist.
			return false;
		}

	} catch(...) {

		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getEvents( const char *scef_id, std::list<uint32_t> scef_ref_ids, DAEventList &events, DataAccessCallback cb,
	void *data )
{
	std::stringstream ss;

	std::vector<std::string> fields = {"scef_id", "scef_ref_id", "msisdn", "extid", "ui_json", "mec_json", "monitoring_type"};

	for (auto it = scef_ref_ids.begin(); it != scef_ref_ids.end(); ++it)
	{
		ss.str( std::string() );

		ss << KEYSPACE << EVENTS << scef_id <<":" << *it;
		std::vector<sw::redis::OptionalString> vals;

		try {
			mp_db->hmget(ss.str(), fields.begin(), fields.end(), std::back_inserter(vals));
			if ( !getEventsData(vals, events) ) {
				return false;
			}
		} catch(...) {

			return false;
		}
	}
	return true;
}

bool RedisDataAccess::getEventsData( std::vector<OptionalString> &vals, DAEventList &events)
{
	DAEvent *event = new DAEvent();
	uint8_t count = 0;
	if(vals[0]) {
		event->scef_id = vals[0].value();
		count++;
	}
	if(vals[1]) {
		event->scef_ref_id = std::stoll(vals[1].value());
		count++;
	}
	if(vals[2]) {
		event->msisdn = std::stoll(vals[2].value());
		count++;
	}
	if(vals[3]) {
		event->extid = vals[3].value();
		count++;
	}

	if(vals[4]) {
		event->ui_json = vals[4].value();
		count++;
	}

	if(vals[5]) {
		event->mec_json = vals[5].value();
		count++;
	}

	if(vals[6]) {
		event->monitoring_type = std::stoll(vals[6].value());
		count++;
	}

	if(count == 0) {
		// All the vector elements are null if hash-key doesn't exist.
		// If so, no need to add to the event list.
		// This scenario is possible when a specific combination of
		// scef_id and scef_ref_id may not exist.
		delete event;
		return true;
	}

	if(count < vals.size()) {
		// Here the events table is represented as redis hash.
		// It is expected to have the required table columns which
		// in the case of redis hash is fields.
		delete event;
		Logger::system().error("RedisDataAccess::%s - Field not found", __func__ );
		return false;
	}

	events.push_back(event);
	return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void RedisDataAccess::deleteEvent( const char *scef_id, uint32_t scef_ref_id )
{
	DAEvent event;
	std::stringstream ss;

	if ( !getEvent( scef_id, scef_ref_id, event ) )
		return;

	{
		ss << KEYSPACE << EVENTS << scef_id << ":" << scef_ref_id;

		if(mp_db->exists(ss.str())) {

			mp_db->del(ss.str());

		}
	}

	//delete events_msisdn values
	if ( event.msisdn != 0 )
	{
		ss.str( std::string() );

		ss << KEYSPACE << EVENTS_MSISDN;

		auto hget_res = mp_db->hget(ss.str(),std::to_string( event.msisdn));

		if (hget_res) {
			std::string str_scef_ref = std::string(scef_id) + ":" + std::to_string(scef_ref_id);
			size_t pos = (*hget_res).find(str_scef_ref);
			if(pos == std::string::npos)
			{
				return;
			}
			if ( pos != 0 )
			{
				 str_scef_ref = "," + str_scef_ref;
				 pos-=1;
			}

			(*hget_res).erase(pos, str_scef_ref.length());

			if ( (*hget_res).front() == ',' ) {
				(*hget_res).erase(0,1);
			}

			if ( (*hget_res).empty() ) {
				mp_db->hdel(ss.str(),std::to_string( event.msisdn));
			}
			else {
				try
				{
					mp_db->hset(ss.str(),std::to_string(event.msisdn), (*hget_res));
				}
				catch(...)
				{
					//throw exception
				}
			}
		}
	}

	// delete events_extid entries
	if ( !event.extid.empty() )
	{
		ss.str( std::string() );
		ss << KEYSPACE << EVENTS_EXTID;

		auto hget_res = mp_db->hget(ss.str(), event.extid);

		if (hget_res) {
			std::string str_scef_ref = std::string(scef_id) + ":" + std::to_string(scef_ref_id);
			size_t pos = (*hget_res).find(str_scef_ref);
			if(pos == std::string::npos)
			{
				return;
			}
			if ( pos != 0 )
			{
				 str_scef_ref = "," + str_scef_ref;
				 pos-=1;
			}

			(*hget_res).erase(pos, str_scef_ref.length());

			if ( (*hget_res).front() == ',' ) {
				(*hget_res).erase(0,1);
			}

			if ( (*hget_res).empty() ) {
				mp_db->hdel(ss.str(), event.extid);
			}
			else {
				try
				{
					mp_db->hset(ss.str(), event.extid, (*hget_res));
				}
				catch(...)
				{
					//throw exception
				}
			}

		}
		else {
			//throw exception
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::checkMSISDNExists( int64_t msisdn )
{
	std::stringstream ss;
	ss << KEYSPACE << MSISDN_IMSI;

	auto zscore_res = mp_db->zscore(ss.str(), std::to_string(msisdn));
	if (zscore_res) {
			return true;
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::checkImsiExists( const char *imsi )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	if ( mp_db->exists(ss.str()) ) {
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool RedisDataAccess::checkExtIdExists( const char *extid )
{
   std::stringstream ss;
   ss << KEYSPACE << EXTID;

   if ( mp_db->sismember(ss.str(), extid) ) {
      return true;
   }

   return false;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getImsiListFromExtId( const char *extid, DAImsiList &imsilst)
{
   std::stringstream ss;
	ss << KEYSPACE << EXTID_IMSI;

	auto hget_res = mp_db->hget(ss.str(), extid);
	if ( hget_res ) {
		splitString(*hget_res, ",", imsilst);
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getExtIdsFromImsi( const char *imsi, DAExtIdList &extids, DataAccessCallback cb, void *data )
{
   std::stringstream ss;
	ss << KEYSPACE << EXTID_IMSI_XREF;

	auto hget_res = mp_db->hget(ss.str(), imsi);
	if ( hget_res ) {
		splitString(*hget_res, ",", extids);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getImsiFromMsisdn( int64_t msisdn, std::string &imsi )
{
	std::stringstream ss;
	ss << KEYSPACE << MSISDN_IMSI;
	auto zscore_res = mp_db->zscore(ss.str(), std::to_string(msisdn));
	if (zscore_res) {
		imsi = zscore_res.value();
		return true;
	}
	return false;
}

bool RedisDataAccess::getImsiFromMsisdn( const char *msisdn, std::string &imsi )
{
	std::stringstream ss;
	ss << KEYSPACE << MSISDN_IMSI;
	auto zscore_res = mp_db->zscore(ss.str(), msisdn);
	if(zscore_res) {
		imsi = zscore_res.value();
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getMsisdnFromImsi( const char *imsi, std::string &msisdn )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	auto hget_res = mp_db->hget(ss.str(), "msisdn");
	if ( hget_res ) {
		msisdn = hget_res.value();
		return true;
	}

	return false;
}

bool RedisDataAccess::getMsisdnFromImsi( const char *imsi, int64_t &msisdn )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	auto hget_res = mp_db->hget(ss.str(), "msisdn");
	if ( hget_res ) {
		msisdn = std::stoll(hget_res.value());
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getImsiInfoData(const char *imsi, std::vector<OptionalString> &result, DAImsiInfo &info )
{
	uint8_t count = 0;

	info.imsi = imsi;
	if ( result[0] ) {
		info.mmehost = result[0].value();
		count++;
	}
	if ( result[1] ) {
		info.mmerealm = result[1].value();
		count++;
	}
	if ( result[2] ) {
		info.ms_ps_status = result[2].value();
		count++;
	}
	if ( result[3] ) {
		info.subscription_data = result[3].value();
		count++;
	}
	if ( result[4] ) {
		info.str_msisdn = result[4].value();
		info.msisdn = std::stoll( info.str_msisdn);
		count++;
	}
	if ( result[5] ) {
		info.visited_plmnid = result[5].value();
		count++;
	}
	if ( result[6] ) {
		info.access_restriction = std::stoi(result[6].value());
		count++;
	}
	if ( result[7] ) {
		info.mme_id = std::stoi(result[7].value());
		count++;
	}

	if( count < result.size()) {
		return false;
	}

	return true;
}

bool RedisDataAccess::getImsiInfo ( const char *imsi, DAImsiInfo &info, DataAccessCallback cb, void *data )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	std::vector<std::string> fields = {"mmehost", "mmerealm", "ms_ps_status", "subscription_data", "msisdn", "visited_plmnid", 		"access_restriction", "mmeidentity_idmmeidentity"};

	std::vector<OptionalString> vals;

	mp_db->hmget(ss.str(), fields.begin(), fields.end(), std::back_inserter(vals));

	return getImsiInfoData(imsi, vals, info );
}

bool RedisDataAccess::executeQueryUsingPipe ( const char *imsi, DAImsiInfo &info, DAExtIdList &extidl, DAEventIdList &eidl,
	DAEventList &el, std::string& host, int32_t &mmeid, int64_t &msisdn)
{
	bool result = getImsiInfoExtIdMmeidUsingPipe(imsi, info, extidl, host, mmeid);
	if (result) {
		result = getEventIdsUsingPipe(extidl, eidl, msisdn);
	}
	if (result) {
		getEventsUsingPipe(eidl, el);
	}

	return result;
}

bool RedisDataAccess::getImsiInfoExtIdMmeidUsingPipe ( const char *imsi, DAImsiInfo &info, DAExtIdList &extidl, std::string& host,	int32_t &mmeid )
{
	std::stringstream ss0;
	ss0 << KEYSPACE << USERS_IMSI_HASH << imsi;
	std::vector<std::string> fields = {"mmehost", "mmerealm", "ms_ps_status", "subscription_data", "msisdn", "visited_plmnid",		"access_restriction", "mmeidentity_idmmeidentity"};
	std::vector<OptionalString> result0;

	std::stringstream ss1;
	ss1 << KEYSPACE << MME_IDENTITY_HOST << host;

	std::stringstream ss2;
	ss2 << KEYSPACE << EXTID_IMSI_XREF;

	auto pipe = mp_db->pipeline(false);

	pipe.hmget(ss0.str(), fields.begin(), fields.end());
	pipe.hget(ss1.str(), "idmmeidentity");
	pipe.hget(ss2.str(), imsi);

	auto replies = pipe.exec();
	replies.get(0, std::back_inserter(result0));
	bool status = getImsiInfoData(imsi, result0, info );
	if (!status) {
		return false;
	}

	auto result1 = replies.get<OptionalString>(1);
	if (result1) {
		mmeid = std::stoll(result1.value());
	}

	auto result2 = replies.get<OptionalString>(2);
	if (result2) {
		splitString(*result2, ",", extidl);
	}

	return true;
}

bool RedisDataAccess::getEventIdsUsingPipe ( DAExtIdList &extidl,DAEventIdList &eidl, int64_t &msisdn )
{
	std::stringstream ss0;
	ss0 << KEYSPACE << EVENTS_EXTID;

	std::stringstream ss1;
	ss1 << KEYSPACE << EVENTS_MSISDN;
	bool status0 = true, status1 = true;

	auto pipe = mp_db->pipeline(false);
	try
	{
		if (!extidl.empty()) {
			pipe.hmget(ss0.str(), extidl.begin(), extidl.end());
		}
		pipe.hget(ss1.str(), std::to_string(msisdn));
		auto replies = pipe.exec();

		std::vector<OptionalString> result0;
		OptionalString result1;
		if (!extidl.empty()) {
			replies.get(0, std::back_inserter(result0));
			status0 = getEventIdsFromExtIdsData(result0, eidl);
			result1 = replies.get<OptionalString>(1);
		}
		else {
			result1 = replies.get<OptionalString>(0);
		}

		std::vector<OptionalString> res_vect({result1});
		status1 = getEventIdsFromMsisdnData(res_vect, eidl);
	}
	catch(const Error &err)
	{
      		Logger::system().error("RedisDataAccess::%s - Exception in pipeline : %s", __func__, err.what());
		return false;
	}

	if(status0 && status1) {
		return true;
	}

	return false;
}

#define ADD_TO_PIPE() \
	for (auto it = scef_ref_ids.begin(); it != scef_ref_ids.end(); ++it) \
	{								     \
		std::stringstream ss; 					     \
		ss << KEYSPACE << EVENTS << scef_id <<":" << *it;	     \
		pipe.hmget(ss.str(), fields.begin(), fields.end());          \
	}

bool RedisDataAccess::getEventsUsingPipe ( DAEventIdList &eidl, DAEventList &events )
{
	std::vector<std::string> fields = {"scef_id", "scef_ref_id", "msisdn", "extid", "ui_json", "mec_json", "monitoring_type"};
        bool success = false;
        std::string scef_id = "";
        std::list<uint32_t> scef_ref_ids;

        if (eidl.size() == 0)
        {
                return true;
        }

        // sort the event id list
        eidl.sort(DAEventIdList::compare);
	try
	{
		auto pipe = mp_db->pipeline(false);
		for (auto it = eidl.begin(); it != eidl.end(); ++it)
		{
			if (scef_id != (*it)->scef_id && scef_ref_ids.size() > 0)
			{
				for (auto it = scef_ref_ids.begin(); it != scef_ref_ids.end(); ++it)
				{
					std::stringstream ss;
					ss << KEYSPACE << EVENTS << scef_id <<":" << *it;
					pipe.hmget(ss.str(), fields.begin(), fields.end());
				}

				// set for the next scef_id
				scef_ref_ids.clear();
				scef_id = (*it)->scef_id;
			}

			scef_ref_ids.push_back((*it)->scef_ref_id);
		}

		if (scef_ref_ids.size() > 0)
		{
			for (auto it = scef_ref_ids.begin(); it != scef_ref_ids.end(); ++it)
			{
				std::stringstream ss;
				ss << KEYSPACE << EVENTS << scef_id <<":" << *it;
				pipe.hmget(ss.str(), fields.begin(), fields.end());
			}
		}

		auto replies = pipe.exec();

		for (size_t idx = 0; idx < replies.size(); idx++) {
			std::vector<sw::redis::OptionalString> result;
			replies.get(idx, std::back_inserter(result));
			if ( !getEventsData(result, events) ) {
				return false;
			}
		}
	}
	catch(const Error &err)
	{
      		Logger::system().error("RedisDataAccess::%s - Exception in pipeline : %s", __func__, err.what());
		return false;
	}
	catch ( ... )
	{
      		Logger::system().error("RedisDataAccess::%s - Unknown exception", __func__);
		return false;
	}

	return true;
}
#undef ADD_TO_PIPE


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getEventIdsFromMsisdnData( std::vector<OptionalString> &result , DAEventIdList &eil )
{

	for ( auto it : result )
	{
		if (!it) {
      			//Logger::system().warn("RedisDataAccess::%s - No associated event ID found", __func__ );
			continue;
		}

		std::list<std::string> str_list;
		splitString(*it, ",", str_list);
		std::size_t pos = 0;
		DAEventId *ei = nullptr;
		for ( auto itr2 : str_list )
		{
			ei = new DAEventId();
			pos = itr2.find(":");
			if(pos == std::string::npos) {
      				Logger::system().error("RedisDataAccess::%s - Error parsing event ID's", __func__ );
				return false;
			}
			ei->scef_id = itr2.substr(0, pos);
			ei->scef_ref_id = std::stoi(itr2.substr(pos + 1));

			eil.push_back( ei );
		}
	}
   	return true;
}

bool RedisDataAccess::getEventIdsFromMsisdn( int64_t msisdn, DAEventIdList &eil, DataAccessCallback cb, void *data )
{
	std::stringstream ss;
	ss << KEYSPACE << EVENTS_MSISDN;

	auto hget_res = mp_db->hget(ss.str(), std::to_string(msisdn));

	std::vector<OptionalString> res_vect({hget_res});
	return getEventIdsFromMsisdnData( res_vect , eil );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void RedisDataAccess::getEventIdsFromExtId( const char *extid, DAEventIdList &eil )
{
	std::stringstream ss;
	ss << KEYSPACE << EVENTS_EXTID;

	auto hget_res = mp_db->hget(ss.str(), extid);

	std::vector<OptionalString> res_vect({hget_res});
	getEventIdsFromExtIdsData( res_vect, eil );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getEventIdsFromExtIds( DAExtIdList &extl, DAEventIdList &el, DataAccessCallback cb, void *data )
{
	std::stringstream ss;
	ss << KEYSPACE << EVENTS_EXTID;

	if (extl.empty()) {
		return true;
	}

	std::vector<OptionalString> result;
	try
	{
		mp_db->hmget(ss.str(), extl.begin(), extl.end(), std::back_inserter(result));
	}
	catch(const Error &err)
	{
      		Logger::system().error("RedisDataAccess::%s - Exception in hmget : %s", __func__, err.what());
		return false;
	}
   	return getEventIdsFromExtIdsData(result, el);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool RedisDataAccess::getEventIdsFromExtIdsData( std::vector<OptionalString> &result, DAEventIdList &el)
{
	for ( auto it : result )
	{
		if (!it) {
      			//Logger::system().warn("RedisDataAccess::%s - No associated event ID found", __func__ );
			continue;
		}

		std::list<std::string> str_list;
		splitString(*it, ",", str_list);
		std::size_t pos = 0;
		DAEventId *ei = nullptr;
		for ( auto itr2 : str_list )
		{
			ei = new DAEventId();
			pos = itr2.find(":");
			if(pos == std::string::npos) {
      				Logger::system().error("RedisDataAccess::%s - Error parsing event ID's", __func__ );
				return false;
			}
			ei->scef_id = itr2.substr(0, pos);
			ei->scef_ref_id = std::stoi(itr2.substr(pos + 1));

			el.push_back( ei );
		}
	}
   	return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void RedisDataAccess::getEventsFromImsi( const char *imsi, DAEventList &el )
{
	DAImsiInfo info;
	if ( !getImsiInfo( imsi, info, NULL, NULL ) )
      		return;
	getEventsFromImsi( info, el );
}

void RedisDataAccess::getEventsFromImsi( DAImsiInfo &info, DAEventList &el )
{
   	DAExtIdList extIdLst;
	DAEventIdList evtIdLst;
	
	getExtIdsFromImsi( info.imsi.c_str(), extIdLst, NULL, NULL );

	// get the EventId's associated with the msisdn
	getEventIdsFromMsisdn( info.msisdn, evtIdLst, NULL, NULL );

	// get the EventId's associated with the external identifiers
	for ( DAExtIdList::iterator it = extIdLst.begin(); it != extIdLst.end(); ++it )
   	{
		getEventIdsFromExtId( it->c_str(), evtIdLst );
	}

   	// get the events associated with the event id's
	for ( DAEventIdList::iterator it = evtIdLst.begin(); it != evtIdLst.end(); ++it ) {
		
		DAEvent *e = new DAEvent();
		if ( getEvent( (*it)->scef_id.c_str(), (*it)->scef_ref_id, *e ) )
			el.push_back( e );
		else
			delete e;
   	}
}

bool RedisDataAccess::checkOpcKeys( const uint8_t opP[16] )
{
	int cnt = 0;
	auto cursor = 0LL;
	std::stringstream pattern;
	pattern << KEYSPACE << USERS_IMSI_HASH << "*";
	auto count = 10;
	std::unordered_set<std::string> keys;
	while (true)
	{
		keys.clear();
		cursor = mp_db->scan(cursor, pattern.str(), count, std::inserter(keys, keys.begin()));

		for(auto &hash_imsi : keys)
		{
         		std::string imsi;
         		std::string key;
         		std::string opc;
	 		uint8_t field_cnt = 0;

			std::vector<std::string> fields = {"imsi", "key", "opc"};
   			std::vector<OptionalString> result;

			mp_db->hmget(hash_imsi, fields.begin(), fields.end(), std::back_inserter(result));

			if ( result[0] ) {
				imsi = result[0].value();
				field_cnt++;
			}
			if ( result[1] ) {
				key = result[1].value();
				field_cnt++;
			}
			if ( result[2] ) {
				opc = result[2].value();
				field_cnt++;
			}
			if ( field_cnt < result.size()) {
				// users_imsi table is represented as hash in redis db,
				// thus all fields in the hash represent table columns.
				// In this way all the fields need to be present.

				return false;
			}

			uint8_t opccalc[16];
         		uint8_t key_bin[16];
         		convert_ascii_to_binary (key_bin, (uint8_t *)key.c_str(), KEY_LENGTH);
         		ComputeOPc (key_bin, opP, opccalc);

         		std::string newopc = Utility::bytes2hex(opccalc, OPC_LENGTH);

			//Logger::system().info("COUNT: %d IMSI: %s KEY: %s OPC: %s NEW OPC: %s", ++cnt, imsi.c_str(), key.c_str(), opc.c_str(), newopc.c_str());

         		if ( !updateOpc(imsi, newopc) ) {
            			return false;
         		}
		}

		if ( cursor == 0 ) {
			break;
		}
	}

	return true;
}

bool RedisDataAccess::updateOpc ( std::string &imsi, std::string& opc )
{
   	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	//return value "value" indicates that the existing value is overwritten.
	//It does not indicate failure.
	try
	{
		mp_db->hset(ss.str(), "opc", opc);
	}
	catch(...)
	{
		return false;
	}
   return true;
}

bool RedisDataAccess::purgeUE ( std::string &imsi )
{
	if ( imsi.empty() ) {
      		return false;
	}

	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	//return value "value" indicates that the existing value is overwritten.
	//It does not indicate failure.
	try
	{
		mp_db->hset(ss.str(), "ms_ps_status", "PURGED");
	}
	catch(...)
	{
		return false;
	}
   	return true;
}

bool RedisDataAccess::getMmeIdentityFromImsi ( std::string &imsi, DAMmeIdentity& mmeid)
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	auto hget_res = mp_db->hget(ss.str(), "mmeidentity_idmmeidentity");
	if (hget_res) {
		return getMmeIdentity( hget_res.value(), mmeid );
	}
	return false;
}

bool RedisDataAccess::getMmeIdentity ( std::string &mme_id, DAMmeIdentity& mmeid )
{
	std::stringstream ss;

	ss << KEYSPACE << MME_IDENTITY << mme_id;


	std::vector<std::string> fields = {"mmehost", "mmerealm", "mmeisdn"};
	std::vector<sw::redis::OptionalString> vals;
	try {
		uint8_t count = 0;
		mp_db->hmget(ss.str(), fields.begin(), fields.end(), std::back_inserter(vals));
		if(vals[0]) {
			mmeid.mme_host = vals[0].value();
			count++;
		}
		if(vals[1]) {
			mmeid.mme_realm = vals[1].value();
			count++;
		}
		if(vals[2]) {
			mmeid.mme_isdn = vals[2].value();
			count++;
		}
		if(count < vals.size()) {
			//the required key or a field does not exist.
			return false;
		}

	} catch(...) {

		return false;
	}

	return true;

}

bool RedisDataAccess::getMmeIdentity ( int32_t mme_id, DAMmeIdentity& mmeid )
{
	std::stringstream ss;

	ss << KEYSPACE << MME_IDENTITY << mme_id;


	std::vector<std::string> fields = {"mmehost", "mmerealm", "mmeisdn"};
	std::vector<sw::redis::OptionalString> vals;
	try {
		uint8_t count = 0;
		mp_db->hmget(ss.str(), fields.begin(), fields.end(), std::back_inserter(vals));

		if(vals[0]) {
			mmeid.mme_host = vals[0].value();
			count++;
		}
		if(vals[1]) {
			mmeid.mme_realm = vals[1].value();
			count++;
		}
		if(vals[2]) {
			mmeid.mme_isdn = vals[2].value();
			count++;
		}
		if(count < vals.size()) {
			//the required key or a field does not exist.
			return false;
		}

	} catch(...) {

		return false;
	}

	return true;
}

bool RedisDataAccess::getLatestIdentity(const char *table_name, int64_t &id, DataAccessCallback cb, void *data)
{
	std::stringstream ss;

	ss << KEYSPACE << GLOBAL_IDS;
	auto zscore_res = mp_db->zscore(ss.str(), table_name);
	if(zscore_res) {
		id = zscore_res.value();
	}

	return true;
}


bool RedisDataAccess::updateLatestIdentity(const char *table_name, DataAccessCallback cb, void *data)
{
	std::stringstream ss;

	ss << KEYSPACE << GLOBAL_IDS;
	auto zscore_res = mp_db->zscore(ss.str(), table_name);

	if(zscore_res) {
		auto id = zscore_res.value()+1;
		mp_db->zadd(ss.str(),table_name, id);
	}
	return true;
}


bool RedisDataAccess::getMmeIdFromHost ( std::string& host, int32_t &mmeid, DataAccessCallback cb, void *data )
{
	std::stringstream ss;

	ss << KEYSPACE << MME_IDENTITY_HOST << host;

	auto hget_res = mp_db->hget(ss.str(), "idmmeidentity");
	if(hget_res) {
		mmeid = std::stoll(hget_res.value());
		return true;
	}
	return false;
}

bool RedisDataAccess::addMmeIdentity(std::string &host, std::string &realm, int32_t mmeid)
{
	if (!addMmeIdentity1(host,realm,mmeid,NULL,NULL))
		return false;

	return addMmeIdentity2(host,realm,mmeid,NULL,NULL);
}

bool RedisDataAccess::addMmeIdentity1(std::string &host, std::string &realm, int32_t mmeid, DataAccessCallback cb, void *data)
{
	std::stringstream ss;
	ss << KEYSPACE << MME_IDENTITY << std::to_string( mmeid);
	std::unordered_map<std::string,std::string> fld_val;
	fld_val.emplace("mmehost", host);
	fld_val.emplace("idmmeidentity", std::to_string(mmeid));
	fld_val.emplace("mmerealm", realm);

	try{
		mp_db->hset(ss.str(), fld_val.begin(), fld_val.end());
	}
	catch(...) {

		return false;
	}

	return true;

}

bool RedisDataAccess::addMmeIdentity2(std::string &host, std::string &realm, int32_t mmeid, DataAccessCallback cb, void *data)
{
	std::stringstream ss;

	ss << KEYSPACE << MME_IDENTITY_HOST << host;
	std::unordered_map<std::string,std::string> fld_val;
	fld_val.emplace("mmehost", host);
	fld_val.emplace("idmmeidentity", std::to_string(mmeid));
	fld_val.emplace("mmerealm", realm);

	try{
		mp_db->hset(ss.str(), fld_val.begin(), fld_val.end());
	}
	catch(...) {

		return false;
	}
	return true;
}

bool RedisDataAccess::updateLocation( DAImsiInfo &location, uint32_t present_flags, int32_t idmmeidentity, DataAccessCallback cb, 	void *data )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << location.imsi;
	std::unordered_map<std::string,std::string> fld_val;

	if ( FLAG_IS_SET( present_flags, IMEI_PRESENT) ) {
		fld_val.emplace("imei", location.imei);
	}

	if ( FLAG_IS_SET( present_flags, SV_PRESENT) ) {
		fld_val.emplace("imei_sv", location.imei_sv);
	}

	if ( FLAG_IS_SET( present_flags, MME_IDENTITY_PRESENT ) ) {
		fld_val.emplace("mmeidentity_idmmeidentity", std::to_string(idmmeidentity));
		fld_val.emplace("mmehost", location.mmehost);
		fld_val.emplace("mmerealm", location.mmerealm);
	}

	std::string status("ATTACHED");
	fld_val.emplace("ms_ps_status", status);
	fld_val.emplace("visited_plmnid", location.visited_plmnid);

	try
	{
		mp_db->hset(ss.str(), fld_val.begin(), fld_val.end());
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool RedisDataAccess::updateLocation ( DAImsiInfo &location, uint32_t present_flags, DataAccessCallback cb, void *data )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << location.imsi;
	std::unordered_map<std::string,std::string> fld_val;

	if ( FLAG_IS_SET( present_flags, IMEI_PRESENT) ) {
		fld_val.emplace("imei", location.imei);
	}

	if ( FLAG_IS_SET( present_flags, SV_PRESENT) ) {
      		fld_val.emplace("imei_sv", location.imei_sv);
	}

   	if ( FLAG_IS_SET( present_flags, MME_IDENTITY_PRESENT ) ) {
      		fld_val.emplace("mmeidentity_idmmeidentity", std::to_string(location.mme_id));
      		fld_val.emplace("mmehost", location.mmehost);
      		fld_val.emplace("mmerealm", location.mmerealm);
   	}

	std::string status("ATTACHED");
	fld_val.emplace("ms_ps_status", status);
	fld_val.emplace("visited_plmnid", location.visited_plmnid);

	try
	{
		mp_db->hset(ss.str(), fld_val.begin(), fld_val.end());
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool RedisDataAccess::getImsiSecData( std::vector<OptionalString> &result, DAImsiSec &imsisec )
{
	std::string key_str;
	int64_t     sqn_nb;
	std::string rand_str;
	std::string opc_str;

	uint8_t count = 0;

	if ( result[0] ) {
		key_str = result[0].value();
		count++;
	}
	if ( result[1] ) {
		sqn_nb = std::stoll(result[1].value());
		count++;
	}
	if ( result[2] ) {
		rand_str = result[2].value();
		count++;
	}
	if ( result[3] ) {
		opc_str = result[3].value();
		count++;
	}

	if( count < result.size()) {
		return false;
	}

	convert_ascii_to_binary(imsisec.key, (uint8_t *)key_str.c_str(), KEY_LENGTH);
	convert_ascii_to_binary(imsisec.rand,(uint8_t *)rand_str.c_str(), RAND_LENGTH);
	convert_ascii_to_binary(imsisec.opc, (uint8_t *)opc_str.c_str(), OPC_LENGTH);

	imsisec.sqn[0] = (sqn_nb & (255UL << 40)) >> 40;
	imsisec.sqn[1] = (sqn_nb & (255UL << 32)) >> 32;
	imsisec.sqn[2] = (sqn_nb & (255UL << 24)) >> 24;
	imsisec.sqn[3] = (sqn_nb & (255UL << 16)) >> 16;
	imsisec.sqn[4] = (sqn_nb & (255UL << 8)) >> 8;
	imsisec.sqn[5] = (sqn_nb & 0xFF);

	return true;
}

bool RedisDataAccess::getImsiSec ( const std::string &imsi, DAImsiSec &imsisec, DataAccessCallback cb, void *data )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	std::vector<std::string> fields = {"key", "sqn", "rand", "opc"};
   	std::vector<OptionalString> vals;

	mp_db->hmget(ss.str(), fields.begin(), fields.end(), std::back_inserter(vals));

   	return getImsiSecData(vals, imsisec);
}

bool RedisDataAccess::updateRandSqn ( const std::string &imsi, uint8_t * rand_p, uint8_t * sqn, bool inc_sqn, DataAccessCallback cb, void *data )
{
	SqnU64Union eu;
	SQN_TO_U64(sqn, eu);

	if (inc_sqn) {
      		eu.u64 += 32;
   	}

	std::stringstream eu_str;
	eu_str << eu.u64;

	std::string rand = Utility::bytes2hex(rand_p, RAND_LENGTH);

	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	std::unordered_map<std::string,std::string> fld_val;
   	fld_val.emplace("rand", rand);
   	fld_val.emplace("sqn", eu_str.str());

	try {
		mp_db->hset(ss.str(), fld_val.begin(), fld_val.end());
	}
	catch(...) {
		return false;
	}
	return true;
}

bool RedisDataAccess::incSqn ( std::string &imsi, uint8_t * sqn )
{
	SqnU64Union eu;

	SQN_TO_U64(sqn, eu);
	eu.u64 += 32;

	std::stringstream eu_str;
	eu_str << eu.u64;
	std::stringstream ss;
	
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	try
	{
		mp_db->hset(ss.str(), "sqn", eu_str.str());
	}
	catch(...) {
		return false;
	}
	return true;
}

bool RedisDataAccess::getSubDataFromImsi( const char *imsi, std::string &sub_data )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	auto hget_res = mp_db->hget(ss.str(), "subscription_data");
	if (hget_res) {
		sub_data = hget_res.value();
		return true;
	}

	return false;
}

void RedisDataAccess::UpdateValidityTime( const char *imsi, std::string &validity_time )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	try {
		mp_db->hset(ss.str(), "niddvalidity", validity_time);
	}
	catch(...) {

	}
}

void RedisDataAccess::UpdateNIRDestination( const char *imsi, std::string &host, std::string &realm )
{
	std::stringstream ss;
	ss << KEYSPACE << USERS_IMSI_HASH << imsi;

	std::unordered_map<std::string,std::string> fld_val;
   	fld_val.emplace("nir_dest_host", host);
   	fld_val.emplace("nir_dest_realm", realm);

	try
	{
		mp_db->hset(ss.str(), fld_val.begin(), fld_val.end());
	}
	catch(...)
	{
	
	}
}
