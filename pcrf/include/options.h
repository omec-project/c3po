/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <stdint.h>
#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
#include <algorithm>


class ConfigRule
{
public:
	ConfigRule();
	~ConfigRule();
	
	const std::string &setRuleName( const char *v ) { m_rule_name = v; return getRuleName(); }
   const std::string &setRuleName( const std::string &v ) { m_rule_name = v; return getRuleName(); }
   const std::string &getRuleName() const { return m_rule_name; }

	const std::string &setFlowInformation( const char *v ) { m_flow_information = v; return getFlowInformation(); }
   const std::string &setFlowInformation( const std::string &v ) { m_flow_information = v; return getFlowInformation(); }
   const std::string &getFlowInformation() const { return m_flow_information; }

	int getQci() { return m_qci; }
	int setQci( int v ) { m_qci = v; return getQci(); }	

	int getMaxRequestedBandwidthUl() { return m_max_requested_bandwidth_ul; }
	int setMaxRequestedBandwidthUl( int v ) { m_max_requested_bandwidth_ul = v; return getMaxRequestedBandwidthUl(); }

	int getMaxRequestedBandwidthDl() { return m_max_requested_bandwidth_dl; }
   int setMaxRequestedBandwidthDl( int v ) { m_max_requested_bandwidth_dl = v; return getMaxRequestedBandwidthDl(); }

	int getGuaranteedBitrateUl() { return m_guaranteed_bitrate_ul; }
	int setGuaranteedBitrateUl( int v ) { m_guaranteed_bitrate_ul = v; return getGuaranteedBitrateUl(); }

	int getGuaranteedBitrateDl() { return m_guaranteed_bitrate_dl; }
   int setGuaranteedBitrateDl( int v ) { m_guaranteed_bitrate_dl = v; return getGuaranteedBitrateDl(); }	

	int getApnAggregateMaxBitrateUl() { return m_apn_aggregate_max_bitrate_ul; }
	int setApnAggregateMaxBitrateUl( int v ) { m_apn_aggregate_max_bitrate_ul = v; return getApnAggregateMaxBitrateUl(); }

	int getApnAggregateMaxBitrateDl() { return m_apn_aggregate_max_bitrate_dl; }
   int setApnAggregateMaxBitrateDl( int v ) { m_apn_aggregate_max_bitrate_dl = v; return getApnAggregateMaxBitrateDl(); }

	int getPriorityLevel() { return m_priority_level; }
	int setPriorityLevel( int v ) { m_priority_level = v; return getPriorityLevel(); }
	
	int getPreemptionCapability() { return m_preemption_capability; }
	int setPreemptionCapability( int v ) { m_preemption_capability = v; return getPreemptionCapability(); }

	int getPreemptionVulnerability() { return m_preemption_vulnerability; }
	int setPreemptionVulnerability( int v ) { m_preemption_vulnerability = v; return getPreemptionVulnerability(); }

private:
	std::string m_rule_name;
	int m_qci;
	int m_max_requested_bandwidth_ul;
	int m_max_requested_bandwidth_dl;
	int m_guaranteed_bitrate_ul;
	int m_guaranteed_bitrate_dl;
	int m_apn_aggregate_max_bitrate_ul;
	int m_apn_aggregate_max_bitrate_dl;
	std::string m_flow_information;
	int m_priority_level;
	int m_preemption_capability;
	int m_preemption_vulnerability;
};

class ServiceSelection
{
public:
	ServiceSelection();
	~ServiceSelection();
	
	int getQci() { return m_qci; }
	int getArp() { return m_arp; }
	int getAmbrUl() { return m_ambr_ul; }
	int getAmbrDl() { return m_ambr_dl; }
	int setQci( int v ) { m_qci = v; return getQci(); }
	int setArp( int v ) { m_arp = v; return getArp(); }
	int setAmbrUl( int v ) { m_ambr_ul = v; return getAmbrUl(); }
	int setAmbrDl( int v ) { m_ambr_dl = v; return getAmbrDl(); }
	const std::string &setServiceName( const char *v ) { m_service_name = v; return getServiceName(); }
   const std::string &setServiceName( const std::string &v ) { m_service_name = v; return getServiceName(); }
	const std::string &getServiceName() const { return m_service_name; }	

	void add_activation_rules_list( std::string v );
   void remove_activation_rules_list( std::string& v );
   bool get_activation_rules_list( std::string v );

private:
	int m_qci;
	int m_arp;
	int m_ambr_ul;
	int m_ambr_dl;
	std::string m_service_name;
	std::list<std::string> m_activation_rules_list;
};

class ServiceProfiles
{
public:
	ServiceProfiles();
	~ServiceProfiles();
	const std::string &setServiceName( const char *v ) { m_service_name = v; return getServiceName(); }
   const std::string &setServiceName( const std::string &v ) { m_service_name = v; return getServiceName(); }

	const std::string &setServiceType( const char* v ) { m_service_type = v; return getServiceType(); }
	const std::string &setServiceType( const std::string& v ) { m_service_type = v; return getServiceType(); }

	const std::string &getServiceName() const { return m_service_name; }
	const std::string &getServiceType() const { return m_service_type; }
	
	void add_service_type_list( std::string v );
	void remove_service_type_list( std::string& v );
	bool get_service_type_list( std::string v );

	void add_service_type_map( std::string key, std::string val );
	void remove_service_type_map( std::string& v );
	std::string get_service_type_map( std::string& v );
private:	
	std::string m_service_name; // service_group_name;
	std::string m_service_type;
	std::list<std::string> m_service_type_list;
	std::unordered_map<std::string, std::string> m_service_type_map;
};


class PoliciesConfig
{
public:
	PoliciesConfig();
	~PoliciesConfig()
   {
      for (std::list<ServiceProfiles *> ::iterator it=service_profile_list.begin(); it!=service_profile_list.end(); ++it)
      {
         ServiceProfiles *service = *it;
         delete( service );
      }
       
	}
	void add_service_group_map( std::string service_name, ServiceProfiles* service_profile );
	void remove_service_group_map( std::string& service_name );
	ServiceProfiles* get_service_group_map( std::string& service_name );

	void add_service_selection_map( std::string service_selection_name, ServiceSelection* service_selection );
	void remove_service_selection_map( std::string& service_selection_name );
	ServiceSelection* get_service_selection_map( std::string& service_selection_name );

	void add_config_rule_map( std::string rule_name, ConfigRule* config_rule );
	ConfigRule* get_config_rule_map( std::string& rule_name );
	void remove_config_rule_map( std::string& rule_name );
public:
	std::list<ServiceProfiles*> service_profile_list;
	std::unordered_map<std::string, ServiceProfiles*> service_group_map;
	std::unordered_map<std::string, ServiceSelection*> service_selection_map;
	std::unordered_map<std::string, ConfigRule*> config_rule_map;
};

class Options
{
public:
   static bool parse( int argc, char **argv );
   static void cleanup() { delete m_singleton; }

   static const std::string &originHost()             { return singleton().m_originhost; }
   static const std::string &originRealm()            { return singleton().m_originrealm; }
   static const std::string &diameterConfiguration()  { return singleton().m_fdcfg; }
   static const std::string &cassandraHost()          { return singleton().m_cassHost; }
   static const std::string &cassandraKeyspace()      { return singleton().m_cassKeyspace; }

   static const int logMaxSize()                      { return singleton().m_logmaxsize; }
   static const int logNumberFiles()                  { return singleton().m_lognbrfiles; }
   static const std::string &logFilename()            { return singleton().m_logfilename; }
   static const int logQueueSize()                    { return singleton().m_logqueuesize; }

   static const int statlogMaxSize()                     { return singleton().m_statlogmaxsize; }
   static const int statlogNumberFiles()                 { return singleton().m_statlognbrfiles; }
   static const std::string &statlogFilename()           { return singleton().m_statlogfilename; }
   static const uint32_t statsFrequency()                { return singleton().m_statsfrequency; }

   static const int auditlogMaxSize()                    { return singleton().m_auditlogmaxsize; }
   static const int auditlogNumberFiles()                { return singleton().m_auditlognbrfiles; }
   static const std::string &auditlogFilename()          { return singleton().m_auditlogfilename; }

   static const bool selfRedirect()                   { return singleton().m_selfredirect; }

   static const bool enableRuleTimers()               { return singleton().m_enableruletimers; }
   static const int ruleCallbackPort()                { return singleton().m_rulecallbackport; }
   static const int ruleCallbackThreads()             { return singleton().m_rulecallbackthreads; }
   static const std::string &ruleCallbackUrl()        { return singleton().m_rulecallbackurl; }
   static const std::string &chronosApiUrl()          { return singleton().m_chronosapiurl; }

   static const int         &getossport()               { return singleton().m_ossport; }
   static const std::string &getossfile()               { return singleton().m_ossfile; }
   static const std::string &getrulesfile()               { return singleton().m_rulesfile; }
   static const PoliciesConfig& getPolicesConfig()            { return *( singleton().m_policies_config ); }

private:
   enum OptionsSelected {
     opt_jsoncfg              = 0x00000001,
     opt_originhost           = 0x00000002,
     opt_originrealm          = 0x00000004,
     opt_fdcfg                = 0x00000008,
     opt_casshost             = 0x00000010,
     opt_cassks               = 0x00000020,
     opt_logmaxsize           = 0x00000040,
     opt_lognbrfiles          = 0x00000080,
     opt_logfilename          = 0x00000100,
     opt_logqueuesize         = 0x00000200,
     opt_selfredirect         = 0x00000400,
     opt_enableruletimers     = 0x00000800,
     opt_rulecallbackport     = 0x00001000,
     opt_rulecallbackthreads  = 0x00002000,
     opt_rulecallbackurl      = 0x00004000,
     opt_chronosapiurl        = 0x00008000,
     opt_statlogfilename      = 0x00010000,
     opt_statlognbrfiles      = 0x00020000,
     opt_statlogmaxsize       = 0x00040000,
     opt_statfrequency        = 0x00080000,
     opt_auditlogfilename     = 0x00100000,
     opt_auditlognbrfiles     = 0x00200000,
     opt_auditlogmaxsize      = 0x00400000,
     opt_ossport              = 0x00800000,
     opt_ossfile              = 0x01000000,
     opt_rulesfile            = 0x02000000
   };

   static Options *m_singleton;
   static Options &singleton() { if ( !m_singleton ) m_singleton = new Options(); return *m_singleton; }
   static void help();

   Options();
   ~Options();

   bool parseInputOptions( int argc, char **argv );
   bool parseJson();
   bool parseSubscriberProfiles( const char* jsonFile );
   bool validateOptions();

   PoliciesConfig* m_policies_config;
   int m_options;

   std::string m_jsoncfg;
   std::string m_originhost;
   std::string m_originrealm;
   std::string m_fdcfg;
   std::string m_cassHost;
   std::string m_cassKeyspace;

   int m_logmaxsize;
   int m_lognbrfiles;
   std::string m_logfilename;
   int m_logqueuesize;

   int m_statlogmaxsize;
   int m_statlognbrfiles;
   std::string m_statlogfilename;
   uint32_t    m_statsfrequency;

   int m_auditlogmaxsize;
   int m_auditlognbrfiles;
   std::string m_auditlogfilename;

   bool m_selfredirect;
   bool m_enableruletimers;
   int m_rulecallbackport;
   int m_rulecallbackthreads;
   std::string m_rulecallbackurl;
   std::string m_chronosapiurl;

   int         m_ossport;
   std::string m_ossfile;
   std::string m_rulesfile;
};

#endif // #define __OPTIONS_H
