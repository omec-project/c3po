/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __AIRTEST_H
#define __AIRTEST_H

#include "satomic.h"
#include "ssync.h"
#include "stimer.h"
#include "sthread.h"

#include "hssperf.h"

struct AIRTracker
{
   AIRTracker()
   {
      sent = false;
      rcvd = false;
      succeeded = false;
   }

   STimerElapsed timer;
   bool sent;
   bool rcvd;
   bool succeeded;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AIRM_START   (ETM_USER + 2)

class AIRTestThread;

class AIRTestSendThread : public SThread
{
public:
   AIRTestSendThread(AIRTestThread &ulrtest);
   ~AIRTestSendThread();

   virtual unsigned long threadProc(void *arg);

private:
   AIRTestThread &m_airtest;
};

////////////////////////////////////////////////////////////////////////////////

class AIRTestStartMessage : public SEventThreadMessage
{
public:
   AIRTestStartMessage(const std::string &imsi, uint8_t *vplmn, const std::string &host, const std::string &realm, int totalcnt, int sendcnt, long sendms, SSemaphore *testsem)
      : SEventThreadMessage( AIRM_START ),
        m_imsi( imsi ),
        m_host( host ),
        m_realm( realm ),
        m_totalcnt( totalcnt ),
        m_sendcnt( sendcnt ),
        m_sendms( sendms ),
        m_testsem( testsem )
   {
      memcpy(m_vplmn, vplmn, 3);
   }

   ~AIRTestStartMessage()
   {
   }

   std::string &getImsi() { return m_imsi; }
   std::string &getHost() { return m_host; }
   std::string &getRealm() { return m_realm; }
   uint8_t *getVisitedPLMN() { return m_vplmn; }
   int getTotalCount() { return m_totalcnt; }
   int getSendCount() { return m_sendcnt; }
   long getSendMilliseconds() { return m_sendms; }
   SSemaphore *getTestSemaphore() { return m_testsem; }

private:
   std::string m_imsi;
   std::string m_host;
   std::string m_realm;
   uint8_t m_vplmn[3];
   int m_totalcnt;
   int m_sendcnt;
   long m_sendms;
   SSemaphore *m_testsem;
};

////////////////////////////////////////////////////////////////////////////////

class AIRTestThread : public SEventThread
{
public:
   AIRTestThread(HSSPerformance &hssperf);
   ~AIRTestThread();

   virtual void dispatch(SEventThreadMessage &msg);
   virtual void onTimer(SEventThread::Timer &ptimer);
   virtual void onInit();

   void start(AIRTestStartMessage &msg);
   void sendSet();
   bool send();
   void receive(AIRTracker &result);
   void printStatus();
   void printStatistics();

private:
   HSSPerformance &m_hssperf;
   uint64_t m_imsi;
   uint8_t m_vplmn[3];
   std::string m_host;
   std::string m_realm;
   int m_totalcnt;
   int m_sendcnt;
   long m_sendms;

   unsigned long m_sent;
   unsigned long m_rcvd;
   AIRTracker *m_results;
   STimerElapsed m_timer;
   SEventThread::Timer m_sndtimer;
   SSemaphore *m_testsem;
   int m_printcnt;
   STimerElapsed m_rcvtimer;
};

#endif
