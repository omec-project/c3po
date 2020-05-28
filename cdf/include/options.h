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
   
   static const std::string &diameterConfiguration() { return m_diameterconfiguration; }

private:
   static void help();

   static std::string m_diameterconfiguration;
};

#endif // #define __OPTIONS_H
