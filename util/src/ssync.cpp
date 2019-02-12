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

#include <errno.h>

#include "ssync.h"
#include "serror.h"

#include <poll.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SMutex::SMutex(bool bInit)
    : mInitialized(false)
{
    if (bInit)
        init(NULL);
}

SMutex::~SMutex()
{
    destroy();
}

void SMutex::init(const char *pName)
{
    if (!mInitialized)
    {
        int res;
        pthread_mutexattr_t attr;

        if ((res = pthread_mutexattr_init(&attr)) != 0)
            SError::throwRuntimeExceptionWithErrno("Unable to initialize mutex");
        else if ((res = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)) != 0)
            SError::throwRuntimeExceptionWithErrno("Unable to initialize mutex");
        else if ((res = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)) != 0)
            SError::throwRuntimeExceptionWithErrno("Unable to initialize mutex");
        else if ((res = pthread_mutex_init(&mMutex, &attr)) != 0)
            SError::throwRuntimeExceptionWithErrno("Unable to initialize mutex");

        mInitialized = true;
    }
}

void SMutex::destroy()
{
    if (mInitialized)
    {
        pthread_mutex_destroy(&mMutex);
        mInitialized = false;
    }
}

bool SMutex::enter(bool wait)
{
    if (!mInitialized)
        SError::throwRuntimeException("SMutex::enter() - SMutex not initialized");

    int res = pthread_mutex_lock(&mMutex);

    if (res != 0 && wait)
        SError::throwRuntimeExceptionWithErrno("SMutex::enter() - Unable to lock mutex", res);

    return res == 0;
}

void SMutex::leave()
{
    if (!mInitialized)
        SError::throwRuntimeException("SMutex::enter() - SMutex not initialized");

    int res = pthread_mutex_unlock(&mMutex);

    if (res != 0)
        SError::throwRuntimeExceptionWithErrno("SMutex::enter() - Unable to unlock mutex", res);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SSemaphore::SSemaphore()
{
    mInitialized = false;
}

SSemaphore::SSemaphore(unsigned int lInitialCount, unsigned int lMaxCount, const char *pszName, bool bInit)
{
    mInitialized = false;

    if (bInit)
        init(lInitialCount, lMaxCount, pszName);
}

SSemaphore::~SSemaphore()
{
    destroy();
}

void SSemaphore::init(unsigned int lInitialCount, unsigned int lMaxCount, const char *pszName)
{
    if (mInitialized)
        SError::throwRuntimeException("Semaphore already initialized");

    if (pszName != NULL)
        mName = pszName;

    mInitialCount = lInitialCount;
    mCurrCount = lInitialCount;
    mMaxCount = lMaxCount;

    if (sem_init(&mSem, (pszName != NULL) ? 1 : 0, lMaxCount) != 0)
        SError::throwRuntimeExceptionWithErrno("Error initializing semaphore");

    for (unsigned int i = lMaxCount; i > lInitialCount; i--)
        sem_wait(&mSem);

    mInitialized = true;
}

void SSemaphore::destroy()
{
    if (mInitialized)
    {
        mInitialCount = 0;
        mCurrCount = 0;
        mMaxCount = 0;

        sem_destroy(&mSem);
        mInitialized = false;
    }
    else
    {
        // not initialized
    }
}

bool SSemaphore::decrement(bool wait)
{
    if (!wait)
    {
        int val = 0;
        if (sem_getvalue(&mSem, &val) != 0)
            SError::throwRuntimeExceptionWithErrno("Error retrieving semaphore value");
        if (val == 0)
            return false;
    }

    while (true)
    {
       if (sem_wait(&mSem) != 0)
       {
          if (errno == 4)
             continue;
          SError::throwRuntimeExceptionWithErrno("Error waiting for semaphore");
       }
       break;
    }

    return true;
}

bool SSemaphore::increment()
{
    if (sem_post(&mSem) != 0)
        SError::throwRuntimeExceptionWithErrno("Error incrementing semaphore");

    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SEvent::SEvent( bool state )
{
   m_pipefd[0] = -1;
   m_pipefd[1] = -1;

   int result = pipe2( m_pipefd, O_NONBLOCK );
   if ( result == -1 )
      SError::throwRuntimeExceptionWithErrno("Error createing pipe (SEvent)");

   if ( state )
      set();
}

SEvent::~SEvent()
{
   closepipe();
}

void SEvent::set()
{
   write( m_pipefd[1], &m_buf, 1 );
}

void SEvent::reset()
{
   while ( read(m_pipefd[0], &m_buf, sizeof(m_buf)) > 0 );
}

bool SEvent::wait( int ms )
{
   bool rval = false;
   struct pollfd fds[] = { { .fd = m_pipefd[0], .events = POLLRDNORM } };

   while (true)
   {
      int result = poll( fds, 1, ms );
      if ( result > 0 ) // event set
      {
         rval = true;
      }
      else if ( result == 0 )
      {
         // timeout
      }
      else
      {
         if (errno == EINTR)
            continue;
      }
      break;
   }

   return rval;
}

void SEvent::closepipe()
{
   if ( m_pipefd[0] != -1 )
   {
      close( m_pipefd[0] );
      m_pipefd[0] = -1;
   }
   if ( m_pipefd[1] != -1 )
   {
      close( m_pipefd[1] );
      m_pipefd[1] = -1;
   }
}
