/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include "csvproc.h"
#include "options.h"
#include "spath.h"
#include "sutility.h"
#include "statsctf.h"
#include "msg_event.h"
#include "sslcommhndlr.h"


CSVProcessor::CSVProcessor()
: m_acrssent(0),
  m_rfapp(NULL)

{
}

CSVProcessor::~CSVProcessor()
{
   if(NULL != m_rfapp){
      delete m_rfapp;
      m_rfapp = NULL;
   }
}

void CSVProcessor::dispatch( SEventThreadMessage &msg )
{
   if ( msg.getId() == ACR_EVENT )
   {
      sendACRs();
   }
   else if ( msg.getId() == ACA_EVENT )
   {
      receivedACA( (ACAEventMessage&)msg );
   }
}

#define NAPTRA(p) ((EPC::DiameterNaptrA*)*p)
#define NAPTRS(p) ((EPC::DiameterNaptrS*)*p)

void CSVProcessor::addAccountingPeer( const EPC::DiameterHost &host, uint16_t port )
{
   std::cout << "adding accounting diameter peer " << host.getName() << ":" << port << std::endl;

   FDPeer *p = new FDPeer( host.getName(), port );

   m_peers.push_back( p );
}

void CSVProcessor::getAccountingPeers()
{
   EPC::DiameterSelector ds;

   std::string realm = SUtility::string_format( "diameter.%s", Options::originRealm().c_str() );

   ds.setRealm( realm.c_str() );
   ds.setApplication( EPC::dia_app_base_accounting );
   ds.setProtocol( EPC::dia_protocol_tcp );

   EPC::DiameterNaptrList &dl(  ds.process() );

   for ( EPC::DiameterNaptrList::const_iterator it = dl.begin();
         it != dl.end();
         ++it )
   {
      // add diameter peer
      if ( (*it)->getType() == EPC::dnt_hostname )
      {
         addAccountingPeer( NAPTRA(it)->getHost(), 3868 );
      }
      else
      {
         for ( EPC::DiameterSrvVector::const_iterator is = NAPTRS(it)->getSrvs().begin();
               is != NAPTRS(it)->getSrvs().end();
               ++is )
         {
            addAccountingPeer( (*is)->getHost(), (*is)->getPort() );
         }
      }
   }
}

void CSVProcessor::onInit()
{

   try
   {
      // set the diameter configuration file
      m_diameter.setConfigFile( Options::diameterConfiguration() );

      // initialize diameter
      if ( !m_diameter.init() ){
         quit();
         return;
      }

      m_rfapp = new rf::Application();

      // advertise support for the accounting application
      m_diameter.advertiseSupport( m_rfapp->getDict().app(), 0, 1 );

      if(!m_diameter.start()){
         quit();
         return;
      }
   }
   catch ( FDException &e )
   {
      quit();
      return;
   }

   HookEvent::init(&StatsCtf::singleton(), m_rfapp);

   getAccountingPeers();

   std::string filemask = Options::dataPrefix() + "*";

   m_file.setSkip( Options::skipRows() );

   m_dir.setDirectory( Options::dataPath() );
   m_dir.setFilemask( filemask );

   m_idletimer.setInterval( Options::idleDuration() );
   m_idletimer.setOneShot( true );
   initTimer( m_idletimer );

   postMessage( ACR_EVENT );
}

void CSVProcessor::onQuit()
{
   m_diameter.uninit();

   m_idletimer.stop();

   if ( m_file.is_open() )
      std::cout << "onQuit() sent " << m_totalsent << " ACR's for [" << m_filename << "]" << std::endl;

   m_diameter.uninit();
}

void CSVProcessor::onTimer( SEventThread::Timer &t )
{
   if ( t.getId() == m_idletimer.getId() )
   {
      postMessage( ACR_EVENT );
   }
} 

bool CSVProcessor::openNextFile()
{
   std::string fn;

   // get the directory listing
   SDirectoryListing dl = m_dir.getListing();

   // keep trying files until one opens
   while ( dl.size() )
   {
      fn = SPath::combine( Options::dataPath(), dl.front() );

      //TODO: Add Other SGX options
      if (!Options::sgxserverIp().empty())
      {
         FILE *stream;
         std::string sn("1");
         sn.append(fn);

         try
         {
            SSLCommHndlr client(Options::sgxserverIp(),
                  Options::sgxserverPort(), Options::certFile(),
                  Options::privkeyFile());

            stream = client.readSgxSSLToFileBuffer(sn, m_file.getStreamBuffer());
            if (!stream)
               break;

            m_file.setStream(stream);
            m_file.setfilename(fn);
         }
         catch (std::exception const& e)
         {
            std::cout << "Exception : " << e.what() << std::endl;
            break;
         }
      }
      else
      {
         if ( m_file.open( fn ) )
            break;
      }
      dl.erase( dl.begin() );
   }

   // if no file was opened, return false
   if ( !m_file.is_open() )
      return false;

   // save the file info
   m_filename = m_file.filename();
   {
      std::string ext = SPath::getExtension( m_filename );
      m_iscur = ext == Options::currentExtension();
   }

   // load the tracking info
   {
      // remove both ".cur" and ".csv" extensions
      std::string fn =  SPath::getFileNameWithoutExtension(
         SPath::getFileNameWithoutExtension( m_filename ) ) + Options::trackingExtension();
      std::string tfn = SPath::combine( Options::trackingPath(), fn );

      m_tracker.setFilename( tfn );
      m_tracker.loadData();

      m_next2send = m_tracker.getFirstRecordNumber();

      // skip over rows that are to be ignored
      while ( m_next2send <= m_file.getSkip() )
      {
         m_tracker.ACRsent( m_next2send, m_next2send == 1 ? 0 : UINT32_MAX );
         m_tracker.ACArcvd( m_next2send );
         m_next2send++;
      }
   }

   // position the file pointer to the last known record
   if ( m_tracker.getFirstRecordOffset() != UINT32_MAX )
   {
      std::cout 
         << "openNextFile() seek to recnbr=" << m_tracker.getFirstRecordNumber()
         << " offset=" << m_tracker.getFirstRecordOffset()
         << std::endl;
      m_file.seek( m_tracker.getFirstRecordNumber(), m_tracker.getFirstRecordOffset() );
   }

   m_totalsent = 0;

   std::cout
      << "openNextFile() [" << m_filename << "]"
      << " iscurrent=" << (m_iscur?"yes":"no")
      << std::endl;

   return true;
}

bool CSVProcessor::positionAtNextRecord( uint32_t recnbr )
{
   if ( recnbr < m_file.datarecnbr() )
   {
      //
      // the next record is before the current file position, so the
      // file position needs to be moved back to the proper position.
      // if "next" record is before first known file position, reposition
      // to the beginning of the file, otherwise reposition to the
      // "latest" known file position
      //
      if ( recnbr < m_tracker.getFirstRecordNumber() )
      {
         // the earliest known position is after the desired position,
         // so position at the beginning
         m_file.seek( 1, 0 );
      }
      else
      {
         m_file.seek( m_tracker.getFirstRecordNumber(), m_tracker.getFirstRecordOffset() );
      }
   }

   // keep reading until we find the desired record
   while ( m_file.datarecnbr() != recnbr && m_file.read() );

   return m_file.datarecnbr() == recnbr;
}

void CSVProcessor::receivedACA( ACAEventMessage &msg )
{
   std::cout << "receivedACA() - received ACA for record number " << msg.getRecordNumber() << std::endl;
   m_acrssent--;

   if ( msg.getResultCode() == ER_DIAMETER_SUCCESS ){
      m_tracker.ACArcvd( msg.getRecordNumber() );
      StatsCtf::singleton().registerStatResult(stat_ctf_acr, 0, ER_DIAMETER_SUCCESS);
   }
   else {
      m_tracker.ACRsent( msg.getRecordNumber(), msg.getRecordOffset(), false );
      StatsCtf::singleton().registerStatResult(stat_ctf_acr, 0, msg.getResultCode());
   }

   if ( m_tracker.getACAcount() >= Options::acaSaveFrequency() )
      m_tracker.saveData();

   if ( msg.getResultCode() == ER_DIAMETER_SUCCESS )
   {
      sendACRs();
   }
   else
   {
      if ( msg.getRecordNumber() < m_next2send )
         m_next2send = msg.getRecordNumber();

      m_idletimer.stop();
      m_idletimer.start();
   }
}

void CSVProcessor::sendACRs()
{
   // stop the idle timer if it is running
   m_idletimer.stop();

   // find an open diameter peer to send the message to
   FDPeer *peer = m_peers.getOpenPeer();
   if ( !peer )
   {
      m_idletimer.start();
      return;
   }

   // if a file is not currently open and there is not another file to process
   if ( !m_file.is_open() && !openNextFile() )
   {
      // no file to process, so wait a little while and check again
      m_idletimer.start();
      return;
   }

   while ( m_acrssent < Options::maxACRs() )
   {
      //
      // find the next record to send, skipping over records that have already
      // been sent this logic only comes into play on a restart
      //
      while ( m_next2send <= m_file.getSkip() || m_tracker.isACRsent( m_next2send ) )
         m_next2send++;

      // read the desired record
      if ( !positionAtNextRecord( m_next2send ) )
         break;

      // parse the data
      m_file.parse();

      // send the ACR, if successfully sent, set the ACR sent flag in the tracker
      if ( m_rfapp->sendACRreq( *this, *peer, m_file ) )
      {
         m_tracker.ACRsent( m_file.datarecnbr(), m_file.dataoffset() );
         m_acrssent++;
         m_totalsent++;

std::cout << "sendACRs() sendRequest() success " << m_acrssent << std::endl;
      }
      else
      {
std::cout << "sendACRs() sendRequest() FAILURE " << m_acrssent << std::endl;
         m_idletimer.start();
         return;
      }
   }

   if ( m_acrssent < Options::maxACRs() )
   {
      m_tracker.saveData();
      //
      // processing stopped because EOF was encountered
      // if this is the "current" file, then check to see if it
      // has been renamed, and if so, close and archive it
      // if it has not been renamed, set the idle timer and
      // wait for a while to check for new data to have been
      // written to the file.
      //
      if ( !isCurrent() || m_file.renamed() )
      {
         std::cout << "sendACRs() sent " << m_totalsent << " ACR's for [" << m_filename << "]" << std::endl;
         m_file.close();
         archiveFile();
         postMessage( ACR_EVENT );
      }
      else
      {
         m_idletimer.start();
      }
   }
   else
   {
      //
      // wait for an ACA to come in before continuing
      // no action is required here since an ACA_EVENT message
      // will be posted when an ACA arrives
      //
   }
}

bool CSVProcessor::verifyDiameter()
{
   return true;
}

void CSVProcessor::archiveFile()
{
   size_t amt = m_filename.find( Options::currentExtension() );
   std::string src = ( amt == std::string::npos ? m_filename : m_filename.substr( 0, amt ) );
   std::string dst = SPath::combine( Options::archivePath(), SPath::getFileName( src ) );

   std::cout << "archiveFile() src [" << src << "] dst [" << dst << "]" << std::endl;

   SUtility::copyfile( dst, src );

   SUtility::deletefile( src );
}
