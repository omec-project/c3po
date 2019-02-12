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
#include <algorithm>

#include <arpa/inet.h>

#include "cdnsparser.h"
#include "serror.h"

using namespace CachedDNS;

Parser::Parser( Query *q, unsigned char *rdata, int rlen )
   : m_query( q )
{
   m_data.setData( rdata, rlen );
}

void Parser::parse()
{
   parseHeader();

   // parse question section
   for (int i = 0; i < m_qdcount; i++ )
      m_query->addQuestion( parseQuestion() );

   // parse answer section
   for ( int i = 0; i < m_ancount; i++ )
      m_query->addAnswer( parseResourceRecord() );

   // prase authority (name server) section
   for ( int i = 0; i < m_nscount; i++ )
      m_query->addAuthority( parseResourceRecord() );

   // prase additional record section
   for ( int i = 0; i < m_arcount; i++ )
      m_query->addAdditional( parseResourceRecord() );
}

void Parser::parseHeader()
{
   m_qdcount = GET_INT16( m_data.getPointer(), HDR_QDCOUNT_OFS );
   m_ancount = GET_INT16( m_data.getPointer(), HDR_ANCOUNT_OFS );
   m_nscount = GET_INT16( m_data.getPointer(), HDR_NSCOUNT_OFS );
   m_arcount = GET_INT16( m_data.getPointer(), HDR_ARCOUNT_OFS );

   m_data.incrementOffset( HDR_FIXED_SIZE );

   //std::cout << "m_qdcount = " << m_qdcount << std::endl;
   //std::cout << "m_ancount = " << m_ancount << std::endl;
   //std::cout << "m_nscount = " << m_nscount << std::endl;
   //std::cout << "m_arcount = " << m_arcount << std::endl;
}

Question* Parser::parseQuestion()
{
   std::string qname;
   ns_type qtype;
   ns_class qclass;

   parseDomainName( qname );
   qtype = (ns_type)GET_INT16( m_data.getPointer(), Q_QTYPE_OFS );
   qclass = (ns_class)GET_INT16( m_data.getPointer(), Q_QCLASS_OFS );
   m_data.incrementOffset( Q_FIXED_SIZE );

   return new Question( qname, qtype, qclass );
}

ResourceRecord* Parser::parseResourceRecord()
{
   // parse the common resource record fields
   parseDomainName( m_name );
   m_type = (ns_type)GET_INT16( m_data.getPointer(), RR_TYPE_OFS );
   m_class = (ns_class)GET_INT16( m_data.getPointer(), RR_CLASS_OFS );
   m_ttl = GET_INT32( m_data.getPointer(), RR_TTL_OFS );
   m_rdlength = GET_INT16( m_data.getPointer(), RR_RDLENGTH_OFS );

   // increment the pointer past the common resource record fields
   m_data.incrementOffset( RR_FIXED_SIZE );

   m_rdata = m_data.getPointer();

   ResourceRecord *rr = NULL;

   if ( m_class == ns_c_in && m_type == ns_t_a )
      rr = parseA();
   else if ( m_class == ns_c_in && m_type == ns_t_ns )
      rr = parseNS();
   else if ( m_class == ns_c_in && m_type == ns_t_cname )
      rr = parseCNAME();
   else if ( m_class == ns_c_in && m_type == ns_t_aaaa )
      rr = parseAAAA();
   else if ( m_class == ns_c_in && m_type == ns_t_srv )
      rr = parseSRV();
   else if ( m_class == ns_c_in && m_type == ns_t_naptr )
      rr = parseNAPTR();
   else
   {
      rr = parseRR();
      //SError::throwRuntimeException( "Parser::parseResourceRecord() - unrecognized record type %d", m_type );
   }

   return rr;
}

ResourceRecord* Parser::parseRR()
{
   // increment the  m_data pointer
   m_data.incrementOffset( m_rdlength );

   return new ResourceRecord( m_name, m_type, m_class, m_ttl );
}

ResourceRecord* Parser::parseA()
{
   struct in_addr address;

   if ( m_rdlength != sizeof(address) )
      SError::throwRuntimeException( "Parser::parseA() - invalid RDLENGTH [%d] expected [%d]",
         m_rdlength, sizeof(address) );

   memcpy( &address, m_rdata, sizeof(address) );

   // increment the pointer past RDATA
   m_data.incrementOffset( m_rdlength );

   return new RRecordA( m_name, m_ttl, address );
}

ResourceRecord* Parser::parseNS()
{
   std::string ns;

   parseDomainName( ns );

   // the m_data pointer since it was incremented in parseDomainname()

   return new RRecordNS( m_name, m_ttl, ns );
}

ResourceRecord* Parser::parseCNAME()
{
   std::string alias;

   parseDomainName( alias );

   // the m_data pointer since it was incremented in parseDomainname()

   return new RRecordCNAME( m_name, m_ttl, alias );
}

ResourceRecord* Parser::parseAAAA()
{
   struct in6_addr address;

   if ( m_rdlength != sizeof(address) )
      SError::throwRuntimeException( "Parser::parseAAAA() - invalid RDLENGTH [%d] expected [%d]",
         m_rdlength, sizeof(address) );

   memcpy( &address, m_rdata, sizeof(address) );

   // increment the pointer past RDATA
   m_data.incrementOffset( m_rdlength );

   return new RRecordAAAA( m_name, m_ttl, address );
}

ResourceRecord* Parser::parseSRV()
{
   int priority = GET_INT16( m_rdata, SRV_PRIORITY_OFS );
   int weight = GET_INT16( m_rdata, SRV_WEIGHT_OFS );
   int port = GET_INT16( m_rdata, SRV_PORT_OFS );
   std::string target;

   // increment the pointer past the fixed SRV data
   m_data.incrementOffset( SRV_FIXED_SIZE );

   parseDomainName( target );
   
   return new RRecordSRV( m_name, m_ttl, priority, weight, port, target );
}

ResourceRecord* Parser::parseNAPTR()
{
   int order = GET_INT16( m_rdata, NAPTR_ORDER_OFS );
   int preference = GET_INT16( m_rdata, NAPTR_PREFERENCE_OFS );
   std::string flags;
   std::string service;
   std::string regexp;
   std::string replacement;

   // increment the pointer past the fixed NAPTR data
   m_data.incrementOffset( NAPTR_FIXED_SIZE );

   parseCharacterString( flags );
   parseCharacterString( service );
   parseCharacterString( regexp );
   parseDomainName( replacement );

   return new RRecordNAPTR( m_name, m_ttl, order, preference, flags, service, regexp, replacement );
}

void Parser::parseDomainName( std::string &dn )
{
   int compressedLength = 0;
   int currOfs = m_data.getOffset();
   bool offsetActive = false;
   int val;
   unsigned char *ptr = m_data.getPointer();

   dn = "";

   while ( *ptr )
   {
      if ( (val = LABEL_LENGTH(ptr)) != -1 )
      {
         // label (2 high order bits set to 00)
         // the label is 1 octet followed by the number of octets
         // indicated in the low order 6 bits of the 1st octet

         // ensure that length is not beyond the end of the message
         if ( !m_data.validateLength(currOfs,val) )
             SError::throwRuntimeException( "label extends beyond end of message" );

         ptr++; // increment ptr to skip length
         dn.append( (const char *)ptr, val );
         ptr += val; // skip to next label/offset

         // increment the compressedLength if an offset is not active
         if ( !offsetActive )
            compressedLength += val + 1;
         currOfs += val + 1;

         // check for another label and append "." as appropriate
         if ( *ptr )
            dn.append( "." );
      }
      else if ( (val = LABEL_OFFSET(ptr)) != -1 )
      {
         // offset (2 high order bits of 1st byte set to 11)
         // the offset value is the integer value represented by
         // 2 octets excluding the 2 high order bits of the 1st octet

         // check for 1st offset field
         if ( !offsetActive )
         {
            offsetActive = true;
            compressedLength += 2; // 2 octets
         }

         // get the data pointer indicated by the offset value
         ptr = m_data.getPointer( val );
         currOfs = val;
      }
      else
      {
         SError::throwRuntimeException( "Unexpected label/offset field %x at offset %d",
            *ptr, m_data.calculateOffset(ptr) );
      }
   }

   if ( !offsetActive )
      compressedLength++; // increment length for final null label

   if ( compressedLength == 1 )
      dn = ".";
  
   m_data.incrementOffset( compressedLength );

   // force the string to lower case
   std::transform( dn.begin(), dn.end(), dn.begin(), ::tolower );
}

void Parser::parseCharacterString( std::string &cs )
{
   // 1 octet length, followed by [length] octets as the character string
   int len = *m_data.getPointer();
   m_data.incrementOffset( 1 );

   // assign the character string
   cs.assign( (const char*)m_data.getPointer(), len );
   m_data.incrementOffset( len );

   // force the string to lower case
   std::transform( cs.begin(), cs.end(), cs.begin(), ::tolower );
}
