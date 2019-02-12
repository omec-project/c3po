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
   static bool parseInputOptions( int argc, char **argv );
   static bool parseJson();
   static bool validateOptions();
   static const std::string &jsonConfig() { return m_jsoncfg; }
   static const std::string &runMode() { return m_runmode; }
   static const std::string &cdrRouterHost() { return m_cdrrouterhost; }
   static const size_t &cdrRouterPort() { return m_cdrrouterport; }
   static const std::string &dealerPort() { return m_dealerport; }
   static const size_t maxConnections() { return m_maxconnections; }
   static std::string &kmsServer() {return m_kmsserver;}
   static std::string &kmsPort() {return m_kmsport;}
   static std::string &keyNamespace() {return m_keynamespace;}
   static std::string &cdrPath() {return m_cdrpath;}
   static std::string &cdrArchPath() {return m_cdrarchpath;}

   static int dryRun;
private:

   enum OptionsSelected {
     jsoncfg               = 0x01,
     runmode	           = 0x02,
     cdrrouterhost         = 0x04,
     cdrrouterport         = 0x08,
     maxconnections	   = 0x10,
     dealerport		   = 0x20,
     kmsserver		   = 0x40,
     kmsport		   = 0x80,
     keynamespace	   = 0x100,
     cdrpath		   = 0x200,
     cdrarchpath	   = 0x400,
   };

   static void help();

   static const int JSONFILEBUFFER;
   static int options;

   static std::string m_jsoncfg;
   static std::string m_runmode;
   static std::string m_cdrrouterhost;
   static size_t m_cdrrouterport;
   static std::string m_dealerport;
   static size_t m_maxconnections;
   static std::string m_kmsserver;
   static std::string m_kmsport;
   static std::string m_keynamespace;
   static std::string m_cdrpath;
   static std::string m_cdrarchpath;
};

#endif // #define __OPTIONS_H
