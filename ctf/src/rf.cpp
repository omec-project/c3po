/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <string>
#include <iostream>
#include <sstream>

#include "rf.h"
#include "rf_impl.h"

namespace rf {

Dictionary::Dictionary()
    : m_app( "Diameter Base Accounting"),
    m_cmd_acr( "Accounting-Request" ),
    m_cmd_aca( "Accounting-Answer" ),
    m_vnd_3gpp2( "3GPP2" ),
    m_vnd_Etsi( "ETSI" ),
    m_vnd_3gpp( "3GPP" ),
    m_avp_lcs_client_dialed_by_ms( "LCS-Client-Dialed-By-MS", m_vnd_3gpp.getId() ),
    m_avp_mme_realm( "MME-Realm", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ggsn_mcc_mnc( "3GPP-GGSN-MCC-MNC", m_vnd_3gpp.getId() ),
    m_avp_event_timestamp( "Event-Timestamp" ),
    m_avp_service_identifier( "Service-Identifier" ),
    m_avp_csg_access_mode( "CSG-Access-Mode", m_vnd_3gpp.getId() ),
    m_avp_prose_range_class( "ProSe-Range-Class", m_vnd_3gpp.getId() ),
    m_avp_ims_emergency_indicator( "IMS-Emergency-Indicator", m_vnd_3gpp.getId() ),
    m_avp_originator_sccp_address( "Originator-SCCP-Address", m_vnd_3gpp.getId() ),
    m_avp_scale_factor( "Scale-Factor", m_vnd_3gpp.getId() ),
    m_avp_cn_operator_selection_entity( "CN-Operator-Selection-Entity", m_vnd_3gpp.getId() ),
    m_avp_acct_multi_session_id( "Acct-Multi-Session-Id" ),
    m_avp_inter_ue_transfer( "Inter-UE-Transfer", m_vnd_3gpp.getId() ),
    m_avp_related_ims_charging_identifier( "Related-IMS-Charging-Identifier", m_vnd_3gpp.getId() ),
    m_avp_initial_ims_charging_identifier( "Initial-IMS-Charging-Identifier", m_vnd_3gpp.getId() ),
    m_avp_called_identity( "Called-Identity", m_vnd_3gpp.getId() ),
    m_avp_sdp_media_component( "SDP-Media-Component", m_vnd_3gpp.getId() ),
    m_avp_radio_parameter_set_values( "Radio-Parameter-Set-Values", m_vnd_3gpp.getId() ),
    m_avp_aoc_service_type( "AoC-Service-Type", m_vnd_3gpp.getId() ),
    m_avp_acct_application_id( "Acct-Application-Id" ),
    m_avp_domain_name( "Domain-Name", m_vnd_3gpp.getId() ),
    m_avp_route_header_received( "Route-Header-Received", m_vnd_3gpp.getId() ),
    m_avp_uwan_user_location_info( "UWAN-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_priority_level( "Priority-Level", m_vnd_3gpp.getId() ),
    m_avp_online_charging_flag( "Online-Charging-Flag", m_vnd_3gpp.getId() ),
    m_avp_aoc_service_obligatory_type( "AoC-Service-Obligatory-Type", m_vnd_3gpp.getId() ),
    m_avp_msc_address( "MSC-Address", m_vnd_3gpp.getId() ),
    m_avp_address_domain( "Address-Domain", m_vnd_3gpp.getId() ),
    m_avp_sgw_address( "SGW-Address", m_vnd_3gpp.getId() ),
    m_avp_from_address( "From-Address", m_vnd_3gpp.getId() ),
    m_avp_service_information( "Service-Information", m_vnd_3gpp.getId() ),
    m_avp_lcs_client_type( "LCS-Client-Type", m_vnd_3gpp.getId() ),
    m_avp_lcs_client_id( "LCS-Client-Id", m_vnd_3gpp.getId() ),
    m_avp_sdp_media_name( "SDP-Media-Name", m_vnd_3gpp.getId() ),
    m_avp_sm_sequence_number( "SM-Sequence-Number", m_vnd_3gpp.getId() ),
    m_avp_time_window( "Time-Window", m_vnd_3gpp.getId() ),
    m_avp_token_text( "Token-Text", m_vnd_3gpp.getId() ),
    m_avp_terminating_ioi( "Terminating-IOI", m_vnd_3gpp.getId() ),
    m_avp_ps_append_free_format_data( "PS-Append-Free-Format-Data", m_vnd_3gpp.getId() ),
    m_avp_sms_result( "SMS-Result", m_vnd_3gpp.getId() ),
    m_avp_guaranteed_bitrate_dl( "Guaranteed-Bitrate-DL", m_vnd_3gpp.getId() ),
    m_avp_subscription_id_data( "Subscription-Id-Data" ),
    m_avp_location_estimate_type( "Location-Estimate-Type", m_vnd_3gpp.getId() ),
    m_avp_adc_rule_base_name( "ADC-Rule-Base-Name", m_vnd_3gpp.getId() ),
    m_avp_location_info( "Location-Info", m_vnd_3gpp.getId() ),
    m_avp_mbms_information( "MBMS-Information", m_vnd_3gpp.getId() ),
    m_avp_aoc_service( "AoC-Service", m_vnd_3gpp.getId() ),
    m_avp_csg_id( "CSG-Id", m_vnd_3gpp.getId() ),
    m_avp_location_estimate( "Location-Estimate", m_vnd_3gpp.getId() ),
    m_avp_mbms_charged_party( "MBMS-Charged-Party", m_vnd_3gpp.getId() ),
    m_avp_prose_reason_for_cancellation( "ProSe-Reason-For-Cancellation", m_vnd_3gpp.getId() ),
    m_avp_pdp_address_prefix_length( "PDP-Address-Prefix-Length", m_vnd_3gpp.getId() ),
    m_avp_epdg_address( "EPDG-Address", m_vnd_3gpp.getId() ),
    m_avp_transit_ioi_list( "Transit-IOI-List", m_vnd_3gpp.getId() ),
    m_avp_access_network_charging_identifier_value( "Access-Network-Charging-Identifier-Value", m_vnd_3gpp.getId() ),
    m_avp_cg_address( "CG-Address", m_vnd_3gpp.getId() ),
    m_avp_prose_direct_communication_transmission_data_container( "ProSe-Direct-Communication-Transmission-Data-Container", m_vnd_3gpp.getId() ),
    m_avp_service_specific_data( "Service-Specific-Data", m_vnd_3gpp.getId() ),
    m_avp_sip_method( "SIP-Method", m_vnd_3gpp.getId() ),
    m_avp_csg_membership_indication( "CSG-Membership-Indication", m_vnd_3gpp.getId() ),
    m_avp_time_last_usage( "Time-Last-Usage", m_vnd_3gpp.getId() ),
    m_avp_accounting_output_octets( "Accounting-Output-Octets" ),
    m_avp_sdp_session_description( "SDP-Session-Description", m_vnd_3gpp.getId() ),
    m_avp_session_direction( "Session-Direction", m_vnd_3gpp.getId() ),
    m_avp_apn_rate_control_downlink( "APN-Rate-Control-Downlink", m_vnd_3gpp.getId() ),
    m_avp_cc_output_octets( "CC-Output-Octets" ),
    m_avp_cn_ip_multicast_distribution( "CN-IP-Multicast-Distribution", m_vnd_3gpp.getId() ),
    m_avp_sm_status( "SM-Status", m_vnd_3gpp.getId() ),
    m_avp_prose_source_ip_address( "ProSe-Source-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_prose_direct_discovery_model( "ProSe-Direct-Discovery-Model", m_vnd_3gpp.getId() ),
    m_avp_rate_control_max_message_size( "Rate-Control-Max-Message-Size", m_vnd_3gpp.getId() ),
    m_avp_base_time_interval( "Base-Time-Interval", m_vnd_3gpp.getId() ),
    m_avp_poc_information( "PoC-Information", m_vnd_3gpp.getId() ),
    m_avp_required_mbms_bearer_capabilities( "Required-MBMS-Bearer-Capabilities", m_vnd_3gpp.getId() ),
    m_avp_3gpp_selection_mode( "3GPP-Selection-Mode", m_vnd_3gpp.getId() ),
    m_avp_supplementary_service( "Supplementary-Service", m_vnd_3gpp.getId() ),
    m_avp_pc3_control_protocol_cause( "PC3-Control-Protocol-Cause", m_vnd_3gpp.getId() ),
    m_avp_terminal_information( "Terminal-Information", m_vnd_3gpp.getId() ),
    m_avp_pdp_address( "PDP-Address", m_vnd_3gpp.getId() ),
    m_avp_charging_characteristics_selection_mode( "Charging-Characteristics-Selection-Mode", m_vnd_3gpp.getId() ),
    m_avp_transmitter_info( "Transmitter-Info", m_vnd_3gpp.getId() ),
    m_avp_time_usage( "Time-Usage", m_vnd_3gpp.getId() ),
    m_avp_content_class( "Content-Class", m_vnd_3gpp.getId() ),
    m_avp_applic_id( "Applic-ID", m_vnd_3gpp.getId() ),
    m_avp_3gpp2_bsid( "3GPP2-BSID", m_vnd_3gpp2.getId() ),
    m_avp_aoc_format( "AoC-Format", m_vnd_3gpp.getId() ),
    m_avp_udp_source_port( "UDP-Source-Port", m_vnd_3gpp.getId() ),
    m_avp_adaptations( "Adaptations", m_vnd_3gpp.getId() ),
    m_avp_positioning_data( "Positioning-Data", m_vnd_3gpp.getId() ),
    m_avp_error_message( "Error-Message" ),
    m_avp_imsi_unauthenticated_flag( "IMSI-Unauthenticated-Flag", m_vnd_3gpp.getId() ),
    m_avp_mandatory_capability( "Mandatory-Capability", m_vnd_3gpp.getId() ),
    m_avp_recipient_sccp_address( "Recipient-SCCP-Address", m_vnd_3gpp.getId() ),
    m_avp_expires( "Expires", m_vnd_3gpp.getId() ),
    m_avp_failed_avp( "Failed-AVP" ),
    m_avp_flows( "Flows", m_vnd_3gpp.getId() ),
    m_avp_requested_plmn_identifier( "Requested-PLMN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_mbms_user_service_type( "MBMS-User-Service-Type", m_vnd_3gpp.getId() ),
    m_avp_application_server( "Application-Server", m_vnd_3gpp.getId() ),
    m_avp_cc_input_octets( "CC-Input-Octets" ),
    m_avp_carrier_select_routing_information( "Carrier-Select-Routing-Information", m_vnd_3gpp.getId() ),
    m_avp_time_stamps( "Time-Stamps", m_vnd_3gpp.getId() ),
    m_avp_start_time( "Start-Time", m_vnd_3gpp.getId() ),
    m_avp_af_charging_identifier( "AF-Charging-Identifier", m_vnd_3gpp.getId() ),
    m_avp_service_data_container( "Service-Data-Container", m_vnd_3gpp.getId() ),
    m_avp_early_media_description( "Early-Media-Description", m_vnd_3gpp.getId() ),
    m_avp_granted_service_unit( "Granted-Service-Unit" ),
    m_avp_max_requested_bandwidth_ul( "Max-Requested-Bandwidth-UL", m_vnd_3gpp.getId() ),
    m_avp_imei( "IMEI", m_vnd_3gpp.getId() ),
    m_avp_mms_information( "MMS-Information", m_vnd_3gpp.getId() ),
    m_avp_user_name( "User-Name" ),
    m_avp_bssid( "BSSID", m_vnd_3gpp.getId() ),
    m_avp_low_priority_indicator( "Low-Priority-Indicator", m_vnd_3gpp.getId() ),
    m_avp_3gpp_pdp_type( "3GPP-PDP-Type", m_vnd_3gpp.getId() ),
    m_avp_change_condition( "Change-Condition", m_vnd_3gpp.getId() ),
    m_avp_data_coding_scheme( "Data-Coding-Scheme", m_vnd_3gpp.getId() ),
    m_avp_pre_emption_capability( "Pre-emption-Capability", m_vnd_3gpp.getId() ),
    m_avp_nni_information( "NNI-Information", m_vnd_3gpp.getId() ),
    m_avp_3gpp_charging_id( "3GPP-Charging-Id", m_vnd_3gpp.getId() ),
    m_avp_usage_information_report_sequence_number( "Usage-Information-Report-Sequence-Number", m_vnd_3gpp.getId() ),
    m_avp_sponsor_identity( "Sponsor-Identity", m_vnd_3gpp.getId() ),
    m_avp_3gpp_rat_type( "3GPP-RAT-Type", m_vnd_3gpp.getId() ),
    m_avp_requestor_plmn_identifier( "Requestor-PLMN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_pre_emption_vulnerability( "Pre-emption-Vulnerability", m_vnd_3gpp.getId() ),
    m_avp_aoc_subscription_information( "AoC-Subscription-Information", m_vnd_3gpp.getId() ),
    m_avp_reason_header( "Reason-Header", m_vnd_3gpp.getId() ),
    m_avp_external_identifier( "External-Identifier", m_vnd_3gpp.getId() ),
    m_avp_rai( "RAI", m_vnd_3gpp.getId() ),
    m_avp_interface_port( "Interface-Port", m_vnd_3gpp.getId() ),
    m_avp_current_tariff( "Current-Tariff", m_vnd_3gpp.getId() ),
    m_avp_ps_information( "PS-Information", m_vnd_3gpp.getId() ),
    m_avp_final_unit_action( "Final-Unit-Action" ),
    m_avp_currency_code( "Currency-Code" ),
    m_avp_presence_reporting_area_status( "Presence-Reporting-Area-Status", m_vnd_3gpp.getId() ),
    m_avp_monitoring_ue_identifier( "Monitoring-UE-Identifier", m_vnd_3gpp.getId() ),
    m_avp_downlink_rate_limit( "Downlink-Rate-Limit", m_vnd_3gpp.getId() ),
    m_avp_accounting_record_number( "Accounting-Record-Number" ),
    m_avp_address_type( "Address-Type", m_vnd_3gpp.getId() ),
    m_avp_destination_host( "Destination-Host" ),
    m_avp_content_type( "Content-Type", m_vnd_3gpp.getId() ),
    m_avp_event( "Event", m_vnd_3gpp.getId() ),
    m_avp_rrc_counter_timestamp( "RRC-Counter-Timestamp", m_vnd_3gpp.getId() ),
    m_avp_guaranteed_bitrate_ul( "Guaranteed-Bitrate-UL", m_vnd_3gpp.getId() ),
    m_avp_acct_session_id( "Acct-Session-Id" ),
    m_avp_additional_content_information( "Additional-Content-Information", m_vnd_3gpp.getId() ),
    m_avp_mme_name( "MME-Name", m_vnd_3gpp.getId() ),
    m_avp_media_initiator_flag( "Media-Initiator-Flag", m_vnd_3gpp.getId() ),
    m_avp_user_equipment_info_type( "User-Equipment-Info-Type" ),
    m_avp_monitoring_ue_hplmn_identifier( "Monitoring-UE-HPLMN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_announcing_ue_hplmn_identifier( "Announcing-UE-HPLMN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_3gpp_charging_characteristics( "3GPP-Charging-Characteristics", m_vnd_3gpp.getId() ),
    m_avp_proxy_info( "Proxy-Info" ),
    m_avp_origin_app_layer_user_id( "Origin-App-Layer-User-Id", m_vnd_3gpp.getId() ),
    m_avp_interface_id( "Interface-Id", m_vnd_3gpp.getId() ),
    m_avp_apn_rate_control_uplink( "APN-Rate-Control-Uplink", m_vnd_3gpp.getId() ),
    m_avp_apn_rate_control( "APN-Rate-Control", m_vnd_3gpp.getId() ),
    m_avp_cause_code( "Cause-Code", m_vnd_3gpp.getId() ),
    m_avp_route_header_transmitted( "Route-Header-Transmitted", m_vnd_3gpp.getId() ),
    m_avp_type_number( "Type-Number", m_vnd_3gpp.getId() ),
    m_avp_sms_information( "SMS-Information", m_vnd_3gpp.getId() ),
    m_avp_sm_protocol_id( "SM-Protocol-ID", m_vnd_3gpp.getId() ),
    m_avp_charging_rule_base_name( "Charging-Rule-Base-Name", m_vnd_3gpp.getId() ),
    m_avp_time_first_reception( "Time-First-Reception", m_vnd_3gpp.getId() ),
    m_avp_user_equipment_info_value( "User-Equipment-Info-Value" ),
    m_avp_route_record( "Route-Record" ),
    m_avp_class_identifier( "Class-Identifier", m_vnd_3gpp.getId() ),
    m_avp_event_type( "Event-Type", m_vnd_3gpp.getId() ),
    m_avp_calling_party_address( "Calling-Party-Address", m_vnd_3gpp.getId() ),
    m_avp_prose_direct_communication_reception_data_container( "ProSe-Direct-Communication-Reception-Data-Container", m_vnd_3gpp.getId() ),
    m_avp_exponent( "Exponent" ),
    m_avp_originator_interface( "Originator-Interface", m_vnd_3gpp.getId() ),
    m_avp_error_reporting_host( "Error-Reporting-Host" ),
    m_avp_nbifom_support( "NBIFOM-Support", m_vnd_3gpp.getId() ),
    m_avp_multiple_services_credit_control( "Multiple-Services-Credit-Control" ),
    m_avp_message_type( "Message-Type", m_vnd_3gpp.getId() ),
    m_avp_pdp_context_type( "PDP-Context-Type", m_vnd_3gpp.getId() ),
    m_avp_application_service_provider_identity( "Application-Service-Provider-Identity", m_vnd_3gpp.getId() ),
    m_avp_proximity_alert_indication( "Proximity-Alert-Indication", m_vnd_3gpp.getId() ),
    m_avp_lcs_requestor_id_string( "LCS-Requestor-ID-String", m_vnd_3gpp.getId() ),
    m_avp_service_context_id( "Service-Context-Id" ),
    m_avp_monitoring_ue_vplmn_identifier( "Monitoring-UE-VPLMN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_max_requested_bandwidth_dl( "Max-Requested-Bandwidth-DL", m_vnd_3gpp.getId() ),
    m_avp_origin_state_id( "Origin-State-Id" ),
    m_avp_ims_application_reference_identifier( "IMS-Application-Reference-Identifier", m_vnd_3gpp.getId() ),
    m_avp_session_id( "Session-Id" ),
    m_avp_bearer_service( "Bearer-Service", m_vnd_3gpp.getId() ),
    m_avp_3gpp_user_location_info( "3GPP-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_interface_type( "Interface-Type", m_vnd_3gpp.getId() ),
    m_avp_destination_interface( "Destination-Interface", m_vnd_3gpp.getId() ),
    m_avp_access_transfer_information( "Access-Transfer-Information", m_vnd_3gpp.getId() ),
    m_avp_accounting_record_type( "Accounting-Record-Type" ),
    m_avp_auth_application_id( "Auth-Application-Id" ),
    m_avp_lcs_apn( "LCS-APN", m_vnd_3gpp.getId() ),
    m_avp_tad_identifier( "TAD-Identifier", m_vnd_3gpp.getId() ),
    m_avp_recipient_received_address( "Recipient-Received-Address", m_vnd_3gpp.getId() ),
    m_avp_dynamic_address_flag( "Dynamic-Address-Flag", m_vnd_3gpp.getId() ),
    m_avp_local_gw_inserted_indication( "Local-GW-Inserted-Indication", m_vnd_3gpp.getId() ),
    m_avp_prose_group_ip_multicast_address( "ProSe-Group-IP-Multicast-Address", m_vnd_3gpp.getId() ),
    m_avp_sm_device_trigger_indicator( "SM-Device-Trigger-Indicator", m_vnd_3gpp.getId() ),
    m_avp_ims_communication_service_identifier( "IMS-Communication-Service-Identifier", m_vnd_3gpp.getId() ),
    m_avp_lcs_requestor_id( "LCS-Requestor-ID", m_vnd_3gpp.getId() ),
    m_avp_visited_plmn_id( "Visited-PLMN-Id", m_vnd_3gpp.getId() ),
    m_avp_mtc_iwf_address( "MTC-IWF-Address", m_vnd_3gpp.getId() ),
    m_avp_cc_service_specific_units( "CC-Service-Specific-Units" ),
    m_avp_proxy_state( "Proxy-State" ),
    m_avp_submission_time( "Submission-Time", m_vnd_3gpp.getId() ),
    m_avp_sm_message_type( "SM-Message-Type", m_vnd_3gpp.getId() ),
    m_avp_associated_uri( "Associated-URI", m_vnd_3gpp.getId() ),
    m_avp_sms_node( "SMS-Node", m_vnd_3gpp.getId() ),
    m_avp_local_sequence_number( "Local-Sequence-Number", m_vnd_3gpp.getId() ),
    m_avp_ps_furnish_charging_information( "PS-Furnish-Charging-Information", m_vnd_3gpp.getId() ),
    m_avp_tdf_ip_address( "TDF-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_time_first_transmission( "Time-First-Transmission", m_vnd_3gpp.getId() ),
    m_avp_user_csg_information( "User-CSG-Information", m_vnd_3gpp.getId() ),
    m_avp_called_identity_change( "Called-Identity-Change", m_vnd_3gpp.getId() ),
    m_avp_target_app_layer_user_id( "Target-App-Layer-User-Id", m_vnd_3gpp.getId() ),
    m_avp_vendor_specific_application_id( "Vendor-Specific-Application-Id" ),
    m_avp_dynamic_address_flag_extension( "Dynamic-Address-Flag-Extension", m_vnd_3gpp.getId() ),
    m_avp_service_specific_info( "Service-Specific-Info", m_vnd_3gpp.getId() ),
    m_avp_addressee_type( "Addressee-Type", m_vnd_3gpp.getId() ),
    m_avp_service_id( "Service-Id", m_vnd_3gpp.getId() ),
    m_avp_sdp_media_description( "SDP-Media-Description", m_vnd_3gpp.getId() ),
    m_avp_number_portability_routing_information( "Number-Portability-Routing-Information", m_vnd_3gpp.getId() ),
    m_avp_accumulated_cost( "Accumulated-Cost", m_vnd_3gpp.getId() ),
    m_avp_layer_2_group_id( "Layer-2-Group-ID", m_vnd_3gpp.getId() ),
    m_avp_mbms_data_transfer_stop( "MBMS-Data-Transfer-Stop", m_vnd_3gpp.getId() ),
    m_avp_additional_type_information( "Additional-Type-Information", m_vnd_3gpp.getId() ),
    m_avp_mbms_session_identity( "MBMS-Session-Identity", m_vnd_3gpp.getId() ),
    m_avp_node_id( "Node-Id", m_vnd_3gpp.getId() ),
    m_avp_origin_host( "Origin-Host" ),
    m_avp_qos_information( "QoS-Information", m_vnd_3gpp.getId() ),
    m_avp_charging_per_ip_can_session_indicator( "Charging-Per-IP-CAN-Session-Indicator", m_vnd_3gpp.getId() ),
    m_avp_role_of_node( "Role-Of-Node", m_vnd_3gpp.getId() ),
    m_avp_rate_element( "Rate-Element", m_vnd_3gpp.getId() ),
    m_avp_aux_applic_info( "Aux-Applic-Info", m_vnd_3gpp.getId() ),
    m_avp_rate_control_max_rate( "Rate-Control-Max-Rate", m_vnd_3gpp.getId() ),
    m_avp_real_time_tariff_information( "Real-Time-Tariff-Information", m_vnd_3gpp.getId() ),
    m_avp_scs_as_address( "SCS-AS-Address", m_vnd_3gpp.getId() ),
    m_avp_qos_class_identifier( "QoS-Class-Identifier", m_vnd_3gpp.getId() ),
    m_avp_message_class( "Message-Class", m_vnd_3gpp.getId() ),
    m_avp_access_transfer_type( "Access-Transfer-Type", m_vnd_3gpp.getId() ),
    m_avp_tmgi( "TMGI", m_vnd_3gpp.getId() ),
    m_avp_mbms_2g_3g_indicator( "MBMS-2G-3G-Indicator", m_vnd_3gpp.getId() ),
    m_avp_mbms_data_transfer_start( "MBMS-Data-Transfer-Start", m_vnd_3gpp.getId() ),
    m_avp_location_type( "Location-Type", m_vnd_3gpp.getId() ),
    m_avp_address_data( "Address-Data", m_vnd_3gpp.getId() ),
    m_avp_tariff_xml( "Tariff-XML", m_vnd_3gpp.getId() ),
    m_avp_wlan_link_layer_id( "WLAN-Link-Layer-Id", m_vnd_3gpp.getId() ),
    m_avp_msisdn( "MSISDN", m_vnd_3gpp.getId() ),
    m_avp_radio_resources_indicator( "Radio-Resources-Indicator", m_vnd_3gpp.getId() ),
    m_avp_aoc_cost_information( "AoC-Cost-Information", m_vnd_3gpp.getId() ),
    m_avp_priority( "Priority", m_vnd_3gpp.getId() ),
    m_avp_content_version( "Content-Version", m_vnd_3gpp.getId() ),
    m_avp_drm_content( "DRM-Content", m_vnd_3gpp.getId() ),
    m_avp_lcs_format_indicator( "LCS-Format-Indicator", m_vnd_3gpp.getId() ),
    m_avp_poc_server_role( "PoC-Server-Role", m_vnd_3gpp.getId() ),
    m_avp_mmtel_information( "MMTel-Information", m_vnd_3gpp.getId() ),
    m_avp_read_reply_report_requested( "Read-Reply-Report-Requested", m_vnd_3gpp.getId() ),
    m_avp_recipient_info( "Recipient-Info", m_vnd_3gpp.getId() ),
    m_avp_lcs_client_external_id( "LCS-Client-External-ID", m_vnd_3gpp.getId() ),
    m_avp_origin_realm( "Origin-Realm" ),
    m_avp_ims_charging_identifier( "IMS-Charging-Identifier", m_vnd_3gpp.getId() ),
    m_avp_mbms_service_type( "MBMS-Service-Type", m_vnd_3gpp.getId() ),
    m_avp_prose_ue_id( "ProSe-UE-ID", m_vnd_3gpp.getId() ),
    m_avp_proximity_alert_timestamp( "Proximity-Alert-Timestamp", m_vnd_3gpp.getId() ),
    m_avp_3gpp_nsapi( "3GPP-NSAPI", m_vnd_3gpp.getId() ),
    m_avp_serving_plmn_rate_control( "Serving-PLMN-Rate-Control", m_vnd_3gpp.getId() ),
    m_avp_enhanced_diagnostics( "Enhanced-Diagnostics", m_vnd_3gpp.getId() ),
    m_avp_accounting_sub_session_id( "Accounting-Sub-Session-Id" ),
    m_avp_presence_reporting_area_identifier( "Presence-Reporting-Area-Identifier", m_vnd_3gpp.getId() ),
    m_avp_originator_address( "Originator-Address", m_vnd_3gpp.getId() ),
    m_avp_scef_id( "SCEF-ID", m_vnd_3gpp.getId() ),
    m_avp_presence_reporting_area_information( "Presence-Reporting-Area-Information", m_vnd_3gpp.getId() ),
    m_avp_ssid( "SSID", m_vnd_3gpp.getId() ),
    m_avp_originator_received_address( "Originator-Received-Address", m_vnd_3gpp.getId() ),
    m_avp_message_size( "Message-Size", m_vnd_3gpp.getId() ),
    m_avp_radio_frequency( "Radio-Frequency", m_vnd_3gpp.getId() ),
    m_avp_apn_aggregate_max_bitrate_dl( "APN-Aggregate-Max-Bitrate-DL", m_vnd_3gpp.getId() ),
    m_avp_rrc_cause_counter( "RRC-Cause-Counter", m_vnd_3gpp.getId() ),
    m_avp_prose_3rd_party_application_id( "ProSe-3rd-Party-Application-ID", m_vnd_3gpp.getId() ),
    m_avp_accounting_realtime_required( "Accounting-Realtime-Required" ),
    m_avp_bearer_identifier( "Bearer-Identifier", m_vnd_3gpp.getId() ),
    m_avp_3gpp_sgsn_mcc_mnc( "3GPP-SGSN-MCC-MNC", m_vnd_3gpp.getId() ),
    m_avp_application_specific_data( "Application-Specific-Data", m_vnd_3gpp.getId() ),
    m_avp_ps_free_format_data( "PS-Free-Format-Data", m_vnd_3gpp.getId() ),
    m_avp_rating_group( "Rating-Group" ),
    m_avp_outgoing_session_id( "Outgoing-Session-Id", m_vnd_3gpp.getId() ),
    m_avp_message_id( "Message-ID", m_vnd_3gpp.getId() ),
    m_avp_allocation_retention_priority( "Allocation-Retention-Priority", m_vnd_3gpp.getId() ),
    m_avp_instance_id( "Instance-Id", m_vnd_3gpp.getId() ),
    m_avp_called_asserted_identity( "Called-Asserted-Identity", m_vnd_3gpp.getId() ),
    m_avp_access_network_info_change( "Access-Network-Info-Change", m_vnd_3gpp.getId() ),
    m_avp_result_code( "Result-Code" ),
    m_avp_acct_interim_interval( "Acct-Interim-Interval" ),
    m_avp_account_expiration( "Account-Expiration", m_vnd_3gpp.getId() ),
    m_avp_mbms_service_area( "MBMS-Service-Area", m_vnd_3gpp.getId() ),
    m_avp_mme_number_for_mt_sms( "MME-Number-for-MT-SMS", m_vnd_3gpp.getId() ),
    m_avp_sip_request_timestamp( "SIP-Request-Timestamp", m_vnd_3gpp.getId() ),
    m_avp_prose_validity_timer( "ProSe-Validity-Timer", m_vnd_3gpp.getId() ),
    m_avp_incoming_trunk_group_id( "Incoming-Trunk-Group-Id", m_vnd_3gpp.getId() ),
    m_avp_traffic_data_volumes( "Traffic-Data-Volumes", m_vnd_3gpp.getId() ),
    m_avp_fixed_user_location_info( "Fixed-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_sdp_offer_timestamp( "SDP-Offer-Timestamp", m_vnd_3gpp.getId() ),
    m_avp_lcs_client_name( "LCS-Client-Name", m_vnd_3gpp.getId() ),
    m_avp_interface_text( "Interface-Text", m_vnd_3gpp.getId() ),
    m_avp_called_station_id( "Called-Station-Id" ),
    m_avp_related_change_condition_information( "Related-Change-Condition-Information", m_vnd_3gpp.getId() ),
    m_avp_ip_realm_default_indication( "IP-Realm-Default-Indication", m_vnd_3gpp.getId() ),
    m_avp_ims_visited_network_identifier( "IMS-Visited-Network-Identifier", m_vnd_3gpp.getId() ),
    m_avp_client_address( "Client-Address", m_vnd_3gpp.getId() ),
    m_avp_message_body( "Message-Body", m_vnd_3gpp.getId() ),
    m_avp_lcs_information( "LCS-Information", m_vnd_3gpp.getId() ),
    m_avp_time_quota_mechanism( "Time-Quota-Mechanism", m_vnd_3gpp.getId() ),
    m_avp_aoc_information( "AoC-Information", m_vnd_3gpp.getId() ),
    m_avp_tariff_information( "Tariff-Information", m_vnd_3gpp.getId() ),
    m_avp_subscription_id( "Subscription-Id" ),
    m_avp_server_capabilities( "Server-Capabilities", m_vnd_3gpp.getId() ),
    m_avp_radio_parameter_set_info( "Radio-Parameter-Set-Info", m_vnd_3gpp.getId() ),
    m_avp_media_component_number( "Media-Component-Number", m_vnd_3gpp.getId() ),
    m_avp_serving_node_type( "Serving-Node-Type", m_vnd_3gpp.getId() ),
    m_avp_mbms_gw_address( "MBMS-GW-Address", m_vnd_3gpp.getId() ),
    m_avp_accounting_input_octets( "Accounting-Input-Octets" ),
    m_avp_scs_realm( "SCS-Realm", m_vnd_3gpp.getId() ),
    m_avp_media_initiator_party( "Media-Initiator-Party", m_vnd_3gpp.getId() ),
    m_avp_prose_role_of_ue( "ProSe-Role-Of-UE", m_vnd_3gpp.getId() ),
    m_avp_stop_time( "Stop-Time", m_vnd_3gpp.getId() ),
    m_avp_diagnostics( "Diagnostics", m_vnd_3gpp.getId() ),
    m_avp_reference_number( "Reference-Number", m_vnd_3gpp.getId() ),
    m_avp_related_ims_charging_identifier_node( "Related-IMS-Charging-Identifier-Node", m_vnd_3gpp.getId() ),
    m_avp_proximity_cancellation_timestamp( "Proximity-Cancellation-Timestamp", m_vnd_3gpp.getId() ),
    m_avp_mmbox_storage_requested( "MMBox-Storage-Requested", m_vnd_3gpp.getId() ),
    m_avp_af_correlation_information( "AF-Correlation-Information", m_vnd_3gpp.getId() ),
    m_avp_cc_total_octets( "CC-Total-Octets" ),
    m_avp_preferred_aoc_currency( "Preferred-AoC-Currency", m_vnd_3gpp.getId() ),
    m_avp_subscription_id_type( "Subscription-Id-Type" ),
    m_avp_pc3_epc_control_protocol_cause( "PC3-EPC-Control-Protocol-Cause", m_vnd_3gpp.getId() ),
    m_avp_prose_information( "ProSe-Information", m_vnd_3gpp.getId() ),
    m_avp_physical_access_id( "Physical-Access-Id", m_vnd_Etsi.getId() ),
    m_avp_transcoder_inserted_indication( "Transcoder-Inserted-Indication", m_vnd_3gpp.getId() ),
    m_avp_cp_ciot_eps_optimisation_indicator( "CP-CIoT-EPS-Optimisation-Indicator", m_vnd_3gpp.getId() ),
    m_avp_server_name( "Server-Name", m_vnd_3gpp.getId() ),
    m_avp_apn_aggregate_max_bitrate_ul( "APN-Aggregate-Max-Bitrate-UL", m_vnd_3gpp.getId() ),
    m_avp_sm_device_trigger_information( "SM-Device-Trigger-Information", m_vnd_3gpp.getId() ),
    m_avp_sdp_answer_timestamp( "SDP-Answer-Timestamp", m_vnd_3gpp.getId() ),
    m_avp_alternate_charged_party_address( "Alternate-Charged-Party-Address", m_vnd_3gpp.getId() ),
    m_avp_prose_app_id( "ProSe-App-Id", m_vnd_3gpp.getId() ),
    m_avp_twan_user_location_info( "TWAN-User-Location-Info", m_vnd_3gpp.getId() ),
    m_avp_cc_money( "CC-Money" ),
    m_avp_logical_access_id( "Logical-Access-Id", m_vnd_Etsi.getId() ),
    m_avp_cellular_network_information( "Cellular-Network-Information", m_vnd_3gpp.getId() ),
    m_avp_offline_charging( "Offline-Charging", m_vnd_3gpp.getId() ),
    m_avp_sm_discharge_time( "SM-Discharge-Time", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ps_data_off_status( "3GPP-PS-Data-Off-Status", m_vnd_3gpp.getId() ),
    m_avp_prose_function_id( "ProSe-Function-ID", m_vnd_3gpp.getId() ),
    m_avp_content_size( "Content-Size", m_vnd_3gpp.getId() ),
    m_avp_uni_pdu_cp_only_flag( "UNI-PDU-CP-Only-Flag", m_vnd_3gpp.getId()),
    m_avp_nni_type( "NNI-Type", m_vnd_3gpp.getId() ),
    m_avp_time_quota_type( "Time-Quota-Type", m_vnd_3gpp.getId() ),
    m_avp_ue_local_ip_address( "UE-Local-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_media_component_status( "Media-Component-Status", m_vnd_3gpp.getId() ),
    m_avp_ggsn_address( "GGSN-Address", m_vnd_3gpp.getId() ),
    m_avp_neighbour_node_address( "Neighbour-Node-Address", m_vnd_3gpp.getId() ),
    m_avp_3gpp_imsi_mcc_mnc( "3GPP-IMSI-MCC-MNC", m_vnd_3gpp.getId() ),
    m_avp_value_digits( "Value-Digits" ),
    m_avp_file_repair_supported( "File-Repair-Supported", m_vnd_3gpp.getId() ),
    m_avp_change_time( "Change-Time", m_vnd_3gpp.getId() ),
    m_avp_node_functionality( "Node-Functionality", m_vnd_3gpp.getId() ),
    m_avp_session_priority( "Session-Priority", m_vnd_3gpp.getId() ),
    m_avp_recipient_address( "Recipient-Address", m_vnd_3gpp.getId() ),
    m_avp_lcs_name_string( "LCS-Name-String", m_vnd_3gpp.getId() ),
    m_avp_counter_value( "Counter-Value", m_vnd_3gpp.getId() ),
    m_avp_trunk_group_id( "Trunk-Group-ID", m_vnd_3gpp.getId() ),
    m_avp_nbifom_mode( "NBIFOM-Mode", m_vnd_3gpp.getId() ),
    m_avp_incremental_cost( "Incremental-Cost", m_vnd_3gpp.getId() ),
    m_avp_twag_address( "TWAG-Address", m_vnd_3gpp.getId() ),
    m_avp_sdp_timestamps( "SDP-TimeStamps", m_vnd_3gpp.getId() ),
    m_avp_presence_reporting_area_elements_list( "Presence-Reporting-Area-Elements-List", m_vnd_3gpp.getId() ),
    m_avp_requested_party_address( "Requested-Party-Address", m_vnd_3gpp.getId() ),
    m_avp_sgi_ptp_tunnelling_method( "SGi-PtP-Tunnelling-Method", m_vnd_3gpp.getId() ),
    m_avp_relationship_mode( "Relationship-Mode", m_vnd_3gpp.getId() ),
    m_avp_conditional_apn_aggregate_max_bitrate( "Conditional-APN-Aggregate-Max-Bitrate", m_vnd_3gpp.getId() ),
    m_avp_cc_unit_type( "CC-Unit-Type" ),
    m_avp_inter_operator_identifier( "Inter-Operator-Identifier", m_vnd_3gpp.getId() ),
    m_avp_coverage_status( "Coverage-Status", m_vnd_3gpp.getId() ),
    m_avp_originating_ioi( "Originating-IOI", m_vnd_3gpp.getId() ),
    m_avp_access_network_information( "Access-Network-Information", m_vnd_3gpp.getId() ),
    m_avp_next_tariff( "Next-Tariff", m_vnd_3gpp.getId() ),
    m_avp_3gpp_imsi( "3GPP-IMSI", m_vnd_3gpp.getId() ),
    m_avp_coverage_info( "Coverage-Info", m_vnd_3gpp.getId() ),
    m_avp_lcs_data_coding_scheme( "LCS-Data-Coding-Scheme", m_vnd_3gpp.getId() ),
    m_avp_requesting_epuid( "Requesting-EPUID", m_vnd_3gpp.getId() ),
    m_avp_flow_number( "Flow-Number", m_vnd_3gpp.getId() ),
    m_avp_rat_type( "RAT-Type", m_vnd_3gpp.getId() ),
    m_avp_ims_information( "IMS-Information", m_vnd_3gpp.getId() ),
    m_avp_mmtel_sservice_type( "MMTel-SService-Type", m_vnd_3gpp.getId() ),
    m_avp_network_call_reference_number( "Network-Call-Reference-Number", m_vnd_3gpp.getId() ),
    m_avp_reply_applic_id( "Reply-Applic-ID", m_vnd_3gpp.getId() ),
    m_avp_delivery_report_requested( "Delivery-Report-Requested", m_vnd_3gpp.getId() ),
    m_avp_smsc_address( "SMSC-Address", m_vnd_3gpp.getId() ),
    m_avp_prose_request_timestamp( "ProSe-Request-Timestamp", m_vnd_3gpp.getId() ),
    m_avp_user_equipment_info( "User-Equipment-Info" ),
    m_avp_additional_exception_reports( "Additional-Exception-Reports", m_vnd_3gpp.getId() ),
    m_avp_scs_address( "SCS-Address", m_vnd_3gpp.getId() ),
    m_avp_quota_consumption_time( "Quota-Consumption-Time", m_vnd_3gpp.getId() ),
    m_avp_ran_nas_release_cause( "RAN-NAS-Release-Cause", m_vnd_3gpp.getId() ),
    m_avp_user_session_id( "User-Session-Id", m_vnd_3gpp.getId() ),
    m_avp_mm_content_type( "MM-Content-Type", m_vnd_3gpp.getId() ),
    m_avp_service_mode( "Service-Mode", m_vnd_3gpp.getId() ),
    m_avp_sm_user_data_header( "SM-User-Data-Header", m_vnd_3gpp.getId() ),
    m_avp_pdn_connection_charging_id( "PDN-Connection-Charging-ID", m_vnd_3gpp.getId() ),
    m_avp_rate_control_time_unit( "Rate-Control-Time-Unit", m_vnd_3gpp.getId() ),
    m_avp_called_party_address( "Called-Party-Address", m_vnd_3gpp.getId() ),
    m_avp_sm_service_type( "SM-Service-Type", m_vnd_3gpp.getId() ),
    m_avp_bearer_capability( "Bearer-Capability", m_vnd_3gpp.getId() ),
    m_avp_time_first_usage( "Time-First-Usage", m_vnd_3gpp.getId() ),
    m_avp_deferred_location_event_type( "Deferred-Location-Event-Type", m_vnd_3gpp.getId() ),
    m_avp_sdp_type( "SDP-Type", m_vnd_3gpp.getId() ),
    m_avp_application_server_information( "Application-Server-Information", m_vnd_3gpp.getId() ),
    m_avp_proxy_host( "Proxy-Host" ),
    m_avp_user_location_info_time( "User-Location-Info-Time", m_vnd_3gpp.getId() ),
    m_avp_sgsn_address( "SGSN-Address", m_vnd_3gpp.getId() ),
    m_avp_announcing_ue_vplmn_identifier( "Announcing-UE-VPLMN-Identifier", m_vnd_3gpp.getId() ),
    m_avp_prose_event_type( "ProSe-Event-Type", m_vnd_3gpp.getId() ),
    m_avp_vcs_information( "VCS-Information", m_vnd_3gpp.getId() ),
    m_avp_cpdt_information( "CPDT-Information", m_vnd_3gpp.getId() ),
    m_avp_3gpp_ms_timezone( "3GPP-MS-TimeZone", m_vnd_3gpp.getId() ),
    m_avp_sgw_change( "SGW-Change", m_vnd_3gpp.getId() ),
    m_avp_ip_can_type( "IP-CAN-Type", m_vnd_3gpp.getId() ),
    m_avp_optional_capability( "Optional-Capability", m_vnd_3gpp.getId() ),
    m_avp_tariff_time_change( "Tariff-Time-Change" ),
    m_avp_cc_time( "CC-Time" ),
    m_avp_served_party_ip_address( "Served-Party-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_envelope_reporting( "Envelope-Reporting", m_vnd_3gpp.getId() ),
    m_avp_prose_function_ip_address( "ProSe-Function-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_reply_path_requested( "Reply-Path-Requested", m_vnd_3gpp.getId() ),
    m_avp_software_version( "Software-Version", m_vnd_3gpp.getId() ),
    m_avp_uplink_rate_limit( "Uplink-Rate-Limit", m_vnd_3gpp.getId() ),
    m_avp_vendor_id( "Vendor-Id" ),
    m_avp_unit_value( "Unit-Value" ),
    m_avp_destination_realm( "Destination-Realm" ),
    m_avp_number_of_messages_sent( "Number-Of-Messages-Sent", m_vnd_3gpp.getId() ),
    m_avp_role_of_prose_function( "Role-Of-ProSe-Function", m_vnd_3gpp.getId() )
{
   std::cout << "Registering rf dictionary" << std::endl;
};

Dictionary::~Dictionary()
{
};

ACRcmd::ACRcmd( Application & app )
   : FDCommandRequest( app.getDict().cmdACR() ),
     m_app( app )
{
}

ACRcmd::~ACRcmd()
{
}

ACRreq::ACRreq( Application &app )
   : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdACR() ),
     m_app(app)
{
}

ACRreq::~ACRreq()
{
}

 
ApplicationBase::ApplicationBase()
{
    setDictionaryEntry( &m_dict.app() );

};

ApplicationBase::~ApplicationBase()
{
};



ScaleFactorExtractor::ScaleFactorExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpScaleFactor() ),
    value_digits( *this, dict.avpValueDigits() ),
    exponent( *this, dict.avpExponent() )
{
    add( value_digits );
    add( exponent );
}

ScaleFactorExtractor::~ScaleFactorExtractor()
{
}

SdpMediaComponentExtractor::SdpMediaComponentExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSdpMediaComponent() ),
    sdp_media_name( *this, dict.avpSdpMediaName() ),
    sdp_media_description( *this, dict.avpSdpMediaDescription() ),
    local_gw_inserted_indication( *this, dict.avpLocalGwInsertedIndication() ),
    ip_realm_default_indication( *this, dict.avpIpRealmDefaultIndication() ),
    transcoder_inserted_indication( *this, dict.avpTranscoderInsertedIndication() ),
    media_initiator_flag( *this, dict.avpMediaInitiatorFlag() ),
    media_initiator_party( *this, dict.avpMediaInitiatorParty() ),
    tgpp_charging_id( *this, dict.avp3gppChargingId() ),
    access_network_charging_identifier_value( *this, dict.avpAccessNetworkChargingIdentifierValue() ),
    sdp_type( *this, dict.avpSdpType() )
{
    add( sdp_media_name );
    add( sdp_media_description );
    add( local_gw_inserted_indication );
    add( ip_realm_default_indication );
    add( transcoder_inserted_indication );
    add( media_initiator_flag );
    add( media_initiator_party );
    add( tgpp_charging_id );
    add( access_network_charging_identifier_value );
    add( sdp_type );
}

SdpMediaComponentExtractor::~SdpMediaComponentExtractor()
{
}

UwanUserLocationInfoExtractor::UwanUserLocationInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUwanUserLocationInfo() ),
    ue_local_ip_address( *this, dict.avpUeLocalIpAddress() ),
    udp_source_port( *this, dict.avpUdpSourcePort() ),
    ssid( *this, dict.avpSsid() ),
    bssid( *this, dict.avpBssid() )
{
    add( ue_local_ip_address );
    add( udp_source_port );
    add( ssid );
    add( bssid );
}

UwanUserLocationInfoExtractor::~UwanUserLocationInfoExtractor()
{
}

AddressDomainExtractor::AddressDomainExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAddressDomain() ),
    domain_name( *this, dict.avpDomainName() ),
    tgpp_imsi_mcc_mnc( *this, dict.avp3gppImsiMccMnc() )
{
    add( domain_name );
    add( tgpp_imsi_mcc_mnc );
}

AddressDomainExtractor::~AddressDomainExtractor()
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

SubscriptionIdExtractorList::SubscriptionIdExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpSubscriptionId() ),
    m_dict( dict )
{
}

SubscriptionIdExtractorList::~SubscriptionIdExtractorList()
{
}

AccumulatedCostExtractor::AccumulatedCostExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAccumulatedCost() ),
    value_digits( *this, dict.avpValueDigits() ),
    exponent( *this, dict.avpExponent() )
{
    add( value_digits );
    add( exponent );
}

AccumulatedCostExtractor::~AccumulatedCostExtractor()
{
}

IncrementalCostExtractor::IncrementalCostExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpIncrementalCost() ),
    value_digits( *this, dict.avpValueDigits() ),
    exponent( *this, dict.avpExponent() )
{
    add( value_digits );
    add( exponent );
}

IncrementalCostExtractor::~IncrementalCostExtractor()
{
}

IncrementalCostExtractorList::IncrementalCostExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpIncrementalCost() ),
    m_dict( dict )
{
}

IncrementalCostExtractorList::~IncrementalCostExtractorList()
{
}

AocCostInformationExtractor::AocCostInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAocCostInformation() ),
    accumulated_cost( *this, dict ),
    incremental_cost( *this, dict ),
    currency_code( *this, dict.avpCurrencyCode() )
{
    add( accumulated_cost );
    add( incremental_cost );
    add( currency_code );
}

AocCostInformationExtractor::~AocCostInformationExtractor()
{
}

RateElementExtractor::RateElementExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRateElement() ),
    cc_unit_type( *this, dict.avpCcUnitType() )
{
    add( cc_unit_type );
}

RateElementExtractor::~RateElementExtractor()
{
}

RateElementExtractorList::RateElementExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpRateElement() ),
    m_dict( dict )
{
}

RateElementExtractorList::~RateElementExtractorList()
{
}

CurrentTariffExtractor::CurrentTariffExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCurrentTariff() ),
    currency_code( *this, dict.avpCurrencyCode() ),
    scale_factor( *this, dict ),
    rate_element( *this, dict )
{
    add( currency_code );
    add( scale_factor );
    add( rate_element );
}

CurrentTariffExtractor::~CurrentTariffExtractor()
{
}

NextTariffExtractor::NextTariffExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpNextTariff() ),
    currency_code( *this, dict.avpCurrencyCode() ),
    scale_factor( *this, dict ),
    rate_element( *this, dict )
{
    add( currency_code );
    add( scale_factor );
    add( rate_element );
}

NextTariffExtractor::~NextTariffExtractor()
{
}

TariffInformationExtractor::TariffInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTariffInformation() ),
    current_tariff( *this, dict ),
    tariff_time_change( *this, dict.avpTariffTimeChange() ),
    next_tariff( *this, dict )
{
    add( current_tariff );
    add( tariff_time_change );
    add( next_tariff );
}

TariffInformationExtractor::~TariffInformationExtractor()
{
}

AocServiceExtractor::AocServiceExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAocService() ),
    aoc_service_obligatory_type( *this, dict.avpAocServiceObligatoryType() ),
    aoc_service_type( *this, dict.avpAocServiceType() )
{
    add( aoc_service_obligatory_type );
    add( aoc_service_type );
}

AocServiceExtractor::~AocServiceExtractor()
{
}

AocServiceExtractorList::AocServiceExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAocService() ),
    m_dict( dict )
{
}

AocServiceExtractorList::~AocServiceExtractorList()
{
}

AocSubscriptionInformationExtractor::AocSubscriptionInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAocSubscriptionInformation() ),
    aoc_service( *this, dict ),
    aoc_format( *this, dict.avpAocFormat() ),
    preferred_aoc_currency( *this, dict.avpPreferredAocCurrency() )
{
    add( aoc_service );
    add( aoc_format );
    add( preferred_aoc_currency );
}

AocSubscriptionInformationExtractor::~AocSubscriptionInformationExtractor()
{
}

AocInformationExtractor::AocInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAocInformation() ),
    aoc_cost_information( *this, dict ),
    tariff_information( *this, dict ),
    aoc_subscription_information( *this, dict )
{
    add( aoc_cost_information );
    add( tariff_information );
    add( aoc_subscription_information );
}

AocInformationExtractor::~AocInformationExtractor()
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

TwanUserLocationInfoExtractor::TwanUserLocationInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTwanUserLocationInfo() ),
    ssid( *this, dict.avpSsid() ),
    bssid( *this, dict.avpBssid() )
{
    add( ssid );
    add( bssid );
}

TwanUserLocationInfoExtractor::~TwanUserLocationInfoExtractor()
{
}

PsFurnishChargingInformationExtractor::PsFurnishChargingInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpPsFurnishChargingInformation() ),
    tgpp_charging_id( *this, dict.avp3gppChargingId() ),
    ps_free_format_data( *this, dict.avpPsFreeFormatData() ),
    ps_append_free_format_data( *this, dict.avpPsAppendFreeFormatData() )
{
    add( tgpp_charging_id );
    add( ps_free_format_data );
    add( ps_append_free_format_data );
}

PsFurnishChargingInformationExtractor::~PsFurnishChargingInformationExtractor()
{
}

TimeQuotaMechanismExtractor::TimeQuotaMechanismExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTimeQuotaMechanism() ),
    time_quota_type( *this, dict.avpTimeQuotaType() ),
    base_time_interval( *this, dict.avpBaseTimeInterval() )
{
    add( time_quota_type );
    add( base_time_interval );
}

TimeQuotaMechanismExtractor::~TimeQuotaMechanismExtractor()
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

MultipleServicesCreditControlExtractor::MultipleServicesCreditControlExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMultipleServicesCreditControl() ),
    granted_service_unit( *this, dict )
{
    add( granted_service_unit );
}

MultipleServicesCreditControlExtractor::~MultipleServicesCreditControlExtractor()
{
}

MultipleServicesCreditControlExtractorList::MultipleServicesCreditControlExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpMultipleServicesCreditControl() ),
    m_dict( dict )
{
}

MultipleServicesCreditControlExtractorList::~MultipleServicesCreditControlExtractorList()
{
}

OfflineChargingExtractor::OfflineChargingExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpOfflineCharging() ),
    quota_consumption_time( *this, dict.avpQuotaConsumptionTime() ),
    time_quota_mechanism( *this, dict ),
    envelope_reporting( *this, dict.avpEnvelopeReporting() ),
    multiple_services_credit_control( *this, dict )
{
    add( quota_consumption_time );
    add( time_quota_mechanism );
    add( envelope_reporting );
    add( multiple_services_credit_control );
}

OfflineChargingExtractor::~OfflineChargingExtractor()
{
}

TrafficDataVolumesExtractor::TrafficDataVolumesExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTrafficDataVolumes() ),
    qos_information( *this, dict ),
    accounting_input_octets( *this, dict.avpAccountingInputOctets() ),
    accounting_output_octets( *this, dict.avpAccountingOutputOctets() ),
    change_condition( *this, dict.avpChangeCondition() ),
    change_time( *this, dict.avpChangeTime() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    uwan_user_location_info( *this, dict ),
    tgpp_charging_id( *this, dict.avp3gppChargingId() ),
    presence_reporting_area_status( *this, dict.avpPresenceReportingAreaStatus() ),
    user_csg_information( *this, dict ),
    tgpp_rat_type( *this, dict.avp3gppRatType() )
{
    add( qos_information );
    add( accounting_input_octets );
    add( accounting_output_octets );
    add( change_condition );
    add( change_time );
    add( tgpp_user_location_info );
    add( uwan_user_location_info );
    add( tgpp_charging_id );
    add( presence_reporting_area_status );
    add( user_csg_information );
    add( tgpp_rat_type );
}

TrafficDataVolumesExtractor::~TrafficDataVolumesExtractor()
{
}

TrafficDataVolumesExtractorList::TrafficDataVolumesExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpTrafficDataVolumes() ),
    m_dict( dict )
{
}

TrafficDataVolumesExtractorList::~TrafficDataVolumesExtractorList()
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

AfCorrelationInformationExtractor::AfCorrelationInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAfCorrelationInformation() ),
    af_charging_identifier( *this, dict.avpAfChargingIdentifier() ),
    flows( *this, dict )
{
    add( af_charging_identifier );
    add( flows );
}

AfCorrelationInformationExtractor::~AfCorrelationInformationExtractor()
{
}

ServiceSpecificInfoExtractor::ServiceSpecificInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServiceSpecificInfo() ),
    service_specific_data( *this, dict.avpServiceSpecificData() )
{
    add( service_specific_data );
}

ServiceSpecificInfoExtractor::~ServiceSpecificInfoExtractor()
{
}

RelatedChangeConditionInformationExtractor::RelatedChangeConditionInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRelatedChangeConditionInformation() ),
    sgsn_address( *this, dict.avpSgsnAddress() ),
    change_condition( *this, dict.avpChangeCondition() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    tgpp2_bsid( *this, dict.avp3gpp2Bsid() ),
    uwan_user_location_info( *this, dict ),
    presence_reporting_area_status( *this, dict.avpPresenceReportingAreaStatus() ),
    user_csg_information( *this, dict ),
    tgpp_rat_type( *this, dict.avp3gppRatType() )
{
    add( sgsn_address );
    add( change_condition );
    add( tgpp_user_location_info );
    add( tgpp2_bsid );
    add( uwan_user_location_info );
    add( presence_reporting_area_status );
    add( user_csg_information );
    add( tgpp_rat_type );
}

RelatedChangeConditionInformationExtractor::~RelatedChangeConditionInformationExtractor()
{
}

ServingPlmnRateControlExtractor::ServingPlmnRateControlExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServingPlmnRateControl() ),
    uplink_rate_limit( *this, dict.avpUplinkRateLimit() ),
    downlink_rate_limit( *this, dict.avpDownlinkRateLimit() )
{
    add( uplink_rate_limit );
    add( downlink_rate_limit );
}

ServingPlmnRateControlExtractor::~ServingPlmnRateControlExtractor()
{
}

ApnRateControlUplinkExtractor::ApnRateControlUplinkExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpApnRateControlUplink() ),
    additional_exception_reports( *this, dict.avpAdditionalExceptionReports() ),
    rate_control_time_unit( *this, dict.avpRateControlTimeUnit() ),
    rate_control_max_rate( *this, dict.avpRateControlMaxRate() )
{
    add( additional_exception_reports );
    add( rate_control_time_unit );
    add( rate_control_max_rate );
}

ApnRateControlUplinkExtractor::~ApnRateControlUplinkExtractor()
{
}

ApnRateControlDownlinkExtractor::ApnRateControlDownlinkExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpApnRateControlDownlink() ),
    rate_control_time_unit( *this, dict.avpRateControlTimeUnit() ),
    rate_control_max_rate( *this, dict.avpRateControlMaxRate() ),
    rate_control_max_message_size( *this, dict.avpRateControlMaxMessageSize() )
{
    add( rate_control_time_unit );
    add( rate_control_max_rate );
    add( rate_control_max_message_size );
}

ApnRateControlDownlinkExtractor::~ApnRateControlDownlinkExtractor()
{
}

ApnRateControlExtractor::ApnRateControlExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpApnRateControl() ),
    apn_rate_control_uplink( *this, dict ),
    apn_rate_control_downlink( *this, dict )
{
    add( apn_rate_control_uplink );
    add( apn_rate_control_downlink );
}

ApnRateControlExtractor::~ApnRateControlExtractor()
{
}

ServiceDataContainerExtractor::ServiceDataContainerExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServiceDataContainer() ),
    af_correlation_information( *this, dict ),
    charging_rule_base_name( *this, dict.avpChargingRuleBaseName() ),
    accounting_input_octets( *this, dict.avpAccountingInputOctets() ),
    accounting_output_octets( *this, dict.avpAccountingOutputOctets() ),
    local_sequence_number( *this, dict.avpLocalSequenceNumber() ),
    qos_information( *this, dict ),
    rating_group( *this, dict.avpRatingGroup() ),
    change_time( *this, dict.avpChangeTime() ),
    service_identifier( *this, dict.avpServiceIdentifier() ),
    service_specific_info( *this, dict ),
    adc_rule_base_name( *this, dict.avpAdcRuleBaseName() ),
    sgsn_address( *this, dict.avpSgsnAddress() ),
    time_first_usage( *this, dict.avpTimeFirstUsage() ),
    time_last_usage( *this, dict.avpTimeLastUsage() ),
    time_usage( *this, dict.avpTimeUsage() ),
    change_condition( *this, dict.avpChangeCondition() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    tgpp2_bsid( *this, dict.avp3gpp2Bsid() ),
    uwan_user_location_info( *this, dict ),
    sponsor_identity( *this, dict.avpSponsorIdentity() ),
    application_service_provider_identity( *this, dict.avpApplicationServiceProviderIdentity() ),
    presence_reporting_area_information( *this, dict ),
    presence_reporting_area_status( *this, dict.avpPresenceReportingAreaStatus() ),
    user_csg_information( *this, dict ),
    tgpp_rat_type( *this, dict.avp3gppRatType() ),
    related_change_condition_information( *this, dict ),
    serving_plmn_rate_control( *this, dict ),
    apn_rate_control( *this, dict ),
    tgpp_ps_data_off_status( *this, dict.avp3gppPsDataOffStatus() )
{
    add( af_correlation_information );
    add( charging_rule_base_name );
    add( accounting_input_octets );
    add( accounting_output_octets );
    add( local_sequence_number );
    add( qos_information );
    add( rating_group );
    add( change_time );
    add( service_identifier );
    add( service_specific_info );
    add( adc_rule_base_name );
    add( sgsn_address );
    add( time_first_usage );
    add( time_last_usage );
    add( time_usage );
    add( change_condition );
    add( tgpp_user_location_info );
    add( tgpp2_bsid );
    add( uwan_user_location_info );
    add( sponsor_identity );
    add( application_service_provider_identity );
    add( presence_reporting_area_information );
    add( presence_reporting_area_status );
    add( user_csg_information );
    add( tgpp_rat_type );
    add( related_change_condition_information );
    add( serving_plmn_rate_control );
    add( apn_rate_control );
    add( tgpp_ps_data_off_status );
}

ServiceDataContainerExtractor::~ServiceDataContainerExtractor()
{
}

ServiceDataContainerExtractorList::ServiceDataContainerExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpServiceDataContainer() ),
    m_dict( dict )
{
}

ServiceDataContainerExtractorList::~ServiceDataContainerExtractorList()
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

TerminalInformationExtractor::TerminalInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTerminalInformation() ),
    imei( *this, dict.avpImei() ),
    software_version( *this, dict.avpSoftwareVersion() )
{
    add( imei );
    add( software_version );
}

TerminalInformationExtractor::~TerminalInformationExtractor()
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

EnhancedDiagnosticsExtractor::EnhancedDiagnosticsExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEnhancedDiagnostics() ),
    ran_nas_release_cause( *this, dict.avpRanNasReleaseCause() )
{
    add( ran_nas_release_cause );
}

EnhancedDiagnosticsExtractor::~EnhancedDiagnosticsExtractor()
{
}

RrcCauseCounterExtractor::RrcCauseCounterExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRrcCauseCounter() ),
    counter_value( *this, dict.avpCounterValue() ),
    rrc_counter_timestamp( *this, dict.avpRrcCounterTimestamp() )
{
    add( counter_value );
    add( rrc_counter_timestamp );
}

RrcCauseCounterExtractor::~RrcCauseCounterExtractor()
{
}

ScsAsAddressExtractor::ScsAsAddressExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpScsAsAddress() ),
    scs_realm( *this, dict.avpScsRealm() ),
    scs_address( *this, dict.avpScsAddress() )
{
    add( scs_realm );
    add( scs_address );
}

ScsAsAddressExtractor::~ScsAsAddressExtractor()
{
}

PsInformationExtractor::PsInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpPsInformation() ),
    tgpp_charging_id( *this, dict.avp3gppChargingId() ),
    pdn_connection_charging_id( *this, dict.avpPdnConnectionChargingId() ),
    node_id( *this, dict.avpNodeId() ),
    tgpp_pdp_type( *this, dict.avp3gppPdpType() ),
    pdp_address( *this, dict.avpPdpAddress() ),
    pdp_address_prefix_length( *this, dict.avpPdpAddressPrefixLength() ),
    dynamic_address_flag( *this, dict.avpDynamicAddressFlag() ),
    dynamic_address_flag_extension( *this, dict.avpDynamicAddressFlagExtension() ),
    qos_information( *this, dict ),
    sgsn_address( *this, dict.avpSgsnAddress() ),
    ggsn_address( *this, dict.avpGgsnAddress() ),
    tdf_ip_address( *this, dict.avpTdfIpAddress() ),
    sgw_address( *this, dict.avpSgwAddress() ),
    epdg_address( *this, dict.avpEpdgAddress() ),
    twag_address( *this, dict.avpTwagAddress() ),
    cg_address( *this, dict.avpCgAddress() ),
    serving_node_type( *this, dict.avpServingNodeType() ),
    sgw_change( *this, dict.avpSgwChange() ),
    tgpp_imsi_mcc_mnc( *this, dict.avp3gppImsiMccMnc() ),
    imsi_unauthenticated_flag( *this, dict.avpImsiUnauthenticatedFlag() ),
    tgpp_ggsn_mcc_mnc( *this, dict.avp3gppGgsnMccMnc() ),
    tgpp_nsapi( *this, dict.avp3gppNsapi() ),
    called_station_id( *this, dict.avpCalledStationId() ),
    tgpp_selection_mode( *this, dict.avp3gppSelectionMode() ),
    tgpp_charging_characteristics( *this, dict.avp3gppChargingCharacteristics() ),
    charging_characteristics_selection_mode( *this, dict.avpChargingCharacteristicsSelectionMode() ),
    tgpp_sgsn_mcc_mnc( *this, dict.avp3gppSgsnMccMnc() ),
    tgpp_ms_timezone( *this, dict.avp3gppMsTimezone() ),
    charging_rule_base_name( *this, dict.avpChargingRuleBaseName() ),
    adc_rule_base_name( *this, dict.avpAdcRuleBaseName() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    user_location_info_time( *this, dict.avpUserLocationInfoTime() ),
    user_csg_information( *this, dict ),
    presence_reporting_area_information( *this, dict ),
    tgpp2_bsid( *this, dict.avp3gpp2Bsid() ),
    twan_user_location_info( *this, dict ),
    uwan_user_location_info( *this, dict ),
    tgpp_rat_type( *this, dict.avp3gppRatType() ),
    ps_furnish_charging_information( *this, dict ),
    pdp_context_type( *this, dict.avpPdpContextType() ),
    offline_charging( *this, dict ),
    traffic_data_volumes( *this, dict ),
    service_data_container( *this, dict ),
    user_equipment_info( *this, dict ),
    terminal_information( *this, dict ),
    start_time( *this, dict.avpStartTime() ),
    stop_time( *this, dict.avpStopTime() ),
    change_condition( *this, dict.avpChangeCondition() ),
    diagnostics( *this, dict.avpDiagnostics() ),
    low_priority_indicator( *this, dict.avpLowPriorityIndicator() ),
    nbifom_mode( *this, dict.avpNbifomMode() ),
    nbifom_support( *this, dict.avpNbifomSupport() ),
    mme_number_for_mt_sms( *this, dict.avpMmeNumberForMtSms() ),
    mme_name( *this, dict.avpMmeName() ),
    mme_realm( *this, dict.avpMmeRealm() ),
    logical_access_id( *this, dict.avpLogicalAccessId() ),
    physical_access_id( *this, dict.avpPhysicalAccessId() ),
    fixed_user_location_info( *this, dict ),
    cn_operator_selection_entity( *this, dict.avpCnOperatorSelectionEntity() ),
    enhanced_diagnostics( *this, dict ),
    sgi_ptp_tunnelling_method( *this, dict.avpSgiPtpTunnellingMethod() ),
    cp_ciot_eps_optimisation_indicator( *this, dict.avpCpCiotEpsOptimisationIndicator() ),
    uni_pdu_cp_only_flag( *this, dict.avpUniPduCpOnlyFlag() ),
    serving_plmn_rate_control( *this, dict ),
    apn_rate_control( *this, dict ),
    charging_per_ip_can_session_indicator( *this, dict.avpChargingPerIpCanSessionIndicator() ),
    rrc_cause_counter( *this, dict ),
    tgpp_ps_data_off_status( *this, dict.avp3gppPsDataOffStatus() ),
    scs_as_address( *this, dict )
{
    add( tgpp_charging_id );
    add( pdn_connection_charging_id );
    add( node_id );
    add( tgpp_pdp_type );
    add( pdp_address );
    add( pdp_address_prefix_length );
    add( dynamic_address_flag );
    add( dynamic_address_flag_extension );
    add( qos_information );
    add( sgsn_address );
    add( ggsn_address );
    add( tdf_ip_address );
    add( sgw_address );
    add( epdg_address );
    add( twag_address );
    add( cg_address );
    add( serving_node_type );
    add( sgw_change );
    add( tgpp_imsi_mcc_mnc );
    add( imsi_unauthenticated_flag );
    add( tgpp_ggsn_mcc_mnc );
    add( tgpp_nsapi );
    add( called_station_id );
    add( tgpp_selection_mode );
    add( tgpp_charging_characteristics );
    add( charging_characteristics_selection_mode );
    add( tgpp_sgsn_mcc_mnc );
    add( tgpp_ms_timezone );
    add( charging_rule_base_name );
    add( adc_rule_base_name );
    add( tgpp_user_location_info );
    add( user_location_info_time );
    add( user_csg_information );
    add( presence_reporting_area_information );
    add( tgpp2_bsid );
    add( twan_user_location_info );
    add( uwan_user_location_info );
    add( tgpp_rat_type );
    add( ps_furnish_charging_information );
    add( pdp_context_type );
    add( offline_charging );
    add( traffic_data_volumes );
    add( service_data_container );
    add( user_equipment_info );
    add( terminal_information );
    add( start_time );
    add( stop_time );
    add( change_condition );
    add( diagnostics );
    add( low_priority_indicator );
    add( nbifom_mode );
    add( nbifom_support );
    add( mme_number_for_mt_sms );
    add( mme_name );
    add( mme_realm );
    add( logical_access_id );
    add( physical_access_id );
    add( fixed_user_location_info );
    add( cn_operator_selection_entity );
    add( enhanced_diagnostics );
    add( sgi_ptp_tunnelling_method );
    add( cp_ciot_eps_optimisation_indicator );
    add( uni_pdu_cp_only_flag );
    add( serving_plmn_rate_control );
    add( apn_rate_control );
    add( charging_per_ip_can_session_indicator );
    add( rrc_cause_counter );
    add( tgpp_ps_data_off_status );
    add( scs_as_address );
}

PsInformationExtractor::~PsInformationExtractor()
{
}

EventTypeExtractor::EventTypeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEventType() ),
    sip_method( *this, dict.avpSipMethod() ),
    event( *this, dict.avpEvent() ),
    expires( *this, dict.avpExpires() )
{
    add( sip_method );
    add( event );
    add( expires );
}

EventTypeExtractor::~EventTypeExtractor()
{
}

CalledIdentityChangeExtractor::CalledIdentityChangeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCalledIdentityChange() ),
    called_identity( *this, dict.avpCalledIdentity() ),
    change_time( *this, dict.avpChangeTime() )
{
    add( called_identity );
    add( change_time );
}

CalledIdentityChangeExtractor::~CalledIdentityChangeExtractor()
{
}

TimeStampsExtractor::TimeStampsExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTimeStamps() ),
    sip_request_timestamp( *this, dict.avpSipRequestTimestamp() )
{
    add( sip_request_timestamp );
}

TimeStampsExtractor::~TimeStampsExtractor()
{
}

ApplicationServerInformationExtractor::ApplicationServerInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpApplicationServerInformation() ),
    application_server( *this, dict.avpApplicationServer() )
{
    add( application_server );
}

ApplicationServerInformationExtractor::~ApplicationServerInformationExtractor()
{
}

ApplicationServerInformationExtractorList::ApplicationServerInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpApplicationServerInformation() ),
    m_dict( dict )
{
}

ApplicationServerInformationExtractorList::~ApplicationServerInformationExtractorList()
{
}

InterOperatorIdentifierExtractor::InterOperatorIdentifierExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpInterOperatorIdentifier() ),
    originating_ioi( *this, dict.avpOriginatingIoi() ),
    terminating_ioi( *this, dict.avpTerminatingIoi() )
{
    add( originating_ioi );
    add( terminating_ioi );
}

InterOperatorIdentifierExtractor::~InterOperatorIdentifierExtractor()
{
}

InterOperatorIdentifierExtractorList::InterOperatorIdentifierExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpInterOperatorIdentifier() ),
    m_dict( dict )
{
}

InterOperatorIdentifierExtractorList::~InterOperatorIdentifierExtractorList()
{
}

SdpMediaComponentExtractorList::SdpMediaComponentExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpSdpMediaComponent() ),
    m_dict( dict )
{
}

SdpMediaComponentExtractorList::~SdpMediaComponentExtractorList()
{
}

ServerCapabilitiesExtractor::ServerCapabilitiesExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServerCapabilities() ),
    mandatory_capability( *this, dict.avpMandatoryCapability() ),
    optional_capability( *this, dict.avpOptionalCapability() ),
    server_name( *this, dict.avpServerName() )
{
    add( mandatory_capability );
    add( optional_capability );
    add( server_name );
}

ServerCapabilitiesExtractor::~ServerCapabilitiesExtractor()
{
}

TrunkGroupIdExtractor::TrunkGroupIdExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTrunkGroupId() ),
    incoming_trunk_group_id( *this, dict.avpIncomingTrunkGroupId() )
{
    add( incoming_trunk_group_id );
}

TrunkGroupIdExtractor::~TrunkGroupIdExtractor()
{
}

ServiceSpecificInfoExtractorList::ServiceSpecificInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpServiceSpecificInfo() ),
    m_dict( dict )
{
}

ServiceSpecificInfoExtractorList::~ServiceSpecificInfoExtractorList()
{
}

MessageBodyExtractor::MessageBodyExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMessageBody() ),
    content_type( *this, dict.avpContentType() )
{
    add( content_type );
}

MessageBodyExtractor::~MessageBodyExtractor()
{
}

MessageBodyExtractorList::MessageBodyExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpMessageBody() ),
    m_dict( dict )
{
}

MessageBodyExtractorList::~MessageBodyExtractorList()
{
}

SdpTimestampsExtractor::SdpTimestampsExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSdpTimestamps() ),
    sdp_offer_timestamp( *this, dict.avpSdpOfferTimestamp() ),
    sdp_answer_timestamp( *this, dict.avpSdpAnswerTimestamp() )
{
    add( sdp_offer_timestamp );
    add( sdp_answer_timestamp );
}

SdpTimestampsExtractor::~SdpTimestampsExtractor()
{
}

EarlyMediaDescriptionExtractor::EarlyMediaDescriptionExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEarlyMediaDescription() ),
    sdp_timestamps( *this, dict ),
    sdp_media_component( *this, dict ),
    sdp_session_description( *this, dict.avpSdpSessionDescription() )
{
    add( sdp_timestamps );
    add( sdp_media_component );
    add( sdp_session_description );
}

EarlyMediaDescriptionExtractor::~EarlyMediaDescriptionExtractor()
{
}

EarlyMediaDescriptionExtractorList::EarlyMediaDescriptionExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpEarlyMediaDescription() ),
    m_dict( dict )
{
}

EarlyMediaDescriptionExtractorList::~EarlyMediaDescriptionExtractorList()
{
}

RealTimeTariffInformationExtractor::RealTimeTariffInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRealTimeTariffInformation() ),
    tariff_information( *this, dict ),
    tariff_xml( *this, dict.avpTariffXml() )
{
    add( tariff_information );
    add( tariff_xml );
}

RealTimeTariffInformationExtractor::~RealTimeTariffInformationExtractor()
{
}

NniInformationExtractor::NniInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpNniInformation() ),
    session_direction( *this, dict.avpSessionDirection() ),
    nni_type( *this, dict.avpNniType() ),
    relationship_mode( *this, dict.avpRelationshipMode() ),
    neighbour_node_address( *this, dict.avpNeighbourNodeAddress() )
{
    add( session_direction );
    add( nni_type );
    add( relationship_mode );
    add( neighbour_node_address );
}

NniInformationExtractor::~NniInformationExtractor()
{
}

NniInformationExtractorList::NniInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpNniInformation() ),
    m_dict( dict )
{
}

NniInformationExtractorList::~NniInformationExtractorList()
{
}

AccessNetworkInfoChangeExtractor::AccessNetworkInfoChangeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAccessNetworkInfoChange() ),
    access_network_information( *this, dict.avpAccessNetworkInformation() ),
    cellular_network_information( *this, dict.avpCellularNetworkInformation() ),
    change_time( *this, dict.avpChangeTime() )
{
    add( access_network_information );
    add( cellular_network_information );
    add( change_time );
}

AccessNetworkInfoChangeExtractor::~AccessNetworkInfoChangeExtractor()
{
}

AccessNetworkInfoChangeExtractorList::AccessNetworkInfoChangeExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAccessNetworkInfoChange() ),
    m_dict( dict )
{
}

AccessNetworkInfoChangeExtractorList::~AccessNetworkInfoChangeExtractorList()
{
}

AccessTransferInformationExtractor::AccessTransferInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAccessTransferInformation() ),
    access_transfer_type( *this, dict.avpAccessTransferType() ),
    access_network_information( *this, dict.avpAccessNetworkInformation() ),
    cellular_network_information( *this, dict.avpCellularNetworkInformation() ),
    inter_ue_transfer( *this, dict.avpInterUeTransfer() ),
    user_equipment_info( *this, dict ),
    instance_id( *this, dict.avpInstanceId() ),
    related_ims_charging_identifier( *this, dict.avpRelatedImsChargingIdentifier() ),
    related_ims_charging_identifier_node( *this, dict.avpRelatedImsChargingIdentifierNode() ),
    change_time( *this, dict.avpChangeTime() )
{
    add( access_transfer_type );
    add( access_network_information );
    add( cellular_network_information );
    add( inter_ue_transfer );
    add( user_equipment_info );
    add( instance_id );
    add( related_ims_charging_identifier );
    add( related_ims_charging_identifier_node );
    add( change_time );
}

AccessTransferInformationExtractor::~AccessTransferInformationExtractor()
{
}

AccessTransferInformationExtractorList::AccessTransferInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAccessTransferInformation() ),
    m_dict( dict )
{
}

AccessTransferInformationExtractorList::~AccessTransferInformationExtractorList()
{
}

ImsInformationExtractor::ImsInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpImsInformation() ),
    event_type( *this, dict ),
    role_of_node( *this, dict.avpRoleOfNode() ),
    node_functionality( *this, dict.avpNodeFunctionality() ),
    user_session_id( *this, dict.avpUserSessionId() ),
    outgoing_session_id( *this, dict.avpOutgoingSessionId() ),
    session_priority( *this, dict.avpSessionPriority() ),
    calling_party_address( *this, dict.avpCallingPartyAddress() ),
    called_party_address( *this, dict.avpCalledPartyAddress() ),
    called_asserted_identity( *this, dict.avpCalledAssertedIdentity() ),
    called_identity_change( *this, dict ),
    number_portability_routing_information( *this, dict.avpNumberPortabilityRoutingInformation() ),
    carrier_select_routing_information( *this, dict.avpCarrierSelectRoutingInformation() ),
    alternate_charged_party_address( *this, dict.avpAlternateChargedPartyAddress() ),
    requested_party_address( *this, dict.avpRequestedPartyAddress() ),
    associated_uri( *this, dict.avpAssociatedUri() ),
    time_stamps( *this, dict ),
    application_server_information( *this, dict ),
    inter_operator_identifier( *this, dict ),
    transit_ioi_list( *this, dict.avpTransitIoiList() ),
    ims_charging_identifier( *this, dict.avpImsChargingIdentifier() ),
    sdp_session_description( *this, dict.avpSdpSessionDescription() ),
    sdp_media_component( *this, dict ),
    served_party_ip_address( *this, dict.avpServedPartyIpAddress() ),
    server_capabilities( *this, dict ),
    trunk_group_id( *this, dict ),
    bearer_service( *this, dict.avpBearerService() ),
    service_id( *this, dict.avpServiceId() ),
    service_specific_info( *this, dict ),
    message_body( *this, dict ),
    cause_code( *this, dict.avpCauseCode() ),
    reason_header( *this, dict.avpReasonHeader() ),
    access_network_information( *this, dict.avpAccessNetworkInformation() ),
    cellular_network_information( *this, dict.avpCellularNetworkInformation() ),
    early_media_description( *this, dict ),
    ims_communication_service_identifier( *this, dict.avpImsCommunicationServiceIdentifier() ),
    ims_application_reference_identifier( *this, dict.avpImsApplicationReferenceIdentifier() ),
    online_charging_flag( *this, dict.avpOnlineChargingFlag() ),
    real_time_tariff_information( *this, dict ),
    account_expiration( *this, dict.avpAccountExpiration() ),
    initial_ims_charging_identifier( *this, dict.avpInitialImsChargingIdentifier() ),
    nni_information( *this, dict ),
    from_address( *this, dict.avpFromAddress() ),
    ims_emergency_indicator( *this, dict.avpImsEmergencyIndicator() ),
    ims_visited_network_identifier( *this, dict.avpImsVisitedNetworkIdentifier() ),
    access_network_info_change( *this, dict ),
    access_transfer_information( *this, dict ),
    related_ims_charging_identifier( *this, dict.avpRelatedImsChargingIdentifier() ),
    related_ims_charging_identifier_node( *this, dict.avpRelatedImsChargingIdentifierNode() ),
    route_header_received( *this, dict.avpRouteHeaderReceived() ),
    route_header_transmitted( *this, dict.avpRouteHeaderTransmitted() ),
    instance_id( *this, dict.avpInstanceId() ),
    tad_identifier( *this, dict.avpTadIdentifier() )
{
    add( event_type );
    add( role_of_node );
    add( node_functionality );
    add( user_session_id );
    add( outgoing_session_id );
    add( session_priority );
    add( calling_party_address );
    add( called_party_address );
    add( called_asserted_identity );
    add( called_identity_change );
    add( number_portability_routing_information );
    add( carrier_select_routing_information );
    add( alternate_charged_party_address );
    add( requested_party_address );
    add( associated_uri );
    add( time_stamps );
    add( application_server_information );
    add( inter_operator_identifier );
    add( transit_ioi_list );
    add( ims_charging_identifier );
    add( sdp_session_description );
    add( sdp_media_component );
    add( served_party_ip_address );
    add( server_capabilities );
    add( trunk_group_id );
    add( bearer_service );
    add( service_id );
    add( service_specific_info );
    add( message_body );
    add( cause_code );
    add( reason_header );
    add( access_network_information );
    add( cellular_network_information );
    add( early_media_description );
    add( ims_communication_service_identifier );
    add( ims_application_reference_identifier );
    add( online_charging_flag );
    add( real_time_tariff_information );
    add( account_expiration );
    add( initial_ims_charging_identifier );
    add( nni_information );
    add( from_address );
    add( ims_emergency_indicator );
    add( ims_visited_network_identifier );
    add( access_network_info_change );
    add( access_transfer_information );
    add( related_ims_charging_identifier );
    add( related_ims_charging_identifier_node );
    add( route_header_received );
    add( route_header_transmitted );
    add( instance_id );
    add( tad_identifier );
}

ImsInformationExtractor::~ImsInformationExtractor()
{
}

OriginatorAddressExtractor::OriginatorAddressExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpOriginatorAddress() ),
    address_type( *this, dict.avpAddressType() ),
    address_data( *this, dict.avpAddressData() ),
    address_domain( *this, dict )
{
    add( address_type );
    add( address_data );
    add( address_domain );
}

OriginatorAddressExtractor::~OriginatorAddressExtractor()
{
}

RecipientAddressExtractor::RecipientAddressExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRecipientAddress() ),
    address_type( *this, dict.avpAddressType() ),
    address_data( *this, dict.avpAddressData() ),
    address_domain( *this, dict ),
    addressee_type( *this, dict.avpAddresseeType() )
{
    add( address_type );
    add( address_data );
    add( address_domain );
    add( addressee_type );
}

RecipientAddressExtractor::~RecipientAddressExtractor()
{
}

RecipientAddressExtractorList::RecipientAddressExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpRecipientAddress() ),
    m_dict( dict )
{
}

RecipientAddressExtractorList::~RecipientAddressExtractorList()
{
}

AdditionalContentInformationExtractor::AdditionalContentInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAdditionalContentInformation() ),
    type_number( *this, dict.avpTypeNumber() ),
    additional_type_information( *this, dict.avpAdditionalTypeInformation() ),
    content_size( *this, dict.avpContentSize() )
{
    add( type_number );
    add( additional_type_information );
    add( content_size );
}

AdditionalContentInformationExtractor::~AdditionalContentInformationExtractor()
{
}

AdditionalContentInformationExtractorList::AdditionalContentInformationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAdditionalContentInformation() ),
    m_dict( dict )
{
}

AdditionalContentInformationExtractorList::~AdditionalContentInformationExtractorList()
{
}

MmContentTypeExtractor::MmContentTypeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMmContentType() ),
    type_number( *this, dict.avpTypeNumber() ),
    additional_type_information( *this, dict.avpAdditionalTypeInformation() ),
    content_size( *this, dict.avpContentSize() ),
    additional_content_information( *this, dict )
{
    add( type_number );
    add( additional_type_information );
    add( content_size );
    add( additional_content_information );
}

MmContentTypeExtractor::~MmContentTypeExtractor()
{
}

MessageClassExtractor::MessageClassExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMessageClass() ),
    class_identifier( *this, dict.avpClassIdentifier() ),
    token_text( *this, dict.avpTokenText() )
{
    add( class_identifier );
    add( token_text );
}

MessageClassExtractor::~MessageClassExtractor()
{
}

MmsInformationExtractor::MmsInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMmsInformation() ),
    originator_address( *this, dict ),
    recipient_address( *this, dict ),
    submission_time( *this, dict.avpSubmissionTime() ),
    mm_content_type( *this, dict ),
    priority( *this, dict.avpPriority() ),
    message_id( *this, dict.avpMessageId() ),
    message_type( *this, dict.avpMessageType() ),
    message_size( *this, dict.avpMessageSize() ),
    message_class( *this, dict ),
    delivery_report_requested( *this, dict.avpDeliveryReportRequested() ),
    read_reply_report_requested( *this, dict.avpReadReplyReportRequested() ),
    mmbox_storage_requested( *this, dict.avpMmboxStorageRequested() ),
    applic_id( *this, dict.avpApplicId() ),
    reply_applic_id( *this, dict.avpReplyApplicId() ),
    aux_applic_info( *this, dict.avpAuxApplicInfo() ),
    content_class( *this, dict.avpContentClass() ),
    drm_content( *this, dict.avpDrmContent() ),
    adaptations( *this, dict.avpAdaptations() )
{
    add( originator_address );
    add( recipient_address );
    add( submission_time );
    add( mm_content_type );
    add( priority );
    add( message_id );
    add( message_type );
    add( message_size );
    add( message_class );
    add( delivery_report_requested );
    add( read_reply_report_requested );
    add( mmbox_storage_requested );
    add( applic_id );
    add( reply_applic_id );
    add( aux_applic_info );
    add( content_class );
    add( drm_content );
    add( adaptations );
}

MmsInformationExtractor::~MmsInformationExtractor()
{
}

LcsClientNameExtractor::LcsClientNameExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLcsClientName() ),
    lcs_data_coding_scheme( *this, dict.avpLcsDataCodingScheme() ),
    lcs_name_string( *this, dict.avpLcsNameString() ),
    lcs_format_indicator( *this, dict.avpLcsFormatIndicator() )
{
    add( lcs_data_coding_scheme );
    add( lcs_name_string );
    add( lcs_format_indicator );
}

LcsClientNameExtractor::~LcsClientNameExtractor()
{
}

LcsRequestorIdExtractor::LcsRequestorIdExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLcsRequestorId() ),
    lcs_data_coding_scheme( *this, dict.avpLcsDataCodingScheme() ),
    lcs_requestor_id_string( *this, dict.avpLcsRequestorIdString() )
{
    add( lcs_data_coding_scheme );
    add( lcs_requestor_id_string );
}

LcsRequestorIdExtractor::~LcsRequestorIdExtractor()
{
}

LcsClientIdExtractor::LcsClientIdExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLcsClientId() ),
    lcs_client_type( *this, dict.avpLcsClientType() ),
    lcs_client_external_id( *this, dict.avpLcsClientExternalId() ),
    lcs_client_dialed_by_ms( *this, dict.avpLcsClientDialedByMs() ),
    lcs_client_name( *this, dict ),
    lcs_apn( *this, dict.avpLcsApn() ),
    lcs_requestor_id( *this, dict )
{
    add( lcs_client_type );
    add( lcs_client_external_id );
    add( lcs_client_dialed_by_ms );
    add( lcs_client_name );
    add( lcs_apn );
    add( lcs_requestor_id );
}

LcsClientIdExtractor::~LcsClientIdExtractor()
{
}

LocationTypeExtractor::LocationTypeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLocationType() ),
    location_estimate_type( *this, dict.avpLocationEstimateType() ),
    deferred_location_event_type( *this, dict.avpDeferredLocationEventType() )
{
    add( location_estimate_type );
    add( deferred_location_event_type );
}

LocationTypeExtractor::~LocationTypeExtractor()
{
}

LcsInformationExtractor::LcsInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLcsInformation() ),
    lcs_client_id( *this, dict ),
    location_type( *this, dict ),
    location_estimate( *this, dict.avpLocationEstimate() ),
    positioning_data( *this, dict.avpPositioningData() ),
    tgpp_imsi( *this, dict.avp3gppImsi() ),
    msisdn( *this, dict.avpMsisdn() )
{
    add( lcs_client_id );
    add( location_type );
    add( location_estimate );
    add( positioning_data );
    add( tgpp_imsi );
    add( msisdn );
}

LcsInformationExtractor::~LcsInformationExtractor()
{
}

PocInformationExtractor::PocInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpPocInformation() ),
    poc_server_role( *this, dict.avpPocServerRole() )
{
    add( poc_server_role );
}

PocInformationExtractor::~PocInformationExtractor()
{
}

MbmsInformationExtractor::MbmsInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMbmsInformation() ),
    tmgi( *this, dict.avpTmgi() ),
    mbms_service_type( *this, dict.avpMbmsServiceType() ),
    mbms_user_service_type( *this, dict.avpMbmsUserServiceType() ),
    file_repair_supported( *this, dict.avpFileRepairSupported() ),
    required_mbms_bearer_capabilities( *this, dict.avpRequiredMbmsBearerCapabilities() ),
    mbms_2g_3g_indicator( *this, dict.avpMbms2g3gIndicator() ),
    rai( *this, dict.avpRai() ),
    mbms_service_area( *this, dict.avpMbmsServiceArea() ),
    mbms_session_identity( *this, dict.avpMbmsSessionIdentity() ),
    cn_ip_multicast_distribution( *this, dict.avpCnIpMulticastDistribution() ),
    mbms_gw_address( *this, dict.avpMbmsGwAddress() ),
    mbms_charged_party( *this, dict.avpMbmsChargedParty() ),
    msisdn( *this, dict.avpMsisdn() ),
    mbms_data_transfer_start( *this, dict.avpMbmsDataTransferStart() ),
    mbms_data_transfer_stop( *this, dict.avpMbmsDataTransferStop() )
{
    add( tmgi );
    add( mbms_service_type );
    add( mbms_user_service_type );
    add( file_repair_supported );
    add( required_mbms_bearer_capabilities );
    add( mbms_2g_3g_indicator );
    add( rai );
    add( mbms_service_area );
    add( mbms_session_identity );
    add( cn_ip_multicast_distribution );
    add( mbms_gw_address );
    add( mbms_charged_party );
    add( msisdn );
    add( mbms_data_transfer_start );
    add( mbms_data_transfer_stop );
}

MbmsInformationExtractor::~MbmsInformationExtractor()
{
}

OriginatorInterfaceExtractor::OriginatorInterfaceExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpOriginatorInterface() ),
    interface_id( *this, dict.avpInterfaceId() ),
    interface_text( *this, dict.avpInterfaceText() ),
    interface_port( *this, dict.avpInterfacePort() ),
    interface_type( *this, dict.avpInterfaceType() )
{
    add( interface_id );
    add( interface_text );
    add( interface_port );
    add( interface_type );
}

OriginatorInterfaceExtractor::~OriginatorInterfaceExtractor()
{
}

DestinationInterfaceExtractor::DestinationInterfaceExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpDestinationInterface() ),
    interface_id( *this, dict.avpInterfaceId() ),
    interface_text( *this, dict.avpInterfaceText() ),
    interface_port( *this, dict.avpInterfacePort() ),
    interface_type( *this, dict.avpInterfaceType() )
{
    add( interface_id );
    add( interface_text );
    add( interface_port );
    add( interface_type );
}

DestinationInterfaceExtractor::~DestinationInterfaceExtractor()
{
}

RecipientReceivedAddressExtractor::RecipientReceivedAddressExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRecipientReceivedAddress() ),
    address_type( *this, dict.avpAddressType() ),
    address_data( *this, dict.avpAddressData() ),
    address_domain( *this, dict )
{
    add( address_type );
    add( address_data );
    add( address_domain );
}

RecipientReceivedAddressExtractor::~RecipientReceivedAddressExtractor()
{
}

RecipientReceivedAddressExtractorList::RecipientReceivedAddressExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpRecipientReceivedAddress() ),
    m_dict( dict )
{
}

RecipientReceivedAddressExtractorList::~RecipientReceivedAddressExtractorList()
{
}

RecipientInfoExtractor::RecipientInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRecipientInfo() ),
    destination_interface( *this, dict ),
    recipient_address( *this, dict ),
    recipient_received_address( *this, dict ),
    recipient_sccp_address( *this, dict.avpRecipientSccpAddress() ),
    sm_protocol_id( *this, dict.avpSmProtocolId() )
{
    add( destination_interface );
    add( recipient_address );
    add( recipient_received_address );
    add( recipient_sccp_address );
    add( sm_protocol_id );
}

RecipientInfoExtractor::~RecipientInfoExtractor()
{
}

RecipientInfoExtractorList::RecipientInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpRecipientInfo() ),
    m_dict( dict )
{
}

RecipientInfoExtractorList::~RecipientInfoExtractorList()
{
}

OriginatorReceivedAddressExtractor::OriginatorReceivedAddressExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpOriginatorReceivedAddress() ),
    address_type( *this, dict.avpAddressType() ),
    address_data( *this, dict.avpAddressData() ),
    address_domain( *this, dict )
{
    add( address_type );
    add( address_data );
    add( address_domain );
}

OriginatorReceivedAddressExtractor::~OriginatorReceivedAddressExtractor()
{
}

SmDeviceTriggerInformationExtractor::SmDeviceTriggerInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSmDeviceTriggerInformation() ),
    mtc_iwf_address( *this, dict.avpMtcIwfAddress() ),
    reference_number( *this, dict.avpReferenceNumber() )
{
    add( mtc_iwf_address );
    add( reference_number );
}

SmDeviceTriggerInformationExtractor::~SmDeviceTriggerInformationExtractor()
{
}

SmsInformationExtractor::SmsInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSmsInformation() ),
    sms_node( *this, dict.avpSmsNode() ),
    client_address( *this, dict.avpClientAddress() ),
    originator_sccp_address( *this, dict.avpOriginatorSccpAddress() ),
    smsc_address( *this, dict.avpSmscAddress() ),
    data_coding_scheme( *this, dict.avpDataCodingScheme() ),
    sm_discharge_time( *this, dict.avpSmDischargeTime() ),
    sm_message_type( *this, dict.avpSmMessageType() ),
    originator_interface( *this, dict ),
    sm_protocol_id( *this, dict.avpSmProtocolId() ),
    reply_path_requested( *this, dict.avpReplyPathRequested() ),
    sm_status( *this, dict.avpSmStatus() ),
    sm_user_data_header( *this, dict.avpSmUserDataHeader() ),
    number_of_messages_sent( *this, dict.avpNumberOfMessagesSent() ),
    sm_sequence_number( *this, dict.avpSmSequenceNumber() ),
    recipient_info( *this, dict ),
    originator_received_address( *this, dict ),
    sm_service_type( *this, dict.avpSmServiceType() ),
    sms_result( *this, dict.avpSmsResult() ),
    sm_device_trigger_indicator( *this, dict.avpSmDeviceTriggerIndicator() ),
    sm_device_trigger_information( *this, dict )
{
    add( sms_node );
    add( client_address );
    add( originator_sccp_address );
    add( smsc_address );
    add( data_coding_scheme );
    add( sm_discharge_time );
    add( sm_message_type );
    add( originator_interface );
    add( sm_protocol_id );
    add( reply_path_requested );
    add( sm_status );
    add( sm_user_data_header );
    add( number_of_messages_sent );
    add( sm_sequence_number );
    add( recipient_info );
    add( originator_received_address );
    add( sm_service_type );
    add( sms_result );
    add( sm_device_trigger_indicator );
    add( sm_device_trigger_information );
}

SmsInformationExtractor::~SmsInformationExtractor()
{
}

VcsInformationExtractor::VcsInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpVcsInformation() ),
    bearer_capability( *this, dict.avpBearerCapability() ),
    network_call_reference_number( *this, dict.avpNetworkCallReferenceNumber() ),
    msc_address( *this, dict.avpMscAddress() )
{
    add( bearer_capability );
    add( network_call_reference_number );
    add( msc_address );
}

VcsInformationExtractor::~VcsInformationExtractor()
{
}

SupplementaryServiceExtractor::SupplementaryServiceExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSupplementaryService() ),
    mmtel_sservice_type( *this, dict.avpMmtelSserviceType() ),
    service_mode( *this, dict.avpServiceMode() )
{
    add( mmtel_sservice_type );
    add( service_mode );
}

SupplementaryServiceExtractor::~SupplementaryServiceExtractor()
{
}

SupplementaryServiceExtractorList::SupplementaryServiceExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpSupplementaryService() ),
    m_dict( dict )
{
}

SupplementaryServiceExtractorList::~SupplementaryServiceExtractorList()
{
}

MmtelInformationExtractor::MmtelInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMmtelInformation() ),
    supplementary_service( *this, dict )
{
    add( supplementary_service );
}

MmtelInformationExtractor::~MmtelInformationExtractor()
{
}

LocationInfoExtractor::LocationInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLocationInfo() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    change_time( *this, dict.avpChangeTime() )
{
    add( tgpp_user_location_info );
    add( change_time );
}

LocationInfoExtractor::~LocationInfoExtractor()
{
}

LocationInfoExtractorList::LocationInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpLocationInfo() ),
    m_dict( dict )
{
}

LocationInfoExtractorList::~LocationInfoExtractorList()
{
}

CoverageInfoExtractor::CoverageInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCoverageInfo() ),
    coverage_status( *this, dict.avpCoverageStatus() ),
    change_time( *this, dict.avpChangeTime() ),
    location_info( *this, dict )
{
    add( coverage_status );
    add( change_time );
    add( location_info );
}

CoverageInfoExtractor::~CoverageInfoExtractor()
{
}

CoverageInfoExtractorList::CoverageInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpCoverageInfo() ),
    m_dict( dict )
{
}

CoverageInfoExtractorList::~CoverageInfoExtractorList()
{
}

RadioParameterSetInfoExtractor::RadioParameterSetInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRadioParameterSetInfo() ),
    radio_parameter_set_values( *this, dict.avpRadioParameterSetValues() ),
    change_time( *this, dict.avpChangeTime() )
{
    add( radio_parameter_set_values );
    add( change_time );
}

RadioParameterSetInfoExtractor::~RadioParameterSetInfoExtractor()
{
}

RadioParameterSetInfoExtractorList::RadioParameterSetInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpRadioParameterSetInfo() ),
    m_dict( dict )
{
}

RadioParameterSetInfoExtractorList::~RadioParameterSetInfoExtractorList()
{
}

TransmitterInfoExtractor::TransmitterInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTransmitterInfo() ),
    prose_source_ip_address( *this, dict.avpProseSourceIpAddress() ),
    prose_ue_id( *this, dict.avpProseUeId() )
{
    add( prose_source_ip_address );
    add( prose_ue_id );
}

TransmitterInfoExtractor::~TransmitterInfoExtractor()
{
}

TransmitterInfoExtractorList::TransmitterInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpTransmitterInfo() ),
    m_dict( dict )
{
}

TransmitterInfoExtractorList::~TransmitterInfoExtractorList()
{
}

ProseDirectCommunicationTransmissionDataContainerExtractor::ProseDirectCommunicationTransmissionDataContainerExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpProseDirectCommunicationTransmissionDataContainer() ),
    local_sequence_number( *this, dict.avpLocalSequenceNumber() ),
    coverage_status( *this, dict.avpCoverageStatus() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    accounting_output_octets( *this, dict.avpAccountingOutputOctets() ),
    change_time( *this, dict.avpChangeTime() ),
    change_condition( *this, dict.avpChangeCondition() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() ),
    usage_information_report_sequence_number( *this, dict.avpUsageInformationReportSequenceNumber() ),
    radio_resources_indicator( *this, dict.avpRadioResourcesIndicator() ),
    radio_frequency( *this, dict.avpRadioFrequency() )
{
    add( local_sequence_number );
    add( coverage_status );
    add( tgpp_user_location_info );
    add( accounting_output_octets );
    add( change_time );
    add( change_condition );
    add( visited_plmn_id );
    add( usage_information_report_sequence_number );
    add( radio_resources_indicator );
    add( radio_frequency );
}

ProseDirectCommunicationTransmissionDataContainerExtractor::~ProseDirectCommunicationTransmissionDataContainerExtractor()
{
}

ProseDirectCommunicationTransmissionDataContainerExtractorList::ProseDirectCommunicationTransmissionDataContainerExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpProseDirectCommunicationTransmissionDataContainer() ),
    m_dict( dict )
{
}

ProseDirectCommunicationTransmissionDataContainerExtractorList::~ProseDirectCommunicationTransmissionDataContainerExtractorList()
{
}

ProseDirectCommunicationReceptionDataContainerExtractor::ProseDirectCommunicationReceptionDataContainerExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpProseDirectCommunicationReceptionDataContainer() ),
    local_sequence_number( *this, dict.avpLocalSequenceNumber() ),
    coverage_status( *this, dict.avpCoverageStatus() ),
    tgpp_user_location_info( *this, dict.avp3gppUserLocationInfo() ),
    accounting_input_octets( *this, dict.avpAccountingInputOctets() ),
    change_time( *this, dict.avpChangeTime() ),
    change_condition( *this, dict.avpChangeCondition() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() ),
    usage_information_report_sequence_number( *this, dict.avpUsageInformationReportSequenceNumber() ),
    radio_resources_indicator( *this, dict.avpRadioResourcesIndicator() ),
    radio_frequency( *this, dict.avpRadioFrequency() )
{
    add( local_sequence_number );
    add( coverage_status );
    add( tgpp_user_location_info );
    add( accounting_input_octets );
    add( change_time );
    add( change_condition );
    add( visited_plmn_id );
    add( usage_information_report_sequence_number );
    add( radio_resources_indicator );
    add( radio_frequency );
}

ProseDirectCommunicationReceptionDataContainerExtractor::~ProseDirectCommunicationReceptionDataContainerExtractor()
{
}

ProseDirectCommunicationReceptionDataContainerExtractorList::ProseDirectCommunicationReceptionDataContainerExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpProseDirectCommunicationReceptionDataContainer() ),
    m_dict( dict )
{
}

ProseDirectCommunicationReceptionDataContainerExtractorList::~ProseDirectCommunicationReceptionDataContainerExtractorList()
{
}

ProseInformationExtractor::ProseInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpProseInformation() ),
    announcing_ue_hplmn_identifier( *this, dict.avpAnnouncingUeHplmnIdentifier() ),
    announcing_ue_vplmn_identifier( *this, dict.avpAnnouncingUeVplmnIdentifier() ),
    monitoring_ue_hplmn_identifier( *this, dict.avpMonitoringUeHplmnIdentifier() ),
    monitoring_ue_vplmn_identifier( *this, dict.avpMonitoringUeVplmnIdentifier() ),
    role_of_prose_function( *this, dict.avpRoleOfProseFunction() ),
    prose_app_id( *this, dict.avpProseAppId() ),
    prose_3rd_party_application_id( *this, dict.avpProse3rdPartyApplicationId() ),
    application_specific_data( *this, dict.avpApplicationSpecificData() ),
    prose_event_type( *this, dict.avpProseEventType() ),
    prose_direct_discovery_model( *this, dict.avpProseDirectDiscoveryModel() ),
    prose_function_ip_address( *this, dict.avpProseFunctionIpAddress() ),
    prose_function_id( *this, dict.avpProseFunctionId() ),
    prose_validity_timer( *this, dict.avpProseValidityTimer() ),
    prose_role_of_ue( *this, dict.avpProseRoleOfUe() ),
    prose_request_timestamp( *this, dict.avpProseRequestTimestamp() ),
    pc3_control_protocol_cause( *this, dict.avpPc3ControlProtocolCause() ),
    monitoring_ue_identifier( *this, dict.avpMonitoringUeIdentifier() ),
    requestor_plmn_identifier( *this, dict.avpRequestorPlmnIdentifier() ),
    origin_app_layer_user_id( *this, dict.avpOriginAppLayerUserId() ),
    wlan_link_layer_id( *this, dict.avpWlanLinkLayerId() ),
    requesting_epuid( *this, dict.avpRequestingEpuid() ),
    target_app_layer_user_id( *this, dict.avpTargetAppLayerUserId() ),
    requested_plmn_identifier( *this, dict.avpRequestedPlmnIdentifier() ),
    time_window( *this, dict.avpTimeWindow() ),
    prose_range_class( *this, dict.avpProseRangeClass() ),
    proximity_alert_indication( *this, dict.avpProximityAlertIndication() ),
    proximity_alert_timestamp( *this, dict.avpProximityAlertTimestamp() ),
    proximity_cancellation_timestamp( *this, dict.avpProximityCancellationTimestamp() ),
    prose_reason_for_cancellation( *this, dict.avpProseReasonForCancellation() ),
    pc3_epc_control_protocol_cause( *this, dict.avpPc3EpcControlProtocolCause() ),
    prose_ue_id( *this, dict.avpProseUeId() ),
    prose_source_ip_address( *this, dict.avpProseSourceIpAddress() ),
    layer_2_group_id( *this, dict.avpLayer2GroupId() ),
    prose_group_ip_multicast_address( *this, dict.avpProseGroupIpMulticastAddress() ),
    coverage_info( *this, dict ),
    radio_parameter_set_info( *this, dict ),
    transmitter_info( *this, dict ),
    time_first_transmission( *this, dict.avpTimeFirstTransmission() ),
    time_first_reception( *this, dict.avpTimeFirstReception() ),
    prose_direct_communication_transmission_data_container( *this, dict ),
    prose_direct_communication_reception_data_container( *this, dict )
{
    add( announcing_ue_hplmn_identifier );
    add( announcing_ue_vplmn_identifier );
    add( monitoring_ue_hplmn_identifier );
    add( monitoring_ue_vplmn_identifier );
    add( role_of_prose_function );
    add( prose_app_id );
    add( prose_3rd_party_application_id );
    add( application_specific_data );
    add( prose_event_type );
    add( prose_direct_discovery_model );
    add( prose_function_ip_address );
    add( prose_function_id );
    add( prose_validity_timer );
    add( prose_role_of_ue );
    add( prose_request_timestamp );
    add( pc3_control_protocol_cause );
    add( monitoring_ue_identifier );
    add( requestor_plmn_identifier );
    add( origin_app_layer_user_id );
    add( wlan_link_layer_id );
    add( requesting_epuid );
    add( target_app_layer_user_id );
    add( requested_plmn_identifier );
    add( time_window );
    add( prose_range_class );
    add( proximity_alert_indication );
    add( proximity_alert_timestamp );
    add( proximity_cancellation_timestamp );
    add( prose_reason_for_cancellation );
    add( pc3_epc_control_protocol_cause );
    add( prose_ue_id );
    add( prose_source_ip_address );
    add( layer_2_group_id );
    add( prose_group_ip_multicast_address );
    add( coverage_info );
    add( radio_parameter_set_info );
    add( transmitter_info );
    add( time_first_transmission );
    add( time_first_reception );
    add( prose_direct_communication_transmission_data_container );
    add( prose_direct_communication_reception_data_container );
}

ProseInformationExtractor::~ProseInformationExtractor()
{
}

CpdtInformationExtractor::CpdtInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCpdtInformation() ),
    external_identifier( *this, dict.avpExternalIdentifier() ),
    scef_id( *this, dict.avpScefId() )
{
    add( external_identifier );
    add( scef_id );
}

CpdtInformationExtractor::~CpdtInformationExtractor()
{
}

ServiceInformationExtractor::ServiceInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServiceInformation() ),
    subscription_id( *this, dict ),
    aoc_information( *this, dict ),
    ps_information( *this, dict ),
    ims_information( *this, dict ),
    mms_information( *this, dict ),
    lcs_information( *this, dict ),
    poc_information( *this, dict ),
    mbms_information( *this, dict ),
    sms_information( *this, dict ),
    vcs_information( *this, dict ),
    mmtel_information( *this, dict ),
    prose_information( *this, dict ),
    cpdt_information( *this, dict )
{
    add( subscription_id );
    add( aoc_information );
    add( ps_information );
    add( ims_information );
    add( mms_information );
    add( lcs_information );
    add( poc_information );
    add( mbms_information );
    add( sms_information );
    add( vcs_information );
    add( mmtel_information );
    add( prose_information );
    add( cpdt_information );
}

ServiceInformationExtractor::~ServiceInformationExtractor()
{
}

FailedAvpExtractor::FailedAvpExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFailedAvp() )
{

}

FailedAvpExtractor::~FailedAvpExtractor()
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

ProxyInfoExtractorList::ProxyInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpProxyInfo() ),
    m_dict( dict )
{
}

ProxyInfoExtractorList::~ProxyInfoExtractorList()
{
}

AccountingRequestExtractor::AccountingRequestExtractor( Dictionary &dict )
   : FDExtractor(),
    session_id( *this, dict.avpSessionId() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    accounting_record_type( *this, dict.avpAccountingRecordType() ),
    accounting_record_number( *this, dict.avpAccountingRecordNumber() ),
    acct_application_id( *this, dict.avpAcctApplicationId() ),
    vendor_specific_application_id( *this, dict ),
    user_name( *this, dict.avpUserName() ),
    destination_host( *this, dict.avpDestinationHost() ),
    accounting_sub_session_id( *this, dict.avpAccountingSubSessionId() ),
    acct_session_id( *this, dict.avpAcctSessionId() ),
    acct_multi_session_id( *this, dict.avpAcctMultiSessionId() ),
    acct_interim_interval( *this, dict.avpAcctInterimInterval() ),
    accounting_realtime_required( *this, dict.avpAccountingRealtimeRequired() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    event_timestamp( *this, dict.avpEventTimestamp() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() ),
    service_context_id( *this, dict.avpServiceContextId() ),
    service_information( *this, dict )
{
    add( session_id );
    add( origin_host );
    add( origin_realm );
    add( destination_realm );
    add( accounting_record_type );
    add( accounting_record_number );
    add( acct_application_id );
    add( vendor_specific_application_id );
    add( user_name );
    add( destination_host );
    add( accounting_sub_session_id );
    add( acct_session_id );
    add( acct_multi_session_id );
    add( acct_interim_interval );
    add( accounting_realtime_required );
    add( origin_state_id );
    add( event_timestamp );
    add( proxy_info );
    add( route_record );
    add( service_context_id );
    add( service_information );
}

AccountingRequestExtractor::~AccountingRequestExtractor()
{
}

AccountingAnswerExtractor::AccountingAnswerExtractor( Dictionary &dict )
   : FDExtractor(),
    session_id( *this, dict.avpSessionId() ),
    result_code( *this, dict.avpResultCode() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    accounting_record_type( *this, dict.avpAccountingRecordType() ),
    accounting_record_number( *this, dict.avpAccountingRecordNumber() ),
    acct_application_id( *this, dict.avpAcctApplicationId() ),
    vendor_specific_application_id( *this, dict ),
    user_name( *this, dict.avpUserName() ),
    accounting_sub_session_id( *this, dict.avpAccountingSubSessionId() ),
    acct_session_id( *this, dict.avpAcctSessionId() ),
    acct_multi_session_id( *this, dict.avpAcctMultiSessionId() ),
    error_message( *this, dict.avpErrorMessage() ),
    error_reporting_host( *this, dict.avpErrorReportingHost() ),
    acct_interim_interval( *this, dict.avpAcctInterimInterval() ),
    failed_avp( *this, dict ),
    accounting_realtime_required( *this, dict.avpAccountingRealtimeRequired() ),
    origin_state_id( *this, dict.avpOriginStateId() ),
    event_timestamp( *this, dict.avpEventTimestamp() ),
    proxy_info( *this, dict )
{
    add( session_id );
    add( result_code );
    add( origin_host );
    add( origin_realm );
    add( accounting_record_type );
    add( accounting_record_number );
    add( acct_application_id );
    add( vendor_specific_application_id );
    add( user_name );
    add( accounting_sub_session_id );
    add( acct_session_id );
    add( acct_multi_session_id );
    add( error_message );
    add( error_reporting_host );
    add( acct_interim_interval );
    add( failed_avp );
    add( accounting_realtime_required );
    add( origin_state_id );
    add( event_timestamp );
    add( proxy_info );
}

AccountingAnswerExtractor::~AccountingAnswerExtractor()
{
}


}
