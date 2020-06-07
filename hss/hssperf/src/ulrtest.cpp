/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <cstdio>
#include <cinttypes>

#include "logger.h"
#include "ulrtest.h"

extern bool gShutdown;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ULRTest::ULRTest(HSSPerformance &hssperf)
   : m_hssperf(hssperf),
     m_imsi( 0 ),
     m_concurrent( 0 ),
     m_count( 0 ),
     m_sent( 0 ),
     m_rcvd( 0 ),
     m_sendsem( NULL ),
     m_results( NULL ),
     m_teststarted(0,1)
{
}

ULRTest::~ULRTest()
{
   if (m_sendsem)
      delete m_sendsem;
   if (m_results)
      delete [] m_results;
}

bool ULRTest::startTest(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, int concurrent, int count)
{
   sscanf(imsi, "%" SCNu64, &m_imsi);
   memcpy(m_vplmnid, vplmnid, 3);
   m_host = host;
   m_realm = realm;
   m_concurrent = concurrent;
   m_count = count;
   m_sent = 0;
   m_rcvd = 0;

   if (m_sendsem)
      delete m_sendsem;

   m_sendsem = new SSemaphore(concurrent, concurrent);
   if (!m_sendsem)
   {
      Logger::s6as6d().error("Fatal error creating semaphore");
      return false;
   }

   if (m_results)
      delete [] m_results;

   m_results = new ULRTracker[m_count];
   if (!m_results)
   {
      Logger::s6as6d().error("Unable to allocate ULRTracker array with %d elements", m_count);
      return false;
   }

   m_timer.Start();

   for (int i=0; i<m_concurrent; i++)
   {
      if (!sendULR())
         return false;
   }

   return true;
}

void ULRTest::stopTest()
{
   m_timer.Stop();
   m_teststarted.increment();
}

bool ULRTest::sendULR()
{
   int sent, rcvd;

   m_sendsem->decrement(true);

   {
      SMutexLock l(m_mutex);
      if (m_sent >= m_count)
      {
         m_sendsem->increment();
         return false;
      }

      char imsi[16];

      sprintf(imsi, "%015" SCNu64, m_imsi + m_sent);

      if (!m_hssperf.s6as6dApp().sendULRreq(imsi, m_vplmnid, m_host.c_str(), m_realm.c_str(),*this,m_results[m_sent]))
      {
         m_sendsem->increment();
         return false;
      }

      m_results[m_sent].sent = true;
      m_results[m_sent].timer.Start();

      m_sent++;
   }

   return true;
}

bool ULRTest::recvULA(ULRTracker &result, bool succeeded)
{
   uint64_t uimsi;
   int ofs;
   bool sendulr = false;

   result.timer.Stop();
   result.rcvd = true;

   m_sendsem->increment();

   {
      SMutexLock l(m_mutex);
      m_rcvd++;
      sendulr = m_rcvd < m_count;
   }

   bool res = true;

   if (sendulr)
      res = sendULR();

   printStatus();

   return res;
}

void ULRTest::printStatus()
{
   if ((m_sent != m_count && m_sent % 1000 == 0) || (m_sent == m_count && (m_rcvd == m_count || m_rcvd % 1000 == 0)))
   {
      printf("\rsent %d received %d", m_sent, m_rcvd);
      if (m_sent == m_count && m_rcvd == m_count)
      {
         stopTest();
         printf(" - Test Complete\n");
        printStatistics();
      }
      fflush(stdout);
   }
}

void ULRTest::printStatistics()
{
   uint64_t
      min = 1000000LL,
      max = 0,
      us = 0,
      et,
      avg;

   for (int i=0; i<m_count; i++)
   {
      et = m_results[i].timer.MicroSeconds();
      us += et;
      if (et < min)
         min = et;
      if (et > max)
         max = et;
   }

   avg = us / m_count;

   printf("\n");
   printf("Number of Transactions %10d\n", m_count);
   printf("Average Duration (us)  %10" SCNu64 "\n", avg);
   printf("Min Duration (us)      %10" SCNu64 "\n", min);
   printf("Max Duration (us)      %10" SCNu64 "\n", max);
   printf("Total Elapsed Time (us)%10lld\n", m_timer.MicroSeconds());
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ULRTestSendThread::ULRTestSendThread(ULRTestThread &ulrtest)
   : SThread(true),
     m_ulrtest(ulrtest)
{
}

ULRTestSendThread::~ULRTestSendThread()
{
}

unsigned long ULRTestSendThread::threadProc(void *arg)
{
   m_ulrtest.sendSet();
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

ULRTestThread::ULRTestThread(HSSPerformance &hssperf)
   : m_hssperf(hssperf)
{
   m_results = NULL;
}

ULRTestThread::~ULRTestThread()
{
}

void ULRTestThread::onInit()
{
   initTimer(m_sndtimer);
}

void ULRTestThread::dispatch(SEventThreadMessage &msg)
{
   switch (msg.getId())
   {
      case ULRM_START:
      {
         start((ULRTestStartMessage&)msg);
         break;
      }
   }
}

void ULRTestThread::onTimer(SEventThread::Timer &ptimer)
{
   if (ptimer.getId() == m_sndtimer.getId())
   {
      ULRTestSendThread *t = new ULRTestSendThread(*this);
      t->init(NULL);
   }
}

void ULRTestThread::start(ULRTestStartMessage &msg)
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

   m_results = new ULRTracker[m_totalcnt];
   if (!m_results)
   {
      Logger::s6as6d().error("Unable to allocate ULRTracker array with %d elements", m_totalcnt);
      return;
   }

   m_sndtimer.setInterval(m_sendms);
   m_sndtimer.setOneShot(false);
   m_sndtimer.start();
   m_rcvtimer.Start();
}

void ULRTestThread::sendSet()
{
   for (int i=0; i<m_sendcnt; i++)
   {
      if (!send())
         break;
   }
}

bool ULRTestThread::send()
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

   if (!m_hssperf.s6as6dApp().sendULRreq(imsi, m_vplmn, m_host.c_str(), m_realm.c_str(),*this, m_results[sent]))
   {
      atomic_dec_fetch(m_sent);
      return false;
   }

   m_results[sent].sent = true;
   m_results[sent].timer.Start();

   if (sent == 0) // is first
      m_timer.Start();

   return true;
}

void ULRTestThread::receive(ULRTracker &result)
{
   result.timer.Stop();
   result.rcvd = true;

   unsigned long cnt = atomic_inc_fetch(m_rcvd);

   if (cnt == m_totalcnt)
      m_timer.Stop();

   if (cnt % 1000 == 0 || cnt == m_totalcnt|| (gShutdown && m_rcvd == m_sent))
      printStatus();
}

void ULRTestThread::printStatus()
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

void ULRTestThread::printStatistics()
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
   printf("ULR TEST\n");
   printf("Number of Transactions %10d\n", m_totalcnt);
   printf("Rate per Second        %10ld\n", (1000/m_sendms)*m_sendcnt);
   printf("Average Duration (us)  %10" SCNu64 "\n", avg);
   printf("Min Duration (us)      %10" SCNu64 "\n", min);
   printf("Max Duration (us)      %10" SCNu64 "\n", max);
   printf("Total Elapsed Time (us)%10lld\n", m_timer.MicroSeconds());
}
