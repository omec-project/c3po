/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef HSSGTW_INCLUDE_JSONVALIDATOR_H_
#define HSSGTW_INCLUDE_JSONVALIDATOR_H_

#include <string>
#include <iostream>
#include <map>

#define RAPIDJSON_NAMESPACE hssrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class JsonValidator {

public:

   static JsonValidator& getInstance();

   bool readDocFromPath(RAPIDJSON_NAMESPACE::Document& doc,  const std::string& path );

   bool readDocFromString(RAPIDJSON_NAMESPACE::Document& doc,  std::string& json );

   bool validate(std::string &keyschema, RAPIDJSON_NAMESPACE::Document& docData);

   bool validate(RAPIDJSON_NAMESPACE::Document& docSchema, RAPIDJSON_NAMESPACE::Document& docData);

   RAPIDJSON_NAMESPACE::Document* getSchema(std::string &key);

   void addSchema(std::string &key, RAPIDJSON_NAMESPACE::Document* schema);

private:

   std::map<std::string, RAPIDJSON_NAMESPACE::Document*> m_jsonschemas;

};



#endif /* HSSGTW_INCLUDE_JSONVALIDATOR_H_ */
