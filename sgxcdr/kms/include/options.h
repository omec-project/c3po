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
   static const std::string &Port() { return m_port; }
   static const size_t maxConnections() { return m_maxconnections; }
   static const std::string &storePath() { return m_storepath; }

private:

   enum OptionsSelected {
     jsoncfg               = 0x01,
     port		           = 0x02,
     maxconnections        = 0x04,
     storepath             = 0x08,
     whitelisted           = 0x10,
   };

   static void help();

   static const int JSONFILEBUFFER;
   static int options;

   static std::string m_jsoncfg;
   static std::string m_port;
   static size_t m_maxconnections;
   static std::string m_storepath;

};

#endif // #define __OPTIONS_H
