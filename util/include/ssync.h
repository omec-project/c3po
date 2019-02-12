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

#ifndef __SSYNC_H
#define __SSYNC_H

#include <stdint.h>

#include <string>
#include <stdexcept>

#include <semaphore.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SMutexLock;

class SMutex
{
   friend SMutexLock;

public:

    SMutex(bool bInit = true);
    ~SMutex();

    void init(const char *pName);
    void destroy();

protected:
    bool enter(bool wait = true);
    void leave();

private:
    pthread_mutex_t mMutex;
    bool mInitialized;
};

class SMutexLock
{
public:
    SMutexLock(SMutex &mtx, bool acq = true)
        : mAcquire(acq), mMutex(mtx)
    {
        if (mAcquire)
            mMutex.enter();
    }

    ~SMutexLock()
    {
        if (mAcquire)
            mMutex.leave();
    }

    bool acquire(bool wait = true)
    {
        if (!mAcquire)
            mAcquire = mMutex.enter(wait);

        return mAcquire;
    }

private:
    bool mAcquire;
    SMutex &mMutex;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SSemaphore
{
public:
    SSemaphore();
    SSemaphore(unsigned int lInitialCount, unsigned int lMaxCount, const char *pszName = NULL, bool bInit = true);

    ~SSemaphore();

    void init(unsigned int lInitialCount, unsigned int lMaxCount, const char *szName = NULL);
    void destroy();

    bool decrement(bool wait = true);
    bool increment();

    int getInitialCount() { return mInitialCount; }
    int getMaxCount() { return mMaxCount; }
    std::string &getName() { return mName; }

private:
    bool mInitialized;
    unsigned int mCurrCount;

    unsigned int mInitialCount;
    unsigned int mMaxCount;
    std::string mName;

    sem_t mSem;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class SEvent
{
public:
   SEvent( bool state = false );
   ~SEvent();

   void reset();
   void set();
   bool wait( int ms = -1 );

   bool isSet() { return wait(0); }

private:
   void closepipe();

   int m_pipefd[2];
   char m_buf[1];
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#endif //#define __SSYNC_H
