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

#include "slogger.h"

SLogger::SLogger( const char *category, std::vector<spdlog::sink_ptr> &sinks, const char *pattern, size_t queue_size )
   : m_log( category, sinks.begin(), sinks.end(), queue_size, spdlog::async_overflow_policy::discard_log_msg, nullptr,
            std::chrono::seconds(2), nullptr)
{
   m_log.set_pattern( pattern );
   m_log.flush_on( spdlog::level::err );
}

void SLogger::trace( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltTrace, format, args );
   va_end( args );
}

void SLogger::trace( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltTrace, format.c_str(), args );
   va_end( args );
}

void SLogger::debug( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltDebug, format, args );
   va_end( args );
}

void SLogger::debug( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltDebug, format.c_str(), args );
   va_end( args );
}

void SLogger::info( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltInfo, format, args );
   va_end( args );
}

void SLogger::info( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltInfo, format.c_str(), args );
   va_end( args );
}

void SLogger::startup( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltStartup, format, args );
   va_end( args );
}

void SLogger::startup( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltStartup, format.c_str(), args );
   va_end( args );
}

void SLogger::warn( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltWarn, format, args );
   va_end( args );
}

void SLogger::warn( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltWarn, format.c_str(), args );
   va_end( args );
}

void SLogger::error( const char *format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltError, format, args );
   va_end( args );
}

void SLogger::error( const std::string &format, ... )
{
   va_list args;
   va_start( args, format );
   log( _ltError, format.c_str(), args );
   va_end( args );
}

void SLogger::set_level( spdlog::level::level_enum lvl )
{
   m_log.set_level( lvl );
}

spdlog::level::level_enum SLogger::get_level(){
   return m_log.level();
}

const std::string & SLogger::get_name(){
   return m_log.name();
}

void SLogger::log( _LogType lt, const char *format, va_list &args )
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
