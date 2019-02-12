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

#ifndef __S6C_H
#define __S6C_H

#include "fd.h"

namespace s6c {

//
// Primary Objects
//

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    FDDictionaryEntryApplication &app()                     { return m_app; }

    FDDictionaryEntryCommand &cmdSERIFSR()                 	{ return m_cmd_serifsr; }
    FDDictionaryEntryCommand &cmdSERIFSA()                 	{ return m_cmd_serifsa; }
    FDDictionaryEntryCommand &cmdALSCR()                   	{ return m_cmd_alscr; }
    FDDictionaryEntryCommand &cmdALSCA()                   	{ return m_cmd_alsca; }
    FDDictionaryEntryCommand &cmdRESDSR()                  	{ return m_cmd_resdsr; }
    FDDictionaryEntryCommand &cmdRESDSA()                  	{ return m_cmd_resdsa; }

    FDDictionaryEntryVendor &vnd3GPP()                     	{ return m_vnd_3gpp; }

    FDDictionaryEntryAVP &avpAuthApplicationId()           	{ return m_avp_auth_application_id; }
    FDDictionaryEntryAVP &avpSgsnSmDeliveryOutcome()       	{ return m_avp_sgsn_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpProxyInfo()                   	{ return m_avp_proxy_info; }
    FDDictionaryEntryAVP &avpSmRpSmea()                    	{ return m_avp_sm_rp_smea; }
    FDDictionaryEntryAVP &avpFeatureListId()               	{ return m_avp_feature_list_id; }
    FDDictionaryEntryAVP &avpMscAbsentUserDiagnosticSm()   	{ return m_avp_msc_absent_user_diagnostic_sm; }
    FDDictionaryEntryAVP &avpMmeName()                     	{ return m_avp_mme_name; }
    FDDictionaryEntryAVP &avpMaximumUeAvailabilityTime()   	{ return m_avp_maximum_ue_availability_time; }
    FDDictionaryEntryAVP &avpSgsnAbsentUserDiagnosticSm()  	{ return m_avp_sgsn_absent_user_diagnostic_sm; }
    FDDictionaryEntryAVP &avpVendorSpecificApplicationId() 	{ return m_avp_vendor_specific_application_id; }
    FDDictionaryEntryAVP &avpUserIdentifier()              	{ return m_avp_user_identifier; }
    FDDictionaryEntryAVP &avpMmeRealm()                    	{ return m_avp_mme_realm; }
    FDDictionaryEntryAVP &avpSgsnNumber()                  	{ return m_avp_sgsn_number; }
    FDDictionaryEntryAVP &avpServingNode()                 	{ return m_avp_serving_node; }
    FDDictionaryEntryAVP &avpIpSmGwRealm()                 	{ return m_avp_ip_sm_gw_realm; }
    FDDictionaryEntryAVP &avpMmeAbsentUserDiagnosticSm()   	{ return m_avp_mme_absent_user_diagnostic_sm; }
    FDDictionaryEntryAVP &avpAbsentUserDiagnosticSm()      	{ return m_avp_absent_user_diagnostic_sm; }
    FDDictionaryEntryAVP &avpSmDeliveryOutcome()           	{ return m_avp_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpIpSmGwName()                  	{ return m_avp_ip_sm_gw_name; }
    FDDictionaryEntryAVP &avpFeatureList()                 	{ return m_avp_feature_list; }
    FDDictionaryEntryAVP &avpExternalIdentifier()          	{ return m_avp_external_identifier; }
    FDDictionaryEntryAVP &avpFailedAvp()                   	{ return m_avp_failed_avp; }
    FDDictionaryEntryAVP &avpSgsnName()                    	{ return m_avp_sgsn_name; }
    FDDictionaryEntryAVP &avpSmDeliveryNotIntended()       	{ return m_avp_sm_delivery_not_intended; }
    FDDictionaryEntryAVP &avpMsisdn()                      	{ return m_avp_msisdn; }
    FDDictionaryEntryAVP &avpExperimentalResult()          	{ return m_avp_experimental_result; }
    FDDictionaryEntryAVP &avpOriginRealm()                 	{ return m_avp_origin_realm; }
    FDDictionaryEntryAVP &avpOriginHost()                  	{ return m_avp_origin_host; }
    FDDictionaryEntryAVP &avpScAddress()                   	{ return m_avp_sc_address; }
    FDDictionaryEntryAVP &avpResultCode()                  	{ return m_avp_result_code; }
    FDDictionaryEntryAVP &avpSmsGmscAlertEvent()           	{ return m_avp_sms_gmsc_alert_event; }
    FDDictionaryEntryAVP &avpSgsnRealm()                   	{ return m_avp_sgsn_realm; }
    FDDictionaryEntryAVP &avpProxyState()                  	{ return m_avp_proxy_state; }
    FDDictionaryEntryAVP &avpAdditionalServingNode()       	{ return m_avp_additional_serving_node; }
    FDDictionaryEntryAVP &avpMmeNumberForMtSms()           	{ return m_avp_mme_number_for_mt_sms; }
    FDDictionaryEntryAVP &avpSrrFlags()                    	{ return m_avp_srr_flags; }
    FDDictionaryEntryAVP &avpSmsmiCorrelationId()          	{ return m_avp_smsmi_correlation_id; }
    FDDictionaryEntryAVP &avpIpSmGwSmDeliveryOutcome()     	{ return m_avp_ip_sm_gw_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpAuthSessionState()            	{ return m_avp_auth_session_state; }
    FDDictionaryEntryAVP &avpDrmp()                        	{ return m_avp_drmp; }
    FDDictionaryEntryAVP &avpSmRpMti()                     	{ return m_avp_sm_rp_mti; }
    FDDictionaryEntryAVP &avpSupportedFeatures()           	{ return m_avp_supported_features; }
    FDDictionaryEntryAVP &avpOriginatingSipUri()           	{ return m_avp_originating_sip_uri; }
    FDDictionaryEntryAVP &avpRouteRecord()                 	{ return m_avp_route_record; }
    FDDictionaryEntryAVP &avpDestinationSipUri()           	{ return m_avp_destination_sip_uri; }
    FDDictionaryEntryAVP &avpDestinationRealm()            	{ return m_avp_destination_realm; }
    FDDictionaryEntryAVP &avpSessionId()                   	{ return m_avp_session_id; }
    FDDictionaryEntryAVP &avpAcctApplicationId()           	{ return m_avp_acct_application_id; }
    FDDictionaryEntryAVP &avpUserName()                    	{ return m_avp_user_name; }
    FDDictionaryEntryAVP &avpMscSmDeliveryOutcome()        	{ return m_avp_msc_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpMscNumber()                   	{ return m_avp_msc_number; }
    FDDictionaryEntryAVP &avpHssId()                       	{ return m_avp_hss_id; }
    FDDictionaryEntryAVP &avpRdrFlags()                    	{ return m_avp_rdr_flags; }
    FDDictionaryEntryAVP &avpMwdStatus()                   	{ return m_avp_mwd_status; }
    FDDictionaryEntryAVP &avpProxyHost()                   	{ return m_avp_proxy_host; }
    FDDictionaryEntryAVP &avpIpSmGwNumber()                	{ return m_avp_ip_sm_gw_number; }
    FDDictionaryEntryAVP &avpLmsi()                        	{ return m_avp_lmsi; }
    FDDictionaryEntryAVP &avpMmeSmDeliveryOutcome()        	{ return m_avp_mme_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpExperimentalResultCode()      	{ return m_avp_experimental_result_code; }
    FDDictionaryEntryAVP &avpDestinationHost()             	{ return m_avp_destination_host; }
    FDDictionaryEntryAVP &avpSmDeliveryCause()             	{ return m_avp_sm_delivery_cause; }
    FDDictionaryEntryAVP &avpVendorId()                    	{ return m_avp_vendor_id; }

private:
    FDDictionaryEntryApplication m_app;                    

    FDDictionaryEntryCommand m_cmd_serifsr;
    FDDictionaryEntryCommand m_cmd_serifsa;
    FDDictionaryEntryCommand m_cmd_alscr;
    FDDictionaryEntryCommand m_cmd_alsca;
    FDDictionaryEntryCommand m_cmd_resdsr;
    FDDictionaryEntryCommand m_cmd_resdsa;

    FDDictionaryEntryVendor m_vnd_3gpp;

    FDDictionaryEntryAVP m_avp_auth_application_id;
    FDDictionaryEntryAVP m_avp_sgsn_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_proxy_info;
    FDDictionaryEntryAVP m_avp_sm_rp_smea;
    FDDictionaryEntryAVP m_avp_feature_list_id;
    FDDictionaryEntryAVP m_avp_msc_absent_user_diagnostic_sm;
    FDDictionaryEntryAVP m_avp_mme_name;
    FDDictionaryEntryAVP m_avp_maximum_ue_availability_time;
    FDDictionaryEntryAVP m_avp_sgsn_absent_user_diagnostic_sm;
    FDDictionaryEntryAVP m_avp_vendor_specific_application_id;
    FDDictionaryEntryAVP m_avp_user_identifier;
    FDDictionaryEntryAVP m_avp_mme_realm;
    FDDictionaryEntryAVP m_avp_sgsn_number;
    FDDictionaryEntryAVP m_avp_serving_node;
    FDDictionaryEntryAVP m_avp_ip_sm_gw_realm;
    FDDictionaryEntryAVP m_avp_mme_absent_user_diagnostic_sm;
    FDDictionaryEntryAVP m_avp_absent_user_diagnostic_sm;
    FDDictionaryEntryAVP m_avp_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_ip_sm_gw_name;
    FDDictionaryEntryAVP m_avp_feature_list;
    FDDictionaryEntryAVP m_avp_external_identifier;
    FDDictionaryEntryAVP m_avp_failed_avp;
    FDDictionaryEntryAVP m_avp_sgsn_name;
    FDDictionaryEntryAVP m_avp_sm_delivery_not_intended;
    FDDictionaryEntryAVP m_avp_msisdn;
    FDDictionaryEntryAVP m_avp_experimental_result;
    FDDictionaryEntryAVP m_avp_origin_realm;
    FDDictionaryEntryAVP m_avp_origin_host;
    FDDictionaryEntryAVP m_avp_sc_address;
    FDDictionaryEntryAVP m_avp_result_code;
    FDDictionaryEntryAVP m_avp_sms_gmsc_alert_event;
    FDDictionaryEntryAVP m_avp_sgsn_realm;
    FDDictionaryEntryAVP m_avp_proxy_state;
    FDDictionaryEntryAVP m_avp_additional_serving_node;
    FDDictionaryEntryAVP m_avp_mme_number_for_mt_sms;
    FDDictionaryEntryAVP m_avp_srr_flags;
    FDDictionaryEntryAVP m_avp_smsmi_correlation_id;
    FDDictionaryEntryAVP m_avp_ip_sm_gw_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_auth_session_state;
    FDDictionaryEntryAVP m_avp_drmp;
    FDDictionaryEntryAVP m_avp_sm_rp_mti;
    FDDictionaryEntryAVP m_avp_supported_features;
    FDDictionaryEntryAVP m_avp_originating_sip_uri;
    FDDictionaryEntryAVP m_avp_route_record;
    FDDictionaryEntryAVP m_avp_destination_sip_uri;
    FDDictionaryEntryAVP m_avp_destination_realm;
    FDDictionaryEntryAVP m_avp_session_id;
    FDDictionaryEntryAVP m_avp_acct_application_id;
    FDDictionaryEntryAVP m_avp_user_name;
    FDDictionaryEntryAVP m_avp_msc_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_msc_number;
    FDDictionaryEntryAVP m_avp_hss_id;
    FDDictionaryEntryAVP m_avp_rdr_flags;
    FDDictionaryEntryAVP m_avp_mwd_status;
    FDDictionaryEntryAVP m_avp_proxy_host;
    FDDictionaryEntryAVP m_avp_ip_sm_gw_number;
    FDDictionaryEntryAVP m_avp_lmsi;
    FDDictionaryEntryAVP m_avp_mme_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_experimental_result_code;
    FDDictionaryEntryAVP m_avp_destination_host;
    FDDictionaryEntryAVP m_avp_sm_delivery_cause;
    FDDictionaryEntryAVP m_avp_vendor_id;

};

//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - SendRoutingInfoForSmRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// SERIFSR request from Clients.
//-
class SERIFSRcmd : public FDCommandRequest
{
public:
    SERIFSRcmd( Application &app );
    ~SERIFSRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    SERIFSRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - SendRoutingInfoForSmRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a SERIFSR
// to the Diameter application and process the corresponding answer.
//
class SERIFSRreq : public FDMessageRequest
{
public:
    SERIFSRreq( Application &app );
    virtual ~SERIFSRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    SERIFSRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - AlertServiceCentreRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// ALSCR request from Clients.
//-
class ALSCRcmd : public FDCommandRequest
{
public:
    ALSCRcmd( Application &app );
    ~ALSCRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    ALSCRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - AlertServiceCentreRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a ALSCR
// to the Diameter application and process the corresponding answer.
//
class ALSCRreq : public FDMessageRequest
{
public:
    ALSCRreq( Application &app );
    virtual ~ALSCRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    ALSCRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - ReportSmDeliveryStatusRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// RESDSR request from Clients.
//-
class RESDSRcmd : public FDCommandRequest
{
public:
    RESDSRcmd( Application &app );
    ~RESDSRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    RESDSRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - ReportSmDeliveryStatusRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a RESDSR
// to the Diameter application and process the corresponding answer.
//
class RESDSRreq : public FDMessageRequest
{
public:
    RESDSRreq( Application &app );
    virtual ~RESDSRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    RESDSRreq();
    Application &m_app;
    FDSession m_session;
};
 

class ApplicationBase : public FDApplication
{
public:
    ApplicationBase();
    ~ApplicationBase();

    Dictionary &getDict() { return m_dict; }

private:
    Dictionary m_dict;
};

//
// Extractor Patterns
//
//    Single occurrance AVP (not grouped) is an instance of FDExtractorAvp
//    Multi-occurrance AVP (not grouped) is an instance of FDExtractorAvpList
//    Single occurance grouped AVP corresonds to a class that is derived from FDExtractor
//        example: class AccountingRequestExtractor
//    Multi-Occurance grouped AVP corresponds to a class that is derived from FDExtractorList
//        example: class SubscriptionIdExtractorList
//
class SgsnSmDeliveryOutcomeExtractor : public FDExtractor
{
 public:
    SgsnSmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict );
    ~SgsnSmDeliveryOutcomeExtractor();

    FDExtractorAvp sm_delivery_cause;
    FDExtractorAvp absent_user_diagnostic_sm;
};

class ProxyInfoExtractor : public FDExtractor
{
 public:
    ProxyInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProxyInfoExtractor();

    FDExtractorAvp proxy_host;
    FDExtractorAvp proxy_state;
};

class VendorSpecificApplicationIdExtractor : public FDExtractor
{
 public:
    VendorSpecificApplicationIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~VendorSpecificApplicationIdExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp auth_application_id;
    FDExtractorAvp acct_application_id;
};

class UserIdentifierExtractor : public FDExtractor
{
 public:
    UserIdentifierExtractor( FDExtractor &parent, Dictionary &dict );
    ~UserIdentifierExtractor();

    FDExtractorAvp user_name;
    FDExtractorAvp msisdn;
    FDExtractorAvp external_identifier;
};

class ServingNodeExtractor : public FDExtractor
{
 public:
    ServingNodeExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServingNodeExtractor();

    FDExtractorAvp sgsn_name;
    FDExtractorAvp sgsn_realm;
    FDExtractorAvp sgsn_number;
    FDExtractorAvp mme_name;
    FDExtractorAvp mme_realm;
    FDExtractorAvp mme_number_for_mt_sms;
    FDExtractorAvp msc_number;
    FDExtractorAvp ip_sm_gw_number;
    FDExtractorAvp ip_sm_gw_name;
    FDExtractorAvp ip_sm_gw_realm;
};

class MmeSmDeliveryOutcomeExtractor : public FDExtractor
{
 public:
    MmeSmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict );
    ~MmeSmDeliveryOutcomeExtractor();

    FDExtractorAvp sm_delivery_cause;
    FDExtractorAvp absent_user_diagnostic_sm;
};

class MscSmDeliveryOutcomeExtractor : public FDExtractor
{
 public:
    MscSmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict );
    ~MscSmDeliveryOutcomeExtractor();

    FDExtractorAvp sm_delivery_cause;
    FDExtractorAvp absent_user_diagnostic_sm;
};

class IpSmGwSmDeliveryOutcomeExtractor : public FDExtractor
{
 public:
    IpSmGwSmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict );
    ~IpSmGwSmDeliveryOutcomeExtractor();

    FDExtractorAvp sm_delivery_cause;
    FDExtractorAvp absent_user_diagnostic_sm;
};

class SmDeliveryOutcomeExtractor : public FDExtractor
{
 public:
    SmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict );
    ~SmDeliveryOutcomeExtractor();

    MmeSmDeliveryOutcomeExtractor mme_sm_delivery_outcome;
    MscSmDeliveryOutcomeExtractor msc_sm_delivery_outcome;
    SgsnSmDeliveryOutcomeExtractor sgsn_sm_delivery_outcome;
    IpSmGwSmDeliveryOutcomeExtractor ip_sm_gw_sm_delivery_outcome;
};

class FailedAvpExtractor : public FDExtractor
{
 public:
    FailedAvpExtractor( FDExtractor &parent, Dictionary &dict );
    ~FailedAvpExtractor();


};

class ExperimentalResultExtractor : public FDExtractor
{
 public:
    ExperimentalResultExtractor( FDExtractor &parent, Dictionary &dict );
    ~ExperimentalResultExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp experimental_result_code;
};

class AdditionalServingNodeExtractor : public FDExtractor
{
 public:
    AdditionalServingNodeExtractor( FDExtractor &parent, Dictionary &dict );
    ~AdditionalServingNodeExtractor();

    FDExtractorAvp sgsn_name;
    FDExtractorAvp sgsn_realm;
    FDExtractorAvp sgsn_number;
    FDExtractorAvp mme_name;
    FDExtractorAvp mme_realm;
    FDExtractorAvp mme_number_for_mt_sms;
    FDExtractorAvp msc_number;
};

class SmsmiCorrelationIdExtractor : public FDExtractor
{
 public:
    SmsmiCorrelationIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~SmsmiCorrelationIdExtractor();

    FDExtractorAvp hss_id;
    FDExtractorAvp originating_sip_uri;
    FDExtractorAvp destination_sip_uri;
};

class SupportedFeaturesExtractor : public FDExtractor
{
 public:
    SupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict );
    ~SupportedFeaturesExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp feature_list_id;
    FDExtractorAvp feature_list;
};

class SupportedFeaturesExtractorList : public FDExtractorList
{
 public:
    SupportedFeaturesExtractorList( FDExtractor &parent, Dictionary &dict );
    ~SupportedFeaturesExtractorList();

    FDExtractor *createExtractor() { return new SupportedFeaturesExtractor( getParent(), m_dict ); }
    std::list<SupportedFeaturesExtractor*> &getList() { return (std::list<SupportedFeaturesExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ProxyInfoExtractorList : public FDExtractorList
{
 public:
    ProxyInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ProxyInfoExtractorList();

    FDExtractor *createExtractor() { return new ProxyInfoExtractor( getParent(), m_dict ); }
    std::list<ProxyInfoExtractor*> &getList() { return (std::list<ProxyInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class SendRoutingInfoForSmRequestExtractor : public FDExtractor
{
 public:
    SendRoutingInfoForSmRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~SendRoutingInfoForSmRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    FDExtractorAvp msisdn;
    FDExtractorAvp user_name;
    SmsmiCorrelationIdExtractor smsmi_correlation_id;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp sc_address;
    FDExtractorAvp sm_rp_mti;
    FDExtractorAvp sm_rp_smea;
    FDExtractorAvp srr_flags;
    FDExtractorAvp sm_delivery_not_intended;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class SendRoutingInfoForSmAnswerExtractor : public FDExtractor
{
 public:
    SendRoutingInfoForSmAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~SendRoutingInfoForSmAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp user_name;
    SupportedFeaturesExtractorList supported_features;
    ServingNodeExtractor serving_node;
    AdditionalServingNodeExtractor additional_serving_node;
    FDExtractorAvp lmsi;
    UserIdentifierExtractor user_identifier;
    FDExtractorAvp mwd_status;
    FDExtractorAvp mme_absent_user_diagnostic_sm;
    FDExtractorAvp msc_absent_user_diagnostic_sm;
    FDExtractorAvp sgsn_absent_user_diagnostic_sm;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class AlertServiceCentreRequestExtractor : public FDExtractor
{
 public:
    AlertServiceCentreRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~AlertServiceCentreRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    FDExtractorAvp sc_address;
    UserIdentifierExtractor user_identifier;
    SmsmiCorrelationIdExtractor smsmi_correlation_id;
    FDExtractorAvp maximum_ue_availability_time;
    FDExtractorAvp sms_gmsc_alert_event;
    ServingNodeExtractor serving_node;
    SupportedFeaturesExtractorList supported_features;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class AlertServiceCentreAnswerExtractor : public FDExtractor
{
 public:
    AlertServiceCentreAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~AlertServiceCentreAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    SupportedFeaturesExtractorList supported_features;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class ReportSmDeliveryStatusRequestExtractor : public FDExtractor
{
 public:
    ReportSmDeliveryStatusRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~ReportSmDeliveryStatusRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    SupportedFeaturesExtractorList supported_features;
    UserIdentifierExtractor user_identifier;
    SmsmiCorrelationIdExtractor smsmi_correlation_id;
    FDExtractorAvp sc_address;
    SmDeliveryOutcomeExtractor sm_delivery_outcome;
    FDExtractorAvp rdr_flags;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class ReportSmDeliveryStatusAnswerExtractor : public FDExtractor
{
 public:
    ReportSmDeliveryStatusAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~ReportSmDeliveryStatusAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    SupportedFeaturesExtractorList supported_features;
    UserIdentifierExtractor user_identifier;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};


}

#endif // __S6C_H

