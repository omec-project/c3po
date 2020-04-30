/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SUTILITY_H
#define __SUTILITY_H

#include <string>
#include <vector>

class SUtility
{
public:
   static int indexOf(const char *path, char search, int start = 0);
   static int indexOfAny(const char *path, const char *search);
   static int lastIndexOfAny(const char *path, const char *search);
   static std::vector<std::string> split( const std::string &s, const char delim );
   static std::string string_format( const char *format, ... );
   static void string_format( std::string &dest, const char *format, ... );

   static void copyfile( const std::string &dst, const std::string &src ) { copyfile( dst.c_str(), src.c_str() ); }
   static void copyfile( const char *dst, const std::string &src )        { copyfile( dst,         src.c_str() ); }
   static void copyfile( const std::string &dst, const char *src )        { copyfile( dst.c_str(), src ); }
   static void copyfile( const char *dst, const char *src );

   static void deletefile( const std::string &fn ) { deletefile( fn.c_str() ); }
   static void deletefile( const char *fn );

   static std::string currentTime();

private:
   static void _string_format( std::string &dest, const char *format, va_list &args );
};

#endif // #define __SUTILITY_H
