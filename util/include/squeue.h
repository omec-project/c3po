/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SQUEUE_H
#define __SQUEUE_H

#include <queue>

#include "ssync.h"

class SQueueMessage
{
public:
   SQueueMessage( uint16_t id );
   virtual ~SQueueMessage();

   uint16_t getId() { return m_id; }
   uint16_t setId( uint16_t id ) { return m_id = id; }

private:
   SQueueMessage();

   uint16_t m_id;
};

class SQueue
{
public:
   SQueue();
   ~SQueue();

   bool push( uint16_t msgid, bool wait = true );
   bool push( SQueueMessage *msg, bool wait = true );

   SQueueMessage *pop( bool wait = true );

private:
   SMutex m_mutex;
   SSemaphore m_sem;
   std::queue<SQueueMessage*> m_queue;
};

#endif // #define __SQUEUE_H
