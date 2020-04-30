/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
