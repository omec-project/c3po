/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "sd.h"
#include "sd_impl.h"

namespace sd {

Dictionary::Dictionary()
    : m_app( "Sd"),
    m_cmd_tsr( "TS-Request" ),
    m_cmd_tsa( "TS-Answer" ),
    m_cmd_crcr( "Credit-Control-Request" ),
    m_cmd_crca( "Credit-Control-Answer" ),
    m_cmd_rear( "Re-Auth-Request" ),
    m_cmd_reaa( "Re-Auth-Answer" ),
    m_vnd_Etsi( "ETSI" ),
    m_vnd_3gpp2( "3GPP2" ),
    m_vnd_3gpp( "3GPP" ),
    m_avp_pre_emption_capability( "Pre-emption-Capability", m_vnd_3gpp.getId() ),
    m_avp_feature_list_id( "Feature-List-ID", m_vnd_3gpp.getId() ),
    m_avp_service_identifier( "Service-Identifier" ),
    m_avp_vendor_specific_application_id( "Vendor-Specific-Application-Id" ),
    m_avp_csg_access_mode( "CSG-Access-Mode", m_vnd_3gpp.getId() ),
    m_avp_henb_local_ip_address( "HeNB-Local-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_dynamic_address_flag_extension( "Dynamic-Address-Flag-Extension", m_vnd_3gpp.getId() ),
    m_avp_apn_aggregate_max_bitrate_ul( "APN-Aggregate-Max-Bitrate-UL", m_vnd_3gpp.getId() ),
    m_avp_3gpp_sgsn_ipv6_address( "3GPP-SGSN-Ipv6-Address", m_vnd_3gpp.getId() ),
    m_avp_exponent( "Exponent" ),
    m_avp_packet_filter_usage( "Packet-Filter-Usage", m_vnd_3gpp.getId() ),
    m_avp_oc_sequence_number( "OC-Sequence-Number" ),
    m_avp_usage_monitoring_support( "Usage-Monitoring-Support", m_vnd_3gpp.getId() ),
    m_avp_tracking_area_identity( "Tracking-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_load_value( "Load-Value" ),
    m_avp_feature_list( "Feature-List", m_vnd_3gpp.getId() ),
    m_avp_omc_id( "OMC-Id", m_vnd_3gpp.getId() ),
    m_avp_rai( "RAI", m_vnd_3gpp.getId() ),
    m_avp_oc_report_type( "OC-Report-Type" ),
    m_avp_experimental_result( "Experimental-Result" ),
    m_avp_secondary_event_charging_function_name( "Secondary-Event-Charging-Function-Name", m_vnd_3gpp.getId() ),
    m_avp_load_type( "Load-Type" ),
    m_avp_redirect_host_usage( "Redirect-Host-Usage" ),
    m_avp_oc_feature_vector( "OC-Feature-Vector" ),
    m_avp_redirect_information( "Redirect-Information", m_vnd_3gpp.getId() ),
    m_avp_list_of_measurements( "List-Of-Measurements", m_vnd_3gpp.getId() ),
    m_avp_qos_information( "QoS-Information", m_vnd_3gpp.getId() ),
    m_avp_final_unit_action( "Final-Unit-Action" ),
    m_avp_drmp( "DRMP" ),
    m_avp_logical_access_id( "Logical-Access-Id", m_vnd_Etsi.getId() ),
    m_avp_rule_deactivation_time( "Rule-Deactivation-Time", m_vnd_3gpp.getId() ),
    m_avp_flow_status( "Flow-Status", m_vnd_3gpp.getId() ),
    m_avp_priority_level( "Priority-Level", m_vnd_3gpp.getId() ),
    m_avp_pre_emption_vulnerability( "Pre-emption-Vulnerability", m_vnd_3gpp.getId() ),
    m_avp_presence_reporting_area_status( "Presence-Reporting-Area-Status", m_vnd_3gpp.getId() ),
    m_avp_reporting_reason( "Reporting-Reason", m_vnd_3gpp.getId() ),
    m_avp_qos_class_identifier( "QoS-Class-Identifier", m_vnd_3gpp.getId() ),
    m_avp_area_scope( "Area-Scope", m_vnd_3gpp.getId() ),
    m_avp_secondary_charging_collection_function_name( "Secondary-Charging-Collection-Function-Name", m_vnd_3gpp.getId() ),
    m_avp_e_utran_cell_global_identity( "E-UTRAN-Cell-Global-Identity", m_vnd_3gpp.getId() ),
    m_avp_called_station_id( "Called-Station-Id" ),
    m_avp_physical_access_id( "Physical-Access-Id", m_vnd_Etsi.getId() ),
    m_avp_an_gw_address( "AN-GW-Address", m_vnd_3gpp.getId() ),
    m_avp_traffic_steering_policy_identifier_dl( "Traffic-Steering-Policy-Identifier-DL", m_vnd_3gpp.getId() ),
    m_avp_destination_host( "Destination-Host" ),
    m_avp_location_area_identity( "Location-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_logging_interval( "Logging-Interval", m_vnd_3gpp.getId() ),
    m_avp_re_auth_request_type( "Re-Auth-Request-Type" ),
    m_avp_flow_information( "Flow-Information", m_vnd_3gpp.getId() ),
    m_avp_ue_local_ip_address( "UE-Local-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_guaranteed_bitrate_ul( "Guaranteed-Bitrate-UL", m_vnd_3gpp.getId() ),
    m_avp_event_charging_timestamp( "Event-Charging-TimeStamp", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ggsn_address( "3GPP-GGSN-Address", m_vnd_3gpp.getId() ),
    m_avp_event_report_indication( "Event-Report-Indication", m_vnd_3gpp.getId() ),
    m_avp_value_digits( "Value-Digits" ),
    m_avp_user_equipment_info_type( "User-Equipment-Info-Type" ),
    m_avp_cc_request_type( "CC-Request-Type" ),
    m_avp_usage_monitoring_information( "Usage-Monitoring-Information", m_vnd_3gpp.getId() ),
    m_avp_framed_ipv6_prefix( "Framed-IPv6-Prefix" ),
    m_avp_3gpp_charging_characteristics( "3GPP-Charging-Characteristics", m_vnd_3gpp.getId() ),
    m_avp_proxy_info( "Proxy-Info" ),
    m_avp_used_service_unit( "Used-Service-Unit" ),
    m_avp_origin_realm( "Origin-Realm" ),
    m_avp_twan_identifier( "TWAN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_origin_host( "Origin-Host" ),
    m_avp_flow_label( "Flow-Label", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ggsn_ipv6_address( "3GPP-GGSN-Ipv6-Address", m_vnd_3gpp.getId() ),
    m_avp_guaranteed_bitrate_dl( "Guaranteed-Bitrate-DL", m_vnd_3gpp.getId() ),
    m_avp_restriction_filter_rule( "Restriction-Filter-Rule" ),
    m_avp_3gpp_sgsn_address( "3GPP-SGSN-Address", m_vnd_3gpp.getId() ),
    m_avp_redirect_address_type( "Redirect-Address-Type" ),
    m_avp_report_interval( "Report-Interval", m_vnd_3gpp.getId() ),
    m_avp_subscription_id_data( "Subscription-Id-Data" ),
    m_avp_redirect_server_address( "Redirect-Server-Address" ),
    m_avp_adc_rule_base_name( "ADC-Rule-Base-Name", m_vnd_3gpp.getId() ),
    m_avp_final_unit_indication( "Final-Unit-Indication" ),
    m_avp_usage_monitoring_report( "Usage-Monitoring-Report", m_vnd_3gpp.getId() ),
    m_avp_session_release_cause( "Session-Release-Cause", m_vnd_3gpp.getId() ),
    m_avp_flow_description( "Flow-Description", m_vnd_3gpp.getId() ),
    m_avp_traffic_steering_policy_identifier_ul( "Traffic-Steering-Policy-Identifier-UL", m_vnd_3gpp.getId() ),
    m_avp_user_equipment_info_value( "User-Equipment-Info-Value" ),
    m_avp_route_record( "Route-Record" ),
    m_avp_presence_reporting_area_identifier( "Presence-Reporting-Area-Identifier", m_vnd_3gpp.getId() ),
    m_avp_csg_information_reporting( "CSG-Information-Reporting", m_vnd_3gpp.getId() ),
    m_avp_filter_id( "Filter-Id" ),
    m_avp_presence_reporting_area_information( "Presence-Reporting-Area-Information", m_vnd_3gpp.getId() ),
    m_avp_tdf_application_identifier( "TDF-Application-Identifier", m_vnd_3gpp.getId() ),
    m_avp_ssid( "SSID", m_vnd_3gpp.getId() ),
    m_avp_metering_method( "Metering-Method", m_vnd_3gpp.getId() ),
    m_avp_framed_ip_address( "Framed-IP-Address" ),
    m_avp_charging_information( "Charging-Information", m_vnd_3gpp.getId() ),
    m_avp_apn_aggregate_max_bitrate_dl( "APN-Aggregate-Max-Bitrate-DL", m_vnd_3gpp.getId() ),
    m_avp_conditional_apn_aggregate_max_bitrate( "Conditional-APN-Aggregate-Max-Bitrate", m_vnd_3gpp.getId() ),
    m_avp_positioning_method( "Positioning-Method", m_vnd_3gpp.getId() ),
    m_avp_oc_olr( "OC-OLR" ),
    m_avp_trace_data( "Trace-Data", m_vnd_3gpp.getId() ),
    m_avp_sourceid( "SourceID" ),
    m_avp_subscription_id_type( "Subscription-Id-Type" ),
    m_avp_usage_monitoring_level( "Usage-Monitoring-Level", m_vnd_3gpp.getId() ),
    m_avp_bearer_identifier( "Bearer-Identifier", m_vnd_3gpp.getId() ),
    m_avp_sponsor_identity( "Sponsor-Identity", m_vnd_3gpp.getId() ),
    m_avp_oc_reduction_percentage( "OC-Reduction-Percentage" ),
    m_avp_trace_collection_entity( "Trace-Collection-Entity", m_vnd_3gpp.getId() ),
    m_avp_mdt_configuration( "MDT-Configuration", m_vnd_3gpp.getId() ),
    m_avp_error_reporting_host( "Error-Reporting-Host" ),
    m_avp_adc_rule_remove( "ADC-Rule-Remove", m_vnd_3gpp.getId() ),
    m_avp_3gpp_sgsn_mcc_mnc( "3GPP-SGSN-MCC-MNC", m_vnd_3gpp.getId() ),
    m_avp_unit_value( "Unit-Value" ),
    m_avp_event_threshold_event_1i( "Event-Threshold-Event-1I", m_vnd_3gpp.getId() ),
    m_avp_rating_group( "Rating-Group" ),
    m_avp_rat_type( "RAT-Type", m_vnd_3gpp.getId() ),
    m_avp_online( "Online", m_vnd_3gpp.getId() ),
    m_avp_application_service_provider_identity( "Application-Service-Provider-Identity", m_vnd_3gpp.getId() ),
    m_avp_event_threshold_event_1f( "Event-Threshold-Event-1F", m_vnd_3gpp.getId() ),
    m_avp_reporting_level( "Reporting-Level", m_vnd_3gpp.getId() ),
    m_avp_allocation_retention_priority( "Allocation-Retention-Priority", m_vnd_3gpp.getId() ),
    m_avp_cell_global_identity( "Cell-Global-Identity", m_vnd_3gpp.getId() ),
    m_avp_oc_validity_duration( "OC-Validity-Duration" ),
    m_avp_csg_membership_indication( "CSG-Membership-Indication", m_vnd_3gpp.getId() ),
    m_avp_fixed_user_location_info( "Fixed-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_adc_rule_definition( "ADC-Rule-Definition", m_vnd_3gpp.getId() ),
    m_avp_trace_ne_type_list( "Trace-NE-Type-List", m_vnd_3gpp.getId() ),
    m_avp_user_equipment_info( "User-Equipment-Info" ),
    m_avp_redirect_host( "Redirect-Host" ),
    m_avp_measurement_period_lte( "Measurement-Period-LTE", m_vnd_3gpp.getId() ),
    m_avp_cc_service_specific_units( "CC-Service-Specific-Units" ),
    m_avp_max_requested_bandwidth_dl( "Max-Requested-Bandwidth-DL", m_vnd_3gpp.getId() ),
    m_avp_quota_consumption_time( "Quota-Consumption-Time", m_vnd_3gpp.getId() ),
    m_avp_origin_state_id( "Origin-State-Id" ),
    m_avp_cc_output_octets( "CC-Output-Octets" ),
    m_avp_trace_event_list( "Trace-Event-List", m_vnd_3gpp.getId() ),
    m_avp_adc_rule_report( "ADC-Rule-Report", m_vnd_3gpp.getId() ),
    m_avp_3gpp_user_location_info( "3GPP-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_result_code( "Result-Code" ),
    m_avp_tariff_change_usage( "Tariff-Change-Usage" ),
    m_avp_pdn_connection_charging_id( "PDN-Connection-Charging-ID", m_vnd_3gpp.getId() ),
    m_avp_routing_rule_identifier( "Routing-Rule-Identifier", m_vnd_3gpp.getId() ),
    m_avp_redirect_max_cache_time( "Redirect-Max-Cache-Time" ),
    m_avp_rule_activation_time( "Rule-Activation-Time", m_vnd_3gpp.getId() ),
    m_avp_load( "Load" ),
    m_avp_acct_application_id( "Acct-Application-Id" ),
    m_avp_redirect_server( "Redirect-Server" ),
    m_avp_an_trusted( "AN-Trusted", m_vnd_3gpp.getId() ),
    m_avp_precedence( "Precedence", m_vnd_3gpp.getId() ),
    m_avp_csg_id( "CSG-Id", m_vnd_3gpp.getId() ),
    m_avp_3gpp_selection_mode( "3GPP-Selection-Mode", m_vnd_3gpp.getId() ),
    m_avp_pcc_rule_status( "PCC-Rule-Status", m_vnd_3gpp.getId() ),
    m_avp_oc_supported_features( "OC-Supported-Features" ),
    m_avp_packet_filter_identifier( "Packet-Filter-Identifier", m_vnd_3gpp.getId() ),
    m_avp_tdf_application_instance_identifier( "TDF-Application-Instance-Identifier", m_vnd_3gpp.getId() ),
    m_avp_proxy_host( "Proxy-Host" ),
    m_avp_event_threshold_rsrp( "Event-Threshold-RSRP", m_vnd_3gpp.getId() ),
    m_avp_event_threshold_rsrq( "Event-Threshold-RSRQ", m_vnd_3gpp.getId() ),
    m_avp_monitoring_flags( "Monitoring-Flags", m_vnd_3gpp.getId() ),
    m_avp_experimental_result_code( "Experimental-Result-Code" ),
    m_avp_collection_period_rrm_lte( "Collection-Period-RRM-LTE", m_vnd_3gpp.getId() ),
    m_avp_auth_application_id( "Auth-Application-Id" ),
    m_avp_revalidation_time( "Revalidation-Time", m_vnd_3gpp.getId() ),
    m_avp_job_type( "Job-Type", m_vnd_3gpp.getId() ),
    m_avp_event_trigger( "Event-Trigger", m_vnd_3gpp.getId() ),
    m_avp_presence_reporting_area_elements_list( "Presence-Reporting-Area-Elements-List", m_vnd_3gpp.getId() ),
    m_avp_logging_duration( "Logging-Duration", m_vnd_3gpp.getId() ),
    m_avp_monitoring_key( "Monitoring-Key", m_vnd_3gpp.getId() ),
    m_avp_adc_rule_install( "ADC-Rule-Install", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ms_timezone( "3GPP-MS-TimeZone", m_vnd_3gpp.getId() ),
    m_avp_3gpp2_bsid( "3GPP2-BSID", m_vnd_3gpp2.getId() ),
    m_avp_dynamic_address_flag( "Dynamic-Address-Flag", m_vnd_3gpp.getId() ),
    m_avp_subscription_id( "Subscription-Id" ),
    m_avp_collection_period_rrm_umts( "Collection-Period-RRM-UMTS", m_vnd_3gpp.getId() ),
    m_avp_ip_can_type( "IP-CAN-Type", m_vnd_3gpp.getId() ),
    m_avp_offline( "Offline", m_vnd_3gpp.getId() ),
    m_avp_udp_source_port( "UDP-Source-Port", m_vnd_3gpp.getId() ),
    m_avp_routing_ip_address( "Routing-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_adc_rule_name( "ADC-Rule-Name", m_vnd_3gpp.getId() ),
    m_avp_mute_notification( "Mute-Notification", m_vnd_3gpp.getId() ),
    m_avp_tariff_time_change( "Tariff-Time-Change" ),
    m_avp_error_message( "Error-Message" ),
    m_avp_credit_management_status( "Credit-Management-Status", m_vnd_3gpp.getId() ),
    m_avp_mdt_allowed_plmn_id( "MDT-Allowed-PLMN-Id", m_vnd_3gpp.getId() ),
    m_avp_reporting_trigger( "Reporting-Trigger", m_vnd_3gpp.getId() ),
    m_avp_failed_avp( "Failed-AVP" ),
    m_avp_routing_area_identity( "Routing-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_cc_request_number( "CC-Request-Number" ),
    m_avp_cc_total_octets( "CC-Total-Octets" ),
    m_avp_trace_reference( "Trace-Reference", m_vnd_3gpp.getId() ),
    m_avp_flow_direction( "Flow-Direction", m_vnd_3gpp.getId() ),
    m_avp_cc_time( "CC-Time" ),
    m_avp_currency_code( "Currency-Code" ),
    m_avp_cc_input_octets( "CC-Input-Octets" ),
    m_avp_measurement_quantity( "Measurement-Quantity", m_vnd_3gpp.getId() ),
    m_avp_trace_depth( "Trace-Depth", m_vnd_3gpp.getId() ),
    m_avp_proxy_state( "Proxy-State" ),
    m_avp_rule_failure_code( "Rule-Failure-Code", m_vnd_3gpp.getId() ),
    m_avp_vendor_id( "Vendor-Id" ),
    m_avp_granted_service_unit( "Granted-Service-Unit" ),
    m_avp_max_requested_bandwidth_ul( "Max-Requested-Bandwidth-UL", m_vnd_3gpp.getId() ),
    m_avp_supported_features( "Supported-Features", m_vnd_3gpp.getId() ),
    m_avp_redirect_support( "Redirect-Support", m_vnd_3gpp.getId() ),
    m_avp_destination_realm( "Destination-Realm" ),
    m_avp_session_id( "Session-Id" ),
    m_avp_tos_traffic_class( "ToS-Traffic-Class", m_vnd_3gpp.getId() ),
    m_avp_bssid( "BSSID", m_vnd_3gpp.getId() ),
    m_avp_cc_money( "CC-Money" ),
    m_avp_application_detection_information( "Application-Detection-Information", m_vnd_3gpp.getId() ),
    m_avp_trace_interface_list( "Trace-Interface-List", m_vnd_3gpp.getId() ),
    m_avp_report_amount( "Report-Amount", m_vnd_3gpp.getId() ),
    m_avp_primary_event_charging_function_name( "Primary-Event-Charging-Function-Name", m_vnd_3gpp.getId() ),
    m_avp_security_parameter_index( "Security-Parameter-Index", m_vnd_3gpp.getId() ),
    m_avp_measurement_period_umts( "Measurement-Period-UMTS", m_vnd_3gpp.getId() ),
    m_avp_primary_charging_collection_function_name( "Primary-Charging-Collection-Function-Name", m_vnd_3gpp.getId() ),
    m_avp_user_csg_information( "User-CSG-Information", m_vnd_3gpp.getId() )
{
    std::cout << "Registering sd dictionary" << std::endl;
};

Dictionary::~Dictionary()
{
};

TSRcmd::TSRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdTSR() ),
      m_app( app )
{
}

TSRcmd::~TSRcmd()
{
}

Dictionary &TSRcmd::getDict()
{
   return m_app.getDict();
}

TSRreq::TSRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdTSR() ),
      m_app(app)
{
}

TSRreq::~TSRreq()
{
}

Dictionary &TSRreq::getDict()
{
   return m_app.getDict();
}

 
CRCRcmd::CRCRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdCRCR() ),
      m_app( app )
{
}

CRCRcmd::~CRCRcmd()
{
}

Dictionary &CRCRcmd::getDict()
{
   return m_app.getDict();
}

CRCRreq::CRCRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdCRCR() ),
      m_app(app)
{
}

CRCRreq::~CRCRreq()
{
}

Dictionary &CRCRreq::getDict()
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

 
ApplicationBase::ApplicationBase()
{
    setDictionaryEntry( &m_dict.app() );

};

ApplicationBase::~ApplicationBase()
{
};



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

RedirectInformationExtractor::RedirectInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRedirectInformation() ),
    redirect_support( *this, dict.avpRedirectSupport() ),
    redirect_address_type( *this, dict.avpRedirectAddressType() ),
    redirect_server_address( *this, dict.avpRedirectServerAddress() )
{
    add( redirect_support );
    add( redirect_address_type );
    add( redirect_server_address );
}

RedirectInformationExtractor::~RedirectInformationExtractor()
{
}

AllocationRetentionPriorityExtractor::AllocationRetentionPriorityExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAllocationRetentionPriority() ),
    priority_level( *this, dict.avpPriorityLevel() ),
    pre_emption_capability( *this, dict.avpPreEmptionCapability() ),
    pre_emption_vulnerability( *this, dict.avpPreEmptionVulnerability() )
{
    add( priority_level );
    add( pre_emption_capability );
    add( pre_emption_vulnerability );
}

AllocationRetentionPriorityExtractor::~AllocationRetentionPriorityExtractor()
{
}

ConditionalApnAggregateMaxBitrateExtractor::ConditionalApnAggregateMaxBitrateExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpConditionalApnAggregateMaxBitrate() ),
    apn_aggregate_max_bitrate_ul( *this, dict.avpApnAggregateMaxBitrateUl() ),
    apn_aggregate_max_bitrate_dl( *this, dict.avpApnAggregateMaxBitrateDl() ),
    ip_can_type( *this, dict.avpIpCanType() ),
    rat_type( *this, dict.avpRatType() )
{
    add( apn_aggregate_max_bitrate_ul );
    add( apn_aggregate_max_bitrate_dl );
    add( ip_can_type );
    add( rat_type );
}

ConditionalApnAggregateMaxBitrateExtractor::~ConditionalApnAggregateMaxBitrateExtractor()
{
}

ConditionalApnAggregateMaxBitrateExtractorList::ConditionalApnAggregateMaxBitrateExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpConditionalApnAggregateMaxBitrate() ),
    m_dict( dict )
{
}

ConditionalApnAggregateMaxBitrateExtractorList::~ConditionalApnAggregateMaxBitrateExtractorList()
{
}

QosInformationExtractor::QosInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpQosInformation() ),
    qos_class_identifier( *this, dict.avpQosClassIdentifier() ),
    max_requested_bandwidth_ul( *this, dict.avpMaxRequestedBandwidthUl() ),
    max_requested_bandwidth_dl( *this, dict.avpMaxRequestedBandwidthDl() ),
    guaranteed_bitrate_ul( *this, dict.avpGuaranteedBitrateUl() ),
    guaranteed_bitrate_dl( *this, dict.avpGuaranteedBitrateDl() ),
    bearer_identifier( *this, dict.avpBearerIdentifier() ),
    allocation_retention_priority( *this, dict ),
    apn_aggregate_max_bitrate_ul( *this, dict.avpApnAggregateMaxBitrateUl() ),
    apn_aggregate_max_bitrate_dl( *this, dict.avpApnAggregateMaxBitrateDl() ),
    conditional_apn_aggregate_max_bitrate( *this, dict )
{
    add( qos_class_identifier );
    add( max_requested_bandwidth_ul );
    add( max_requested_bandwidth_dl );
    add( guaranteed_bitrate_ul );
    add( guaranteed_bitrate_dl );
    add( bearer_identifier );
    add( allocation_retention_priority );
    add( apn_aggregate_max_bitrate_ul );
    add( apn_aggregate_max_bitrate_dl );
    add( conditional_apn_aggregate_max_bitrate );
}

QosInformationExtractor::~QosInformationExtractor()
{
}

AreaScopeExtractor::AreaScopeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAreaScope() ),
    cell_global_identity( *this, dict.avpCellGlobalIdentity() ),
    e_utran_cell_global_identity( *this, dict.avpEUtranCellGlobalIdentity() ),
    routing_area_identity( *this, dict.avpRoutingAreaIdentity() ),
    location_area_identity( *this, dict.avpLocationAreaIdentity() ),
    tracking_area_identity( *this, dict.avpTrackingAreaIdentity() )
{
    add( cell_global_identity );
    add( e_utran_cell_global_identity );
    add( routing_area_identity );
    add( location_area_identity );
    add( tracking_area_identity );
}

AreaScopeExtractor::~AreaScopeExtractor()
{
}

FlowInformationExtractor::FlowInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFlowInformation() ),
    flow_description( *this, dict.avpFlowDescription() ),
    packet_filter_identifier( *this, dict.avpPacketFilterIdentifier() ),
    packet_filter_usage( *this, dict.avpPacketFilterUsage() ),
    tos_traffic_class( *this, dict.avpTosTrafficClass() ),
    security_parameter_index( *this, dict.avpSecurityParameterIndex() ),
    flow_label( *this, dict.avpFlowLabel() ),
    flow_direction( *this, dict.avpFlowDirection() ),
    routing_rule_identifier( *this, dict.avpRoutingRuleIdentifier() )
{
    add( flow_description );
    add( packet_filter_identifier );
    add( packet_filter_usage );
    add( tos_traffic_class );
    add( security_parameter_index );
    add( flow_label );
    add( flow_direction );
    add( routing_rule_identifier );
}

FlowInformationExtractor::~FlowInformationExtractor()
{
}

UserCsgInformationExtractor::UserCsgInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUserCsgInformation() ),
    csg_id( *this, dict.avpCsgId() ),
    csg_access_mode( *this, dict.avpCsgAccessMode() ),
    csg_membership_indication( *this, dict.avpCsgMembershipIndication() )
{
    add( csg_id );
    add( csg_access_mode );
    add( csg_membership_indication );
}

UserCsgInformationExtractor::~UserCsgInformationExtractor()
{
}

MdtConfigurationExtractor::MdtConfigurationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMdtConfiguration() ),
    job_type( *this, dict.avpJobType() ),
    area_scope( *this, dict ),
    list_of_measurements( *this, dict.avpListOfMeasurements() ),
    reporting_trigger( *this, dict.avpReportingTrigger() ),
    report_interval( *this, dict.avpReportInterval() ),
    report_amount( *this, dict.avpReportAmount() ),
    event_threshold_rsrp( *this, dict.avpEventThresholdRsrp() ),
    event_threshold_rsrq( *this, dict.avpEventThresholdRsrq() ),
    logging_interval( *this, dict.avpLoggingInterval() ),
    logging_duration( *this, dict.avpLoggingDuration() ),
    measurement_period_lte( *this, dict.avpMeasurementPeriodLte() ),
    measurement_period_umts( *this, dict.avpMeasurementPeriodUmts() ),
    collection_period_rrm_lte( *this, dict.avpCollectionPeriodRrmLte() ),
    collection_period_rrm_umts( *this, dict.avpCollectionPeriodRrmUmts() ),
    positioning_method( *this, dict.avpPositioningMethod() ),
    measurement_quantity( *this, dict.avpMeasurementQuantity() ),
    event_threshold_event_1f( *this, dict.avpEventThresholdEvent1f() ),
    event_threshold_event_1i( *this, dict.avpEventThresholdEvent1i() ),
    mdt_allowed_plmn_id( *this, dict.avpMdtAllowedPlmnId() )
{
    add( job_type );
    add( area_scope );
    add( list_of_measurements );
    add( reporting_trigger );
    add( report_interval );
    add( report_amount );
    add( event_threshold_rsrp );
    add( event_threshold_rsrq );
    add( logging_interval );
    add( logging_duration );
    add( measurement_period_lte );
    add( measurement_period_umts );
    add( collection_period_rrm_lte );
    add( collection_period_rrm_umts );
    add( positioning_method );
    add( measurement_quantity );
    add( event_threshold_event_1f );
    add( event_threshold_event_1i );
    add( mdt_allowed_plmn_id );
}

MdtConfigurationExtractor::~MdtConfigurationExtractor()
{
}

TraceDataExtractor::TraceDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTraceData() ),
    trace_reference( *this, dict.avpTraceReference() ),
    trace_depth( *this, dict.avpTraceDepth() ),
    trace_ne_type_list( *this, dict.avpTraceNeTypeList() ),
    trace_interface_list( *this, dict.avpTraceInterfaceList() ),
    trace_event_list( *this, dict.avpTraceEventList() ),
    omc_id( *this, dict.avpOmcId() ),
    trace_collection_entity( *this, dict.avpTraceCollectionEntity() ),
    mdt_configuration( *this, dict )
{
    add( trace_reference );
    add( trace_depth );
    add( trace_ne_type_list );
    add( trace_interface_list );
    add( trace_event_list );
    add( omc_id );
    add( trace_collection_entity );
    add( mdt_configuration );
}

TraceDataExtractor::~TraceDataExtractor()
{
}

PresenceReportingAreaInformationExtractor::PresenceReportingAreaInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpPresenceReportingAreaInformation() ),
    presence_reporting_area_identifier( *this, dict.avpPresenceReportingAreaIdentifier() ),
    presence_reporting_area_status( *this, dict.avpPresenceReportingAreaStatus() ),
    presence_reporting_area_elements_list( *this, dict.avpPresenceReportingAreaElementsList() )
{
    add( presence_reporting_area_identifier );
    add( presence_reporting_area_status );
    add( presence_reporting_area_elements_list );
}

PresenceReportingAreaInformationExtractor::~PresenceReportingAreaInformationExtractor()
{
}

EventReportIndicationExtractor::EventReportIndicationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEventReportIndication() ),
    an_trusted( *this, dict.avpAnTrusted() ),
    event_trigger( *this, dict.avpEventTrigger() ),
    user_csg_information( *this, dict ),
    ip_can_type( *this, dict.avpIpCanType() ),
    an_gw_address( *this, dict.avpAnGwAddress() ),
    tgpp_sgsn_address( *this, dict.avp3gppSgsnAddress() ),
    tgpp_sgsn_ipv6_address( *this, dict.avp3gppSgsnIpv6Address() ),
    tgpp_sgsn_mcc_mnc( *this, dict.avp3gppSgsnMccMnc() ),
    framed_ip_address( *this, dict.avpFramedIpAddress() ),
    rat_type( *this, dict.avpRatType() ),
    rai( *this, dict.avpRai() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    trace_data( *this, dict ),
    trace_reference( *this, dict.avpTraceReference() ),
    tgpp2_bsid( *this, dict.avp3gpp2Bsid() ),
    tgpp_ms_timezone( *this, dict.avp3gppMsTimezone() ),
    routing_ip_address( *this, dict.avpRoutingIpAddress() ),
    ue_local_ip_address( *this, dict.avpUeLocalIpAddress() ),
    henb_local_ip_address( *this, dict.avpHenbLocalIpAddress() ),
    udp_source_port( *this, dict.avpUdpSourcePort() ),
    presence_reporting_area_information( *this, dict )
{
    add( an_trusted );
    add( event_trigger );
    add( user_csg_information );
    add( ip_can_type );
    add( an_gw_address );
    add( tgpp_sgsn_address );
    add( tgpp_sgsn_ipv6_address );
    add( tgpp_sgsn_mcc_mnc );
    add( framed_ip_address );
    add( rat_type );
    add( rai );
    add( tgpp_user_location_info );
    add( trace_data );
    add( trace_reference );
    add( tgpp2_bsid );
    add( tgpp_ms_timezone );
    add( routing_ip_address );
    add( ue_local_ip_address );
    add( henb_local_ip_address );
    add( udp_source_port );
    add( presence_reporting_area_information );
}

EventReportIndicationExtractor::~EventReportIndicationExtractor()
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

GrantedServiceUnitExtractorList::GrantedServiceUnitExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpGrantedServiceUnit() ),
    m_dict( dict )
{
}

GrantedServiceUnitExtractorList::~GrantedServiceUnitExtractorList()
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

UsedServiceUnitExtractorList::UsedServiceUnitExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpUsedServiceUnit() ),
    m_dict( dict )
{
}

UsedServiceUnitExtractorList::~UsedServiceUnitExtractorList()
{
}

UsageMonitoringInformationExtractor::UsageMonitoringInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUsageMonitoringInformation() ),
    monitoring_key( *this, dict.avpMonitoringKey() ),
    granted_service_unit( *this, dict ),
    used_service_unit( *this, dict ),
    quota_consumption_time( *this, dict.avpQuotaConsumptionTime() ),
    usage_monitoring_level( *this, dict.avpUsageMonitoringLevel() ),
    usage_monitoring_report( *this, dict.avpUsageMonitoringReport() ),
    usage_monitoring_support( *this, dict.avpUsageMonitoringSupport() )
{
    add( monitoring_key );
    add( granted_service_unit );
    add( used_service_unit );
    add( quota_consumption_time );
    add( usage_monitoring_level );
    add( usage_monitoring_report );
    add( usage_monitoring_support );
}

UsageMonitoringInformationExtractor::~UsageMonitoringInformationExtractor()
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

RedirectServerExtractor::RedirectServerExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRedirectServer() ),
    redirect_address_type( *this, dict.avpRedirectAddressType() ),
    redirect_server_address( *this, dict.avpRedirectServerAddress() )
{
    add( redirect_address_type );
    add( redirect_server_address );
}

RedirectServerExtractor::~RedirectServerExtractor()
{
}

FinalUnitIndicationExtractor::FinalUnitIndicationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFinalUnitIndication() ),
    final_unit_action( *this, dict.avpFinalUnitAction() ),
    restriction_filter_rule( *this, dict.avpRestrictionFilterRule() ),
    filter_id( *this, dict.avpFilterId() ),
    redirect_server( *this, dict )
{
    add( final_unit_action );
    add( restriction_filter_rule );
    add( filter_id );
    add( redirect_server );
}

FinalUnitIndicationExtractor::~FinalUnitIndicationExtractor()
{
}

ChargingInformationExtractor::ChargingInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpChargingInformation() ),
    primary_event_charging_function_name( *this, dict.avpPrimaryEventChargingFunctionName() ),
    secondary_event_charging_function_name( *this, dict.avpSecondaryEventChargingFunctionName() ),
    primary_charging_collection_function_name( *this, dict.avpPrimaryChargingCollectionFunctionName() ),
    secondary_charging_collection_function_name( *this, dict.avpSecondaryChargingCollectionFunctionName() )
{
    add( primary_event_charging_function_name );
    add( secondary_event_charging_function_name );
    add( primary_charging_collection_function_name );
    add( secondary_charging_collection_function_name );
}

ChargingInformationExtractor::~ChargingInformationExtractor()
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

AdcRuleRemoveExtractor::AdcRuleRemoveExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAdcRuleRemove() ),
    adc_rule_name( *this, dict.avpAdcRuleName() ),
    adc_rule_base_name( *this, dict.avpAdcRuleBaseName() )
{
    add( adc_rule_name );
    add( adc_rule_base_name );
}

AdcRuleRemoveExtractor::~AdcRuleRemoveExtractor()
{
}

FixedUserLocationInfoExtractor::FixedUserLocationInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFixedUserLocationInfo() ),
    ssid( *this, dict.avpSsid() ),
    bssid( *this, dict.avpBssid() ),
    logical_access_id( *this, dict.avpLogicalAccessId() ),
    physical_access_id( *this, dict.avpPhysicalAccessId() )
{
    add( ssid );
    add( bssid );
    add( logical_access_id );
    add( physical_access_id );
}

FixedUserLocationInfoExtractor::~FixedUserLocationInfoExtractor()
{
}

FlowInformationExtractorList::FlowInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpFlowInformation() ),
    m_dict( dict )
{
}

FlowInformationExtractorList::~FlowInformationExtractorList()
{
}

AdcRuleDefinitionExtractor::AdcRuleDefinitionExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAdcRuleDefinition() ),
    adc_rule_name( *this, dict.avpAdcRuleName() ),
    tdf_application_identifier( *this, dict.avpTdfApplicationIdentifier() ),
    flow_information( *this, dict ),
    service_identifier( *this, dict.avpServiceIdentifier() ),
    rating_group( *this, dict.avpRatingGroup() ),
    reporting_level( *this, dict.avpReportingLevel() ),
    online( *this, dict.avpOnline() ),
    offline( *this, dict.avpOffline() ),
    metering_method( *this, dict.avpMeteringMethod() ),
    precedence( *this, dict.avpPrecedence() ),
    flow_status( *this, dict.avpFlowStatus() ),
    qos_information( *this, dict ),
    monitoring_key( *this, dict.avpMonitoringKey() ),
    sponsor_identity( *this, dict.avpSponsorIdentity() ),
    application_service_provider_identity( *this, dict.avpApplicationServiceProviderIdentity() ),
    redirect_information( *this, dict ),
    mute_notification( *this, dict.avpMuteNotification() ),
    traffic_steering_policy_identifier_dl( *this, dict.avpTrafficSteeringPolicyIdentifierDl() ),
    traffic_steering_policy_identifier_ul( *this, dict.avpTrafficSteeringPolicyIdentifierUl() ),
    tos_traffic_class( *this, dict.avpTosTrafficClass() )
{
    add( adc_rule_name );
    add( tdf_application_identifier );
    add( flow_information );
    add( service_identifier );
    add( rating_group );
    add( reporting_level );
    add( online );
    add( offline );
    add( metering_method );
    add( precedence );
    add( flow_status );
    add( qos_information );
    add( monitoring_key );
    add( sponsor_identity );
    add( application_service_provider_identity );
    add( redirect_information );
    add( mute_notification );
    add( traffic_steering_policy_identifier_dl );
    add( traffic_steering_policy_identifier_ul );
    add( tos_traffic_class );
}

AdcRuleDefinitionExtractor::~AdcRuleDefinitionExtractor()
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

AdcRuleReportExtractor::AdcRuleReportExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAdcRuleReport() ),
    adc_rule_name( *this, dict.avpAdcRuleName() ),
    adc_rule_base_name( *this, dict.avpAdcRuleBaseName() ),
    pcc_rule_status( *this, dict.avpPccRuleStatus() ),
    rule_failure_code( *this, dict.avpRuleFailureCode() ),
    final_unit_indication( *this, dict )
{
    add( adc_rule_name );
    add( adc_rule_base_name );
    add( pcc_rule_status );
    add( rule_failure_code );
    add( final_unit_indication );
}

AdcRuleReportExtractor::~AdcRuleReportExtractor()
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

OcSupportedFeaturesExtractor::OcSupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpOcSupportedFeatures() ),
    oc_feature_vector( *this, dict.avpOcFeatureVector() )
{
    add( oc_feature_vector );
}

OcSupportedFeaturesExtractor::~OcSupportedFeaturesExtractor()
{
}

AdcRuleDefinitionExtractorList::AdcRuleDefinitionExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAdcRuleDefinition() ),
    m_dict( dict )
{
}

AdcRuleDefinitionExtractorList::~AdcRuleDefinitionExtractorList()
{
}

AdcRuleInstallExtractor::AdcRuleInstallExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAdcRuleInstall() ),
    adc_rule_definition( *this, dict ),
    adc_rule_name( *this, dict.avpAdcRuleName() ),
    adc_rule_base_name( *this, dict.avpAdcRuleBaseName() ),
    monitoring_flags( *this, dict.avpMonitoringFlags() ),
    rule_activation_time( *this, dict.avpRuleActivationTime() ),
    rule_deactivation_time( *this, dict.avpRuleDeactivationTime() )
{
    add( adc_rule_definition );
    add( adc_rule_name );
    add( adc_rule_base_name );
    add( monitoring_flags );
    add( rule_activation_time );
    add( rule_deactivation_time );
}

AdcRuleInstallExtractor::~AdcRuleInstallExtractor()
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

ApplicationDetectionInformationExtractor::ApplicationDetectionInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpApplicationDetectionInformation() ),
    tdf_application_identifier( *this, dict.avpTdfApplicationIdentifier() ),
    tdf_application_instance_identifier( *this, dict.avpTdfApplicationInstanceIdentifier() ),
    flow_information( *this, dict )
{
    add( tdf_application_identifier );
    add( tdf_application_instance_identifier );
    add( flow_information );
}

ApplicationDetectionInformationExtractor::~ApplicationDetectionInformationExtractor()
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

AdcRuleInstallExtractorList::AdcRuleInstallExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAdcRuleInstall() ),
    m_dict( dict )
{
}

AdcRuleInstallExtractorList::~AdcRuleInstallExtractorList()
{
}

UsageMonitoringInformationExtractorList::UsageMonitoringInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpUsageMonitoringInformation() ),
    m_dict( dict )
{
}

UsageMonitoringInformationExtractorList::~UsageMonitoringInformationExtractorList()
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

TsRequestExtractor::TsRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    oc_supported_features( *this, dict ),
    subscription_id( *this, dict ),
    supported_features( *this, dict ),
    framed_ip_address( *this, dict.avpFramedIpAddress() ),
    framed_ipv6_prefix( *this, dict.avpFramedIpv6Prefix() ),
    ip_can_type( *this, dict.avpIpCanType() ),
    rat_type( *this, dict.avpRatType() ),
    an_trusted( *this, dict.avpAnTrusted() ),
    user_equipment_info( *this, dict ),
    qos_information( *this, dict ),
    an_gw_address( *this, dict.avpAnGwAddress() ),
    tgpp_sgsn_address( *this, dict.avp3gppSgsnAddress() ),
    tgpp_sgsn_ipv6_address( *this, dict.avp3gppSgsnIpv6Address() ),
    tgpp_ggsn_address( *this, dict.avp3gppGgsnAddress() ),
    tgpp_ggsn_ipv6_address( *this, dict.avp3gppGgsnIpv6Address() ),
    tgpp_selection_mode( *this, dict.avp3gppSelectionMode() ),
    dynamic_address_flag( *this, dict.avpDynamicAddressFlag() ),
    dynamic_address_flag_extension( *this, dict.avpDynamicAddressFlagExtension() ),
    pdn_connection_charging_id( *this, dict.avpPdnConnectionChargingId() ),
    tgpp_sgsn_mcc_mnc( *this, dict.avp3gppSgsnMccMnc() ),
    rai( *this, dict.avpRai() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    fixed_user_location_info( *this, dict ),
    user_csg_information( *this, dict ),
    twan_identifier( *this, dict.avpTwanIdentifier() ),
    tgpp_ms_timezone( *this, dict.avp3gppMsTimezone() ),
    tgpp_charging_characteristics( *this, dict.avp3gppChargingCharacteristics() ),
    called_station_id( *this, dict.avpCalledStationId() ),
    charging_information( *this, dict ),
    online( *this, dict.avpOnline() ),
    offline( *this, dict.avpOffline() ),
    adc_rule_install( *this, dict ),
    revalidation_time( *this, dict.avpRevalidationTime() ),
    usage_monitoring_information( *this, dict ),
    csg_information_reporting( *this, dict.avpCsgInformationReporting() ),
    event_trigger( *this, dict.avpEventTrigger() ),
    presence_reporting_area_information( *this, dict ),
    logical_access_id( *this, dict.avpLogicalAccessId() ),
    physical_access_id( *this, dict.avpPhysicalAccessId() ),
    tgpp2_bsid( *this, dict.avp3gpp2Bsid() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( origin_host );
    add( origin_realm );
    add( destination_realm );
    add( destination_host );
    add( origin_state_id );
    add( oc_supported_features );
    add( subscription_id );
    add( supported_features );
    add( framed_ip_address );
    add( framed_ipv6_prefix );
    add( ip_can_type );
    add( rat_type );
    add( an_trusted );
    add( user_equipment_info );
    add( qos_information );
    add( an_gw_address );
    add( tgpp_sgsn_address );
    add( tgpp_sgsn_ipv6_address );
    add( tgpp_ggsn_address );
    add( tgpp_ggsn_ipv6_address );
    add( tgpp_selection_mode );
    add( dynamic_address_flag );
    add( dynamic_address_flag_extension );
    add( pdn_connection_charging_id );
    add( tgpp_sgsn_mcc_mnc );
    add( rai );
    add( tgpp_user_location_info );
    add( fixed_user_location_info );
    add( user_csg_information );
    add( twan_identifier );
    add( tgpp_ms_timezone );
    add( tgpp_charging_characteristics );
    add( called_station_id );
    add( charging_information );
    add( online );
    add( offline );
    add( adc_rule_install );
    add( revalidation_time );
    add( usage_monitoring_information );
    add( csg_information_reporting );
    add( event_trigger );
    add( presence_reporting_area_information );
    add( logical_access_id );
    add( physical_access_id );
    add( tgpp2_bsid );
    add( proxy_info );
    add( route_record );
}

TsRequestExtractor::~TsRequestExtractor()
{
}

AdcRuleReportExtractorList::AdcRuleReportExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAdcRuleReport() ),
    m_dict( dict )
{
}

AdcRuleReportExtractorList::~AdcRuleReportExtractorList()
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

TsAnswerExtractor::TsAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    supported_features( *this, dict ),
    adc_rule_report( *this, dict ),
    event_report_indication( *this, dict ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() ),
    load( *this, dict )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( origin_host );
    add( origin_realm );
    add( result_code );
    add( experimental_result );
    add( origin_state_id );
    add( oc_supported_features );
    add( oc_olr );
    add( supported_features );
    add( adc_rule_report );
    add( event_report_indication );
    add( error_message );
    add( error_reporting_host );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
    add( load );
}

TsAnswerExtractor::~TsAnswerExtractor()
{
}

ApplicationDetectionInformationExtractorList::ApplicationDetectionInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpApplicationDetectionInformation() ),
    m_dict( dict )
{
}

ApplicationDetectionInformationExtractorList::~ApplicationDetectionInformationExtractorList()
{
}

CreditControlRequestExtractor::CreditControlRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    oc_supported_features( *this, dict ),
    cc_request_type( *this, dict.avpCcRequestType() ),
    cc_request_number( *this, dict.avpCcRequestNumber() ),
    credit_management_status( *this, dict.avpCreditManagementStatus() ),
    destination_host( *this, dict.avpDestinationHost() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    framed_ip_address( *this, dict.avpFramedIpAddress() ),
    framed_ipv6_prefix( *this, dict.avpFramedIpv6Prefix() ),
    adc_rule_report( *this, dict ),
    application_detection_information( *this, dict ),
    event_trigger( *this, dict.avpEventTrigger() ),
    event_report_indication( *this, dict ),
    usage_monitoring_information( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() ),
    supported_features( *this, dict )
{
    add( session_id );
    add( drmp );
    add( auth_application_id );
    add( origin_host );
    add( origin_realm );
    add( destination_realm );
    add( oc_supported_features );
    add( cc_request_type );
    add( cc_request_number );
    add( credit_management_status );
    add( destination_host );
    add( origin_state_id );
    add( framed_ip_address );
    add( framed_ipv6_prefix );
    add( adc_rule_report );
    add( application_detection_information );
    add( event_trigger );
    add( event_report_indication );
    add( usage_monitoring_information );
    add( proxy_info );
    add( route_record );
    add( supported_features );
}

CreditControlRequestExtractor::~CreditControlRequestExtractor()
{
}

AdcRuleRemoveExtractorList::AdcRuleRemoveExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAdcRuleRemove() ),
    m_dict( dict )
{
}

AdcRuleRemoveExtractorList::~AdcRuleRemoveExtractorList()
{
}

CreditControlAnswerExtractor::CreditControlAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    cc_request_type( *this, dict.avpCcRequestType() ),
    cc_request_number( *this, dict.avpCcRequestNumber() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    framed_ipv6_prefix( *this, dict.avpFramedIpv6Prefix() ),
    qos_information( *this, dict ),
    event_trigger( *this, dict.avpEventTrigger() ),
    event_report_indication( *this, dict ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    redirect_host( *this, dict.avpRedirectHost() ),
    redirect_host_usage( *this, dict.avpRedirectHostUsage() ),
    redirect_max_cache_time( *this, dict.avpRedirectMaxCacheTime() ),
    adc_rule_remove( *this, dict ),
    adc_rule_install( *this, dict ),
    revalidation_time( *this, dict.avpRevalidationTime() ),
    usage_monitoring_information( *this, dict ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() ),
    supported_features( *this, dict ),
    load( *this, dict )
{
    add( session_id );
    add( drmp );
    add( auth_application_id );
    add( origin_host );
    add( origin_realm );
    add( result_code );
    add( experimental_result );
    add( cc_request_type );
    add( cc_request_number );
    add( oc_supported_features );
    add( oc_olr );
    add( framed_ipv6_prefix );
    add( qos_information );
    add( event_trigger );
    add( event_report_indication );
    add( origin_state_id );
    add( redirect_host );
    add( redirect_host_usage );
    add( redirect_max_cache_time );
    add( adc_rule_remove );
    add( adc_rule_install );
    add( revalidation_time );
    add( usage_monitoring_information );
    add( error_message );
    add( error_reporting_host );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
    add( supported_features );
    add( load );
}

CreditControlAnswerExtractor::~CreditControlAnswerExtractor()
{
}

ReAuthRequestExtractor::ReAuthRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    re_auth_request_type( *this, dict.avpReAuthRequestType() ),
    session_release_cause( *this, dict.avpSessionReleaseCause() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    oc_supported_features( *this, dict ),
    qos_information( *this, dict ),
    event_trigger( *this, dict.avpEventTrigger() ),
    event_report_indication( *this, dict ),
    adc_rule_remove( *this, dict ),
    adc_rule_install( *this, dict ),
    revalidation_time( *this, dict.avpRevalidationTime() ),
    usage_monitoring_information( *this, dict ),
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
    add( re_auth_request_type );
    add( session_release_cause );
    add( origin_state_id );
    add( oc_supported_features );
    add( qos_information );
    add( event_trigger );
    add( event_report_indication );
    add( adc_rule_remove );
    add( adc_rule_install );
    add( revalidation_time );
    add( usage_monitoring_information );
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
    origin_state_id( *this, dict.avpOriginStateId() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    adc_rule_report( *this, dict ),
    event_report_indication( *this, dict ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    load( *this, dict )
{
    add( session_id );
    add( drmp );
    add( origin_host );
    add( origin_realm );
    add( result_code );
    add( experimental_result );
    add( origin_state_id );
    add( oc_supported_features );
    add( oc_olr );
    add( adc_rule_report );
    add( event_report_indication );
    add( error_message );
    add( error_reporting_host );
    add( failed_avp );
    add( proxy_info );
    add( load );
}

ReAuthAnswerExtractor::~ReAuthAnswerExtractor()
{
}


}
