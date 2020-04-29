/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "rx.h"
#include "rx_impl.h"

namespace rx {

Dictionary::Dictionary()
    : m_app( "Rx"),
    m_cmd_aar( "AA-Request" ),
    m_cmd_aaa( "AA-Answer" ),
    m_cmd_rear( "Re-Auth-Request" ),
    m_cmd_reaa( "Re-Auth-Answer" ),
    m_cmd_setr( "Session-Termination-Request" ),
    m_cmd_seta( "Session-Termination-Answer" ),
    m_cmd_absr( "Abort-Session-Request" ),
    m_cmd_absa( "Abort-Session-Answer" ),
    m_vnd_Etsi( "ETSI" ),
    m_vnd_3gpp( "3GPP" ),
    m_avp_mcptt_identifier( "MCPTT-Identifier", m_vnd_3gpp.getId() ),
    m_avp_redirect_host_usage( "Redirect-Host-Usage" ),
    m_avp_af_requested_data( "AF-Requested-Data", m_vnd_3gpp.getId() ),
    m_avp_redirect_max_cache_time( "Redirect-Max-Cache-Time" ),
    m_avp_termination_cause( "Termination-Cause" ),
    m_avp_exponent( "Exponent" ),
    m_avp_sourceid( "SourceID" ),
    m_avp_load_value( "Load-Value" ),
    m_avp_access_network_charging_identifier_value( "Access-Network-Charging-Identifier-Value", m_vnd_3gpp.getId() ),
    m_avp_feature_list_id( "Feature-List-ID", m_vnd_3gpp.getId() ),
    m_avp_experimental_result( "Experimental-Result" ),
    m_avp_load( "Load" ),
    m_avp_oc_feature_vector( "OC-Feature-Vector" ),
    m_avp_specific_action( "Specific-Action", m_vnd_3gpp.getId() ),
    m_avp_auth_session_state( "Auth-Session-State" ),
    m_avp_cc_money( "CC-Money" ),
    m_avp_cc_total_octets( "CC-Total-Octets" ),
    m_avp_reporting_reason( "Reporting-Reason", m_vnd_3gpp.getId() ),
    m_avp_load_type( "Load-Type" ),
    m_avp_an_gw_address( "AN-GW-Address", m_vnd_3gpp.getId() ),
    m_avp_tcp_source_port( "TCP-Source-Port", m_vnd_3gpp.getId() ),
    m_avp_destination_host( "Destination-Host" ),
    m_avp_service_authorization_info( "Service-Authorization-Info", m_vnd_3gpp.getId() ),
    m_avp_gcs_identifier( "GCS-Identifier", m_vnd_3gpp.getId() ),
    m_avp_retry_interval( "Retry-Interval", m_vnd_3gpp.getId() ),
    m_avp_af_application_identifier( "AF-Application-Identifier", m_vnd_3gpp.getId() ),
    m_avp_ue_local_ip_address( "UE-Local-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_media_component_status( "Media-Component-Status", m_vnd_3gpp.getId() ),
    m_avp_sip_forking_indication( "SIP-Forking-Indication", m_vnd_3gpp.getId() ),
    m_avp_netloc_access_support( "NetLoc-Access-Support", m_vnd_3gpp.getId() ),
    m_avp_mps_identifier( "MPS-Identifier", m_vnd_3gpp.getId() ),
    m_avp_sponsored_connectivity_data( "Sponsored-Connectivity-Data", m_vnd_3gpp.getId() ),
    m_avp_access_network_charging_address( "Access-Network-Charging-Address", m_vnd_3gpp.getId() ),
    m_avp_value_digits( "Value-Digits" ),
    m_avp_user_equipment_info_type( "User-Equipment-Info-Type" ),
    m_avp_framed_ipv6_prefix( "Framed-IPv6-Prefix" ),
    m_avp_rx_request_type( "Rx-Request-Type", m_vnd_3gpp.getId() ),
    m_avp_proxy_info( "Proxy-Info" ),
    m_avp_used_service_unit( "Used-Service-Unit" ),
    m_avp_origin_realm( "Origin-Realm" ),
    m_avp_twan_identifier( "TWAN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_origin_host( "Origin-Host" ),
    m_avp_class( "Class" ),
    m_avp_subscription_id_data( "Subscription-Id-Data" ),
    m_avp_ip_domain_id( "IP-Domain-Id", m_vnd_3gpp.getId() ),
    m_avp_user_equipment_info_value( "User-Equipment-Info-Value" ),
    m_avp_route_record( "Route-Record" ),
    m_avp_framed_ip_address( "Framed-IP-Address" ),
    m_avp_oc_olr( "OC-OLR" ),
    m_avp_subscription_id_type( "Subscription-Id-Type" ),
    m_avp_feature_list( "Feature-List", m_vnd_3gpp.getId() ),
    m_avp_sponsor_identity( "Sponsor-Identity", m_vnd_3gpp.getId() ),
    m_avp_oc_reduction_percentage( "OC-Reduction-Percentage" ),
    m_avp_error_reporting_host( "Error-Reporting-Host" ),
    m_avp_3gpp_sgsn_mcc_mnc( "3GPP-SGSN-MCC-MNC", m_vnd_3gpp.getId() ),
    m_avp_flow_number( "Flow-Number", m_vnd_3gpp.getId() ),
    m_avp_rat_type( "RAT-Type", m_vnd_3gpp.getId() ),
    m_avp_event_charging_timestamp( "Event-Charging-TimeStamp", m_vnd_3gpp.getId() ),
    m_avp_application_service_provider_identity( "Application-Service-Provider-Identity", m_vnd_3gpp.getId() ),
    m_avp_oc_validity_duration( "OC-Validity-Duration" ),
    m_avp_oc_sequence_number( "OC-Sequence-Number" ),
    m_avp_redirect_host( "Redirect-Host" ),
    m_avp_result_code( "Result-Code" ),
    m_avp_user_equipment_info( "User-Equipment-Info" ),
    m_avp_origin_state_id( "Origin-State-Id" ),
    m_avp_cc_output_octets( "CC-Output-Octets" ),
    m_avp_ran_nas_release_cause( "RAN-NAS-Release-Cause", m_vnd_3gpp.getId() ),
    m_avp_reservation_priority( "Reservation-Priority", m_vnd_Etsi.getId() ),
    m_avp_3gpp_user_location_info( "3GPP-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_max_requested_bandwidth_dl( "Max-Requested-Bandwidth-DL", m_vnd_3gpp.getId() ),
    m_avp_tariff_change_usage( "Tariff-Change-Usage" ),
    m_avp_pre_emption_control_info( "Pre-emption-Control-Info", m_vnd_3gpp.getId() ),
    m_avp_acceptable_service_info( "Acceptable-Service-Info", m_vnd_3gpp.getId() ),
    m_avp_abort_cause( "Abort-Cause", m_vnd_3gpp.getId() ),
    m_avp_an_trusted( "AN-Trusted", m_vnd_3gpp.getId() ),
    m_avp_called_station_id( "Called-Station-Id" ),
    m_avp_service_urn( "Service-URN", m_vnd_3gpp.getId() ),
    m_avp_oc_supported_features( "OC-Supported-Features" ),
    m_avp_proxy_host( "Proxy-Host" ),
    m_avp_content_version( "Content-Version", m_vnd_3gpp.getId() ),
    m_avp_user_location_info_time( "User-Location-Info-Time", m_vnd_3gpp.getId() ),
    m_avp_experimental_result_code( "Experimental-Result-Code" ),
    m_avp_auth_application_id( "Auth-Application-Id" ),
    m_avp_3gpp_ms_timezone( "3GPP-MS-TimeZone", m_vnd_3gpp.getId() ),
    m_avp_subscription_id( "Subscription-Id" ),
    m_avp_ip_can_type( "IP-CAN-Type", m_vnd_3gpp.getId() ),
    m_avp_udp_source_port( "UDP-Source-Port", m_vnd_3gpp.getId() ),
    m_avp_media_component_number( "Media-Component-Number", m_vnd_3gpp.getId() ),
    m_avp_tariff_time_change( "Tariff-Time-Change" ),
    m_avp_sponsoring_action( "Sponsoring-Action", m_vnd_3gpp.getId() ),
    m_avp_error_message( "Error-Message" ),
    m_avp_required_access_info( "Required-Access-Info", m_vnd_3gpp.getId() ),
    m_avp_failed_avp( "Failed-AVP" ),
    m_avp_flows( "Flows", m_vnd_3gpp.getId() ),
    m_avp_supported_features( "Supported-Features", m_vnd_3gpp.getId() ),
    m_avp_cc_service_specific_units( "CC-Service-Specific-Units" ),
    m_avp_cc_time( "CC-Time" ),
    m_avp_currency_code( "Currency-Code" ),
    m_avp_cc_input_octets( "CC-Input-Octets" ),
    m_avp_media_component_description( "Media-Component-Description", m_vnd_3gpp.getId() ),
    m_avp_proxy_state( "Proxy-State" ),
    m_avp_access_network_charging_identifier( "Access-Network-Charging-Identifier", m_vnd_3gpp.getId() ),
    m_avp_af_charging_identifier( "AF-Charging-Identifier", m_vnd_3gpp.getId() ),
    m_avp_reference_id( "Reference-Id", m_vnd_3gpp.getId() ),
    m_avp_vendor_id( "Vendor-Id" ),
    m_avp_granted_service_unit( "Granted-Service-Unit" ),
    m_avp_max_requested_bandwidth_ul( "Max-Requested-Bandwidth-UL", m_vnd_3gpp.getId() ),
    m_avp_unit_value( "Unit-Value" ),
    m_avp_final_unit_action( "Final-Unit-Action" ),
    m_avp_service_info_status( "Service-Info-Status", m_vnd_3gpp.getId() ),
    m_avp_destination_realm( "Destination-Realm" ),
    m_avp_session_id( "Session-Id" ),
    m_avp_drmp( "DRMP" ),
    m_avp_oc_report_type( "OC-Report-Type" )
{
    std::cout << "Registering rx dictionary" << std::endl;
};

Dictionary::~Dictionary()
{
};

AARcmd::AARcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdAAR() ),
      m_app( app )
{
}

AARcmd::~AARcmd()
{
}

Dictionary &AARcmd::getDict()
{
   return m_app.getDict();
}

AARreq::AARreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdAAR() ),
      m_app(app)
{
}

AARreq::~AARreq()
{
}

Dictionary &AARreq::getDict()
{
   return m_app.getDict();
}

 
REARcmd::REARcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdREAR() ),
      m_app( app )
{
}

REARcmd::~REARcmd()
{
}

Dictionary &REARcmd::getDict()
{
   return m_app.getDict();
}

REARreq::REARreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdREAR() ),
      m_app(app)
{
}

REARreq::~REARreq()
{
}

Dictionary &REARreq::getDict()
{
   return m_app.getDict();
}

 
SETRcmd::SETRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdSETR() ),
      m_app( app )
{
}

SETRcmd::~SETRcmd()
{
}

Dictionary &SETRcmd::getDict()
{
   return m_app.getDict();
}

SETRreq::SETRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdSETR() ),
      m_app(app)
{
}

SETRreq::~SETRreq()
{
}

Dictionary &SETRreq::getDict()
{
   return m_app.getDict();
}

 
ABSRcmd::ABSRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdABSR() ),
      m_app( app )
{
}

ABSRcmd::~ABSRcmd()
{
}

Dictionary &ABSRcmd::getDict()
{
   return m_app.getDict();
}

ABSRreq::ABSRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdABSR() ),
      m_app(app)
{
}

ABSRreq::~ABSRreq()
{
}

Dictionary &ABSRreq::getDict()
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

LoadExtractor::LoadExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLoad() ),
    load_type( *this, dict.avpLoadType() ),
    load_value( *this, dict.avpLoadValue() ),
    sourceid( *this, dict.avpSourceid() )
{
    add( load_type );
    add( load_value );
    add( sourceid );
}

LoadExtractor::~LoadExtractor()
{
}

UnitValueExtractor::UnitValueExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUnitValue() ),
    value_digits( *this, dict.avpValueDigits() ),
    exponent( *this, dict.avpExponent() )
{
    add( value_digits );
    add( exponent );
}

UnitValueExtractor::~UnitValueExtractor()
{
}

CcMoneyExtractor::CcMoneyExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCcMoney() ),
    unit_value( *this, dict ),
    currency_code( *this, dict.avpCurrencyCode() )
{
    add( unit_value );
    add( currency_code );
}

CcMoneyExtractor::~CcMoneyExtractor()
{
}

GrantedServiceUnitExtractor::GrantedServiceUnitExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpGrantedServiceUnit() ),
    tariff_time_change( *this, dict.avpTariffTimeChange() ),
    cc_time( *this, dict.avpCcTime() ),
    cc_money( *this, dict ),
    cc_total_octets( *this, dict.avpCcTotalOctets() ),
    cc_input_octets( *this, dict.avpCcInputOctets() ),
    cc_output_octets( *this, dict.avpCcOutputOctets() ),
    cc_service_specific_units( *this, dict.avpCcServiceSpecificUnits() )
{
    add( tariff_time_change );
    add( cc_time );
    add( cc_money );
    add( cc_total_octets );
    add( cc_input_octets );
    add( cc_output_octets );
    add( cc_service_specific_units );
}

GrantedServiceUnitExtractor::~GrantedServiceUnitExtractor()
{
}

UsedServiceUnitExtractor::UsedServiceUnitExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUsedServiceUnit() ),
    reporting_reason( *this, dict.avpReportingReason() ),
    tariff_change_usage( *this, dict.avpTariffChangeUsage() ),
    cc_time( *this, dict.avpCcTime() ),
    cc_money( *this, dict ),
    cc_total_octets( *this, dict.avpCcTotalOctets() ),
    cc_input_octets( *this, dict.avpCcInputOctets() ),
    cc_output_octets( *this, dict.avpCcOutputOctets() ),
    cc_service_specific_units( *this, dict.avpCcServiceSpecificUnits() ),
    event_charging_timestamp( *this, dict.avpEventChargingTimestamp() )
{
    add( reporting_reason );
    add( tariff_change_usage );
    add( cc_time );
    add( cc_money );
    add( cc_total_octets );
    add( cc_input_octets );
    add( cc_output_octets );
    add( cc_service_specific_units );
    add( event_charging_timestamp );
}

UsedServiceUnitExtractor::~UsedServiceUnitExtractor()
{
}

SponsoredConnectivityDataExtractor::SponsoredConnectivityDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSponsoredConnectivityData() ),
    sponsor_identity( *this, dict.avpSponsorIdentity() ),
    application_service_provider_identity( *this, dict.avpApplicationServiceProviderIdentity() ),
    granted_service_unit( *this, dict ),
    used_service_unit( *this, dict ),
    sponsoring_action( *this, dict.avpSponsoringAction() )
{
    add( sponsor_identity );
    add( application_service_provider_identity );
    add( granted_service_unit );
    add( used_service_unit );
    add( sponsoring_action );
}

SponsoredConnectivityDataExtractor::~SponsoredConnectivityDataExtractor()
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

OcOlrExtractor::OcOlrExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpOcOlr() ),
    oc_sequence_number( *this, dict.avpOcSequenceNumber() ),
    oc_report_type( *this, dict.avpOcReportType() ),
    oc_reduction_percentage( *this, dict.avpOcReductionPercentage() ),
    oc_validity_duration( *this, dict.avpOcValidityDuration() )
{
    add( oc_sequence_number );
    add( oc_report_type );
    add( oc_reduction_percentage );
    add( oc_validity_duration );
}

OcOlrExtractor::~OcOlrExtractor()
{
}

UserEquipmentInfoExtractor::UserEquipmentInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUserEquipmentInfo() ),
    user_equipment_info_type( *this, dict.avpUserEquipmentInfoType() ),
    user_equipment_info_value( *this, dict.avpUserEquipmentInfoValue() )
{
    add( user_equipment_info_type );
    add( user_equipment_info_value );
}

UserEquipmentInfoExtractor::~UserEquipmentInfoExtractor()
{
}

MediaComponentDescriptionExtractor::MediaComponentDescriptionExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMediaComponentDescription() ),
    media_component_number( *this, dict.avpMediaComponentNumber() )
{
    add( media_component_number );
}

MediaComponentDescriptionExtractor::~MediaComponentDescriptionExtractor()
{
}

MediaComponentDescriptionExtractorList::MediaComponentDescriptionExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpMediaComponentDescription() ),
    m_dict( dict )
{
}

MediaComponentDescriptionExtractorList::~MediaComponentDescriptionExtractorList()
{
}

AcceptableServiceInfoExtractor::AcceptableServiceInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAcceptableServiceInfo() ),
    media_component_description( *this, dict ),
    max_requested_bandwidth_dl( *this, dict.avpMaxRequestedBandwidthDl() ),
    max_requested_bandwidth_ul( *this, dict.avpMaxRequestedBandwidthUl() )
{
    add( media_component_description );
    add( max_requested_bandwidth_dl );
    add( max_requested_bandwidth_ul );
}

AcceptableServiceInfoExtractor::~AcceptableServiceInfoExtractor()
{
}

OcSupportedFeaturesExtractor::OcSupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpOcSupportedFeatures() ),
    oc_feature_vector( *this, dict.avpOcFeatureVector() )
{
    add( oc_feature_vector );
}

OcSupportedFeaturesExtractor::~OcSupportedFeaturesExtractor()
{
}

SubscriptionIdExtractor::SubscriptionIdExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSubscriptionId() ),
    subscription_id_type( *this, dict.avpSubscriptionIdType() ),
    subscription_id_data( *this, dict.avpSubscriptionIdData() )
{
    add( subscription_id_type );
    add( subscription_id_data );
}

SubscriptionIdExtractor::~SubscriptionIdExtractor()
{
}

FailedAvpExtractor::FailedAvpExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFailedAvp() )
{

}

FailedAvpExtractor::~FailedAvpExtractor()
{
}

FlowsExtractor::FlowsExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFlows() ),
    media_component_number( *this, dict.avpMediaComponentNumber() ),
    flow_number( *this, dict.avpFlowNumber() ),
    content_version( *this, dict.avpContentVersion() ),
    final_unit_action( *this, dict.avpFinalUnitAction() ),
    media_component_status( *this, dict.avpMediaComponentStatus() )
{
    add( media_component_number );
    add( flow_number );
    add( content_version );
    add( final_unit_action );
    add( media_component_status );
}

FlowsExtractor::~FlowsExtractor()
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

FlowsExtractorList::FlowsExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpFlows() ),
    m_dict( dict )
{
}

FlowsExtractorList::~FlowsExtractorList()
{
}

AccessNetworkChargingIdentifierExtractor::AccessNetworkChargingIdentifierExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAccessNetworkChargingIdentifier() ),
    access_network_charging_identifier_value( *this, dict.avpAccessNetworkChargingIdentifierValue() ),
    flows( *this, dict )
{
    add( access_network_charging_identifier_value );
    add( flows );
}

AccessNetworkChargingIdentifierExtractor::~AccessNetworkChargingIdentifierExtractor()
{
}

SubscriptionIdExtractorList::SubscriptionIdExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpSubscriptionId() ),
    m_dict( dict )
{
}

SubscriptionIdExtractorList::~SubscriptionIdExtractorList()
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

AaRequestExtractor::AaRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    ip_domain_id( *this, dict.avpIpDomainId() ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    af_application_identifier( *this, dict.avpAfApplicationIdentifier() ),
    media_component_description( *this, dict ),
    service_info_status( *this, dict.avpServiceInfoStatus() ),
    af_charging_identifier( *this, dict.avpAfChargingIdentifier() ),
    sip_forking_indication( *this, dict.avpSipForkingIndication() ),
    specific_action( *this, dict.avpSpecificAction() ),
    subscription_id( *this, dict ),
    oc_supported_features( *this, dict ),
    supported_features( *this, dict ),
    reservation_priority( *this, dict.avpReservationPriority() ),
    framed_ip_address( *this, dict.avpFramedIpAddress() ),
    framed_ipv6_prefix( *this, dict.avpFramedIpv6Prefix() ),
    called_station_id( *this, dict.avpCalledStationId() ),
    service_urn( *this, dict.avpServiceUrn() ),
    sponsored_connectivity_data( *this, dict ),
    mps_identifier( *this, dict.avpMpsIdentifier() ),
    gcs_identifier( *this, dict.avpGcsIdentifier() ),
    mcptt_identifier( *this, dict.avpMcpttIdentifier() ),
    rx_request_type( *this, dict.avpRxRequestType() ),
    required_access_info( *this, dict.avpRequiredAccessInfo() ),
    af_requested_data( *this, dict.avpAfRequestedData() ),
    reference_id( *this, dict.avpReferenceId() ),
    pre_emption_control_info( *this, dict.avpPreEmptionControlInfo() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( auth_application_id );
    add( origin_host );
    add( origin_realm );
    add( destination_realm );
    add( destination_host );
    add( ip_domain_id );
    add( auth_session_state );
    add( af_application_identifier );
    add( media_component_description );
    add( service_info_status );
    add( af_charging_identifier );
    add( sip_forking_indication );
    add( specific_action );
    add( subscription_id );
    add( oc_supported_features );
    add( supported_features );
    add( reservation_priority );
    add( framed_ip_address );
    add( framed_ipv6_prefix );
    add( called_station_id );
    add( service_urn );
    add( sponsored_connectivity_data );
    add( mps_identifier );
    add( gcs_identifier );
    add( mcptt_identifier );
    add( rx_request_type );
    add( required_access_info );
    add( af_requested_data );
    add( reference_id );
    add( pre_emption_control_info );
    add( origin_state_id );
    add( proxy_info );
    add( route_record );
}

AaRequestExtractor::~AaRequestExtractor()
{
}

AccessNetworkChargingIdentifierExtractorList::AccessNetworkChargingIdentifierExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAccessNetworkChargingIdentifier() ),
    m_dict( dict )
{
}

AccessNetworkChargingIdentifierExtractorList::~AccessNetworkChargingIdentifierExtractorList()
{
}

LoadExtractorList::LoadExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpLoad() ),
    m_dict( dict )
{
}

LoadExtractorList::~LoadExtractorList()
{
}

AaAnswerExtractor::AaAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    access_network_charging_identifier( *this, dict ),
    access_network_charging_address( *this, dict.avpAccessNetworkChargingAddress() ),
    acceptable_service_info( *this, dict ),
    an_gw_address( *this, dict.avpAnGwAddress() ),
    an_trusted( *this, dict.avpAnTrusted() ),
    service_authorization_info( *this, dict.avpServiceAuthorizationInfo() ),
    ip_can_type( *this, dict.avpIpCanType() ),
    netloc_access_support( *this, dict.avpNetlocAccessSupport() ),
    rat_type( *this, dict.avpRatType() ),
    flows( *this, dict ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    supported_features( *this, dict ),
    subscription_id( *this, dict ),
    user_equipment_info( *this, dict ),
    tgpp_sgsn_mcc_mnc( *this, dict.avp3gppSgsnMccMnc() ),
    _class( *this, dict.avpClass() ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    failed_avp( *this, dict ),
    retry_interval( *this, dict.avpRetryInterval() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    redirect_host( *this, dict.avpRedirectHost() ),
    redirect_host_usage( *this, dict.avpRedirectHostUsage() ),
    redirect_max_cache_time( *this, dict.avpRedirectMaxCacheTime() ),
    proxy_info( *this, dict ),
    load( *this, dict )
{
    add( session_id );
    add( drmp );
    add( auth_application_id );
    add( origin_host );
    add( origin_realm );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( access_network_charging_identifier );
    add( access_network_charging_address );
    add( acceptable_service_info );
    add( an_gw_address );
    add( an_trusted );
    add( service_authorization_info );
    add( ip_can_type );
    add( netloc_access_support );
    add( rat_type );
    add( flows );
    add( oc_supported_features );
    add( oc_olr );
    add( supported_features );
    add( subscription_id );
    add( user_equipment_info );
    add( tgpp_sgsn_mcc_mnc );
    add( _class );
    add( error_message );
    add( error_reporting_host );
    add( failed_avp );
    add( retry_interval );
    add( origin_state_id );
    add( redirect_host );
    add( redirect_host_usage );
    add( redirect_max_cache_time );
    add( proxy_info );
    add( load );
}

AaAnswerExtractor::~AaAnswerExtractor()
{
}

ReAuthRequestExtractor::ReAuthRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    specific_action( *this, dict.avpSpecificAction() ),
    oc_supported_features( *this, dict ),
    access_network_charging_identifier( *this, dict ),
    access_network_charging_address( *this, dict.avpAccessNetworkChargingAddress() ),
    an_gw_address( *this, dict.avpAnGwAddress() ),
    an_trusted( *this, dict.avpAnTrusted() ),
    flows( *this, dict ),
    subscription_id( *this, dict ),
    abort_cause( *this, dict.avpAbortCause() ),
    ip_can_type( *this, dict.avpIpCanType() ),
    netloc_access_support( *this, dict.avpNetlocAccessSupport() ),
    rat_type( *this, dict.avpRatType() ),
    sponsored_connectivity_data( *this, dict ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    user_location_info_time( *this, dict.avpUserLocationInfoTime() ),
    tgpp_ms_timezone( *this, dict.avp3gppMsTimezone() ),
    ran_nas_release_cause( *this, dict.avpRanNasReleaseCause() ),
    tgpp_sgsn_mcc_mnc( *this, dict.avp3gppSgsnMccMnc() ),
    twan_identifier( *this, dict.avpTwanIdentifier() ),
    tcp_source_port( *this, dict.avpTcpSourcePort() ),
    udp_source_port( *this, dict.avpUdpSourcePort() ),
    ue_local_ip_address( *this, dict.avpUeLocalIpAddress() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    _class( *this, dict.avpClass() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( origin_host );
    add( origin_realm );
    add( destination_realm );
    add( destination_host );
    add( auth_application_id );
    add( specific_action );
    add( oc_supported_features );
    add( access_network_charging_identifier );
    add( access_network_charging_address );
    add( an_gw_address );
    add( an_trusted );
    add( flows );
    add( subscription_id );
    add( abort_cause );
    add( ip_can_type );
    add( netloc_access_support );
    add( rat_type );
    add( sponsored_connectivity_data );
    add( tgpp_user_location_info );
    add( user_location_info_time );
    add( tgpp_ms_timezone );
    add( ran_nas_release_cause );
    add( tgpp_sgsn_mcc_mnc );
    add( twan_identifier );
    add( tcp_source_port );
    add( udp_source_port );
    add( ue_local_ip_address );
    add( origin_state_id );
    add( _class );
    add( proxy_info );
    add( route_record );
}

ReAuthRequestExtractor::~ReAuthRequestExtractor()
{
}

ReAuthAnswerExtractor::ReAuthAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    media_component_description( *this, dict ),
    service_urn( *this, dict.avpServiceUrn() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    _class( *this, dict.avpClass() ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    redirect_host( *this, dict.avpRedirectHost() ),
    redirect_host_usage( *this, dict.avpRedirectHostUsage() ),
    redirect_max_cache_time( *this, dict.avpRedirectMaxCacheTime() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict )
{
    add( session_id );
    add( drmp );
    add( origin_host );
    add( origin_realm );
    add( result_code );
    add( experimental_result );
    add( oc_supported_features );
    add( oc_olr );
    add( media_component_description );
    add( service_urn );
    add( origin_state_id );
    add( _class );
    add( error_message );
    add( error_reporting_host );
    add( redirect_host );
    add( redirect_host_usage );
    add( redirect_max_cache_time );
    add( failed_avp );
    add( proxy_info );
}

ReAuthAnswerExtractor::~ReAuthAnswerExtractor()
{
}

SessionTerminationRequestExtractor::SessionTerminationRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    termination_cause( *this, dict.avpTerminationCause() ),
    destination_host( *this, dict.avpDestinationHost() ),
    oc_supported_features( *this, dict ),
    required_access_info( *this, dict.avpRequiredAccessInfo() ),
    _class( *this, dict.avpClass() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( origin_host );
    add( origin_realm );
    add( destination_realm );
    add( auth_application_id );
    add( termination_cause );
    add( destination_host );
    add( oc_supported_features );
    add( required_access_info );
    add( _class );
    add( origin_state_id );
    add( proxy_info );
    add( route_record );
}

SessionTerminationRequestExtractor::~SessionTerminationRequestExtractor()
{
}

SessionTerminationAnswerExtractor::SessionTerminationAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    result_code( *this, dict.avpResultCode() ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    failed_avp( *this, dict ),
    sponsored_connectivity_data( *this, dict ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    user_location_info_time( *this, dict.avpUserLocationInfoTime() ),
    tgpp_ms_timezone( *this, dict.avp3gppMsTimezone() ),
    ran_nas_release_cause( *this, dict.avpRanNasReleaseCause() ),
    tgpp_sgsn_mcc_mnc( *this, dict.avp3gppSgsnMccMnc() ),
    twan_identifier( *this, dict.avpTwanIdentifier() ),
    tcp_source_port( *this, dict.avpTcpSourcePort() ),
    udp_source_port( *this, dict.avpUdpSourcePort() ),
    ue_local_ip_address( *this, dict.avpUeLocalIpAddress() ),
    netloc_access_support( *this, dict.avpNetlocAccessSupport() ),
    _class( *this, dict.avpClass() ),
    redirect_host( *this, dict.avpRedirectHost() ),
    redirect_host_usage( *this, dict.avpRedirectHostUsage() ),
    redirect_max_cache_time( *this, dict.avpRedirectMaxCacheTime() ),
    proxy_info( *this, dict ),
    load( *this, dict )
{
    add( session_id );
    add( drmp );
    add( origin_host );
    add( origin_realm );
    add( result_code );
    add( error_message );
    add( error_reporting_host );
    add( oc_supported_features );
    add( oc_olr );
    add( failed_avp );
    add( sponsored_connectivity_data );
    add( origin_state_id );
    add( tgpp_user_location_info );
    add( user_location_info_time );
    add( tgpp_ms_timezone );
    add( ran_nas_release_cause );
    add( tgpp_sgsn_mcc_mnc );
    add( twan_identifier );
    add( tcp_source_port );
    add( udp_source_port );
    add( ue_local_ip_address );
    add( netloc_access_support );
    add( _class );
    add( redirect_host );
    add( redirect_host_usage );
    add( redirect_max_cache_time );
    add( proxy_info );
    add( load );
}

SessionTerminationAnswerExtractor::~SessionTerminationAnswerExtractor()
{
}

AbortSessionRequestExtractor::AbortSessionRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    oc_supported_features( *this, dict ),
    abort_cause( *this, dict.avpAbortCause() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( origin_host );
    add( origin_realm );
    add( destination_realm );
    add( destination_host );
    add( auth_application_id );
    add( oc_supported_features );
    add( abort_cause );
    add( origin_state_id );
    add( proxy_info );
    add( route_record );
}

AbortSessionRequestExtractor::~AbortSessionRequestExtractor()
{
}

AbortSessionAnswerExtractor::AbortSessionAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    result_code( *this, dict.avpResultCode() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    failed_avp( *this, dict ),
    redirect_host( *this, dict.avpRedirectHost() ),
    redirect_host_usage( *this, dict.avpRedirectHostUsage() ),
    redirect_max_cache_time( *this, dict.avpRedirectMaxCacheTime() ),
    proxy_info( *this, dict )
{
    add( session_id );
    add( drmp );
    add( origin_host );
    add( origin_realm );
    add( result_code );
    add( oc_supported_features );
    add( oc_olr );
    add( origin_state_id );
    add( error_message );
    add( error_reporting_host );
    add( failed_avp );
    add( redirect_host );
    add( redirect_host_usage );
    add( redirect_max_cache_time );
    add( proxy_info );
}

AbortSessionAnswerExtractor::~AbortSessionAnswerExtractor()
{
}


}
