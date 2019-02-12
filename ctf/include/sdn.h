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
