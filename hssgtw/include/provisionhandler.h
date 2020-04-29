/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#ifndef HSSGTW_INCLUDE_PROVISIONHANDLER_H_
#define HSSGTW_INCLUDE_PROVISIONHANDLER_H_

#include <iostream>
#include <pistache/endpoint.h>
#include "db.h"

#include <pistache/client.h>

#define RAPIDJSON_NAMESPACE hssrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

extern RAPIDJSON_NAMESPACE::Document docSchema;

class ProvisionHandler : public Pistache::Http::Handler {

public:
    HTTP_PROTOTYPE(ProvisionHandler)

    void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);

    void fillEvent(RAPIDJSON_NAMESPACE::Document &document, const char *imsi, int64_t msisdn,
          const char *prev_imei, const char *prev_imei_sv, const char *new_imei, const char *new_imei_sv);
    void fillBody(RAPIDJSON_NAMESPACE::Document &document, std::string &jsonbody);

};

#endif /* HSSGTW_INCLUDE_PROVISIONHANDLER_H_ */
