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

#ifndef __STHREAD_H
#define __STHREAD_H

#include <signal.h>

#include "ssync.h"
#include "squeue.h"

class SThread
{
public:
   enum RunState
   {
    rsWaitingToRun,
    rsRunning,
    rsDoneRunning
   };

   SThread( bool selfDestruct = false );
   virtual ~SThread();

   virtual unsigned long threadProc(void *arg) = 0;

   void init(void *arg, bool suspended = false);
   void resume();
   void join();

   static void sleep(int milliseconds);
   static void yield();

   bool isInitialized();
   bool isSuspended()
   {
    return mSuspendCnt == 0 ? false : true;
   }
   void setRunState(RunState state) { mState = state; }

   RunState getRunState() { return mState; }

   bool isWaitingToRun()   { return mState == SThread::rsWaitingToRun; }
   bool isRunning()    { return mState == SThread::rsRunning; }
   bool isDoneRunning()   { return mState == SThread::rsDoneRunning; }

   void setUpdateStateManually(bool val)   { mUpdateStateManually = val; }
   bool getUpdateStateManually()      { return mUpdateStateManually; }

   bool keepGoing()    { return mKeepGoing; }

   virtual void shutdown()
   {
    // This method is called in the event of the object being destroyed
    // while the thread is running.  This is intended for use for threads
    // that do not support a message queue (FTTM_QUIT will be issued) to
    // inform the thread that it needs to exit.
   }

   void cancelWait();

protected:

   virtual void suspend();
   void _suspend();

private:
   bool mSelfDestruct;
   bool mInitialized;
   pthread_t mThread;
   static void *_threadProc(void *arg);
   void _shutdown();

   SMutex mMutex;
   RunState mState;
   bool mKeepGoing;
   bool mUpdateStateManually;
   SSemaphore mSuspended;
   SSemaphore mSuspendSem;
   int mSuspendCnt;
   void *mArg;
   unsigned long mExitCode;
};

const uint16_t ETM_INIT    = 1;
const uint16_t ETM_QUIT    = 2;
const uint16_t ETM_SUSPEND = 3;
const uint16_t ETM_TIMER   = 4;
const uint16_t ETM_USER    = 10000;

class SEventThreadMessage : public SQueueMessage
{
public:
   SEventThreadMessage( uint16_t id ) : SQueueMessage( id ) { }
   virtual ~SEventThreadMessage() { }
};

class SEventThread : public SThread
{
public:

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   class Timer
   {
      friend class SEventThread;

   protected:
      void init(SEventThread* pThread);

   public:
      Timer();
      Timer(long milliseconds, bool oneshot=false);
      ~Timer();

      void destroy();
      void start();
      void stop();

      void setInterval(long interval) { m_interval = interval; }
      void setOneShot(bool oneshot) { m_oneshot = oneshot; }
      long getId() { return m_id; }

   private:
      static long m_nextid;

      long m_id;
      SEventThread* m_thread;
      bool m_oneshot;
      long m_interval;
      timer_t m_timer;
      static void _timerHandler(int signo, siginfo_t *pinfo, void *pcontext);
   };

   class TimerHandler
   {
   public:
      TimerHandler() { init(); }
      ~TimerHandler() { uninit(); }

      void init();
      void uninit();
   };

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   SEventThread( bool selfDestruct = false );
   virtual ~SEventThread();

   //
   // called after this object is constructed
   //
   void init( void *arg, bool suspended = false );

   //
   // these methods can be called from this thread or another
   //
   void postMessage(uint16_t message);
   void postMessage(SEventThreadMessage *msg);

   void quit();
   void suspend();

   void initTimer( SEventThread::Timer &t );

   //
   // these methods are executed by the SEventThread internals in the thread context
   //
   virtual void dispatch( SEventThreadMessage &msg ) = 0;

   virtual void onInit();
   virtual void onQuit();
   virtual void onSuspend();
   virtual void onTimer(SEventThread::Timer &ptimer);

protected:

private:
   unsigned long threadProc( void *arg );
   void dispatch();

   static TimerHandler m_th;
   SQueue m_events;
};

class STimerMessage : public SEventThreadMessage
{
public:
   STimerMessage( SEventThread::Timer *timer ) : SEventThreadMessage( ETM_TIMER ), m_timer( timer ) {}

   SEventThread::Timer *getTimer() { return m_timer; }

private:
   SEventThread::Timer *m_timer;
};

#endif // #define __STHREAD_H
