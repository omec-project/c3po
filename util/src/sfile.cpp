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

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "sfile.h"

SFile::SFile()
{
   close();
}

SFile::~SFile()
{
}

void SFile::close()
{
   if (csv_file != NULL)
	fclose(csv_file);

   if (stream_buffer)
	free(stream_buffer);

   m_filename.clear();
   m_data.clear();
   m_datarn = 0;
   m_dataofs = 0;
   m_nextrn = 1;
   m_nextofs = 0;
   csv_file = NULL;
   stream_buffer = NULL;
}

bool SFile::open( const char *filename )
{
   close();

   m_filename = filename;

   csv_file = fopen(m_filename.c_str(), "r");

   return (csv_file != NULL);
}

bool SFile::seek( uint32_t recnbr, std::ios::streampos offset )
{
   clearerr(csv_file);

   long old = ftell(csv_file);

   fseek(csv_file,(long)offset, SEEK_SET);

   if (!feof(csv_file))
   {
      m_nextrn = recnbr;
      m_nextofs = offset;
      return true;
   }
   else
   {
      std::cout << "m_stream is not good after seekg, good=" <<
            ferror(csv_file) << " eof=" << feof(csv_file) << std::endl;
   }

   clearerr(csv_file);

   fseek(csv_file,old,SEEK_SET);
   return false;
}

bool SFile::read()
{
   // create temporary buffer
   char data[2048];

   // seek to the beginning of the record
   clearerr(csv_file);

   fseek(csv_file,(long)m_nextofs,SEEK_SET);

   // read the next line
   fgets(data, sizeof(data), csv_file);
   data[strlen(data)-1] = '\0';

   if (!feof(csv_file))
   {
      // update the info
      m_data = data;
      m_dataofs = m_nextofs;
      m_datarn = m_nextrn;
      m_nextofs = ftell(csv_file);
      m_nextrn++;
      return true;
   }

   return false;
}

bool SFile::renamed()
{
   return access( m_filename.c_str(), F_OK ) != 0;
}
