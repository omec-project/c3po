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
   static const std::string &hssHost()                { return singleton().m_hsshost; }
   static const std::string &hssRealm()               { return singleton().m_hssrealm; }

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
     opt_hsshost               = 0x00000010,
     opt_hssrealm              = 0x00000020,
     opt_logmaxsize            = 0x00000040,
     opt_lognbrfiles           = 0x00000080,
     opt_logfilename           = 0x00000100,
     opt_logqueuesize          = 0x00000200
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
   std::string m_hsshost;
   std::string m_hssrealm;

   int m_logmaxsize;
   int m_lognbrfiles;
   std::string m_logfilename;
   int m_logqueuesize;
};

#endif // #define __OPTIONS_H
