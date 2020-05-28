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
