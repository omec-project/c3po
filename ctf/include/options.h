/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <stdint.h>
#include <string>

class Options
{
public:

   static bool parse( int argc, char **argv );
   static bool parseInputOptions( int argc, char **argv );
   static bool parseJson();
   static bool validateOptions();
   
   static const int logMaxSize()                         { return m_logmaxsize; }
   static const int logNumberFiles()                     { return m_lognbrfiles; }
   static const std::string &logFilename()               { return m_logfilename; }
   static const int logQueueSize()                       { return m_logqueuesize; }

   static const int statlogMaxSize()                     { return m_statlogmaxsize; }
   static const int statlogNumberFiles()                 { return m_statlognbrfiles; }
   static const std::string &statlogFilename()           { return m_statlogfilename; }
   static const uint32_t statsFrequency()                { return m_statsfrequency; }

   static const int auditlogMaxSize()                    { return m_auditlogmaxsize; }
   static const int auditlogNumberFiles()                { return m_auditlognbrfiles; }
   static const std::string &auditlogFilename()          { return m_auditlogfilename; }

   static const int &getossport()                        { return m_ossport; }
   static const std::string &getossfile()                { return m_ossfile; }

   static const std::string &jsonConfig() { return m_jsoncfg; }
   static const std::string &dataPrefix() { return m_dataprefix; }
   static const std::string &dataPath() { return m_datapath; }
   static const std::string &archivePath() { return m_archivepath; }
   static const std::string &trackingPath() { return m_trackingpath; }
   static const std::string &currentExtension() { return m_currentextension; }
   static const uint32_t skipRows() { return m_skiprows; }
   static const long idleDuration() { return m_idledur; }
   static const uint32_t maxACRs() { return m_maxacrs; }
   static const int acaSaveFrequency() { return m_acasavefrequency; }
   static const std::string &trackingExtension() { return m_trackingextension; }
   static const std::string &diameterConfiguration() { return m_diameterconfiguration; }
   static const std::string &memcachedServer() { return m_memcachedserver; }
   static const int memcachedPort() { return m_memcachedport; }
   static const std::string &originHost() { return m_originhost; }
   static const std::string &originRealm() { return m_originrealm; }
   static const std::string &sgxserverIp() { return m_sgxserverip; }
   static const int sgxserverPort() { return m_sgxserverport; }
   static const std::string &privkeyFile() { return m_privkeyfile; }
   static const std::string &certFile() { return m_certfile; }
   static const std::string &mrSigner() { return m_mrsigner; }
   static const std::string &mrEnclave() { return m_mrenclave; }
   static const int isvSvn() { return m_isvsvn; }
   static const std::string &streamName() { return m_streamname;}

   static const bool runTests() { return (options & runtests); }

private:

   enum OptionsSelected {
     jsoncfg               = 0x01,
     dataprefix            = 0x02,
     datapath              = 0x04,
     archivepath           = 0x08,
     trackingpath          = 0x10,
     currentextension      = 0x20,
     trackingextension     = 0x40,
     diameterconfiguration = 0x80,
     memcachedserver       = 0x100,
     memcachedport         = 0x200,
     originhost            = 0x400,
     originrealm           = 0x800,
     skiprows              = 0x1000,
     idledur               = 0x2000,
     maxacrs               = 0x4000,
     acasavefrequency      = 0x8000,
     statsfrequency        = 0x10000,
     runtests              = 0x20000,

     logmaxsize            = 0x40000,
     lognbrfiles           = 0x80000,
     logfilename           = 0x100000,
     logqueuesize          = 0x200000,
     statlogfilename       = 0x400000,
     statlognbrfiles       = 0x800000,
     statlogmaxsize        = 0x1000000,
     statfrequency         = 0x2000000,
     auditlogfilename      = 0x4000000,
     auditlognbrfiles      = 0x8000000,
     auditlogmaxsize       = 0x10000000,
     ossport               = 0x20000000,
     ossfile               = 0x40000000,

#ifdef SGX_CDR
     sgxserverip           = 0x80000000,
     sgxserverport         = 0x100000000,
     privkeyfile           = 0x200000000,
     certfile              = 0x400000000,
     mrsigner              = 0x800000000,
     mrenclave             = 0x1000000000,
     isvsvn                = 0x2000000000,
     streamname            = 0x4000000000,
#endif
   };

   static void help();

   static const int JSONFILEBUFFER;
   static long int options;

   static int m_logmaxsize;
   static int m_lognbrfiles;
   static std::string m_logfilename;
   static int m_logqueuesize;

   static int m_statlogmaxsize;
   static int m_statlognbrfiles;
   static std::string m_statlogfilename;
   static uint32_t    m_statsfrequency;

   static int m_auditlogmaxsize;
   static int m_auditlognbrfiles;
   static std::string m_auditlogfilename;

   static std::string m_jsoncfg;
   static std::string m_dataprefix;
   static std::string m_datapath;
   static std::string m_archivepath;
   static std::string m_trackingpath;
   static std::string m_currentextension;
   static uint32_t m_skiprows;
   static long m_idledur;
   static uint32_t m_maxacrs;
   static int m_acasavefrequency;
   static std::string m_trackingextension;
   static std::string m_diameterconfiguration;
   static std::string m_memcachedserver;
   static int m_memcachedport;
   static std::string m_originhost;
   static std::string m_originrealm;
   static int         m_ossport;
   static std::string m_ossfile;
   static std::string m_sgxserverip;
   static int m_sgxserverport;
   static std::string m_privkeyfile;
   static std::string m_certfile;
   static std::string m_mrsigner;
   static std::string m_mrenclave;
   static std::string m_streamname;
   static int m_isvsvn;
};

#endif // #define __OPTIONS_H
