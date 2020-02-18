/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "provisionhandler.h"
#include "jsonvalidator.h"
#include "restclient.h"
#include <pistache/client.h>
#include "options.h"

#define ADD_MEMBER(doc, key, val, allocator) \
{ \
   RAPIDJSON_NAMESPACE::Value __key(key, allocator); \
   RAPIDJSON_NAMESPACE::Value __val(val, allocator); \
   doc.AddMember(__key, __val, allocator); \
}

#define ADD_MEMBER_INT64(doc, key, val, allocator) \
{ \
   RAPIDJSON_NAMESPACE::Value __key(key, allocator); \
   doc.AddMember(__key, val, allocator); \
}

void ProvisionHandler::fillEvent(RAPIDJSON_NAMESPACE::Document &document, const char *imsi, int64_t msisdn,
      const char *prev_imei, const char *prev_imei_sv, const char *new_imei, const char *new_imei_sv)
{
   document.SetObject();
   RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = document.GetAllocator();

   ADD_MEMBER( document, "imsi", imsi, allocator );
   ADD_MEMBER_INT64( document, "msisdn", msisdn, allocator );
   ADD_MEMBER( document, "prev_imei", prev_imei, allocator );
   ADD_MEMBER( document, "prev_imei_sv", prev_imei_sv, allocator );
   ADD_MEMBER( document, "new_imei", new_imei, allocator );
   ADD_MEMBER( document, "new_imei_sv", new_imei_sv, allocator );
}

void ProvisionHandler::fillBody(RAPIDJSON_NAMESPACE::Document &document, std::string &jsonbody)
{
   RAPIDJSON_NAMESPACE::StringBuffer buffer;
   buffer.Clear();
   RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(buffer);
   document.Accept(writer);
   jsonbody = buffer.GetString();
}

 void ProvisionHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    std::cout << request.resource() << std::endl;
    std::cout << request.method() << std::endl;
    std::cout << request.body() << std::endl;

    std::string url = "http://" + Options::gethsshost() + ":" + std::to_string(Options::gethssport()) + "/imsis";

    if (request.resource() == "/imsis") {
       ImsiEntity ie;
       RAPIDJSON_NAMESPACE::Document docData;
       std::string json = request.body();
       std::string keyschema("userschema");

       if(!JsonValidator::getInstance().readDocFromString(docData, json) ){
          std::cout << "Body parsing error offset="
                    << docData.GetErrorOffset()
                    << " error=" << RAPIDJSON_NAMESPACE::GetParseError_En(docData.GetParseError())
                    << std::endl;
          response.send(Pistache::Http::Code::Bad_Request, "");
          return;
       }

       if(!JsonValidator::getInstance().validate(keyschema, docData) ){
          std::cout << "Body schema validation error" << std::endl;
          response.send(Pistache::Http::Code::Bad_Request, "");
          return;
       }

       if(request.method()  == Pistache::Http::Method::Post){
          std::string imsi( docData["imsi"].GetString() );
          std::string prev_imei("");
          std::string prev_imei_sv("");
          std::string new_imei( docData["imei"].GetString() );
          std::string new_imei_sv( docData["imei_sv"].GetString() );

          if ( DB::getInstance().checkUserImsi(imsi) )
          {
             std::stringstream msg;
             msg << "Invalid POST.  IMSI [" << imsi << "] already exists.";
             std::cout << msg.str() << std::endl;
             response.send(Pistache::Http::Code::Bad_Request, msg.str());
             return;
          }

          if ( !ie.parseJson(docData) )
          {
             std::stringstream msg;
             msg << "Invalid POST.  JSON body is invalid.";
             std::cout << msg.str() << std::endl;
             response.send(Pistache::Http::Code::Bad_Request, msg.str());
             return;
          }

          if ( !ie.msisdn_pres || !ie.msisdn || DB::getInstance().checkUserMsisdn(ie.msisdn) )
          {
             std::stringstream msg;
             if ( !ie.msisdn_pres )
                msg << "Invalid POST.  MSISDN not present.";
             else if ( ie.msisdn )
                msg << "Invalid POST.  MSISDN [" << ie.msisdn << "] already exists.";
             else
                msg << "Invalid POST.  MSISDN is zero.";
             std::cout << msg.str() << std::endl;
             response.send(Pistache::Http::Code::Bad_Request, msg.str());
             return;
          }

          if ( !DB::getInstance().insertUserImsi( ie ) )
          {
             response.send(Pistache::Http::Code::Bad_Request, "Error inserting data");
             return;
          }

          RAPIDJSON_NAMESPACE::Document document;

          fillEvent(document, imsi.c_str(), ie.msisdn, prev_imei.c_str(), prev_imei_sv.c_str(), new_imei.c_str(), new_imei_sv.c_str() );
          std::string jsonbody;
          fillBody(document, jsonbody);
          RestClient::getInstance().post(url, jsonbody);

          response.send(Pistache::Http::Code::Ok, "");
          return;
       }
       else if (request.method()  == Pistache::Http::Method::Put)
       {
          std::string imsi( docData["imsi"].GetString() );
          int64_t old_msisdn;
          std::string prev_imei;
          std::string prev_imei_sv;
          std::string new_imei(docData["imei"].GetString());
          std::string new_imei_sv(docData["imei_sv"].GetString());
          DB::getInstance().getImeiMsisdnFromImsi(imsi.c_str(), prev_imei, prev_imei_sv, old_msisdn);

          ImsiEntity ie;

          if ( !ie.parseJson(docData) )
          {
             std::stringstream msg;
             msg << "Invalid POST.  JSON body is invalid.";
             std::cout << msg.str() << std::endl;
             response.send(Pistache::Http::Code::Bad_Request, msg.str());
             return;
          }

          DB::getInstance().updateUserImsi( ie );

          if (ie.msisdn_pres && ie.msisdn != old_msisdn)
          {
             // delete entry in msisdn_imsi
             DB::getInstance().deleteUserMsisdn( old_msisdn );
             if (!DB::getInstance().insertUserMsisdn( ie.imsi, ie.msisdn ))
             {
                response.send(Pistache::Http::Code::Bad_Request, "Error updating msisdn_imsi");
                return;
             }
          }

          if(prev_imei != new_imei || prev_imei_sv != new_imei_sv){
             RAPIDJSON_NAMESPACE::Document document;
             fillEvent(document, imsi.c_str(), ie.msisdn, prev_imei.c_str(), prev_imei_sv.c_str(), new_imei.c_str(), new_imei_sv.c_str() );
             std::string jsonbody;
             fillBody(document, jsonbody);
             std::cout << jsonbody << std::endl;
             RestClient::getInstance().post(url, jsonbody);
          }
          response.send(Pistache::Http::Code::Ok, "");
          return;
       }
       else{
          response.send(Pistache::Http::Code::Method_Not_Allowed, "");
          return;
       }
    }
    else if (request.method() == Pistache::Http::Method::Delete && request.resource().find("/imsis/")  != std::string::npos)
    {
       std::string searched = "/imsis/";
       std::size_t pos = request.resource().find(searched);
       std::string imsi = request.resource().substr(pos + searched.length());
       std::cout << imsi << std::endl;

       int64_t old_msisdn;
       std::string prev_imei;
       std::string prev_imei_sv;
       std::string new_imei;
       std::string new_imei_sv;
       DB::getInstance().getImeiMsisdnFromImsi(imsi.c_str(), prev_imei, prev_imei_sv, old_msisdn);
       DB::getInstance().deleteUserImsi(imsi);
       DB::getInstance().deleteUserMsisdn(old_msisdn);

       RAPIDJSON_NAMESPACE::Document document;

       fillEvent(document, imsi.c_str(), old_msisdn, prev_imei.c_str(), prev_imei_sv.c_str(), new_imei.c_str(), new_imei_sv.c_str() );
       std::string jsonbody;
       fillBody(document, jsonbody);
       RestClient::getInstance().post(url, jsonbody);

       response.send(Pistache::Http::Code::Ok, "");
       return;
    }
    else {
       std::cout << "********* not allowed" << std::endl;
       response.send(Pistache::Http::Code::Method_Not_Allowed, "");
    }
 }
