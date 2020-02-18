/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __ULRTEST_H
#define __ULRTEST_H

#include "satomic.h"
#include "ssync.h"
#include "stimer.h"
#include "sthread.h"

#include "hssperf.h"

struct ULRTracker
{
   ULRTracker()
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

class ULRTest
{
public:
   ULRTest(HSSPerformance &hssperf);
   ~ULRTest();

   bool startTest(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, int max_outstanding, int count);
   void stopTest();
   bool sendULR();
   bool recvULA(ULRTracker &result, bool success);

   int getCount() { return m_count; }
   int getSent() { return m_sent; }
   int getReceived() { return m_rcvd; }
   ULRTracker &getResult(int ofs) { return m_results[ofs]; }
   SSemaphore &getSemaphore() { return m_teststarted; }

   void printStatus();
   void printStatistics();

private:
   ULRTest();

   HSSPerformance &m_hssperf;
   SMutex m_mutex;
   uint64_t m_imsi;
   uint8_t m_vplmnid[3];
   std::string m_host;
   std::string m_realm;
   int m_concurrent;
   int m_count;
   int m_sent;
   int m_rcvd;
   SSemaphore *m_sendsem;
   ULRTracker *m_results;
   STimerElapsed m_timer;
   SSemaphore m_teststarted;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define ULRM_START   (ETM_USER + 1)

class ULRTestThread;

class ULRTestSendThread : public SThread
{
public:
   ULRTestSendThread(ULRTestThread &ulrtest);
   ~ULRTestSendThread();

   virtual unsigned long threadProc(void *arg);

private:
   ULRTestThread &m_ulrtest;
};

////////////////////////////////////////////////////////////////////////////////

class ULRTestStartMessage : public SEventThreadMessage
{
public:
   ULRTestStartMessage(const std::string &imsi, uint8_t *vplmn, const std::string &host, const std::string &realm, int totalcnt, int sendcnt, long sendms, SSemaphore *testsem)
      : SEventThreadMessage( ULRM_START ),
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

   ~ULRTestStartMessage()
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

class ULRTestThread : public SEventThread
{
public:
   ULRTestThread(HSSPerformance &hssperf);
   ~ULRTestThread();

   virtual void dispatch(SEventThreadMessage &msg);
   virtual void onTimer(SEventThread::Timer &ptimer);
   virtual void onInit();

   void start(ULRTestStartMessage &msg);
   void sendSet();
   bool send();
   void receive(ULRTracker &result);
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
   ULRTracker *m_results;
   STimerElapsed m_timer;
   SEventThread::Timer m_sndtimer;
   SSemaphore *m_testsem;
   int m_printcnt;
   STimerElapsed m_rcvtimer;
};

#endif
