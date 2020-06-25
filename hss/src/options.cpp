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

int Options::m_logmaxsize;
int Options::m_lognbrfiles;
std::string Options::m_logfilename;
int Options::m_logqueuesize;

int Options::m_statlogmaxsize;
int Options::m_statlognbrfiles;
std::string Options::m_statlogfilename;

int Options::m_auditlogmaxsize;
int Options::m_auditlognbrfiles;
std::string Options::m_auditlogfilename;


std::string Options::m_jsoncfg;
std::string Options::m_diameterconfiguration;

std::string Options::m_cassserver;
std::string Options::m_cassuser;
std::string Options::m_casspwd;
std::string Options::m_cassdb;
unsigned    Options::m_casscoreconnections = 1;
unsigned    Options::m_cassmaxconnections = 2;
unsigned    Options::m_cassioqueuesize = 8192;
unsigned    Options::m_cassiothreads = 1;
bool        Options::m_randvector;
std::string Options::m_optkey;
bool        Options::m_reloadkey;
bool        Options::m_onlyloadkey;
int         Options::m_gtwport;
std::string Options::m_gtwhost;
int         Options::m_restport;
int         Options::m_ossport;
std::string Options::m_ossfile;
std::string Options::m_synchimsi;
std::string Options::m_synchauts;
int         Options::m_numworkers;
int         Options::m_concurrent;
uint32_t    Options::m_statsfrequency;
bool        Options::m_verify_roaming;

void Options::help()
{
   std::cout << std::endl
             << "Usage:  c3pohss -j path  [OPTIONS]..." << std::endl
             << "  -h, --help                   Print help and exit" << std::endl
             << "  -j, --jsoncfg filename       Read the application configuration from this file." << std::endl
             << "  -f, --fdcfg filename         Read the freeDiameter configuration from this file." << std::endl
             << "                               instead of the default location (" DEFAULT_CONF_PATH "/" FD_DEFAULT_CONF_FILENAME ")." << std::endl
             << "  -s, --casssrv host           Cassandra server." << std::endl
             << "  -u, --cassusr user           Cassandra user." << std::endl
             << "  -p, --casspwd password       Cassandra password." << std::endl
             << "  -d, --cassdb db              Cassandra database name." << std::endl
             << "      --casscoreconnections num   Core # Cassandra connections per host." << std::endl
             << "      --cassmaxconnections num    Max # Cassandra connections per host." << std::endl
             << "      --cassioqueuesize size   Cassandra I/O queue size." << std::endl
             << "      --cassiothreads num      Number of Cassandra I/O threads." << std::endl
             << "  -r, --randv  boolean         Random subscriber vector generation" << std::endl
             << "  -o, --optkey  key            Operator key" << std::endl
             << "  -i, --reloadkey  boolean     Reload operator keys at init" << std::endl
             << "  -q, --onlyloadkey  boolean   Only load operator keys at init" << std::endl
             << "      --synchimsi  imsi        The IMSI to calculate a new SQN for" << std::endl
             << "      --synchauts  auts        The AUTS value returned by the UE in a SYNCH FAILURE" << std::endl
             << "      --numworkers workers     The number of worker threads" << std::endl
             << "      --concurrent num         The number of concurrent transactions to process"
             << std::endl
             ;
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

   if(doc.HasMember("common")){
      const RAPIDJSON_NAMESPACE::Value& commonSection = doc["common"];
      if(!(options & diameterconfiguration) && commonSection.HasMember("fdcfg")){
         if(!commonSection["fdcfg"].IsString()){ std::cout << "Error parsing json value: [fdcfg]" << std::endl; return false; }
         m_diameterconfiguration = commonSection["fdcfg"].GetString();
         options |= diameterconfiguration;
      }
   }
   if(doc.HasMember("hss")){
      const RAPIDJSON_NAMESPACE::Value& hssSection = doc["hss"];
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
      if(hssSection.HasMember("casscoreconnections")){
         if(!hssSection["casscoreconnections"].IsInt()) { std::cout << "Error parsing json value: [casscoreconnections]" << std::endl; return false; }
         m_casscoreconnections = hssSection["casscoreconnections"].GetUint();
      }
      if(hssSection.HasMember("cassmaxconnections")){
         if(!hssSection["cassmaxconnections"].IsInt()) { std::cout << "Error parsing json value: [cassmaxconnections]" << std::endl; return false; }
         m_cassmaxconnections = hssSection["cassmaxconnections"].GetUint();
      }
      if(hssSection.HasMember("cassioqueuesize")){
         if(!hssSection["cassioqueuesize"].IsInt()) { std::cout << "Error parsing json value: [cassioqueuesize]" << std::endl; return false; }
         m_cassioqueuesize = hssSection["cassioqueuesize"].GetUint();
      }
      if(hssSection.HasMember("cassiothreads")){
         if(!hssSection["cassiothreads"].IsInt()) { std::cout << "Error parsing json value: [cassiothreads]" << std::endl; return false; }
         m_cassiothreads = hssSection["cassiothreads"].GetUint();
      }
      if(!(options & randvector) && hssSection.HasMember("randv")){
         if(!hssSection["randv"].IsBool()) { std::cout << "Error parsing json value: [randv]" << std::endl; return false; }
         m_randvector = hssSection["randv"].GetBool();
         options |= randvector;
      }
      if(!(options & optkey) && hssSection.HasMember("optkey")){
         if(!hssSection["optkey"].IsString()) { std::cout << "Error parsing json value: [optkey]" << std::endl; return false; }
         m_optkey = hssSection["optkey"].GetString();
         options |= optkey;
      }
      if(!(options & reloadkey) && hssSection.HasMember("reloadkey")){
         if(!hssSection["reloadkey"].IsBool()) { std::cout << "Error parsing json value: [reloadkey]" << std::endl; return false; }
         m_reloadkey = hssSection["reloadkey"].GetBool();
         options |= reloadkey;
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
      if(!(options & restport) && hssSection.HasMember("restport")){
         if(!hssSection["restport"].IsInt()) { std::cout << "Error parsing json value: [restport]" << std::endl; return false; }
         m_restport = hssSection["restport"].GetInt();
         options |= restport;
      }
      if(!(options & logmaxsize) && hssSection.HasMember("logsize")){
         if(!hssSection["logsize"].IsInt()) { std::cout << "Error parsing json value: [logsize]" << std::endl; return false; }
         m_logmaxsize = hssSection["logsize"].GetInt();
         options |= logmaxsize;
      }
      if(!(options & lognbrfiles) && hssSection.HasMember("lognumber")){
         if(!hssSection["lognumber"].IsInt()) { std::cout << "Error parsing json value: [lognumber]" << std::endl; return false; }
         m_lognbrfiles = hssSection["lognumber"].GetInt();
         options |= lognbrfiles;
      }
      if(!(options & logfilename) && hssSection.HasMember("logname")){
         if(!hssSection["logname"].IsString()) { std::cout << "Error parsing json value: [logname]" << std::endl; return false; }
         m_logfilename = hssSection["logname"].GetString();
         options |= logfilename;
      }
      if(!(options & logqueuesize) && hssSection.HasMember("logqsize")){
         if(!hssSection["logqsize"].IsInt()) { std::cout << "Error parsing json value: [logqsize]" << std::endl; return false; }
         m_logqueuesize = hssSection["logqsize"].GetInt();
         options |= logqueuesize;
      }
      if(!(options & statlogmaxsize) && hssSection.HasMember("statlogsize")){
         if(!hssSection["statlogsize"].IsInt()) { std::cout << "Error parsing json value: [statlogsize]" << std::endl; return false; }
         m_statlogmaxsize = hssSection["statlogsize"].GetInt();
         options |= statlogmaxsize;
      }
      if(!(options & statlognbrfiles) && hssSection.HasMember("statlognumber")){
         if(!hssSection["statlognumber"].IsInt()) { std::cout << "Error parsing json value: [statlognumber]" << std::endl; return false; }
         m_statlognbrfiles = hssSection["statlognumber"].GetInt();
         options |= statlognbrfiles;
      }
      if(!(options & statlogfilename) && hssSection.HasMember("statlogname")){
         if(!hssSection["statlogname"].IsString()) { std::cout << "Error parsing json value: [statlogname]" << std::endl; return false; }
         m_statlogfilename = hssSection["statlogname"].GetString();
         options |= statlogfilename;
      }
      if(!(options & statfrequency) && hssSection.HasMember("statfreq")){
         if(!hssSection["statfreq"].IsInt64()) { std::cout << "Error parsing json value: [statfreq]" << std::endl; return false; }
         m_statsfrequency = hssSection["statfreq"].GetInt64();
         options |= statfrequency;
      }

      if(!(options & auditlogmaxsize) && hssSection.HasMember("auditlogsize")){
         if(!hssSection["auditlogsize"].IsInt()) { std::cout << "Error parsing json value: [auditlogsize]" << std::endl; return false; }
         m_auditlogmaxsize = hssSection["auditlogsize"].GetInt();
         options |= auditlogmaxsize;
      }
      if(!(options & auditlognbrfiles) && hssSection.HasMember("auditlognumber")){
         if(!hssSection["auditlognumber"].IsInt()) { std::cout << "Error parsing json value: [auditlognumber]" << std::endl; return false; }
         m_auditlognbrfiles = hssSection["auditlognumber"].GetInt();
         options |= auditlognbrfiles;
      }
      if(!(options & auditlogfilename) && hssSection.HasMember("auditlogname")){
         if(!hssSection["auditlogname"].IsString()) { std::cout << "Error parsing json value: [auditlogname]" << std::endl; return false; }
         m_auditlogfilename = hssSection["auditlogname"].GetString();
         options |= auditlogfilename;
      }
      if(!(options & numworkers) && hssSection.HasMember("numworkers")){
         if(!hssSection["numworkers"].IsInt()) { std::cout << "Error parsing json value: [numworkers]" << std::endl; return false; }
         m_numworkers = hssSection["numworkers"].GetInt();
         options |= numworkers;
      }
      if(!(options & concurrent) && hssSection.HasMember("concurrent")){
         if(!hssSection["concurrent"].IsInt()) { std::cout << "Error parsing json value: [concurrent]" << std::endl; return false; }
         m_concurrent = hssSection["concurrent"].GetInt();
         options |= concurrent;
      }

      if(!(options & ossport) && hssSection.HasMember("ossport")){
         if(!hssSection["ossport"].IsInt()) { std::cout << "Error parsing json value: [ossport]" << std::endl; return false; }
         m_ossport = hssSection["ossport"].GetInt();
         options |= ossport;
      }

      if(!(options & ossfile) && hssSection.HasMember("ossfile")){
         if(!hssSection["ossfile"].IsString()) { std::cout << "Error parsing json value: [ossfile]" << std::endl; return false; }
         m_ossfile = hssSection["ossfile"].GetString();
         options |= ossfile;
      }
      m_verify_roaming = false; /* disable by default */
      if(hssSection.HasMember("verifyroamingsubscribers")){
         if(!hssSection["verifyroamingsubscribers"].IsBool()) { std::cout << "Error parsing json value: [verifyroamingsubscribers]" << std::endl; return false; }
         m_verify_roaming = hssSection["verifyroamingsubscribers"].GetBool();
      }
 
   }

   return true;
}

bool Options::validateOptions(){

   return (
            (options & diameterconfiguration)
         && (options & cassserver)
         && (options & cassuser)
         && (options & casspwd)
         && (options & cassdb)
         && (options & optkey)
         && (options & gtwport)
         && (options & gtwhost)
         && (options & restport)
         && (options & logmaxsize)
         && (options & lognbrfiles)
         && (options & logfilename)
         && (options & logqueuesize)
         && (options & statlogmaxsize)
         && (options & statlognbrfiles)
         && (options & statlogfilename)
         && (options & statfrequency)
         && (options & auditlogmaxsize)
         && (options & auditlognbrfiles)
         && (options & auditlogfilename)
         && (options & numworkers)
         && (options & concurrent)
         && (options & ossport)
         && (options & ossfile)
         );
}

bool Options::parseInputOptions( int argc, char **argv )
{
   int c;
   int option_index = 0;
   bool result = true;

   struct option long_options[] = {
      { "help",            no_argument,        NULL, 'h' },
      { "jsoncfg",         required_argument,  NULL, 'j' },
      { "fdcfg",           required_argument,  NULL, 'f' },

      { "casssrv",         required_argument,  NULL, 's' },
      { "cassusr",         required_argument,  NULL, 'u' },
      { "casspwd",         required_argument,  NULL, 'p' },
      { "cassdb",          required_argument,  NULL, 'd' },

      { "randv",           no_argument,        NULL, 'r' },
      { "optkey",          required_argument,  NULL, 'o' },
      { "reloadkey",       no_argument,        NULL, 'i' },
      { "onlyloadkey",     no_argument,        NULL, 'q' },
      { "synchimsi",       required_argument,  NULL, 'x' },
      { "synchauts",       required_argument,  NULL, 'y' },
      { "numworkers",      required_argument,  NULL, 'z' },
      { "concurrent",      required_argument,  NULL, 'A' },

      { "logname",         required_argument,  NULL, 'l' },
      { "lognumber",       required_argument,  NULL, 'n' },
      { "logqsize",        required_argument,  NULL, 't' },
      { "logsize",         required_argument,  NULL, 'v' },

      { "statlogname",         required_argument,  NULL, 'L' },
      { "statlognumber",       required_argument,  NULL, 'N' },
      { "statlogsize",         required_argument,  NULL, 'V' },
      { "statfrequency",       required_argument,  NULL, 'W' },

      { "casscoreconnections",   required_argument,  NULL, 'C' },
      { "cassmaxconnections",    required_argument,  NULL, 'D' },
      { "cassioqueuesize",       required_argument,  NULL, 'E' },
      { "cassiothreads",         required_argument,  NULL, 'F' },

      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "hj:f:s:u:p:d:ro:iqx:y:l:L:n:N:t:v:V:W:z:A:C:D:E:F:", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'h': { help(); exit(0);                                                              break; }
         case 'j': { m_jsoncfg = optarg;                       options |= jsoncfg;                 break; }
         case 'f': { m_diameterconfiguration = optarg;         options |= diameterconfiguration;   break; }
         case 's': { m_cassserver = optarg;                    options |= cassserver;              break; }
         case 'u': { m_cassuser = optarg;                      options |= cassuser;                break; }
         case 'p': { m_casspwd = optarg;                       options |= casspwd;                 break; }
         case 'd': { m_cassdb = optarg;                        options |= cassdb;                  break; }
         case 'r': { m_randvector = true;                      options |= randvector;              break; }
         case 'o': { m_optkey = true;                          options |= optkey;                  break; }
         case 'i': { m_reloadkey = true;                       options |= reloadkey;               break; }
         case 'q': { m_onlyloadkey = true;                     options |= onlyloadkey;             break; }
         case 'x': { m_synchimsi = optarg;                                                         break; }
         case 'y': { m_synchauts = optarg;                                                         break; }
         case 'v': { m_logmaxsize = atoi(optarg);              options |= logmaxsize;              break; }
         case 'V': { m_statlogmaxsize = atoi(optarg);          options |= statlogmaxsize;          break; }
         case 'n': { m_lognbrfiles = atoi(optarg);             options |= lognbrfiles;             break; }
         case 'N': { m_statlognbrfiles = atoi(optarg);         options |= statlognbrfiles;         break; }
         case 'l': { m_logfilename = optarg;                   options |= logfilename;             break; }
         case 'L': { m_statlogfilename = optarg;               options |= statlogfilename;         break; }
         case 't': { m_logqueuesize = atoi(optarg);            options |= logqueuesize;            break; }
         case 'W': { m_statsfrequency = atol(optarg);          options |= statfrequency;           break; }
         case 'z': { m_numworkers = atoi(optarg);              options |= numworkers;              break; }
         case 'A': { m_concurrent = atoi(optarg);              options |= concurrent;              break; }
         case 'C': { m_casscoreconnections = atoi(optarg);                                         break; }
         case 'D': { m_cassmaxconnections = atoi(optarg);                                          break; }
         case 'E': { m_cassioqueuesize = atoi(optarg);                                             break; }
         case 'F': { m_cassiothreads = atoi(optarg);                                               break; }

         case '?':
         {
            switch ( optopt )
            {
               case 'j': { std::cout << "Option -j (config json file) requires an argument"           << std::endl; break; }
               case 'f': { std::cout << "Option -f (diameter config) requires an argument"            << std::endl; break; }
               case 's': { std::cout << "Option -s (cass server) requires an argument"                << std::endl; break; }
               case 'u': { std::cout << "Option -u (cass user) requires an argument"                  << std::endl; break; }
               case 'p': { std::cout << "Option -p (cass pwd) requires an argument"                   << std::endl; break; }
               case 'd': { std::cout << "Option -d (cass db) requires an argument"                    << std::endl; break; }
               case 'c': { std::cout << "Option -c (cassthreads) requires an argument"                << std::endl; break; }
               case 'o': { std::cout << "Option -o (operator key) requires an argument"               << std::endl; break; }
               case 'x': { std::cout << "Option --synchimsi requires an argument"                     << std::endl; break; }
               case 'y': { std::cout << "Option --synchauts requires an argument"                     << std::endl; break; }
               case 'l': { std::cout << "Option -l (log file name) requires an argument"              << std::endl; break; }
               case 'L': { std::cout << "Option -L (stat log file name) requires an argument"         << std::endl; break; }
               case 'n': { std::cout << "Option -n (log number of files) requires an argument"        << std::endl; break; }
               case 'N': { std::cout << "Option -N (stat log number of files) requires an argument"   << std::endl; break; }
               case 't': { std::cout << "Option -t (log queue size) requires an argument"             << std::endl; break; }
               case 'v': { std::cout << "Option -v (log size) requires an argument"                   << std::endl; break; }
               case 'V': { std::cout << "Option -v (stat log size) requires an argument"              << std::endl; break; }
               case 'z': { std::cout << "Option -z (numworkers) requires an argument"                 << std::endl; break; }
               case 'A': { std::cout << "Option --concurrent requires an argument"                    << std::endl; break; }
               case 'C': { std::cout << "Option --casscoreconnections requires an argument"           << std::endl; break; }
               case 'D': { std::cout << "Option --cassmaxconnections requires an argument"            << std::endl; break; }
               case 'E': { std::cout << "Option --cassioqueuesize requires an argument"               << std::endl; break; }
               case 'F': { std::cout << "Option --cassiothreads requires an argument"                 << std::endl; break; }
               default: { std::cout << "Unrecognized option [" << c << "]"                            << std::endl; break; }
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

void Options::fillhssconfig(hss_config_t *hss_config_p) 
{

   hss_config_p->cassandra_server      = strdup(m_cassserver.c_str());
   hss_config_p->cassandra_user        = strdup(m_cassuser.c_str());
   hss_config_p->cassandra_password    = strdup(m_casspwd.c_str());
   hss_config_p->cassandra_database    = strdup(m_cassdb.c_str());
   hss_config_p->operator_key          = strdup(m_optkey.c_str());
   hss_config_p->freediameter_config   = strdup(m_diameterconfiguration.c_str());


   if (strlen (hss_config_p->operator_key) == 32) {
     int ret = sscanf (hss_config_p->operator_key,
                   "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                   (unsigned int *)&hss_config_p->operator_key_bin[0], (unsigned int *)&hss_config_p->operator_key_bin[1],
                   (unsigned int *)&hss_config_p->operator_key_bin[2], (unsigned int *)&hss_config_p->operator_key_bin[3],
                   (unsigned int *)&hss_config_p->operator_key_bin[4], (unsigned int *)&hss_config_p->operator_key_bin[5],
                   (unsigned int *)&hss_config_p->operator_key_bin[6], (unsigned int *)&hss_config_p->operator_key_bin[7],
                   (unsigned int *)&hss_config_p->operator_key_bin[8], (unsigned int *)&hss_config_p->operator_key_bin[9],
                   (unsigned int *)&hss_config_p->operator_key_bin[10], (unsigned int *)&hss_config_p->operator_key_bin[11],
                   (unsigned int *)&hss_config_p->operator_key_bin[12], (unsigned int *)&hss_config_p->operator_key_bin[13], (unsigned int *)&hss_config_p->operator_key_bin[14], (unsigned int *)&hss_config_p->operator_key_bin[15]);

     if (ret != 16) {
       printf( "Error in configuration file: operator key: %s\n", hss_config_p->operator_key);
       abort ();
     }
     hss_config_p->valid_op = 1;
   }

   if(m_randvector) {
      hss_config_p->random = (char*)"true";
      hss_config_p->random_bool = 1;
   }
   else{
      hss_config_p->random = (char*)"false";
   }
   hss_config_p->verify_roaming = m_verify_roaming;
}

