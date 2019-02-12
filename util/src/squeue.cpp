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
