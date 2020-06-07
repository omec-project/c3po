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

   static const std::string &originHost()             { return singleton().m_originhost; }
   static const std::string &originRealm()            { return singleton().m_originrealm; }
   static const std::string &diameterConfiguration()  { return singleton().m_fdcfg; }
   static const std::string &cassandraHost()          { return singleton().m_cassHost; }
   static const std::string &cassandraKeyspace()      { return singleton().m_cassKeyspace; }
   static const std::string &pcrfHost()               { return singleton().m_pcrfhost; }
   static const std::string &pcrfRealm()              { return singleton().m_pcrfrealm; }

   static const int logMaxSize()                      { return singleton().m_logmaxsize; }
   static const int logNumberFiles()                  { return singleton().m_lognbrfiles; }
   static const std::string &logFilename()            { return singleton().m_logfilename; }
   static const int logQueueSize()                    { return singleton().m_logqueuesize; }

private:
   enum OptionsSelected {
     opt_jsoncfg               = 0x00000001,
     opt_originhost            = 0x00000002,
     opt_originrealm           = 0x00000004,
     opt_fdcfg                 = 0x00000008,
     opt_casshost              = 0x00000010,
     opt_cassks                = 0x00000020,
     opt_pcrfhost              = 0x00000040,
     opt_pcrfrealm             = 0x00000080,
     opt_logmaxsize            = 0x00000100,
     opt_lognbrfiles           = 0x00000200,
     opt_logfilename           = 0x00000400,
     opt_logqueuesize          = 0x00000800
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
   std::string m_pcrfhost;
   std::string m_pcrfrealm;

   int m_logmaxsize;
   int m_lognbrfiles;
   std::string m_logfilename;
   int m_logqueuesize;
};

#endif // #define __OPTIONS_H
