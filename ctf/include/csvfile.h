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

#ifndef __CSVFILE_H
#define __CSVFILE_H

#include <stdlib.h>

#include <string>
#include <vector>

#include "sfile.h"

class CSVRow : public std::vector<std::string>
{
public:
   CSVRow() {}
   ~CSVRow() {}

   CSVRow &parse( const std::string &data );

   int32_t getInt32( int ofs ) const { return atoi( (*this)[ofs].c_str() ); }
   int64_t getInt64( int ofs ) const { return strtoll( (*this)[ofs].c_str(), NULL, 10 ); }
   uint32_t getUint32( int ofs ) const { return (uint32_t)strtoul( (*this)[ofs].c_str(), NULL, 10 ); }
   uint64_t getUint64( int ofs ) const { return strtoull( (*this)[ofs].c_str(), NULL, 10 ); }
   
};

class CSVFile : public SFile
{
public:
   CSVFile();
   ~CSVFile();

   const CSVRow &row() { return m_row; }

   uint32_t getSkip() { return m_skip; }
   uint32_t setSkip( uint32_t s ) { return m_skip = s; }

   void parse();

private:
   CSVRow m_row;
   uint32_t m_skip;
};

#endif // #define __CSVFILE_H
