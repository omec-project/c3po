/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __ATTACHTEST_H
#define __ATTACHTEST_H

#include "satomic.h"
#include "ssync.h"
#include "stimer.h"
#include "sthread.h"

#include "hssperf.h"

struct AttachTracker
{
   AttachTracker()
   {
      rcvdcnt = 0;
   }

   STimerElapsed timer;
   unsigned long rcvdcnt;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define ATTACHM_START   (ETM_USER + 3)

class AttachTestThread;

class AttachTestSendThread : public SThread
{
public:
   AttachTestSendThread(AttachTestThread &attachtest);
   ~AttachTestSendThread();

   virtual unsigned long threadProc(void *arg);

private:
   AttachTestThread &m_attachtest;
};

////////////////////////////////////////////////////////////////////////////////

class AttachTestStartMessage : public SEventThreadMessage
{
public:
   AttachTestStartMessage(const std::string &imsi, uint8_t *vplmn, const std::string &host, const std::string &realm, int totalcnt, int sendcnt, long sendms, SSemaphore *testsem)
      : SEventThreadMessage( ATTACHM_START ),
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

   ~AttachTestStartMessage()
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

class AttachTestThread : public SEventThread
{
public:
   AttachTestThread(HSSPerformance &hssperf);
   ~AttachTestThread();

   virtual void dispatch(SEventThreadMessage &msg);
   virtual void onTimer(SEventThread::Timer &ptimer);
   virtual void onInit();

   void start(AttachTestStartMessage &msg);
   void sendSet();
   bool send();
   void receive(AttachTracker &result);
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
   AttachTracker *m_results;
   STimerElapsed m_timer;
   SEventThread::Timer m_sndtimer;
   SSemaphore *m_testsem;
   int m_printcnt;
   STimerElapsed m_rcvtimer;
};

#endif
