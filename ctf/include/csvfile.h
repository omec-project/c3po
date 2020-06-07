/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
