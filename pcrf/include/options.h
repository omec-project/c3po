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

class Options
{
public:
   static bool parse( int argc, char **argv );
   static void cleanup() { delete m_singleton; }

   static const std::string &originHost()             { return singleton().m_originhost; }
   static const std::string &originRealm()            { return singleton().m_originrealm; }
   static const std::string &diameterConfiguration()  { return singleton().m_fdcfg; }
   static const std::string &cassandraHost()          { return singleton().m_cassHost; }
   static const std::string &cassandraKeyspace()      { return singleton().m_cassKeyspace; }

   static const int logMaxSize()                      { return singleton().m_logmaxsize; }
   static const int logNumberFiles()                  { return singleton().m_lognbrfiles; }
   static const std::string &logFilename()            { return singleton().m_logfilename; }
   static const int logQueueSize()                    { return singleton().m_logqueuesize; }

   static const int statlogMaxSize()                     { return singleton().m_statlogmaxsize; }
   static const int statlogNumberFiles()                 { return singleton().m_statlognbrfiles; }
   static const std::string &statlogFilename()           { return singleton().m_statlogfilename; }
   static const uint32_t statsFrequency()                { return singleton().m_statsfrequency; }

   static const int auditlogMaxSize()                    { return singleton().m_auditlogmaxsize; }
   static const int auditlogNumberFiles()                { return singleton().m_auditlognbrfiles; }
   static const std::string &auditlogFilename()          { return singleton().m_auditlogfilename; }

   static const bool selfRedirect()                   { return singleton().m_selfredirect; }

   static const bool enableRuleTimers()               { return singleton().m_enableruletimers; }
   static const int ruleCallbackPort()                { return singleton().m_rulecallbackport; }
   static const int ruleCallbackThreads()             { return singleton().m_rulecallbackthreads; }
   static const std::string &ruleCallbackUrl()        { return singleton().m_rulecallbackurl; }
   static const std::string &chronosApiUrl()          { return singleton().m_chronosapiurl; }

   static const int         &getossport()               { return singleton().m_ossport; }
   static const std::string &getossfile()               { return singleton().m_ossfile; }

private:
   enum OptionsSelected {
     opt_jsoncfg              = 0x00000001,
     opt_originhost           = 0x00000002,
     opt_originrealm          = 0x00000004,
     opt_fdcfg                = 0x00000008,
     opt_casshost             = 0x00000010,
     opt_cassks               = 0x00000020,
     opt_logmaxsize           = 0x00000040,
     opt_lognbrfiles          = 0x00000080,
     opt_logfilename          = 0x00000100,
     opt_logqueuesize         = 0x00000200,
     opt_selfredirect         = 0x00000400,
     opt_enableruletimers     = 0x00000800,
     opt_rulecallbackport     = 0x00001000,
     opt_rulecallbackthreads  = 0x00002000,
     opt_rulecallbackurl      = 0x00004000,
     opt_chronosapiurl        = 0x00008000,
     opt_statlogfilename      = 0x00010000,
     opt_statlognbrfiles      = 0x00020000,
     opt_statlogmaxsize       = 0x00040000,
     opt_statfrequency        = 0x00080000,
     opt_auditlogfilename     = 0x00100000,
     opt_auditlognbrfiles     = 0x00200000,
     opt_auditlogmaxsize      = 0x00400000,
     opt_ossport              = 0x00800000,
     opt_ossfile              = 0x01000000,
   };

   static Options *m_singleton;
   static Options &singleton() { if ( !m_singleton ) m_singleton = new Options(); return *m_singleton; }
   static void help();

   Options();
   ~Options();

   bool parseInputOptions( int argc, char **argv );
   bool parseJson();
   bool validateOptions();

   int m_options;

   std::string m_jsoncfg;
   std::string m_originhost;
   std::string m_originrealm;
   std::string m_fdcfg;
   std::string m_cassHost;
   std::string m_cassKeyspace;

   int m_logmaxsize;
   int m_lognbrfiles;
   std::string m_logfilename;
   int m_logqueuesize;

   int m_statlogmaxsize;
   int m_statlognbrfiles;
   std::string m_statlogfilename;
   uint32_t    m_statsfrequency;

   int m_auditlogmaxsize;
   int m_auditlognbrfiles;
   std::string m_auditlogfilename;

   bool m_selfredirect;
   bool m_enableruletimers;
   int m_rulecallbackport;
   int m_rulecallbackthreads;
   std::string m_rulecallbackurl;
   std::string m_chronosapiurl;

   int         m_ossport;
   std::string m_ossfile;
};

#endif // #define __OPTIONS_H
