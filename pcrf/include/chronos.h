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

#ifndef PCRF_INCLUDE_CHRONOS_H_
#define PCRF_INCLUDE_CHRONOS_H_

#include <pistache/client.h>
#include <pistache/endpoint.h>
#include <pistache/http_header.h>

class ChronosHandler : public Pistache::Http::Handler
{
public:
   HTTP_PROTOTYPE(ChronosHandler)

   void onRequest( const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response );
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class ChronosLocation : public Pistache::Http::Header::Header
{
public:
   NAME( "Location" )

   ChronosLocation() {}

   void parseRaw( const char *str, size_t len )
   {
      m_location.assign( str, len );
   }

   void write( std::ostream &os ) const
   {
      os << m_location;
   }

   const std::string &getLocation() const
   {
      return m_location;
   }

private:
   std::string m_location;
};

class Chronos
{
public:
   static void init();
   static void uninit();
   static Pistache::Http::Client &client() { return m_client; }

private:
   static Pistache::Http::Client m_client;
};

#endif /* PCRF_INCLUDE_CHRONOS_H_ */
