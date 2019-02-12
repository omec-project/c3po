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

#include <freeDiameter/freeDiameter-host.h>

#include "options.h"

#define RAPIDJSON_NAMESPACE scefrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

Options *Options::m_singleton = NULL;

Options::Options()
{
}

Options::~Options()
{
}

void Options::help()
{
   std::cout << std::endl
             << "Usage:  c3poctf -t casshost [OPTIONS]..." << std::endl
             << "  -h, --help                   Print help and exit" << std::endl
             << "  -j, --jsoncfg filename       The JSON configuration file." << std::endl
             << "  -s, --originhost host        The diameter destination host." << std::endl
             << "  -r, --originrealm realm      The diameter destination realm." << std::endl
             << "  -a, --hsshost host           The diameter host for the HSS." << std::endl
             << "  -b, --hssrealm realm         The diameter realm for the HSS." << std::endl
             << "  -z, --logsize size           The maximum log size in MB." << std::endl
             << "  -n, --lognumber number       The number of log files to maintain." << std::endl
             << "  -l, --logname filename       The base filename for the log files." << std::endl
             << "  -q, --logqsize size          The log queue size in bytes, must be a power of 2." << std::endl
             << "  -c, --fdcfg filename         Read the freeDiameter configuration from this file" << std::endl
             << "                               instead of the default location (" DEFAULT_CONF_PATH "/" FD_DEFAULT_CONF_FILENAME ")." << std::endl
   ;
}

bool Options::parse( int argc, char **argv ){

   bool ret = true;

   ret = singleton().parseInputOptions( argc, argv );

   if(ret && !singleton().m_jsoncfg.empty()){
      ret &= singleton().parseJson();
   }

   ret &= singleton().validateOptions();

   return ret;
}


bool Options::parseInputOptions( int argc, char **argv )
{
   int c;
   int option_index = 0;
   bool result = true;
   bool ffound = false;

   struct option long_options[] = {
      { "help",         no_argument,        NULL, 'h' },
      { "jsoncfg",      required_argument,  NULL, 'j' },
      { "originhost",   required_argument,  NULL, 's' },
      { "originrealm",  required_argument,  NULL, 'r' },
      { "hsshost",      required_argument,  NULL, 'a' },
      { "hssrealm",     required_argument,  NULL, 'b' },
      { "logsize",      required_argument,  NULL, 'z' },
      { "lognumber",    required_argument,  NULL, 'n' },
      { "logname",      required_argument,  NULL, 'l' },
      { "logqsize",     required_argument,  NULL, 'q' },
      { "fdcfg",        required_argument,  NULL, 'c' },
      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "j:hs:r:a:b:c:z:n:d:l:", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'h': { help(); exit(0); }
         case 'j': { m_jsoncfg = optarg;              m_options |= opt_jsoncfg; break; }
         case 's': { m_originhost = optarg;           m_options |= opt_originhost; break; }
         case 'r': { m_originrealm = optarg;          m_options |= opt_originrealm; break; }
         case 'a': { m_hsshost = optarg;              m_options |= opt_hsshost; break; }
         case 'b': { m_hssrealm = optarg;             m_options |= opt_hssrealm; break; }
         case 'c': { m_fdcfg = optarg;                m_options |= opt_fdcfg; break; }
         case 'z': { m_logmaxsize = atoi(optarg);     m_options |= opt_logmaxsize; break; }
         case 'n': { m_lognbrfiles = atoi(optarg);    m_options |= opt_lognbrfiles; break; }
         case 'l': { m_logfilename = optarg;          m_options |= opt_logfilename; break; }
         case '1': { m_logqueuesize = atoi(optarg);   m_options |= opt_logqueuesize; break; }
         case '?':
         {
            switch ( optopt )
            {
               case 'j': { std::cout << "Option -j (json config) requires an argument" << std::endl; break; }
               case 's': { std::cout << "Option -s (origin host) requires an argument" << std::endl; break; }
               case 'r': { std::cout << "Option -r (origin realm) requires an argument" << std::endl; break; }
               case 'a': { std::cout << "Option -a (HSS host) requires an argument" << std::endl; break; }
               case 'b': { std::cout << "Option -b (HSS realm) requires an argument" << std::endl; break; }
               case 'c': { std::cout << "Option -c (diameter configuration file) requires an argument" << std::endl; break; }
               case 'z': { std::cout << "Option -z (log max size) requires an argument" << std::endl; break; }
               case 'n': { std::cout << "Option -n (log number of files) requires an argument" << std::endl; break; }
               case 'l': { std::cout << "Option -l (log filename) requires an argument" << std::endl; break; }
               case 'q': { std::cout << "Option -q (log queue size) requires an argument" << std::endl; break; }
               default: { std::cout << "Unrecognized option [" << c << "]" << std::endl; break; }
            }
            result = false;
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

bool Options::parseJson()
{
   char buf[2048];

   FILE *fp = fopen ( m_jsoncfg.c_str() , "r");
   RAPIDJSON_NAMESPACE::FileReadStream is( fp, buf, sizeof(buf) );
   RAPIDJSON_NAMESPACE::Document doc;
   doc.ParseStream<0>( is );
   fclose( fp );

   if(!doc.IsObject()) {
      std::cout << "Error parsing the json config file [" << m_jsoncfg << "]" << std::endl;
      return false;
   }

   if(doc.HasMember("common")){
      const RAPIDJSON_NAMESPACE::Value& commonSection = doc["common"];
      if(!(m_options & opt_fdcfg) && commonSection.HasMember("fdcfg")){
         if(!commonSection["fdcfg"].IsString()){ std::cout << "Error parsing json value: [fdcfg]" << std::endl; return false; }
         m_fdcfg = commonSection["fdcfg"].GetString();
         m_options |= opt_fdcfg;
      }
      if(!(m_options & opt_originhost) && commonSection.HasMember("originhost")){
         if(!commonSection["originhost"].IsString()) { std::cout << "Error parsing json value: [originhost]" << std::endl; return false; }
         m_originhost = commonSection["originhost"].GetString();
         m_options |= opt_originhost;
      }
      if(!(m_options & opt_originrealm) && commonSection.HasMember("originrealm")){
         if(!commonSection["originrealm"].IsString()) { std::cout << "Error parsing json value: [originrealm]" << std::endl; return false; }
         m_originrealm = commonSection["originrealm"].GetString();
         m_options |= opt_originrealm;
      }
   }
   if(doc.HasMember("smsrouter")){
      const RAPIDJSON_NAMESPACE::Value& smsSection = doc["smsrouter"];
      if(!(m_options & opt_hsshost) && smsSection.HasMember("hsshost")){
         if(!smsSection["hsshost"].IsString()) { std::cout << "Error parsing json value: [hsshost]" << std::endl; return false; }
         m_hsshost = smsSection["hsshost"].GetString();
         m_options |= opt_hsshost;
      }
      if(!(m_options & opt_hssrealm) && smsSection.HasMember("hssrealm")){
         if(!smsSection["hssrealm"].IsString()) { std::cout << "Error parsing json value: [hssrealm]" << std::endl; return false; }
         m_hssrealm = smsSection["hssrealm"].GetString();
         m_options |= opt_hssrealm;
      }
      if(!(m_options & opt_logmaxsize) && smsSection.HasMember("logsize")){
         if(!smsSection["logsize"].IsInt()) { std::cout << "Error parsing json value: [logsize]" << std::endl; return false; }
         m_logmaxsize = smsSection["logsize"].GetInt();
         m_options |= opt_logmaxsize;
      }
      if(!(m_options & opt_lognbrfiles) && smsSection.HasMember("lognumber")){
         if(!smsSection["lognumber"].IsInt()) { std::cout << "Error parsing json value: [lognumber]" << std::endl; return false; }
         m_lognbrfiles = smsSection["lognumber"].GetInt();
         m_options |= opt_lognbrfiles;
      }
      if(!(m_options & opt_logfilename) && smsSection.HasMember("logname")){
         if(!smsSection["logname"].IsString()) { std::cout << "Error parsing json value: [logname]" << std::endl; return false; }
         m_logfilename = smsSection["logname"].GetString();
         m_options |= opt_logfilename;
      }
      if(!(m_options & opt_logqueuesize) && smsSection.HasMember("logqsize")){
         if(!smsSection["logqsize"].IsInt()) { std::cout << "Error parsing json value: [logqsize]" << std::endl; return false; }
         m_logqueuesize = smsSection["logqsize"].GetInt();
         m_options |= opt_logqueuesize;
      }
   }

   return true;
}

bool Options::validateOptions(){
   return
         ( m_options & opt_jsoncfg )
      && ( m_options & opt_originhost )
      && ( m_options & opt_originrealm )
      && ( m_options & opt_fdcfg )
      && ( m_options & opt_hsshost )
      && ( m_options & opt_hssrealm )
      && ( m_options & opt_logmaxsize )
      && ( m_options & opt_lognbrfiles )
      && ( m_options & opt_logfilename )
      ;
}

