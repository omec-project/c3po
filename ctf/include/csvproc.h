/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __CSVPROC_H
#define __CSVPROC_H

#include "sthread.h"
#include "sdir.h"
#include "csvfile.h"
#include "csvtrack.h"
#include "epc.h"
#include "fd.h"
#include "rf_impl.h"
#include "sstats.h"

class StatsCtf;

const uint16_t ACR_EVENT = ETM_USER + 1;
const uint16_t ACA_EVENT = ETM_USER + 2;

class ACAEventMessage : public SEventThreadMessage
{
public:
   ACAEventMessage( const std::string &filename, uint32_t recnbr, int64_t recofs, uint32_t resultcode )
      : SEventThreadMessage( ACA_EVENT ),
        m_filename( filename ),
        m_recnbr( recnbr ),
        m_recofs( recofs ),
        m_resultcode( resultcode )
   {
   }

   const std::string &getFilename() { return m_filename; }
   uint32_t getRecordNumber() { return m_recnbr; }
   int64_t getRecordOffset() { return m_recofs; }
   uint32_t getResultCode() { return m_resultcode; }

private:
   ACAEventMessage();

   std::string m_filename;
   uint32_t m_recnbr;
   int64_t m_recofs;
   uint32_t m_resultcode;
};

class CSVProcessor : public SEventThread
{
public:
   CSVProcessor();
   ~CSVProcessor();

   void onInit();
   void onQuit();
   void onTimer( SEventThread::Timer &t );

   virtual void dispatch( SEventThreadMessage &msg );

   void sendACRs();
   void receivedACA( ACAEventMessage &msg );

   bool isCurrent() { return m_iscur; }

private:
   bool openNextFile();
   bool positionAtNextRecord( uint32_t recnbr );
   bool verifyDiameter();
   void archiveFile();
   void addAccountingPeer( const EPC::DiameterHost &host, uint16_t port );
   void getAccountingPeers();

   SDirectory m_dir;
   std::string m_filemask;
   std::string m_filename;
   bool m_iscur;
   uint32_t m_next2send;
   SEventThread::Timer m_idletimer;
   CSVFile m_file;
   CSVTracker m_tracker;
   uint32_t m_acrssent;
   uint32_t m_totalsent;
   FDPeerList m_peers;
   rf::Application *m_rfapp;
   FDEngine m_diameter;
};

#endif // #define __CSVPROC_H
