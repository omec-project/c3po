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

#ifndef __SDIR_H
#define __SDIR_H

#include <string>
#include <list>

#include <dirent.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef std::list<std::string> SDirectoryListing;

class SDirectory
{
public:
   SDirectory();
   ~SDirectory();

   const char *getFirstEntry( const char *directory, const char *filemask );
   const char *getFirstEntry();
   const char *getNextEntry();

   static void getCurrentDirectory(std::string &dir);

   SDirectoryListing getListing( const char *directory, const char *filemask );
   SDirectoryListing getListing();

   const std::string &getDirectory() { return m_directory; }
   const std::string &setDirectory( const std::string &directory ) { return setDirectory( directory.c_str() ); }
   const std::string &setDirectory( const char *directory ) { return m_directory = directory; }

   const std::string &getFilemask() { return m_filemask; }
   const std::string &setFilemask( const std::string &filemask ) { return setFilemask( filemask.c_str() ); }
   const std::string &setFilemask( const char *filemask );

private:
   static void buildTable();
   static bool match( const char *str, const char *mask, bool ignoreCase = false );

   void closeHandle();

   static char *m_table;

   DIR* m_handle;
   std::string m_directory;
   std::string m_filemask;
   std::string m_filename;
};

#endif // #define __SDIR_H
