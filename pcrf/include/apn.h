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

#ifndef __APN_H
#define __APN_H

#include <string>
#include <map>

#include "rule.h"

class Apn
{
public:
   Apn() {}
   ~Apn() {}

   const std::string &getApn() const { return m_apn; }
   const std::string &getSySubscriptionId() const { return m_sy_subscription_id; }
   bool getFailOnUninstallableRule() const { return m_fail_on_uninstallable_rule; }
   bool getAutoCreateSubscriber() const { return m_auto_create_subscriber; }
   const std::string &getMembershipValue() const { return m_membership_value; }
   const std::string &getDomain() const { return m_domain; }
   const RulesList &getComputedRules() const { return m_computed_rules; }
   bool getSyRequired() const { return m_syrequired; }
   int32_t getDefaultBearerCtlMode() const { return m_default_bearer_ctl_mode; }
   bool getForceDefaultBearerCtlMode() const { return m_force_default_bearer_ctl_mode; }

   const std::string &setApn( const char *v ) { m_apn = v; return getApn(); }
   const std::string &setApn( const std::string &v ) { m_apn = v; return getApn(); }
   const std::string &setSySubscriptionId( const char *v ) { m_sy_subscription_id = v; return getSySubscriptionId(); }
   const std::string &setSySubscriptionId( const std::string &v ) { m_sy_subscription_id = v; return getSySubscriptionId(); }
   bool setFailOnUninstallableRule( bool v ) { m_fail_on_uninstallable_rule = v; return getFailOnUninstallableRule(); }
   bool setAutoCreateSubscriber( bool v ) { m_auto_create_subscriber = v; return getAutoCreateSubscriber(); }
   const std::string &setMembershipValue( const char *v ) { m_membership_value = v; return getMembershipValue(); }
   const std::string &setMembershipValue( const std::string &v ) { m_membership_value = v; return getMembershipValue(); }
   const std::string &setDomain( const char *v ) { m_domain = v; return getDomain(); }
   const std::string &setDomain( const std::string &v ) { m_domain = v; return getDomain(); }
   bool setSyRequired( bool v ) { m_syrequired = v; return getSyRequired(); }
   int32_t setDefaultBearerCtlMode( int32_t v ) { m_default_bearer_ctl_mode = v; return getDefaultBearerCtlMode(); }
   bool setForceDefaultBearerCtlMode( bool v ) { m_force_default_bearer_ctl_mode = v; return getForceDefaultBearerCtlMode(); }

   Apn &addComputedRule( Rule *r ) { m_computed_rules.push_back( r ); return *this; }
private:
   std::string m_apn;
   std::string m_sy_subscription_id;
   bool m_fail_on_uninstallable_rule;
   bool m_auto_create_subscriber;
   std::string m_membership_value;
   std::string m_domain;
   RulesList m_computed_rules;
   bool m_syrequired;
   int32_t m_default_bearer_ctl_mode;
   bool m_force_default_bearer_ctl_mode;
};

class ApnMap : public std::map<std::string,Apn*>
{
public:
   ApnMap();
   ~ApnMap();

   bool getApn( const std::string &apn, Apn* &papn );
};

#endif // __APN_H
