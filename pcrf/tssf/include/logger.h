/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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

   static _Logger &system() { return *singleton().m_system; }
   static _Logger &gx() { return *singleton().m_gx; }
   static _Logger &rx() { return *singleton().m_rx; }
   static _Logger &sd() { return *singleton().m_sd; }
   static _Logger &st() { return *singleton().m_st; }

private:
   static Logger *m_singleton;
   static Logger &singleton() { if (!m_singleton) m_singleton = new Logger(); return *m_singleton; }

   Logger() {}
   ~Logger() {}

   void _init( const char *app );

   std::vector<spdlog::sink_ptr> m_sinks;

   std::string m_pattern;

   _Logger *m_system;
   _Logger *m_gx;
   _Logger *m_rx;
   _Logger *m_sd;
   _Logger *m_st;
};

#endif // __LOGGER_H
