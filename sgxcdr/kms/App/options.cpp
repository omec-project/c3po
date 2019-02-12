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


#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include "options.h"

const int Options::JSONFILEBUFFER = 1024;

int Options::options;

std::string Options::m_jsoncfg;
std::string Options::m_storepath;
std::string Options::m_port;
size_t Options::m_maxconnections;

void Options::help()
{
   std::cout << std::endl
             << "Usage:  kms [OPTIONS]..." << std::endl
             << "  -h, --help                   Print help and exit" << std::endl
             << "  -j, --jsoncfg filename       Read the application configuration from this file." << std::endl
             << "  -p, --port port    		    Port number for kms." << std::endl
			 << "  -s, --storepath path    		Path to store sealed key files." << std::endl
             << "  -n, --maxconnections number  Maximum no. of connections to be allowed by the kms." << std::endl;
}

bool Options::parse( int argc, char **argv ){

   bool ret = true;

   ret = parseInputOptions( argc, argv );

   if(ret && !m_jsoncfg.empty()){
      ret &= parseJson();
   }

   ret &= validateOptions();

   return ret;
}

bool Options::parseJson(){

   FILE* fp = fopen(m_jsoncfg.c_str(), "r");

   if(fp == NULL){
      std::cout << "The json config file specified does not exists" << std::endl;
      return false;
   }

   char readBuffer[JSONFILEBUFFER];
   rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
   rapidjson::Document doc;
   doc.ParseStream(is);
   fclose(fp);

   if(!doc.IsObject()) {
      std::cout << "Error parsing the json config file" << std::endl;
      return false;
   }

   if(doc.HasMember("kms")){
      const rapidjson::Value& kmsSection = doc["kms"];
      if(!(options & storepath) && kmsSection.HasMember("storepath")){
         if(!kmsSection["storepath"].IsString()) { std::cout << "Error parsing json value: [storepath]" << std::endl; return false; }
         m_storepath = kmsSection["storepath"].GetString();
         options |= storepath;
      }
      if(!(options & maxconnections) && kmsSection.HasMember("maxconnections")){
         if(!kmsSection["maxconnections"].IsString()) { std::cout << "Error parsing json value: [maxconnections]" << std::endl; return false; }
         m_maxconnections = atoi(kmsSection["maxconnections"].GetString());
         options |= maxconnections;
      }
      if(!(options & port) && kmsSection.HasMember("port")){
         if(!kmsSection["port"].IsString()) { std::cout << "Error parsing json value: [port]" << std::endl; return false; }
         m_port = kmsSection["port"].GetString();
         options |= port;
      }
   }


   return true;
}

bool Options::validateOptions(){
   // validate max connection range
   if(options & maxconnections)
   {
	size_t conn = m_maxconnections;
	if( conn <=0 || conn > 100)
	{
		std::cout << "Invalid value for max connections: " << conn << ". It must be between 1 to 100" << std::endl;
		return false;
	}
   }
   
   // set default value for port to 443
   if(!(options & port))
   {
	m_port = "443";
	options |= port;
   }

   return (
            (options & storepath)
         && (options & port)
         && (options & maxconnections)
         );
}

bool Options::parseInputOptions( int argc, char **argv )
{
   int c;
   int option_index = 0;
   bool result = true;

   struct option long_options[] = {
      { "help",         	no_argument,        NULL, 'h' },
      { "jsoncfg",      	required_argument,  NULL, 'j' },
      { "storepath",      	required_argument,  NULL, 's' },
      { "maxconnections",	required_argument,  NULL, 'n' },
      { "port",     		required_argument,  NULL, 'p' },
      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "hj:s:n:p:", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'h': { help(); exit(0);                                                          break; }
         case 'j': { m_jsoncfg = optarg;                   options |= jsoncfg;                 break; }
         case 's': { m_storepath = optarg;			       options |= storepath;               break; }
         case 'n': { m_maxconnections = atoi(optarg);      options |= maxconnections;          break; }
         case 'p': { m_port = optarg;			           options |= port;             	   break; }
         case '?':
         {
            switch ( optopt )
            {
               case 'j': { std::cout << "Option -j (config json file) requires an argument"    << std::endl; break; }
               case 's': { std::cout << "Option -s (path to store keys) requires an argument"  << std::endl; break; }
               case 'n': { std::cout << "Option -n (max connections) requires an argument"     << std::endl; break; }
               case 'p': { std::cout << "Option -p (Port) requires an argument"                << std::endl; break; }
               default: { std::cout << "Unrecognized option [" << c << "]"                     << std::endl; break; }
            }
            result = false;
            break;
         }
         default:
         {
            std::cout << "Unrecognized option [" << c << "]" << std::endl;
            result = false;
         }
      }
   }

   // set default config file location to "conf/kms.json"
   // this settings has to be checked here
   if(!(options & jsoncfg))
   {
	std::cout << "Looking for default config file into conf/kms.json" << std::endl;
	m_jsoncfg = "conf/kms.json";
	options |= jsoncfg;
   }


   return result;
}
