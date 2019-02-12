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

#include "worker.h"
#include "logger.h"

WorkerManager::WorkerManager()
{
}

WorkerManager::~WorkerManager()
{
}

bool WorkerManager::init(int numWorkers)
{
   for (int i=0; i<numWorkers; i++)
   {
      WorkerThread *wt = new WorkerThread(*this);
      if (wt)
      {
         wt->init(NULL);
         m_numWorkers++;
      }
      else
      {
         Logger::system().error("Unable to allocate worker %d", i);
         return false;
      }
   }

   return true;
}

bool WorkerManager::addWork(WorkerMessage *msg)
{
   return m_queue.push(msg);
}

WorkerMessage *WorkerManager::getWork()
{
   return (WorkerMessage*)m_queue.pop();
}

void WorkerManager::waitForShutdown()
{
   if (m_numWorkers == 0)
   {
      m_shutdown.set();
      return;
   }

   for (int i=0; i<m_numWorkers; i++)
      m_queue.push(new WorkerMessage(WORKER_SHUTDOWN));

   m_shutdown.wait();
}

void WorkerManager::threadShutdown()
{
   SMutexLock l(m_mutex);
   m_numWorkers--;
   if (m_numWorkers <= 0)
      m_shutdown.set();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

WorkerMessage::WorkerMessage(uint16_t id, WorkProcessor *processor)
   : SQueueMessage( id ),
     m_processor( processor )
{
}

WorkerMessage::WorkerMessage(uint16_t id)
   : SQueueMessage( id ),
     m_processor( NULL )
{
}

WorkerMessage::~WorkerMessage()
{
   if (m_processor)
      delete m_processor;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

WorkerThread::WorkerThread(WorkerManager &mgr)
   : SThread( true ),
     m_mgr( mgr )
{
}

WorkerThread::~WorkerThread()
{
}

unsigned long WorkerThread::threadProc(void *arg)
{
   WorkerMessage *msg;

   for(;;)
   {
      msg = m_mgr.getWork();

      if (msg->getId() == WORKER_SHUTDOWN)
      {
         delete msg;
         break;
      }
      else if (msg->getId() == WORKER_EVENT)
      {
         if (msg->getProcessor())
            msg->getProcessor()->process();
      }
      else
      {
         Logger::system().warn("Unrecognized worker event (%d)", msg->getId());
      }

      delete msg;
   }

   m_mgr.threadShutdown();
   return 0;
}
