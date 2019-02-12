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


