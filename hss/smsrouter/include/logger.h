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

#ifndef __LOGGER_H
#define __LOGGER_H

#include <cstdarg>
#include <stdexcept>
#include <vector>

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

class _Logger
{
public:
   _Logger( const char *category, std::vector<spdlog::sink_ptr> &sinks, const char *pattern );

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
private:
   _Logger();

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

   spdlog::logger m_log;
};

class Logger
{
public:

   static void init( const char *app ) { singleton()._init( app ); }
   static void init( const std::string &app ) { init( app.c_str() ); }
   static void cleanup() { singleton()._cleanup(); }
   static void flush() { singleton()._flush(); }

   static _Logger &system() { return *singleton().m_system; }
   static _Logger &s6c() { return *singleton().m_s6c; }
   static _Logger &sgd() { return *singleton().m_sgd; }

private:
   static Logger *m_singleton;
   static Logger &singleton() { if (!m_singleton) m_singleton = new Logger(); return *m_singleton; }

   Logger() {}
   ~Logger() {}

   void _init( const char *app );
   void _cleanup();
   void _flush();

   std::vector<spdlog::sink_ptr> m_sinks;

   std::string m_pattern;

   _Logger *m_system;
   _Logger *m_s6c;
   _Logger *m_sgd;
};

#endif // __LOGGER_H
