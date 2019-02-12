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

#include <stdarg.h>
#include <stdio.h>
#include <memory.h>
#include <poll.h>
#include <ares.h>

#include <iostream>
#include <list>

#include "serror.h"
#include "sthread.h"
#include "ssync.h"
#include "cdnscache.h"
#include "cdnsparser.h"

using namespace CachedDNS;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<class T>
class Buffer
{
public:
   Buffer(size_t size) { msize = size; mbuf = new T[msize]; }
   ~Buffer() { if (mbuf) delete [] mbuf; }
   T *get() { return mbuf; }
private:
   Buffer();
   size_t msize;
   T *mbuf;
};

static std::string string_format( const char *format, ... )
{
   va_list args;

   va_start( args, format );
   size_t size = vsnprintf( NULL, 0, format, args ) + 1; // Extra space for '\0'
   va_end( args );

   Buffer<char> buf( size );

   va_start( args, format );
   vsnprintf( buf.get(), size, format, args  );
   va_end( args );

   return std::string( buf.get(), size - 1 ); // We don't want the '\0' inside
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

namespace CachedDNS
{
   class QueryProcessor
   {
   public:
      QueryProcessor( Query *q )
         : m_query( q )
      {
         m_status = ARES_SUCCESS;
         m_channel = NULL;
         m_query = q;
      }
   
      ~QueryProcessor()
      {
         ares_destroy( m_channel );
      }

      int getStatus() { return m_status; }

      void execute()
      {
         if ( (m_status = ares_init(&m_channel)) == ARES_SUCCESS )
         {
            struct ares_options opt;
            opt.timeout = 1000;
            opt.ndots = 0;
            opt.flags = ARES_FLAG_EDNS;
            opt.ednspsz = 8192;
            ares_init_options( &m_channel, &opt, ARES_OPT_TIMEOUTMS | ARES_OPT_NDOTS | ARES_OPT_EDNSPSZ | ARES_OPT_FLAGS );
    
            ares_query( m_channel, m_query->getDomain().c_str(), ns_c_in, m_query->getType(), ares_callback, this );

            wait_for_completion();
         }

         m_event.set();
      }

   private:
      QueryProcessor() {}

      static void ares_callback( void *arg, int status, int timeouts, unsigned char *abuf, int alen )
      {
         QueryProcessor *qp = (QueryProcessor*)arg;
         qp->process( status, timeouts, abuf, alen );
      }

      void wait_for_completion()
      {
         struct timeval tv;
         int timeout;
         ares_socket_t sockets[ ARES_GETSOCK_MAXNUM ];
         struct pollfd fds[ ARES_GETSOCK_MAXNUM ];

         while( true )
         {
            int rwbits = ares_getsock( m_channel, sockets, ARES_GETSOCK_MAXNUM );
            int fdcnt = 0;

            memset( fds, 0, sizeof(fds) );

            for ( int i = 0; i < ARES_GETSOCK_MAXNUM; i++ )
            {
               fds[fdcnt].fd = sockets[i];
               fds[fdcnt].events |= ARES_GETSOCK_READABLE(rwbits,i) ? (POLLIN | POLLRDNORM) : 0;
               fds[fdcnt].events |= ARES_GETSOCK_WRITABLE(rwbits,i) ? (POLLOUT | POLLWRNORM) : 0;
               if ( fds[fdcnt].events != 0 )
                  fdcnt++;
            }

            if ( !fdcnt )
               break;

            memset( &tv, 0, sizeof(tv) );
            ares_timeout( m_channel, NULL, &tv );
            timeout = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

            if ( poll(fds,fdcnt,timeout) != 0 )
            {
               for ( int i = 0; i < fdcnt; i++ )
               {
                  if ( fds[i].revents != 0 )
                  {
                     ares_process_fd( m_channel,
                        fds[i].revents & (POLLIN | POLLRDNORM) ? fds[i].fd : ARES_SOCKET_BAD,
                        fds[i].revents & (POLLOUT | POLLWRNORM) ? fds[i].fd : ARES_SOCKET_BAD);
                  }
               }
            }
            else
            {
               // timeout
               ares_process_fd( m_channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD );
            }
         }
      }

      void process( int status, int timeouts, unsigned char *abuf, int alen )
      {
         try
         {
            Parser p( m_query, abuf, alen );
            p.parse();
         }
         catch (std::exception &ex)
         {
            std::cout << "EXCEPTION - " << ex.what() << std::endl;
         }
      }

      int m_status;
      ares_channel m_channel;
      SEvent m_event;
      Query *m_query;
   };

   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////

   class QueryThread : public SThread
   {
   public:
      QueryThread( ns_type rtype, const std::string &domain, CachedDNSQueryCallback cb, void *data = NULL )
         : SThread( true ),
           m_type( rtype ),
           m_domain( domain ),
           m_cb( cb ),
           m_data( data )
      {
      }

      ~QueryThread()
      {
      }

      virtual unsigned long threadProc(void *arg)
      {
         bool cacheHit = false;
         Query *q = Cache::getInstance().query( m_type, m_domain, cacheHit );
         m_cb( q, cacheHit, m_data );
         return 0;
      }

   private:
      ns_type m_type;
      const std::string m_domain;
      CachedDNSQueryCallback m_cb;
      void *m_data;
   };
} // namespace CachedDNS

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Cache::Cache()
{
   int status = ares_library_init( ARES_LIB_INIT_ALL );
   if ( status != ARES_SUCCESS )
   {
      std::string msg( string_format("Cache::Cache() - ares_library_init() failed status = %d", status) );
      SError::throwRuntimeException( msg );
   }
}

Cache::~Cache()
{
   QueryCache::iterator it;

   while ((it = m_cache.begin()) != m_cache.end())
   {
      Query *q = it->second;
      m_cache.erase( it );
      delete q;
   }

   ares_library_cleanup();
}

Cache& Cache::getInstance()
{
   static Cache instance;
   return instance;
}

Query* Cache::query( ns_type rtype, const std::string & domain, bool &cacheHit )
{
   Query *q = lookupQuery( rtype, domain );

   cacheHit = !( !q || q->isExpired() );

   if ( !cacheHit ) // query not found or expired
      q = processQuery( rtype, domain );

   return q;
}

void Cache::query( ns_type rtype, const std::string &domain, CachedDNSQueryCallback cb, void *data )
{
   Query *q = lookupQuery( rtype, domain );

   bool cacheHit = !( !q || q ->isExpired() );

   if ( cacheHit )
   {
      cb( q, cacheHit, data );
   }
   else
   {
      QueryThread *qt = new QueryThread( rtype, domain, cb, data );
      qt->init( NULL );
      // do not delete qt since the thread is set to selfDestruct
   }
}

Query* Cache::lookupQuery( ns_type rtype, const std::string &domain )
{
   SMutexLock l( m_cachemutex );
   QueryCacheKey qck( rtype, domain );
   QueryCache::const_iterator it = m_cache.find( qck );
   return it != m_cache.end() ? it->second : NULL;
}

Query* Cache::processQuery( ns_type rtype, const std::string &domain )
{
   Query* q = new Query( rtype, domain );
   QueryProcessor qp( q );
   qp.execute();

   if ( qp.getStatus() == ARES_SUCCESS )
   {
      Query *oldQuery = NULL;

      {
         SMutexLock l( m_cachemutex );
         QueryCacheKey qck( rtype, domain );

         // lookup the query in the cache
         QueryCache::iterator it = m_cache.find( qck );
         if ( it != m_cache.end() ) // found the entry
         {
            // save the query* to be deleted
            oldQuery = it->second;
            // update the cache with the new query results
            it->second = q;
         }
         else
         {
            // add the entry since it was not found
            m_cache[qck] = q;
         }
      }

      if ( oldQuery )
         delete oldQuery;
   }
   else
   {
      // delete the query object since it failed
      delete q;
      q = NULL;
   }

   return q;
}

void Cache::ares_callback( void *arg, int status, int timeouts, unsigned char *abuf, int alen )
{
}

