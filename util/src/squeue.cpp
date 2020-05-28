/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <climits>

#include "squeue.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SQueueMessage::SQueueMessage( uint16_t id )
{
   m_id = id;
}

SQueueMessage::~SQueueMessage()
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SQueue::SQueue()
{
   //m_sem.init( 0, SEM_VALUE_MAX );
   m_sem.init( 0, 0 );
}

SQueue::~SQueue()
{
   SQueueMessage *m;

   while ( ( m = pop( false ) ) )
      delete m;
}

bool SQueue::push( uint16_t msgid, bool wait )
{
   SQueueMessage *m = new SQueueMessage( msgid );

   bool result = push( m, wait );

   if ( !result )
      delete m;

   return result;
}

bool SQueue::push( SQueueMessage *msg, bool wait )
{
   SMutexLock l( m_mutex, false );

   if ( l.acquire( wait ) )
   {
      m_queue.push( msg );
      m_sem.increment();
      return true;
   }

   return false;
}

SQueueMessage *SQueue::pop( bool wait )
{
   SQueueMessage *msg = NULL;

   if ( m_sem.decrement( wait ) )
   {
      SMutexLock l( m_mutex, false );

      if ( l.acquire( wait ) )
      {
         msg = m_queue.front();
         m_queue.pop();
      }
      else
      {
         // increment the message count since we could not lock the queue
         m_sem.increment();
      }
   }
   
   return msg;
}
