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

#ifndef __SGD_H
#define __SGD_H

#include "fd.h"

namespace sgd {

//
// Primary Objects
//

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    FDDictionaryEntryApplication &app()                     { return m_app; }

    FDDictionaryEntryCommand &cmdMOFSMR()                  	{ return m_cmd_mofsmr; }
    FDDictionaryEntryCommand &cmdMOFSMA()                  	{ return m_cmd_mofsma; }
    FDDictionaryEntryCommand &cmdMTFSMR()                  	{ return m_cmd_mtfsmr; }
    FDDictionaryEntryCommand &cmdMTFSMA()                  	{ return m_cmd_mtfsma; }
    FDDictionaryEntryCommand &cmdALSCR()                   	{ return m_cmd_alscr; }
    FDDictionaryEntryCommand &cmdALSCA()                   	{ return m_cmd_alsca; }

    FDDictionaryEntryVendor &vnd3GPP()                     	{ return m_vnd_3gpp; }

    FDDictionaryEntryAVP &avpAuthApplicationId()           	{ return m_avp_auth_application_id; }
    FDDictionaryEntryAVP &avpSgsnSmDeliveryOutcome()       	{ return m_avp_sgsn_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpProxyInfo()                   	{ return m_avp_proxy_info; }
    FDDictionaryEntryAVP &avpMmeRealm()                    	{ return m_avp_mme_realm; }
    FDDictionaryEntryAVP &avpFeatureListId()               	{ return m_avp_feature_list_id; }
    FDDictionaryEntryAVP &avpMmeName()                     	{ return m_avp_mme_name; }
    FDDictionaryEntryAVP &avpTfrFlags()                    	{ return m_avp_tfr_flags; }
    FDDictionaryEntryAVP &avpVendorSpecificApplicationId() 	{ return m_avp_vendor_specific_application_id; }
    FDDictionaryEntryAVP &avpSmsGmscAddress()              	{ return m_avp_sms_gmsc_address; }
    FDDictionaryEntryAVP &avpMaximumUeAvailabilityTime()   	{ return m_avp_maximum_ue_availability_time; }
    FDDictionaryEntryAVP &avpMaximumRetransmissionTime()   	{ return m_avp_maximum_retransmission_time; }
    FDDictionaryEntryAVP &avpSmDeliveryStartTime()         	{ return m_avp_sm_delivery_start_time; }
    FDDictionaryEntryAVP &avpUserIdentifier()              	{ return m_avp_user_identifier; }
    FDDictionaryEntryAVP &avpSgsnNumber()                  	{ return m_avp_sgsn_number; }
    FDDictionaryEntryAVP &avpIpSmGwRealm()                 	{ return m_avp_ip_sm_gw_realm; }
    FDDictionaryEntryAVP &avpSmDeliveryOutcome()           	{ return m_avp_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpIpSmGwName()                  	{ return m_avp_ip_sm_gw_name; }
    FDDictionaryEntryAVP &avpFeatureList()                 	{ return m_avp_feature_list; }
    FDDictionaryEntryAVP &avpExternalIdentifier()          	{ return m_avp_external_identifier; }
    FDDictionaryEntryAVP &avpFailedAvp()                   	{ return m_avp_failed_avp; }
    FDDictionaryEntryAVP &avpSgsnName()                    	{ return m_avp_sgsn_name; }
    FDDictionaryEntryAVP &avpAbsentUserDiagnosticSm()      	{ return m_avp_absent_user_diagnostic_sm; }
    FDDictionaryEntryAVP &avpMsisdn()                      	{ return m_avp_msisdn; }
    FDDictionaryEntryAVP &avpExperimentalResult()          	{ return m_avp_experimental_result; }
    FDDictionaryEntryAVP &avpOriginRealm()                 	{ return m_avp_origin_realm; }
    FDDictionaryEntryAVP &avpOriginHost()                  	{ return m_avp_origin_host; }
    FDDictionaryEntryAVP &avpScAddress()                   	{ return m_avp_sc_address; }
    FDDictionaryEntryAVP &avpResultCode()                  	{ return m_avp_result_code; }
    FDDictionaryEntryAVP &avpSmsGmscAlertEvent()           	{ return m_avp_sms_gmsc_alert_event; }
    FDDictionaryEntryAVP &avpSmRpUi()                      	{ return m_avp_sm_rp_ui; }
    FDDictionaryEntryAVP &avpSgsnRealm()                   	{ return m_avp_sgsn_realm; }
    FDDictionaryEntryAVP &avpProxyState()                  	{ return m_avp_proxy_state; }
    FDDictionaryEntryAVP &avpMmeNumberForMtSms()           	{ return m_avp_mme_number_for_mt_sms; }
    FDDictionaryEntryAVP &avpOfrFlags()                    	{ return m_avp_ofr_flags; }
    FDDictionaryEntryAVP &avpSmsmiCorrelationId()          	{ return m_avp_smsmi_correlation_id; }
    FDDictionaryEntryAVP &avpServingNode()                 	{ return m_avp_serving_node; }
    FDDictionaryEntryAVP &avpIpSmGwSmDeliveryOutcome()     	{ return m_avp_ip_sm_gw_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpAuthSessionState()            	{ return m_avp_auth_session_state; }
    FDDictionaryEntryAVP &avpDrmp()                        	{ return m_avp_drmp; }
    FDDictionaryEntryAVP &avpVendorId()                    	{ return m_avp_vendor_id; }
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
    FDDictionaryEntryAVP &avpSmDeliveryTimer()             	{ return m_avp_sm_delivery_timer; }
    FDDictionaryEntryAVP &avpProxyHost()                   	{ return m_avp_proxy_host; }
    FDDictionaryEntryAVP &avpIpSmGwNumber()                	{ return m_avp_ip_sm_gw_number; }
    FDDictionaryEntryAVP &avpMmeSmDeliveryOutcome()        	{ return m_avp_mme_sm_delivery_outcome; }
    FDDictionaryEntryAVP &avpExperimentalResultCode()      	{ return m_avp_experimental_result_code; }
    FDDictionaryEntryAVP &avpDestinationHost()             	{ return m_avp_destination_host; }
    FDDictionaryEntryAVP &avpSmDeliveryCause()             	{ return m_avp_sm_delivery_cause; }

private:
    FDDictionaryEntryApplication m_app;                    

    FDDictionaryEntryCommand m_cmd_mofsmr;
    FDDictionaryEntryCommand m_cmd_mofsma;
    FDDictionaryEntryCommand m_cmd_mtfsmr;
    FDDictionaryEntryCommand m_cmd_mtfsma;
    FDDictionaryEntryCommand m_cmd_alscr;
    FDDictionaryEntryCommand m_cmd_alsca;

    FDDictionaryEntryVendor m_vnd_3gpp;

    FDDictionaryEntryAVP m_avp_auth_application_id;
    FDDictionaryEntryAVP m_avp_sgsn_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_proxy_info;
    FDDictionaryEntryAVP m_avp_mme_realm;
    FDDictionaryEntryAVP m_avp_feature_list_id;
    FDDictionaryEntryAVP m_avp_mme_name;
    FDDictionaryEntryAVP m_avp_tfr_flags;
    FDDictionaryEntryAVP m_avp_vendor_specific_application_id;
    FDDictionaryEntryAVP m_avp_sms_gmsc_address;
    FDDictionaryEntryAVP m_avp_maximum_ue_availability_time;
    FDDictionaryEntryAVP m_avp_maximum_retransmission_time;
    FDDictionaryEntryAVP m_avp_sm_delivery_start_time;
    FDDictionaryEntryAVP m_avp_user_identifier;
    FDDictionaryEntryAVP m_avp_sgsn_number;
    FDDictionaryEntryAVP m_avp_ip_sm_gw_realm;
    FDDictionaryEntryAVP m_avp_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_ip_sm_gw_name;
    FDDictionaryEntryAVP m_avp_feature_list;
    FDDictionaryEntryAVP m_avp_external_identifier;
    FDDictionaryEntryAVP m_avp_failed_avp;
    FDDictionaryEntryAVP m_avp_sgsn_name;
    FDDictionaryEntryAVP m_avp_absent_user_diagnostic_sm;
    FDDictionaryEntryAVP m_avp_msisdn;
    FDDictionaryEntryAVP m_avp_experimental_result;
    FDDictionaryEntryAVP m_avp_origin_realm;
    FDDictionaryEntryAVP m_avp_origin_host;
    FDDictionaryEntryAVP m_avp_sc_address;
    FDDictionaryEntryAVP m_avp_result_code;
    FDDictionaryEntryAVP m_avp_sms_gmsc_alert_event;
    FDDictionaryEntryAVP m_avp_sm_rp_ui;
    FDDictionaryEntryAVP m_avp_sgsn_realm;
    FDDictionaryEntryAVP m_avp_proxy_state;
    FDDictionaryEntryAVP m_avp_mme_number_for_mt_sms;
    FDDictionaryEntryAVP m_avp_ofr_flags;
    FDDictionaryEntryAVP m_avp_smsmi_correlation_id;
    FDDictionaryEntryAVP m_avp_serving_node;
    FDDictionaryEntryAVP m_avp_ip_sm_gw_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_auth_session_state;
    FDDictionaryEntryAVP m_avp_drmp;
    FDDictionaryEntryAVP m_avp_vendor_id;
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
    FDDictionaryEntryAVP m_avp_sm_delivery_timer;
    FDDictionaryEntryAVP m_avp_proxy_host;
    FDDictionaryEntryAVP m_avp_ip_sm_gw_number;
    FDDictionaryEntryAVP m_avp_mme_sm_delivery_outcome;
    FDDictionaryEntryAVP m_avp_experimental_result_code;
    FDDictionaryEntryAVP m_avp_destination_host;
    FDDictionaryEntryAVP m_avp_sm_delivery_cause;

};

//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - MoForwardShortMessageRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// MOFSMR request from Clients.
//-
class MOFSMRcmd : public FDCommandRequest
{
public:
    MOFSMRcmd( Application &app );
    ~MOFSMRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    MOFSMRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - MoForwardShortMessageRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a MOFSMR
// to the Diameter application and process the corresponding answer.
//
class MOFSMRreq : public FDMessageRequest
{
public:
    MOFSMRreq( Application &app );
    virtual ~MOFSMRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    MOFSMRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - MtForwardShortMessageRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// MTFSMR request from Clients.
//-
class MTFSMRcmd : public FDCommandRequest
{
public:
    MTFSMRcmd( Application &app );
    ~MTFSMRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    MTFSMRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - MtForwardShortMessageRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a MTFSMR
// to the Diameter application and process the corresponding answer.
//
class MTFSMRreq : public FDMessageRequest
{
public:
    MTFSMRreq( Application &app );
    virtual ~MTFSMRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    MTFSMRreq();
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

class SmsmiCorrelationIdExtractor : public FDExtractor
{
 public:
    SmsmiCorrelationIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~SmsmiCorrelationIdExtractor();

    FDExtractorAvp hss_id;
    FDExtractorAvp originating_sip_uri;
    FDExtractorAvp destination_sip_uri;
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

class MoForwardShortMessageRequestExtractor : public FDExtractor
{
 public:
    MoForwardShortMessageRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~MoForwardShortMessageRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    FDExtractorAvp sc_address;
    FDExtractorAvp ofr_flags;
    SupportedFeaturesExtractorList supported_features;
    UserIdentifierExtractor user_identifier;
    FDExtractorAvp sm_rp_ui;
    SmsmiCorrelationIdExtractor smsmi_correlation_id;
    SmDeliveryOutcomeExtractor sm_delivery_outcome;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class MoForwardShortMessageAnswerExtractor : public FDExtractor
{
 public:
    MoForwardShortMessageAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~MoForwardShortMessageAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    SupportedFeaturesExtractorList supported_features;
};

class MtForwardShortMessageRequestExtractor : public FDExtractor
{
 public:
    MtForwardShortMessageRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~MtForwardShortMessageRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    FDExtractorAvp user_name;
    SupportedFeaturesExtractorList supported_features;
    SmsmiCorrelationIdExtractor smsmi_correlation_id;
    FDExtractorAvp sc_address;
    FDExtractorAvp sm_rp_ui;
    FDExtractorAvp mme_number_for_mt_sms;
    FDExtractorAvp sgsn_number;
    FDExtractorAvp tfr_flags;
    FDExtractorAvp sm_delivery_timer;
    FDExtractorAvp sm_delivery_start_time;
    FDExtractorAvp maximum_retransmission_time;
    FDExtractorAvp sms_gmsc_address;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class MtForwardShortMessageAnswerExtractor : public FDExtractor
{
 public:
    MtForwardShortMessageAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~MtForwardShortMessageAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp absent_user_diagnostic_sm;
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


}

#endif // __SGD_H

