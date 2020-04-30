/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SUBSCRIBER_H
#define __SUBSCRIBER_H

#include <string>
#include <map>

#include "rule.h"
#include "apn.h"

class SubscriberApn
{
public:
   SubscriberApn() : m_syrequired( false ) {}
   ~SubscriberApn() {}

   SubscriberApn &operator=( const Apn &a );

   const std::string &getApn() const { return m_apn; }
   const std::string &getMembershipValue() const { return m_membership_value; }
   const std::string &getDomain() const { return m_domain; }
   const RulesList &getComputedRules() const { return m_computed_rules; }
   bool getSyRequired() const { return m_syrequired; }
   const std::string &getTransferPolicy() const { return m_transfer_policy; }

   const std::string &setApn( const char *v ) { m_apn = v; return getApn(); }
   const std::string &setApn( const std::string &v ) { m_apn = v; return getApn(); }
   const std::string &setMembershipValue( const char *v ) { m_membership_value = v; return getMembershipValue(); }
   const std::string &setMembershipValue( const std::string &v ) { m_membership_value = v; return getMembershipValue(); }
   const std::string &setDomain( const char *v ) { m_domain = v; return getDomain(); }
   const std::string &setDomain( const std::string &v ) { m_domain = v; return getDomain(); }
   bool setSyRequired( bool v ) { m_syrequired = v; return getSyRequired(); }
   const std::string &setTransferPolicy( const char *v ) { m_transfer_policy = v; return getTransferPolicy(); }
   const std::string &setTransferPolicy( const std::string &v ) { m_transfer_policy = v; return getTransferPolicy(); }

   SubscriberApn &addComputedRule( Rule *r ) { m_computed_rules.push_back( r ); return *this; }

private:
   std::string m_apn;
   std::string m_membership_value;
   std::string m_domain;
   RulesList m_computed_rules;
   bool m_syrequired;
   std::string m_transfer_policy;
};

class SubscriberApnMap : public std::map<std::string,SubscriberApn*>
{
public:
   SubscriberApnMap();
   ~SubscriberApnMap();

   bool getSubscriberApn( const std::string &apn, SubscriberApn* &sa ) const;
   bool getSubscriberApn( const char *apn, SubscriberApn* &sa ) const { std::string s( apn ); return getSubscriberApn( s, sa ); }
};

class Subscriber
{
public:
   Subscriber() {}
   ~Subscriber() {}

   const std::string &getImsi() const { return m_imsi; }
   const std::string &getSySubscriptionId() const { return m_sy_subscription_id; }
   bool getFailOnUninstallableRule() const { return m_fail_on_uninstallable_rule; }
   const SubscriberApnMap &getApnPolicies() const { return m_apn_policies; }

   const std::string &setImsi( const char *v ) { m_imsi = v; return getImsi(); }
   const std::string &setImsi( const std::string &v ) { m_imsi = v; return getImsi(); }
   const std::string &setSySubscriptionId( const char *v ) { m_sy_subscription_id = v; return getSySubscriptionId(); }
   const std::string &setSySubscriptionId( const std::string &v ) { m_sy_subscription_id = v; return getSySubscriptionId(); }
   bool setFailOnUninstallableRule( bool v ) { m_fail_on_uninstallable_rule = v; return getFailOnUninstallableRule(); }

   void addSubscriberApn( SubscriberApn *sa );

private:
   std::string m_imsi;
   std::string m_sy_subscription_id;
   bool m_fail_on_uninstallable_rule;
   SubscriberApnMap m_apn_policies;
};

class SubscriberMap : public std::map<std::string,Subscriber*>
{
public:
   SubscriberMap();
   ~SubscriberMap();

   bool getSubscriber( const std::string &imsi, Subscriber* &subscriber ) const;
   bool getSubscriber( const char *imsi, Subscriber* &subscriber ) const { std::string s( imsi ); return getSubscriber( s, subscriber ); }
};

#endif // __SUBSCRIBER_H
