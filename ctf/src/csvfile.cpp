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
