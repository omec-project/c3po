/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <iostream>

#include "tests.h"

#include "sdir.h"
#include "spath.h"
#include "squeue.h"
#include "sthread.h"
#include "sfile.h"
#include "sutility.h"
#include "csvtrack.h"
#include "fd.h"
#include "rf_impl.h"
#include "stimer.h"
#include "stime.h"
#include "ssyslog.h"

const uint16_t EVENT1 = ETM_USER + 1;
const uint16_t EVENT2 = ETM_USER + 2;

class TestThread : public SEventThread
{
public:
   TestThread() : m_cnt(0) {}
   ~TestThread()
   {
      delete m_t1;
      delete m_t2;
   }

   void dispatch( SEventThreadMessage &msg )
   {
      if ( msg.getId() == EVENT1 )
      {
         std::cout << "received EVENT1" << std::endl;
      }
      else if ( msg.getId() == EVENT2 )
      {
         std::cout << "received EVENT2 - starting timers" << std::endl;
         m_t1->start();
         m_t2->start();
      }
   }

   void onInit()
   {
      m_t1 = new SEventThread::Timer( 1000, false );
      m_t2 = new SEventThread::Timer( 5000, true );

      initTimer( *m_t1 );
      initTimer( *m_t2 );
   }

   void onTimer( SEventThread::Timer &t )
   {
      if ( t.getId() == m_t1->getId() )
      {
         std::cout << "Timer 1 expired - m_cnt=" << ++m_cnt << std::endl;
      }
      else if ( t.getId() == m_t2->getId() )
      {
         std::cout << "Timer 2 expired - triggering exit" << std::endl;
         quit();
      }
   }

private:
   SEventThread::Timer *m_t1;
   SEventThread::Timer *m_t2;
   int m_cnt;
};

bool Tests::syslogTest()
{
   std::string ident("c3poctt");
   SSysLog aLog (ident, (LOG_NDELAY|LOG_PID|LOG_CONS), LOG_USER);

   std::string aLogContent("content log");
   aLog.syslog(LOG_INFO, "%s%s", "This is a sys log ", aLogContent.c_str());

   return true;
}

bool Tests::dateTimeTest()
{
    STime t;

    t.Now();

    std::string s;
    t.Format(s, "%Y-%m-%d %H:%M:%S.%0", false);
    std::cout << s << std::endl;
    t.Format(s, "%Y-%m-%d %H:%M:%S.%0", true);
    std::cout << s << std::endl;

    const char *pszDate;

    pszDate = "2009-12-09 09:12:56.373";
    t.ParseDateTime(pszDate, true);
    t.Format(s, "%Y-%m-%d %H:%M:%S.%0", true);
    std::cout << "Is [" << pszDate << "] equal to [" << s << "]" << std::endl;

    pszDate = "09 Dec 2009 11:42:13:767";
    t.ParseDateTime(pszDate, false);
    t.Format(s, "%Y-%m-%d %H:%M:%S.%0", false);
    std::cout << "Is [" << pszDate << "] equal to [" << s << "]" << std::endl;

    return true;
}

bool Tests::timerTest(){

   int sleeptime = 1000;

   STimerElapsed timerElapsed;
   timerElapsed.Start();
   stime_t aInit = timerElapsed.MicroSeconds();
   SThread::sleep( sleeptime );
   stime_t aEnd = timerElapsed.MicroSeconds();
   assert(aEnd >= aInit + sleeptime * 1000);

   return true;
}

bool Tests::queueTest()
{
   SQueue q;

   std::cout << "pushing 1" << std::endl; q.push( 1 );
   std::cout << "pushing 2" << std::endl; q.push( 2 );
   std::cout << "pushing 3" << std::endl; q.push( 3 );
   std::cout << "pushing 4" << std::endl; q.push( 4 );
   std::cout << "pushing 5" << std::endl; q.push( 5 );

   SQueueMessage *m = NULL;

   while ( ( m = q.pop(false) ) )
   {
      std::cout << "popped " << m->getId() << std::endl;
      delete m;
   }

   return true;
}

bool Tests::threadTest()
{
   TestThread tt;

   tt.init( NULL );
   tt.postMessage( EVENT1 );
   tt.postMessage( EVENT2 );
   tt.join();
   std::cout << "the test thread has exited" << std::endl;

   return true;
}

bool Tests::directoryTest()
{
   SDirectory d;

   const char *de = d.getFirstEntry( ".", "*.*" );

   while ( de )
   {
      std::cout << "File in \".\" - " << de << std::endl;
      de = d.getNextEntry();
   }

   return true;
}

bool Tests::pathTest()
{
   std::string s = SPath::combine( SPath::combine( SPath::combine( "path1/", "path2"), "path3/" ), "path4" );
   std::cout << s << std::endl;

   return true;
}

bool Tests::csvTest()
{
   SFile f;

   const char *fn = "test.csv";

   if ( f.open( fn ) )
   {
      uint32_t recnbr = 0;
      std::ios::streampos offset = -1;

std::cout << "***** before read loop" << std::endl;
      while ( f.read() )
      {
         std::cout << f.datarecnbr() << ":" << f.dataoffset() << " [" << f.data() << "]" << std::endl;
         
         if ( f.datarecnbr() == 10 )
         {
            recnbr = f.datarecnbr();
            offset = f.dataoffset();
         }
      }
std::cout << "***** after read loop" << std::endl;

      if ( !f.seek( recnbr, offset ) )
      {
         std::cout << "***** seek failed" << std::endl;
      }
      else
      {
std::cout << "***** before 2nd read loop" << std::endl;
         for ( int i = 0; i < 1000; i++ )
         {
            while ( f.read() )
               std::cout << f.datarecnbr() << ":" << f.dataoffset() << " [" << f.data() << "]" << std::endl;
            SThread::sleep( 1000 );
         }
      }
   }
   else
   {
      std::cout << "Unable to open " << fn << std::endl;
   }

   return true;
}

bool Tests::splitTest()
{
   std::string s;

   s = "1,2,3,4,5,a,b,c,d,e";

   std::vector<std::string> a = SUtility::split( s, ',' );

   for ( std::vector<std::string>::const_iterator it = a.begin(); it != a.end(); ++it )
      std::cout << "[" << *it << "]" << std::endl;

   return true;
}

bool Tests::trackTest()
{
   int cnt = 60;
   CSVTracker t1;

   t1.setFilename( "tracktest" );

//std::cout << "trackTest() recnbr=" << t1.getFirstRecordNumber() << " recofs=" << t1.getFirstRecordOffset() << std::endl;
   t1.ACRsent( 1, 0 );
   std::cout << "isACRsent(1)=" << t1.isACRsent(1) << " isACRsent(2)=" << t1.isACRsent(2) << std::endl;

   std::cout << "setting ACRsent for";
   for ( uint32_t r = 0; r < cnt; r+=2 )
   {
      std::cout << " " << r;
      t1.ACRsent( r + 1, r * 100 );
   }
   std::cout << std::endl;

   std::cout << "setting ACRrcvd for";
   for ( uint32_t r = 1; r < cnt; r+=2 )
   {
      std::cout << " " << r;
      t1.ACArcvd( r + 1 );
   }
   std::cout << std::endl;

   for ( uint32_t r = 0; r < cnt; r++ )
      std::cout << (t1.isACRsent(r+1)?"1":"0") << (t1.isACArcvd(r+1)?"1":"0") << " ";
   std::cout << std::endl;

   std::cout << "before the save recnbr=" << t1.getFirstRecordNumber() << " recofs=" << t1.getFirstRecordOffset() <<  " data.size()=" << t1.getDataSize() << " ofs.size()=" << t1.getOffsetSize() << std::endl;
   t1.saveData();
   std::cout << "after the save recnbr=" << t1.getFirstRecordNumber() << " recofs=" << t1.getFirstRecordOffset() <<  " data.size()=" << t1.getDataSize() << " ofs.size()=" << t1.getOffsetSize() << std::endl;

   CSVTracker t2;
   t2.setFilename( "tracktest" );
   t2.loadData();

   for ( uint32_t r = 0; r < cnt; r++ )
      std::cout << (t2.isACRsent(r+1)?"1":"0") << (t2.isACArcvd(r+1)?"1":"0") << " ";
   std::cout << std::endl;

   for ( uint32_t r = 0; r < cnt; r++ )
   {
      t1.ACRsent( r + 1, r * 100 );
      t1.ACArcvd( r + 1 );
   }

   std::cout << "before the save recnbr=" << t1.getFirstRecordNumber() << " recofs=" << t1.getFirstRecordOffset() <<  " data.size()=" << t1.getDataSize() << " ofs.size()=" << t1.getOffsetSize() << std::endl;
   t1.saveData();
   std::cout << "after the save recnbr=" << t1.getFirstRecordNumber() << " recofs=" << t1.getFirstRecordOffset() <<  " data.size()=" << t1.getDataSize() << " ofs.size()=" << t1.getOffsetSize() << std::endl;

   CSVTracker t2b;
   t2b.setFilename( "tracktest" );
   t2b.loadData();

   for ( uint32_t r = 0; r < cnt; r++ )
      std::cout << (t2b.isACRsent(r+1)?"1":"0") << (t2b.isACArcvd(r+1)?"1":"0") << " ";
   std::cout << std::endl;

   /////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////

   cnt = 1000000;
   CSVTracker t3;
   t3.setFilename( "tracktest2" );
   for ( uint32_t r=0; r < cnt; r+=2 )
      t3.ACRsent( r + 1, r * 100 );
   for ( uint32_t r=1; r < cnt; r+=2 )
      t3.ACArcvd( r + 1 );
   for ( uint32_t r=0; r < cnt; r++ )
   {
      if ( ((r % 2) == 0 && (!t3.isACRsent(r + 1) || t3.isACArcvd(r + 1))) ||
           ((r % 2) == 1 && (t3.isACRsent(r + 1) || !t3.isACArcvd(r + 1))) )
      {
         std::cout << "index " << r << " isACRsent=" << t3.isACRsent(r + 1) << " isACArcvd=" << t3.isACArcvd(r + 1) << std::endl;
      }
   }
   std::cout << "1st verification complete for " << cnt << " entries" << std::endl;
   t3.saveData();

   /////////////////////////

   CSVTracker t4;
   t4.setFilename( "tracktest2" );
   t4.loadData();

   for ( uint32_t r=0; r < cnt; r++ )
   {
      if ( ((r % 2) == 0 && (!t4.isACRsent(r + 1) || t4.isACArcvd(r + 1))) ||
           ((r % 2) == 1 && (t4.isACRsent(r + 1) || !t4.isACArcvd(r + 1))) )
      {
         std::cout << "index " << r << " isACRsent=" << t4.isACRsent(r + 1) << " isACArcvd=" << t4.isACArcvd(r + 1) << std::endl;
      }
   }
   std::cout << "2nd verification complete for " << cnt << " entries" << std::endl;

   return true;
}

bool Tests::fdTest1(rf::Application& app)
{
   time_t t = time(NULL);

   FDMessageRequest req( &app.getDict().cmdACR() );

   req.add( app.getDict().avpSessionId(), "this-is-the-session-id" );
   req.add( app.getDict().avpAccountingRecordType(), 1 ) ;
   req.add( app.getDict().avpAccountingRecordNumber(), 0 ) ;
   req.add( app.getDict().avpEventTimestamp(), time(NULL) ) ;
   req.add( app.getDict().avpOriginHost(), "myhost.mydomain"  );
   req.add( app.getDict().avpOriginRealm(), "mydomain" ) ;

   req.dump();

   std::string sessionid, originhost, originrealm, hostip;
   int art = 0, arn = 0;
   time_t et = 0;

   FDAvp a1 = req.findAVP( app.getDict().avpSessionId() );
   FDAvp a2 = req.findAVP( app.getDict().avpAccountingRecordType() );
   FDAvp a3 = req.findAVP( app.getDict().avpAccountingRecordNumber() );
   FDAvp a4 = req.findAVP( app.getDict().avpEventTimestamp() );
   FDAvp a5 = req.findAVP( app.getDict().avpOriginHost() );
   FDAvp a6 = req.findAVP( app.getDict().avpOriginRealm() );

#if 0
   if ( !a1.get( sessionid ) ) { std::cout << "req.get() for sessionid failed" << std::endl; }
   if ( !a2.get( art ) ) { std::cout << "req.get() for art failed" << std::endl; }
   if ( !a3.get( arn ) ) { std::cout << "req.get() for arn failed" << std::endl; }
   if ( !a4.get( et ) ) { std::cout << "req.get() for et failed" << std::endl; }
   if ( !a5.get( originhost ) ) { std::cout << "req.get() for originhost failed" << std::endl; }
   if ( !a6.get( originrealm ) ) { std::cout << "req.get() for originrealm failed" << std::endl; }

   std::cout << "[Session-Id]               = " << sessionid << std::endl;
   std::cout << "[Accounting-Record-Type]   = " << art << std::endl;
   std::cout << "[Accounting-Record-Number] = " << arn << std::endl;
   std::cout << "[Event-Timestamp]          = " << et << " orig=" << t << std::endl;
   std::cout << "[Origin-Host]              = " << originhost << std::endl;
   std::cout << "[Origin-Realm]             = " << originrealm << std::endl;
#endif

   if ( !req.get( app.getDict().avpSessionId(), sessionid ) ) { std::cout << "req.get() for sessionid failed" << std::endl; }
   if ( !req.get( app.getDict().avpAccountingRecordType(), art ) ) { std::cout << "req.get() for art failed" << std::endl; }
   if ( !req.get( app.getDict().avpAccountingRecordNumber(), arn ) ) { std::cout << "req.get() for arn failed" << std::endl; }
   if ( !req.get( app.getDict().avpEventTimestamp(), et ) ) { std::cout << "req.get() for et failed" << std::endl; }
   if ( !req.get( app.getDict().avpOriginHost(), originhost ) ) { std::cout << "req.get() for originhost failed" << std::endl; }
   if ( !req.get( app.getDict().avpOriginRealm(), originrealm ) ) { std::cout << "req.get() for originrealm failed" << std::endl; }

   std::cout << "[Session-Id]               = " << sessionid << std::endl;
   std::cout << "[Accounting-Record-Type]   = " << art << std::endl;
   std::cout << "[Accounting-Record-Number] = " << arn << std::endl;
   std::cout << "[Event-Timestamp]          = " << et << " orig=" << t << std::endl;
   std::cout << "[Origin-Host]              = " << originhost << std::endl;
   std::cout << "[Origin-Realm]             = " << originrealm << std::endl;

   return true;
}
