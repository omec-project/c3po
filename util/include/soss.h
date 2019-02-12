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

#ifndef SOSS_H_
#define SOSS_H_

#include <iostream>
#include <pistache/endpoint.h>
#include <pistache/router.h>

#include <memory>

#include "slogger.h"
#include "stime.h"
#include "sstats.h"

#define RAPIDJSON_NAMESPACE fdrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"


class OssOptionReader{

public:

   OssOptionReader(const std::string& file) :
   m_file(file) {

   }

   bool readOssOptions(std::string& res){

      FILE* fp = fopen(m_file.c_str(), "r");

      if(fp == NULL){
         fclose(fp);
         return false;
      }

      char readBuffer[1024];
      RAPIDJSON_NAMESPACE::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
      RAPIDJSON_NAMESPACE::Document doc;
      doc.ParseStream(is);
      fclose(fp);

      if(!doc.IsObject()) {
         return false;
      }

      fdrapidjson::StringBuffer strbuf;
      fdrapidjson::Writer<fdrapidjson::StringBuffer> writer(strbuf);
      doc.Accept(writer);
      res =  strbuf.GetString();
      return true;
   }

private:
   std::string m_file;
};


template <typename T>
class OssRestHandler {
public:
   OssRestHandler(SStats* stats, SLogger* auditlogger, T* logger_manager, const std::string &ossoptionfile) :
   m_stats(stats),
   m_auditlogger(auditlogger),
   m_logger_manager(logger_manager),
   m_ossoption_reader(ossoptionfile)
   {
   }
   void getLoggers(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
      logAuditLog(request);
      response.send(Pistache::Http::Code::Ok, m_logger_manager->serialize());
   }
   void updateLogger(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
      logAuditLog(request);
      RAPIDJSON_NAMESPACE::Document doc;
      doc.Parse(request.body().c_str());
      if(doc.HasParseError()) {
         response.send(Pistache::Http::Code::Bad_Request, "{\"result\": \"ERROR\"}");
         return;
      }
      if(!doc.HasMember("name") || !doc["name"].IsString()){
         response.send(Pistache::Http::Code::Bad_Request, "{\"result\": \"ERROR\"}");
         return;
      }
      if(!doc.HasMember("level") || !doc["level"].IsInt()){
         response.send(Pistache::Http::Code::Bad_Request, "{\"result\": \"ERROR\"}");
         return;
      }

      std::string loggerName = doc["name"].GetString();
      int level = doc["level"].GetInt();
      if(m_logger_manager->updateLogger(loggerName, level)){
         response.send(Pistache::Http::Code::Ok, "{\"result\": \"OK\"}");
      }
      else{
         response.send(Pistache::Http::Code::Bad_Request, "{\"result\": \"ERROR\"}");
      }
   }
   void getStatFrequency(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
      logAuditLog(request);
      std::string res = "{\"statfreq\": " + std::to_string(m_stats->getInterval()) + "}";
      response.send(Pistache::Http::Code::Ok, res);
   }
   void getStatLive(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
      logAuditLog(request);
      std::shared_ptr<std::string> stats = m_stats->getLive();

      if(!stats->empty()){
         response.send(Pistache::Http::Code::Ok, *stats);
      }
      else{
         response.send(Pistache::Http::Code::Internal_Server_Error, "");
      }
   }
   void updateStatFrequency(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
      logAuditLog(request);
      RAPIDJSON_NAMESPACE::Document doc;
      doc.Parse(request.body().c_str());
      if(doc.HasParseError()) {
         response.send(Pistache::Http::Code::Bad_Request, "{\"result\": \"ERROR\"}");
         return;
      }
      if(!doc.HasMember("statfreq") || !doc["statfreq"].IsUint64()){
         response.send(Pistache::Http::Code::Bad_Request, "{\"result\": \"ERROR\"}");
         return;
      }
      unsigned long statfreq = doc["statfreq"].GetUint64();
      m_stats->updateInterval(statfreq);
      response.send(Pistache::Http::Code::Ok, "{\"result\": \"OK\"}");
   }
   void getOssOptions(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
      std::string res;

      if(m_ossoption_reader.readOssOptions(res)){
         response.send(Pistache::Http::Code::Ok, res);
      }
      else{
         response.send(Pistache::Http::Code::Bad_Request, "{\"result\": \"ERROR\"}");
      }
   }


private:
   void logAuditLog(const Pistache::Http::Request& request) {
      STime time_now = STime::Now();
      std::string now_str;
      time_now.Format(now_str, "%Y-%m-%d %H:%M:%S.%0", false);
      std::stringstream ss;
      ss << now_str <<  "," << "administrator" << "," << request.method() << "," <<  request.resource()  << "," << request.body();
      m_auditlogger->info(ss.str().c_str());
   }
private:
    SStats* m_stats;
    SLogger* m_auditlogger;
    T* m_logger_manager;
    OssOptionReader m_ossoption_reader;
};

template <typename T>
class OssEndpoint {

public:
   OssEndpoint(Pistache::Address addr, SStats* stats, SLogger* auditlogger, T* loggermanager, const std::string &ossoptionfile) :
   m_httpendpoint(std::make_shared<Pistache::Http::Endpoint>(addr)),
   m_handler(stats, auditlogger, loggermanager, ossoptionfile) {

   }
   void init(size_t thr = 1) {
      auto opts = Pistache::Http::Endpoint::options()
          .threads(thr)
          .flags(Pistache::Tcp::Options::ReuseAddr);
      m_httpendpoint->init(opts);
      setupRoutes();
   }
   void start() {
      m_httpendpoint->setHandler(m_router.handler());
      m_httpendpoint->serveThreaded();
   }
   void shutdown() {
      m_httpendpoint->shutdown();
   }

private:
   void setupRoutes() {
      Pistache::Rest::Routes::Get(m_router, "/logger", Pistache::Rest::Routes::bind(&OssRestHandler<T>::getLoggers, &m_handler));
      Pistache::Rest::Routes::Post(m_router, "/logger", Pistache::Rest::Routes::bind(&OssRestHandler<T>::updateLogger, &m_handler));
      Pistache::Rest::Routes::Get(m_router, "/statfreq", Pistache::Rest::Routes::bind(&OssRestHandler<T>::getStatFrequency, &m_handler));
      Pistache::Rest::Routes::Post(m_router, "/statfreq", Pistache::Rest::Routes::bind(&OssRestHandler<T>::updateStatFrequency, &m_handler));
      Pistache::Rest::Routes::Get(m_router, "/statlive", Pistache::Rest::Routes::bind(&OssRestHandler<T>::getStatLive, &m_handler));
      Pistache::Rest::Routes::Get(m_router, "/ossoptions", Pistache::Rest::Routes::bind(&OssRestHandler<T>::getOssOptions, &m_handler));
   }
   std::shared_ptr<Pistache::Http::Endpoint> m_httpendpoint;
   Pistache::Rest::Router m_router;
   OssRestHandler<T> m_handler;
};


#endif /* SOSS_H_ */
