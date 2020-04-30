/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "subscriber.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SubscriberApnMap::SubscriberApnMap()
{
}

SubscriberApnMap::~SubscriberApnMap()
{
   SubscriberApnMap::iterator it;

   while ( (it = begin()) != end() )
   {
      delete it->second;
      erase( it );
   }
}

bool SubscriberApnMap::getSubscriberApn( const std::string &apn, SubscriberApn* &sa ) const
{
   auto pair = find( apn );

   sa = pair == end() ? NULL : pair->second;

   return sa != NULL;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SubscriberApn &SubscriberApn::operator=( const Apn &a )
{
   setApn( a.getApn() );
   setMembershipValue( a.getMembershipValue() );
   setDomain( a.getDomain() );
   setSyRequired( a.getSyRequired() );

   for ( auto &r : a.getComputedRules() )
      addComputedRule( r );

   return *this;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SubscriberMap::SubscriberMap()
{
}

SubscriberMap::~SubscriberMap()
{
   SubscriberMap::iterator it;

   while ( (it = begin()) != end() )
   {
      delete it->second;
      erase( it );
   }
}

bool SubscriberMap::getSubscriber( const std::string &imsi, Subscriber* &subscriber ) const
{
   auto pair = find( imsi );

   subscriber = pair == end() ? NULL : pair->second;

   return subscriber != NULL;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Subscriber::addSubscriberApn( SubscriberApn *sa )
{
   m_apn_policies.insert( std::pair<std::string,SubscriberApn*>( sa->getApn(), sa ) );
}
