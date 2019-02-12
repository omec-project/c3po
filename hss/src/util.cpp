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
