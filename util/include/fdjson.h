/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __FDJSON_H
#define __FDJSON_H

#include <freeDiameter/freeDiameter-host.h>
#include "freeDiameter/libfdproto.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
void fdJsonGetJSON( msg_or_avp *ref, std::string &json, void (*errfunc)(const char *) );
bool fdJsonGetValueOfMember( std::string json, std::string member, std::string &value );
bool fdJsonGetApnValueFromSubData( std::string json, std::string &apn );
extern "C" {
#endif

int fdJsonAddAvps( const char *json, msg_or_avp *msg, void (*errfunc)(const char *) );
const char *fdJsonGetJSON( msg_or_avp *msg, void (*errfunc)(const char *) );

#define FDJSON_SUCCESS             0
#define FDJSON_JSON_PARSING_ERROR  1
#define FDJSON_EXCEPTION           2

#ifdef __cplusplus
};
#endif

#endif /* #define __FDJSON_H */
