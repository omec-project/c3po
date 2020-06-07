/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "bearer.h"

BearerMap::BearerMap()
{
}

BearerMap::~BearerMap()
{
   BearerMap::iterator it;

   while ( (it = begin()) != end() )
   {
      delete it->second;
      erase( it );
   }
}
