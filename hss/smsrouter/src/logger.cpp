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

#include <iostream>
#include <sstream>
#include <string>

#include "logger.h"
#include "options.h"

Logger *Logger::m_singleton = NULL;

void Logger::_init( const char *app )
{
   spdlog::set_async_mode( Options::logQueueSize() );

   m_sinks.push_back( std::make_shared<spdlog::sinks::syslog_sink>() );
   m_sinks.push_back( std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>() );
   m_sinks.push_back( std::make_shared<spdlog::sinks::rotating_file_sink_mt>( Options::logFilename(), Options::logMaxSize() * 1024 * 1024, Options::logNumberFiles() ) );

   m_sinks[0]->set_level( spdlog::level::info );
   m_sinks[1]->set_level( spdlog::level::trace );
   m_sinks[2]->set_level( spdlog::level::trace );

   std::stringstream ss;
   ss << "[%Y-%m-%dT%H:%M:%S.%e] [" << app << "] [%n] [%l] %v";

   m_system = new _Logger( "system", m_sinks, ss.str().c_str() );
   m_s6c = new _Logger( "m_s6c", m_sinks, ss.str().c_str() );
   m_sgd = new _Logger( "m_sgd", m_sinks, ss.str().c_str() );

   m_system->set_level( spdlog::level::trace );
   m_s6c->set_level( spdlog::level::trace );
   m_sgd->set_level( spdlog::level::trace );
}

void Logger::_cleanup()
{
   if ( m_system )
      delete m_system;
   if ( m_s6c )
      delete m_s6c;
   if ( m_sgd )
      delete m_sgd;
}

void Logger::_flush()
{
   if ( m_system )
      m_system->flush();
   if ( m_s6c )
      m_s6c->flush();
   if ( m_sgd )
      m_sgd->flush();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

_Logger::_Logger( const char *category, std::vector<spdlog::sink_ptr> &sinks, const char *pattern )
   : m_log( category, sinks.begin(), sinks.end() )
{
   m_log.set_pattern( pattern );
}

void _Logger::trace( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltTrace, format, args );
   va_end( args );
}

void _Logger::trace( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltTrace, format.c_str(), args );
   va_end( args );
}

void _Logger::debug( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltDebug, format, args );
   va_end( args );
}

void _Logger::debug( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltDebug, format.c_str(), args );
   va_end( args );
}

void _Logger::info( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltInfo, format, args );
   va_end( args );
}

void _Logger::info( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltInfo, format.c_str(), args );
   va_end( args );
}

void _Logger::startup( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltStartup, format, args );
   va_end( args );
}

void _Logger::startup( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltStartup, format.c_str(), args );
   va_end( args );
}

void _Logger::warn( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltWarn, format, args );
   va_end( args );
}

void _Logger::warn( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltWarn, format.c_str(), args );
   va_end( args );
}

void _Logger::error( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltError, format, args );
   va_end( args );
}

void _Logger::error( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltError, format.c_str(), args );
   va_end( args );
}

void _Logger::set_level( spdlog::level::level_enum lvl )
{
   m_log.set_level( lvl );
}

void _Logger::log( _LogType lt, const char *format, va_list &args )
{
   char buffer[ 2048 ];

   vsnprintf( buffer, sizeof(buffer), format, args );

   switch ( lt )
   {
      case _ltTrace: m_log.trace( buffer ); break;
      case _ltDebug: m_log.debug( buffer ); break;
      case _ltInfo: m_log.info( buffer ); break;
      case _ltStartup: m_log.warn( buffer ); break;
      case _ltWarn: m_log.error( buffer ); break;
      case _ltError: m_log.critical( buffer ); break;
   }
}
