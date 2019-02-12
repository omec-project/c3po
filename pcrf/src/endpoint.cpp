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
