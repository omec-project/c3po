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

#ifndef __SCASSANDRA_H
#define __SCASSANDRA_H

#include <stdint.h>
#include <string>

#include <cassandra.h>

#include "stime.h"

class SCassValue
{
public:
   SCassValue();
   SCassValue( const CassValue *value );
   ~SCassValue();

   SCassValue &operator=( const CassValue *value );
   SCassValue &operator=( SCassValue &rval );

   bool get( int32_t &v );
   bool get( int64_t &v );
   bool get( float &v );
   bool get( double &v );
   bool get( bool &v );
   bool get( std::string &v );
   bool get( const char* &v, size_t &len );
   bool get( const uint8_t* &v, size_t &len );
   bool get( STime &v );

   bool get( uint32_t &v ) { int64_t v2; if ( !get( v2 ) ) return false; v = (uint32_t)v2; return true; }

   CassValueType type();
   bool isNull();

   CassError error() { return m_error; }

private:
   const CassValue *m_value;
   CassError m_error;
};

class SCassRow
{
public:
   SCassRow();
   SCassRow( const CassRow *row );
   ~SCassRow();

   SCassRow &operator=( const CassRow *row );
   SCassRow &operator=( SCassRow &rval );

   SCassValue getColumn( size_t index );
   SCassValue getColumn( const char *name );
   SCassValue getColumn( std::string &name );

   bool valid() { return m_row != NULL; }

private:
   const CassRow *m_row;
};

class SCassIterator
{
public:
   SCassIterator();
   SCassIterator( CassIterator *it );
   ~SCassIterator();

   SCassIterator &operator=( CassIterator *it );
   SCassIterator &operator=( SCassIterator &rval );

   CassIteratorType type();
   bool nextRow();
   SCassRow row();
   SCassValue column();
   SCassValue value();
   SCassValue mapKey();
   SCassValue mapValue();

private:
   void release();
   
   CassIterator *m_iterator;
};

class SCassStatement;

class SCassResult
{
   friend SCassStatement;

public:
   SCassResult();
   SCassResult( const CassResult *result );
   ~SCassResult();

   SCassResult &operator=( const CassResult *result );
   SCassResult &operator=( SCassResult &result );

   size_t rowCount() { return cass_result_row_count( m_result ); }
   size_t columnCount() { return cass_result_column_count( m_result ); }

   bool columnName( size_t index, std::string &cn );
   CassValueType columnType( size_t index );

   SCassRow firstRow();
   SCassIterator rows();

   bool morePages();

protected:
   const CassResult *getResult() { return m_result; }

private:
   void release();

   const CassResult *m_result;
};

class SCassFuture
{
public:
   SCassFuture( CassFuture *future, bool incb=false );
   ~SCassFuture();

   SCassFuture &operator=( SCassFuture &rval );

   bool ready() { return cass_future_ready( m_future ); }
   void wait() { cass_future_wait( m_future ); }
   bool wait( uint64_t us ) { return cass_future_wait_timed( m_future, us ); }
   bool setCallback(CassFutureCallback cb, void *data)
   {
      return (m_error = cass_future_set_callback(m_future, cb, data)) == CASS_OK;
   }

   CassError errorCode();
   SCassResult result();

private:
   SCassFuture();
   void release();

   CassError m_error;
   CassFuture *m_future;
   bool m_incb;
};

class SCassandra;

class SCassStatement
{
   friend SCassandra;
public:
   SCassStatement();
   SCassStatement( const char *qry );
   SCassStatement( const std::string &qry );
   ~SCassStatement();

   SCassStatement &query( const char *qry );
   SCassStatement &query( const std::string &qry );

   CassError setPagingSize(int page_size);
   CassError setPagingState( SCassResult &result );

protected:
   void release();
   SCassFuture execute( CassSession *session );

private:
   std::string m_query;
   CassStatement *m_statement;
};

class SCassandra
{
public:
   SCassandra();
   ~SCassandra();

   SCassFuture execute( SCassStatement &statement ) { return statement.execute( m_session ); }

   SCassFuture connect();
   void disconnect();

   std::string &host( const char *hst ) { return m_host = hst; }
   std::string &host( const std::string &hst ) { return host( hst.c_str() ); }
   std::string &host() { return m_host; }

   std::string &keyspace( const char *ks ) { return m_keyspace = ks; }
   std::string &keyspace( const std::string &ks ) { return keyspace( ks.c_str() ); }
   std::string &keyspace() { return m_keyspace; }

   int protocolVersion( int protver ) { return m_protver = protver; }
   int protocolVersion() { return m_protver; }

   bool setCoreConnectionsPerHost(uint32_t num);
   bool setMaxConnectionsPerHost(uint32_t num);
   bool setIONumberThreads(uint32_t num);
   bool setIOQueueSize(uint32_t size);

private:
   void release();

   CassCluster *m_cluster;
   CassSession *m_session;
   std::string m_host;
   std::string m_keyspace;
   int m_protver;
};

#endif // __SCASSANDRA_H
