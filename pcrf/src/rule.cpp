/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <iostream>

#include <vector>

#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/client.h>

#include "options.h"
#include "logger.h"
#include "rule.h"
#include "session.h"
#include "chronos.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

RulesMap::RulesMap()
{
}

RulesMap::~RulesMap()
{
   RulesMap::iterator it;

   while ( (it = begin()) != end() )
   {
      delete it->second;
      erase( it );
   }
}

bool RulesMap::getRule( const std::string &rulename, Rule* &rule )
{
   auto it = find( rulename );
   if ( it == end() )
      return false;

   rule = it->second;
   return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

RulesList::RulesList( bool free_on_destroy )
   : m_free_on_destroy( free_on_destroy )
{
}

RulesList::~RulesList()
{
   if ( m_free_on_destroy )
   {
      std::list<Rule*>::iterator it;

      while ( (it = m_rules.begin()) != m_rules.end() )
      {
         delete *it;
         m_rules.erase( it );
      }
   }
}

void RulesList::push_back( Rule *r )
{
   m_rules.push_back( r );
}

bool RulesList::exists( Rule *r )
{
   return find( r ) != m_rules.end();
}

bool RulesList::erase( Rule *r )
{
   std::list<Rule*>::iterator it = find( r );
   if ( it != m_rules.end() )
   {
      m_rules.erase( it );
      return true;
   }

   return false;
}

std::list<Rule*>::iterator RulesList::erase( std::list<Rule*>::iterator &it )
{
   return m_rules.erase( it );
}

void RulesList::addGxSession( GxSession *gx )
{
   for ( auto r : m_rules )
      if ( r->getRuleTimer() )
         r->getRuleTimer()->addSession( gx );
}

void RulesList::removeGxSession( GxSession *gx )
{
   for ( auto r : m_rules )
      if ( r->getRuleTimer() )
         r->getRuleTimer()->removeSession( gx );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


RulesReportList::RulesReportList( bool free_on_destroy )
   : m_free_on_destroy( free_on_destroy )
{
}

RulesReportList::~RulesReportList()
{
   if ( m_free_on_destroy )
   {
      std::list<GxChargingRuleReport*>::iterator it;

      while ( (it = m_rules.begin()) != m_rules.end() )
      {
         delete *it;
         m_rules.erase( it );
      }
   }
}

void RulesReportList::push_back( GxChargingRuleReport *r )
{
   m_rules.push_back( r );
}

bool RulesReportList::exists( GxChargingRuleReport *r )
{
   return find( r ) != m_rules.end();
}

bool RulesReportList::erase( GxChargingRuleReport *r )
{
   std::list<GxChargingRuleReport*>::iterator it = find( r );
   if ( it != m_rules.end() )
   {
      m_rules.erase( it );
      return true;
   }

   return false;
}

std::list<GxChargingRuleReport*>::iterator RulesReportList::erase( std::list<GxChargingRuleReport*>::iterator &it )
{
   return m_rules.erase( it );
}

/*
void RulesReportList::addGxSession( GxSession *gx )
{
   for ( auto r : m_rules )
      if ( r->getRuleTimer() )
         r->getRuleTimer()->addSession( gx );
}

void RulesReportList::removeGxSession( GxSession *gx )
{
   for ( auto r : m_rules )
      if ( r->getRuleTimer() )
         r->getRuleTimer()->removeSession( gx );
}
*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

RuleEvaluator::RuleEvaluator()
{
}

RuleEvaluator::~RuleEvaluator()
{
}

bool RuleEvaluator::evaluate( GxSession &pcef, const RulesList &rules, RulesList &gxInstalled, RulesList &sdInstalled, RulesList &stInstalled, bool failOnUninstallableRule )
{
   bool result = true;

   //
   // determine which rules need to be activated now
   //
   uint64_t pcefFeatures = pcef.getSupportedFeatures();
   uint64_t tdfFeatures = pcef.getTdfSession().getSupportedFeatures();
   uint64_t tssfFeatures = pcef.getTssfSession().getSupportedFeatures();
   bool tdfRequired = pcef.getTdfSession().required();
   bool tssfRequired = pcef.getTssfSession().required();
   auto ruleit = rules.begin();

   while ( result && ruleit != rules.end() )
   {
      // check to see if the rule applies to the PCEF
		printf ("SOHAN : %s:%d RULE NAME : %s\n", __FILE__, __LINE__, (*ruleit)->getRuleName().c_str());
      if ( ( (*ruleit)->getFeatureMask() & pcefFeatures ) == (*ruleit)->getFeatureMask() )
      {
         if ( !Options::enableRuleTimers() || (*ruleit)->activeNow() )
         {
				if ( (*ruleit)->getDefaultFlag() == false && !m_gxPendingRules.exists( *ruleit ))
				{
					addGxPendingRule( *ruleit );
					printf ("SOHAN : ADD IN PENDING LIST : %s\n", (*ruleit)->getRuleName().c_str() );
				}
            else
				if ( !gxInstalled.exists( *ruleit ) )
				{
               addGxInstallRule( *ruleit );
					printf ("SOHAN : ADD IN INSTALL LIST : %s\n", (*ruleit)->getRuleName().c_str() );
				}
				
         }
         else
         {
            if ( gxInstalled.exists( *ruleit ) )
				{
               addGxRemoveRule( *ruleit );
				}
         }

         //ruleit = rules.getRules().erase( ruleit );
         //ruleit = rules.erase( ruleit );
      }
      // check to see if the rule applies to the TDF
      else if ( tdfRequired && ( (*ruleit)->getFeatureMask() & tdfFeatures ) == (*ruleit)->getFeatureMask() )
      {
         if ( !Options::enableRuleTimers() || (*ruleit)->activeNow() )
         {
            if ( !sdInstalled.exists( *ruleit ) )
                addSdInstallRule( *ruleit );
         }
         else
         {
            if ( sdInstalled.exists( *ruleit ) )
               addSdRemoveRule( *ruleit );
         }

         //ruleit = rules.erase( ruleit );
      }
      // check to see if the rule applies to the TSSF
      else if ( tssfRequired && ( (*ruleit)->getFeatureMask() & tssfFeatures ) == (*ruleit)->getFeatureMask() )
      {
         if ( !Options::enableRuleTimers() || (*ruleit)->activeNow() )
         {
            if ( !stInstalled.exists( *ruleit ) )
               addStInstallRule( *ruleit );
         }
         else
         {
            if ( stInstalled.exists( *ruleit ) )
               addStRemoveRule( *ruleit );
         }

         //ruleit = rules.erase( ruleit );
      }
      // check for fail on unassigned rule
      else if ( failOnUninstallableRule )
      {
         Logger::gx().error( "%s:%d - Aborting, unable to install rule=[%s] for imsi=[%s] apn=[%s]",
               __FILE__, __LINE__, pcef.getImsi().c_str(), pcef.getApn().c_str(), (*ruleit)->getRuleName().c_str() );
         result = false;
      }

      ruleit++;
   }

   return result;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

RuleTimer::RuleTimer()
{

}

RuleTimer::~RuleTimer()
{

}

bool RuleTimer::addSession( GxSession *gx )
{
   SMutexLock l( m_mutex );
   auto res = m_sessions.insert( gx );
   return res.second;
}

void RuleTimer::removeSession( GxSession *gx )
{
   SMutexLock l( m_mutex );
   m_sessions.erase( gx );
}

#define CHRONOS_POST( interval, repeat_for, uri, opaque, postbody ) \
{ \
   std::stringstream ss; \
   ss \
      << "{" \
         << "\"timing\": {" \
            << "\"interval\": " << interval << "," \
            << "\"repeat-for\": " << repeat_for \
         << "}," \
         << "\"callback\": {" \
            << "\"http\": {" \
               << "\"uri\": \"" << uri << "\"," \
               << "\"opaque\": \"" << opaque << "\"" \
            << "}" \
         << "}," \
         << "\"reliability\": {"  \
            << "\"replication-factor\": 99" \
         << "}" \
      << "}"; \
   postbody = ss.str(); \
}

void RuleTimer::setNextInterval()
{
   int interval = getRule()->getTimerInterval();

   if ( interval < 0 )
   {
      Logger::chronos().error( "%s:%d - the timer interval for [%s] is less than zero (%d)",
            __FILE__, __LINE__, getRule()->getRuleName().c_str(), interval );
      return;
   }

   //
   // build the request json
   //
   std::string postbody;
   CHRONOS_POST( interval, interval, Options::ruleCallbackUrl(), getRuleName(), postbody );

   //
   // setup the http client
   //
   std::string chronos = Options::chronosApiUrl() + "/timers";

   Logger::chronos().debug( "%s:%d - sending request to Chronos for [%s] interval [%d]",
         __FILE__, __LINE__, getRule()->getRuleName().c_str(), interval );
   auto resp = Chronos::client().post(chronos).body(postbody).send();
   resp.then( [&](Pistache::Http::Response response) {
      Logger::chronos().debug( "%s:%d - processing Chronos response",
            __FILE__, __LINE__ );
      if ( response.code() == Pistache::Http::Code::Ok ) {
         if ( response.headers().has("Location") ) {
            auto loc = response.headers().get<ChronosLocation>();
            setTimerId( loc->getLocation() );
            Logger::chronos().debug( "%s:%d - the TimerId for [%s] is [%s]",
                  __FILE__, __LINE__, getRule()->getRuleName().c_str(), loc->getLocation().c_str() );
         }
         else {
            Logger::chronos().error( "%s:%d - the Chronos POST response for [%s] does not contain the Location header",
                  __FILE__, __LINE__, getRule()->getRuleName().c_str() );
         }
      }
      else {
         Logger::chronos().error( "%s:%d - the Chronos POST for [%s] returned (%d)",
               __FILE__, __LINE__, getRule()->getRuleName().c_str(), response.code() );
      }
   }, Pistache::Async::IgnoreException );

   Logger::chronos().debug( "%s:%d - RuleTimer::setNextInterval() - exiting",
         __FILE__, __LINE__ );
}

void RuleTimer::processIntervalExpiration()
{
   Logger::chronos().debug( "%s:%d - RuleTimer::processIntervalExpiration() for [%s]",
         __FILE__, __LINE__, getRule()->getRuleName().c_str() );

   SMutexLock l( m_mutex );
   Rule *r = getRule();
   RulesList rl;

   for ( auto gx : m_sessions )
   {
      RuleEvaluator re;

      rl.clear();
      rl.push_back( r );

      bool evres = re.evaluate( *gx, rl, gx->getInstalledRules(),
            gx->getTdfSession().getInstalledRules(),
            gx->getTssfSession().getInstalledRules(),
            gx->getSubscriber().getFailOnUninstallableRule() );

      if ( evres )
      {
         GxProcessRulesUpdate *gxe = new GxProcessRulesUpdate( gx->getPCRF(), gx );
         SdProcessRulesUpdate *sde = new SdProcessRulesUpdate( gx->getPCRF(), &gx->getTdfSession() );
         StProcessRulesUpdate *ste = new StProcessRulesUpdate( gx->getPCRF(), &gx->getTssfSession() );

         if ( !gxe->processPhase1( re ) )
            delete gxe;
         if ( !sde->processPhase1( re ) )
            delete sde;
         if ( !ste->processPhase1( re ) )
            delete ste;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SMutex RuleTimers::m_mutex;
RuleTimers *RuleTimers::m_singleton = NULL;

RuleTimers::RuleTimers()
{
}

RuleTimers::~RuleTimers()
{
}

RuleTimer *RuleTimers::addRuleTimer( Rule *rule )
{
   if ( !Options::enableRuleTimers() || !rule->getTimeSensitive() )
      return NULL;

   SMutexLock l( m_mutex );
   auto it = m_map.find( rule->getRuleName() );

   if ( it == m_map.end() )
   {
      RuleTimer *rt = new RuleTimer();
      rt->setRule( rule );

      auto res = m_map.insert( std::pair<std::string,RuleTimer*>( rt->getRuleName(), rt ) );

      if ( res.second ) // insert succeeded
      {
         rule->setRuleTimer( rt );
      }
      else
      {
         Logger::gx().error( "%s:%d - Unable to insert RuleTimer for rule [%s]",
               __FILE__, __LINE__, rule->getRuleName().c_str() );
         delete rt;
      }
   }
   else
   {
      rule->setRuleTimer( it->second );
   }

   return rule->getRuleTimer();
}

RuleTimer *RuleTimers::getRuleTimer( const std::string &name )
{
   SMutexLock l( m_mutex );
   auto it = m_map.find( name );

   if ( it == m_map.end() )
      return NULL;

   return it->second;
}

void RuleTimers::removeRuleTimer( Rule *rule )
{
   RuleTimer *rt = rule->getRuleTimer();

   if ( rt )
   {
      SMutexLock l( m_mutex );
      m_map.erase( rule->getRuleName() );
      rule->setRuleTimer( NULL );
      delete rt;
   }
}
