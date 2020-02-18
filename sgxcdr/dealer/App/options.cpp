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

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include "options.h"

const int Options::JSONFILEBUFFER = 1024;

int Options::options;

std::string Options::m_jsoncfg;
std::string Options::m_runmode;
std::string Options::m_cdrrouterhost;
size_t Options::m_cdrrouterport;
std::string Options::m_dealerport;
size_t Options::m_maxconnections;
std::string Options::m_kmsserver;
std::string Options::m_kmsport;
std::string Options::m_keynamespace;
std::string Options::m_cdrpath;
std::string Options::m_cdrarchpath;
int Options::dryRun;

void Options::help()
{
   std::cout << std::endl
             << "Usage:  dealer [OPTIONS]..." << std::endl
             << "  -h, --help                   Print help and exit" << std::endl
             << "  -m, --runmode mode		Dealer run mode. IN|OUT" << std::endl
             << "  -j, --jsoncfg filename       Read the application configuration from this file." << std::endl
             << "  -r, --cdrhost host         	Hostname/IP of the CDR router host" << std::endl
             << "  -q, --cdrport port    	Port number of CDR router." << std::endl
             << "  -p, --port port    		Port number for dealer." << std::endl
             << "  -k, --kmsserver host		Hostname/IP of KMS server." << std::endl
             << "  -l, --kmsport port  		Port number for KMS server." << std::endl
             << "  -s, --keynamespace namespace	Namespace to be used for fetching key from KMS." << std::endl
             << "  -n, --maxconnections number  Maximum no. of connections to be allowed by the dealer." << std::endl
             << "  -c, --cdrpath  Path to be used for storing protected file temporarily." << std::endl
             << "  -d, --cdrarchpath  Path to be used for archieving protected file." << std::endl
             << "  -x, --dryRun                 Print MRENCLAVE & MRSIGNER of dealer." << std::endl;
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

   if(doc.HasMember("common")){
      const rapidjson::Value& commonSection = doc["common"];
      if(!(options & maxconnections) && commonSection.HasMember("maxconnections")){
         if(!commonSection["maxconnections"].IsString()){ std::cout << "Error parsing json value: [maxconnections]" << std::endl; return false; }
         m_maxconnections = atoi(commonSection["maxconnections"].GetString());
         options |= maxconnections;
      }
      if(!(options & kmsserver) && commonSection.HasMember("kmsserver")){
         if(!commonSection["kmsserver"].IsString()){ std::cout << "Error parsing json value: [kmsserver]" << std::endl; return false; }
         m_kmsserver = commonSection["kmsserver"].GetString();
         options |= kmsserver;
      }
      if(!(options & kmsport) && commonSection.HasMember("kmsport")){
         if(!commonSection["kmsport"].IsString()){ std::cout << "Error parsing json value: [kmsport]" << std::endl; return false; }
         m_kmsport = commonSection["kmsport"].GetString();
         options |= kmsport;
      }
      if(!(options & keynamespace) && commonSection.HasMember("keynamespace")){
         if(!commonSection["keynamespace"].IsString()){ std::cout << "Error parsing json value: [keynamespace]" << std::endl; return false; }
         m_keynamespace = commonSection["keynamespace"].GetString();
         options |= keynamespace;
      }
   }
   if(doc.HasMember("dealer")){
      const rapidjson::Value& dealerSection = doc["dealer"];
      if(!(options & runmode) && dealerSection.HasMember("runmode")){
         if(!dealerSection["runmode"].IsString()) { std::cout << "Error parsing json value: [runmode]" << std::endl; return false; }
         m_runmode = dealerSection["runmode"].GetString();
         options |= runmode;
      }
      if(!(options & cdrrouterhost) && dealerSection.HasMember("cdrhost")){
         if(!dealerSection["cdrhost"].IsString()) { std::cout << "Error parsing json value: [cdrhost]" << std::endl; return false; }
         m_cdrrouterhost = dealerSection["cdrhost"].GetString();
         options |= cdrrouterhost;
      }
      if(!(options & cdrrouterport) && dealerSection.HasMember("cdrport")){
         if(!dealerSection["cdrport"].IsString()) { std::cout << "Error parsing json value: [cdrport]" << std::endl; return false; }
         m_cdrrouterport = atoi(dealerSection["cdrport"].GetString());
         options |= cdrrouterport;
      }
      if(!(options & dealerport) && dealerSection.HasMember("port")){
         if(!dealerSection["port"].IsString()) { std::cout << "Error parsing json value: [port]" << std::endl; return false; }
         m_dealerport = dealerSection["port"].GetString();
         options |= dealerport;
      }
      if(!(options & cdrpath) && dealerSection.HasMember("cdrpath")){
         if(!dealerSection["cdrpath"].IsString()) { std::cout << "Error parsing json value: [cdrpath]" << std::endl; return false; }
         m_cdrpath = dealerSection["cdrpath"].GetString();
         options |= cdrpath;
      }
      if(!(options & cdrarchpath) && dealerSection.HasMember("cdrarchpath")){
         if(!dealerSection["cdrarchpath"].IsString()) { std::cout << "Error parsing json value: [cdrarchpath]" << std::endl; return false; }
         m_cdrarchpath = dealerSection["cdrarchpath"].GetString();
         options |= cdrarchpath;
      }
   }


   return true;
}

bool Options::validateOptions(){
   // validate run mode
   if(options & runmode)
   {
	if( !(m_runmode == std::string("IN") || m_runmode == std::string("OUT")) )
	{
		std::cout << "Invalid run mode: " << m_runmode << ". It must be IN|OUT" << std::endl;
		return false;
	}
   }

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
   if(!(options & dealerport))
   {
	m_dealerport = "443";
	options |= dealerport;
   }

   return (
            (options & runmode)
         && (options & cdrrouterhost)
         && (options & cdrrouterport)
         && (options & dealerport)
         && (options & maxconnections)
         && (options & kmsserver)
         && (options & kmsport)
         && (options & keynamespace)
         && (options & cdrpath)
         && (options & cdrarchpath)
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
      { "runmode",      	required_argument,  NULL, 'm' },
      { "maxconnections",	required_argument,  NULL, 'n' },
      { "cdrhost",     		required_argument,  NULL, 'r' },
      { "cdrport",     		required_argument,  NULL, 'q' },
      { "port",     		required_argument,  NULL, 'p' },
      { "kmsserver",   		required_argument,  NULL, 'k' },
      { "kmsport",     		required_argument,  NULL, 'l' },
      { "keynamespace",		required_argument,  NULL, 's' },
      { "cdrpath",	    	required_argument,  NULL, 'c' },
      { "cdrarchpath",		required_argument,  NULL, 'd' },
      { "dryRun",         	no_argument,        NULL, 'x' },
      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "hxj:m:n:r:q:p:k:l:s:c:d:", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'h': { help(); exit(0);                                                              break; }
         case 'x': { Options::dryRun = 1;                                                          break; }
         case 'j': { m_jsoncfg = optarg;                       options |= jsoncfg;                 break; }
         case 'm': { m_runmode = optarg;		               options |= runmode;                 break; }
         case 'n': { m_maxconnections = atoi(optarg);          options |= maxconnections;          break; }
         case 'r': { m_cdrrouterhost = optarg;                 options |= cdrrouterhost;           break; }
         case 'q': { m_cdrrouterport = atoi(optarg);           options |= cdrrouterport;           break; }
         case 'p': { m_dealerport = optarg;		               options |= dealerport;              break; }
         case 'k': { m_kmsserver = optarg;		               options |= kmsserver;               break; }
         case 'l': { m_kmsport = atoi(optarg);		           options |= kmsport;                 break; }
         case 's': { m_keynamespace = optarg;		           options |= keynamespace;            break; }
         case 'c': { m_cdrpath = optarg;     		           options |= cdrpath;                 break; }
         case 'd': { m_cdrarchpath = optarg;		           options |= cdrarchpath;             break; }
         case '?':
         {
            switch ( optopt )
            {
               case 'j': { std::cout << "Option -j (config json file) requires an argument"        << std::endl; break; }
               case 'm': { std::cout << "Option -m (runmode for the dealer) requires an argument IN|OUT"  << std::endl; break; }
               case 'n': { std::cout << "Option -n (max connections) requires an argument"               << std::endl; break; }
               case 'r': { std::cout << "Option -r (CDR Router Host) requires an argument"            << std::endl; break; }
               case 'q': { std::cout << "Option -q (CDR Router Port) requires an argument"           << std::endl; break; }
               case 'p': { std::cout << "Option -p (Port) requires an argument"           << std::endl; break; }
               case 'k': { std::cout << "Option -k (KMS Host) requires an argument"           << std::endl; break; }
               case 'l': { std::cout << "Option -l (KMS Port) requires an argument"           << std::endl; break; }
               case 's': { std::cout << "Option -s (Key Namespace) requires an argument"           << std::endl; break; }
               case 'c': { std::cout << "Option -c (CDR Path) requires an argument"           << std::endl; break; }
               case 'd': { std::cout << "Option -d (CDR Archieve Path) requires an argument"           << std::endl; break; }
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

   // set default config file location to "conf/dealer.json"
   // this settings has to be checked here
   if(!(options & jsoncfg))
   {
	std::cout << "Looking for default config file into conf/dealer.json" << std::endl;
	m_jsoncfg = "conf/dealer.json";
	options |= jsoncfg;
   }


   return result;
}
