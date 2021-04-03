/*
* Copyright (c) 2003-2020, Great Software Laboratory Pvt. Ltd.
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <freeDiameter/freeDiameter-host.h>

#include "options.h"
#include "session.h"

#define RAPIDJSON_NAMESPACE pcrfrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

static const char* kTypeNames[] = 
    { "Null", "False", "True", "Object", "Array", "String", "Number" };


PoliciesConfig::PoliciesConfig()
{
}

void PoliciesConfig::add_service_group_map( std::string service_name, ServiceProfiles* service_profile )
{
	service_group_map[service_name] = service_profile; 
}

void PoliciesConfig::remove_service_group_map( std::string& service_name )
{
	service_group_map.erase( service_name );
}

ServiceProfiles* PoliciesConfig::get_service_group_map( std::string& service_name ) const 
{
	auto itr = service_group_map.find( service_name );
	if( itr == service_group_map.end() )
	{
		return NULL;
	}
	return itr->second;
}

void PoliciesConfig::add_service_selection_map( std::string service_selection_name, ServiceSelection* service_selection )
{
	service_selection_map[service_selection_name] = service_selection; 
}

void PoliciesConfig::remove_service_selection_map( std::string& service_selection_name )
{
	service_selection_map.erase( service_selection_name );
}

ServiceSelection* PoliciesConfig::get_service_selection_map( std::string& service_selection_name ) const
{
	auto itr = service_selection_map.find( service_selection_name );
	if( itr == service_selection_map.end() )
	{
		return NULL;
	}
	return itr->second;
}

void PoliciesConfig::add_config_rule_map( std::string rule_name, ConfigRule* config_rule )
{
	config_rule_map[rule_name] = config_rule;
}

ConfigRule* PoliciesConfig::get_config_rule_map( std::string& rule_name ) const
{
	auto itr = config_rule_map.find( rule_name );
   if( itr == config_rule_map.end() )
   {
      return NULL;
   }
   return itr->second;
}

void PoliciesConfig::remove_config_rule_map( std::string& rule_name )
{
	config_rule_map.erase( rule_name );
}

void PoliciesConfig::getDefaultRule( std::string& apn_name, DefaultRule* default_rule ) const
{
	ServiceProfiles* service_profile = 	get_service_group_map( apn_name );	
	if( service_profile != NULL )
	{
		std::string service_type = service_profile->getServiceType();
		std::string service_name = service_profile->get_service_type_map( service_type );
	
		ServiceSelection* service_selection = get_service_selection_map( service_name );
		if( service_selection != NULL )
		{
			default_rule->setApnAggregateMaxBitrateUl( service_selection->getAmbrUl() );
			default_rule->setApnAggregateMaxBitrateDl( service_selection->getAmbrDl() );	
			std::list<int> activation_rules_index_list = service_selection->get_activation_rules_index_list();
			for (std::list<int>::iterator it=activation_rules_index_list.begin(); it != activation_rules_index_list.end(); ++it)
			{
    			int index = *it;
				std::string activation_rule_name = service_selection->get_activation_rules_map( index );
				ConfigRule* config_rule = get_config_rule_map( activation_rule_name );
				if( config_rule != NULL )
				{
					default_rule->setRuleName( config_rule->getRuleName() );
					default_rule->setPriorityLevel( config_rule->getPriorityLevel() );
					default_rule->setPreemptionCapability( config_rule->getPreemptionCapability() );
					default_rule->setPreemptionVulnerability( config_rule->getPreemptionVulnerability() );
					default_rule->setQci( config_rule->getQci() );
					default_rule->setDefinition( config_rule->getDefinition() );
				}
			}
		}		
	}
}


ServiceProfiles::ServiceProfiles()
{
}

void ServiceProfiles::add_service_type_list( std::string v )
{
	m_service_type_list.push_back( v );
}

void ServiceProfiles::remove_service_type_list( std::string& v )
{
	m_service_type_list.remove( v );
}

bool ServiceProfiles::get_service_type_list( std::string v )
{
	std::list<std::string>::iterator it;
	it = std::find( m_service_type_list.begin(), m_service_type_list.end(), v );
	if( it != m_service_type_list.end() )
	{
		return true;
	}
	return false;
}

void ServiceProfiles::add_service_type_map( std::string key, std::string val )
{
	m_service_type_map[key] = val;
}

void ServiceProfiles::remove_service_type_map( std::string& key )
{
	m_service_type_map.erase( key );
}

std::string ServiceProfiles::get_service_type_map( std::string& key )
{
	auto itr = m_service_type_map.find( key );
   if( itr == m_service_type_map.end() )
   {
      return NULL;
   }
   return itr->second;	
}

ServiceSelection::ServiceSelection()
{
}

ServiceSelection::~ServiceSelection()
{
}

void ServiceSelection::add_activation_rules_index_list( int index )
{
	m_activation_rules_index_list.push_back( index );
}

void ServiceSelection::remove_activation_rules_index_list( int index )
{
	m_activation_rules_index_list.remove( index );
}

std::list<int>& ServiceSelection::get_activation_rules_index_list()
{
	return m_activation_rules_index_list;
}

void ServiceSelection::add_activation_rules_map( int index, std::string v )
{
	
   //m_activation_rules_list.push_back( v );
	m_activation_rules_map[index] = v;
	
}

void ServiceSelection::remove_activation_rules_map( int index )
{
  // m_activation_rules_list.remove( v );
	m_activation_rules_map.erase( index );
}

std::string ServiceSelection::get_activation_rules_map( int index )
{
	/*
   std::list<std::string>::iterator it;
   it = std::find( m_activation_rules_list.begin(), m_activation_rules_list.end(), v );
   if( it != m_activation_rules_list.end() )
   {
      return true;
   }
   return false;
	*/

	auto itr = m_activation_rules_map.find( index );
   if( itr == m_activation_rules_map.end() )
   {
      return "";
   }
   return itr->second;
}

ConfigRule::ConfigRule()
{
}

ConfigRule::~ConfigRule()
{
}

Options *Options::m_singleton = NULL;

Options::Options()
   : m_selfredirect( false ),
     m_enableruletimers( false ),
     m_rulecallbackport( 9081 ),
     m_rulecallbackthreads( 1 ),
     m_options( 0 ),
     m_logmaxsize( 0 ),
     m_lognbrfiles( 0 ),
     m_logqueuesize( 0 ),
     m_statlogmaxsize ( 0 ),
     m_statlognbrfiles ( 0 ),
     m_statsfrequency( 0 ),
     m_auditlogmaxsize ( 0 ),
     m_auditlognbrfiles ( 0 ),
     m_ossport ( 0 ),
     m_policies_config( NULL ),
     m_restport ( 0 )
{
}

Options::~Options()
{
}

void Options::help()
{
   std::cout << std::endl
             << "Usage:  pcrf -t casshost [OPTIONS]..." << std::endl
             << "  -b, --rulecallbackthreads n  The number of threads to process Chronos \"pop\" notifications." << std::endl
             << "  -c, --fdcfg filename         Read the freeDiameter configuration from this file" << std::endl
             << "                               instead of the default location (" DEFAULT_CONF_PATH "/" FD_DEFAULT_CONF_FILENAME ")." << std::endl
             << "  -d, --selfredirect           If present, enables the self-redirect feature." << std::endl
             << "  -e, --enableruletimers       If present, enables rule timers (if specified in the rule definition)." << std::endl
             << "  -f, --rulecallbackurl url    The URL used by Chronos to send \"pop\" notifications." << std::endl
             << "  -h, --help                   Print help and exit" << std::endl
             << "  -j, --jsoncfg filename       The JSON configuration file." << std::endl
             << "  -k, --casskeyspace keyspace  The Cassandra keyspace." << std::endl
             << "  -l, --logname filename       The base filename for the log files." << std::endl
             << "  -n, --lognumber number       The number of log files to maintain." << std::endl
             << "  -p, --rulecallbackport port  The IP port to receive Chronos \"pop\" notifications when rule timers expire." << std::endl
             << "  -q, --logqsize size          The log queue size in bytes, must be a power of 2." << std::endl
             << "  -r, --originrealm realm      The diameter destination realm." << std::endl
             << "  -s, --originhost host        The diameter destination host." << std::endl
             << "  -t, --casshost host          The Cassandra host IP address." << std::endl
             << "  -z, --logsize size           The maximum log size in MB." << std::endl
   ;
}

bool Options::parse( int argc, char **argv ){

   bool ret = true;

   ret = singleton().parseInputOptions( argc, argv );
   std::cout<<"parseInputOptions -  "<<ret<<std::endl;

   if(ret && !singleton().m_jsoncfg.empty()){
      ret &= singleton().parseJson();
      std::cout<<"json config parse - "<<ret<<std::endl;
   }

   ret &= singleton().validateOptions();
   std::cout<<"validate options - "<<ret<<std::endl;

   return ret;
}


bool Options::parseInputOptions( int argc, char **argv )
{
   int c;
   int option_index = 0;
   bool result = true;
   bool ffound = false;

   struct option long_options[] = {
      { "rulecallbackthreads",   required_argument,  NULL, 'b' },
      { "fdcfg",                 required_argument,  NULL, 'c' },
      { "selfredirect",          no_argument,        NULL, 'd' },
      { "enableruletimers",      no_argument,        NULL, 'e' },
      { "rulecallbackurl",       required_argument,  NULL, 'f' },
      { "chronosapiurl",         required_argument,  NULL, 'g' },
      { "help",                  no_argument,        NULL, 'h' },
      { "jsoncfg",               required_argument,  NULL, 'j' },
      { "casskeyspace",          required_argument,  NULL, 'k' },
      { "logname",               required_argument,  NULL, 'l' },
      { "lognumber",             required_argument,  NULL, 'n' },
      { "rulecallbackport",      required_argument,  NULL, 'p' },
      { "logqsize",              required_argument,  NULL, 'q' },
      { "originrealm",           required_argument,  NULL, 'r' },
      { "originhost",            required_argument,  NULL, 's' },
      { "casshost",              required_argument,  NULL, 't' },
      { "logsize",               required_argument,  NULL, 'z' },
      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "b:c:def:g:hj:k:l:n:p:q:r:s:t:z:", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'b': { m_rulecallbackthreads = atoi(optarg);  m_options |= opt_rulecallbackthreads; break; }
         case 'c': { m_fdcfg = optarg;                      m_options |= opt_fdcfg; break; }
         case 'd': { m_selfredirect = true;                 m_options |= opt_selfredirect; break; }
         case 'e': { m_enableruletimers = true;             m_options |= opt_enableruletimers; break; }
         case 'f': { m_rulecallbackurl = optarg;            m_options |= opt_rulecallbackurl; break; }
         case 'g': { m_chronosapiurl = optarg;              m_options |= opt_chronosapiurl; break; }
         case 'h': { help(); exit(0); }
         case 'j': { m_jsoncfg = optarg;                    m_options |= opt_jsoncfg; break; }
         case 'k': { m_cassKeyspace = optarg;               m_options |= opt_cassks; break; }
         case 'l': { m_logfilename = optarg;                m_options |= opt_logfilename; break; }
         case 'n': { m_lognbrfiles = atoi(optarg);          m_options |= opt_lognbrfiles; break; }
         case 'p': { m_rulecallbackport = atoi(optarg);     m_options |= opt_rulecallbackport; break; }
         case 'q': { m_logqueuesize = atoi(optarg);         m_options |= opt_logqueuesize; break; }
         case 'r': { m_originrealm = optarg;                m_options |= opt_originrealm; break; }
         case 's': { m_originhost = optarg;                 m_options |= opt_originhost; break; }
         case 't': { m_cassHost = optarg;                   m_options |= opt_casshost; break; }
         case 'z': { m_logmaxsize = atoi(optarg);           m_options |= opt_logmaxsize; break; }
         case '?':
         {
            switch ( optopt )
            {
               case 'b': { std::cout << "Option -b (rule callback threads) requires an argument" << std::endl; break; }
               case 'c': { std::cout << "Option -c (diameter configuration file) requires an argument" << std::endl; break; }
               case 'f': { std::cout << "Option -f (rule callback url) requires an argument" << std::endl; break; }
               case 'g': { std::cout << "Option -g (Chronos API url) requires an argument" << std::endl; break; }
               case 'j': { std::cout << "Option -j (json config) requires an argument" << std::endl; break; }
               case 'k': { std::cout << "Option -k (Cassandra keyspace) requires an argument" << std::endl; break; }
               case 'l': { std::cout << "Option -l (log filename) requires an argument" << std::endl; break; }
               case 'n': { std::cout << "Option -n (log number of files) requires an argument" << std::endl; break; }
               case 'p': { std::cout << "Option -p (rule callback port) requires an argument" << std::endl; break; }
               case 'q': { std::cout << "Option -q (log queue size) requires an argument" << std::endl; break; }
               case 'r': { std::cout << "Option -r (origin realm) requires an argument" << std::endl; break; }
               case 's': { std::cout << "Option -s (origin host) requires an argument" << std::endl; break; }
               case 't': { std::cout << "Option -t (Cassandra host) requires an argument" << std::endl; break; }
               case 'z': { std::cout << "Option -z (log max size) requires an argument" << std::endl; break; }
               default: { std::cout << "Unrecognized option [" << c << "]" << std::endl; break; }
            }
            result = false;
         }
         default:
         {
            std::cout << "Unrecognized option [" << c << "]" << std::endl;
            result = false;
         }
      }
   }

   return result;
}

bool Options::parseJson()
{
   char buf[2048];

   FILE *fp = fopen ( m_jsoncfg.c_str() , "r");
   RAPIDJSON_NAMESPACE::FileReadStream is( fp, buf, sizeof(buf) );
   RAPIDJSON_NAMESPACE::Document doc;
   doc.ParseStream<0>( is );
   fclose( fp );

   if(!doc.IsObject()) {
      std::cout << "Error parsing the json config file [" << m_jsoncfg << "]" << std::endl;
      return false;
   }

   if(doc.HasMember("common")){
      const RAPIDJSON_NAMESPACE::Value& commonSection = doc["common"];
      if(!(m_options & opt_fdcfg) && commonSection.HasMember("fdcfg")){
         if(!commonSection["fdcfg"].IsString()){ std::cout << "Error parsing json value: [fdcfg]" << std::endl; return false; }
         m_fdcfg = commonSection["fdcfg"].GetString();
         m_options |= opt_fdcfg;
      }
      if(!(m_options & opt_originhost) && commonSection.HasMember("originhost")){
         if(!commonSection["originhost"].IsString()) { std::cout << "Error parsing json value: [originhost]" << std::endl; return false; }
         m_originhost = commonSection["originhost"].GetString();
         m_options |= opt_originhost;
      }
      if(!(m_options & opt_originrealm) && commonSection.HasMember("originrealm")){
         if(!commonSection["originrealm"].IsString()) { std::cout << "Error parsing json value: [originrealm]" << std::endl; return false; }
         m_originrealm = commonSection["originrealm"].GetString();
         m_options |= opt_originrealm;
      }
   }
   if(doc.HasMember("pcrf"))
	{
      const RAPIDJSON_NAMESPACE::Value& pcrfSection = doc["pcrf"];
      if(!(m_options & opt_casshost) && pcrfSection.HasMember("casshost")){
         if(!pcrfSection["casshost"].IsString()) { std::cout << "Error parsing json value: [casshost]" << std::endl; return false; }
         m_cassHost = pcrfSection["casshost"].GetString();
         m_options |= opt_casshost;
      }
      if(!(m_options & opt_cassks) && pcrfSection.HasMember("casskeyspace")){
         if(!pcrfSection["casskeyspace"].IsString()) { std::cout << "Error parsing json value: [casskeyspace]" << std::endl; return false; }
         m_cassKeyspace = pcrfSection["casskeyspace"].GetString();
         m_options |= opt_cassks;
      }
      if(!(m_options & opt_logmaxsize) && pcrfSection.HasMember("logsize")){
         if(!pcrfSection["logsize"].IsInt()) { std::cout << "Error parsing json value: [logsize]" << std::endl; return false; }
         m_logmaxsize = pcrfSection["logsize"].GetInt();
         m_options |= opt_logmaxsize;
      }
      if(!(m_options & opt_lognbrfiles) && pcrfSection.HasMember("lognumber")){
         if(!pcrfSection["lognumber"].IsInt()) { std::cout << "Error parsing json value: [lognumber]" << std::endl; return false; }
         m_lognbrfiles = pcrfSection["lognumber"].GetInt();
         m_options |= opt_lognbrfiles;
      }
      if(!(m_options & opt_logfilename) && pcrfSection.HasMember("logname")){
         if(!pcrfSection["logname"].IsString()) { std::cout << "Error parsing json value: [logname]" << std::endl; return false; }
         m_logfilename = pcrfSection["logname"].GetString();
         m_options |= opt_logfilename;
      }
      if(!(m_options & opt_logqueuesize) && pcrfSection.HasMember("logqsize")){
         if(!pcrfSection["logqsize"].IsInt()) { std::cout << "Error parsing json value: [logqsize]" << std::endl; return false; }
         m_logqueuesize = pcrfSection["logqsize"].GetInt();
         m_options |= opt_logqueuesize;
      }
      if(!(m_options & opt_selfredirect) && pcrfSection.HasMember("selfredirect")){
         if(!pcrfSection["selfredirect"].IsBool()) { std::cout << "Error parsing json value: [selfredirect]" << std::endl; return false; }
         m_selfredirect = pcrfSection["selfredirect"].GetBool();
         m_options |= opt_selfredirect;
      }
      if(!(m_options & opt_enableruletimers) && pcrfSection.HasMember("enableruletimers")){
         if(!pcrfSection["enableruletimers"].IsBool()) { std::cout << "Error parsing json value: [enableruletimers]" << std::endl; return false; }
         m_enableruletimers = pcrfSection["enableruletimers"].GetBool();
         m_options |= opt_enableruletimers;
      }
      if(!(m_options & opt_rulecallbackport) && pcrfSection.HasMember("rulecallbackport")){
         if(!pcrfSection["rulecallbackport"].IsInt()) { std::cout << "Error parsing json value: [rulecallbackport]" << std::endl; return false; }
         m_rulecallbackport = pcrfSection["rulecallbackport"].GetInt();
         m_options |= opt_rulecallbackport;
      }
      if(!(m_options & opt_rulecallbackthreads) && pcrfSection.HasMember("rulecallbackthreads")){
         if(!pcrfSection["rulecallbackthreads"].IsInt()) { std::cout << "Error parsing json value: [rulecallbackport]" << std::endl; return false; }
         m_rulecallbackport = pcrfSection["rulecallbackthreads"].GetInt();
         m_options |= opt_rulecallbackthreads;
      }
      if(!(m_options & opt_rulecallbackurl) && pcrfSection.HasMember("rulecallbackurl")){
         if(!pcrfSection["rulecallbackurl"].IsString()) { std::cout << "Error parsing json value: [rulecallbackport]" << std::endl; return false; }
         m_rulecallbackurl = pcrfSection["rulecallbackurl"].GetString();
         m_options |= opt_rulecallbackurl;
      }
      if(!(m_options & opt_chronosapiurl) && pcrfSection.HasMember("chronosapiurl")){
         if(!pcrfSection["chronosapiurl"].IsString()) { std::cout << "Error parsing json value: [rulecallbackport]" << std::endl; return false; }
         m_chronosapiurl= pcrfSection["chronosapiurl"].GetString();
         m_options |= opt_chronosapiurl;
      }

      if(!(m_options & opt_statlogmaxsize) && pcrfSection.HasMember("statlogsize")){
         if(!pcrfSection["statlogsize"].IsInt()) { std::cout << "Error parsing json value: [statlogsize]" << std::endl; return false; }
         m_statlogmaxsize = pcrfSection["statlogsize"].GetInt();
         m_options |= opt_statlogmaxsize;
      }
      if(!(m_options & opt_statlognbrfiles) && pcrfSection.HasMember("statlognumber")){
         if(!pcrfSection["statlognumber"].IsInt()) { std::cout << "Error parsing json value: [statlognumber]" << std::endl; return false; }
         m_statlognbrfiles = pcrfSection["statlognumber"].GetInt();
         m_options |= opt_statlognbrfiles;
      }
      if(!(m_options & opt_statlogfilename) && pcrfSection.HasMember("statlogname")){
         if(!pcrfSection["statlogname"].IsString()) { std::cout << "Error parsing json value: [statlogname]" << std::endl; return false; }
         m_statlogfilename = pcrfSection["statlogname"].GetString();
         m_options |= opt_statlogfilename;
      }
      if(!(m_options & opt_statfrequency) && pcrfSection.HasMember("statfreq")){
         if(!pcrfSection["statfreq"].IsInt64()) { std::cout << "Error parsing json value: [statfreq]" << std::endl; return false; }
         m_statsfrequency = pcrfSection["statfreq"].GetInt64();
         m_options |= opt_statfrequency;
      }

      if(!(m_options & opt_auditlogmaxsize) && pcrfSection.HasMember("auditlogsize")){
         if(!pcrfSection["auditlogsize"].IsInt()) { std::cout << "Error parsing json value: [auditlogsize]" << std::endl; return false; }
         m_auditlogmaxsize = pcrfSection["auditlogsize"].GetInt();
         m_options |= opt_auditlogmaxsize;
      }
      if(!(m_options & opt_auditlognbrfiles) && pcrfSection.HasMember("auditlognumber")){
         if(!pcrfSection["auditlognumber"].IsInt()) { std::cout << "Error parsing json value: [auditlognumber]" << std::endl; return false; }
         m_auditlognbrfiles = pcrfSection["auditlognumber"].GetInt();
         m_options |= opt_auditlognbrfiles;
      }
      if(!(m_options & opt_auditlogfilename) && pcrfSection.HasMember("auditlogname")){
         if(!pcrfSection["auditlogname"].IsString()) { std::cout << "Error parsing json value: [auditlogname]" << std::endl; return false; }
         m_auditlogfilename = pcrfSection["auditlogname"].GetString();
         m_options |= opt_auditlogfilename;
      }
      if(!(m_options & opt_ossport) && pcrfSection.HasMember("ossport")){
         if(!pcrfSection["ossport"].IsInt()) { std::cout << "Error parsing json value: [ossport]" << std::endl; return false; }
         m_ossport = pcrfSection["ossport"].GetInt();
         m_options |= opt_ossport;
      }
      if(!(m_options & opt_ossfile) && pcrfSection.HasMember("ossfile")){
         if(!pcrfSection["ossfile"].IsString()) { std::cout << "Error parsing json value: [ossfile]" << std::endl; return false; }
         m_ossfile = pcrfSection["ossfile"].GetString();
         m_options |= opt_ossfile;
      }
		if( /*!( m_options & opt_rulesfile ) &&*/ pcrfSection.HasMember( "rulesfile" ) ) 
		{
			if( !pcrfSection["rulesfile"].IsString() )
			{
				std::cout << "Error parsing json value: [rulesfile]" << std::endl;
				return false;
			}
			m_rulesfile = pcrfSection["rulesfile"].GetString();
			m_options |= opt_rulesfile;
			parseSubscriberProfiles( m_rulesfile.c_str() );
		}
      if(!(m_options & opt_restport) && pcrfSection.HasMember("restport")){
         if(!pcrfSection["restport"].IsInt()) { std::cout << "Error parsing json value: [restport]" << std::endl; return false; }
         m_restport = pcrfSection["restport"].GetInt();
         m_options |= opt_restport;
      }
   }
   return true;
}

bool Options::parseSubscriberProfiles( const char* jsonFile )
{
   FILE* fp = fopen( jsonFile, "r" );
   if ( fp == NULL )
   {
      std::cout << "RulesConfig file specified does not exists " << std::endl;
      return false;
   }
   char readBuffer[65536];
   RAPIDJSON_NAMESPACE::FileReadStream is( fp, readBuffer, sizeof( readBuffer ) );
   fclose( fp );
   RAPIDJSON_NAMESPACE::Document doc;
   doc.ParseStream( is );
   if( !doc.IsObject() )
   {
      std::cout << "Error Parsing the Json config file " << std::endl;
      return false;
   }
   m_policies_config = new PoliciesConfig();
   if( doc.HasMember( "Policies" ) )
   {
		
		const RAPIDJSON_NAMESPACE::Value& subPolicies = doc["Policies"];
		for( RAPIDJSON_NAMESPACE::Value::ConstMemberIterator service_policies_itr = subPolicies.MemberBegin(); service_policies_itr != subPolicies.MemberEnd(); ++service_policies_itr )
		{
			std::string service_group_name = service_policies_itr->name.GetString();
			if( strcmp( service_group_name.c_str(), "service-groups" ) == 0 )
			{
				const RAPIDJSON_NAMESPACE::Value& subServiceGroup = service_policies_itr->value;
				for( RAPIDJSON_NAMESPACE::Value::ConstMemberIterator sub_service_group_itr = subServiceGroup.MemberBegin(); sub_service_group_itr != subServiceGroup.MemberEnd(); ++sub_service_group_itr )
				{
					ServiceProfiles* service_profile = new ServiceProfiles();
					std::string service_group_name = sub_service_group_itr->name.GetString();
					service_profile->setServiceName( service_group_name );
					const RAPIDJSON_NAMESPACE::Value& subServiceSection = sub_service_group_itr->value;
					for( RAPIDJSON_NAMESPACE::Value::ConstMemberIterator sub_service_section_itr = subServiceSection.MemberBegin(); sub_service_section_itr != subServiceSection.MemberEnd(); ++sub_service_section_itr )
					{
						std::string activate_service_val;
						std::string activate_service_name = sub_service_section_itr->name.GetString();
						service_profile->setServiceType( activate_service_name );
						const RAPIDJSON_NAMESPACE::Value& service_section_value = sub_service_section_itr->value;
						for( uint32_t i = 0; i < sub_service_section_itr->value.Size(); i++ )
						{	
							// TODO : default-activate-service may contain multiple activate services, so need to maintain the list in ServiceProfiles class
							activate_service_val =  sub_service_section_itr->value[i].GetString();
						}
						// TODO: Add string list as a 2 type in map
						service_profile->add_service_type_map( activate_service_name, activate_service_val );
					}
					m_policies_config->add_service_group_map( service_group_name, service_profile );
				}
			}
			else
			if( strcmp( service_group_name.c_str(), "services" ) == 0 )
			{
				const RAPIDJSON_NAMESPACE::Value& subServiceSelection = service_policies_itr->value;
				for( RAPIDJSON_NAMESPACE::Value::ConstMemberIterator sub_service_selection_itr = subServiceSelection.MemberBegin(); sub_service_selection_itr != subServiceSelection.MemberEnd(); ++sub_service_selection_itr )
            {
					ServiceSelection* service_selection = new ServiceSelection();
					std::string service_selection_name = sub_service_selection_itr->name.GetString();
               service_selection->setServiceName( service_selection_name );
               const RAPIDJSON_NAMESPACE::Value& subServiceSection = sub_service_selection_itr->value;
					
               for( RAPIDJSON_NAMESPACE::Value::ConstMemberIterator sub_service_section_itr1 = subServiceSection.MemberBegin(); sub_service_section_itr1 != subServiceSection.MemberEnd(); ++sub_service_section_itr1 )
               {
						if( strcmp( service_selection_name.c_str(), "video-non-gbr-1" ) == 0 )
						{
							continue;
						}
						
						if( strcmp( sub_service_section_itr1->name.GetString(), "qci" ) == 0 )
						{
							service_selection->setQci( sub_service_section_itr1->value.GetInt() );
						}
						else
						if( strcmp( sub_service_section_itr1->name.GetString(), "arp" ) == 0 )
						{
							service_selection->setArp( sub_service_section_itr1->value.GetInt() );	
						}
						else
						if( strcmp( sub_service_section_itr1->name.GetString(), "AMBR_DL" ) == 0 )
						{
							service_selection->setAmbrDl( subServiceSection["AMBR_DL"].GetInt() );	
						}
						else
						if( strcmp( sub_service_section_itr1->name.GetString(), "AMBR_UL" ) == 0 )
						{
							service_selection->setAmbrUl( subServiceSection["AMBR_UL"].GetInt() );
						}
						else
						if( strcmp( sub_service_section_itr1->name.GetString(), "service-activation-rules" ) == 0 )
						{
							std::string activation_rule;
							const RAPIDJSON_NAMESPACE::Value& subServiceActivationRules = subServiceSection["service-activation-rules"];
							for( uint32_t i = 0; i < sub_service_section_itr1->value.Size(); i++ )
							{
								activation_rule =  sub_service_section_itr1->value[i].GetString();	
								service_selection->add_activation_rules_index_list( i );
								service_selection->add_activation_rules_map( i, activation_rule );	
							}
						}
					}
					m_policies_config->add_service_selection_map( service_selection_name, service_selection );
				}	
			}
			else
			if( strcmp( service_group_name.c_str(), "rules" ) == 0 )
			{
				const RAPIDJSON_NAMESPACE::Value& subRule = service_policies_itr->value;
            for( RAPIDJSON_NAMESPACE::Value::ConstMemberIterator sub_rule_itr = subRule.MemberBegin(); sub_rule_itr != subRule.MemberEnd(); ++sub_rule_itr )
            {
					std::string rule_name = sub_rule_itr->name.GetString();
					ConfigRule* config_rule = new ConfigRule();

					const RAPIDJSON_NAMESPACE::Value& subRuleObject = sub_rule_itr->value;
            	for( RAPIDJSON_NAMESPACE::Value::ConstMemberIterator sub_rule_selection_itr = subRuleObject.MemberBegin(); sub_rule_selection_itr != subRuleObject.MemberEnd(); ++sub_rule_selection_itr )
            	{	
						const RAPIDJSON_NAMESPACE::Value& subRuleDefinition = sub_rule_selection_itr->value;
						RAPIDJSON_NAMESPACE::StringBuffer buffer;
						RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer( buffer );
						subRuleDefinition.Accept( writer );
						//printf( "SOHAN : Definition String : %s\n", buffer.GetString() );
						config_rule->setDefinition( buffer.GetString() );
						if( subRuleDefinition.HasMember( "Charging-Rule-Name" ) )
						{
							std::string rule_name = subRuleDefinition["Charging-Rule-Name"].GetString();
							config_rule->setRuleName( rule_name );
						}
						if( subRuleDefinition.HasMember( "QoS-Information" ) )
						{
							const RAPIDJSON_NAMESPACE::Value& qosInformation = subRuleDefinition["QoS-Information"];
							if( qosInformation.HasMember( "QoS-Class-Identifier" ) )
							{
								int qci = qosInformation["QoS-Class-Identifier"].GetInt();
								config_rule->setQci( qci );
							}
							if( qosInformation.HasMember( "Max-Requested-Bandwidth-UL" ) )
							{
								int max_requested_bandwidth_ul = qosInformation["Max-Requested-Bandwidth-UL"].GetInt();
								config_rule->setMaxRequestedBandwidthUl( max_requested_bandwidth_ul );
							}
							if( qosInformation.HasMember( "Max-Requested-Bandwidth-DL" ) )
							{
								int max_requested_bandwidth_dl = qosInformation["Max-Requested-Bandwidth-DL"].GetInt();
								config_rule->setMaxRequestedBandwidthDl( max_requested_bandwidth_dl );
							}
							if( qosInformation.HasMember( "Guaranteed-Bitrate-UL" ) )
							{
								int guaranteed_bitrate_ul = qosInformation["Guaranteed-Bitrate-UL"].GetInt();
								config_rule->setGuaranteedBitrateUl( guaranteed_bitrate_ul );
							}
							if( qosInformation.HasMember( "Guaranteed-Bitrate-DL" ) )
							{
								int guaranteed_bitrate_dl = qosInformation["Guaranteed-Bitrate-DL"].GetInt();
								config_rule->setGuaranteedBitrateDl( guaranteed_bitrate_dl );
							}
							if( qosInformation.HasMember( "Allocation-Retention-Priority" ) )
							{
								const RAPIDJSON_NAMESPACE::Value& arp = qosInformation["Allocation-Retention-Priority"];
								if( arp.HasMember( "Priority-Level" ) )
								{
									int priority_level = arp["Priority-Level"].GetInt();
									config_rule->setPriorityLevel( priority_level );
								}
								if( arp.HasMember( "Pre-emption-Capability" ) ) 
								{
									int preemption_capability = arp["Pre-emption-Capability"].GetInt();
									config_rule->setPreemptionCapability( preemption_capability );
								}
								if( arp.HasMember( "Pre-emption-Vulnerability" ) )
								{
									int preemption_vulnerability = arp["Pre-emption-Vulnerability"].GetInt();
									config_rule->setPreemptionVulnerability( preemption_vulnerability );
								}
							}
							if( qosInformation.HasMember( "APN-Aggregate-Max-Bitrate-UL" ) )
							{
								int aggregate_max_bitrate_ul = qosInformation["APN-Aggregate-Max-Bitrate-UL"].GetInt();
								config_rule->setApnAggregateMaxBitrateUl( aggregate_max_bitrate_ul );
							} 
							if( qosInformation.HasMember( "APN-Aggregate-Max-Bitrate-DL" ) )
							{
								int aggregate_max_bitrate_dl = qosInformation["APN-Aggregate-Max-Bitrate-DL"].GetInt();
								config_rule->setApnAggregateMaxBitrateDl( aggregate_max_bitrate_dl );
							}
						}
					}
					m_policies_config->add_config_rule_map( rule_name, config_rule );
				}
			}
		}
   }
}

bool Options::validateOptions()
{
   return ( m_options & opt_jsoncfg )
      && ( m_options & opt_originhost )
      && ( m_options & opt_originrealm )
      && ( m_options & opt_fdcfg )
      && ( m_options & opt_casshost )
      && ( m_options & opt_cassks )
      && ( m_options & opt_logmaxsize )
      && ( m_options & opt_lognbrfiles )
      && ( m_options & opt_logfilename )
      //&& ( m_options & opt_selfredirect )
      //&& ( m_options & opt_enableruletimers )
      //&& ( m_options & opt_rulecallbackport )
      //&& ( m_options & opt_rulecallbackthreads )
      //&& ( m_options & opt_rulecallbackurl )
      //&& ( m_options & opt_chronosapiurl )
      && ( m_options & opt_statlogfilename )
      && ( m_options & opt_statlognbrfiles )
      && ( m_options & opt_statlogmaxsize )
      && ( m_options & opt_statfrequency )
      && ( m_options & opt_auditlogfilename )
      && ( m_options & opt_auditlognbrfiles )
      && ( m_options & opt_auditlogmaxsize )
      && ( m_options & opt_ossport )
      && ( m_options & opt_ossfile )
      && ( m_options & opt_rulesfile );
}

