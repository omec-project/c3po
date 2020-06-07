/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __S6AS6D_IMPL_H
#define __S6AS6D_IMPL_H

#include "s6as6d.h"

class HSSPerformance;

class ULRTest;
class ULRTracker;
class ULRTestThread;

class AIRTracker;
class AIRTestThread;

class AttachTracker;
class AttachTestThread;

namespace s6as6d {

// Member functions that customize the individual application
class Application : public ApplicationBase
{
    friend UPLRreq;
    friend CALRreq;
    friend AUIRreq;
    friend INSDRreq;
    friend DESDRreq;
    friend PUURreq;
    friend RERreq;

public:
    Application();
    ~Application();

    //UPLRcmd &getUPLRcmd() { return m_cmd_uplr; }
    //CALRcmd &getCALRcmd() { return m_cmd_calr; }
    //AUIRcmd &getAUIRcmd() { return m_cmd_auir; }
    //INSDRcmd &getINSDRcmd() { return m_cmd_insdr; }
    //DESDRcmd &getDESDRcmd() { return m_cmd_desdr; }
    //PUURcmd &getPUURcmd() { return m_cmd_puur; }
    //RERcmd &getRERcmd() { return m_cmd_rer; }

    // Parameters for sendXXXreq, if present below, may be changed
    // based upon processing needs
    bool sendUPLRreq(FDPeer &peer);
    bool sendCALRreq(FDPeer &peer);
    bool sendAUIRreq(FDPeer &peer);
    bool sendINSDRreq(FDPeer &peer);
    bool sendDESDRreq(FDPeer &peer);
    bool sendPUURreq(FDPeer &peer);
    bool sendRERreq(FDPeer &peer);

    bool sendULRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, ULRTest &ulrtest, ULRTracker &result);
    bool sendULRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, ULRTestThread &ulrtest, ULRTracker &result);
    bool sendULRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, AttachTestThread &ulrtest, AttachTracker &result);
    bool sendAIRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, AIRTestThread &airtest, AIRTracker &result);
    bool sendAIRreq(const char *imsi, uint8_t *vplmnid, const char *host, const char *realm, AttachTestThread &airulrtest, AttachTracker &result);

private:
    void registerHandlers();
    //UPLRcmd m_cmd_uplr;
    //CALRcmd m_cmd_calr;
    //AUIRcmd m_cmd_auir;
    //INSDRcmd m_cmd_insdr;
    //DESDRcmd m_cmd_desdr;
    //PUURcmd m_cmd_puur;
    //RERcmd m_cmd_rer;

    // the parameters for createXXXreq, if present below, may be
    // changed based processing needs
    UPLRreq *createUPLRreq(FDPeer &peer);
    CALRreq *createCALRreq(FDPeer &peer);
    AUIRreq *createAUIRreq(FDPeer &peer);
    INSDRreq *createINSDRreq(FDPeer &peer);
    DESDRreq *createDESDRreq(FDPeer &peer);
    PUURreq *createPUURreq(FDPeer &peer);
    RERreq *createRERreq(FDPeer &peer);
};

////////////////////////////////////////////////////////////////////////////////

class TestULRreq : public UPLRreq
{
public:
   TestULRreq( Application &app, ULRTest &ulrtest, ULRTracker &tracker );
   ~TestULRreq();

   virtual void processAnswer( FDMessageAnswer &ans );

private:
   ULRTest &m_ulrtest;
   ULRTracker &m_result;
};

////////////////////////////////////////////////////////////////////////////////

class TestULRreq2 : public UPLRreq
{
public:
   TestULRreq2( Application &app, ULRTestThread &ulrtest, ULRTracker &tracker );
   ~TestULRreq2();

   virtual void processAnswer( FDMessageAnswer &ans );

private:
   ULRTestThread &m_ulrtest;
   ULRTracker &m_result;
};

////////////////////////////////////////////////////////////////////////////////

class TestAIRreq : public AUIRreq
{
public:
   TestAIRreq( Application &app, AIRTestThread &airtest, AIRTracker &tracker );
   ~TestAIRreq();

   virtual void processAnswer( FDMessageAnswer &ans );

private:
   AIRTestThread &m_airtest;
   AIRTracker &m_result;
};

////////////////////////////////////////////////////////////////////////////////

class TestAIRAttachreq : public AUIRreq
{
public:
   TestAIRAttachreq( Application &app, AttachTestThread &airtest, AttachTracker &tracker );
   ~TestAIRAttachreq();

   virtual void processAnswer( FDMessageAnswer &ans );

private:
   AttachTestThread &m_attachtest;
   AttachTracker &m_result;
};

class TestULRAttachreq : public UPLRreq
{
public:
   TestULRAttachreq( Application &app, AttachTestThread &airtest, AttachTracker &tracker );
   ~TestULRAttachreq();

   virtual void processAnswer( FDMessageAnswer &ans );

private:
   AttachTestThread &m_attachtest;
   AttachTracker &m_result;
};

}

#endif // __S6AS6D_IMPL_H
