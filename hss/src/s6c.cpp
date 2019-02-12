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

#include <string>
#include <iostream>
#include <sstream>

#include "s6c.h"
#include "s6c_impl.h"

namespace s6c {

Dictionary::Dictionary()
    : m_app( "S6c"),
    m_cmd_serifsr( "Send-Routing-Info-for-SM-Request" ),
    m_cmd_serifsa( "Send-Routing-Info-for-SM-Answer" ),
    m_cmd_alscr( "Alert-Service-Centre-Request" ),
    m_cmd_alsca( "Alert-Service-Centre-Answer" ),
    m_cmd_resdsr( "Report-SM-Delivery-Status-Request" ),
    m_cmd_resdsa( "Report-SM-Delivery-Status-Answer" ),
    m_vnd_3gpp( "3GPP" ),
    m_avp_auth_application_id( "Auth-Application-Id" ),
    m_avp_sgsn_sm_delivery_outcome( "SGSN-SM-Delivery-Outcome", m_vnd_3gpp.getId() ),
    m_avp_proxy_info( "Proxy-Info" ),
    m_avp_sm_rp_smea( "SM-RP-SMEA", m_vnd_3gpp.getId() ),
    m_avp_feature_list_id( "Feature-List-ID", m_vnd_3gpp.getId() ),
    m_avp_msc_absent_user_diagnostic_sm( "MSC-Absent-User-Diagnostic-SM", m_vnd_3gpp.getId() ),
    m_avp_mme_name( "MME-Name", m_vnd_3gpp.getId() ),
    m_avp_maximum_ue_availability_time( "Maximum-UE-Availability-Time", m_vnd_3gpp.getId() ),
    m_avp_sgsn_absent_user_diagnostic_sm( "SGSN-Absent-User-Diagnostic-SM", m_vnd_3gpp.getId() ),
    m_avp_vendor_specific_application_id( "Vendor-Specific-Application-Id" ),
    m_avp_user_identifier( "User-Identifier", m_vnd_3gpp.getId() ),
    m_avp_mme_realm( "MME-Realm", m_vnd_3gpp.getId() ),
    m_avp_sgsn_number( "SGSN-Number", m_vnd_3gpp.getId() ),
    m_avp_serving_node( "Serving-Node", m_vnd_3gpp.getId() ),
    m_avp_ip_sm_gw_realm( "IP-SM-GW-Realm", m_vnd_3gpp.getId() ),
    m_avp_mme_absent_user_diagnostic_sm( "MME-Absent-User-Diagnostic-SM", m_vnd_3gpp.getId() ),
    m_avp_absent_user_diagnostic_sm( "Absent-User-Diagnostic-SM", m_vnd_3gpp.getId() ),
    m_avp_sm_delivery_outcome( "SM-Delivery-Outcome", m_vnd_3gpp.getId() ),
    m_avp_ip_sm_gw_name( "IP-SM-GW-Name", m_vnd_3gpp.getId() ),
    m_avp_feature_list( "Feature-List", m_vnd_3gpp.getId() ),
    m_avp_external_identifier( "External-Identifier", m_vnd_3gpp.getId() ),
    m_avp_failed_avp( "Failed-AVP" ),
    m_avp_sgsn_name( "SGSN-Name", m_vnd_3gpp.getId() ),
    m_avp_sm_delivery_not_intended( "SM-Delivery-Not-Intended", m_vnd_3gpp.getId() ),
    m_avp_msisdn( "MSISDN", m_vnd_3gpp.getId() ),
    m_avp_experimental_result( "Experimental-Result" ),
    m_avp_origin_realm( "Origin-Realm" ),
    m_avp_origin_host( "Origin-Host" ),
    m_avp_sc_address( "SC-Address", m_vnd_3gpp.getId() ),
    m_avp_result_code( "Result-Code" ),
    m_avp_sms_gmsc_alert_event( "SMS-GMSC-Alert-Event", m_vnd_3gpp.getId() ),
    m_avp_sgsn_realm( "SGSN-Realm", m_vnd_3gpp.getId() ),
    m_avp_proxy_state( "Proxy-State" ),
    m_avp_additional_serving_node( "Additional-Serving-Node", m_vnd_3gpp.getId() ),
    m_avp_mme_number_for_mt_sms( "MME-Number-for-MT-SMS", m_vnd_3gpp.getId() ),
    m_avp_srr_flags( "SRR-Flags", m_vnd_3gpp.getId() ),
    m_avp_smsmi_correlation_id( "SMSMI-Correlation-ID", m_vnd_3gpp.getId() ),
    m_avp_ip_sm_gw_sm_delivery_outcome( "IP-SM-GW-SM-Delivery-Outcome", m_vnd_3gpp.getId() ),
    m_avp_auth_session_state( "Auth-Session-State" ),
    m_avp_drmp( "DRMP" ),
    m_avp_sm_rp_mti( "SM-RP-MTI", m_vnd_3gpp.getId() ),
    m_avp_supported_features( "Supported-Features", m_vnd_3gpp.getId() ),
    m_avp_originating_sip_uri( "Originating-SIP-URI", m_vnd_3gpp.getId() ),
    m_avp_route_record( "Route-Record" ),
    m_avp_destination_sip_uri( "Destination-SIP-URI", m_vnd_3gpp.getId() ),
    m_avp_destination_realm( "Destination-Realm" ),
    m_avp_session_id( "Session-Id" ),
    m_avp_acct_application_id( "Acct-Application-Id" ),
    m_avp_user_name( "User-Name" ),
    m_avp_msc_sm_delivery_outcome( "MSC-SM-Delivery-Outcome", m_vnd_3gpp.getId() ),
    m_avp_msc_number( "MSC-Number", m_vnd_3gpp.getId() ),
    m_avp_hss_id( "HSS-ID", m_vnd_3gpp.getId() ),
    m_avp_rdr_flags( "RDR-Flags", m_vnd_3gpp.getId() ),
    m_avp_mwd_status( "MWD-Status", m_vnd_3gpp.getId() ),
    m_avp_proxy_host( "Proxy-Host" ),
    m_avp_ip_sm_gw_number( "IP-SM-GW-Number", m_vnd_3gpp.getId() ),
    m_avp_lmsi( "LMSI", m_vnd_3gpp.getId() ),
    m_avp_mme_sm_delivery_outcome( "MME-SM-Delivery-Outcome", m_vnd_3gpp.getId() ),
    m_avp_experimental_result_code( "Experimental-Result-Code" ),
    m_avp_destination_host( "Destination-Host" ),
    m_avp_sm_delivery_cause( "SM-Delivery-Cause", m_vnd_3gpp.getId() ),
    m_avp_vendor_id( "Vendor-Id" )
{
    std::cout << "Registering s6c dictionary" << std::endl;
};

Dictionary::~Dictionary()
{
};

SERIFSRcmd::SERIFSRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdSERIFSR() ),
      m_app( app )
{
}

SERIFSRcmd::~SERIFSRcmd()
{
}

Dictionary &SERIFSRcmd::getDict()
{
   return m_app.getDict();
}

SERIFSRreq::SERIFSRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdSERIFSR() ),
      m_app(app)
{
}

SERIFSRreq::~SERIFSRreq()
{
}

Dictionary &SERIFSRreq::getDict()
{
   return m_app.getDict();
}

 
ALSCRcmd::ALSCRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdALSCR() ),
      m_app( app )
{
}

ALSCRcmd::~ALSCRcmd()
{
}

Dictionary &ALSCRcmd::getDict()
{
   return m_app.getDict();
}

ALSCRreq::ALSCRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdALSCR() ),
      m_app(app)
{
}

ALSCRreq::~ALSCRreq()
{
}

Dictionary &ALSCRreq::getDict()
{
   return m_app.getDict();
}

 
RESDSRcmd::RESDSRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdRESDSR() ),
      m_app( app )
{
}

RESDSRcmd::~RESDSRcmd()
{
}

Dictionary &RESDSRcmd::getDict()
{
   return m_app.getDict();
}

RESDSRreq::RESDSRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdRESDSR() ),
      m_app(app)
{
}

RESDSRreq::~RESDSRreq()
{
}

Dictionary &RESDSRreq::getDict()
{
   return m_app.getDict();
}

 
ApplicationBase::ApplicationBase()
{
    setDictionaryEntry( &m_dict.app() );

};

ApplicationBase::~ApplicationBase()
{
};



SgsnSmDeliveryOutcomeExtractor::SgsnSmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSgsnSmDeliveryOutcome() ),
    sm_delivery_cause( *this, dict.avpSmDeliveryCause() ),
    absent_user_diagnostic_sm( *this, dict.avpAbsentUserDiagnosticSm() )
{
    add( sm_delivery_cause );
    add( absent_user_diagnostic_sm );
}

SgsnSmDeliveryOutcomeExtractor::~SgsnSmDeliveryOutcomeExtractor()
{
}

ProxyInfoExtractor::ProxyInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpProxyInfo() ),
    proxy_host( *this, dict.avpProxyHost() ),
    proxy_state( *this, dict.avpProxyState() )
{
    add( proxy_host );
    add( proxy_state );
}

ProxyInfoExtractor::~ProxyInfoExtractor()
{
}

VendorSpecificApplicationIdExtractor::VendorSpecificApplicationIdExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpVendorSpecificApplicationId() ),
    vendor_id( *this, dict.avpVendorId() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    acct_application_id( *this, dict.avpAcctApplicationId() )
{
    add( vendor_id );
    add( auth_application_id );
    add( acct_application_id );
}

VendorSpecificApplicationIdExtractor::~VendorSpecificApplicationIdExtractor()
{
}

UserIdentifierExtractor::UserIdentifierExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUserIdentifier() ),
    user_name( *this, dict.avpUserName() ),
    msisdn( *this, dict.avpMsisdn() ),
    external_identifier( *this, dict.avpExternalIdentifier() )
{
    add( user_name );
    add( msisdn );
    add( external_identifier );
}

UserIdentifierExtractor::~UserIdentifierExtractor()
{
}

ServingNodeExtractor::ServingNodeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServingNode() ),
    sgsn_name( *this, dict.avpSgsnName() ),
    sgsn_realm( *this, dict.avpSgsnRealm() ),
    sgsn_number( *this, dict.avpSgsnNumber() ),
    mme_name( *this, dict.avpMmeName() ),
    mme_realm( *this, dict.avpMmeRealm() ),
    mme_number_for_mt_sms( *this, dict.avpMmeNumberForMtSms() ),
    msc_number( *this, dict.avpMscNumber() ),
    ip_sm_gw_number( *this, dict.avpIpSmGwNumber() ),
    ip_sm_gw_name( *this, dict.avpIpSmGwName() ),
    ip_sm_gw_realm( *this, dict.avpIpSmGwRealm() )
{
    add( sgsn_name );
    add( sgsn_realm );
    add( sgsn_number );
    add( mme_name );
    add( mme_realm );
    add( mme_number_for_mt_sms );
    add( msc_number );
    add( ip_sm_gw_number );
    add( ip_sm_gw_name );
    add( ip_sm_gw_realm );
}

ServingNodeExtractor::~ServingNodeExtractor()
{
}

MmeSmDeliveryOutcomeExtractor::MmeSmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMmeSmDeliveryOutcome() ),
    sm_delivery_cause( *this, dict.avpSmDeliveryCause() ),
    absent_user_diagnostic_sm( *this, dict.avpAbsentUserDiagnosticSm() )
{
    add( sm_delivery_cause );
    add( absent_user_diagnostic_sm );
}

MmeSmDeliveryOutcomeExtractor::~MmeSmDeliveryOutcomeExtractor()
{
}

MscSmDeliveryOutcomeExtractor::MscSmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMscSmDeliveryOutcome() ),
    sm_delivery_cause( *this, dict.avpSmDeliveryCause() ),
    absent_user_diagnostic_sm( *this, dict.avpAbsentUserDiagnosticSm() )
{
    add( sm_delivery_cause );
    add( absent_user_diagnostic_sm );
}

MscSmDeliveryOutcomeExtractor::~MscSmDeliveryOutcomeExtractor()
{
}

IpSmGwSmDeliveryOutcomeExtractor::IpSmGwSmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpIpSmGwSmDeliveryOutcome() ),
    sm_delivery_cause( *this, dict.avpSmDeliveryCause() ),
    absent_user_diagnostic_sm( *this, dict.avpAbsentUserDiagnosticSm() )
{
    add( sm_delivery_cause );
    add( absent_user_diagnostic_sm );
}

IpSmGwSmDeliveryOutcomeExtractor::~IpSmGwSmDeliveryOutcomeExtractor()
{
}

SmDeliveryOutcomeExtractor::SmDeliveryOutcomeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSmDeliveryOutcome() ),
    mme_sm_delivery_outcome( *this, dict ),
    msc_sm_delivery_outcome( *this, dict ),
    sgsn_sm_delivery_outcome( *this, dict ),
    ip_sm_gw_sm_delivery_outcome( *this, dict )
{
    add( mme_sm_delivery_outcome );
    add( msc_sm_delivery_outcome );
    add( sgsn_sm_delivery_outcome );
    add( ip_sm_gw_sm_delivery_outcome );
}

SmDeliveryOutcomeExtractor::~SmDeliveryOutcomeExtractor()
{
}

FailedAvpExtractor::FailedAvpExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFailedAvp() )
{

}

FailedAvpExtractor::~FailedAvpExtractor()
{
}

ExperimentalResultExtractor::ExperimentalResultExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpExperimentalResult() ),
    vendor_id( *this, dict.avpVendorId() ),
    experimental_result_code( *this, dict.avpExperimentalResultCode() )
{
    add( vendor_id );
    add( experimental_result_code );
}

ExperimentalResultExtractor::~ExperimentalResultExtractor()
{
}

AdditionalServingNodeExtractor::AdditionalServingNodeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAdditionalServingNode() ),
    sgsn_name( *this, dict.avpSgsnName() ),
    sgsn_realm( *this, dict.avpSgsnRealm() ),
    sgsn_number( *this, dict.avpSgsnNumber() ),
    mme_name( *this, dict.avpMmeName() ),
    mme_realm( *this, dict.avpMmeRealm() ),
    mme_number_for_mt_sms( *this, dict.avpMmeNumberForMtSms() ),
    msc_number( *this, dict.avpMscNumber() )
{
    add( sgsn_name );
    add( sgsn_realm );
    add( sgsn_number );
    add( mme_name );
    add( mme_realm );
    add( mme_number_for_mt_sms );
    add( msc_number );
}

AdditionalServingNodeExtractor::~AdditionalServingNodeExtractor()
{
}

SmsmiCorrelationIdExtractor::SmsmiCorrelationIdExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSmsmiCorrelationId() ),
    hss_id( *this, dict.avpHssId() ),
    originating_sip_uri( *this, dict.avpOriginatingSipUri() ),
    destination_sip_uri( *this, dict.avpDestinationSipUri() )
{
    add( hss_id );
    add( originating_sip_uri );
    add( destination_sip_uri );
}

SmsmiCorrelationIdExtractor::~SmsmiCorrelationIdExtractor()
{
}

SupportedFeaturesExtractor::SupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSupportedFeatures() ),
    vendor_id( *this, dict.avpVendorId() ),
    feature_list_id( *this, dict.avpFeatureListId() ),
    feature_list( *this, dict.avpFeatureList() )
{
    add( vendor_id );
    add( feature_list_id );
    add( feature_list );
}

SupportedFeaturesExtractor::~SupportedFeaturesExtractor()
{
}

SupportedFeaturesExtractorList::SupportedFeaturesExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpSupportedFeatures() ),
    m_dict( dict )
{
}

SupportedFeaturesExtractorList::~SupportedFeaturesExtractorList()
{
}

ProxyInfoExtractorList::ProxyInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpProxyInfo() ),
    m_dict( dict )
{
}

ProxyInfoExtractorList::~ProxyInfoExtractorList()
{
}

SendRoutingInfoForSmRequestExtractor::SendRoutingInfoForSmRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    msisdn( *this, dict.avpMsisdn() ),
    user_name( *this, dict.avpUserName() ),
    smsmi_correlation_id( *this, dict ),
    supported_features( *this, dict ),
    sc_address( *this, dict.avpScAddress() ),
    sm_rp_mti( *this, dict.avpSmRpMti() ),
    sm_rp_smea( *this, dict.avpSmRpSmea() ),
    srr_flags( *this, dict.avpSrrFlags() ),
    sm_delivery_not_intended( *this, dict.avpSmDeliveryNotIntended() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( destination_host );
    add( destination_realm );
    add( msisdn );
    add( user_name );
    add( smsmi_correlation_id );
    add( supported_features );
    add( sc_address );
    add( sm_rp_mti );
    add( sm_rp_smea );
    add( srr_flags );
    add( sm_delivery_not_intended );
    add( proxy_info );
    add( route_record );
}

SendRoutingInfoForSmRequestExtractor::~SendRoutingInfoForSmRequestExtractor()
{
}

SendRoutingInfoForSmAnswerExtractor::SendRoutingInfoForSmAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    user_name( *this, dict.avpUserName() ),
    supported_features( *this, dict ),
    serving_node( *this, dict ),
    additional_serving_node( *this, dict ),
    lmsi( *this, dict.avpLmsi() ),
    user_identifier( *this, dict ),
    mwd_status( *this, dict.avpMwdStatus() ),
    mme_absent_user_diagnostic_sm( *this, dict.avpMmeAbsentUserDiagnosticSm() ),
    msc_absent_user_diagnostic_sm( *this, dict.avpMscAbsentUserDiagnosticSm() ),
    sgsn_absent_user_diagnostic_sm( *this, dict.avpSgsnAbsentUserDiagnosticSm() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( user_name );
    add( supported_features );
    add( serving_node );
    add( additional_serving_node );
    add( lmsi );
    add( user_identifier );
    add( mwd_status );
    add( mme_absent_user_diagnostic_sm );
    add( msc_absent_user_diagnostic_sm );
    add( sgsn_absent_user_diagnostic_sm );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

SendRoutingInfoForSmAnswerExtractor::~SendRoutingInfoForSmAnswerExtractor()
{
}

AlertServiceCentreRequestExtractor::AlertServiceCentreRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    sc_address( *this, dict.avpScAddress() ),
    user_identifier( *this, dict ),
    smsmi_correlation_id( *this, dict ),
    maximum_ue_availability_time( *this, dict.avpMaximumUeAvailabilityTime() ),
    sms_gmsc_alert_event( *this, dict.avpSmsGmscAlertEvent() ),
    serving_node( *this, dict ),
    supported_features( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( destination_host );
    add( destination_realm );
    add( sc_address );
    add( user_identifier );
    add( smsmi_correlation_id );
    add( maximum_ue_availability_time );
    add( sms_gmsc_alert_event );
    add( serving_node );
    add( supported_features );
    add( proxy_info );
    add( route_record );
}

AlertServiceCentreRequestExtractor::~AlertServiceCentreRequestExtractor()
{
}

AlertServiceCentreAnswerExtractor::AlertServiceCentreAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    supported_features( *this, dict ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( supported_features );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

AlertServiceCentreAnswerExtractor::~AlertServiceCentreAnswerExtractor()
{
}

ReportSmDeliveryStatusRequestExtractor::ReportSmDeliveryStatusRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    supported_features( *this, dict ),
    user_identifier( *this, dict ),
    smsmi_correlation_id( *this, dict ),
    sc_address( *this, dict.avpScAddress() ),
    sm_delivery_outcome( *this, dict ),
    rdr_flags( *this, dict.avpRdrFlags() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( destination_host );
    add( destination_realm );
    add( supported_features );
    add( user_identifier );
    add( smsmi_correlation_id );
    add( sc_address );
    add( sm_delivery_outcome );
    add( rdr_flags );
    add( proxy_info );
    add( route_record );
}

ReportSmDeliveryStatusRequestExtractor::~ReportSmDeliveryStatusRequestExtractor()
{
}

ReportSmDeliveryStatusAnswerExtractor::ReportSmDeliveryStatusAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    supported_features( *this, dict ),
    user_identifier( *this, dict ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( supported_features );
    add( user_identifier );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

ReportSmDeliveryStatusAnswerExtractor::~ReportSmDeliveryStatusAnswerExtractor()
{
}


}
