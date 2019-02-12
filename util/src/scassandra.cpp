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

#include "scassandra.h"

SCassValue::SCassValue()
   : m_value( NULL )
{
}

SCassValue::SCassValue( const CassValue *value )
   : m_value( value )
{
}

SCassValue::~SCassValue()
{
}

SCassValue &SCassValue::operator=( const CassValue *value )
{
   m_value = value;
   return *this;
}

SCassValue &SCassValue::operator=( SCassValue &rval )
{
   m_value = rval.m_value;
   return *this;
}

bool SCassValue::get( int32_t &v )
{
   m_error = cass_value_get_int32( m_value, &v );
   return m_error == CASS_OK;
}

bool SCassValue::get( int64_t &v )
{
   m_error = cass_value_get_int64( m_value, &v );
   return m_error == CASS_OK;
}

bool SCassValue::get( float &v )
{
   m_error = cass_value_get_float( m_value, &v );
   return m_error == CASS_OK;
}

bool SCassValue::get( double &v )
{
   m_error = cass_value_get_double( m_value, &v );
   return m_error == CASS_OK;
}

bool SCassValue::get( bool &v )
{
   cass_bool_t cb;
   m_error = cass_value_get_bool( m_value, &cb );
   v = cb;
   return m_error == CASS_OK;
}

bool SCassValue::get( std::string &v )
{
   const char *val;
   size_t len;

   v.clear();

   m_error = cass_value_get_string( m_value, &val, &len );

   if ( m_error == CASS_OK )
   {
      v.assign( val, len );
      return true;
   }

   return false;
}

bool SCassValue::get( const char* &v, size_t &len )
{
   m_error = cass_value_get_string( m_value, &v, &len );
   return m_error == CASS_OK;
}

bool SCassValue::get( const uint8_t* &v, size_t &len )
{
   m_error = cass_value_get_bytes( m_value, &v, &len );
   return m_error == CASS_OK;
}

bool SCassValue::get( STime &v )
{
   int64_t ms;
   if ( SCassValue::get( ms ) )
   {
      v = ms;
      return true;
   }

   return false;
}


CassValueType SCassValue::type()
{
   return cass_value_type( m_value );
}

bool SCassValue::isNull()
{
   return cass_value_is_null( m_value );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SCassRow::SCassRow()
   : m_row( NULL )
{
}

SCassRow::SCassRow( const CassRow *row )
   : m_row( row )
{
}

SCassRow::~SCassRow()
{
}

SCassRow &SCassRow::operator=( const CassRow *row )
{
   m_row = row;
   return *this;
}

SCassRow &SCassRow::operator=( SCassRow &rval )
{
   m_row = rval.m_row;
   return *this;
}

SCassValue SCassRow::getColumn( size_t index )
{
   return SCassValue( cass_row_get_column( m_row, index ) );
}

SCassValue SCassRow::getColumn( const char *name )
{
   return SCassValue( cass_row_get_column_by_name( m_row, name ) );
}

SCassValue SCassRow::getColumn( std::string &name )
{
   return getColumn( name.c_str() );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SCassIterator::SCassIterator()
   : m_iterator( NULL )
{
}

SCassIterator::SCassIterator( CassIterator *it )
   : m_iterator( it )
{
}

SCassIterator::~SCassIterator()
{
   release();
}

SCassIterator &SCassIterator::operator=( CassIterator *it )
{
   release();
   m_iterator = it;
   return *this;
}

SCassIterator &SCassIterator::operator=( SCassIterator &rval )
{
   release();
   m_iterator = rval.m_iterator;
   rval.m_iterator = NULL;
   return *this;
}

CassIteratorType SCassIterator::type()
{
   return cass_iterator_type( m_iterator );
}

bool SCassIterator::nextRow()
{
   return cass_iterator_next( m_iterator );
}

SCassRow SCassIterator::row()
{
   return SCassRow( cass_iterator_get_row( m_iterator ) );
}

SCassValue SCassIterator::column()
{
   return SCassValue( cass_iterator_get_column( m_iterator ) );
}

SCassValue SCassIterator::value()
{
   return SCassValue( cass_iterator_get_value( m_iterator ) );
}

SCassValue SCassIterator::mapKey()
{
   return SCassValue( cass_iterator_get_map_key( m_iterator ) );
}

SCassValue SCassIterator::mapValue()
{
   return SCassValue( cass_iterator_get_map_value( m_iterator ) );
}

void SCassIterator::release()
{
   if ( m_iterator )
   {
      cass_iterator_free( m_iterator );
      m_iterator = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SCassResult::SCassResult()
   : m_result( NULL )
{
}

SCassResult::SCassResult( const CassResult *result )
   : m_result( result )
{
}

SCassResult::~SCassResult()
{
   release();
}

SCassResult &SCassResult::operator=( const CassResult *result )
{
   release();
   m_result = result;
   return *this;
}

SCassResult &SCassResult::operator=( SCassResult &result )
{
   release();
   m_result = result.m_result;
   result.m_result = NULL;
   return *this;
}

bool SCassResult::columnName( size_t index, std::string &cn )
{
   const char *name;
   size_t name_length;

   cn.clear();

   if ( cass_result_column_name( m_result, index, &name, &name_length ) == CASS_OK )
      cn.assign( name, name_length );

   return cn.empty() ? false : true;
}

CassValueType SCassResult::columnType( size_t index )
{
   return cass_result_column_type( m_result, index );
}

SCassRow SCassResult::firstRow()
{
   return SCassRow( cass_result_first_row( m_result ) );
}

SCassIterator SCassResult::rows()
{
   return SCassIterator( cass_iterator_from_result( m_result ) );
}

bool SCassResult::morePages()
{
   return cass_result_has_more_pages( m_result );
}

void SCassResult::release()
{
   if ( m_result )
   {
      cass_result_free( m_result );
      m_result = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SCassFuture::SCassFuture( CassFuture *future, bool incb )
   : m_error( (CassError)-1 ),
     m_future( future ),
     m_incb( incb )
{
}

SCassFuture::~SCassFuture()
{
   release();
}

SCassFuture& SCassFuture::operator=( SCassFuture &rval )
{
   release();

   m_future = rval.m_future;
   rval.m_future = NULL;

   return *this;
}

CassError SCassFuture::errorCode()
{
   if ( m_error == (CassError)-1 )
      m_error = cass_future_error_code( m_future );
   return m_error;
}

void SCassFuture::release()
{
   if ( m_future )
   {
      if ( !m_incb )
         cass_future_free( m_future );
      m_future = NULL;
   }

   if ( m_error != (CassError)-1 )
      m_error = (CassError)-1;
}

SCassResult SCassFuture::result()
{
   return SCassResult( cass_future_get_result( m_future ) );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SCassStatement::SCassStatement()
   : m_statement( NULL )
{
}

SCassStatement::SCassStatement( const char *qry )
   : m_statement( NULL )
{
   query( qry );
}

SCassStatement::SCassStatement( const std::string &qry )
   : m_statement( NULL )
{
   query( qry );
}

SCassStatement::~SCassStatement()
{
   release();
}

SCassStatement &SCassStatement::query( const char *qry )
{
   release();
   m_query = qry;
   m_statement = cass_statement_new( m_query.c_str(), 0 );
   return *this;
}

SCassStatement &SCassStatement::query( const std::string &qry )
{
   return query( qry.c_str() );
}

void SCassStatement::release()
{
   if ( m_statement )
   {
      cass_statement_free( m_statement );
      m_statement = NULL;
   }
}

SCassFuture SCassStatement::execute( CassSession *session )
{
   return SCassFuture( cass_session_execute( session, m_statement ) );
}

CassError SCassStatement::setPagingSize(int page_size)
{
   return cass_statement_set_paging_size( m_statement, page_size );
}

CassError SCassStatement::setPagingState(SCassResult &result)
{
   return cass_statement_set_paging_state( m_statement, result.getResult() );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SCassandra::SCassandra()
   : m_cluster( NULL ),
     m_session( NULL ),
     m_protver( 3 )
{
}

SCassandra::~SCassandra()
{
   release();
}

void SCassandra::release()
{
   if ( m_cluster )
   {
      cass_cluster_free( m_cluster );
      m_cluster = NULL;
   }

   if ( m_session )
   {
      cass_session_free( m_session );
      m_session = NULL;
   }
}

SCassFuture SCassandra::connect()
{
   release();

   // create the cluster object
   m_cluster = cass_cluster_new();

   // set the protocol version
   cass_cluster_set_protocol_version( m_cluster, m_protver );

   // create the session object
   m_session = cass_session_new();

   // add contact points
   cass_cluster_set_contact_points( m_cluster, m_host.c_str() );

   // open without keyspace if keyspace is not defined
   if ( m_keyspace.empty() )
      return SCassFuture( cass_session_connect( m_session, m_cluster ) );

   return SCassFuture( cass_session_connect_keyspace( m_session, m_cluster, m_keyspace.c_str() ) );
}

void SCassandra::disconnect()
{
   release();
}

bool SCassandra::setCoreConnectionsPerHost(uint32_t num)
{
   return cass_cluster_set_core_connections_per_host(m_cluster, num) == CASS_OK;;
}

bool SCassandra::setMaxConnectionsPerHost(uint32_t num)
{
   return cass_cluster_set_max_connections_per_host(m_cluster, num) == CASS_OK;;
}

bool SCassandra::setIONumberThreads(uint32_t num)
{
   cass_cluster_set_queue_size_io(m_cluster, 32768);
   cass_cluster_set_core_connections_per_host(m_cluster, 2);
   cass_cluster_set_max_connections_per_host(m_cluster, 8);
   return cass_cluster_set_num_threads_io(m_cluster, num) == CASS_OK;
}

bool SCassandra::setIOQueueSize(uint32_t size)
{
   return cass_cluster_set_queue_size_io(m_cluster, size) == CASS_OK;;
}
