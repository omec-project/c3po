/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SDN_H
#define __SDN_H

#include <stdexcept>
#include <string>

struct memcached_st;
struct memcached_server_st;


class SDNException : public std::runtime_error
{
public:
   SDNException(const char *m) : std::runtime_error(m) {}
   SDNException(const std::string &m) : std::runtime_error(m) {}
};

class SDN
{
public:

   SDN();
   ~SDN();

   bool get( const char *ipaddr );
   bool get( const std::string &ipaddr ) { return get( ipaddr.c_str() ); }

   const std::string &getIpAddress() { return m_ipaddress; }
   const std::string &getIMSI() { return m_imsi; }
   const std::string &getMDN() { return m_mdn; }
   const std::string &getContext() { return m_context; }
   time_t getStartTime() { return m_start; }
   time_t getEndTime() { return m_end; }

private:
   memcached_st *m_mc;
   memcached_server_st *m_mcs;
   
   std::string m_ipaddress;
   std::string m_imsi;
   std::string m_mdn;
   std::string m_context;
   time_t m_start;
   time_t m_end;
};

#endif // #define __SDN_H
