/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
