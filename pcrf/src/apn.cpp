/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "apn.h"

ApnMap::ApnMap()
{
}

ApnMap::~ApnMap()
{
   ApnMap::iterator it;

   while ( (it = begin()) != end() )
   {
      delete it->second;
      erase( it );
   }
}

bool ApnMap::getApn( const std::string &apn, Apn* &papn )
{
   auto it = find( apn );
   if ( it == end() )
      return false;

   papn = it->second;
   return true;
}
