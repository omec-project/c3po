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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <iostream>
#include "options.h"

#include "jsonvalidator.h"
#include "provisionhandler.h"
#include "db.h"

#include <pistache/endpoint.h>
#include <pistache/net.h>

#define RAPIDJSON_NAMESPACE hssrapidjson
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

void handler(int signo, siginfo_t *pinfo, void *pcontext)
{
   std::cout << "caught signal (" << signo << ") initiating shutdown" << std::endl;
}

void initHandler()
{
   struct sigaction sa;
   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = handler;
   sigemptyset(&sa.sa_mask);
   int signo = SIGINT;
   if (sigaction(signo, &sa, NULL) == -1){

   }
   signo = SIGTERM;
   if (sigaction(signo, &sa, NULL) == -1){

   }

}

int main(int argc, char **argv)
{
   if ( !Options::parse( argc, argv ) )
   {
      std::cout << "Options::parse() failed" << std::endl;
      return 1;
   }

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////
   std::cout << "Options::cassserver            : " << Options::getcassserver()            << std::endl;
   std::cout << "Options::cassuser              : " << Options::getcassuser()              << std::endl;
   std::cout << "Options::casspwd               : " << Options::getcasspwd()               << std::endl;
   std::cout << "Options::cassdb                : " << Options::getcassdb()                << std::endl;
   std::cout << "Options::userschema            : " << Options::getuserschema()            << std::endl;
   std::cout << "Options::gtwport               : " << Options::getgtwport()               << std::endl;
   std::cout << "Options::gtwhost               : " << Options::getgtwhost()               << std::endl;
   std::cout << "Options::hssport               : " << Options::gethssport()               << std::endl;
   std::cout << "Options::hsshost               : " << Options::gethsshost()               << std::endl;
   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   DB::getInstance().connect(Options::getcassserver().c_str(), "vhss");

   RAPIDJSON_NAMESPACE::Document* userschema = new RAPIDJSON_NAMESPACE::Document();

   if(!JsonValidator::getInstance().readDocFromPath(*userschema, Options::getuserschema()) )
   {
      std::cout << "Failed reading userschema" << std::endl;
      return 1;
   }
   else
   {
      std::string keyschema("userschema");
      JsonValidator::getInstance().addSchema(keyschema, userschema);
   }

   Pistache::Http::listenAndServe<ProvisionHandler>(Pistache::Address(Options::getgtwhost(), Options::getgtwport()));

   return 0;
}
