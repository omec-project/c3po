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

#include "options.h"

#define RAPIDJSON_NAMESPACE pcrfrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

Options *Options::m_singleton = NULL;

Options::Options()
   : m_selfredirect( false ),
     m_enableruletimers( false ),
     m_rulecallbackport( 9081 ),
     m_rulecallbackthreads( 1 ),
     m_options( 0 ),
     m_logmaxsize( 0 ),
     m_lognbrfiles( 0 ),
     m_logqueuesize( 0 ),
     m_statlogmaxsize ( 0 ),
     m_statlognbrfiles ( 0 ),
     m_statsfrequency( 0 ),
     m_auditlogmaxsize ( 0 ),
     m_auditlognbrfiles ( 0 ),
     m_ossport ( 0 )
{
}

Options::~Options()
{
}

void Options::help()
{
   std::cout << std::endl
             << "Usage:  pcrf -t casshost [OPTIONS]..." << std::endl
             << "  -b, --rulecallbackthreads n  The number of threads to process Chronos \"pop\" notifications." << std::endl
             << "  -c, --fdcfg filename         Read the freeDiameter configuration from this file" << std::endl
             << "                               instead of the default location (" DEFAULT_CONF_PATH "/" FD_DEFAULT_CONF_FILENAME ")." << std::endl
             << "  -d, --selfredirect           If present, enables the self-redirect feature." << std::endl
             << "  -e, --enableruletimers       If present, enables rule timers (if specified in the rule definition)." << std::endl
             << "  -f, --rulecallbackurl url    The URL used by Chronos to send \"pop\" notifications." << std::endl
             << "  -h, --help                   Print help and exit" << std::endl
             << "  -j, --jsoncfg filename       The JSON configuration file." << std::endl
             << "  -k, --casskeyspace keyspace  The Cassandra keyspace." << std::endl
             << "  -l, --logname filename       The base filename for the log files." << std::endl
             << "  -n, --lognumber number       The number of log files to maintain." << std::endl
             << "  -p, --rulecallbackport port  The IP port to receive Chronos \"pop\" notifications when rule timers expire." << std::endl
             << "  -q, --logqsize size          The log queue size in bytes, must be a power of 2." << std::endl
             << "  -r, --originrealm realm      The diameter destination realm." << std::endl
             << "  -s, --originhost host        The diameter destination host." << std::endl
             << "  -t, --casshost host          The Cassandra host IP address." << std::endl
             << "  -z, --logsize size           The maximum log size in MB." << std::endl
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
      { "rulecallbackthreads",   required_argument,  NULL, 'b' },
      { "fdcfg",                 required_argument,  NULL, 'c' },
      { "selfredirect",          no_argument,        NULL, 'd' },
      { "enableruletimers",      no_argument,        NULL, 'e' },
      { "rulecallbackurl",       required_argument,  NULL, 'f' },
      { "chronosapiurl",         required_argument,  NULL, 'g' },
      { "help",                  no_argument,        NULL, 'h' },
      { "jsoncfg",               required_argument,  NULL, 'j' },
      { "casskeyspace",          required_argument,  NULL, 'k' },
      { "logname",               required_argument,  NULL, 'l' },
      { "lognumber",             required_argument,  NULL, 'n' },
      { "rulecallbackport",      required_argument,  NULL, 'p' },
      { "logqsize",              required_argument,  NULL, 'q' },
      { "originrealm",           required_argument,  NULL, 'r' },
      { "originhost",            required_argument,  NULL, 's' },
      { "casshost",              required_argument,  NULL, 't' },
      { "logsize",               required_argument,  NULL, 'z' },
      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "b:c:def:g:hj:k:l:n:p:q:r:s:t:z:", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'b': { m_rulecallbackthreads = atoi(optarg);  m_options |= opt_rulecallbackthreads; break; }
         case 'c': { m_fdcfg = optarg;                      m_options |= opt_fdcfg; break; }
         case 'd': { m_selfredirect = true;                 m_options |= opt_selfredirect; break; }
         case 'e': { m_enableruletimers = true;             m_options |= opt_enableruletimers; break; }
         case 'f': { m_rulecallbackurl = optarg;            m_options |= opt_rulecallbackurl; break; }
         case 'g': { m_chronosapiurl = optarg;              m_options |= opt_chronosapiurl; break; }
         case 'h': { help(); exit(0); }
         case 'j': { m_jsoncfg = optarg;                    m_options |= opt_jsoncfg; break; }
         case 'k': { m_cassKeyspace = optarg;               m_options |= opt_cassks; break; }
         case 'l': { m_logfilename = optarg;                m_options |= opt_logfilename; break; }
         case 'n': { m_lognbrfiles = atoi(optarg);          m_options |= opt_lognbrfiles; break; }
         case 'p': { m_rulecallbackport = atoi(optarg);     m_options |= opt_rulecallbackport; break; }
         case 'q': { m_logqueuesize = atoi(optarg);         m_options |= opt_logqueuesize; break; }
         case 'r': { m_originrealm = optarg;                m_options |= opt_originrealm; break; }
         case 's': { m_originhost = optarg;                 m_options |= opt_originhost; break; }
         case 't': { m_cassHost = optarg;                   m_options |= opt_casshost; break; }
         case 'z': { m_logmaxsize = atoi(optarg);           m_options |= opt_logmaxsize; break; }
         case '?':
         {
            switch ( optopt )
            {
               case 'b': { std::cout << "Option -b (rule callback threads) requires an argument" << std::endl; break; }
               case 'c': { std::cout << "Option -c (diameter configuration file) requires an argument" << std::endl; break; }
               case 'f': { std::cout << "Option -f (rule callback url) requires an argument" << std::endl; break; }
               case 'g': { std::cout << "Option -g (Chronos API url) requires an argument" << std::endl; break; }
               case 'j': { std::cout << "Option -j (json config) requires an argument" << std::endl; break; }
               case 'k': { std::cout << "Option -k (Cassandra keyspace) requires an argument" << std::endl; break; }
               case 'l': { std::cout << "Option -l (log filename) requires an argument" << std::endl; break; }
               case 'n': { std::cout << "Option -n (log number of files) requires an argument" << std::endl; break; }
               case 'p': { std::cout << "Option -p (rule callback port) requires an argument" << std::endl; break; }
               case 'q': { std::cout << "Option -q (log queue size) requires an argument" << std::endl; break; }
               case 'r': { std::cout << "Option -r (origin realm) requires an argument" << std::endl; break; }
               case 's': { std::cout << "Option -s (origin host) requires an argument" << std::endl; break; }
               case 't': { std::cout << "Option -t (Cassandra host) requires an argument" << std::endl; break; }
               case 'z': { std::cout << "Option -z (log max size) requires an argument" << std::endl; break; }
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
   if(doc.HasMember("pcrf")){
      const RAPIDJSON_NAMESPACE::Value& pcrfSection = doc["pcrf"];
      if(!(m_options & opt_casshost) && pcrfSection.HasMember("casshost")){
         if(!pcrfSection["casshost"].IsString()) { std::cout << "Error parsing json value: [casshost]" << std::endl; return false; }
         m_cassHost = pcrfSection["casshost"].GetString();
         m_options |= opt_casshost;
      }
      if(!(m_options & opt_cassks) && pcrfSection.HasMember("casskeyspace")){
         if(!pcrfSection["casskeyspace"].IsString()) { std::cout << "Error parsing json value: [casskeyspace]" << std::endl; return false; }
         m_cassKeyspace = pcrfSection["casskeyspace"].GetString();
         m_options |= opt_cassks;
      }
      if(!(m_options & opt_logmaxsize) && pcrfSection.HasMember("logsize")){
         if(!pcrfSection["logsize"].IsInt()) { std::cout << "Error parsing json value: [logsize]" << std::endl; return false; }
         m_logmaxsize = pcrfSection["logsize"].GetInt();
         m_options |= opt_logmaxsize;
      }
      if(!(m_options & opt_lognbrfiles) && pcrfSection.HasMember("lognumber")){
         if(!pcrfSection["lognumber"].IsInt()) { std::cout << "Error parsing json value: [lognumber]" << std::endl; return false; }
         m_lognbrfiles = pcrfSection["lognumber"].GetInt();
         m_options |= opt_lognbrfiles;
      }
      if(!(m_options & opt_logfilename) && pcrfSection.HasMember("logname")){
         if(!pcrfSection["logname"].IsString()) { std::cout << "Error parsing json value: [logname]" << std::endl; return false; }
         m_logfilename = pcrfSection["logname"].GetString();
         m_options |= opt_logfilename;
      }
      if(!(m_options & opt_logqueuesize) && pcrfSection.HasMember("logqsize")){
         if(!pcrfSection["logqsize"].IsInt()) { std::cout << "Error parsing json value: [logqsize]" << std::endl; return false; }
         m_logqueuesize = pcrfSection["logqsize"].GetInt();
         m_options |= opt_logqueuesize;
      }
      if(!(m_options & opt_selfredirect) && pcrfSection.HasMember("selfredirect")){
         if(!pcrfSection["selfredirect"].IsBool()) { std::cout << "Error parsing json value: [selfredirect]" << std::endl; return false; }
         m_selfredirect = pcrfSection["selfredirect"].GetBool();
         m_options |= opt_selfredirect;
      }
      if(!(m_options & opt_enableruletimers) && pcrfSection.HasMember("enableruletimers")){
         if(!pcrfSection["enableruletimers"].IsBool()) { std::cout << "Error parsing json value: [enableruletimers]" << std::endl; return false; }
         m_enableruletimers = pcrfSection["enableruletimers"].GetBool();
         m_options |= opt_enableruletimers;
      }
      if(!(m_options & opt_rulecallbackport) && pcrfSection.HasMember("rulecallbackport")){
         if(!pcrfSection["rulecallbackport"].IsInt()) { std::cout << "Error parsing json value: [rulecallbackport]" << std::endl; return false; }
         m_rulecallbackport = pcrfSection["rulecallbackport"].GetInt();
         m_options |= opt_rulecallbackport;
      }
      if(!(m_options & opt_rulecallbackthreads) && pcrfSection.HasMember("rulecallbackthreads")){
         if(!pcrfSection["rulecallbackthreads"].IsInt()) { std::cout << "Error parsing json value: [rulecallbackport]" << std::endl; return false; }
         m_rulecallbackport = pcrfSection["rulecallbackthreads"].GetInt();
         m_options |= opt_rulecallbackthreads;
      }
      if(!(m_options & opt_rulecallbackurl) && pcrfSection.HasMember("rulecallbackurl")){
         if(!pcrfSection["rulecallbackurl"].IsString()) { std::cout << "Error parsing json value: [rulecallbackport]" << std::endl; return false; }
         m_rulecallbackurl = pcrfSection["rulecallbackurl"].GetString();
         m_options |= opt_rulecallbackurl;
      }
      if(!(m_options & opt_chronosapiurl) && pcrfSection.HasMember("chronosapiurl")){
         if(!pcrfSection["chronosapiurl"].IsString()) { std::cout << "Error parsing json value: [rulecallbackport]" << std::endl; return false; }
         m_chronosapiurl= pcrfSection["chronosapiurl"].GetString();
         m_options |= opt_chronosapiurl;
      }

      if(!(m_options & opt_statlogmaxsize) && pcrfSection.HasMember("statlogsize")){
         if(!pcrfSection["statlogsize"].IsInt()) { std::cout << "Error parsing json value: [statlogsize]" << std::endl; return false; }
         m_statlogmaxsize = pcrfSection["statlogsize"].GetInt();
         m_options |= opt_statlogmaxsize;
      }
      if(!(m_options & opt_statlognbrfiles) && pcrfSection.HasMember("statlognumber")){
         if(!pcrfSection["statlognumber"].IsInt()) { std::cout << "Error parsing json value: [statlognumber]" << std::endl; return false; }
         m_statlognbrfiles = pcrfSection["statlognumber"].GetInt();
         m_options |= opt_statlognbrfiles;
      }
      if(!(m_options & opt_statlogfilename) && pcrfSection.HasMember("statlogname")){
         if(!pcrfSection["statlogname"].IsString()) { std::cout << "Error parsing json value: [statlogname]" << std::endl; return false; }
         m_statlogfilename = pcrfSection["statlogname"].GetString();
         m_options |= opt_statlogfilename;
      }
      if(!(m_options & opt_statfrequency) && pcrfSection.HasMember("statfreq")){
         if(!pcrfSection["statfreq"].IsInt64()) { std::cout << "Error parsing json value: [statfreq]" << std::endl; return false; }
         m_statsfrequency = pcrfSection["statfreq"].GetInt64();
         m_options |= opt_statfrequency;
      }

      if(!(m_options & opt_auditlogmaxsize) && pcrfSection.HasMember("auditlogsize")){
         if(!pcrfSection["auditlogsize"].IsInt()) { std::cout << "Error parsing json value: [auditlogsize]" << std::endl; return false; }
         m_auditlogmaxsize = pcrfSection["auditlogsize"].GetInt();
         m_options |= opt_auditlogmaxsize;
      }
      if(!(m_options & opt_auditlognbrfiles) && pcrfSection.HasMember("auditlognumber")){
         if(!pcrfSection["auditlognumber"].IsInt()) { std::cout << "Error parsing json value: [auditlognumber]" << std::endl; return false; }
         m_auditlognbrfiles = pcrfSection["auditlognumber"].GetInt();
         m_options |= opt_auditlognbrfiles;
      }
      if(!(m_options & opt_auditlogfilename) && pcrfSection.HasMember("auditlogname")){
         if(!pcrfSection["auditlogname"].IsString()) { std::cout << "Error parsing json value: [auditlogname]" << std::endl; return false; }
         m_auditlogfilename = pcrfSection["auditlogname"].GetString();
         m_options |= opt_auditlogfilename;
      }
      if(!(m_options & opt_ossport) && pcrfSection.HasMember("ossport")){
         if(!pcrfSection["ossport"].IsInt()) { std::cout << "Error parsing json value: [ossport]" << std::endl; return false; }
         m_ossport = pcrfSection["ossport"].GetInt();
         m_options |= opt_ossport;
      }
      if(!(m_options & opt_ossfile) && pcrfSection.HasMember("ossfile")){
         if(!pcrfSection["ossfile"].IsString()) { std::cout << "Error parsing json value: [ossfile]" << std::endl; return false; }
         m_ossfile = pcrfSection["ossfile"].GetString();
         m_options |= opt_ossfile;
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
      && ( m_options & opt_casshost )
      && ( m_options & opt_cassks )
      && ( m_options & opt_logmaxsize )
      && ( m_options & opt_lognbrfiles )
      && ( m_options & opt_logfilename )
      && ( m_options & opt_selfredirect )
      && ( m_options & opt_enableruletimers )
      && ( m_options & opt_rulecallbackport )
      && ( m_options & opt_rulecallbackthreads )
      && ( m_options & opt_rulecallbackurl )
      && ( m_options & opt_chronosapiurl )
      && ( m_options & opt_statlogfilename )
      && ( m_options & opt_statlognbrfiles )
      && ( m_options & opt_statlogmaxsize )
      && ( m_options & opt_statfrequency )
      && ( m_options & opt_auditlogfilename )
      && ( m_options & opt_auditlognbrfiles )
      && ( m_options & opt_auditlogmaxsize )
      && ( m_options & opt_ossport )
      && ( m_options & opt_ossfile );
}

