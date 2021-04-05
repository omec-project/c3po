/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/



#ifndef PCRF_INCLUDE_PROVISIONHANDLER_H_
#define PCRF_INCLUDE_PROVISIONHANDLER_H_

#include <iostream>
#include <pistache/endpoint.h>
#include <pistache/router.h>

#include <memory>

#define RAPIDJSON_NAMESPACE fdrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"


extern RAPIDJSON_NAMESPACE::Document docSchema;

class SStats;

class RestHandler : public Pistache::Http::Handler {

public:
    HTTP_PROTOTYPE(RestHandler)
    void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
};

#endif  /* PCRF_INCLUDE_PROVISIONHANDLER_H_ */
