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

#ifndef __CDNSQUERY_H
#define __CDNSQUERY_H

#include <iostream>
#include <string>
#include <map>
#include <list>

#include "cdnsrecord.h"

namespace CachedDNS
{
   class Query
   {
   public:
      Query( ns_type rtype, const std::string &domain )
         : m_type( rtype ),
           m_domain( domain ),
           m_expires( 0 )
      {
      }

      ~Query()
      {
/*
         while ( !m_question.empty() )
         {
            Question *q = m_question.front();
            m_question.pop_front();
            delete q;
         }

         while ( !m_answer.empty() )
         {
            ResourceRecord *a = m_answer.front();
            m_answer.pop_front();
            delete a;
         }

         while ( !m_authority.empty() )
         {
            ResourceRecord *a = m_authority.front();
            m_authority.pop_front();
            delete a;
         }

         while ( !m_additional.empty() )
         {
            ResourceRecord *a = m_additional.front();
            m_additional.pop_front();
            delete a;
         }
*/
      }
   
      void addQuestion( Question *q )
      {
         if ( q )
            m_question.push_back( q );
      }

      void addAnswer( ResourceRecord *a )
      {
         if ( a )
         {
            if (m_expires == 0 || a->getExpires() < m_expires)
               m_expires = a->getExpires();
            m_answer.push_back( a );
         }
      }

      void addAuthority( ResourceRecord *a )
      {
         if ( a )
         {
            if (m_expires == 0 || a->getExpires() < m_expires)
               m_expires = a->getExpires();
            m_authority.push_back( a );
         }
      }

      void addAdditional( ResourceRecord *a )
      {
         if ( a )
         {
            if (m_expires == 0 || a->getExpires() < m_expires)
               m_expires = a->getExpires();
            m_additional.push_back( a );
         }
      }

      ns_type getType() { return m_type; }
      const std::string &getDomain() { return m_domain; }

      bool isExpired() { return time(NULL) >= m_expires; }

      const std::list<Question*> &getQuestions() { return m_question; }
      const ResourceRecordList &getAnswers() { return m_answer; }
      const ResourceRecordList &getAuthorities() { return m_authority; }
      const ResourceRecordList &getAdditional() { return m_additional; }

      void dump()
      {
         std::cout << "QUERY type=" << getType() << " domain=" << getDomain() << std::endl;
         std::cout << "QUESTION:" << std::endl;
         for (QuestionList::const_iterator it = getQuestions().begin();
              it != getQuestions().end();
              ++it )
         {
            (*it)->dump();
         }

         std::cout << "ANSWER:" << std::endl;
         for (ResourceRecordList::const_iterator it = getAnswers().begin();
              it != getAnswers().end();
              ++it )
         {
            (*it)->dump();
         }

         std::cout << "AUTHORITY:" << std::endl;
         for (ResourceRecordList::const_iterator it = getAuthorities().begin();
              it != getAuthorities().end();
              ++it )
         {
            (*it)->dump();
         }

         std::cout << "ADDITIONAL:" << std::endl;
         for (ResourceRecordList::const_iterator it = getAdditional().begin();
              it != getAdditional().end();
              ++it )
         {
            (*it)->dump();
         }
      }
   
   private:
      ns_type m_type;
      std::string m_domain;
      QuestionList m_question;
      ResourceRecordList m_answer;
      ResourceRecordList m_authority;
      ResourceRecordList m_additional;
      time_t m_expires;
   };

   class QueryCacheKey
   {
   public:
      QueryCacheKey( ns_type rtype, const std::string &domain )
         : m_type( rtype ),
           m_domain( domain )
      {
      }

      QueryCacheKey( const QueryCacheKey &other )
      {
         m_type = other.m_type;
         m_domain = other.m_domain;
      }

      const QueryCacheKey& operator=( const QueryCacheKey &r )
      {
         m_type = r.m_type;
         m_domain = r.m_domain;
         return *this;
      }

      bool operator<( const QueryCacheKey &r ) const
      {
         return
            this->m_type < r.m_type ? true :
            this->m_type > r.m_type ? false :
            this->m_domain < r.m_domain ? true : false;
      }

      const ns_type getType() { return m_type; }
      const std::string &getDomain() { return m_domain; }
   
   private:
      ns_type m_type;
      std::string m_domain;
   };

   typedef std::map<QueryCacheKey, Query*> QueryCache;
}

#endif // #ifndef __CDNSQUERY_H
