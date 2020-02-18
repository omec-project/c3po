/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <freeDiameter/freeDiameter-host.h>

#define RAPIDJSON_NAMESPACE hssrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include "options.h"

const int Options::JSONFILEBUFFER = 1024;

int Options::options;

std::string Options::m_jsoncfg;

std::string Options::m_cassserver;
std::string Options::m_cassuser;
std::string Options::m_casspwd;
std::string Options::m_cassdb;
std::string Options::m_user_schema;
int         Options::m_gtwport;
std::string Options::m_gtwhost;
int         Options::m_hssport;
std::string Options::m_hsshost;




void Options::help()
{
   std::cout << std::endl
             << "Usage:  c3pohss -j path  [OPTIONS]..." << std::endl
             << "  -h, --help                   Print help and exit" << std::endl
             << "  -j, --jsoncfg filename       Read the application configuration from this file." << std::endl
             << "  -s, --casssrv host           Cassandra server." << std::endl
             << "  -u, --cassusr user           Cassandra user." << std::endl
             << "  -p, --casspwd password       Cassandra password." << std::endl
             << "  -d, --cassdb db              Cassandra database name." << std::endl
             << "  -e, --usersch user_schema    Json user schema." << std::endl;
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
   RAPIDJSON_NAMESPACE::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
   RAPIDJSON_NAMESPACE::Document doc;
   doc.ParseStream(is);
   fclose(fp);

   if(!doc.IsObject()) {
      std::cout << "Error parsing the json config file" << std::endl;
      return false;
   }

   if(doc.HasMember("hssgtw")){
      const RAPIDJSON_NAMESPACE::Value& hssSection = doc["hssgtw"];
      if(!(options & cassserver) && hssSection.HasMember("casssrv")){
         if(!hssSection["casssrv"].IsString()) { std::cout << "Error parsing json value: [cassserver]" << std::endl; return false; }
         m_cassserver = hssSection["casssrv"].GetString();
         options |= cassserver;
      }
      if(!(options & cassuser) && hssSection.HasMember("cassusr")){
         if(!hssSection["cassusr"].IsString()) { std::cout << "Error parsing json value: [cassuser]" << std::endl; return false; }
         m_cassuser = hssSection["cassusr"].GetString();
         options |= cassuser;
      }
      if(!(options & casspwd) && hssSection.HasMember("casspwd")){
         if(!hssSection["casspwd"].IsString()) { std::cout << "Error parsing json value: [casspwd]" << std::endl; return false; }
         m_casspwd = hssSection["casspwd"].GetString();
         options |= casspwd;
      }
      if(!(options & cassdb) && hssSection.HasMember("cassdb")){
         if(!hssSection["cassdb"].IsString()) { std::cout << "Error parsing json value: [cassdb]" << std::endl; return false; }
         m_cassdb = hssSection["cassdb"].GetString();
         options |= cassdb;
      }
      if(!(options & userschema) && hssSection.HasMember("userschema")){
         if(!hssSection["userschema"].IsString()) { std::cout << "Error parsing json value: [userschema]" << std::endl; return false; }
         m_user_schema = hssSection["userschema"].GetString();
         options |= userschema;
      }
      if(!(options & gtwport) && hssSection.HasMember("gtwport")){
         if(!hssSection["gtwport"].IsInt()) { std::cout << "Error parsing json value: [gtwport]" << std::endl; return false; }
         m_gtwport = hssSection["gtwport"].GetInt();
         options |= gtwport;
      }
      if(!(options & gtwhost) && hssSection.HasMember("gtwhost")){
         if(!hssSection["gtwhost"].IsString()) { std::cout << "Error parsing json value: [gtwhost]" << std::endl; return false; }
         m_gtwhost = hssSection["gtwhost"].GetString();
         options |= gtwhost;
      }

      if(!(options & hssport) && hssSection.HasMember("hss_target_port")){
         if(!hssSection["hss_target_port"].IsInt()) { std::cout << "Error parsing json value: [hss_target_port]" << std::endl; return false; }
         m_hssport = hssSection["hss_target_port"].GetInt();
         options |= hssport;
      }
      if(!(options & hsshost) && hssSection.HasMember("hss_target_host")){
         if(!hssSection["hss_target_host"].IsString()) { std::cout << "Error parsing json value: [hss_host_port]" << std::endl; return false; }
         m_hsshost = hssSection["hss_target_host"].GetString();
         options |= hsshost;
      }
   }

   return true;
}

bool Options::validateOptions(){

   return (
            (options & cassserver)
         && (options & cassuser)
         && (options & casspwd)
         && (options & cassdb)
         && (options & gtwport)
         && (options & gtwhost)
         );


}

bool Options::parseInputOptions( int argc, char **argv )
{
   int c;
   int option_index = 0;
   bool result = true;

   struct option long_options[] = {
      { "help",         no_argument,        NULL, 'h' },
      { "jsoncfg",      required_argument,  NULL, 'j' },

      { "casssrv",      required_argument,  NULL, 's' },
      { "cassusr",      required_argument,  NULL, 'u' },
      { "casspwd",      required_argument,  NULL, 'p' },
      { "cassdb",       required_argument,  NULL, 'd' },

      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "hj:s:u:p:d:", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'h': { help(); exit(0);                                                              break; }
         case 'j': { m_jsoncfg = optarg;                                                           break; }

         case 's': { m_cassserver = optarg;                    options |= cassserver;              break; }
         case 'u': { m_cassuser = optarg;                      options |= cassuser;                break; }
         case 'p': { m_casspwd = optarg;                       options |= casspwd;                 break; }
         case 'd': { m_cassdb = optarg;                        options |= cassdb;                  break; }

         case '?':
         {
            switch ( optopt )
            {
               case 'j': { std::cout << "Option -j (config json file) requires an argument"        << std::endl; break; }
               case 's': { std::cout << "Option -s (cass server) requires an argument"             << std::endl; break; }
               case 'u': { std::cout << "Option -u (cass user) requires an argument"               << std::endl; break; }
               case 'p': { std::cout << "Option -p (cass pwd) requires an argument"                << std::endl; break; }
               case 'd': { std::cout << "Option -d (cass db) requires an argument"                 << std::endl; break; }
               default: { std::cout << "Unrecognized option [" << c << "]"                         << std::endl; break; }
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
   return result;
}

