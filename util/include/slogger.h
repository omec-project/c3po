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

#ifndef __SLOGGER_H
#define __SLOGGER_H

//#define SPDLOG_LEVEL_NAMES { "trace", "debug", "info",  "warning", "error", "critical", "off" };
#define SPDLOG_LEVEL_NAMES { "trace", "debug", "info",  "startup", "warn", "error", "off" };

#define SPDLOG_ENABLE_SYSLOG
#include "spdlog/spdlog.h"

class LoggerException : public std::runtime_error
{
public:
   LoggerException(const char *m) : std::runtime_error(m) {}
   LoggerException(const std::string &m) : std::runtime_error(m) {}
};

class SLogger
{
public:
   SLogger( const char *category, std::vector<spdlog::sink_ptr> &sinks, const char *pattern, size_t queue_size );
   void trace( const char *format, ... );
   void trace( const std::string &format, ... );
   void debug( const char *format, ... );
   void debug( const std::string &format, ... );
   void info( const char *format, ... );
   void info( const std::string &format, ... );
   void startup( const char *format, ... );
   void startup( const std::string &format, ... );
   void warn( const char *format, ... );
   void warn( const std::string &format, ... );
   void error( const char *format, ... );
   void error( const std::string &format, ... );

   void flush() { m_log.flush(); }

   void set_level( spdlog::level::level_enum lvl );

   spdlog::level::level_enum get_level();

   const std::string & get_name();

private:
   SLogger();

   enum _LogType
   {
      _ltTrace,
      _ltDebug,
      _ltInfo,
      _ltStartup,
      _ltWarn,
      _ltError
   };

   void log( _LogType lt, const char *format, va_list &args );

   spdlog::async_logger m_log;
};


#endif // #define __SLOGGER_H
