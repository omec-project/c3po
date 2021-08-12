/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include "resthandler.h"
#include "rapidjson/error/en.h"
#include <pistache/endpoint.h>
#include "options.h"


#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


void RestHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response)
{
   static bool needConfig = true;

   if (request.resource() == "/v1/config-check") {
     if (needConfig == true) {
        response.send(Pistache::Http::Code::Not_Found);
        needConfig = false;
     } else {
        response.send(Pistache::Http::Code::Ok);
     }
   } else if (request.resource() == "/v1/config/policies") {
      std::cout<<"hitting config policy resource "<<request.body()<<std::endl;
      switch(request.method()) {
        case Pistache::Http::Method::Post: {
             std::cout<<"Post method callback on resource "<<request.resource().c_str()<<std::endl;
             auto content_type = request.headers().tryGet<Pistache::Http::Header::ContentType>();
             if (content_type != nullptr) {
                 RAPIDJSON_NAMESPACE::Document doc;
                 std::string body = request.body();
                 std::cout<<"Request body - "<<body.c_str()<<std::endl;
                 doc.Parse(body.c_str());
                 if(!doc.IsObject()) {
                     std::cout<<"Error parsing the json config file "<<body.c_str()<<std::endl;
                     response.send(Pistache::Http::Code::Bad_Request);
                     return;
                 }
                 bool result = Options::parseJsonDoc(doc);
                 if(result == false) {
                     response.send(Pistache::Http::Code::Bad_Request);
                 } else {
                     response.send(Pistache::Http::Code::Ok);
                 }
             } else {
                 response.send(Pistache::Http::Code::Bad_Request);
             }
             break;
        }
        default: {
            response.send(Pistache::Http::Code::Bad_Request);
            std::cout<<"Unhandled method "<<int(request.method())<<std::endl;
            break;
        }
      }
   } else {
      std::cout << request.resource() << std::endl;
      std::cout << request.method() << std::endl;
      std::cout << request.body() << std::endl;
      std::stringstream ss;
      ss << "Unrecognized resource [" << request.resource() << "]";
      response.send(Pistache::Http::Code::Bad_Request, ss.str() );
   }
}

