/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <iostream>
#include <sdn.h>
#include <sutility.h>
#include <libmemcached/memcached.h>
#include "options.h"
#include "sutility.h"

SDN::SDN()
{
   m_mc = memcached_create( NULL );
   if ( !m_mc )
      throw SDNException(
         SUtility::string_format( "%s:%d - error creating memcached handle",
         __FILE__, __LINE__ )
      );

   std::cout << "memcached server: " << Options::memcachedServer() << " port: " << Options::memcachedPort() << std::endl;
   memcached_return rc;
   memcached_server_st *server = memcached_server_list_append( NULL, Options::memcachedServer().c_str(), Options::memcachedPort(), &rc );
   if ( !server || rc != MEMCACHED_SUCCESS )
      throw SDNException(
         SUtility::string_format( "%s:%d - error creating memcached server %d",
         __FILE__, __LINE__, rc )
      );

   rc = memcached_server_push( m_mc, server );
   if ( rc != MEMCACHED_SUCCESS )
      throw SDNException(
         SUtility::string_format( "%s:%d - error pushing memcached server %d",
         __FILE__, __LINE__, rc )
      );
}

SDN::~SDN()
{
   if ( m_mc )
      memcached_free( m_mc );
}

bool SDN::get( const char *ipaddr )
{
   memcached_return rc;
   char *v;
   size_t vlen;
   uint32_t flags;
   std::string val;

   m_ipaddress.clear();
   m_imsi.clear();
   m_mdn.clear();
   m_context.clear();
   m_start = 0;
   m_end = 0;

   v = memcached_get( m_mc, ipaddr, strlen( ipaddr ), &vlen, &flags, &rc );
   if ( rc != MEMCACHED_SUCCESS )
      return false;

   val = v;
   std::vector<std::string> vals = SUtility::split( val, ',' );

   if ( vals.size() < 6 )
      return false;

   m_ipaddress = vals[0];
   m_imsi = vals[1];
   m_mdn = vals[2];
   m_context = vals[3];

   m_start = vals[4].empty() ? 0 : strtol( vals[4].c_str(), NULL, 10 ) / 1000;
   m_end = vals[5].empty() ? 0 : strtol( vals[5].c_str(), NULL, 10 ) / 1000;
   
   return true;
}
