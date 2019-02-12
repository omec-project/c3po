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
   m_s6as6d = new SLogger( "s6as6d", m_sinks, ss.str().c_str(), Options::logQueueSize() );
   m_s6c = new SLogger( "rx", m_sinks, ss.str().c_str(), Options::logQueueSize() );
   m_s6t = new SLogger( "sd", m_sinks, ss.str().c_str(), Options::logQueueSize() );
   m_stat = new SLogger( "stat", m_statsinks, "%v", Options::logQueueSize() );
   m_audit = new SLogger( "audit", m_auditsinks, "%v", Options::logQueueSize() );

   m_system->set_level( spdlog::level::info );
   m_s6as6d->set_level( spdlog::level::warn );
   m_s6c->set_level( spdlog::level::warn );
   m_s6t->set_level( spdlog::level::warn );
   m_stat->set_level(spdlog::level::info);
   m_audit->set_level(spdlog::level::trace);
}

void Logger::_cleanup()
{
   if ( m_system )
      delete m_system;
   if ( m_s6as6d )
      delete m_s6as6d;
   if ( m_s6c )
      delete m_s6c;
   if ( m_s6t )
      delete m_s6t;
}

void Logger::_flush()
{
   if ( m_system )
      m_system->flush();
   if ( m_s6as6d )
      m_s6as6d->flush();
   if ( m_s6c )
      m_s6c->flush();
   if ( m_s6t )
      m_s6t->flush();
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

   RAPIDJSON_NAMESPACE::Value s6as6dlog(RAPIDJSON_NAMESPACE::kObjectType);
   s6as6dlog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_s6as6d->get_name().c_str()), allocator);
   s6as6dlog.AddMember("level", m_s6as6d->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value s6clog(RAPIDJSON_NAMESPACE::kObjectType);
   s6clog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_s6c->get_name().c_str()), allocator);
   s6clog.AddMember("level", m_s6c->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value s6tlog(RAPIDJSON_NAMESPACE::kObjectType);
   s6tlog.AddMember("name", RAPIDJSON_NAMESPACE::StringRef(m_s6t->get_name().c_str()), allocator);
   s6tlog.AddMember("level", m_s6t->get_level(), allocator);

   RAPIDJSON_NAMESPACE::Value array(RAPIDJSON_NAMESPACE::kArrayType);
   array.PushBack(systemlog, allocator).PushBack(s6as6dlog, allocator).PushBack(s6clog, allocator).PushBack(s6tlog, allocator);

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
   else if(loggerName == "s6as6d"){
      m_s6as6d->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else if(loggerName == "rx"){
      m_s6c->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else if(loggerName == "sd"){
      m_s6t->set_level(static_cast<spdlog::level::level_enum>(value));
      return true;
   }
   else{
      return false;
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
