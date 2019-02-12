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
