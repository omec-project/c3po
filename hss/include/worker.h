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

#ifndef __WORKER_H
#define __WORKER_H

#include <queue>

#include "ssync.h"
#include "squeue.h"
#include "sthread.h"
#include "scassandra.h"

#define WORKER_SHUTDOWN  99
#define WORKER_EVENT    100

class WorkerMessage;

class WorkerManager
{
public:
   WorkerManager();
   ~WorkerManager();

   bool init(int numWorkers);

   bool addWork(WorkerMessage *msg);

   WorkerMessage *getWork();

   void waitForShutdown();

   void threadShutdown();

private:
   SMutex m_mutex;
   SEvent m_shutdown;
   SQueue m_queue;
   int m_numWorkers;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class WorkProcessor
{
public:
   WorkProcessor() {}
   virtual ~WorkProcessor() {}

   virtual void process() = 0;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class WorkerMessage : public SQueueMessage
{
public:
   WorkerMessage(uint16_t id, WorkProcessor *processor);
   WorkerMessage(uint16_t id);
   virtual ~WorkerMessage();

   WorkProcessor *getProcessor() { return m_processor; }

private:
   WorkerMessage();

   WorkProcessor *m_processor;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class WorkerThread : public SThread
{
public:
   WorkerThread(WorkerManager &mgr);
   ~WorkerThread();

   unsigned long threadProc(void *arg);

private:
   WorkerThread();

   WorkerManager &m_mgr;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class QueueManager
{
public:
   QueueManager()
      : m_concurrent(10),
        m_active(0),
        m_pending(0)
   {
   }

   ~QueueManager()
   {
   }

   int setConcurrent(int concurrent)
   {
      return m_concurrent = concurrent;
   }

   size_t queueDepth()
   {
      return m_queue.size();
   }

protected:
   void addEntry(void *data)
   {
      SMutexLock l(m_mutex);
      m_queue.push(data);
      m_pending++;
   }

   void *startMessage()
   {
      void *data = NULL;

      SMutexLock l(m_mutex);
      if (m_active < m_concurrent && m_pending > 0)
      {
         data = m_queue.front();
         m_queue.pop();
         m_pending--;
         m_active++;
      }

      return data;
   }

   void finishMessage()
   {
      SMutexLock l(m_mutex);
      m_active--;
   }

private:
   SMutex m_mutex;
   std::queue<void*> m_queue;
   int m_concurrent;
   int m_active;
   int m_pending;
};

class QueueProcessor
{
public:
   QueueProcessor() {}
   ~QueueProcessor() {}

   virtual void triggerNextPhase() = 0;
};

#endif
