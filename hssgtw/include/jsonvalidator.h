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
