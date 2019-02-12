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

#include "serror.h"
#include "sdir.h"

#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SDirectory::SDirectory()
{
   m_handle = NULL;
}

SDirectory::~SDirectory()
{
   closeHandle();
}

const std::string &SDirectory::setFilemask( const char *filemask )
{
   // remove any consecutive asterisks from the file mask
   const char *pMask = filemask;
   const char *pLastAsterisk = NULL;

   m_filemask = "";
   while (*pMask)
   {
      if (*pMask == '*')
      {
         if (!pLastAsterisk || (pLastAsterisk && pLastAsterisk != pMask - 1))
            m_filemask += *pMask;
         pLastAsterisk = pMask;
      }
      else
      {
         m_filemask += *pMask;
      }
      pMask++;
   }

   return m_filemask;
}

const char *SDirectory::getFirstEntry( const char *directory, const char *filemask )
{
   setDirectory( directory );
   setFilemask( filemask );

   return getFirstEntry();
}

const char *SDirectory::getFirstEntry()
{
   closeHandle();
   m_handle = opendir(m_directory.c_str());
   if (!m_handle)
      SError::throwRuntimeExceptionWithErrno( "Error getting the first directory entry" );

   try
   {
      getNextEntry();
   }
   catch (...)
   {
      SError::throwRuntimeExceptionWithErrno( "Error getting the first directory entry" );
   }

   return m_filename.length() == 0 ? NULL : m_filename.c_str();
}

const char *SDirectory::getNextEntry()
{
   struct dirent de;
   struct dirent *pde;
   const char *pMask = m_filemask.c_str();

   while (true)
   {
      int result = readdir_r(m_handle, &de, &pde);
      if (result)
         SError::throwRuntimeException( "Error getting the next directory entry - %d", result );

      if (!pde)
      {
         m_filename = "";
         break;
      }

      if (match(de.d_name, pMask))
      {
         m_filename = de.d_name;
         break;
      }
   }

   return m_filename.length() == 0 ? NULL : m_filename.c_str();
}

void SDirectory::getCurrentDirectory( std::string &dir )
{
   char cwd[FILENAME_MAX];

   if (!getcwd(cwd, sizeof(cwd)))
      SError::throwRuntimeExceptionWithErrno( "Error getting the current directory" );

   dir = cwd;
}

void SDirectory::closeHandle()
{
   if (m_handle != NULL)
   {
      closedir(m_handle);
      m_handle = NULL;
   }
}

#define MATCH(a,b,ignoreCase) (ignoreCase?m_table[(int)a]==m_table[(int)b]:a==b)

char *SDirectory::m_table = NULL;

void SDirectory::buildTable()
{
   m_table = new char[256];
   for (int i=0; i<256; i++)
      m_table[i] = (i >= 'a' && i <= 'z') ? 'A' + i - 'a' : (char)i;
}

bool SDirectory::match( const char *str, const char *mask, bool ignoreCase )
{
   if (!m_table)
      buildTable();

   const char *mp = NULL;
   const char *sp = NULL;

   while(*str && *mask != '*')
   {
      if (*mask != '?' && !MATCH(*str,*mask,ignoreCase))
         return false;
      str++;
      mask++;
   }

   while (*str)
   {
      if (*mask == '*')
      {
         if (!*++mask)
            return true;
         mp = mask;
         sp = str + 1;
      }
      else if (*mask == '?' || MATCH(*str,*mask,ignoreCase))
      {
         str++;
         mask++;
      }
      else
      {
         mask = mp;
         str = sp++;
      }
   }

   while (*mask == '*')
      mask++;

   return !*mask;
}

SDirectoryListing SDirectory::getListing( const char *directory, const char *filemask )
{
   setDirectory( directory );
   setFilemask( filemask );

   return getListing();
}

SDirectoryListing SDirectory::getListing()
{
   SDirectoryListing dl;
   const char *de = getFirstEntry();
   std::string s;

   while ( de )
   {
      s = de;
      dl.push_back( s );
      de = getNextEntry();
   }

   dl.sort();

   return dl;
}
