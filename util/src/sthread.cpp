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

#include "sthread.h"
#include "serror.h"
#include "satomic.h"

#include <sched.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SThread::SThread( bool selfDestruct )
   : mSuspended(0, 1000), mSuspendSem(0, 1)
{
   mSelfDestruct = selfDestruct;
   mState = SThread::rsWaitingToRun;
   mSuspendCnt = 1;
   mArg = NULL;
   mExitCode = 0;
   mKeepGoing = true;
   mUpdateStateManually = false;

   mInitialized = false;
}

SThread::~SThread()
{
   if (isRunning())
   {
      _shutdown();
      join();
   }
}

void SThread::_shutdown()
{
   SMutexLock l(mMutex);

   if (isRunning())
   {
      suspend();
      shutdown();
      resume();
   }
}

void SThread::init(void *arg, bool suspended)
{
   {
      SMutexLock l(mMutex);

      if (isInitialized())
         SError::throwRuntimeExceptionWithErrno("Thread already initialized");

      mArg = arg;

      pthread_attr_t attr;

      pthread_attr_init(&attr);
      pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

      if (pthread_create(&mThread, &attr, _threadProc, (void *)this) != 0)
         SError::throwRuntimeExceptionWithErrno("Error initializing thread");

      mInitialized = true;

      if (suspended)
         mSuspendCnt++;

      resume();
   }

   while (mSuspendCnt == 0 && isWaitingToRun())
      yield();
}

bool SThread::isInitialized()
{
   return mInitialized;
}

void SThread::resume()
{
   if (!isInitialized())
      SError::throwRuntimeException("The semaphore is not initialized");

   {
      if (mSuspendCnt > 0)
      {
         mSuspendCnt--;
         if (mSuspendCnt == 0)
         {
            mSuspendSem.increment();
         }
      }
   }

   yield();
}

void SThread::suspend()
{
   int suspendCnt;

   {
      SMutexLock l(mMutex);
      mSuspendCnt++;
      suspendCnt = mSuspendCnt;
   }

   if (suspendCnt == 1)
      mSuspended.decrement();
}

void SThread::_suspend()
{
   mSuspended.increment();
   mSuspendSem.decrement();
}

void SThread::join()
{
   {
      SMutexLock l(mMutex);
      if (!isRunning() && !isSuspended())
         return;
   }

   void *value;
   pthread_join(mThread, &value);
}

void SThread::sleep(int milliseconds)
{
   timespec tmReq;

   tmReq.tv_sec = (time_t)(milliseconds / 1000);
   tmReq.tv_nsec = (milliseconds % 1000) * 1000 * 1000;

   // we're not interested in remaining time nor in return value
   (void)nanosleep(&tmReq, (timespec*)NULL);
}

void SThread::yield()
{
   sched_yield();
}

void *SThread::_threadProc(void *arg)
{
   SThread *ths = (SThread*)arg;

   // do not continue until resumed
   ths->mSuspendSem.decrement();

   // set to running state
   {
      SMutexLock l(ths->mMutex);
      if (!ths->getUpdateStateManually())
         ths->mState = SThread::rsRunning;
   }

   unsigned long ret = ths->threadProc(ths->mArg);

   // set to not running state
   {
      SMutexLock l(ths->mMutex);
      if (!ths->getUpdateStateManually())
         ths->mState = SThread::rsDoneRunning;
   }

   if ( ths->mSelfDestruct )
   {
      pthread_t t = ths->mThread;
      delete ths;
      pthread_detach(t);
   }

   return (void *)ret;
}

extern "C" void _UserSignal1Handler(int)
{
}

void SThread::cancelWait()
{
   mKeepGoing = false;

   signal(SIGUSR1, _UserSignal1Handler);
   pthread_kill(mThread, SIGUSR1);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


SEventThread::SEventThread( bool selfDestruct )
   : SThread( selfDestruct )
{
}

SEventThread::~SEventThread()
{
}

void SEventThread::init( void *arg, bool suspended )
{
   SThread::init( arg, suspended );
   postMessage( ETM_INIT );
}

void SEventThread::postMessage( uint16_t msg )
{
   postMessage( new SEventThreadMessage( msg ) );
}

void SEventThread::postMessage( SEventThreadMessage *msg )
{
   m_events.push( msg );
}

void SEventThread::quit()
{
   postMessage( ETM_QUIT );
}

void SEventThread::suspend()
{
   postMessage( ETM_SUSPEND );
   SThread::suspend();
}

void SEventThread::initTimer( SEventThread::Timer &t )
{
   t.init( this );
}

void SEventThread::onInit()
{
}

void SEventThread::onQuit()
{
}

void SEventThread::onSuspend()
{
   _suspend();
}

void SEventThread::onTimer(SEventThread::Timer &ptimer)
{
}

unsigned long SEventThread::threadProc( void *arg )
{
   dispatch();
   return 0;
}

void SEventThread::dispatch()
{
   bool done = false;

   while ( !done )
   {
     SEventThreadMessage *m = (SEventThreadMessage*)m_events.pop();

     if ( m )
     {
       switch ( m->getId() )
       {
         case ETM_INIT:
            onInit();
            break;
         case ETM_QUIT:
            done = true;
            onQuit();
            break;
         case ETM_SUSPEND:
            onSuspend();
            break;
         case ETM_TIMER:
            onTimer( *((STimerMessage*)m)->getTimer() );
            break;
         default:
            dispatch( *m );
            break;
       }

       delete m;
     }
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

long SEventThread::Timer::m_nextid = 0;

SEventThread::Timer::Timer()
{
   // assign the id
   m_id = atomic_inc_fetch(m_nextid);
   m_thread = NULL;
   m_interval = 0;
   m_oneshot = true;

   m_timer = NULL;
}

SEventThread::Timer::Timer(long milliseconds, bool oneshot)
{
   // assign the id
   m_id = atomic_inc_fetch(m_nextid);
   m_thread = NULL;
   m_interval = milliseconds;
   m_oneshot = oneshot;

   m_timer = NULL;
}

SEventThread::Timer::~Timer()
{
   destroy();
}

void SEventThread::Timer::init(SEventThread* pThread)
{
   destroy();

   m_thread = pThread;

   struct sigevent sev;
   sev.sigev_notify = SIGEV_SIGNAL;
   sev.sigev_signo = SIGRTMIN;
   sev.sigev_value.sival_ptr = this;
   if (timer_create(CLOCK_REALTIME, &sev, &m_timer) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to initialize timer" );
}

void SEventThread::Timer::destroy()
{
   if (m_timer != NULL)
   {
      stop();
      timer_delete(m_timer);
      m_timer = NULL;
   }
}

void SEventThread::Timer::start()
{
//std::cout << "SEventThread::Timer::start() 1" << std::endl;
   if (m_timer == NULL)
      SError::throwRuntimeException( "Timer is not initialized" );

   struct itimerspec its;
   its.it_value.tv_sec = m_interval / 1000; // seconds
   its.it_value.tv_nsec = (m_interval % 1000) * 1000000; // nano-seconds
   its.it_interval.tv_sec = m_oneshot ? 0 : its.it_value.tv_sec;
   its.it_interval.tv_nsec = m_oneshot ? 0 : its.it_value.tv_nsec;
//std::cout << "SEventThread::Timer::start() 2 - value.tv_sec=" << its.it_value.tv_sec << " value.tv_nsec=" << its.it_value.tv_nsec << " interval.tv_sec=" << its.it_interval.tv_sec << " interval.tv_nsec=" << its.it_interval.tv_nsec << std::endl;
   if (timer_settime(m_timer, 0, &its, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to start timer" );
//std::cout << "SEventThread::Timer::start() 3" << std::endl;
}

void SEventThread::Timer::stop()
{
   if (m_timer != NULL)
   {
      struct itimerspec its;
      its.it_value.tv_sec = 0; // seconds
      its.it_value.tv_nsec = 0; // nano-seconds
      its.it_interval.tv_sec = its.it_value.tv_sec;
      its.it_interval.tv_nsec = its.it_value.tv_nsec;
      timer_settime(m_timer, 0, &its, NULL);
   }
}

void SEventThread::Timer::_timerHandler(int signo, siginfo_t *pinfo, void *pcontext)
{
//std::cout << "SEventThread::Timer::_timerHandler() 1" << std::endl;
   SEventThread::Timer* pTimer = (SEventThread::Timer*)pinfo->si_value.sival_ptr;
   if (pTimer)
   {
//std::cout << "SEventThread::Timer::_timerHandler() 2" << std::endl;
      pTimer->m_thread->postMessage( new STimerMessage( pTimer ) );
   }
}

SEventThread::TimerHandler _initTimerHandler;

void SEventThread::TimerHandler::init()
{
//std::cout << "SEventThread::TimerHandler::init()" << std::endl;
   struct sigaction sa;
   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = SEventThread::Timer::_timerHandler;
   sigemptyset(&sa.sa_mask);
   int signo = SIGRTMIN;
   if (sigaction(signo, &sa, NULL) == -1)
      SError::throwRuntimeExceptionWithErrno( "Unable to register timer handler" );
}

void SEventThread::TimerHandler::uninit()
{
//std::cout << "SEventThread::TimerHandler::uninit()" << std::endl;
}
