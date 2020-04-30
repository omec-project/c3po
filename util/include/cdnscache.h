/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __CDNSCACHE_H
#define __CDNSCACHE_H

#include "cdnsquery.h"
#include "ssync.h"

namespace CachedDNS
{
   extern "C" typedef void(*CachedDNSQueryCallback)(Query *q, bool cacheHit, void *data);

   class QueryProcessor;
   
   class Cache
   {
      friend QueryProcessor;

   public:
      static Cache& getInstance();

      Query* query( ns_type rtype, const std::string &domain, bool &cacheHit );
      void query( ns_type rtype, const std::string &domain, CachedDNSQueryCallback cb, void *data=NULL );

   protected:
      Query* processQuery( ns_type rtype, const std::string &domain );

   private:
      Cache();
      ~Cache();

      Query* lookupQuery( ns_type rtype, const std::string &domain );
      bool processQuery( ns_type rtype, const std::string &domain, CachedDNSQueryCallback cb);

      static void ares_callback(void *arg, int status, int timeouts, unsigned char *abuf, int alen);

      QueryCache m_cache;
      SMutex m_cachemutex;
   };
}

#endif // #ifndef __CDNSCACHE_H
