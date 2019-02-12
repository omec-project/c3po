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

#include "jsonvalidator.h"

JsonValidator& JsonValidator::getInstance()
{
   static JsonValidator instance;
   return instance;
}

bool JsonValidator::readDocFromPath(RAPIDJSON_NAMESPACE::Document& doc,  const std::string& path ) {

   FILE* fp = fopen(path.c_str(), "r");
   if(fp == NULL){
      std::cout << "The json config file specified does not exists" << std::endl;
      return false;
   }
   char readBuffer[1024];
   RAPIDJSON_NAMESPACE::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
   if( doc.ParseStream(is).HasParseError() ){
      fclose(fp);
      return false;
   }
   fclose(fp);
   return true;
}

bool JsonValidator::readDocFromString(RAPIDJSON_NAMESPACE::Document& doc,  std::string& json ) {
   if( doc.Parse(json.c_str()).HasParseError() ) {
      return false;
   }
   return true;
}

bool JsonValidator::validate(std::string &keyschema, RAPIDJSON_NAMESPACE::Document& docData)
{
   RAPIDJSON_NAMESPACE::Document* schema = getSchema(keyschema);

   if(schema != NULL)
   {
      return validate(*schema, docData);
   }
   else
   {
      return false;
   }

}

bool JsonValidator::validate(RAPIDJSON_NAMESPACE::Document& docSchema, RAPIDJSON_NAMESPACE::Document& docData){
   RAPIDJSON_NAMESPACE::SchemaDocument schema(docSchema);
   RAPIDJSON_NAMESPACE::SchemaValidator validator(schema);

   if (!docData.Accept(validator)) {
       // Input JSON is invalid according to the schema
       // Output diagnostic information
      RAPIDJSON_NAMESPACE::StringBuffer sb;
      validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
      std::cout << "Invalid schema: " <<  sb.GetString() << std::endl;
      std::cout << "Invalid keyword: " << validator.GetInvalidSchemaKeyword() << std::endl;
      sb.Clear();
      validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
      std::cout << "Invalid document: " << sb.GetString() << std::endl;
      return false;
   }
   return true;
}

RAPIDJSON_NAMESPACE::Document* JsonValidator::getSchema(std::string &key)
{
   try{
      return m_jsonschemas.at(key);
   }
   catch(...){
      return NULL;
   }

}

void JsonValidator::addSchema(std::string &key, RAPIDJSON_NAMESPACE::Document* schema)
{
   m_jsonschemas[key] = schema;
}
