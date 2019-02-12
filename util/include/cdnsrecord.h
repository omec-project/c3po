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

#ifndef __CDNSRECORD_H
#define __CDNSRECORD_H

#include <string>
#include <list>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/inet.h>
#include <memory.h>
#include <time.h>

namespace CachedDNS
{
   class Question
   {
   public:
      Question( const std::string& qname, ns_type qtype, ns_class qclass )
         : m_qname( qname ),
           m_qtype( qtype ),
           m_qclass( qclass )
      {
      }

      std::string &getQName() { return m_qname; }
      ns_type getQType() { return m_qtype; }
      ns_class getQClass() { return m_qclass; }

      void dump()
      {
         std::cout << "Question:"
            << " qtype=" << m_qtype
            << " qclass=" << m_qclass
            << " qname=" << m_qname
            << std::endl;
      }

   private:
      Question() {}

      std::string m_qname;
      ns_type m_qtype;
      ns_class m_qclass;
   };

   class QuestionList : public std::list<Question*>
   {
   public:
      QuestionList() {}
      ~QuestionList()
      {
         while ( !empty() )
         {
            Question *q = *begin();
            erase( begin() );
            delete q;
         }
      }
   };

   class ResourceRecord
   {
   public:
      ResourceRecord( const std::string &name,
                      ns_type rtype,
                      ns_class rclass,
                      int32_t ttl )
         : m_name( name ),
           m_type( rtype ),
           m_class( rclass ),
           m_ttl( ttl ),
           m_expires( time(NULL) + ttl )
      {
      }

      virtual ~ResourceRecord() {}

      const std::string &getName() { return m_name; }
      ns_type getType() { return m_type; }
      ns_class getClass() { return m_class; }
      uint32_t getTTL() { return m_ttl; }
      time_t getExpires() { return m_expires; }

      bool isExpired() { return m_expires <= time(NULL); }

      virtual void dump()
      {
         std::cout << "ResourceRecord:"
            << " type=" << getType()
            << " class=" << getClass()
            << " ttl=" << getTTL()
            << " expires=" << getExpires()
            << " name=" << getName()
            << std::endl;
      }
   
   private:
      std::string m_name;
      ns_type m_type;
      ns_class m_class;
      int32_t m_ttl;
      time_t m_expires;
   };

   class ResourceRecordList : public std::list<ResourceRecord*>
   {
   public:
      ResourceRecordList() {}
      ~ResourceRecordList()
      {
         while ( !empty() )
         {
            ResourceRecord *rr = *begin();
            erase( begin() );
            delete rr;
         }
      }
   };

   class RRecordA : public ResourceRecord
   {
   public:
      RRecordA( const std::string &name,
                int32_t ttl,
                const struct in_addr &address )
         : ResourceRecord(name, ns_t_a, ns_c_in, ttl)
      {
         memcpy( &m_address, &address, sizeof(m_address) );
      }

      const struct in_addr &getAddress() { return m_address; }

      std::string getAddressString()
      {
         char address[ INET_ADDRSTRLEN ];
         inet_ntop( AF_INET, &m_address, address, sizeof(address) );
         return std::string( address );
      }

      virtual void dump()
      {
         std::cout << "RRecordA:"
            << " type=" << getType()
            << " class=" << getClass()
            << " ttl=" << getTTL()
            << " expires=" << getExpires()
            << " address=" << getAddressString()
            << " name=" << getName()
            << std::endl;
      }
   
   private:
      struct in_addr m_address;
   };

   class RRecordNS : public ResourceRecord
   {
   public:
      RRecordNS( const std::string &name,
                    int32_t ttl,
                    const std::string &ns )
         : ResourceRecord( name, ns_t_cname, ns_c_in, ttl ),
           m_namedserver( ns )
      {
      }

      const std::string &getNamedServer() { return m_namedserver; }

      virtual void dump()
      {
         std::cout << "RRecordNS:"
            << " type=" << getType()
            << " class=" << getClass()
            << " ttl=" << getTTL()
            << " expires=" << getExpires()
            << " ns=" << getNamedServer()
            << " name=" << getName()
            << std::endl;
      }
   
   private:
      std::string m_namedserver;
   };

   class RRecordAAAA : public ResourceRecord
   {
   public:
      RRecordAAAA( const std::string &name,
                   int32_t ttl,
                   const struct in6_addr &address )
         : ResourceRecord( name, ns_t_aaaa, ns_c_in, ttl )
      {
         memcpy( &m_address, &address, sizeof(m_address) );
      }

      const struct in6_addr &getAddress() { return m_address; }

      std::string getAddressString()
      {
         char address[ INET6_ADDRSTRLEN ];
         inet_ntop( AF_INET6, &m_address, address, sizeof(address) );
         return std::string( address );
      }

      virtual void dump()
      {
         std::cout << "RRecordAAAA:"
            << " type=" << getType()
            << " class=" << getClass()
            << " ttl=" << getTTL()
            << " expires=" << getExpires()
            << " address=" << getAddressString()
            << " name=" << getName()
            << std::endl;
      }
   
   private:
      struct in6_addr m_address;
   };

   class RRecordCNAME : public ResourceRecord
   {
   public:
      RRecordCNAME( const std::string &name,
                    int32_t ttl,
                    const std::string &alias )
         : ResourceRecord( name, ns_t_cname, ns_c_in, ttl ),
           m_alias( alias )
      {
      }

      const std::string &getAlias() { return m_alias; }

      virtual void dump()
      {
         std::cout << "RRecordCNAME:"
            << " type=" << getType()
            << " class=" << getClass()
            << " ttl=" << getTTL()
            << " expires=" << getExpires()
            << " alias=" << getAlias()
            << " name=" << getName()
            << std::endl;
      }
   
   private:
      std::string m_alias;
   };

   class RRecordSRV : public ResourceRecord
   {
   public:
      RRecordSRV( const std::string &name,
                  int32_t ttl,
                  uint16_t priority,
                  uint16_t weight,
                  uint16_t port,
                  const std::string &target)
         : ResourceRecord( name, ns_t_srv, ns_c_in, ttl ),
           m_priority( priority ),
           m_weight( weight ),
           m_port( port ),
           m_target( target )
      {
      }

      uint16_t getPriority() { return m_priority; }
      uint16_t getWeight() { return m_weight; }
      uint16_t getPort() { return m_port; }
      const std::string &getTarget() { return m_target; }

      virtual void dump()
      {
         std::cout << "RRecordSRV:"
            << " type=" << getType()
            << " class=" << getClass()
            << " ttl=" << getTTL()
            << " expires=" << getExpires()
            << " priority=" << getPriority()
            << " weight=" << getWeight()
            << " port=" << getPort()
            << " target=" << getTarget()
            << " name=" << getName()
            << std::endl;
      }
   
   private:
      uint16_t m_priority;
      uint16_t m_weight;
      uint16_t m_port;
      std::string m_target;
   };

   class RRecordNAPTR : public ResourceRecord
   {
   public:
      RRecordNAPTR( const std::string &name,
                    int32_t ttl,
                    uint16_t order,
                    uint16_t preference,
                    const std::string &flags,
                    const std::string &service,
                    const std::string &regexp,
                    const std::string &replacement )
         : ResourceRecord( name, ns_t_naptr, ns_c_in, ttl ),
           m_order( order ),
           m_preference( preference ),
           m_flags( flags ),
           m_service( service ),
           m_regexp( regexp ),
           m_replacement( replacement )
      {
      }

      const uint16_t getOrder() const { return m_order; }
      const uint16_t getPreference() const { return m_preference; }
      const std::string &getFlags() { return m_flags; }
      const std::string &getService() { return m_service; }
      const std::string &getRegexp() { return m_regexp; }
      const std::string &getReplacement() { return m_replacement; }

      virtual void dump()
      {
         std::cout << "RRecordNAPTR:"
            << " type=" << getType()
            << " class=" << getClass()
            << " ttl=" << getTTL()
            << " expires=" << getExpires()
            << " order=" << getOrder()
            << " preference=" << getPreference()
            << " flags=" << getFlags()
            << " service=" << getService()
            << " regexp=" << getRegexp()
            << " replacement=" << getReplacement()
            << " name=" << getName()
            << std::endl;
      }
   
   private:
      uint16_t m_order;
      uint16_t m_preference;
      std::string m_flags;
      std::string m_service;
      std::string m_regexp;
      std::string m_replacement;
   };
}

#endif // #ifdef __CDNSRECORD_H
