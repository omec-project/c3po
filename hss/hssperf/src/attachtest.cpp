/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <cstdio>
#include <cinttypes>

#include "logger.h"
#include "attachtest.h"

extern bool gShutdown;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

AttachTestSendThread::AttachTestSendThread(AttachTestThread &attachtest)
   : SThread(true),
     m_attachtest(attachtest)
{
}

AttachTestSendThread::~AttachTestSendThread()
{
}

unsigned long AttachTestSendThread::threadProc(void *arg)
{
   m_attachtest.sendSet();
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

AttachTestThread::AttachTestThread(HSSPerformance &hssperf)
   : m_hssperf(hssperf)
{
   m_results = NULL;
}

AttachTestThread::~AttachTestThread()
{
}

void AttachTestThread::onInit()
{
   initTimer(m_sndtimer);
}

void AttachTestThread::dispatch(SEventThreadMessage &msg)
{
   switch (msg.getId())
   {
      case ATTACHM_START:
      {
         start((AttachTestStartMessage&)msg);
         break;
      }
   }
}

void AttachTestThread::onTimer(SEventThread::Timer &ptimer)
{
   if (ptimer.getId() == m_sndtimer.getId())
   {
      AttachTestSendThread *t = new AttachTestSendThread(*this);
      t->init(NULL);
   }
}

void AttachTestThread::start(AttachTestStartMessage &msg)
{
   sscanf(msg.getImsi().c_str(), "%" SCNu64, &m_imsi);
   memcpy(m_vplmn, msg.getVisitedPLMN(), 3);
   m_host = msg.getHost();
   m_realm = msg.getRealm();
   m_totalcnt = msg.getTotalCount();
   m_sendcnt = msg.getSendCount();
   m_sendms = msg.getSendMilliseconds();
   m_testsem = msg.getTestSemaphore();

   m_printcnt = 0;
   m_sent = 0;
   m_rcvd = 0;

   if (m_results)
      delete [] m_results;

   m_results = new AttachTracker[m_totalcnt];
   if (!m_results)
   {
      Logger::s6as6d().error("Unable to allocate AttachTracker array with %d elements", m_totalcnt);
      return;
   }

   m_sndtimer.setInterval(m_sendms);
   m_sndtimer.setOneShot(false);
   m_sndtimer.start();
   m_rcvtimer.Start();
}

void AttachTestThread::sendSet()
{
   for (int i=0; i<m_sendcnt; i++)
   {
      if (!send())
         break;
   }
}

bool AttachTestThread::send()
{
   if (gShutdown)
   {
      m_sndtimer.stop();
      return false;
   }

   unsigned long sent = atomic_inc_fetch(m_sent);

   if (sent > m_totalcnt)
   {
      atomic_dec_fetch(m_sent);
      m_sndtimer.stop();
      return false;
   }

   sent--;

   char imsi[16];

   sprintf(imsi, "%015" SCNu64, m_imsi + sent);

   if (!m_hssperf.s6as6dApp().sendAIRreq(imsi, m_vplmn, m_host.c_str(), m_realm.c_str(), *this, m_results[sent]))
   {
      atomic_dec_fetch(m_sent);
      return false;
   }
   if (!m_hssperf.s6as6dApp().sendULRreq(imsi, m_vplmn, m_host.c_str(), m_realm.c_str(), *this, m_results[sent]))
   {
      atomic_dec_fetch(m_sent);
      return false;
   }

   m_results[sent].timer.Start();

   if (sent == 0) // is first
      m_timer.Start();

   return true;
}

void AttachTestThread::receive(AttachTracker &result)
{
   unsigned long rcvdcnt = atomic_inc_fetch(result.rcvdcnt);

   if (rcvdcnt < 2)
      return;

   result.timer.Stop();

   unsigned long cnt = atomic_inc_fetch(m_rcvd);

   if (cnt == m_totalcnt)
      m_timer.Stop();

   if (cnt % 1000 == 0 || cnt == m_totalcnt || (gShutdown && m_rcvd == m_sent))
      printStatus();
}

void AttachTestThread::printStatus()
{
   printf("sent %lu received %lu elapsed (ms) %lld\n", m_sent, m_rcvd, m_rcvtimer.MilliSeconds());
   m_rcvtimer.Start();
   if (m_rcvd == m_totalcnt || (gShutdown && m_rcvd == m_sent))
   {
      m_sndtimer.stop();
      printf(" - Test Complete\n");
      printStatistics();
      if (m_testsem)
         m_testsem->increment();
   }
   fflush(stdout);
}

void AttachTestThread::printStatistics()
{
   uint64_t
      min = 1000000LLU,
      max = 0,
      us = 0,
      et,
      avg;

   for (int i=0; i<m_sent; i++)
   {
      et = m_results[i].timer.MicroSeconds();
      us += et;
      if (et < min)
         min = et;
      if (et > max)
         max = et;
   }

   avg = us / m_sent;

   printf("\n");
   printf("ATTACH TEST\n");
   printf("Number of Transactions %10d\n", m_totalcnt);
   printf("Rate per Second        %10ld\n", (1000/m_sendms)*m_sendcnt);
   printf("Average Duration (us)  %10" SCNu64 "\n", avg);
   printf("Min Duration (us)      %10" SCNu64 "\n", min);
   printf("Max Duration (us)      %10" SCNu64 "\n", max);
   printf("Total Elapsed Time (us)%10lld\n", m_timer.MicroSeconds());
}
