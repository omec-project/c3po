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


typedef struct access_profile
{
    char access_profile_name[64];

}access_profile_t;


/*
class AccessProfile
{
public:
   AccessProfile();
   ~AccessProfile();
   const std::string &getAccessProfileName() const { return m_access_profile_name; }
   const std::string &setAccessProfileName( const char *v ) { m_access_profile_name = v; return getAccessProfileName(); }
   const std::string &setAccessProfileName( const std::string &v ) { m_access_profile_name = v; return getAccessProfileName(); }
private:
   std::string m_access_profile_name;
};
*/

typedef struct qos_profile
{
    char qos_profile_name[64];
    uint32_t apn_ambr_ul;
    uint32_t apn_ambr_dl;
}qos_profile_t;


/*
class QosProfile
{
public:
   QosProfile();
   ~QosProfile();
   const std::string &getQosProfileName() const { return m_qos_profile_name; }
   const std::string &setQosProfileName( const char *v ) { m_qos_profile_name = v; return getQosProfileName(); }
   const std::string &setQosProfileName( const std::string &v ) { m_qos_profile_name = v; return getQosProfileName(); }
   uint32_t getApnAmbrUlVal() const { return m_apn_ambr_ul; }
   uint32_t getApnAmbrDlVal() const { return m_apn_ambr_dl; }
   
   uint32_t setApnAmbrUlVal( uint32_t v ) { m_apn_ambr_ul = v; return getApnAmbrUlVal(); }
   uint32_t setApnAmbrDlVal( uint32_t v ) { m_apn_ambr_dl = v; return getApnAmbrDlVal(); }
private:
   std::string m_qos_profile_name;
   uint32_t m_apn_ambr_ul;
   uint32_t m_apn_ambr_dl;
};
*/

typedef struct user_plane_profile
{
    char user_plane_profile_name[64];
    char user_plane_service[64];
    uint32_t upf_addr; // run time information 
    bool     global_address; // true : control plane allocates address, false : upf allocates address 
}user_plane_profile_t;

/*
class UserPlaneProfile
{
public:
   UserPlaneProfile();
   ~UserPlaneProfile();
   
   const std::string &getUserPlaneProfileName() const { return m_user_plane_profile_name; }
   const std::string &setUserPlaneProfileName( const char *v ) { m_user_plane_profile_name = v; return getUserPlaneProfileName(); }
   const std::string &setUserPlaneProfileName( const std::string &v ) { m_user_plane_profile_name = v; return getUserPlaneProfileName(); }
   
   
   const std::string &getUserPlaneService() const { return m_user_plane_service; }
   const std::string &setUserPlaneService( const char *v ) { m_user_plane_service = v; return getUserPlaneService(); }
   const std::string &setUserPlaneService( const std::string &v ) { m_user_plane_service = v; return getUserPlaneService(); }
   
   uint32_t getUpfAddr() { return m_upf_addr; }
   uint32_t setUpfAddr( uint32_t v ) { m_upf_addr = v; return getUpfAddr(); }
   
   bool getGlobalAddress() { return m_global_address; }
   bool setGlobalAddress( bool v ) { m_global_address = v; return getGlobalAddress(); }
   
private:
   std::string m_user_plane_profile_name;
   std::string m_user_plane_service;
   uint32_t m_upf_addr;
   bool m_global_address;
};
*/

#define MAX_NETCAP_LEN               (64)
typedef struct apn_profile
{
    char apn_profile_name[64];
    char apn_name[64];
    uint8_t apn_name_length;
    int apn_usage_type;
    char apn_net_cap[MAX_NETCAP_LEN];
    bool gx_enabled;
    uint32_t dns_primary;
    uint32_t dns_secondary;
    uint16_t mtu;
} apn_profile_t;

/*
class ApnProfile
{
public:
   ApnProfile();
   ~ApnProfile();
   const std::string &getApnProfileName() const { return m_apn_profile_name; }
   const std::string &setApnProfileName( const char *v ) { m_apn_profile_name = v; return getApnProfileName(); }
   const std::string &setApnProfileName( const std::string &v ) { m_apn_profile_name = v; return getApnProfileName(); }
   
   const std::string &getApnName() const { return m_apn_name; }
   const std::string &setApnName( const char *v ) { m_apn_name = v; return getApnName(); }
   const std::string &setApnName( const std::string &v ) { m_apn_name = v; return getApnName(); }
   
   uint8_t setApnNameLength( uint8_t v ) { m_apn_name_length = v; return getApnNameLength(); }
   uint8_t getApnNameLength() { return m_apn_name_length; }
   
   int setApnUsageType( int v ) { m_apn_usage_type = v; return getApnUsageType(); }
   int getApnUsageType() { return m_apn_usage_type; }
   
   const std::string &getApnNetCap() const { return m_apn_net_cap; }
   const std::string &setApnNetCap( const char *v ) { m_apn_net_cap = v; return getApnNetCap(); }
   const std::string &setApnNetCap( const std::string &v ) { m_apn_net_cap = v; return getApnNetCap(); }
   
   bool getGxEnabled() { return m_gx_enabled; }
   bool setGxEnabled( bool v ) { m_gx_enabled = v; return getGxEnabled(); }
   
   uint32_t getDnsPrimary() { return m_dns_primary; }
   uint32_t setDnsPrimary( uint32_t v ) { m_dns_primary = v; return getDnsPrimary(); }
   
   uint32_t getDnsSecondary() { return m_dns_secondary; }
   uint32_t setDnsSecondary( uint32_t v ) { m_dns_secondary = v; return getDnsSecondary(); }
   
   uint16_t getMtu() { return m_mtu; }
   uint16_t setMtu( uint16_t v ) { m_mtu = v; return getMtu(); }
   
private:
   std::string m_apn_profile_name;
   std::string m_apn_name;
   uint8_t m_apn_name_length;
   int m_apn_usage_type;
   //char m_apn_net_cap[MAX_NETCAP_LEN];
   std::string m_apn_net_cap;
   bool m_gx_enabled;
   uint32_t m_dns_primary;
   uint32_t m_dns_secondary;
   uint16_t m_mtu;
};
*/

struct sub_selection_rule
{
    uint32_t rule_priority;
    sub_selection_keys_t *keys;
    char* selected_apn_profile;
    char* selected_user_plane_profile;
    char* selected_qos_profile;
    char* selected_access_profile[4]; /* 4 access profiles per subscriber */

};
typedef struct sub_selection_rule sub_selection_rule_t;

typedef struct sub_selection_keys
{
    imsi_selection_t imsi;
    serving_plmn_t plmn;
    apn_key_t      apn;
} sub_selection_keys_t;


typedef struct pcrf_config_profile
{
   void *config; /* C code should not typecast this to any structure. */
}pcrf_config_profile_t;



class RuleOptions
{
public:
	RuleOptions();
	~RuleOptions()
   {
      for (std::list<sub_selection_rule_t *> ::iterator it=sub_sel_rules.begin(); it!=sub_sel_rules.end(); ++it)
      {
         sub_selection_rule_t *rule = *it;
         delete(rule->keys);
         free(rule->selected_apn_profile);
         free(rule->selected_user_plane_profile);
         free(rule->selected_qos_profile);
         free(rule->selected_access_profile[0]);
         free(rule->selected_access_profile[1]);
         free(rule->selected_access_profile[2]);
         free(rule->selected_access_profile[3]);
       }
       
      for (std::list<apn_profile_t*>::iterator it=apn_profile_list.begin(); it!=apn_profile_list.end(); ++it)
      {
         apn_profile_t *apn=*it;
         delete apn;
      }
      for (std::list<user_plane_profile_t*>::iterator it=user_plane_list.begin(); it!=user_plane_list.end(); ++it)
      {
         user_plane_profile_t *up=*it;
         delete up;
      }
      for (std::list<qos_profile_t*>::iterator it=qos_profile_list.begin(); it!=qos_profile_list.end(); ++it)
      {
         qos_profile_t *qos=*it;
         delete qos;
      }
      for (std::list<access_profile_t*>::iterator it=access_profile_list.begin(); it!=access_profile_list.end(); ++it)
      {
         access_profile_t *access=*it;
         delete access;
      }
	}
   
   apn_profile_t* get_apn_profile(char *name)
   {
      for (std::list<apn_profile_t*>::iterator it=apn_profile_list.begin(); it!=apn_profile_list.end(); ++it)
      {
         apn_profile_t *apn=*it;
         if(strcmp(apn->apn_profile_name, name) == 0)
         {
             return apn;
         }
      }
      return nullptr;
   }
   
   user_plane_profile_t* get_user_plane_profile( char* name )
   {
      for (std::list<user_plane_profile_t*>::iterator it=user_plane_list.begin(); it!=user_plane_list.end(); ++it)
      {
         user_plane_profile_t *up=*it;
         if(strcmp(up->user_plane_profile_name, name) == 0)
         {
            return up;
         }
      }
      return nullptr;
   }
   
   qos_profile_t* get_qos_profile( char* name )
   {
      for (std::list<qos_profile_t*>::iterator it=qos_profile_list.begin(); it!=qos_profile_list.end(); ++it)
      {
         qos_profile_t *qos=*it;
         if(strcmp(qos->qos_profile_name, name) == 0)
         {
            return qos;
         }
      }
      return nullptr;
   }
   
   access_profile_t* get_access_profile( char* name )
   {
      for (std::list<access_profile_t*>::iterator it=access_profile_list.begin(); it!=access_profile_list.end(); ++it)
      {
          access_profile_t *access=*it;
          if(strcmp(access->access_profile_name, name) == 0)
          {
             return access;
          }
          return nullptr;
      }
   }
public:
	std::list<sub_selection_rule_t *> sub_sel_rules;
	std::list<apn_profile_t *> apn_profile_list;
   std::list<user_plane_profile_t*> user_plane_list;
   std::list<access_profile_t*> access_profile_list;
	std::list<qos_profile_t*> qos_profile_list;
	std::list<std::string> service_groups_list;
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
   static const RulesOptions& getRulesOptions()            { return *(singleton().m_rules_options); }

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
     opt_rulesfile              = 0x01200000
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

   RulesOptions* m_rules_options;
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
