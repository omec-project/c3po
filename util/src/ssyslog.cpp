/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "ssyslog.h"
#include <iostream>

SSysLog::SSysLog()
   :m_option(0),
    m_facility(0),
    m_isopen(false)
{
   openSysLog();
}

SSysLog::SSysLog(const std::string &identity)
   :m_ident(identity),
    m_isopen(false)
{
   m_option = (LOG_NDELAY|LOG_PID|LOG_CONS);
   m_facility = LOG_USER;
   openSysLog();
}
SSysLog::SSysLog(const std::string &identity, int option)
   :m_ident(identity),
    m_option(option),
    m_isopen(false)
{
   m_facility = LOG_USER;
   openSysLog();
}
SSysLog::SSysLog(const std::string &identity, int option, int facility)
   :m_ident(identity),
    m_option(option),
    m_facility(facility)
{
   openSysLog();
}


SSysLog::~SSysLog()
{
   closelog();
}

void SSysLog::openSysLog(){
   openlog(m_ident.c_str(), m_option, m_facility);
}

void SSysLog::syslog(int priority, const char* format, ...)
{
   va_list args;
   va_start(args, format);
   vsyslog(priority, format, args);
   va_end(args);
}

void SSysLog::syslogs(const std::string& val){
   ::syslog(0, "%s", val.c_str());
}
