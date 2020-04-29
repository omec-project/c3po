/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __CSVTRACK_H
#define __CSVTRACK_H

#include <stdint.h>
#include <string>
#include <vector>

#include <iostream>
#include <iomanip>

#define CSVTRACKER_VERIFY_RECNBR1(a) a--; if ( a < m_recnbr ) return;
#define CSVTRACKER_VERIFY_RECNBR2(a,b) a--; if ( a < m_recnbr ) return b;
#define CSVTRACKER_INDEX(a) uint32_t idx = ( a - m_recnbr ) / 32
#define CSVTRACKER_VERIFY_INDEX() while ((idx+1)>m_data.size()) { m_data.push_back(0); }
#define CSVTRACKER_SHIFT(a,b) int shift = (a % 32) * 2 + b
#define CSVTRACKER_GET() (m_data[idx] & (((uint64_t)1)<<shift) ? true : false)
#define CSVTRACKER_SET() m_data[idx] = (state ? m_data[idx] | (((uint64_t)1) << shift) : m_data[idx] & ~(((uint64_t)1) << shift))
#define CSVTRACKER_VERIFY_SIZE() if ((idx+1) > m_data.size()) return false

class CSVTracker
{
public:
   CSVTracker()
   {
      m_recnbr = 0;
      m_recofs = 0;
   }

   ~CSVTracker() {}

   //
   // NOTE: recnbr is a 1 based
   //
   void ACRsent( uint32_t recnbr, uint32_t recofs, bool state = true )
   {
      CSVTRACKER_VERIFY_RECNBR1( recnbr );
      uint32_t rnidx = recnbr - m_recnbr;
      while ( rnidx >= m_ofs.size() ) m_ofs.push_back( UINT32_MAX );
      m_ofs[rnidx] = recofs;
      CSVTRACKER_INDEX( recnbr );
      CSVTRACKER_VERIFY_INDEX();
      CSVTRACKER_SHIFT( recnbr, 0 );
      CSVTRACKER_SET();
   }

   void ACArcvd( uint32_t recnbr, bool state = true )
   {
      CSVTRACKER_VERIFY_RECNBR1( recnbr );
      CSVTRACKER_INDEX( recnbr );
      CSVTRACKER_VERIFY_INDEX();
      CSVTRACKER_SHIFT( recnbr, 1 );
      CSVTRACKER_SET();
      m_acacnt++;
   }

   bool isACRsent( uint32_t recnbr )
   {
      CSVTRACKER_VERIFY_RECNBR2( recnbr, true );
      CSVTRACKER_INDEX( recnbr );
      CSVTRACKER_VERIFY_SIZE();
      CSVTRACKER_SHIFT( recnbr, 0 );
//uint64_t val = 1 << shift; std::cout << "isACRsent() idx=" << idx << " shift=" << shift << " val=" << val << " m_data=" << m_data[idx] << std::endl;
      return CSVTRACKER_GET();
   }

   bool isACArcvd( uint32_t recnbr )
   {
      CSVTRACKER_VERIFY_RECNBR2( recnbr, true );
      CSVTRACKER_INDEX( recnbr );
      CSVTRACKER_VERIFY_SIZE();
      CSVTRACKER_SHIFT( recnbr, 1 );
      return CSVTRACKER_GET();
   }

   const std::string &getFilename() { return m_filename; }
   const std::string &setFilename( const char *filename ) { return m_filename = filename; }
   const std::string &setFilename( const std::string &filename ) { return m_filename = filename; }

   void loadData();
   void saveData();

   uint32_t getFirstRecordNumber() { return m_recnbr + 1; }
   uint32_t getFirstRecordOffset() { return m_recofs; }
   int getACAcount() { return m_acacnt; }

   uint32_t getRecordOffset( uint32_t recnbr )
   {
      if ( recnbr < m_recnbr)
         return UINT32_MAX;
      uint32_t rnidx = recnbr - m_recnbr;
      while ( rnidx > m_ofs.size() ) m_ofs.push_back( UINT32_MAX );
      return m_ofs[rnidx];
   }

   void clear()
   {
      // empty the vectors
      m_data.clear();
      m_ofs.clear();
      m_recnbr = 0;
      m_recofs = 0;
      m_acacnt = 0;
   }

   std::vector<uint64_t>::size_type getDataSize() { return m_data.size(); }
   std::vector<uint64_t>::size_type getOffsetSize() { return m_ofs.size(); }

private:
   std::string m_filename;
   uint32_t m_recnbr;
   uint32_t m_recofs;
   int m_acacnt;
   std::vector<uint64_t> m_data;
   std::vector<uint32_t> m_ofs;
};

#endif // #define __CSVTRACK_H
