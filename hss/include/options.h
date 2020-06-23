/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <stdint.h>
#include <string>

extern "C" {
   #include "hss_config.h"
}


class Options
{
public:

   static bool parse( int argc, char **argv );
   static bool parseInputOptions( int argc, char **argv );
   static bool parseJson();
   static bool validateOptions();

   static const int logMaxSize()                         { return m_logmaxsize; }
   static const int logNumberFiles()                     { return m_lognbrfiles; }
   static const std::string &logFilename()               { return m_logfilename; }
   static const int logQueueSize()                       { return m_logqueuesize; }

   static const int statlogMaxSize()                     { return m_statlogmaxsize; }
   static const int statlogNumberFiles()                 { return m_statlognbrfiles; }
   static const std::string &statlogFilename()           { return m_statlogfilename; }
   static const uint32_t statsFrequency()                { return m_statsfrequency; }

   static const int auditlogMaxSize()                    { return m_auditlogmaxsize; }
   static const int auditlogNumberFiles()                { return m_auditlognbrfiles; }
   static const std::string &auditlogFilename()          { return m_auditlogfilename; }

   static const std::string &getjsonConfig()             { return m_jsoncfg; }
   static const std::string &getdiameterconfiguration()  { return m_diameterconfiguration; }
   static const std::string &getcassserver()             { return m_cassserver; }
   static const std::string &getcassuser()               { return m_cassuser; }
   static const std::string &getcasspwd()                { return m_casspwd; }
   static const std::string &getcassdb()                 { return m_cassdb; }
   static const unsigned &getcasscoreconnections()       { return m_casscoreconnections; }
   static const unsigned &getcassmaxconnections()        { return m_cassmaxconnections; }
   static const unsigned &getcassioqueuesize()           { return m_cassioqueuesize; }
   static const unsigned &getcassiothreads()             { return m_cassiothreads; }

   static bool               getrandvector()             { return m_randvector; }
   static const std::string &getoptkey()                 { return m_optkey; }
   static bool               getreloadkey()              { return m_reloadkey; }
   static bool               getonlyloadkey()            { return m_onlyloadkey; }
   static const int         &getgtwport()                { return m_gtwport; }
   static const std::string &getgtwhost()                { return m_gtwhost; }
   static const int         &getrestport()               { return m_restport; }
   static const int         &getossport()                { return m_ossport; }
   static const std::string &getossfile()                { return m_ossfile; }
   static const std::string &getsynchimsi()              { return m_synchimsi; }
   static const std::string &getsynchauts()              { return m_synchauts; }
   static const int         &getnumworkers()             { return m_numworkers; }
   static const int         &getconcurrent()             { return m_concurrent; }

   static void fillhssconfig(hss_config_t *hss_config_p);

   //static const std::string &setsynchauts(const std::string &auts) { return m_synchauts = auts; }

private:

   enum OptionsSelected {
     jsoncfg                           = 0x01,
     diameterconfiguration             = 0x02,
     cassserver                        = 0x04,
     cassuser                          = 0x08,
     casspwd                           = 0x10,
     cassdb                            = 0x20,
     randvector                        = 0x40,
     optkey                            = 0x80,
     reloadkey                         = 0x100,
     onlyloadkey                       = 0x200,
     gtwport                           = 0x400,
     gtwhost                           = 0x800,
     restport                          = 0x1000,
     logmaxsize                        = 0x2000,
     lognbrfiles                       = 0x4000,
     logfilename                       = 0x8000,
     logqueuesize                      = 0x10000,
     numworkers                        = 0x20000,
     concurrent                        = 0x40000,
     statlogfilename                   = 0x80000,
     statlognbrfiles                   = 0x100000,
     statlogmaxsize                    = 0x200000,
     statfrequency                     = 0x400000,
     auditlogfilename                  = 0x800000,
     auditlognbrfiles                  = 0x1000000,
     auditlogmaxsize                   = 0x2000000,
     ossport                           = 0x4000000,
     ossfile                           = 0x8000000
   };

   static void help();

   static const int JSONFILEBUFFER;
   static int options;

   static int m_logmaxsize;
   static int m_lognbrfiles;
   static std::string m_logfilename;
   static int m_logqueuesize;

   static int m_statlogmaxsize;
   static int m_statlognbrfiles;
   static std::string m_statlogfilename;
   static uint32_t    m_statsfrequency;

   static int m_auditlogmaxsize;
   static int m_auditlognbrfiles;
   static std::string m_auditlogfilename;

   static std::string m_jsoncfg;
   static std::string m_diameterconfiguration;

   static std::string m_cassserver;
   static std::string m_cassuser;
   static std::string m_casspwd;
   static std::string m_cassdb;
   static unsigned    m_casscoreconnections;
   static unsigned    m_cassmaxconnections;
   static unsigned    m_cassioqueuesize;
   static unsigned    m_cassiothreads;
   static bool        m_randvector;
   static std::string m_optkey;
   static bool        m_reloadkey;
   static bool        m_onlyloadkey;
   static int         m_gtwport;
   static std::string m_gtwhost;
   static int         m_restport;
   static int         m_ossport;
   static std::string m_ossfile;
   static std::string m_synchimsi;
   static std::string m_synchauts;
   static int         m_numworkers;
   static int         m_concurrent;
   static bool        m_verify_roaming; 

};

#endif // #define __OPTIONS_H
