/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SSYSLOG_H
#define __SSYSLOG_H


#include <syslog.h>
#include <string>
#include <cstdarg>
#include "ssync.h"

class SSysLog
{
public:

   SSysLog(const std::string &identity);
   SSysLog(const std::string &identity, int option);
   SSysLog(const std::string &identity, int option, int facility);


   ~SSysLog();

   std::string& getIdentity()                { return m_ident; }
   std::string& setIdentity(std::string& v)  { m_ident = v; return getIdentity(); }
   int getOption()                           { return m_option; }
   int setOption(int v)                      { m_option = v; return getOption(); }
   int getFacility()                         { return m_facility; }
   int setFacility(int v)                    { m_facility = v; return getFacility(); }

   void syslog(int priority, const char* format, ...);
   void syslogs(const std::string& val);

protected:
private:

   SSysLog();

   void openSysLog();
   void closeSysLog();

   std::string m_ident;
   int         m_option;
   int         m_facility;
   bool        m_isopen;

   SMutex      m_mutex;

};




#endif //#define __SSYSLOG_H
