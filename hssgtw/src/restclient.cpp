/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include "restclient.h"

RestClient& RestClient::getInstance(){
   static RestClient instance;
   return instance;
}


void RestClient::post(std::string &url, std::string &body){

   std::cout << "sending request" << std::endl;
   auto resp = m_restclient.post(url).body(body).send();
   resp.then([&](Pistache::Http::Response response) {
       std::cout << "Response code = " << response.code() << std::endl;
       auto body = response.body();
       if (!body.empty())
          std::cout << "Response body = " << body << std::endl;
   }, Pistache::Async::IgnoreException);

}

RestClient::RestClient(){
   //TOO move these harcoded values to options
   auto opts = Pistache::Http::Client::options()
       .threads(1)
       .maxConnectionsPerHost(8);
   m_restclient.init(opts);
}


