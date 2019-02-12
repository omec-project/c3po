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



#ifndef HSSGTW_INCLUDE_RESTCLIENT_H_
#define HSSGTW_INCLUDE_RESTCLIENT_H_


#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/client.h>


class RestClient {

   public:
      static RestClient& getInstance();
      void post(std::string &url, std::string &body);

   private:
      RestClient();
      Pistache::Http::Client m_restclient;
};


#endif /* HSSGTW_INCLUDE_RESTCLIENT_H_ */
