/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
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
