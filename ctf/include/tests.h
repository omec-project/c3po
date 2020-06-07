/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __TESTS_H
#define __TESTS_H

namespace rf { class Application; }

class Tests
{
public:
   static bool syslogTest();
   static bool dateTimeTest();
   static bool timerTest();
   static bool queueTest();
   static bool threadTest();
   static bool directoryTest();
   static bool pathTest();
   static bool csvTest();
   static bool splitTest();
   static bool trackTest();
   static bool fdTest1(rf::Application& app);
};

#endif // #define __TESTS_H 
