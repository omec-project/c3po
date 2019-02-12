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


#include <stdarg.h>
#include <string.h>

#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>

#include "sutility.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int SUtility::indexOf(const char *path, char search, int start)
{
   if (start > (int)strlen(path))
      return -1;

   path = &path[start];
   int ofs = start;
   for (; *path && *path != search; ofs++, path++);

   return *path ? -1 : ofs;
}

int SUtility::indexOfAny(const char *path, const char *search)
{
   int ofs = 0;

   for (const char *p = path; *p; ofs++, p++)
   {
      const char *sp = search;
      for (; *sp && *p != *sp; ++sp);
      if (*sp)
         break;
   }

   return path[ofs] ? ofs : -1;
}

int SUtility::lastIndexOfAny(const char *path, const char *search)
{
   if (!*path)
      return -1;

   const char *p = &path[strlen(path)-1];

   for (; p >= path; p--)
   {
      const char *sp = search;
      for (; *sp && *p != *sp; ++sp);
      if (*sp)
         break;
   }

   return p < path ? -1 : (int)(p - path);
}

std::vector<std::string> SUtility::split( const std::string &s, const char delim )
{
   std::vector<std::string> a;
   std::istringstream ss( s );
   std::string f;

   while ( ss.good() )
   {
      std::getline( ss, f, delim );
      a.push_back( f );
   }

   return a;
}

void SUtility::copyfile( const char *dst, const char *src )
{
   std::ofstream fdst( dst, std::ios::binary);
   std::ifstream fsrc( src, std::ios::binary);

   fdst << fsrc.rdbuf();
}

void SUtility::deletefile( const char *fn )
{
   remove( fn );
}

void SUtility::_string_format( std::string &dest, const char *format, va_list &args )
{
   char buf[2048];
   uint32_t len = vsnprintf( buf, sizeof(buf), format, args  );
   dest.assign( buf, len < sizeof(buf) ? len : sizeof(buf) );
}

std::string SUtility::string_format( const char *format, ... )
{
   va_list args;
   std::string str;

   va_start( args, format );
   _string_format( str, format, args );
   va_end( args );

   return str;
}

void SUtility::string_format( std::string &dest, const char *format, ... )
{
   va_list args;

   va_start( args, format );
   _string_format( dest, format, args );
   va_end( args );
}

std::string SUtility::currentTime()
{
   time_t t = time( NULL );
   struct tm *now = localtime( &t );
   std::stringstream ss;

   ss << (now->tm_year + 1900) << '-'
      << (now->tm_mon + 1) << '-'
      << (now->tm_mday) << ' '
      <<  now->tm_hour << ':'
      <<  now->tm_min << ':'
      <<  now->tm_sec;

   return ss.str();
}

