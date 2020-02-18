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
   m_gx = new SLogger( "gx", m_sinks, ss.str().c_str() , Options::logQueueSize() );
   m_rx = new SLogger( "rx", m_sinks, ss.str().c_str() , Options::logQueueSize() );
   m_sd = new SLogger( "sd", m_sinks, ss.str().c_str() , Options::logQueueSize() );
   m_st = new SLogger( "st", m_sinks, ss.str().c_str() , Options::logQueueSize());
   m_chronos = new SLogger( "chronos", m_sinks, ss.str().c_str() , Options::logQueueSize() );
   m_stat = new SLogger( "stat", m_statsinks, "%v", Options::logQueueSize() );
   m_audit = new SLogger( "audit", m_auditsinks, "%v", Options::logQueueSize() );

   m_system->set_level( spdlog::level::info );
   m_gx->set_level( spdlog::level::warn );
   m_rx->set_level( spdlog::level::warn );
   m_sd->set_level( spdlog::level::warn );
   m_st->set_level( spdlog::level::warn );
   m_chronos->set_level( spdlog::level::warn );
   m_stat->set_level(spdlog::level::info);
   m_audit->set_level(spdlog::level::trace);
}

void Logger::_cleanup()
{
   if ( m_system )
      delete m_system;
   if ( m_gx )
      delete m_gx;
   if ( m_rx )
      delete m_rx;
   if ( m_sd )
      delete m_sd;
   if ( m_st )
      delete m_st;
   if ( m_chronos )
      delete m_chronos;
   if ( m_stat )
      delete m_stat;
   if ( m_audit )
      delete m_audit;
}

void Logger::_flush()
{
   if ( m_system )
      m_system->flush();
   if ( m_gx )
      m_gx->flush();
   if ( m_rx )
      m_rx->flush();
   if ( m_sd )
      m_sd->flush();
   if ( m_st )
      m_st->flush();
   if ( m_chronos )
      m_chronos->flush();
   if ( m_stat )
      m_stat->flush();
   if ( m_audit )
      m_audit->flush();
}

std::string Logger::_serialize()
{
   RAPIDJSON_NAMESPACE::Document document;
   document.SetObject();
   RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = document.GetAllocator();

   RAPIDJSON_NAMESPACE::Value systemlog(RAPIDJSON_NAMESPACE::kObjectType);
   systemlog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_system->get_name().c_str()), allocator);
   systemlog.AddMember("level", m_system->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value gxlog(RAPIDJSON_NAMESPACE::kObjectType);
   gxlog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_gx->get_name().c_str()), allocator);
   gxlog.AddMember("level", m_gx->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value rxlog(RAPIDJSON_NAMESPACE::kObjectType);
   rxlog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_rx->get_name().c_str()), allocator);
   rxlog.AddMember("level", m_rx->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value sdlog(RAPIDJSON_NAMESPACE::kObjectType);
   sdlog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_sd->get_name().c_str()), allocator);
   sdlog.AddMember("level", m_sd->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value stlog(RAPIDJSON_NAMESPACE::kObjectType);
   stlog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_st->get_name().c_str()), allocator);
   stlog.AddMember("level", m_st->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value chronoslog(RAPIDJSON_NAMESPACE::kObjectType);
   chronoslog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_chronos->get_name().c_str()), allocator);
   chronoslog.AddMember("level", m_chronos->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value array(RAPIDJSON_NAMESPACE::kArrayType);
   array.PushBack(systemlog, allocator).PushBack(gxlog, allocator).
         PushBack(rxlog, allocator).PushBack(sdlog, allocator).
         PushBack(stlog, allocator).PushBack(chronoslog, allocator);

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
   else if(loggerName == "gx"){
      m_gx->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else if(loggerName == "rx"){
      m_rx->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else if(loggerName == "sd"){
      m_sd->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else if(loggerName == "st"){
      m_st->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else if(loggerName == "chronos"){
      m_chronos->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else{
      return false;
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
