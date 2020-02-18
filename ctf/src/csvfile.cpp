/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <sstream>

#include "csvfile.h"
#include "sutility.h"

CSVRow &CSVRow::parse( const std::string &data )
{
   clear();

   std::istringstream ss( data );
   std::string f;

   while ( ss.good() )
   {
      std::getline( ss, f, ',' );
      push_back( f );
   }

   return *this;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


CSVFile::CSVFile()
   : m_skip( 0 )
{
}

CSVFile::~CSVFile()
{
}

void CSVFile::parse()
{
   m_row.parse( data() );
}
