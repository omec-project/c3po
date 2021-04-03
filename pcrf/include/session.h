/*
* Copyright (c) 2003-2020, Great Software Laboratory Pvt. Ltd.
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SESSION_H
#define __SESSION_H

#include <string>
#include <map>
#include <set>

#include "fd.h"
#include "ssync.h"

#include "gx_impl.h"

#include "apn.h"
#include "bearer.h"
#include "endpoint.h"
#include "rule.h"
#include "subscriber.h"
#include "stimer.h"
#include "sthread.h"
#include "stime.h"

class PCRF;
class GxSession;
class SdSession;
class StSession;
class GxCreditControlRequest;

class SdSessionMap;
const uint16_t RARPendingRuleInstallTimeout    =    ETM_USER + 10;
const uint16_t RARPendingRuleRemoveTimeout     =    ETM_USER + 11;
const uint16_t RARDefaultRuleRemoveTimeout     =    ETM_USER + 12;
const uint16_t MaxCallDurationTimeout          =    ETM_USER + 13;

enum EventTriggerValues
{
	USER_LOCATION_CHANGE = 13,
	UE_TIMEZONE_CHANGE = 25,
	USAGE_REPORT = 33
};

enum ValidateErrorCode
{
    success = 0,
    failure,
    subscriptionIdMissing,
    imsiInvalid,
    imsiEmpty,
    calledStationIdMissing,
    calledStationIdEmpty,
    apnMissing,
    contextExists,
    unableToAddNewSubscriber,
    apnNotConfigured,
    unableToAssociateApnWithSubscriber,
    invalidIPForFrammedIPAddress,
    ipv4OrIpv6Missing,
    invalidFeatureListId,
    unableToAddPcef,
    unableToAddPcefInDatabase,
    originHostMissing,
    unableToAddTdfEndpoint,
    unableToAddTdfEndpointToDatabase,
    exceptionWhileAddingTdfEndpoint,
    tdfEndpointEntryInvalid,
    tssfEndpointEntryInvalid,
    syInterfaceNotSuported,
    unableToAddSessionToDatabase,
    unableToAddSessionInSessionMap,
    unableToAllocateSdIpCan1Object,
    sdEstablishSessionFailed,
    unableToAllocateStIpCan1EstablishSessionObject,
    stEstablishSessionFailed
};

enum RARTrigger
{
	triggerRARInstall = 0,
	triggerRARRemove,
	triggerRARPending,
	triggerCallDisconnect
};

namespace sd
{
   class SdTSRreq;
}

class SdSession
{
   friend class sd::SdTSRreq;

public:
   enum State
   {
      sIdle,
      sOpening,
      sOpen,
      sClosing,
      sClosed,
      sFailed
   };

   enum SessionReleaseCause
   {
      srcUnspecifiedReason = 0,
      srcUeSubscription_reason = 1,
      srcInsufficientServerResources = 2,
      srcIpCanSessionTermination = 3,
      srcUeIpAddressRelease = 4
   };

   SdSession( GxSession &pcef );
   ~SdSession();

   PCRF &getPCRF();

   GxSession &getGxSession() { return m_gx; }

   Endpoint *getEndpoint() { return m_ep; }
   Endpoint *setEndpoint( Endpoint *ep ) { m_ep = ep; return getEndpoint(); }

   State getState() { return m_state; }
   State setState( State s ) { return m_state = s; }
   const char *getStateDescription( SdSession::State s );
   const char *getStateDescription() { return getStateDescription( getState() ); }

   std::string &getSessionId() { return m_sessionid; }

   uint64_t getSupportedFeatures() { return m_supportedFeatures; }
   uint64_t setSupportedFeatures( uint64_t sf ) { return m_supportedFeatures = sf; }

   bool required() { return m_ep ? true : false; }

   RulesList &getInstalledRules() { return m_installed; }

private:
   SdSession();

   static SdSessionMap m_sdSessions;

   GxSession &m_gx;
   Endpoint *m_ep;
   FDPeer m_peer;
   State m_state;
   std::string m_sessionid;
   uint64_t m_supportedFeatures;
   RulesList m_installed;
};

class SdSessionMap : public std::map<const std::string,SdSession*>
{
public:
   ~SdSessionMap();

   static SdSessionMap &getInstance();

   bool addSession( const std::string &sessionid, SdSession *s );
   void eraseSession( const std::string &sessionid );
   bool findSession( const std::string &sessionid, SdSession* &s );

private:
   SdSessionMap();

   SMutex m_mutex;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class StSession
{
public:
   enum State
   {
      sIdle,
      sOpening,
      sOpen,
      sClosing,
      sClosed,
      sFailed
   };

   enum TerminationCause
   {
      tcDiameterLogout = 1,
      tcDiameterServiceNotProvided = 2,
      tcDiameterBadAnswer = 3,
      tcDiameterAdministrative = 4,
      tcDiameterLinkBroken = 5,
      tcDiameterAuthExpired = 6,
      tcDiameterUserMoved = 7,
      tcDiameterSessionTimeout = 8
   };

   StSession( GxSession &gx );
   ~StSession();

   PCRF &getPCRF();

   GxSession &getGxSession() { return m_gx; }

   Endpoint *getEndpoint() { return m_ep; }
   Endpoint *setEndpoint( Endpoint *ep ) { m_ep = ep; return getEndpoint(); }

   State getState() { return m_state; }
   State setState( State s ) { return m_state = s; }
   const char *getStateDescription( StSession::State s );
   const char *getStateDescription() { return getStateDescription( getState() ); }

   std::string &getSessionId() { return m_sessionid; }

   uint64_t getSupportedFeatures() { return m_supportedFeatures; }
   uint64_t setSupportedFeatures( uint64_t sf ) { return m_supportedFeatures = sf; }

   bool required() { return m_ep ? true : false; }

   RulesList &getInstalledRules() { return m_installed; }

private:
   StSession();

   GxSession &m_gx;
   Endpoint *m_ep;
   FDPeer m_peer;
   State m_state;
   std::string m_sessionid;
   uint64_t m_supportedFeatures;
   RulesList m_installed;
};

class StSessionMap : public std::map<std::string,StSession*>
{
public:
   ~StSessionMap();

   static StSessionMap &getInstance();

   bool addSession( const std::string &sessionid, StSession *s );
   void eraseSession( const std::string &sessionid );
   bool findSession( const std::string &sessionid, StSession* &s );

private:
   StSessionMap();

   SMutex m_mutex;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class GxSessionKey
{
public:
   GxSessionKey()
   {
   }

   GxSessionKey( const GxSessionKey &sk )
      : m_imsi( sk.m_imsi ),
        m_apn( sk.m_apn )
   {
   }

   GxSessionKey( const std::string &imsi, const std::string &apn )
      : m_imsi( imsi ),
        m_apn( apn )
   {
   }

   ~GxSessionKey()
   {
   }

   bool operator<( const GxSessionKey &rh ) const
   {
      return m_imsi < rh.m_imsi ? true : m_apn < rh.m_apn;
   }

   GxSessionKey &operator=( const GxSessionKey &rh )
   {
      m_imsi = rh.m_imsi;
      m_apn = rh.m_apn;
      return *this;
   }

   const std::string &getImsi() const { return m_imsi; }
   const std::string &getApn() const { return m_apn; }

   const std::string &setImsi( const char *v ) { m_imsi = v; return getImsi(); }
   const std::string &setImsi( const std::string &v ) { m_imsi = v; return getImsi(); }

   const std::string &setApn( const char *v ) { m_apn = v; return getApn(); }
   const std::string &setApn( const std::string &v ) { m_apn = v; return getApn(); }

private:
   std::string m_imsi;
   std::string m_apn;
};

class GxSessionState;

class GxSessionProc
{
public:
	GxSessionProc( PCRF& pcrf, SessionEvent* current_event );
	virtual ~GxSessionProc();
	std::string& getProcName() { return m_procname; }
	std::string& setProcName( char const* name) { m_procname = name; return getProcName(); }
	std::string& setProcName( std::string& name) { m_procname = name; return getProcName(); }
	virtual int accept( GxSessionState* current_state, gx::ReAuthAnswerExtractor& raa ); // modify pending state
	virtual int accept( GxSessionState* current_state, gx::CreditControlRequestExtractor& ccr ); // pending state
private:
	PCRF& m_pcrf;
	SessionEvent* mp_sessionevent;
	std::string m_procname;
};

class GxSessionInstallProc : public GxSessionProc
{
public:
	GxSessionInstallProc( PCRF& pcrf, SessionEvent* current_event );
	~GxSessionInstallProc();
	int accept( GxSessionState* current_state, gx::ReAuthAnswerExtractor& raa );
};

class GxSessionRemoveProc : public GxSessionProc
{
public:
	GxSessionRemoveProc( PCRF& pcrf, SessionEvent* current_event );
	~GxSessionRemoveProc();
	int accept( GxSessionState* current_state, gx::ReAuthAnswerExtractor& raa );
};

class GxSessionValidateProc : public GxSessionProc
{
public:
	GxSessionValidateProc( PCRF& pcrf, SessionEvent* current_event );
	~GxSessionValidateProc();
	int accept( GxSessionState* current_state, gx::CreditControlRequestExtractor& ccr );
};

class GxSessionDefaultRemoveProc : public GxSessionProc
{
public:
	GxSessionDefaultRemoveProc( PCRF& pcrf, SessionEvent* current_event );
	~GxSessionDefaultRemoveProc();
	int accept( GxSessionState* current_state, gx::ReAuthAnswerExtractor& raa );
};

class GxSessionState
{
public:
	GxSessionState( PCRF& pcrf, SessionEvent* current_session );
	virtual ~GxSessionState();
	SessionEvent* getCurrentEvent() { return mp_sessionevent; }
	std::string& getStateName() { return m_statename; }
	std::string& setStateName( char const* name) { m_statename = name; return getStateName(); }
   std::string& setStateName( std::string& name) { m_statename = name; return getStateName(); }
	// events function
	virtual int rcvdRAA( GxSessionProc* current_proc, gx::ReAuthAnswerExtractor& raa ); //modify pending state
	virtual int validateReq( GxSessionProc* current_proc, gx::CreditControlRequestExtractor& ccr ); // pending state
	// visitor functions
	virtual int visit( GxSessionInstallProc* current_proc, gx::ReAuthAnswerExtractor& raa ); //install raa
	virtual int visit( GxSessionRemoveProc* current_proc, gx::ReAuthAnswerExtractor& raa ); // remove raa
	virtual int visit( GxSessionValidateProc* cuurent_proc, gx::CreditControlRequestExtractor& ccr ); // validate ccr request 
	virtual int visit( GxSessionDefaultRemoveProc* current_proc, gx::ReAuthAnswerExtractor& raa ); // default rule remove raa 

private:
	PCRF& m_pcrf;
	SessionEvent* mp_sessionevent;
	std::string m_statename;
};

class GxSessionPendingState : public GxSessionState
{
public:
	GxSessionPendingState( PCRF& pcrf, SessionEvent* current_session );
	~GxSessionPendingState();
	int validateReq( GxSessionProc* current_proc, gx::CreditControlRequestExtractor& ccr );
	int visit( GxSessionValidateProc* current_proc, gx::CreditControlRequestExtractor& ccr );
};

class GxSessionActivePendingState : public GxSessionState
{
public:
    GxSessionActivePendingState( PCRF& pcrf, SessionEvent* current_session );
    ~GxSessionActivePendingState();
    int validateReq( GxSessionProc* current_proc, gx::CreditControlRequestExtractor& ccr );
    int visit( GxSessionValidateProc* current_proc, gx::CreditControlRequestExtractor& ccr );
};

class GxSessionActiveState : public GxSessionState
{
public:
	GxSessionActiveState( PCRF& pcrf, SessionEvent* current_session );
	~GxSessionActiveState();
};

class GxSessionInactiveState : public GxSessionState
{
public:
	GxSessionInactiveState( PCRF& pcrf, SessionEvent* current_session );
	~GxSessionInactiveState();

};

class GxSessionModifyPendingState : public GxSessionState
{
public:
	GxSessionModifyPendingState( PCRF& pcrf, SessionEvent* current_session );
	~GxSessionModifyPendingState();
	int rcvdRAA( GxSessionProc* current_proc, gx::ReAuthAnswerExtractor& raa );
	int visit( GxSessionInstallProc* current_proc, gx::ReAuthAnswerExtractor& raa );
	int visit( GxSessionRemoveProc* current_proc, gx::ReAuthAnswerExtractor& raa );
	int visit( GxSessionDefaultRemoveProc* current_proc, gx::ReAuthAnswerExtractor& raa );
};


class DefaultRule
{
public:
	DefaultRule();
	~DefaultRule();
	const std::string &setRuleName( const char *v ) { m_rule_name = v; return getRuleName(); }
   const std::string &setRuleName( const std::string &v ) { m_rule_name = v; return getRuleName(); }
   const std::string &getRuleName() const { return m_rule_name; }

	const std::string &setDefinition( const char *v ) { m_rule_definition = v; return getDefinition(); }
   const std::string &setDefinition( const std::string &v ) { m_rule_definition = v; return getDefinition(); }
   const std::string &getDefinition() const { return m_rule_definition; }

	int getApnAggregateMaxBitrateUl() { return m_apn_aggregate_max_bitrate_ul; }
   int setApnAggregateMaxBitrateUl( int v ) { m_apn_aggregate_max_bitrate_ul = v; return getApnAggregateMaxBitrateUl(); }

   int getApnAggregateMaxBitrateDl() { return m_apn_aggregate_max_bitrate_dl; }
   int setApnAggregateMaxBitrateDl( int v ) { m_apn_aggregate_max_bitrate_dl = v; return getApnAggregateMaxBitrateDl(); }

	int getPriorityLevel() { return m_priority_level; }
   int setPriorityLevel( int v ) { m_priority_level = v; return getPriorityLevel(); }

   int getPreemptionCapability() { return m_preemption_capability; }
   int setPreemptionCapability( int v ) { m_preemption_capability = v; return getPreemptionCapability(); }

   int getPreemptionVulnerability() { return m_preemption_vulnerability; }
   int setPreemptionVulnerability( int v ) { m_preemption_vulnerability = v; return getPreemptionVulnerability(); }

	int getQci() { return m_qci; }
   int setQci( int v ) { m_qci = v; return getQci(); }

private:
	std::string m_rule_name;
	std::string m_rule_definition;
	int m_priority_level;
   int m_preemption_capability;
   int m_preemption_vulnerability;
	int m_qci;
	int m_apn_aggregate_max_bitrate_ul;
   int m_apn_aggregate_max_bitrate_dl;
};

class GxSession
{
public:
   enum State
   {
      sIdle,
      sOpening,
      sOpen,
      sClosing,
      sClosed,
      sFailed
   };

   enum Event
   {
      eIpCan1,
      eIpCan2,
      eIpCan3,
      eIpcan4
   };

   GxSession( PCRF &pcrf, SessionEvent* current_event );
   ~GxSession();

   bool canDelete();

   State getState() { return m_state; }
   State setState( State s ) { return m_state = s; }

   PCRF &getPCRF() const { return m_pcrf; }

   const std::string &getImsi() const { return m_imsi; }
   const std::string &setImsi( const char *v ) { m_imsi = v; return getImsi(); }
   const std::string &setImsi( const std::string &v ) { m_imsi = v; return getImsi(); }

   const std::string &getApn() const { return m_apn; }
   const std::string &setApn( const char *v ) { m_apn = v; return getApn(); }
   const std::string &setApn( const std::string &v ) { m_apn = v; return getApn(); }

   const std::string &getSessionId() const { return m_sessionid; }
   const std::string &setSessionId( const char *v ) { m_sessionid = v; return getSessionId(); }
   const std::string &setSessionId( const std::string &v ) { m_sessionid = v; return getSessionId(); }

   Apn *getApnEntry() const { return m_apnentry; }
   Apn *setApnEntry( Apn *apn ) { return m_apnentry = apn; }

   Endpoint *getPcefEndpoint() const { return m_pcef_endpoint; }
   Endpoint *setPcefEndpoint( Endpoint *v ) { m_pcef_endpoint = v; return getPcefEndpoint(); }

   Endpoint *getPcrfEndpoint() const { return m_pcrf_endpoint; }
   Endpoint *setPcrfEndpoint( Endpoint *v ) { m_pcrf_endpoint = v; return getPcrfEndpoint(); }

   size_t getIPv4Len() { return m_ipv4len; }
   size_t setIPv4Len( size_t v ) { return m_ipv4len = v; }
   const struct in_addr &getIPv4() const { return m_ipv4; }
   const struct in_addr &setIPv4( struct in_addr &ia, size_t len );
   const std::string &getIPv4str() const { return m_sipv4; }

   size_t getIPv6Len() { return m_ipv6len; }
   size_t setIPv6Len( size_t v ) { return m_ipv6len = v; }
   const struct in6_addr &getIPv6() const { return m_ipv6; }
   const struct in6_addr &setIPv6( const uint8_t *ia, size_t len );
   const std::string &getIPv6str() const { return m_sipv6; }

   uint64_t getSupportedFeatures() const { return m_supported_features; }
   uint64_t setSupportedFeatures( int64_t v ) { return m_supported_features = v; }

   const BearerMap &getBearers() const { return m_bearers; }

   SdSession &getTdfSession() { return m_tdf; }
   StSession &getTssfSession() { return m_tssf; }

   RulesList &getRules() { return m_rules; }
   RulesList &getInstalledRules() { return m_installed; }

	RulesReportList &getRulesReport() { return m_rulesreport; }

   Subscriber &getSubscriber() { return m_subscriber; }

   SMutex &getMutex() { return m_mutex; }

	DefaultRule* getDefaultRule() { return m_default_rule; }
	void setDefaultRule( DefaultRule* default_rule ) { m_default_rule = default_rule; }
	
	GxSessionState* getCurrentState() { return mp_currentstate; }
   void setCurrentState( GxSessionState* current_state) 
	{ 
		if ( getCurrentState() != NULL )
		{
			delete ( getCurrentState() );
		}
		mp_currentstate = current_state; 
	}

   GxSessionProc* getCurrentProc() { return mp_currentproc; }
   void setCurrentProc( GxSessionProc* current_proc ) 
	{ 
		if ( getCurrentProc() != NULL )
		{
			delete( getCurrentProc() );
		}
		mp_currentproc = current_proc; 
	}

	SessionEvent* getCurrentEvent() { return mp_currentevent; }
	void setCurrentEvent( SessionEvent* current_event ) { mp_currentevent = current_event; }

   static void teardownSession( const char *source, GxSession *gx, SdSession::SessionReleaseCause src, bool lock = true ) { teardownSession( source, gx, src, StSession::tcDiameterLogout, lock ); }
   static void teardownSession( const char *source, GxSession *gx, StSession::TerminationCause tc, bool lock = true ) { teardownSession( source, gx, SdSession::srcUnspecifiedReason, tc, lock ); }
   static void teardownSession( const char *source, GxSession *gx, bool lock = true ) { teardownSession( source, gx, SdSession::srcUnspecifiedReason, StSession::tcDiameterLogout, lock ); }
   static void teardownSession( const char *source, GxSession *gx, SdSession::SessionReleaseCause src, StSession::TerminationCause tc, bool lock = true );

private:
   SMutex m_mutex;
   State m_state;
	SessionEvent* mp_currentevent;
	GxSessionState* mp_currentstate;
   GxSessionProc* mp_currentproc;
   PCRF &m_pcrf;
   std::string m_imsi;
   std::string m_apn;
   std::string m_sessionid;
   Apn *m_apnentry;
   Endpoint *m_pcef_endpoint;
   Endpoint *m_pcrf_endpoint;
	DefaultRule* m_default_rule;
   struct in_addr m_ipv4;
   size_t m_ipv4len;
   std::string m_sipv4;
   struct in6_addr m_ipv6;
   size_t m_ipv6len;
   std::string m_sipv6;
   uint64_t m_supported_features;
   BearerMap m_bearers;
   SdSession m_tdf;
   StSession m_tssf;
   RulesList m_rules;
   RulesList m_installed;
	RulesReportList m_rulesreport;
   Subscriber m_subscriber;
};

class GxSessionMap : public std::map<const GxSessionKey,GxSession*>
{
   friend GxSession;
public:
   ~GxSessionMap();

   static GxSessionMap &getInstance();

   bool exists( GxSession *gx, bool lock = true );
   bool exists( const std::string &sessionid, bool lock = true );
   bool addSession( GxSession *gx, bool lock = true );
   bool eraseSession( const std::string &imsi, const std::string &apn, bool lock = true );
   bool findSession( const std::string &imsi, const std::string &apn, GxSession* &gx, bool lock = true );
   bool findSession( const std::string &sessionid, GxSession* &gx, bool lock = true );

   bool isTerminating( GxSession *gx, bool lock = true );

   SMutex &getMutex() { return m_mutex; }

protected:
   void terminateSession( GxSession *gx );
   void deleteSession( GxSession *gx, bool lock = true );

private:
   GxSessionMap();

   SMutex m_mutex;
   std::set<GxSession*> m_sessions;
   std::map<std::string,GxSession*> m_sessionids;
   std::set<GxSession*> m_terminating;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SessionEvent
{
public:
   enum EventType
   {
      etSdIpCan1EstablishSession,
      etStIpCan1EstablishSession,
      etSdIpCan1ProcessRules,
      etStIpCan1ProcessRules,
      etSdTerminateSession,
      etStTerminateSession,
      etGxProcessRulesUpdate,
      etSdProcessRulesUpdate,
      etStProcessRulesUpdate,

      etIpCan1,
      etIpCan2,
      etIpCan3,
      etIpCan4,
      etPcrfTdf1,
      etPcrfTdf2,
      etTdf1,
      etTdf2,
      etTdf3
   };

   enum EventStatus
   {
      esPending,
      esProcessing,
      esComplete,
      esFailed
   };

   SessionEvent( PCRF &pcrf );
   virtual ~SessionEvent();

   virtual EventType getEventType() = 0;

   PCRF &getPCRF() { return m_pcrf; }

   EventStatus getStatus() { return m_status; }
   EventStatus setStatus( EventStatus status ) { m_status = status; }

   const char *getStatusDescription( EventStatus es )
   {
      switch ( es )
      {
         case esPending:      return "esPending";
         case esProcessing:   return "esProcessing";
         case esComplete:     return "esComplete";
         case esFailed:       return "esFailed";
         default:             return "Unkonwn";
      }
   }

   const char *getStatusDescription() { return getStatusDescription( getStatus() ); }

private:
   SessionEvent();

   PCRF &m_pcrf;
   EventStatus m_status;
};

class GxIpCan1;

class SdIpCan1EstablishSession : public SessionEvent
{
public:
   SdIpCan1EstablishSession( PCRF &pcrf, GxIpCan1 *gxevent );
   ~SdIpCan1EstablishSession();

   EventType getEventType() { return etSdIpCan1EstablishSession; }

   bool processPhase1(); // start establishing the session
   bool processPhase2( bool success ); // session established or failed

   GxIpCan1 *getGxEvent() { return m_gxevent; }

private:
   SdIpCan1EstablishSession();

   SMutex m_mutex;
   GxIpCan1 *m_gxevent;
};

class StIpCan1EstablishSession : public SessionEvent
{
public:
   StIpCan1EstablishSession( PCRF &pcrf, GxIpCan1 *gxevent );
   ~StIpCan1EstablishSession();

   EventType getEventType() { return etStIpCan1EstablishSession; }

   bool processPhase1(); // start establishing the session
   bool processPhase2( bool success ); // session established or failed

   GxIpCan1 *getGxEvent() { return m_gxevent; }

private:
   StIpCan1EstablishSession();

   SMutex m_mutex;
   GxIpCan1 *m_gxevent;
};

class SdIpCan1ProcessRules : public SessionEvent
{
public:
   SdIpCan1ProcessRules( PCRF &pcrf, GxIpCan1 *gxevent );
   ~SdIpCan1ProcessRules();

   EventType getEventType() { return etSdIpCan1ProcessRules; }

   bool processPhase1();               // send the RAR to provision the rules
   bool processPhase2( bool success ); // process the RAA result

   GxIpCan1 *getGxEvent() { return m_gxevent; }

private:
   SdIpCan1ProcessRules();

   GxIpCan1 *m_gxevent;
};

class StIpCan1ProcessRules : public SessionEvent
{
public:
   StIpCan1ProcessRules( PCRF &pcrf, GxIpCan1 *gxevent );
   ~StIpCan1ProcessRules();

   EventType getEventType() { return etStIpCan1ProcessRules; }

   bool processPhase1();               // send the RAR to provision the rules
   bool processPhase2( bool success ); // process the RAA result

   GxIpCan1 *getGxEvent() { return m_gxevent; }

private:
   StIpCan1ProcessRules();

   GxIpCan1 *m_gxevent;
};

class SdTerminateSession : public SessionEvent
{
public:
   SdTerminateSession( GxSession *gxsession );
   ~SdTerminateSession();

   EventType getEventType() { return etSdTerminateSession; }

   bool processPhase1( SdSession::SessionReleaseCause src ); // start establishing the session
   bool processPhase2( bool success ); // session established or failed

   GxSession *getGxSession() { return m_gx; }

private:
   SdTerminateSession();

   GxSession *m_gx;
};

class StTerminateSession : public SessionEvent
{
public:
   StTerminateSession( GxSession *gxsession );
   ~StTerminateSession();

   EventType getEventType() { return etStTerminateSession; }

   bool processPhase1( StSession::TerminationCause tc ); // start establishing the session
   bool processPhase2( bool success ); // session established or failed

   GxSession *getGxSession() { return m_gx; }

private:
   StTerminateSession();

   GxSession *m_gx;
};


class TriggerTimer : public SEventThread
{
public:
	TriggerTimer( GxIpCan1* gxIpCan1, int triggerRARValue, int timer );
	~TriggerTimer();
	void onInit();
   void onQuit();
   void onTimer( SEventThread::Timer &t);
   void dispatch( SEventThreadMessage &msg);
	SEventThread::Timer* getTimer() { return m_reqTimer; }
private :
	GxIpCan1* m_gxipcan1;
	int m_timer;
	int m_triggerRARValue;
	SEventThread::Timer *m_reqTimer;
	
};

class GxIpCan1 : public SessionEvent
{
public:
   GxIpCan1( PCRF &pcrf, FDMessageRequest *req, gx::Dictionary &dict );

   SessionEvent::EventType getEventType() { return SessionEvent::etIpCan1; }

   gx::Dictionary &getDict()                    { return m_dict; }
   gx::CreditControlRequestExtractor &getCCR()  { return m_ccr; }
   FDMessageAnswer &getCCA()                    { return m_cca; }

   GxSession *getGxSession()                    { return m_gxsession; }
   GxSession *setGxSession( GxSession *v )      { m_gxsession = v; return getGxSession(); }
   bool getDeleteGxSession()                    { return m_delete_gxsession; }

   RuleEvaluator &getRulesEvaluator()           { return m_rulesEval; }

   bool processPhase1();   // parse, lookups and initiate TDF/TSSF sessions
                           // a return value of true indicates that processing should continue
                           // a return value of false indicates that processing should halt

   bool processPhase2( bool lockit = true );
                           // evaluate rules and send if good
                           // when sent, then trigger sending rules to PCEF

   bool processPhase3();   // if rules sent successfully to TDF & TSSF generate
                           // positive CCA reply with PCEF rules, otherwise genrate
                           // a CCA error and start the teardown process

   void sendCCA();
   int validate( gx::CreditControlRequestExtractor& ccr );
   int cleanupSession();

	GxSessionState* getCurrentState() { getGxSession()->getCurrentState(); }
	void setCurrentState( GxSessionState* current_state) { getGxSession()->setCurrentState( current_state ); }

	GxSessionProc* getCurrentProc() { getGxSession()->getCurrentProc(); }
	void setCurrentProc( GxSessionProc* current_proc ) { getGxSession()->setCurrentProc( current_proc ); }

	TriggerTimer* getTriggerTimer() { return m_triggertimer; }

   void incrementUsage() { SMutexLock l( m_mutex ); m_usagecnt++; }
   void decrementUsage() { SMutexLock l( m_mutex ); m_usagecnt--; }
   static void release( GxIpCan1 *gxevent );

   void sendRAR( int triggerValue );
	void rcvdRAA( FDMessageAnswer &ans );
   int rcvdInstallRAA( gx::ReAuthAnswerExtractor& raa );
   int rcvdRemoveRAA( gx::ReAuthAnswerExtractor& raa );
	int rcvdDefaultRemoveRAA( gx::ReAuthAnswerExtractor& raa );

private:
   GxIpCan1();
   ~GxIpCan1();

   bool setDeleteGxSession( bool v )            { return m_delete_gxsession = v; }

   SMutex m_mutex;
   int m_usagecnt;

   gx::Dictionary &m_dict;
   gx::CreditControlRequestExtractor m_ccr;
   FDMessageAnswer m_cca;

   GxSession *m_gxsession;
   bool m_delete_gxsession;

   RuleEvaluator m_rulesEval;

   int32_t m_default_bearer_mode;

   SdIpCan1EstablishSession *m_sdEstablishSession;
   StIpCan1EstablishSession *m_stEstablishSession;
   SdIpCan1ProcessRules *m_sdProcessRules;
   StIpCan1ProcessRules *m_stProcessRules;
   //SEventThread::Timer *m_rarTimer;
	TriggerTimer *m_triggertimer;
   //SEventThread::Timer *m_raaTimer;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class GxProcessRulesUpdate : public SessionEvent
{
public:
   GxProcessRulesUpdate( PCRF &pcrf, GxSession *gx );
   ~GxProcessRulesUpdate();

   EventType getEventType() { return etGxProcessRulesUpdate; }

   bool processPhase1( RuleEvaluator &re ); // send the RAR to provision the rules
   bool processPhase2( bool success ); // process the RAA result

   GxSession *getGxSession() { return m_gx; }

private:
   GxProcessRulesUpdate();

   GxSession *m_gx;
   RulesList m_irules;
   RulesList m_rrules;
};

class SdProcessRulesUpdate : public SessionEvent
{
public:
   SdProcessRulesUpdate( PCRF &pcrf, SdSession *sd );
   ~SdProcessRulesUpdate();

   EventType getEventType() { return etSdProcessRulesUpdate; }

   bool processPhase1( RuleEvaluator &re ); // send the RAR to provision the rules
   bool processPhase2( bool success ); // process the RAA result

   SdSession *getSdSession() { return m_sd; }

private:
   SdProcessRulesUpdate();

   SdSession *m_sd;
   RulesList m_irules;
   RulesList m_rrules;
};

class StProcessRulesUpdate : public SessionEvent
{
public:
   StProcessRulesUpdate( PCRF &pcrf, StSession *st );
   ~StProcessRulesUpdate();

   EventType getEventType() { return etStProcessRulesUpdate; }

   bool processPhase1( RuleEvaluator &re ); // send the RAR to provision the rules
   bool processPhase2( bool success ); // process the RAA result

   StSession *getStSession() { return m_st; }

private:
   StProcessRulesUpdate();

   StSession *m_st;
   RulesList m_irules;
   RulesList m_rrules;
};


#endif // __SESSION_H
