/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <iostream>
#include <sstream>
#include <string>

#define RAPIDJSON_NAMESPACE hssrapidjson
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "logger.h"
#include "options.h"

Logger *Logger::m_singleton = NULL;

void Logger::_init( const char *app )
{
   m_sinks.push_back( std::make_shared<spdlog::sinks::syslog_sink>() );
   m_sinks.push_back( std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>() );
   m_sinks.push_back( std::make_shared<spdlog::sinks::rotating_file_sink_mt>( Options::logFilename(), Options::logMaxSize() * 1024 * 1024, Options::logNumberFiles() ) );

   m_sinks[0]->set_level( spdlog::level::warn );
   m_sinks[1]->set_level( spdlog::level::info );
   m_sinks[2]->set_level( spdlog::level::trace );

   m_statsinks.push_back( std::make_shared<spdlog::sinks::rotating_file_sink_mt>( Options::statlogFilename(), Options::statlogMaxSize() * 1024 * 1024, Options::statlogNumberFiles() ) );
   m_statsinks[0]->set_level( spdlog::level::info );

   m_auditsinks.push_back( std::make_shared<spdlog::sinks::rotating_file_sink_mt>( Options::auditlogFilename(), Options::auditlogMaxSize() * 1024 * 1024, Options::statlogNumberFiles() ) );
   m_statsinks[0]->set_level( spdlog::level::trace );

   std::stringstream ss;
   ss << "[%Y-%m-%dT%H:%M:%S.%e] [" << app << "] [%n] [%l] %v";

   m_system = new SLogger( "system", m_sinks, ss.str().c_str(), Options::logQueueSize() );
   m_rf = new SLogger( "rf", m_sinks, ss.str().c_str(), Options::logQueueSize() );
   m_stat = new SLogger( "stat", m_statsinks, "%v", Options::logQueueSize() );
   m_audit = new SLogger( "audit", m_auditsinks, "%v", Options::logQueueSize() );

   m_system->set_level( spdlog::level::info );
   m_rf->set_level( spdlog::level::warn );
   m_stat->set_level(spdlog::level::info);
   m_audit->set_level(spdlog::level::trace);
}

void Logger::_cleanup()
{
   if ( m_system )
      delete m_system;
   if ( m_rf )
      delete m_rf;
}

void Logger::_flush()
{
   if ( m_system )
      m_system->flush();
   if ( m_rf )
      m_rf->flush();
   if ( m_stat )
      m_stat->flush();
}

std::string Logger::_serialize()
{
   RAPIDJSON_NAMESPACE::Document document;
   document.SetObject();
   RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = document.GetAllocator();

   RAPIDJSON_NAMESPACE::Value systemlog(RAPIDJSON_NAMESPACE::kObjectType);
   systemlog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_system->get_name().c_str()), allocator);
   systemlog.AddMember("level", m_system->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value rflog(RAPIDJSON_NAMESPACE::kObjectType);
   rflog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_rf->get_name().c_str()), allocator);
   rflog.AddMember("level", m_rf->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value array(RAPIDJSON_NAMESPACE::kArrayType);
   array.PushBack(systemlog, allocator).PushBack(rflog, allocator);

   document.AddMember("loggers", array, allocator);

   RAPIDJSON_NAMESPACE::StringBuffer strbuf;
   RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(strbuf);
   document.Accept(writer);
   return strbuf.GetString();
}

bool Logger::_updateLogger(const std::string &loggerName, int value){

   if(loggerName == "system"){
      m_system->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else if(loggerName == "rf"){
      m_rf->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else{
      return false;
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
