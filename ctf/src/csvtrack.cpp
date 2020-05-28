/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include "csvtrack.h"

void CSVTracker::loadData()
{
   // clear the data
   clear();

   // open the file:
   std::ifstream file( m_filename.c_str(), std::ios::binary );

   if ( !file.is_open() )
      return;

   // Stop eating new lines in binary mode!!!
   file.unsetf(std::ios::skipws);

   // get its size:
   std::streampos fileSize;

   file.seekg( 0, std::ios::end );
   fileSize = file.tellg();
   file.seekg( 0, std::ios::beg );

   // reserve capacity
   uint32_t amt = ( fileSize - (std::streampos)(sizeof(m_recnbr) + sizeof(m_recofs)) ) / 8;
   m_data.reserve( amt );

   // read the first record number
   uint32_t u1;
   if ( !file.read( (char*)&u1, sizeof( u1 ) ) )
   {
      clear();
      return;
   }
   m_recnbr = u1;

   // read the first record offset
   if ( !file.read( (char*)&u1, sizeof( u1 ) ) )
   {
      clear();
      return;
   }
   m_recofs = u1;
   m_ofs.push_back( m_recofs );

   // read the data:
   uint64_t u;
   for ( uint32_t idx = 0; idx < amt; idx++ )
   {
      if ( file.read( (char*)&u, sizeof(u)) )
         m_data.push_back( u );
      else
         break;
   }

   // clear the ACR sent flag where the ACA has not been received
   uint32_t last = m_data.size() * 32 + m_recnbr + 1;
   for ( u1 = m_recnbr + 1; u1 < last; u1++ )
   {
      if ( !isACArcvd( u1 ) )
         ACRsent( u1, getRecordOffset( u1 ), false );
   }
}

void CSVTracker::saveData()
{
   // identify the first uint_64 entry that has not been completely processed
   uint32_t firstblock = 0;
   for ( ; firstblock < m_data.size() && m_data[firstblock] == UINT64_MAX; firstblock++ );
   uint32_t firstrecnbr = firstblock * 32 + m_recnbr;

   // get the offset of the firstentry
   uint32_t firstrecofs = getRecordOffset( firstrecnbr );

   // open the output file
   std::ofstream file( m_filename.c_str(), std::ofstream::binary );

   // write the first record number
   file.write( (char*)&firstrecnbr, sizeof( firstrecnbr ) );

   // write the first record offset
   file.write( (char*)&firstrecofs, sizeof( firstrecofs ) );

   // write the data
   for ( uint32_t idx = firstblock; idx < m_data.size(); idx++ )
   {
      uint64_t u = m_data[idx];
      file.write( reinterpret_cast<char*>(&u), sizeof(u) );
   }

   // declare the new vectors
   std::vector<uint64_t> newdata;
   std::vector<uint32_t> newofs;

   // shrink the data
   for ( uint32_t idx = firstblock; idx < m_data.size(); idx++ )
      newdata.push_back( m_data[ idx ] );
   
   // shrink the offsets
   for ( uint32_t idx = firstblock * 32; idx < m_ofs.size(); idx++ )
      newofs.push_back( m_ofs [ idx ] );

   // assign the new values
   m_data = newdata;
   m_ofs = newofs;
   m_recnbr = firstrecnbr;
   m_recofs = firstrecofs;
   m_acacnt = 0;
}
