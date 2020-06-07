/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <string>
#include <iostream>
#include <sstream>

#include "gx.h"
#include "gx_impl.h"

namespace gx {

Dictionary::Dictionary()
    : m_app( "Gx"),
    m_cmd_crcr( "Credit-Control-Request" ),
    m_cmd_crca( "Credit-Control-Answer" ),
    m_cmd_rear( "Re-Auth-Request" ),
    m_cmd_reaa( "Re-Auth-Answer" ),
    m_vnd_Etsi( "ETSI" ),
    m_vnd_3gpp2( "3GPP2" ),
    m_vnd_3gpp( "3GPP" ),
    m_avp_tdf_destination_host( "TDF-Destination-Host", m_vnd_3gpp.getId() ),
    m_avp_pre_emption_capability( "Pre-emption-Capability", m_vnd_3gpp.getId() ),
    m_avp_packet_filter_content( "Packet-Filter-Content", m_vnd_3gpp.getId() ),
    m_avp_feature_list_id( "Feature-List-ID", m_vnd_3gpp.getId() ),
    m_avp_resource_release_notification( "Resource-Release-Notification", m_vnd_3gpp.getId() ),
    m_avp_ran_rule_support( "RAN-Rule-Support", m_vnd_3gpp.getId() ),
    m_avp_service_identifier( "Service-Identifier" ),
    m_avp_physical_access_id( "Physical-Access-Id", m_vnd_Etsi.getId() ),
    m_avp_csg_access_mode( "CSG-Access-Mode", m_vnd_3gpp.getId() ),
    m_avp_henb_local_ip_address( "HeNB-Local-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_flow_number( "Flow-Number", m_vnd_3gpp.getId() ),
    m_avp_bearer_identifier( "Bearer-Identifier", m_vnd_3gpp.getId() ),
    m_avp_apn_aggregate_max_bitrate_ul( "APN-Aggregate-Max-Bitrate-UL", m_vnd_3gpp.getId() ),
    m_avp_network_request_support( "Network-Request-Support", m_vnd_3gpp.getId() ),
    m_avp_termination_cause( "Termination-Cause" ),
    m_avp_traffic_steering_policy_identifier_ul( "Traffic-Steering-Policy-Identifier-UL", m_vnd_3gpp.getId() ),
    m_avp_sponsor_identity( "Sponsor-Identity", m_vnd_3gpp.getId() ),
    m_avp_3gpp_rat_type( "3GPP-RAT-Type", m_vnd_3gpp.getId() ),
    m_avp_af_signalling_protocol( "AF-Signalling-Protocol", m_vnd_3gpp.getId() ),
    m_avp_packet_filter_usage( "Packet-Filter-Usage", m_vnd_3gpp.getId() ),
    m_avp_oc_sequence_number( "OC-Sequence-Number" ),
    m_avp_usage_monitoring_support( "Usage-Monitoring-Support", m_vnd_3gpp.getId() ),
    m_avp_tracking_area_identity( "Tracking-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_load_value( "Load-Value" ),
    m_avp_feature_list( "Feature-List", m_vnd_3gpp.getId() ),
    m_avp_omc_id( "OMC-Id", m_vnd_3gpp.getId() ),
    m_avp_rai( "RAI", m_vnd_3gpp.getId() ),
    m_avp_oc_report_type( "OC-Report-Type" ),
    m_avp_tft_filter( "TFT-Filter", m_vnd_3gpp.getId() ),
    m_avp_secondary_event_charging_function_name( "Secondary-Event-Charging-Function-Name", m_vnd_3gpp.getId() ),
    m_avp_pcscf_restoration_indication( "PCSCF-Restoration-Indication", m_vnd_3gpp.getId() ),
    m_avp_load( "Load" ),
    m_avp_oc_feature_vector( "OC-Feature-Vector" ),
    m_avp_cc_service_specific_units( "CC-Service-Specific-Units" ),
    m_avp_pra_remove( "PRA-Remove", m_vnd_3gpp.getId() ),
    m_avp_cc_time( "CC-Time" ),
    m_avp_list_of_measurements( "List-Of-Measurements", m_vnd_3gpp.getId() ),
    m_avp_qos_information( "QoS-Information", m_vnd_3gpp.getId() ),
    m_avp_final_unit_action( "Final-Unit-Action" ),
    m_avp_conditional_policy_information( "Conditional-Policy-Information", m_vnd_3gpp.getId() ),
    m_avp_drmp( "DRMP" ),
    m_avp_pra_install( "PRA-Install", m_vnd_3gpp.getId() ),
    m_avp_logical_access_id( "Logical-Access-Id", m_vnd_Etsi.getId() ),
    m_avp_dynamic_address_flag_extension( "Dynamic-Address-Flag-Extension", m_vnd_3gpp.getId() ),
    m_avp_resource_allocation_notification( "Resource-Allocation-Notification", m_vnd_3gpp.getId() ),
    m_avp_rule_deactivation_time( "Rule-Deactivation-Time", m_vnd_3gpp.getId() ),
    m_avp_flow_status( "Flow-Status", m_vnd_3gpp.getId() ),
    m_avp_priority_level( "Priority-Level", m_vnd_3gpp.getId() ),
    m_avp_pre_emption_vulnerability( "Pre-emption-Vulnerability", m_vnd_3gpp.getId() ),
    m_avp_bearer_usage( "Bearer-Usage", m_vnd_3gpp.getId() ),
    m_avp_reporting_reason( "Reporting-Reason", m_vnd_3gpp.getId() ),
    m_avp_qos_class_identifier( "QoS-Class-Identifier", m_vnd_3gpp.getId() ),
    m_avp_charging_correlation_indicator( "Charging-Correlation-Indicator", m_vnd_3gpp.getId() ),
    m_avp_area_scope( "Area-Scope", m_vnd_3gpp.getId() ),
    m_avp_trace_ne_type_list( "Trace-NE-Type-List", m_vnd_3gpp.getId() ),
    m_avp_e_utran_cell_global_identity( "E-UTRAN-Cell-Global-Identity", m_vnd_3gpp.getId() ),
    m_avp_nbifom_support( "NBIFOM-Support", m_vnd_3gpp.getId() ),
    m_avp_usage_monitoring_level( "Usage-Monitoring-Level", m_vnd_3gpp.getId() ),
    m_avp_redirect_host_usage( "Redirect-Host-Usage" ),
    m_avp_an_gw_address( "AN-GW-Address", m_vnd_3gpp.getId() ),
    m_avp_charging_rule_definition( "Charging-Rule-Definition", m_vnd_3gpp.getId() ),
    m_avp_traffic_steering_policy_identifier_dl( "Traffic-Steering-Policy-Identifier-DL", m_vnd_3gpp.getId() ),
    m_avp_secondary_charging_collection_function_name( "Secondary-Charging-Collection-Function-Name", m_vnd_3gpp.getId() ),
    m_avp_routing_filter( "Routing-Filter", m_vnd_3gpp.getId() ),
    m_avp_tdf_information( "TDF-Information", m_vnd_3gpp.getId() ),
    m_avp_destination_host( "Destination-Host" ),
    m_avp_location_area_identity( "Location-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_logging_interval( "Logging-Interval", m_vnd_3gpp.getId() ),
    m_avp_re_auth_request_type( "Re-Auth-Request-Type" ),
    m_avp_flow_information( "Flow-Information", m_vnd_3gpp.getId() ),
    m_avp_ue_local_ip_address( "UE-Local-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_tunnel_information( "Tunnel-Information", m_vnd_3gpp.getId() ),
    m_avp_sourceid( "SourceID" ),
    m_avp_required_access_info( "Required-Access-Info", m_vnd_3gpp.getId() ),
    m_avp_guaranteed_bitrate_ul( "Guaranteed-Bitrate-UL", m_vnd_3gpp.getId() ),
    m_avp_event_charging_timestamp( "Event-Charging-TimeStamp", m_vnd_3gpp.getId() ),
    m_avp_charging_rule_install( "Charging-Rule-Install", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ggsn_address( "3GPP-GGSN-Address", m_vnd_3gpp.getId() ),
    m_avp_application_service_provider_identity( "Application-Service-Provider-Identity", m_vnd_3gpp.getId() ),
    m_avp_content_version( "Content-Version", m_vnd_3gpp.getId() ),
    m_avp_event_report_indication( "Event-Report-Indication", m_vnd_3gpp.getId() ),
    m_avp_job_type( "Job-Type", m_vnd_3gpp.getId() ),
    m_avp_bearer_operation( "Bearer-Operation", m_vnd_3gpp.getId() ),
    m_avp_user_equipment_info_type( "User-Equipment-Info-Type" ),
    m_avp_redirect_server_address( "Redirect-Server-Address" ),
    m_avp_cc_request_number( "CC-Request-Number" ),
    m_avp_framed_ipv6_prefix( "Framed-IPv6-Prefix" ),
    m_avp_tdf_application_instance_identifier( "TDF-Application-Instance-Identifier", m_vnd_3gpp.getId() ),
    m_avp_3gpp_charging_characteristics( "3GPP-Charging-Characteristics", m_vnd_3gpp.getId() ),
    m_avp_proxy_info( "Proxy-Info" ),
    m_avp_used_service_unit( "Used-Service-Unit" ),
    m_avp_3gpp_sgsn_ipv6_address( "3GPP-SGSN-Ipv6-Address", m_vnd_3gpp.getId() ),
    m_avp_origin_realm( "Origin-Realm" ),
    m_avp_twan_identifier( "TWAN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_origin_host( "Origin-Host" ),
    m_avp_flow_label( "Flow-Label", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ggsn_ipv6_address( "3GPP-GGSN-Ipv6-Address", m_vnd_3gpp.getId() ),
    m_avp_guaranteed_bitrate_dl( "Guaranteed-Bitrate-DL", m_vnd_3gpp.getId() ),
    m_avp_restriction_filter_rule( "Restriction-Filter-Rule" ),
    m_avp_3gpp_sgsn_address( "3GPP-SGSN-Address", m_vnd_3gpp.getId() ),
    m_avp_redirect_address_type( "Redirect-Address-Type" ),
    m_avp_tdf_destination_realm( "TDF-Destination-Realm", m_vnd_3gpp.getId() ),
    m_avp_event_threshold_rsrq( "Event-Threshold-RSRQ", m_vnd_3gpp.getId() ),
    m_avp_subscription_id_data( "Subscription-Id-Data" ),
    m_avp_cc_request_type( "CC-Request-Type" ),
    m_avp_nbifom_mode( "NBIFOM-Mode", m_vnd_3gpp.getId() ),
    m_avp_final_unit_indication( "Final-Unit-Indication" ),
    m_avp_usage_monitoring_report( "Usage-Monitoring-Report", m_vnd_3gpp.getId() ),
    m_avp_session_release_cause( "Session-Release-Cause", m_vnd_3gpp.getId() ),
    m_avp_flow_description( "Flow-Description", m_vnd_3gpp.getId() ),
    m_avp_charging_rule_base_name( "Charging-Rule-Base-Name", m_vnd_3gpp.getId() ),
    m_avp_trace_collection_entity( "Trace-Collection-Entity", m_vnd_3gpp.getId() ),
    m_avp_user_equipment_info_value( "User-Equipment-Info-Value" ),
    m_avp_route_record( "Route-Record" ),
    m_avp_presence_reporting_area_identifier( "Presence-Reporting-Area-Identifier", m_vnd_3gpp.getId() ),
    m_avp_csg_information_reporting( "CSG-Information-Reporting", m_vnd_3gpp.getId() ),
    m_avp_filter_id( "Filter-Id" ),
    m_avp_presence_reporting_area_information( "Presence-Reporting-Area-Information", m_vnd_3gpp.getId() ),
    m_avp_csg_id( "CSG-Id", m_vnd_3gpp.getId() ),
    m_avp_ssid( "SSID", m_vnd_3gpp.getId() ),
    m_avp_metering_method( "Metering-Method", m_vnd_3gpp.getId() ),
    m_avp_framed_ip_address( "Framed-IP-Address" ),
    m_avp_3gpp_ps_data_off_status( "3GPP-PS-Data-Off-Status", m_vnd_3gpp.getId() ),
    m_avp_logging_duration( "Logging-Duration", m_vnd_3gpp.getId() ),
    m_avp_apn_aggregate_max_bitrate_dl( "APN-Aggregate-Max-Bitrate-DL", m_vnd_3gpp.getId() ),
    m_avp_conditional_apn_aggregate_max_bitrate( "Conditional-APN-Aggregate-Max-Bitrate", m_vnd_3gpp.getId() ),
    m_avp_access_network_charging_identifier_gx( "Access-Network-Charging-Identifier-Gx", m_vnd_3gpp.getId() ),
    m_avp_positioning_method( "Positioning-Method", m_vnd_3gpp.getId() ),
    m_avp_oc_olr( "OC-OLR" ),
    m_avp_routing_rule_install( "Routing-Rule-Install", m_vnd_3gpp.getId() ),
    m_avp_presence_reporting_area_status( "Presence-Reporting-Area-Status", m_vnd_3gpp.getId() ),
    m_avp_trace_data( "Trace-Data", m_vnd_3gpp.getId() ),
    m_avp_an_gw_status( "AN-GW-Status", m_vnd_3gpp.getId() ),
    m_avp_execution_time( "Execution-Time", m_vnd_3gpp.getId() ),
    m_avp_subscription_id_type( "Subscription-Id-Type" ),
    m_avp_access_network_charging_identifier_value( "Access-Network-Charging-Identifier-Value", m_vnd_3gpp.getId() ),
    m_avp_credit_management_status( "Credit-Management-Status", m_vnd_3gpp.getId() ),
    m_avp_exponent( "Exponent" ),
    m_avp_oc_reduction_percentage( "OC-Reduction-Percentage" ),
    m_avp_default_qos_name( "Default-QoS-Name", m_vnd_3gpp.getId() ),
    m_avp_routing_rule_definition( "Routing-Rule-Definition", m_vnd_3gpp.getId() ),
    m_avp_report_interval( "Report-Interval", m_vnd_3gpp.getId() ),
    m_avp_mdt_configuration( "MDT-Configuration", m_vnd_3gpp.getId() ),
    m_avp_error_reporting_host( "Error-Reporting-Host" ),
    m_avp_charging_rule_remove( "Charging-Rule-Remove", m_vnd_3gpp.getId() ),
    m_avp_3gpp_sgsn_mcc_mnc( "3GPP-SGSN-MCC-MNC", m_vnd_3gpp.getId() ),
    m_avp_unit_value( "Unit-Value" ),
    m_avp_bearer_control_mode( "Bearer-Control-Mode", m_vnd_3gpp.getId() ),
    m_avp_event_threshold_event_1i( "Event-Threshold-Event-1I", m_vnd_3gpp.getId() ),
    m_avp_rating_group( "Rating-Group" ),
    m_avp_rat_type( "RAT-Type", m_vnd_3gpp.getId() ),
    m_avp_online( "Online", m_vnd_3gpp.getId() ),
    m_avp_default_access( "Default-Access", m_vnd_3gpp.getId() ),
    m_avp_event_threshold_event_1f( "Event-Threshold-Event-1F", m_vnd_3gpp.getId() ),
    m_avp_reporting_level( "Reporting-Level", m_vnd_3gpp.getId() ),
    m_avp_allocation_retention_priority( "Allocation-Retention-Priority", m_vnd_3gpp.getId() ),
    m_avp_coa_ip_address( "CoA-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_cell_global_identity( "Cell-Global-Identity", m_vnd_3gpp.getId() ),
    m_avp_access_availability_change_reason( "Access-Availability-Change-Reason", m_vnd_3gpp.getId() ),
    m_avp_oc_validity_duration( "OC-Validity-Duration" ),
    m_avp_csg_membership_indication( "CSG-Membership-Indication", m_vnd_3gpp.getId() ),
    m_avp_media_component_status( "Media-Component-Status", m_vnd_3gpp.getId() ),
    m_avp_sharing_key_dl( "Sharing-Key-DL", m_vnd_3gpp.getId() ),
    m_avp_default_eps_bearer_qos( "Default-EPS-Bearer-QoS", m_vnd_3gpp.getId() ),
    m_avp_experimental_result( "Experimental-Result" ),
    m_avp_user_equipment_info( "User-Equipment-Info" ),
    m_avp_redirect_support( "Redirect-Support", m_vnd_3gpp.getId() ),
    m_avp_measurement_period_lte( "Measurement-Period-LTE", m_vnd_3gpp.getId() ),
    m_avp_routing_rule_report( "Routing-Rule-Report", m_vnd_3gpp.getId() ),
    m_avp_max_requested_bandwidth_dl( "Max-Requested-Bandwidth-DL", m_vnd_3gpp.getId() ),
    m_avp_quota_consumption_time( "Quota-Consumption-Time", m_vnd_3gpp.getId() ),
    m_avp_origin_state_id( "Origin-State-Id" ),
    m_avp_qos_negotiation( "QoS-Negotiation", m_vnd_3gpp.getId() ),
    m_avp_monitoring_key( "Monitoring-Key", m_vnd_3gpp.getId() ),
    m_avp_ran_nas_release_cause( "RAN-NAS-Release-Cause", m_vnd_3gpp.getId() ),
    m_avp_sharing_key_ul( "Sharing-Key-UL", m_vnd_3gpp.getId() ),
    m_avp_netloc_access_support( "NetLoc-Access-Support", m_vnd_3gpp.getId() ),
    m_avp_trace_event_list( "Trace-Event-List", m_vnd_3gpp.getId() ),
    m_avp_3gpp_user_location_info( "3GPP-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_result_code( "Result-Code" ),
    m_avp_trace_interface_list( "Trace-Interface-List", m_vnd_3gpp.getId() ),
    m_avp_fixed_user_location_info( "Fixed-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_default_qos_information( "Default-QoS-Information", m_vnd_3gpp.getId() ),
    m_avp_routing_rule_identifier( "Routing-Rule-Identifier", m_vnd_3gpp.getId() ),
    m_avp_redirect_max_cache_time( "Redirect-Max-Cache-Time" ),
    m_avp_rule_activation_time( "Rule-Activation-Time", m_vnd_3gpp.getId() ),
    m_avp_routing_rule_remove( "Routing-Rule-Remove", m_vnd_3gpp.getId() ),
    m_avp_redirect_server( "Redirect-Server" ),
    m_avp_an_trusted( "AN-Trusted", m_vnd_3gpp.getId() ),
    m_avp_precedence( "Precedence", m_vnd_3gpp.getId() ),
    m_avp_called_station_id( "Called-Station-Id" ),
    m_avp_maximum_wait_time( "Maximum-Wait-Time", m_vnd_3gpp.getId() ),
    m_avp_3gpp_selection_mode( "3GPP-Selection-Mode", m_vnd_3gpp.getId() ),
    m_avp_pcc_rule_status( "PCC-Rule-Status", m_vnd_3gpp.getId() ),
    m_avp_oc_supported_features( "OC-Supported-Features" ),
    m_avp_packet_filter_identifier( "Packet-Filter-Identifier", m_vnd_3gpp.getId() ),
    m_avp_cc_output_octets( "CC-Output-Octets" ),
    m_avp_packet_filter_operation( "Packet-Filter-Operation", m_vnd_3gpp.getId() ),
    m_avp_proxy_host( "Proxy-Host" ),
    m_avp_event_threshold_rsrp( "Event-Threshold-RSRP", m_vnd_3gpp.getId() ),
    m_avp_user_location_info_time( "User-Location-Info-Time", m_vnd_3gpp.getId() ),
    m_avp_packet_filter_information( "Packet-Filter-Information", m_vnd_3gpp.getId() ),
    m_avp_monitoring_flags( "Monitoring-Flags", m_vnd_3gpp.getId() ),
    m_avp_experimental_result_code( "Experimental-Result-Code" ),
    m_avp_collection_period_rrm_lte( "Collection-Period-RRM-LTE", m_vnd_3gpp.getId() ),
    m_avp_pdn_connection_id( "PDN-Connection-ID", m_vnd_3gpp.getId() ),
    m_avp_access_network_charging_address( "Access-Network-Charging-Address", m_vnd_3gpp.getId() ),
    m_avp_auth_application_id( "Auth-Application-Id" ),
    m_avp_revalidation_time( "Revalidation-Time", m_vnd_3gpp.getId() ),
    m_avp_value_digits( "Value-Digits" ),
    m_avp_event_trigger( "Event-Trigger", m_vnd_3gpp.getId() ),
    m_avp_presence_reporting_area_elements_list( "Presence-Reporting-Area-Elements-List", m_vnd_3gpp.getId() ),
    m_avp_charging_information( "Charging-Information", m_vnd_3gpp.getId() ),
    m_avp_tft_packet_filter_information( "TFT-Packet-Filter-Information", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ms_timezone( "3GPP-MS-TimeZone", m_vnd_3gpp.getId() ),
    m_avp_charging_rule_name( "Charging-Rule-Name", m_vnd_3gpp.getId() ),
    m_avp_3gpp2_bsid( "3GPP2-BSID", m_vnd_3gpp2.getId() ),
    m_avp_dynamic_address_flag( "Dynamic-Address-Flag", m_vnd_3gpp.getId() ),
    m_avp_subscription_id( "Subscription-Id" ),
    m_avp_collection_period_rrm_umts( "Collection-Period-RRM-UMTS", m_vnd_3gpp.getId() ),
    m_avp_charging_rule_report( "Charging-Rule-Report", m_vnd_3gpp.getId() ),
    m_avp_ip_can_type( "IP-CAN-Type", m_vnd_3gpp.getId() ),
    m_avp_offline( "Offline", m_vnd_3gpp.getId() ),
    m_avp_udp_source_port( "UDP-Source-Port", m_vnd_3gpp.getId() ),
    m_avp_routing_ip_address( "Routing-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_redirect_information( "Redirect-Information", m_vnd_3gpp.getId() ),
    m_avp_primary_event_charging_function_name( "Primary-Event-Charging-Function-Name", m_vnd_3gpp.getId() ),
    m_avp_media_component_number( "Media-Component-Number", m_vnd_3gpp.getId() ),
    m_avp_tariff_time_change( "Tariff-Time-Change" ),
    m_avp_error_message( "Error-Message" ),
    m_avp_qos_upgrade( "QoS-Upgrade", m_vnd_3gpp.getId() ),
    m_avp_ip_can_session_charging_scope( "IP-CAN-Session-Charging-Scope", m_vnd_3gpp.getId() ),
    m_avp_reporting_trigger( "Reporting-Trigger", m_vnd_3gpp.getId() ),
    m_avp_failed_avp( "Failed-AVP" ),
    m_avp_routing_area_identity( "Routing-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_usage_monitoring_information( "Usage-Monitoring-Information", m_vnd_3gpp.getId() ),
    m_avp_cc_total_octets( "CC-Total-Octets" ),
    m_avp_security_parameter_index( "Security-Parameter-Index", m_vnd_3gpp.getId() ),
    m_avp_trace_reference( "Trace-Reference", m_vnd_3gpp.getId() ),
    m_avp_flow_direction( "Flow-Direction", m_vnd_3gpp.getId() ),
    m_avp_tdf_application_identifier( "TDF-Application-Identifier", m_vnd_3gpp.getId() ),
    m_avp_currency_code( "Currency-Code" ),
    m_avp_cc_input_octets( "CC-Input-Octets" ),
    m_avp_measurement_quantity( "Measurement-Quantity", m_vnd_3gpp.getId() ),
    m_avp_removal_of_access( "Removal-Of-Access", m_vnd_3gpp.getId() ),
    m_avp_measurement_period_umts( "Measurement-Period-UMTS", m_vnd_3gpp.getId() ),
    m_avp_trace_depth( "Trace-Depth", m_vnd_3gpp.getId() ),
    m_avp_proxy_state( "Proxy-State" ),
    m_avp_rule_failure_code( "Rule-Failure-Code", m_vnd_3gpp.getId() ),
    m_avp_af_charging_identifier( "AF-Charging-Identifier", m_vnd_3gpp.getId() ),
    m_avp_tunnel_header_length( "Tunnel-Header-Length", m_vnd_3gpp.getId() ),
    m_avp_routing_rule_failure_code( "Routing-Rule-Failure-Code", m_vnd_3gpp.getId() ),
    m_avp_coa_information( "CoA-Information", m_vnd_3gpp.getId() ),
    m_avp_default_bearer_indication( "Default-Bearer-Indication", m_vnd_3gpp.getId() ),
    m_avp_vendor_id( "Vendor-Id" ),
    m_avp_granted_service_unit( "Granted-Service-Unit" ),
    m_avp_max_requested_bandwidth_ul( "Max-Requested-Bandwidth-UL", m_vnd_3gpp.getId() ),
    m_avp_supported_features( "Supported-Features", m_vnd_3gpp.getId() ),
    m_avp_load_type( "Load-Type" ),
    m_avp_redirect_host( "Redirect-Host" ),
    m_avp_destination_realm( "Destination-Realm" ),
    m_avp_session_id( "Session-Id" ),
    m_avp_tos_traffic_class( "ToS-Traffic-Class", m_vnd_3gpp.getId() ),
    m_avp_origination_time_stamp( "Origination-Time-Stamp", m_vnd_3gpp.getId() ),
    m_avp_bssid( "BSSID", m_vnd_3gpp.getId() ),
    m_avp_cc_money( "CC-Money" ),
    m_avp_application_detection_information( "Application-Detection-Information", m_vnd_3gpp.getId() ),
    m_avp_mdt_allowed_plmn_id( "MDT-Allowed-PLMN-Id", m_vnd_3gpp.getId() ),
    m_avp_tariff_change_usage( "Tariff-Change-Usage" ),
    m_avp_report_amount( "Report-Amount", m_vnd_3gpp.getId() ),
    m_avp_mute_notification( "Mute-Notification", m_vnd_3gpp.getId() ),
    m_avp_pdn_connection_charging_id( "PDN-Connection-Charging-ID", m_vnd_3gpp.getId() ),
    m_avp_tdf_ip_address( "TDF-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_flows( "Flows", m_vnd_3gpp.getId() ),
    m_avp_ps_to_cs_session_continuity( "PS-to-CS-Session-Continuity", m_vnd_3gpp.getId() ),
    m_avp_primary_charging_collection_function_name( "Primary-Charging-Collection-Function-Name", m_vnd_3gpp.getId() ),
    m_avp_user_csg_information( "User-CSG-Information", m_vnd_3gpp.getId() )
{
    std::cout << "Registering gx dictionary" << std::endl;
};

Dictionary::~Dictionary()
{
};

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

PraRemoveExtractor::PraRemoveExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpPraRemove() ),
    presence_reporting_area_identifier( *this, dict.avpPresenceReportingAreaIdentifier() )
{
    add( presence_reporting_area_identifier );
}

PraRemoveExtractor::~PraRemoveExtractor()
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

DefaultEpsBearerQosExtractor::DefaultEpsBearerQosExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpDefaultEpsBearerQos() ),
    qos_class_identifier( *this, dict.avpQosClassIdentifier() ),
    allocation_retention_priority( *this, dict )
{
    add( qos_class_identifier );
    add( allocation_retention_priority );
}

DefaultEpsBearerQosExtractor::~DefaultEpsBearerQosExtractor()
{
}

ConditionalPolicyInformationExtractor::ConditionalPolicyInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpConditionalPolicyInformation() ),
    execution_time( *this, dict.avpExecutionTime() ),
    default_eps_bearer_qos( *this, dict ),
    apn_aggregate_max_bitrate_ul( *this, dict.avpApnAggregateMaxBitrateUl() ),
    apn_aggregate_max_bitrate_dl( *this, dict.avpApnAggregateMaxBitrateDl() ),
    conditional_apn_aggregate_max_bitrate( *this, dict )
{
    add( execution_time );
    add( default_eps_bearer_qos );
    add( apn_aggregate_max_bitrate_ul );
    add( apn_aggregate_max_bitrate_dl );
    add( conditional_apn_aggregate_max_bitrate );
}

ConditionalPolicyInformationExtractor::~ConditionalPolicyInformationExtractor()
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

PresenceReportingAreaInformationExtractorList::PresenceReportingAreaInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpPresenceReportingAreaInformation() ),
    m_dict( dict )
{
}

PresenceReportingAreaInformationExtractorList::~PresenceReportingAreaInformationExtractorList()
{
}

PraInstallExtractor::PraInstallExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpPraInstall() ),
    presence_reporting_area_information( *this, dict )
{
    add( presence_reporting_area_information );
}

PraInstallExtractor::~PraInstallExtractor()
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

FlowInformationExtractorList::FlowInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpFlowInformation() ),
    m_dict( dict )
{
}

FlowInformationExtractorList::~FlowInformationExtractorList()
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

FlowsExtractorList::FlowsExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpFlows() ),
    m_dict( dict )
{
}

FlowsExtractorList::~FlowsExtractorList()
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

ChargingRuleDefinitionExtractor::ChargingRuleDefinitionExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpChargingRuleDefinition() ),
    charging_rule_name( *this, dict.avpChargingRuleName() ),
    service_identifier( *this, dict.avpServiceIdentifier() ),
    rating_group( *this, dict.avpRatingGroup() ),
    flow_information( *this, dict ),
    default_bearer_indication( *this, dict.avpDefaultBearerIndication() ),
    tdf_application_identifier( *this, dict.avpTdfApplicationIdentifier() ),
    flow_status( *this, dict.avpFlowStatus() ),
    qos_information( *this, dict ),
    ps_to_cs_session_continuity( *this, dict.avpPsToCsSessionContinuity() ),
    reporting_level( *this, dict.avpReportingLevel() ),
    online( *this, dict.avpOnline() ),
    offline( *this, dict.avpOffline() ),
    metering_method( *this, dict.avpMeteringMethod() ),
    precedence( *this, dict.avpPrecedence() ),
    af_charging_identifier( *this, dict.avpAfChargingIdentifier() ),
    flows( *this, dict ),
    monitoring_key( *this, dict.avpMonitoringKey() ),
    redirect_information( *this, dict ),
    mute_notification( *this, dict.avpMuteNotification() ),
    af_signalling_protocol( *this, dict.avpAfSignallingProtocol() ),
    sponsor_identity( *this, dict.avpSponsorIdentity() ),
    application_service_provider_identity( *this, dict.avpApplicationServiceProviderIdentity() ),
    required_access_info( *this, dict.avpRequiredAccessInfo() ),
    sharing_key_dl( *this, dict.avpSharingKeyDl() ),
    sharing_key_ul( *this, dict.avpSharingKeyUl() ),
    traffic_steering_policy_identifier_dl( *this, dict.avpTrafficSteeringPolicyIdentifierDl() ),
    traffic_steering_policy_identifier_ul( *this, dict.avpTrafficSteeringPolicyIdentifierUl() ),
    content_version( *this, dict.avpContentVersion() )
{
    add( charging_rule_name );
    add( service_identifier );
    add( rating_group );
    add( flow_information );
    add( default_bearer_indication );
    add( tdf_application_identifier );
    add( flow_status );
    add( qos_information );
    add( ps_to_cs_session_continuity );
    add( reporting_level );
    add( online );
    add( offline );
    add( metering_method );
    add( precedence );
    add( af_charging_identifier );
    add( flows );
    add( monitoring_key );
    add( redirect_information );
    add( mute_notification );
    add( af_signalling_protocol );
    add( sponsor_identity );
    add( application_service_provider_identity );
    add( required_access_info );
    add( sharing_key_dl );
    add( sharing_key_ul );
    add( traffic_steering_policy_identifier_dl );
    add( traffic_steering_policy_identifier_ul );
    add( content_version );
}

ChargingRuleDefinitionExtractor::~ChargingRuleDefinitionExtractor()
{
}

RoutingFilterExtractor::RoutingFilterExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRoutingFilter() ),
    flow_description( *this, dict.avpFlowDescription() ),
    flow_direction( *this, dict.avpFlowDirection() ),
    tos_traffic_class( *this, dict.avpTosTrafficClass() ),
    security_parameter_index( *this, dict.avpSecurityParameterIndex() ),
    flow_label( *this, dict.avpFlowLabel() )
{
    add( flow_description );
    add( flow_direction );
    add( tos_traffic_class );
    add( security_parameter_index );
    add( flow_label );
}

RoutingFilterExtractor::~RoutingFilterExtractor()
{
}

TdfInformationExtractor::TdfInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTdfInformation() ),
    tdf_destination_realm( *this, dict.avpTdfDestinationRealm() ),
    tdf_destination_host( *this, dict.avpTdfDestinationHost() ),
    tdf_ip_address( *this, dict.avpTdfIpAddress() )
{
    add( tdf_destination_realm );
    add( tdf_destination_host );
    add( tdf_ip_address );
}

TdfInformationExtractor::~TdfInformationExtractor()
{
}

TunnelInformationExtractor::TunnelInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTunnelInformation() ),
    tunnel_header_length( *this, dict.avpTunnelHeaderLength() )
{
    add( tunnel_header_length );
}

TunnelInformationExtractor::~TunnelInformationExtractor()
{
}

ChargingRuleDefinitionExtractorList::ChargingRuleDefinitionExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpChargingRuleDefinition() ),
    m_dict( dict )
{
}

ChargingRuleDefinitionExtractorList::~ChargingRuleDefinitionExtractorList()
{
}

ChargingRuleInstallExtractor::ChargingRuleInstallExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpChargingRuleInstall() ),
    charging_rule_definition( *this, dict ),
    charging_rule_name( *this, dict.avpChargingRuleName() ),
    charging_rule_base_name( *this, dict.avpChargingRuleBaseName() ),
    bearer_identifier( *this, dict.avpBearerIdentifier() ),
    monitoring_flags( *this, dict.avpMonitoringFlags() ),
    rule_activation_time( *this, dict.avpRuleActivationTime() ),
    rule_deactivation_time( *this, dict.avpRuleDeactivationTime() ),
    resource_allocation_notification( *this, dict.avpResourceAllocationNotification() ),
    charging_correlation_indicator( *this, dict.avpChargingCorrelationIndicator() ),
    ip_can_type( *this, dict.avpIpCanType() )
{
    add( charging_rule_definition );
    add( charging_rule_name );
    add( charging_rule_base_name );
    add( bearer_identifier );
    add( monitoring_flags );
    add( rule_activation_time );
    add( rule_deactivation_time );
    add( resource_allocation_notification );
    add( charging_correlation_indicator );
    add( ip_can_type );
}

ChargingRuleInstallExtractor::~ChargingRuleInstallExtractor()
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

AccessNetworkChargingIdentifierGxExtractor::AccessNetworkChargingIdentifierGxExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAccessNetworkChargingIdentifierGx() ),
    access_network_charging_identifier_value( *this, dict.avpAccessNetworkChargingIdentifierValue() ),
    charging_rule_base_name( *this, dict.avpChargingRuleBaseName() ),
    charging_rule_name( *this, dict.avpChargingRuleName() ),
    ip_can_session_charging_scope( *this, dict.avpIpCanSessionChargingScope() )
{
    add( access_network_charging_identifier_value );
    add( charging_rule_base_name );
    add( charging_rule_name );
    add( ip_can_session_charging_scope );
}

AccessNetworkChargingIdentifierGxExtractor::~AccessNetworkChargingIdentifierGxExtractor()
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

RoutingFilterExtractorList::RoutingFilterExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpRoutingFilter() ),
    m_dict( dict )
{
}

RoutingFilterExtractorList::~RoutingFilterExtractorList()
{
}

RoutingRuleDefinitionExtractor::RoutingRuleDefinitionExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRoutingRuleDefinition() ),
    routing_rule_identifier( *this, dict.avpRoutingRuleIdentifier() ),
    routing_filter( *this, dict ),
    precedence( *this, dict.avpPrecedence() ),
    routing_ip_address( *this, dict.avpRoutingIpAddress() ),
    ip_can_type( *this, dict.avpIpCanType() )
{
    add( routing_rule_identifier );
    add( routing_filter );
    add( precedence );
    add( routing_ip_address );
    add( ip_can_type );
}

RoutingRuleDefinitionExtractor::~RoutingRuleDefinitionExtractor()
{
}

RoutingRuleDefinitionExtractorList::RoutingRuleDefinitionExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpRoutingRuleDefinition() ),
    m_dict( dict )
{
}

RoutingRuleDefinitionExtractorList::~RoutingRuleDefinitionExtractorList()
{
}

RoutingRuleInstallExtractor::RoutingRuleInstallExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRoutingRuleInstall() ),
    routing_rule_definition( *this, dict )
{
    add( routing_rule_definition );
}

RoutingRuleInstallExtractor::~RoutingRuleInstallExtractor()
{
}

ChargingRuleRemoveExtractor::ChargingRuleRemoveExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpChargingRuleRemove() ),
    charging_rule_name( *this, dict.avpChargingRuleName() ),
    charging_rule_base_name( *this, dict.avpChargingRuleBaseName() ),
    required_access_info( *this, dict.avpRequiredAccessInfo() ),
    resource_release_notification( *this, dict.avpResourceReleaseNotification() )
{
    add( charging_rule_name );
    add( charging_rule_base_name );
    add( required_access_info );
    add( resource_release_notification );
}

ChargingRuleRemoveExtractor::~ChargingRuleRemoveExtractor()
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

RoutingRuleReportExtractor::RoutingRuleReportExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRoutingRuleReport() ),
    routing_rule_identifier( *this, dict.avpRoutingRuleIdentifier() ),
    pcc_rule_status( *this, dict.avpPccRuleStatus() ),
    routing_rule_failure_code( *this, dict.avpRoutingRuleFailureCode() )
{
    add( routing_rule_identifier );
    add( pcc_rule_status );
    add( routing_rule_failure_code );
}

RoutingRuleReportExtractor::~RoutingRuleReportExtractor()
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

DefaultQosInformationExtractor::DefaultQosInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpDefaultQosInformation() ),
    qos_class_identifier( *this, dict.avpQosClassIdentifier() ),
    max_requested_bandwidth_ul( *this, dict.avpMaxRequestedBandwidthUl() ),
    max_requested_bandwidth_dl( *this, dict.avpMaxRequestedBandwidthDl() ),
    default_qos_name( *this, dict.avpDefaultQosName() )
{
    add( qos_class_identifier );
    add( max_requested_bandwidth_ul );
    add( max_requested_bandwidth_dl );
    add( default_qos_name );
}

DefaultQosInformationExtractor::~DefaultQosInformationExtractor()
{
}

RoutingRuleRemoveExtractor::RoutingRuleRemoveExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRoutingRuleRemove() ),
    routing_rule_identifier( *this, dict.avpRoutingRuleIdentifier() )
{
    add( routing_rule_identifier );
}

RoutingRuleRemoveExtractor::~RoutingRuleRemoveExtractor()
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

PacketFilterInformationExtractor::PacketFilterInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpPacketFilterInformation() ),
    packet_filter_identifier( *this, dict.avpPacketFilterIdentifier() ),
    precedence( *this, dict.avpPrecedence() ),
    packet_filter_content( *this, dict.avpPacketFilterContent() ),
    tos_traffic_class( *this, dict.avpTosTrafficClass() ),
    security_parameter_index( *this, dict.avpSecurityParameterIndex() ),
    flow_label( *this, dict.avpFlowLabel() ),
    flow_direction( *this, dict.avpFlowDirection() )
{
    add( packet_filter_identifier );
    add( precedence );
    add( packet_filter_content );
    add( tos_traffic_class );
    add( security_parameter_index );
    add( flow_label );
    add( flow_direction );
}

PacketFilterInformationExtractor::~PacketFilterInformationExtractor()
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

TftPacketFilterInformationExtractor::TftPacketFilterInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTftPacketFilterInformation() ),
    precedence( *this, dict.avpPrecedence() ),
    tft_filter( *this, dict.avpTftFilter() ),
    tos_traffic_class( *this, dict.avpTosTrafficClass() ),
    security_parameter_index( *this, dict.avpSecurityParameterIndex() ),
    flow_label( *this, dict.avpFlowLabel() ),
    flow_direction( *this, dict.avpFlowDirection() )
{
    add( precedence );
    add( tft_filter );
    add( tos_traffic_class );
    add( security_parameter_index );
    add( flow_label );
    add( flow_direction );
}

TftPacketFilterInformationExtractor::~TftPacketFilterInformationExtractor()
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

ChargingRuleReportExtractor::ChargingRuleReportExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpChargingRuleReport() ),
    charging_rule_name( *this, dict.avpChargingRuleName() ),
    charging_rule_base_name( *this, dict.avpChargingRuleBaseName() ),
    bearer_identifier( *this, dict.avpBearerIdentifier() ),
    pcc_rule_status( *this, dict.avpPccRuleStatus() ),
    rule_failure_code( *this, dict.avpRuleFailureCode() ),
    final_unit_indication( *this, dict ),
    ran_nas_release_cause( *this, dict.avpRanNasReleaseCause() ),
    content_version( *this, dict.avpContentVersion() )
{
    add( charging_rule_name );
    add( charging_rule_base_name );
    add( bearer_identifier );
    add( pcc_rule_status );
    add( rule_failure_code );
    add( final_unit_indication );
    add( ran_nas_release_cause );
    add( content_version );
}

ChargingRuleReportExtractor::~ChargingRuleReportExtractor()
{
}

FailedAvpExtractor::FailedAvpExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFailedAvp() )
{

}

FailedAvpExtractor::~FailedAvpExtractor()
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

CoaInformationExtractor::CoaInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCoaInformation() ),
    tunnel_information( *this, dict ),
    coa_ip_address( *this, dict.avpCoaIpAddress() )
{
    add( tunnel_information );
    add( coa_ip_address );
}

CoaInformationExtractor::~CoaInformationExtractor()
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

PacketFilterInformationExtractorList::PacketFilterInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpPacketFilterInformation() ),
    m_dict( dict )
{
}

PacketFilterInformationExtractorList::~PacketFilterInformationExtractorList()
{
}

TftPacketFilterInformationExtractorList::TftPacketFilterInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpTftPacketFilterInformation() ),
    m_dict( dict )
{
}

TftPacketFilterInformationExtractorList::~TftPacketFilterInformationExtractorList()
{
}

ChargingRuleReportExtractorList::ChargingRuleReportExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpChargingRuleReport() ),
    m_dict( dict )
{
}

ChargingRuleReportExtractorList::~ChargingRuleReportExtractorList()
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

AccessNetworkChargingIdentifierGxExtractorList::AccessNetworkChargingIdentifierGxExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAccessNetworkChargingIdentifierGx() ),
    m_dict( dict )
{
}

AccessNetworkChargingIdentifierGxExtractorList::~AccessNetworkChargingIdentifierGxExtractorList()
{
}

CoaInformationExtractorList::CoaInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpCoaInformation() ),
    m_dict( dict )
{
}

CoaInformationExtractorList::~CoaInformationExtractorList()
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

CreditControlRequestExtractor::CreditControlRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_application_id( *this, dict.avpAuthApplicationId() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    cc_request_type( *this, dict.avpCcRequestType() ),
    cc_request_number( *this, dict.avpCcRequestNumber() ),
    credit_management_status( *this, dict.avpCreditManagementStatus() ),
    destination_host( *this, dict.avpDestinationHost() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    subscription_id( *this, dict ),
    oc_supported_features( *this, dict ),
    supported_features( *this, dict ),
    tdf_information( *this, dict ),
    network_request_support( *this, dict.avpNetworkRequestSupport() ),
    packet_filter_information( *this, dict ),
    packet_filter_operation( *this, dict.avpPacketFilterOperation() ),
    bearer_identifier( *this, dict.avpBearerIdentifier() ),
    bearer_operation( *this, dict.avpBearerOperation() ),
    dynamic_address_flag( *this, dict.avpDynamicAddressFlag() ),
    dynamic_address_flag_extension( *this, dict.avpDynamicAddressFlagExtension() ),
    pdn_connection_charging_id( *this, dict.avpPdnConnectionChargingId() ),
    framed_ip_address( *this, dict.avpFramedIpAddress() ),
    framed_ipv6_prefix( *this, dict.avpFramedIpv6Prefix() ),
    ip_can_type( *this, dict.avpIpCanType() ),
    tgpp_rat_type( *this, dict.avp3gppRatType() ),
    an_trusted( *this, dict.avpAnTrusted() ),
    rat_type( *this, dict.avpRatType() ),
    termination_cause( *this, dict.avpTerminationCause() ),
    user_equipment_info( *this, dict ),
    qos_information( *this, dict ),
    qos_negotiation( *this, dict.avpQosNegotiation() ),
    qos_upgrade( *this, dict.avpQosUpgrade() ),
    default_eps_bearer_qos( *this, dict ),
    default_qos_information( *this, dict ),
    an_gw_address( *this, dict.avpAnGwAddress() ),
    an_gw_status( *this, dict.avpAnGwStatus() ),
    tgpp_sgsn_mcc_mnc( *this, dict.avp3gppSgsnMccMnc() ),
    tgpp_sgsn_address( *this, dict.avp3gppSgsnAddress() ),
    tgpp_sgsn_ipv6_address( *this, dict.avp3gppSgsnIpv6Address() ),
    tgpp_ggsn_address( *this, dict.avp3gppGgsnAddress() ),
    tgpp_ggsn_ipv6_address( *this, dict.avp3gppGgsnIpv6Address() ),
    tgpp_selection_mode( *this, dict.avp3gppSelectionMode() ),
    rai( *this, dict.avpRai() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    fixed_user_location_info( *this, dict ),
    user_location_info_time( *this, dict.avpUserLocationInfoTime() ),
    user_csg_information( *this, dict ),
    twan_identifier( *this, dict.avpTwanIdentifier() ),
    tgpp_ms_timezone( *this, dict.avp3gppMsTimezone() ),
    ran_nas_release_cause( *this, dict.avpRanNasReleaseCause() ),
    tgpp_charging_characteristics( *this, dict.avp3gppChargingCharacteristics() ),
    called_station_id( *this, dict.avpCalledStationId() ),
    pdn_connection_id( *this, dict.avpPdnConnectionId() ),
    bearer_usage( *this, dict.avpBearerUsage() ),
    online( *this, dict.avpOnline() ),
    offline( *this, dict.avpOffline() ),
    tft_packet_filter_information( *this, dict ),
    charging_rule_report( *this, dict ),
    application_detection_information( *this, dict ),
    event_trigger( *this, dict.avpEventTrigger() ),
    event_report_indication( *this, dict ),
    access_network_charging_address( *this, dict.avpAccessNetworkChargingAddress() ),
    access_network_charging_identifier_gx( *this, dict ),
    coa_information( *this, dict ),
    usage_monitoring_information( *this, dict ),
    nbifom_support( *this, dict.avpNbifomSupport() ),
    nbifom_mode( *this, dict.avpNbifomMode() ),
    default_access( *this, dict.avpDefaultAccess() ),
    origination_time_stamp( *this, dict.avpOriginationTimeStamp() ),
    maximum_wait_time( *this, dict.avpMaximumWaitTime() ),
    access_availability_change_reason( *this, dict.avpAccessAvailabilityChangeReason() ),
    routing_rule_install( *this, dict ),
    routing_rule_remove( *this, dict ),
    henb_local_ip_address( *this, dict.avpHenbLocalIpAddress() ),
    ue_local_ip_address( *this, dict.avpUeLocalIpAddress() ),
    udp_source_port( *this, dict.avpUdpSourcePort() ),
    presence_reporting_area_information( *this, dict ),
    logical_access_id( *this, dict.avpLogicalAccessId() ),
    physical_access_id( *this, dict.avpPhysicalAccessId() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() ),
    tgpp_ps_data_off_status( *this, dict.avp3gppPsDataOffStatus() )
{
    add( session_id );
    add( drmp );
    add( auth_application_id );
    add( origin_host );
    add( origin_realm );
    add( destination_realm );
    add( cc_request_type );
    add( cc_request_number );
    add( credit_management_status );
    add( destination_host );
    add( origin_state_id );
    add( subscription_id );
    add( oc_supported_features );
    add( supported_features );
    add( tdf_information );
    add( network_request_support );
    add( packet_filter_information );
    add( packet_filter_operation );
    add( bearer_identifier );
    add( bearer_operation );
    add( dynamic_address_flag );
    add( dynamic_address_flag_extension );
    add( pdn_connection_charging_id );
    add( framed_ip_address );
    add( framed_ipv6_prefix );
    add( ip_can_type );
    add( tgpp_rat_type );
    add( an_trusted );
    add( rat_type );
    add( termination_cause );
    add( user_equipment_info );
    add( qos_information );
    add( qos_negotiation );
    add( qos_upgrade );
    add( default_eps_bearer_qos );
    add( default_qos_information );
    add( an_gw_address );
    add( an_gw_status );
    add( tgpp_sgsn_mcc_mnc );
    add( tgpp_sgsn_address );
    add( tgpp_sgsn_ipv6_address );
    add( tgpp_ggsn_address );
    add( tgpp_ggsn_ipv6_address );
    add( tgpp_selection_mode );
    add( rai );
    add( tgpp_user_location_info );
    add( fixed_user_location_info );
    add( user_location_info_time );
    add( user_csg_information );
    add( twan_identifier );
    add( tgpp_ms_timezone );
    add( ran_nas_release_cause );
    add( tgpp_charging_characteristics );
    add( called_station_id );
    add( pdn_connection_id );
    add( bearer_usage );
    add( online );
    add( offline );
    add( tft_packet_filter_information );
    add( charging_rule_report );
    add( application_detection_information );
    add( event_trigger );
    add( event_report_indication );
    add( access_network_charging_address );
    add( access_network_charging_identifier_gx );
    add( coa_information );
    add( usage_monitoring_information );
    add( nbifom_support );
    add( nbifom_mode );
    add( default_access );
    add( origination_time_stamp );
    add( maximum_wait_time );
    add( access_availability_change_reason );
    add( routing_rule_install );
    add( routing_rule_remove );
    add( henb_local_ip_address );
    add( ue_local_ip_address );
    add( udp_source_port );
    add( presence_reporting_area_information );
    add( logical_access_id );
    add( physical_access_id );
    add( proxy_info );
    add( route_record );
    add( tgpp_ps_data_off_status );
}

CreditControlRequestExtractor::~CreditControlRequestExtractor()
{
}

ChargingRuleRemoveExtractorList::ChargingRuleRemoveExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpChargingRuleRemove() ),
    m_dict( dict )
{
}

ChargingRuleRemoveExtractorList::~ChargingRuleRemoveExtractorList()
{
}

ChargingRuleInstallExtractorList::ChargingRuleInstallExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpChargingRuleInstall() ),
    m_dict( dict )
{
}

ChargingRuleInstallExtractorList::~ChargingRuleInstallExtractorList()
{
}

QosInformationExtractorList::QosInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpQosInformation() ),
    m_dict( dict )
{
}

QosInformationExtractorList::~QosInformationExtractorList()
{
}

RoutingRuleReportExtractorList::RoutingRuleReportExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpRoutingRuleReport() ),
    m_dict( dict )
{
}

RoutingRuleReportExtractorList::~RoutingRuleReportExtractorList()
{
}

ConditionalPolicyInformationExtractorList::ConditionalPolicyInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpConditionalPolicyInformation() ),
    m_dict( dict )
{
}

ConditionalPolicyInformationExtractorList::~ConditionalPolicyInformationExtractorList()
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
    supported_features( *this, dict ),
    bearer_control_mode( *this, dict.avpBearerControlMode() ),
    event_trigger( *this, dict.avpEventTrigger() ),
    event_report_indication( *this, dict ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    redirect_host( *this, dict.avpRedirectHost() ),
    redirect_host_usage( *this, dict.avpRedirectHostUsage() ),
    redirect_max_cache_time( *this, dict.avpRedirectMaxCacheTime() ),
    charging_rule_remove( *this, dict ),
    charging_rule_install( *this, dict ),
    charging_information( *this, dict ),
    online( *this, dict.avpOnline() ),
    offline( *this, dict.avpOffline() ),
    qos_information( *this, dict ),
    revalidation_time( *this, dict.avpRevalidationTime() ),
    default_eps_bearer_qos( *this, dict ),
    default_qos_information( *this, dict ),
    bearer_usage( *this, dict.avpBearerUsage() ),
    usage_monitoring_information( *this, dict ),
    csg_information_reporting( *this, dict.avpCsgInformationReporting() ),
    user_csg_information( *this, dict ),
    pra_install( *this, dict ),
    pra_remove( *this, dict ),
    presence_reporting_area_information( *this, dict ),
    session_release_cause( *this, dict.avpSessionReleaseCause() ),
    nbifom_support( *this, dict.avpNbifomSupport() ),
    nbifom_mode( *this, dict.avpNbifomMode() ),
    default_access( *this, dict.avpDefaultAccess() ),
    ran_rule_support( *this, dict.avpRanRuleSupport() ),
    routing_rule_report( *this, dict ),
    conditional_policy_information( *this, dict ),
    removal_of_access( *this, dict.avpRemovalOfAccess() ),
    ip_can_type( *this, dict.avpIpCanType() ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() ),
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
    add( supported_features );
    add( bearer_control_mode );
    add( event_trigger );
    add( event_report_indication );
    add( origin_state_id );
    add( redirect_host );
    add( redirect_host_usage );
    add( redirect_max_cache_time );
    add( charging_rule_remove );
    add( charging_rule_install );
    add( charging_information );
    add( online );
    add( offline );
    add( qos_information );
    add( revalidation_time );
    add( default_eps_bearer_qos );
    add( default_qos_information );
    add( bearer_usage );
    add( usage_monitoring_information );
    add( csg_information_reporting );
    add( user_csg_information );
    add( pra_install );
    add( pra_remove );
    add( presence_reporting_area_information );
    add( session_release_cause );
    add( nbifom_support );
    add( nbifom_mode );
    add( default_access );
    add( ran_rule_support );
    add( routing_rule_report );
    add( conditional_policy_information );
    add( removal_of_access );
    add( ip_can_type );
    add( error_message );
    add( error_reporting_host );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
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
    event_trigger( *this, dict.avpEventTrigger() ),
    event_report_indication( *this, dict ),
    charging_rule_remove( *this, dict ),
    charging_rule_install( *this, dict ),
    default_eps_bearer_qos( *this, dict ),
    qos_information( *this, dict ),
    default_qos_information( *this, dict ),
    revalidation_time( *this, dict.avpRevalidationTime() ),
    usage_monitoring_information( *this, dict ),
    pcscf_restoration_indication( *this, dict.avpPcscfRestorationIndication() ),
    conditional_policy_information( *this, dict ),
    removal_of_access( *this, dict.avpRemovalOfAccess() ),
    ip_can_type( *this, dict.avpIpCanType() ),
    pra_install( *this, dict ),
    pra_remove( *this, dict ),
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
    add( event_trigger );
    add( event_report_indication );
    add( charging_rule_remove );
    add( charging_rule_install );
    add( default_eps_bearer_qos );
    add( qos_information );
    add( default_qos_information );
    add( revalidation_time );
    add( usage_monitoring_information );
    add( pcscf_restoration_indication );
    add( conditional_policy_information );
    add( removal_of_access );
    add( ip_can_type );
    add( pra_install );
    add( pra_remove );
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
    ip_can_type( *this, dict.avpIpCanType() ),
    rat_type( *this, dict.avpRatType() ),
    an_trusted( *this, dict.avpAnTrusted() ),
    an_gw_address( *this, dict.avpAnGwAddress() ),
    tgpp_sgsn_mcc_mnc( *this, dict.avp3gppSgsnMccMnc() ),
    tgpp_sgsn_address( *this, dict.avp3gppSgsnAddress() ),
    tgpp_sgsn_ipv6_address( *this, dict.avp3gppSgsnIpv6Address() ),
    rai( *this, dict.avpRai() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    user_location_info_time( *this, dict.avpUserLocationInfoTime() ),
    netloc_access_support( *this, dict.avpNetlocAccessSupport() ),
    user_csg_information( *this, dict ),
    tgpp_ms_timezone( *this, dict.avp3gppMsTimezone() ),
    default_qos_information( *this, dict ),
    charging_rule_report( *this, dict ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict )
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
    add( ip_can_type );
    add( rat_type );
    add( an_trusted );
    add( an_gw_address );
    add( tgpp_sgsn_mcc_mnc );
    add( tgpp_sgsn_address );
    add( tgpp_sgsn_ipv6_address );
    add( rai );
    add( tgpp_user_location_info );
    add( user_location_info_time );
    add( netloc_access_support );
    add( user_csg_information );
    add( tgpp_ms_timezone );
    add( default_qos_information );
    add( charging_rule_report );
    add( error_message );
    add( error_reporting_host );
    add( failed_avp );
    add( proxy_info );
}

ReAuthAnswerExtractor::~ReAuthAnswerExtractor()
{
}


}
