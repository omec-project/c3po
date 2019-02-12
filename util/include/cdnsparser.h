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

#ifndef __CDNSPARSER_H
#define __CDNSPARSER_H

#include <string>

#include "cdnsquery.h"

#define LABEL_LENGTH(a) ((int)(((a[0] & 0xc0) == 0) ? a[0] & 0x3f : -1))
#define LABEL_OFFSET(a) ((int)(((a[0] & 0xc0) == 0xc0) ? ((a[0] & 0x3f) << 8) + a[1] : -1))
#define GET_INT16(a,b) ((((int)(a[b])) << 8) + (int)a[b+1])
#define GET_INT32(a,b) ((((int)a[b]) << 24) + (((int)a[b+1]) << 16) + (((int)a[b+2]) << 8) + ((int)a[b+3]))

namespace CachedDNS
{
   class MessageBuffer
   {
   public:
      MessageBuffer()
      {
         setData( NULL, 0 );
      }

      MessageBuffer( unsigned char *data, int len )
      {
         setData( data, len );
      }

      MessageBuffer &setData( unsigned char *data, int len )
      {
         m_data = data;
         m_len = len;
         m_ofs = 0;

         return *this;
      }
  
      unsigned char *getPointer() { return m_ofs >= m_len ? NULL : m_data + m_ofs; }
      unsigned char *getPointer( int ofs ) { return ofs >= m_len ? NULL : m_data + ofs; }
      bool validateLength( int ofs, int len ) { return (ofs + len) >= m_len ? false : true; }
      //bool validateLength( int ofs, int len ) { std::cout << "validateLength() ofs=" << ofs << " len=" << len << " m_len=" << m_len << std::endl; return (ofs + len) >= m_len ? false : true; }
  
      int incrementOffset( int len ) { return m_ofs = m_ofs + len > m_len ? m_len : m_ofs + len; }
      int getOffset() { return m_ofs; }
      int setOffset( int ofs ) { return m_ofs = ofs; }
      int calculateOffset( unsigned char *ptr ) { return (int)(ptr - m_data); }

   private:
  
      unsigned char * m_data;
      int m_len;
      int m_ofs;
   };

   class Parser
   {
   public:
      Parser( Query *q, unsigned char *rdata, int rlen );

      void parse();

   private:
      Parser() {}

      Question* parseQuestion();
      ResourceRecord* parseResourceRecord();
      ResourceRecord* parseRR();
      ResourceRecord* parseA();
      ResourceRecord* parseNS();
      ResourceRecord* parseCNAME();
      ResourceRecord* parseAAAA();
      ResourceRecord* parseSRV();
      ResourceRecord* parseNAPTR();

      void parseHeader();
      void parseDomainName( std::string &dn );
      void parseCharacterString( std::string &cs );

      Query *m_query;
      MessageBuffer m_data;

      int m_qdcount;
      int m_ancount;
      int m_nscount;
      int m_arcount;

      std::string m_name;
      ns_type m_type;
      ns_class m_class;
      int m_ttl;
      int m_rdlength;
      unsigned char *m_rdata;

      // RFC 1035
      static const int HDR_FIXED_SIZE       = 12;
      static const int HDR_QDCOUNT_OFS      = 4;
      static const int HDR_ANCOUNT_OFS      = 6;
      static const int HDR_NSCOUNT_OFS      = 8;
      static const int HDR_ARCOUNT_OFS      = 10;

      static const int Q_FIXED_SIZE         = 4;
      static const int Q_QTYPE_OFS          = 0;
      static const int Q_QCLASS_OFS         = 2;

      static const int RR_FIXED_SIZE        = 10;
      static const int RR_TYPE_OFS          = 0;
      static const int RR_CLASS_OFS         = 2;
      static const int RR_TTL_OFS           = 4;
      static const int RR_RDLENGTH_OFS      = 8;
      static const int RR_RDATA_OFS         = 10;

      static const int A_FIXED_SIZE         = 0;
      static const int A_ADDRESS_OFS        = 0;

      static const int CNAME_FIXED_SIZE     = 0;
      static const int CNAME_TARGET_OFS     = 0;

      // RFC 3596
      static const int AAAA_FIXED_SIZE      = 0;
      static const int AAAA_ADDRESS_OFS     = 0;

      // RFC 2915
      static const int NAPTR_FIXED_SIZE     = 4;
      static const int NAPTR_ORDER_OFS      = 0;
      static const int NAPTR_PREFERENCE_OFS = 2;

      // RFC 2782
      static const int SRV_FIXED_SIZE       = 6;
      static const int SRV_PRIORITY_OFS     = 0;
      static const int SRV_WEIGHT_OFS       = 2;
      static const int SRV_PORT_OFS         = 4;
      static const int SRV_TARGET_OFS       = 6;
   };
}

#endif // #ifndef CDNSPARSER_H
