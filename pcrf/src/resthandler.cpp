/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include "resthandler.h"
#include "rapidjson/error/en.h"
#include <pistache/endpoint.h>

void RestHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response)
{
   std::cout << request.resource() << std::endl;
   std::cout << request.method() << std::endl;
   std::cout << request.body() << std::endl;

   if (request.resource() == "/v1/config/policies")
   {
      std::cout<<"hitting config policy resource "<<request.body()<<std::endl;
      RAPIDJSON_NAMESPACE::Document doc;
      doc.Parse(request.body().c_str());
      if(doc.HasParseError())
      {
              std::stringstream ss;
              ss << "Body parsing error offset="
                      << doc.GetErrorOffset()
                      << " error=" << RAPIDJSON_NAMESPACE::GetParseError_En(doc.GetParseError());
              response.send(Pistache::Http::Code::Bad_Request, ss.str());
              return;
      }
      response.send(Pistache::Http::Code::Ok, "");
   }
   else
   {
      std::stringstream ss;
      ss << "Unrecognized resource [" << request.resource() << "]";
      response.send(Pistache::Http::Code::Bad_Request, ss.str() );
   }
}

