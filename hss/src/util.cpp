/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "util.h"

std::string Utility::bytes2hex( const uint8_t *bytes, size_t len, char delim, bool upper )
{
   static const char *hexl = "0123456789abcdef";
   static const char *hexu = "0123456789ABCDEF";

   size_t olen(delim ? 3 : 2);
   const char *hexc(upper ? hexu : hexl);
   char hex[ len * olen + 1 ];

   for (size_t i=0; i<len; i++)
   {
      hex[i * olen]     = hexc[ (bytes[i] >> 4) & 0x0f ];
      hex[i * olen + 1] = hexc[  bytes[i]       & 0x0f ];
      if ( delim )
         hex[i * olen + 2] = delim;
   }
   hex[len * olen] = '\0';

   return std::string(hex);
}
