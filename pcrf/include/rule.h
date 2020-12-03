/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __RULE_H
#define __RULE_H

#include <string>
#include <list>
#include <map>
#include <set>
#include <algorithm>

#include <pistache/client.h>

#include "ssync.h"
#include "options.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class GxSession;
class RuleTimer;

class Rule
{
public:
   Rule() : m_active_toggle(true), m_default_rule_flag(false) {}
   ~Rule() {}

   const std::string &getRuleName() { return m_rulename; }
   const std::string &getBaseName() { return m_basename; }
   const std::string &getType() { return m_type; }
   const std::string &getDefinition() { return m_definition; }
   const std::string &getTimeOfDay() { return m_timeofday; }
   const std::string &getUsageMonitoringInfo() { return m_usage_monitoring_info; }
   bool getSyRequired() { return m_syrequired; }
   uint64_t getTimeMask() { return m_timemask; }
   uint64_t getFeatureMask() { return m_featuremask; }
   bool getActiveNow() { return m_active_toggle; }
   bool getDefaultFlag() { return m_default_rule_flag; }

   const std::string &setRuleName( const char *v ) { m_rulename = v; return getRuleName(); }
   const std::string &setRuleName( const std::string &v ) { m_rulename = v; return getRuleName(); }
   const std::string &setBaseName( const char *v ) { m_basename = v; return getBaseName(); }
   const std::string &setBaseName( const std::string &v ) { m_basename = v; return getBaseName(); }
   const std::string &setType( const char *v ) { m_type = v; return getType(); }
   const std::string &setType( const std::string &v ) { m_type = v; return getType(); }
   const std::string &setDefinition( const char *v ) { m_definition = v; return getDefinition(); }
   const std::string &setDefinition( const std::string &v ) { m_definition = v; return getDefinition(); }
   const std::string &setTimeOfDay( const char *v ) { m_timeofday = v; return getTimeOfDay(); }
   const std::string &setTimeOfDay( const std::string &v ) { m_timeofday = v; return getTimeOfDay(); }
   const std::string &setUsageMonitoringInfo( const char *v ) { m_usage_monitoring_info = v; return getUsageMonitoringInfo(); }
   const std::string &setUsageMonitoringInfo( const std::string &v ) { m_usage_monitoring_info = v; return getUsageMonitoringInfo(); }
   bool setSyRequired( bool v ) { m_syrequired = v; return getSyRequired(); }
   uint64_t setTimeMask( uint64_t v ) { m_timemask = v; return getTimeMask(); }
   uint64_t setFeatureMask( uint64_t v ) { m_featuremask = v; return getFeatureMask(); }
   bool setActiveNow( bool v ) { m_active_toggle = v; return getActiveNow(); }
   bool setDefaultFlag( bool flag ) { m_default_rule_flag = flag; return getDefaultFlag(); }

   bool getTimeSensitive() { return m_timemask != 0; }

   RuleTimer *getRuleTimer() { return m_rt; }
   RuleTimer *setRuleTimer( RuleTimer *rt ) { return m_rt = rt; }

   //
   // the activeNow() and getTimerInterval() will have to be updated
   // to reflect evaluation of the timemask when it's definition is available
   //
   bool activeNow()           { m_active_toggle = !m_active_toggle; return !Options::enableRuleTimers() || !m_active_toggle; }
//   int getTimerInterval()     { static int val = 20; if (val == 20) { val=1000; return 20; } return val; }
   int getTimerInterval()     { return 10; }

private:
   std::string m_rulename;
   std::string m_basename;
   std::string m_type;
   std::string m_definition;
   std::string m_timeofday;
   std::string m_usage_monitoring_info;
   bool m_syrequired;
   uint64_t m_timemask;
   uint64_t m_featuremask;
   RuleTimer *m_rt;
   bool m_active_toggle;
	bool m_default_rule_flag;
};

class GxChargingRuleReport
{
public :

	std::string &getRuleName() { return m_rulename; }
   std::string &getBaseName() { return m_basename; }
   int getPccStatus() { return m_pcc_status; }
   int getRuleFailureCode() { return m_rule_failure_code; }

	std::string &setRuleName( const char *v ) { m_rulename = v; return getRuleName(); }
   std::string &setRuleName( const std::string &v ) { m_rulename = v; return getRuleName(); }
   std::string &setBaseName( const char *v ) { m_basename = v; return getBaseName(); }
   std::string &setBaseName( const std::string &v ) { m_basename = v; return getBaseName(); }
   void setPccStatus( int status ) { m_pcc_status = status; }
   void setRuleFailureCode( int failure_code ) { m_rule_failure_code = failure_code; }
private :
   std::string m_rulename;
   std::string m_basename;
   int m_pcc_status;
   int m_rule_failure_code;
};

class RulesMap : public std::map<std::string,Rule*>
{
public:
   RulesMap();
   ~RulesMap();

   bool getRule( const std::string &rulename, Rule* &rule );
};

class RulesList
{
public:
   RulesList( bool free_on_destroy = false );
   ~RulesList();

   RulesList &operator=( const RulesList &rl )
   {
      m_rules = rl.m_rules;
      return *this;
   }

   void push_back( Rule *r );
   bool exists( Rule *r );
   bool erase( Rule *r );
   std::list<Rule*>::iterator erase( std::list<Rule*>::iterator &it );

   std::list<Rule*>::iterator begin() { return m_rules.begin(); }
   std::list<Rule*>::iterator end() { return m_rules.end(); }
   std::list<Rule*>::const_iterator begin() const { return m_rules.begin(); }
   std::list<Rule*>::const_iterator end() const { return m_rules.end(); }

   void clear() { m_rules.clear(); }
   bool empty() const { return m_rules.empty(); }
   size_t size() const { return m_rules.size(); }

   const std::list<Rule*> &getRules() const { return m_rules; }

   void addGxSession( GxSession *gx );
   void removeGxSession( GxSession *gx );

private:
   std::list<Rule*>::iterator find( Rule *r )
   {
      return std::find( m_rules.begin(), m_rules.end(), r );
   }

   bool m_free_on_destroy;
   std::list<Rule*> m_rules;
};

class RulesReportList
{
public:
   RulesReportList( bool free_on_destroy = false );
   ~RulesReportList();

   RulesReportList &operator=( const RulesReportList &rl )
   {
      m_rules = rl.m_rules;
      return *this;
   }

   void push_back( GxChargingRuleReport *r );
   bool exists( GxChargingRuleReport *r );
   bool erase( GxChargingRuleReport *r );
   std::list<GxChargingRuleReport*>::iterator erase( std::list<GxChargingRuleReport*>::iterator &it );

   std::list<GxChargingRuleReport*>::iterator begin() { return m_rules.begin(); }
   std::list<GxChargingRuleReport*>::iterator end() { return m_rules.end(); }
   std::list<GxChargingRuleReport*>::const_iterator begin() const { return m_rules.begin(); }
   std::list<GxChargingRuleReport*>::const_iterator end() const { return m_rules.end(); }

   void clear() { m_rules.clear(); }
   bool empty() const { return m_rules.empty(); }
   size_t size() const { return m_rules.size(); }

   const std::list<GxChargingRuleReport*> &getRules() const { return m_rules; }

   //void addGxSession( GxSession *gx );
   //void removeGxSession( GxSession *gx );

private:
   std::list<GxChargingRuleReport*>::iterator find( GxChargingRuleReport *r )
   {
      return std::find( m_rules.begin(), m_rules.end(), r );
   }

   bool m_free_on_destroy;
   std::list<GxChargingRuleReport*> m_rules;
};


class GxSession;

class RuleEvaluator
{
public:
   RuleEvaluator();
   ~RuleEvaluator();

   void addGxInstallRule( Rule *rule )    { m_gxInstallRules.push_back( rule ); }
   void addGxRemoveRule( Rule *rule )     { m_gxRemoveRules.push_back( rule ); }
   void addGxPendingRule( Rule *rule )    { m_gxPendingRules.push_back( rule ); }

   void addSdInstallRule( Rule *rule )    { m_sdInstallRules.push_back( rule ); }
   void addSdRemoveRule( Rule *rule )     { m_sdRemoveRules.push_back( rule ); }

   void addStInstallRule( Rule *rule )    { m_stInstallRules.push_back( rule ); }
   void addStRemoveRule( Rule *rule )     { m_stRemoveRules.push_back( rule ); }

   RulesList &getGxInstallRules()         { return m_gxInstallRules; }
   RulesList &getGxRemoveRules()          { return m_gxRemoveRules; }
   RulesList &getGxPendingRules()          { return m_gxPendingRules; }

   RulesList &getSdInstallRules()         { return m_sdInstallRules; }
   RulesList &getSdRemoveRules()          { return m_sdRemoveRules; }

   RulesList &getStRemoveRules()          { return m_stRemoveRules; }
   RulesList &getStInstallRules()         { return m_stInstallRules; }

   bool evaluate( GxSession &pcef, const RulesList &rules, RulesList &gxInstalled, RulesList &sdInstalled, RulesList &stInstalled, bool failOnUninstallableRule );

private:
   RulesList m_gxInstallRules;
   RulesList m_sdInstallRules;
   RulesList m_stInstallRules;
   RulesList m_gxRemoveRules;
   RulesList m_sdRemoveRules;
   RulesList m_stRemoveRules;
   RulesList m_gxPendingRules;
};

class GxSession;

class RuleTimer
{
public:
   RuleTimer();
   ~RuleTimer();

   Rule *getRule() { return m_rule; }
   Rule *setRule( Rule *r ) { return m_rule = r; }

   const std::string &getRuleName() { return m_rule->getRuleName(); }

   const std::string &getTimerId() { return m_timerid; }
   const std::string &setTimerId( const char *s ) { return m_timerid = s; }
   const std::string &setTimerId( const std::string &s ) { return m_timerid = s; }

   bool addSession( GxSession *gx );
   void removeSession( GxSession *gx );

   void setNextInterval();
   void processIntervalExpiration();

private:
   SMutex m_mutex;
   Rule *m_rule;
   std::string m_timerid;
   std::set<GxSession*> m_sessions;
};

class RuleTimers
{
public:
   ~RuleTimers();

   static RuleTimers &singleton()
   {
      if ( !m_singleton )
      {
         SMutexLock l( m_mutex );
         if ( !m_singleton )
         {
            RuleTimers *temp = new RuleTimers();
            m_singleton = temp;
         }
      }

      return *m_singleton;
   }

   RuleTimer *addRuleTimer( Rule *rule );
   RuleTimer *getRuleTimer( const std::string &name );
   void removeRuleTimer( Rule *rule );

private:
   RuleTimers();

   static SMutex m_mutex;
   static RuleTimers *m_singleton;

   std::map<std::string,RuleTimer*> m_map;
};

#endif
