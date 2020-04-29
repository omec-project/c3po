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

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include "options.h"

const int Options::JSONFILEBUFFER = 1024;

long int Options::options;

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
std::string Options::m_dataprefix;
std::string Options::m_datapath;
std::string Options::m_archivepath;
std::string Options::m_trackingpath;
std::string Options::m_currentextension;
std::string Options::m_trackingextension;
std::string Options::m_diameterconfiguration;
std::string Options::m_memcachedserver;
int Options::m_memcachedport;
std::string Options::m_originhost;
std::string Options::m_originrealm;
uint32_t Options::m_skiprows;
long Options::m_idledur;
uint32_t Options::m_maxacrs;
int Options::m_acasavefrequency;
uint32_t Options::m_statsfrequency;
int         Options::m_ossport;
std::string Options::m_ossfile;


std::string Options::m_sgxserverip;
int Options::m_sgxserverport;
std::string Options::m_certfile;
std::string Options::m_privkeyfile;
std::string Options::m_mrsigner;
std::string Options::m_mrenclave;
int Options::m_isvsvn;
std::string Options::m_streamname;

void Options::help()
{
   std::cout << std::endl
             << "Usage:  c3poctf -d path -a path -t path -p prefix [OPTIONS]..." << std::endl
             << "  -h, --help                   Print help and exit" << std::endl
             << "  -i, --idledur millisecond    Number of milliseconds the system goes idle waiting for csv input." << std::endl
             << "  -j, --jsoncfg filename       Read the application configuration from this file." << std::endl
             << "  -f, --fdcfg filename         Read the freeDiameter configuration from this file" << std::endl
             << "                               instead of the default location (" DEFAULT_CONF_PATH "/" FD_DEFAULT_CONF_FILENAME ")." << std::endl
             << "  -p, --datapfx prefix         The prefix of the DPN CSV file." << std::endl
             << "  -d, --datapath path          The system path to the DPN CSV files." << std::endl
             << "  -a, --archivepath path       The system path where DPN CSV files will be archived." << std::endl
             << "  -t, --trackpath path         The system path where CSV file tracking info is stored." << std::endl
             << "  -e, --trackext ext           The file extension for tracking files." << std::endl
             << "  -c, --curext ext             The file extension that identifies the \"current\" CSV file." << std::endl
             << "  -s, --skiprows rows          The number of rows to skip at the beginning of a CSV file." << std::endl
             << "  -m, --maxacrs max            The maximum number of unanswered ACR's to allow." << std::endl
             << "  -q, --savefreq count         The number of ACA's to process before saving the tracking information." << std::endl
             << "  -n, --sdnmemcsvr             The memcached server where the SDN data is located." << std::endl
             << "  -N, --sdnmemcport            The memcached server port where the SDN data is located." << std::endl
             << "  -x, --originhost host        The diameter destination host." << std::endl
             << "  -y, --originrealm realm      The diameter destination realm." << std::endl
             << "  -u, --statsfreq milliseconds Number of milliseconds the system writes stats on the syslog ." << std::endl
#ifdef SGX_CDR
             << "  -g, --sgxserverip            The SGX server IP." << std::endl
             << "  -G, --sgxserverport          The SGX server Port." << std::endl
             << "  -b, --privkeyfile            The Private key file." << std::endl
             << "  -k, --certfile               The certificate file." << std::endl
             << "  -l, --mrsigner               The mrsigner of enclave." << std::endl
             << "  -o, --mrenclave              The mrenclave of enclave." << std::endl
             << "  -v, --isvsvn                 The isvsvn." << std::endl
             << "  -z, --streamname             The Stream name to be read." << std::endl

#endif

             << "  -r, --runtests           Run the system tests." << std::endl;

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
      if(!(options & diameterconfiguration) && commonSection.HasMember("fdcfg")){
         if(!commonSection["fdcfg"].IsString()){ std::cout << "Error parsing json value: [fdcfg]" << std::endl; return false; }
         m_diameterconfiguration = commonSection["fdcfg"].GetString();
         options |= diameterconfiguration;
      }
      if(!(options & originhost) && commonSection.HasMember("originhost")){
         if(!commonSection["originhost"].IsString()) { std::cout << "Error parsing json value: [originhost]" << std::endl; return false; }
         m_originhost = commonSection["originhost"].GetString();
         options |= originhost;
      }
      if(!(options & originrealm) && commonSection.HasMember("originrealm")){
         if(!commonSection["originrealm"].IsString()) { std::cout << "Error parsing json value: [originrealm]" << std::endl; return false; }
         m_originrealm = commonSection["originrealm"].GetString();
         options |= originrealm;
      }
   }
   if(doc.HasMember("ctf")){
      const rapidjson::Value& ctfSection = doc["ctf"];
      if(!(options & dataprefix) && ctfSection.HasMember("datapfx")){
         if(!ctfSection["datapfx"].IsString()) { std::cout << "Error parsing json value: [datapfx]" << std::endl; return false; }
         m_dataprefix = ctfSection["datapfx"].GetString();
         options |= dataprefix;
      }
      if(!(options & datapath) && ctfSection.HasMember("datapath")){
         if(!ctfSection["datapath"].IsString()) { std::cout << "Error parsing json value: [datapath]" << std::endl; return false; }
         m_datapath = ctfSection["datapath"].GetString();
         options |= datapath;
      }
      if(!(options & archivepath) && ctfSection.HasMember("archivepath")){
         if(!ctfSection["archivepath"].IsString()) { std::cout << "Error parsing json value: [archivepath]" << std::endl; return false; }
         m_archivepath = ctfSection["archivepath"].GetString();
         options |= archivepath;
      }
      if(!(options & trackingpath) && ctfSection.HasMember("trackpath")){
         if(!ctfSection["trackpath"].IsString()) { std::cout << "Error parsing json value: [trackpath]" << std::endl; return false; }
         m_trackingpath = ctfSection["trackpath"].GetString();
         options |= trackingpath;
      }
      if(!(options & trackingextension) && ctfSection.HasMember("trackext")){
         if(!ctfSection["trackext"].IsString()) { std::cout << "Error parsing json value: [trackext]" << std::endl; return false; }
         m_trackingextension = ctfSection["trackext"].GetString();
         options |= trackingextension;
      }
      if(!(options & currentextension) && ctfSection.HasMember("curext")){
         if(!ctfSection["curext"].IsString()) { std::cout << "Error parsing json value: [curext]" << std::endl; return false; }
         m_currentextension = ctfSection["curext"].GetString();
         options |= currentextension;
      }
      if(!(options & skiprows) && ctfSection.HasMember("skiprows")){
         if(!ctfSection["skiprows"].IsUint()) { std::cout << "Error parsing json value: [skiprows]" << std::endl; return false; }
         m_skiprows = ctfSection["skiprows"].GetUint();
         options |= skiprows;
      }
      if(!(options & idledur) && ctfSection.HasMember("idledur")){
         if(!ctfSection["idledur"].IsUint()) { std::cout << "Error parsing json value: [m_idledur]" << std::endl; return false; }
         m_idledur = ctfSection["idledur"].GetUint();
         options |= idledur;
      }
      if(!(options & maxacrs) && ctfSection.HasMember("maxacrs")){
         if(!ctfSection["trackpath"].IsString()) { std::cout << "Error parsing json value: [trackpath]" << std::endl; return false; }
         m_maxacrs = ctfSection["maxacrs"].GetUint();
         options |= maxacrs;
      }
      if(!(options & acasavefrequency) && ctfSection.HasMember("savefreq")){
         if(!ctfSection["savefreq"].IsInt()) { std::cout << "Error parsing json value: [savefreq]" << std::endl; return false; }
         m_acasavefrequency = ctfSection["savefreq"].GetInt();
         options |= acasavefrequency;
      }
      if(!(options & memcachedserver) && ctfSection.HasMember("sdnmemcsvr")){
         if(!ctfSection["sdnmemcsvr"].IsString()) { std::cout << "Error parsing json value: [sdnmemcsvr]" << std::endl; return false; }
         m_memcachedserver = ctfSection["sdnmemcsvr"].GetString();
         options |= memcachedserver;
      }
      if(!(options & memcachedport) && ctfSection.HasMember("sdnmemcport")){
         if(!ctfSection["sdnmemcport"].IsInt()) { std::cout << "Error parsing json value: [sdnmemcport]" << std::endl; return false; }
         m_memcachedport = ctfSection["sdnmemcport"].GetInt();
         options |= memcachedport;
      }

      if(!(options & logmaxsize) && ctfSection.HasMember("logsize")){
         if(!ctfSection["logsize"].IsInt()) { std::cout << "Error parsing json value: [logsize]" << std::endl; return false; }
         m_logmaxsize = ctfSection["logsize"].GetInt();
         options |= logmaxsize;
      }
      if(!(options & lognbrfiles) && ctfSection.HasMember("lognumber")){
         if(!ctfSection["lognumber"].IsInt()) { std::cout << "Error parsing json value: [lognumber]" << std::endl; return false; }
         m_lognbrfiles = ctfSection["lognumber"].GetInt();
         options |= lognbrfiles;
      }
      if(!(options & logfilename) && ctfSection.HasMember("logname")){
         if(!ctfSection["logname"].IsString()) { std::cout << "Error parsing json value: [logname]" << std::endl; return false; }
         m_logfilename = ctfSection["logname"].GetString();
         options |= logfilename;
      }
      if(!(options & logqueuesize) && ctfSection.HasMember("logqsize")){
         if(!ctfSection["logqsize"].IsInt()) { std::cout << "Error parsing json value: [logqsize]" << std::endl; return false; }
         m_logqueuesize = ctfSection["logqsize"].GetInt();
         options |= logqueuesize;
      }
      if(!(options & statlogmaxsize) && ctfSection.HasMember("statlogsize")){
         if(!ctfSection["statlogsize"].IsInt()) { std::cout << "Error parsing json value: [statlogsize]" << std::endl; return false; }
         m_statlogmaxsize = ctfSection["statlogsize"].GetInt();
         options |= statlogmaxsize;
      }
      if(!(options & statlognbrfiles) && ctfSection.HasMember("statlognumber")){
         if(!ctfSection["statlognumber"].IsInt()) { std::cout << "Error parsing json value: [statlognumber]" << std::endl; return false; }
         m_statlognbrfiles = ctfSection["statlognumber"].GetInt();
         options |= statlognbrfiles;
      }
      if(!(options & statlogfilename) && ctfSection.HasMember("statlogname")){
         if(!ctfSection["statlogname"].IsString()) { std::cout << "Error parsing json value: [statlogname]" << std::endl; return false; }
         m_statlogfilename = ctfSection["statlogname"].GetString();
         options |= statlogfilename;
      }
      if(!(options & statfrequency) && ctfSection.HasMember("statfreq")){
         if(!ctfSection["statfreq"].IsInt64()) { std::cout << "Error parsing json value: [statfreq]" << std::endl; return false; }
         m_statsfrequency = ctfSection["statfreq"].GetInt64();
         options |= statfrequency;
      }

      if(!(options & auditlogmaxsize) && ctfSection.HasMember("auditlogsize")){
         if(!ctfSection["auditlogsize"].IsInt()) { std::cout << "Error parsing json value: [auditlogsize]" << std::endl; return false; }
         m_auditlogmaxsize = ctfSection["auditlogsize"].GetInt();
         options |= auditlogmaxsize;
      }
      if(!(options & auditlognbrfiles) && ctfSection.HasMember("auditlognumber")){
         if(!ctfSection["auditlognumber"].IsInt()) { std::cout << "Error parsing json value: [auditlognumber]" << std::endl; return false; }
         m_auditlognbrfiles = ctfSection["auditlognumber"].GetInt();
         options |= auditlognbrfiles;
      }
      if(!(options & auditlogfilename) && ctfSection.HasMember("auditlogname")){
         if(!ctfSection["auditlogname"].IsString()) { std::cout << "Error parsing json value: [auditlogname]" << std::endl; return false; }
         m_auditlogfilename = ctfSection["auditlogname"].GetString();
         options |= auditlogfilename;
      }
      if(!(options & ossport) && ctfSection.HasMember("ossport")){
         if(!ctfSection["ossport"].IsInt()) { std::cout << "Error parsing json value: [ossport]" << std::endl; return false; }
         m_ossport = ctfSection["ossport"].GetInt();
         options |= ossport;
      }
      if(!(options & ossfile) && ctfSection.HasMember("ossfile")){
         if(!ctfSection["ossfile"].IsString()) { std::cout << "Error parsing json value: [ossfile]" << std::endl; return false; }
         m_ossfile = ctfSection["ossfile"].GetString();
         options |= ossfile;
      }

#ifdef SGX_CDR
      if(!(options & sgxserverip) && ctfSection.HasMember("sgxserverip")){
         if(!ctfSection["sgxserverip"].IsString()) { std::cout << "Error parsing json value: [sgxserverip]" << std::endl; return false; }
         m_sgxserverip = ctfSection["sgxserverip"].GetString();
         options |= sgxserverip;
      }
      if(!(options & sgxserverport) && ctfSection.HasMember("sgxserverport")){
         if(!ctfSection["sgxserverport"].IsUint()) { std::cout << "Error parsing json value: [sgxserverport]" << std::endl; return false; }
         m_sgxserverport = ctfSection["sgxserverport"].GetUint();
         options |= sgxserverport;
      }
      if(!(options & privkeyfile) && ctfSection.HasMember("privkeyfile")){
         if(!ctfSection["privkeyfile"].IsString()) { std::cout << "Error parsing json value: [privkeyfile]" << std::endl; return false; }
         m_privkeyfile = ctfSection["privkeyfile"].GetString();
         options |= privkeyfile;
      }
      if(!(options & certfile) && ctfSection.HasMember("certfile")){
         if(!ctfSection["certfile"].IsString()) { std::cout << "Error parsing json value: [certfile]" << std::endl; return false; }
         m_certfile = ctfSection["certfile"].GetString();
         options |= certfile;
      }
      if(!(options & mrsigner) && ctfSection.HasMember("mrsigner")){
         if(!ctfSection["mrsigner"].IsString()) { std::cout << "Error parsing json value: [mrsigner]" << std::endl; return false; }
         m_mrsigner = ctfSection["mrsigner"].GetString();
         options |= mrsigner;
      }
      if(!(options & mrenclave) && ctfSection.HasMember("mrenclave")){
         if(!ctfSection["mrenclave"].IsString()) { std::cout << "Error parsing json value: [mrenclave]" << std::endl; return false; }
         m_mrenclave = ctfSection["mrenclave"].GetString();
         options |= mrenclave;
      }
      if(!(options & isvsvn) && ctfSection.HasMember("isvsvn")){
         if(!ctfSection["isvsvn"].IsUint()) { std::cout << "Error parsing json value: [isvsvn]" << std::endl; return false; }
         m_isvsvn = ctfSection["isvsvn"].GetUint();
         options |= isvsvn;
      }
      if(!(options & streamname) && ctfSection.HasMember("streamname")){
         if(!ctfSection["streamname"].IsString()) { std::cout << "Error parsing json value: [streamname]" << std::endl; return false; }
         m_streamname = ctfSection["streamname"].GetString();
         options |= streamname;
      }
#endif

   }
   return true;
}

bool Options::validateOptions(){
   if(options & runtests){
      return true;
   }

   return (
            (options & dataprefix)
         && (options & datapath)
         && (options & archivepath)
         && (options & trackingpath)
         && (options & currentextension)
         && (options & trackingextension)
         && (options & diameterconfiguration)
         && (options & memcachedserver)
         && (options & memcachedport)
         && (options & originhost)
         && (options & originrealm)
         && (options & skiprows)
         && (options & idledur)
         && (options & maxacrs)
         && (options & acasavefrequency)
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
         && (options & ossport)
         && (options & ossfile)
#ifdef SGX_CDR
         && (options & sgxserverip)
         && (options & sgxserverport)
         && (options & privkeyfile)
         && (options & certfile)
         && (options & streamname)
#endif
         );
}

bool Options::parseInputOptions( int argc, char **argv )
{
   int c;
   int option_index = 0;
   bool result = true;

   struct option long_options[] = {
      { "help",         no_argument,        NULL, 'h' },
      { "idledur",      required_argument,  NULL, 'i' },
      { "jsoncfg",      required_argument,  NULL, 'j' },
      { "fdcfg",        required_argument,  NULL, 'f' },
      { "datapfx",      required_argument,  NULL, 'p' },
      { "datapath",     required_argument,  NULL, 'd' },
      { "archivepath",  required_argument,  NULL, 'a' },
      { "trackpath",    required_argument,  NULL, 't' },
      { "trackext",     required_argument,  NULL, 'e' },
      { "curext",       required_argument,  NULL, 'c' },
      { "skiprows",     required_argument,  NULL, 's' },
      { "maxacrs",      required_argument,  NULL, 'm' },
      { "savefreq",     required_argument,  NULL, 'q' },
      { "sdnmemcsvr",   required_argument,  NULL, 'n' },
      { "sdnmemcport",  required_argument,  NULL, 'N' },
      { "runtests",     no_argument,        NULL, 'r' },
      { "originhost",   required_argument,  NULL, 'x' },
      { "originrealm",  required_argument,  NULL, 'y' },
      { "statsfreq",    required_argument,  NULL, 'u' },
#ifdef SGX_CDR
      { "sgxserverip",  required_argument,  NULL, 'g' },
      { "sgxserverport",required_argument,  NULL, 'G' },
      { "privkeyfile",  required_argument,  NULL, 'b' },
      { "certfile",     required_argument,  NULL, 'k' },
      { "mrsigner",     required_argument,  NULL, 'l' },
      { "mrenclave",    required_argument,  NULL, 'o' },
      { "isvsvn",       required_argument,  NULL, 'v' },
      { "streamname",   required_argument,  NULL, 'z' },
#endif
      { NULL,0,NULL,0 }
   };

   // Loop on arguments
   while (1)
   {
      c = getopt_long(argc, argv, "hi:j:p:d:a:t:e:c:s:m:f:q:n:N:x:y:u:r:g:G:b:k:l:o:z:v", long_options, &option_index );
      if (c == -1)
         break;	// Exit from the loop.

      switch (c)
      {
         case 'h': { help(); exit(0);                                                              break; }
         case 'i': { m_idledur = atol( optarg );               options |= idledur;                 break; }
         case 'j': { m_jsoncfg = optarg;                       options |= jsoncfg;                 break; }
         case 'p': { m_dataprefix = optarg;                    options |= dataprefix;              break; }
         case 'd': { m_datapath = optarg;                      options |= datapath;                break; }
         case 'a': { m_archivepath = optarg;                   options |= archivepath;             break; }
         case 't': { m_trackingpath = optarg;                  options |= trackingpath;            break; }
         case 'e': { m_trackingextension = optarg;             options |= trackingextension;       break; }
         case 'c': { m_currentextension = optarg;              options |= currentextension;        break; }
         case 's': { m_skiprows = strtoul( optarg, NULL, 10 ); options |= skiprows;                break; }
         case 'm': { m_maxacrs = strtoul( optarg, NULL, 10 );  options |= maxacrs;                 break; }
         case 'n': { m_memcachedserver = optarg;               options |= memcachedserver;         break; }
         case 'N': { m_memcachedport = atoi( optarg );         options |= memcachedport;           break; }
         case 'f': { m_diameterconfiguration = optarg;         options |= diameterconfiguration;   break; }
         case 'q': { m_acasavefrequency = atoi( optarg );      options |= acasavefrequency;        break; }
         case 'x': { m_originhost = optarg;                    options |= originhost;              break; }
         case 'y': { m_originrealm = optarg;                   options |= originrealm;             break; }
         case 'u': { m_statsfrequency = atoi(optarg);          options |= statsfrequency;          break; }
         case 'r': {                                           options |= runtests;                break; }
#ifdef SGX_CDR
         case 'g': { m_sgxserverip = optarg;                   options |= sgxserverip;             break; }
         case 'G': { m_sgxserverport = atoi(optarg);           options |= sgxserverport;           break; }
         case 'b': { m_privkeyfile = optarg;                   options |= privkeyfile;             break; }
         case 'k': { m_certfile = optarg;                      options |= certfile;                break; }
         case 'l': { m_mrsigner = optarg;                      options |= mrsigner;                break; }
         case 'o': { m_mrenclave = optarg;                     options |= mrenclave;               break; }
         case 'v': { m_isvsvn = atoi(optarg);                  options |= isvsvn;                  break; }
         case 'z': { m_streamname = optarg;                    options |= streamname;              break; }
#endif
         case '?':
         {
            switch ( optopt )
            {
               case 'i': { std::cout << "Option -i (idle duration) requires an argument"           << std::endl; break; }
               case 'j': { std::cout << "Option -j (config json file) requires an argument"        << std::endl; break; }
               case 'p': { std::cout << "Option -p (data file prefix) requires an argument"        << std::endl; break; }
               case 'd': { std::cout << "Option -a (data path) requires an argument"               << std::endl; break; }
               case 'a': { std::cout << "Option -a (archive path) requires an argument"            << std::endl; break; }
               case 't': { std::cout << "Option -t (tracking path) requires an argument"           << std::endl; break; }
               case 'e': { std::cout << "Option -e (tracking extension) requires an argument"      << std::endl; break; }
               case 'c': { std::cout << "Option -c (current extension) requires an argument"       << std::endl; break; }
               case 's': { std::cout << "Option -s (skip rows) requires an argument"               << std::endl; break; }
               case 'm': { std::cout << "Option -m (max outstanding ACR's) requires an argument"   << std::endl; break; }
               case 'n': { std::cout << "Option -n (memcached server) requires an argument"        << std::endl; break; }
               case 'N': { std::cout << "Option -N (memcached server port) requires an argument"   << std::endl; break; }
               case 'f': { std::cout << "Option -f (diameter config) requires an argument"         << std::endl; break; }
               case 'q': { std::cout << "Option -f (ACA save frequency) requires an argument"      << std::endl; break; }
               case 'x': { std::cout << "Option -x (origin host) requires an argument"             << std::endl; break; }
               case 'y': { std::cout << "Option -y (origin realm) requires an argument"            << std::endl; break; }
               case 'u': { std::cout << "Option -u (stats frequency) requires an argument"         << std::endl; break; }
#ifdef SGX_CDR
               case 'g': { std::cout << "Option -g (sgx server IP) requires an argument"           << std::endl; break; }
               case 'G': { std::cout << "Option -G (sgx server Port) requires an argument"         << std::endl; break; }
               case 'b': { std::cout << "Option -b (private key file) requires an argument"        << std::endl; break; }
               case 'k': { std::cout << "Option -k (certificate file) requires an argument"        << std::endl; break; }
               case 'l': { std::cout << "Option -l (mrsigner of enclave) requires an argument"     << std::endl; break; }
               case 'o': { std::cout << "Option -o (mrenclave of enclave) requires an argument"    << std::endl; break; }
               case 'v': { std::cout << "Option -r (isvsvn) requires an argument"                  << std::endl; break; }
               case 'z': { std::cout << "Option -z (streamname) requires an argument"              << std::endl; break; }
#endif
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

