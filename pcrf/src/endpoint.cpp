/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "endpoint.h"

EndpointMap::EndpointMap()
{
}

EndpointMap::~EndpointMap()
{
   EndpointMap::iterator it;

   while ( (it = begin()) != end() )
   {
      delete it->second;
      erase( it );
   }
}

bool EndpointMap::getEndpoint( const std::string &endpoint, Endpoint* &pendpoint )
{
   SMutexLock l( m_mutex );

   auto it = find( endpoint );
   if ( it == end() )
      return false;

   pendpoint = it->second;
   return true;
}

bool EndpointMap::addEndpoint( Endpoint *pendpoint )
{
   auto it = insert( std::pair<std::string,Endpoint*>( pendpoint->getHost(), pendpoint ) );
   return it.second;
}
