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


#include "resthandler.h"
#include "rapidjson/error/en.h"
#include "fdhss.h"

#include "logger.h"
#include "sstats.h"

#include <pistache/endpoint.h>

void RestHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response)
{
   std::cout << request.resource() << std::endl;
   std::cout << request.method() << std::endl;
   std::cout << request.body() << std::endl;

   if (request.resource() == "/imsis")
   {
      ImsiImeiData data;
      RAPIDJSON_NAMESPACE::Document doc;
      std::string keyschema("userschema");

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

      data.imsi = doc["imsi"].GetString();
      data.msisdn = doc["msisdn"].GetInt64();
      data.old_imei = doc["prev_imei"].GetString();
      data.old_imei_sv = doc["prev_imei_sv"].GetString();
      data.new_imei = doc["new_imei"].GetString();
      data.new_imei_sv = doc["new_imei_sv"].GetString();

      response.send(Pistache::Http::Code::Ok, "");

      if ((data.old_imei == data.new_imei) && (data.old_imei_sv == data.new_imei_sv))
         return;

      fdHss.sendRIR_ChangeImsiImeiSvAssn(data);
   }
   else
   {
      std::stringstream ss;
      ss << "Unrecognized resource [" << request.resource() << "]";
      response.send(Pistache::Http::Code::Bad_Request, ss.str() );
   }
}

