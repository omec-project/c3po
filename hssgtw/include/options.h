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
   
   static const std::string &getjsonConfig()             { return m_jsoncfg; }
   static const std::string &getcassserver()             { return m_cassserver; }
   static const std::string &getcassuser()               { return m_cassuser; }
   static const std::string &getcasspwd()                { return m_casspwd; }
   static const std::string &getcassdb()                 { return m_cassdb; }
   static const std::string &getuserschema()             { return m_user_schema; }
   static const int &getgtwport()                        { return m_gtwport; }
   static const std::string &getgtwhost()                { return m_gtwhost; }
   static const int &gethssport()                        { return m_hssport; }
   static const std::string &gethsshost()                { return m_hsshost; }


private:

   enum OptionsSelected {
     cassserver                        = 0x01,
     cassuser                          = 0x02,
     casspwd                           = 0x04,
     cassdb                            = 0x08,
     userschema                        = 0x10,
     gtwport                           = 0x20,
     gtwhost                           = 0x40,
     hsshost                           = 0x80,
     hssport                           = 0x100,
   };

   static void help();

   static const int JSONFILEBUFFER;
   static int options;

   static std::string m_jsoncfg;

   static std::string m_cassserver;
   static std::string m_cassuser;
   static std::string m_casspwd;
   static std::string m_cassdb;
   static std::string m_user_schema;
   static int         m_gtwport;
   static std::string m_gtwhost;
   static int         m_hssport;
   static std::string m_hsshost;
};

#endif // #define __OPTIONS_H
