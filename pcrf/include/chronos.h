/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
