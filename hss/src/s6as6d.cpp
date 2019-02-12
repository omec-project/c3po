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

#include "s6as6d.h"
#include "s6as6d_impl.h"

namespace s6as6d {

Dictionary::Dictionary()
    : m_app( "S6as6d"),
    m_cmd_uplr( "Update-Location-Request" ),
    m_cmd_upla( "Update-Location-Answer" ),
    m_cmd_calr( "Cancel-Location-Request" ),
    m_cmd_cala( "Cancel-Location-Answer" ),
    m_cmd_auir( "Authentication-Information-Request" ),
    m_cmd_auia( "Authentication-Information-Answer" ),
    m_cmd_insdr( "Insert-Subscriber-Data-Request" ),
    m_cmd_insda( "Insert-Subscriber-Data-Answer" ),
    m_cmd_desdr( "Delete-Subscriber-Data-Request" ),
    m_cmd_desda( "Delete-Subscriber-Data-Answer" ),
    m_cmd_puur( "Purge-UE-Request" ),
    m_cmd_puua( "Purge-UE-Answer" ),
    m_cmd_rer( "Reset-Request" ),
    m_cmd_rea( "Reset-Answer" ),
    m_vnd_3gpp( "3GPP" ),
    m_avp_ts_code( "TS-Code", m_vnd_3gpp.getId() ),
    m_avp_reachability_type( "Reachability-Type", m_vnd_3gpp.getId() ),
    m_avp_eps_user_state( "EPS-User-State", m_vnd_3gpp.getId() ),
    m_avp_pre_emption_capability( "Pre-emption-Capability", m_vnd_3gpp.getId() ),
    m_avp_feature_list_id( "Feature-List-ID", m_vnd_3gpp.getId() ),
    m_avp_service_area_identity( "Service-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_day_of_week_mask( "Day-Of-Week-Mask" ),
    m_avp_all_apn_configurations_included_indicator( "All-APN-Configurations-Included-Indicator", m_vnd_3gpp.getId() ),
    m_avp_e_utran_vector( "E-UTRAN-Vector", m_vnd_3gpp.getId() ),
    m_avp_pua_flags( "PUA-Flags", m_vnd_3gpp.getId() ),
    m_avp_maximum_response_time( "Maximum-Response-Time", m_vnd_3gpp.getId() ),
    m_avp_vendor_specific_application_id( "Vendor-Specific-Application-Id" ),
    m_avp_csg_access_mode( "CSG-Access-Mode", m_vnd_3gpp.getId() ),
    m_avp_additional_context_identifier( "Additional-Context-Identifier", m_vnd_3gpp.getId() ),
    m_avp_access_restriction_data( "Access-Restriction-Data", m_vnd_3gpp.getId() ),
    m_avp_mip6_home_link_prefix( "MIP6-Home-Link-Prefix" ),
    m_avp_gprs_subscription_data( "GPRS-Subscription-Data", m_vnd_3gpp.getId() ),
    m_avp_sres( "SRES", m_vnd_3gpp.getId() ),
    m_avp_sourceid( "SourceID" ),
    m_avp_load_value( "Load-Value" ),
    m_avp_feature_list( "Feature-List", m_vnd_3gpp.getId() ),
    m_avp_apn_oi_replacement( "APN-OI-Replacement", m_vnd_3gpp.getId() ),
    m_avp_monitoring_event_config_status( "Monitoring-Event-Config-Status", m_vnd_3gpp.getId() ),
    m_avp_oc_report_type( "OC-Report-Type" ),
    m_avp_experimental_result( "Experimental-Result" ),
    m_avp_edrx_cycle_length( "eDRX-Cycle-Length", m_vnd_3gpp.getId() ),
    m_avp_ims_voice_over_ps_sessions_supported( "IMS-Voice-Over-PS-Sessions-Supported", m_vnd_3gpp.getId() ),
    m_avp_ue_usage_type( "UE-Usage-Type", m_vnd_3gpp.getId() ),
    m_avp_user_id( "User-Id", m_vnd_3gpp.getId() ),
    m_avp_lcs_privacyexception( "LCS-PrivacyException", m_vnd_3gpp.getId() ),
    m_avp_v2x_subscription_data( "V2X-Subscription-Data", m_vnd_3gpp.getId() ),
    m_avp_adjacent_access_restriction_data( "Adjacent-Access-Restriction-Data", m_vnd_3gpp.getId() ),
    m_avp_qos_class_identifier( "QoS-Class-Identifier", m_vnd_3gpp.getId() ),
    m_avp_ext_pdp_type( "Ext-PDP-Type", m_vnd_3gpp.getId() ),
    m_avp_oc_feature_vector( "OC-Feature-Vector" ),
    m_avp_relay_node_indicator( "Relay-Node-Indicator", m_vnd_3gpp.getId() ),
    m_avp_communication_duration_time( "Communication-Duration-Time", m_vnd_3gpp.getId() ),
    m_avp_acct_application_id( "Acct-Application-Id" ),
    m_avp_item_number( "Item-Number", m_vnd_3gpp.getId() ),
    m_avp_list_of_measurements( "List-Of-Measurements", m_vnd_3gpp.getId() ),
    m_avp_geran_vector( "GERAN-Vector", m_vnd_3gpp.getId() ),
    m_avp_auth_session_state( "Auth-Session-State" ),
    m_avp_eps_location_information( "EPS-Location-Information", m_vnd_3gpp.getId() ),
    m_avp_drmp( "DRMP" ),
    m_avp_trace_ne_type_list( "Trace-NE-Type-List", m_vnd_3gpp.getId() ),
    m_avp_adjacent_plmns( "Adjacent-PLMNs", m_vnd_3gpp.getId() ),
    m_avp_priority_level( "Priority-Level", m_vnd_3gpp.getId() ),
    m_avp_pre_emption_vulnerability( "Pre-emption-Vulnerability", m_vnd_3gpp.getId() ),
    m_avp_geodetic_information( "Geodetic-Information", m_vnd_3gpp.getId() ),
    m_avp_load_type( "Load-Type" ),
    m_avp_area_scope( "Area-Scope", m_vnd_3gpp.getId() ),
    m_avp_preferred_data_mode( "Preferred-Data-Mode", m_vnd_3gpp.getId() ),
    m_avp_imei( "IMEI", m_vnd_3gpp.getId() ),
    m_avp_reachability_information( "Reachability-Information", m_vnd_3gpp.getId() ),
    m_avp_node_type( "Node-Type", m_vnd_3gpp.getId() ),
    m_avp_clr_flags( "CLR-Flags", m_vnd_3gpp.getId() ),
    m_avp_call_barring_info( "Call-Barring-Info", m_vnd_3gpp.getId() ),
    m_avp_service_report( "Service-Report", m_vnd_3gpp.getId() ),
    m_avp_destination_host( "Destination-Host" ),
    m_avp_gmlc_address( "GMLC-Address", m_vnd_3gpp.getId() ),
    m_avp_location_area_identity( "Location-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_logging_interval( "Logging-Interval", m_vnd_3gpp.getId() ),
    m_avp_roaming_restricted_due_to_unsupported_feature( "Roaming-Restricted-Due-To-Unsupported-Feature", m_vnd_3gpp.getId() ),
    m_avp_rat_type( "RAT-Type", m_vnd_3gpp.getId() ),
    m_avp_edrx_cycle_length_value( "eDRX-Cycle-Length-Value", m_vnd_3gpp.getId() ),
    m_avp_time_zone( "Time-Zone", m_vnd_3gpp.getId() ),
    m_avp_msisdn( "MSISDN", m_vnd_3gpp.getId() ),
    m_avp_mo_lr( "MO-LR", m_vnd_3gpp.getId() ),
    m_avp_active_apn( "Active-APN", m_vnd_3gpp.getId() ),
    m_avp_job_type( "Job-Type", m_vnd_3gpp.getId() ),
    m_avp_ss_code( "SS-Code", m_vnd_3gpp.getId() ),
    m_avp_time_of_day_start( "Time-Of-Day-Start" ),
    m_avp_wlan_offloadability( "WLAN-offloadability", m_vnd_3gpp.getId() ),
    m_avp_mip_home_agent_host( "MIP-Home-Agent-Host" ),
    m_avp_pdn_type( "PDN-Type", m_vnd_3gpp.getId() ),
    m_avp_communication_pattern_set( "Communication-Pattern-Set", m_vnd_3gpp.getId() ),
    m_avp_periodic_communication_indicator( "Periodic-Communication-Indicator", m_vnd_3gpp.getId() ),
    m_avp_prose_permission( "ProSe-Permission", m_vnd_3gpp.getId() ),
    m_avp_equivalent_plmn_list( "Equivalent-PLMN-List", m_vnd_3gpp.getId() ),
    m_avp_charged_party( "Charged-Party", m_vnd_3gpp.getId() ),
    m_avp_supported_services( "Supported-Services", m_vnd_3gpp.getId() ),
    m_avp_3gpp_charging_characteristics( "3GPP-Charging-Characteristics", m_vnd_3gpp.getId() ),
    m_avp_proxy_info( "Proxy-Info" ),
    m_avp_subscribed_periodic_rau_tau_timer( "Subscribed-Periodic-RAU-TAU-Timer", m_vnd_3gpp.getId() ),
    m_avp_ss_status( "SS-Status", m_vnd_3gpp.getId() ),
    m_avp_enodeb_id( "eNodeB-Id", m_vnd_3gpp.getId() ),
    m_avp_utran_vector( "UTRAN-Vector", m_vnd_3gpp.getId() ),
    m_avp_origin_realm( "Origin-Realm" ),
    m_avp_origin_host( "Origin-Host" ),
    m_avp_event_threshold_rsrq( "Event-Threshold-RSRQ", m_vnd_3gpp.getId() ),
    m_avp_vplmn_dynamic_address_allowed( "VPLMN-Dynamic-Address-Allowed", m_vnd_3gpp.getId() ),
    m_avp_accuracy( "Accuracy", m_vnd_3gpp.getId() ),
    m_avp_pur_flags( "PUR-Flags", m_vnd_3gpp.getId() ),
    m_avp_ue_reachability_configuration( "UE-Reachability-Configuration", m_vnd_3gpp.getId() ),
    m_avp_apn_configuration( "APN-Configuration", m_vnd_3gpp.getId() ),
    m_avp_a_msisdn( "A-MSISDN", m_vnd_3gpp.getId() ),
    m_avp_monte_location_type( "MONTE-Location-Type", m_vnd_3gpp.getId() ),
    m_avp_last_ue_activity_time( "Last-UE-Activity-Time", m_vnd_3gpp.getId() ),
    m_avp_local_time_zone( "Local-Time-Zone", m_vnd_3gpp.getId() ),
    m_avp_authentication_info( "Authentication-Info", m_vnd_3gpp.getId() ),
    m_avp_service_type( "Service-Type", m_vnd_3gpp.getId() ),
    m_avp_v2x_permission( "V2X-Permission", m_vnd_3gpp.getId() ),
    m_avp_pdp_address( "PDP-Address", m_vnd_3gpp.getId() ),
    m_avp_imsi_group_id( "IMSI-Group-Id", m_vnd_3gpp.getId() ),
    m_avp_dsr_flags( "DSR-Flags", m_vnd_3gpp.getId() ),
    m_avp_complete_data_list_included_indicator( "Complete-Data-List-Included-Indicator", m_vnd_3gpp.getId() ),
    m_avp_trace_collection_entity( "Trace-Collection-Entity", m_vnd_3gpp.getId() ),
    m_avp_wlan_offloadability_eutran( "WLAN-offloadability-EUTRAN", m_vnd_3gpp.getId() ),
    m_avp_route_record( "Route-Record" ),
    m_avp_subscription_data( "Subscription-Data", m_vnd_3gpp.getId() ),
    m_avp_reference_id_validity_time( "Reference-ID-Validity-Time", m_vnd_3gpp.getId() ),
    m_avp_dl_buffering_suggested_packet_count( "DL-Buffering-Suggested-Packet-Count", m_vnd_3gpp.getId() ),
    m_avp_scef_id( "SCEF-ID", m_vnd_3gpp.getId() ),
    m_avp_trace_event_list( "Trace-Event-List", m_vnd_3gpp.getId() ),
    m_avp_csg_id( "CSG-Id", m_vnd_3gpp.getId() ),
    m_avp_mdt_configuration( "MDT-Configuration", m_vnd_3gpp.getId() ),
    m_avp_error_diagnostic( "Error-Diagnostic", m_vnd_3gpp.getId() ),
    m_avp_routing_area_identity( "Routing-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_current_location_retrieved( "Current-Location-Retrieved", m_vnd_3gpp.getId() ),
    m_avp_user_csg_information( "User-CSG-Information", m_vnd_3gpp.getId() ),
    m_avp_mip6_agent_info( "MIP6-Agent-Info", m_vnd_3gpp.getId() ),
    m_avp_logging_duration( "Logging-Duration", m_vnd_3gpp.getId() ),
    m_avp_group_plmn_id( "Group-PLMN-Id", m_vnd_3gpp.getId() ),
    m_avp_collection_period_rrm_umts( "Collection-Period-RRM-UMTS", m_vnd_3gpp.getId() ),
    m_avp_ida_flags( "IDA-Flags", m_vnd_3gpp.getId() ),
    m_avp_non_ip_pdn_type_indicator( "Non-IP-PDN-Type-Indicator", m_vnd_3gpp.getId() ),
    m_avp_tracking_area_identity( "Tracking-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_oc_olr( "OC-OLR" ),
    m_avp_client_identity( "Client-Identity", m_vnd_3gpp.getId() ),
    m_avp_age_of_location_information( "Age-Of-Location-Information", m_vnd_3gpp.getId() ),
    m_avp_trace_data( "Trace-Data", m_vnd_3gpp.getId() ),
    m_avp_subscribed_vsrvcc( "Subscribed-VSRVCC", m_vnd_3gpp.getId() ),
    m_avp_plmn_client( "PLMN-Client", m_vnd_3gpp.getId() ),
    m_avp_vplmn_lipa_allowed( "VPLMN-LIPA-Allowed", m_vnd_3gpp.getId() ),
    m_avp_re_synchronization_info( "Re-Synchronization-Info", m_vnd_3gpp.getId() ),
    m_avp_air_flags( "AIR-Flags", m_vnd_3gpp.getId() ),
    m_avp_specific_apn_info( "Specific-APN-Info", m_vnd_3gpp.getId() ),
    m_avp_rand( "RAND", m_vnd_3gpp.getId() ),
    m_avp_oc_reduction_percentage( "OC-Reduction-Percentage" ),
    m_avp_report_interval( "Report-Interval", m_vnd_3gpp.getId() ),
    m_avp_eps_subscribed_qos_profile( "EPS-Subscribed-QoS-Profile", m_vnd_3gpp.getId() ),
    m_avp_service_result_code( "Service-Result-Code", m_vnd_3gpp.getId() ),
    m_avp_omc_id( "OMC-Id", m_vnd_3gpp.getId() ),
    m_avp_oc_sequence_number( "OC-Sequence-Number" ),
    m_avp_teleservice_list( "Teleservice-List", m_vnd_3gpp.getId() ),
    m_avp_apn_configuration_profile( "APN-Configuration-Profile", m_vnd_3gpp.getId() ),
    m_avp_event_threshold_event_1i( "Event-Threshold-Event-1I", m_vnd_3gpp.getId() ),
    m_avp_supported_monitoring_events( "Supported-Monitoring-Events", m_vnd_3gpp.getId() ),
    m_avp_dsa_flags( "DSA-Flags", m_vnd_3gpp.getId() ),
    m_avp_event_threshold_event_1f( "Event-Threshold-Event-1F", m_vnd_3gpp.getId() ),
    m_avp_allocation_retention_priority( "Allocation-Retention-Priority", m_vnd_3gpp.getId() ),
    m_avp_cell_global_identity( "Cell-Global-Identity", m_vnd_3gpp.getId() ),
    m_avp_oc_validity_duration( "OC-Validity-Duration" ),
    m_avp_ula_flags( "ULA-Flags", m_vnd_3gpp.getId() ),
    m_avp_csg_membership_indication( "CSG-Membership-Indication", m_vnd_3gpp.getId() ),
    m_avp_service_selection( "Service-Selection" ),
    m_avp_network_access_mode( "Network-Access-Mode", m_vnd_3gpp.getId() ),
    m_avp_3gpp2_meid( "3GPP2-MEID", m_vnd_3gpp.getId() ),
    m_avp_mme_location_information( "MME-Location-Information", m_vnd_3gpp.getId() ),
    m_avp_sgsn_location_information( "SGSN-Location-Information", m_vnd_3gpp.getId() ),
    m_avp_ulr_flags( "ULR-Flags", m_vnd_3gpp.getId() ),
    m_avp_measurement_period_lte( "Measurement-Period-LTE", m_vnd_3gpp.getId() ),
    m_avp_result_code( "Result-Code" ),
    m_avp_gmlc_restriction( "GMLC-Restriction", m_vnd_3gpp.getId() ),
    m_avp_user_state( "User-State", m_vnd_3gpp.getId() ),
    m_avp_sgs_mme_identity( "SGs-MME-Identity", m_vnd_3gpp.getId() ),
    m_avp_coupled_node_diameter_id( "Coupled-Node-Diameter-ID", m_vnd_3gpp.getId() ),
    m_avp_kc( "Kc", m_vnd_3gpp.getId() ),
    m_avp_wlan_offloadability_utran( "WLAN-offloadability-UTRAN", m_vnd_3gpp.getId() ),
    m_avp_servicetypeidentity( "ServiceTypeIdentity", m_vnd_3gpp.getId() ),
    m_avp_stn_sr( "STN-SR", m_vnd_3gpp.getId() ),
    m_avp_mme_number_for_mt_sms( "MME-Number-for-MT-SMS", m_vnd_3gpp.getId() ),
    m_avp_requested_utran_geran_authentication_info( "Requested-UTRAN-GERAN-Authentication-Info", m_vnd_3gpp.getId() ),
    m_avp_notification_to_ue_user( "Notification-To-UE-User", m_vnd_3gpp.getId() ),
    m_avp_homogeneous_support_of_ims_voice_over_ps_sessions( "Homogeneous-Support-of-IMS-Voice-Over-PS-Sessions", m_vnd_3gpp.getId() ),
    m_avp_pdp_context( "PDP-Context", m_vnd_3gpp.getId() ),
    m_avp_max_requested_bandwidth_dl( "Max-Requested-Bandwidth-DL", m_vnd_3gpp.getId() ),
    m_avp_trace_interface_list( "Trace-Interface-List", m_vnd_3gpp.getId() ),
    m_avp_pdn_connection_continuity( "PDN-Connection-Continuity", m_vnd_3gpp.getId() ),
    m_avp_scef_reference_id( "SCEF-Reference-ID", m_vnd_3gpp.getId() ),
    m_avp_sgsn_number( "SGSN-Number", m_vnd_3gpp.getId() ),
    m_avp_rat_frequency_selection_priority_id( "RAT-Frequency-Selection-Priority-ID", m_vnd_3gpp.getId() ),
    m_avp_number_of_requested_vectors( "Number-Of-Requested-Vectors", m_vnd_3gpp.getId() ),
    m_avp_subscription_data_deletion( "Subscription-Data-Deletion", m_vnd_3gpp.getId() ),
    m_avp_load( "Load" ),
    m_avp_context_identifier( "Context-Identifier", m_vnd_3gpp.getId() ),
    m_avp_integrity_key( "Integrity-Key", m_vnd_3gpp.getId() ),
    m_avp_confidentiality_key( "Confidentiality-Key", m_vnd_3gpp.getId() ),
    m_avp_e_utran_cell_global_identity( "E-UTRAN-Cell-Global-Identity", m_vnd_3gpp.getId() ),
    m_avp_oc_supported_features( "OC-Supported-Features" ),
    m_avp_monitoring_type( "Monitoring-Type", m_vnd_3gpp.getId() ),
    m_avp_ue_pc5_ambr( "UE-PC5-AMBR", m_vnd_3gpp.getId() ),
    m_avp_proxy_host( "Proxy-Host" ),
    m_avp_event_threshold_rsrp( "Event-Threshold-RSRP", m_vnd_3gpp.getId() ),
    m_avp_terminal_information( "Terminal-Information", m_vnd_3gpp.getId() ),
    m_avp_xres( "XRES", m_vnd_3gpp.getId() ),
    m_avp_subscriber_status( "Subscriber-Status", m_vnd_3gpp.getId() ),
    m_avp_experimental_result_code( "Experimental-Result-Code" ),
    m_avp_collection_period_rrm_lte( "Collection-Period-RRM-LTE", m_vnd_3gpp.getId() ),
    m_avp_maximum_number_of_reports( "Maximum-Number-of-Reports", m_vnd_3gpp.getId() ),
    m_avp_hplmn_odb( "HPLMN-ODB", m_vnd_3gpp.getId() ),
    m_avp_mme_user_state( "MME-User-State", m_vnd_3gpp.getId() ),
    m_avp_auth_application_id( "Auth-Application-Id" ),
    m_avp_reset_id( "Reset-ID", m_vnd_3gpp.getId() ),
    m_avp_lipa_permission( "LIPA-Permission", m_vnd_3gpp.getId() ),
    m_avp_requested_eutran_authentication_info( "Requested-EUTRAN-Authentication-Info", m_vnd_3gpp.getId() ),
    m_avp_mps_priority( "MPS-Priority", m_vnd_3gpp.getId() ),
    m_avp_monitoring_event_report( "Monitoring-Event-Report", m_vnd_3gpp.getId() ),
    m_avp_extended_enodeb_id( "Extended-eNodeB-Id", m_vnd_3gpp.getId() ),
    m_avp_idr_flags( "IDR-Flags", m_vnd_3gpp.getId() ),
    m_avp_aese_communication_pattern( "AESE-Communication-Pattern", m_vnd_3gpp.getId() ),
    m_avp_visited_network_identifier( "Visited-Network-Identifier", m_vnd_3gpp.getId() ),
    m_avp_lcs_info( "LCS-Info", m_vnd_3gpp.getId() ),
    m_avp_restoration_priority( "Restoration-Priority", m_vnd_3gpp.getId() ),
    m_avp_mdt_user_consent( "MDT-User-Consent", m_vnd_3gpp.getId() ),
    m_avp_cancellation_type( "Cancellation-Type", m_vnd_3gpp.getId() ),
    m_avp_stationary_indication( "Stationary-Indication", m_vnd_3gpp.getId() ),
    m_avp_pdn_gw_allocation_type( "PDN-GW-Allocation-Type", m_vnd_3gpp.getId() ),
    m_avp_gmlc_number( "GMLC-Number", m_vnd_3gpp.getId() ),
    m_avp_subscription_data_flags( "Subscription-Data-Flags", m_vnd_3gpp.getId() ),
    m_avp_ics_indicator( "ICS-Indicator", m_vnd_3gpp.getId() ),
    m_avp_visited_plmn_id( "Visited-PLMN-Id", m_vnd_3gpp.getId() ),
    m_avp_qos_subscribed( "QoS-Subscribed", m_vnd_3gpp.getId() ),
    m_avp_positioning_method( "Positioning-Method", m_vnd_3gpp.getId() ),
    m_avp_monitoring_duration( "Monitoring-Duration", m_vnd_3gpp.getId() ),
    m_avp_reporting_trigger( "Reporting-Trigger", m_vnd_3gpp.getId() ),
    m_avp_failed_avp( "Failed-AVP" ),
    m_avp_ambr( "AMBR", m_vnd_3gpp.getId() ),
    m_avp_daylight_saving_time( "Daylight-Saving-Time", m_vnd_3gpp.getId() ),
    m_avp_expiration_date( "Expiration-Date", m_vnd_3gpp.getId() ),
    m_avp_group_service_id( "Group-Service-Id", m_vnd_3gpp.getId() ),
    m_avp_trace_reference( "Trace-Reference", m_vnd_3gpp.getId() ),
    m_avp_local_group_id( "Local-Group-Id", m_vnd_3gpp.getId() ),
    m_avp_measurement_quantity( "Measurement-Quantity", m_vnd_3gpp.getId() ),
    m_avp_geographical_information( "Geographical-Information", m_vnd_3gpp.getId() ),
    m_avp_kasme( "KASME", m_vnd_3gpp.getId() ),
    m_avp_served_party_ip_address( "Served-Party-IP-Address", m_vnd_3gpp.getId() ),
    m_avp_trace_depth( "Trace-Depth", m_vnd_3gpp.getId() ),
    m_avp_scheduled_communication_time( "Scheduled-Communication-Time", m_vnd_3gpp.getId() ),
    m_avp_proxy_state( "Proxy-State" ),
    m_avp_operator_determined_barring( "Operator-Determined-Barring", m_vnd_3gpp.getId() ),
    m_avp_autn( "AUTN", m_vnd_3gpp.getId() ),
    m_avp_pdp_type( "PDP-Type", m_vnd_3gpp.getId() ),
    m_avp_periodic_time( "Periodic-Time", m_vnd_3gpp.getId() ),
    m_avp_time_of_day_end( "Time-Of-Day-End" ),
    m_avp_software_version( "Software-Version", m_vnd_3gpp.getId() ),
    m_avp_sipto_local_network_permission( "SIPTO-Local-Network-Permission", m_vnd_3gpp.getId() ),
    m_avp_sms_register_request( "SMS-Register-Request", m_vnd_3gpp.getId() ),
    m_avp_sgsn_user_state( "SGSN-User-State", m_vnd_3gpp.getId() ),
    m_avp_vendor_id( "Vendor-Id" ),
    m_avp_max_requested_bandwidth_ul( "Max-Requested-Bandwidth-UL", m_vnd_3gpp.getId() ),
    m_avp_supported_features( "Supported-Features", m_vnd_3gpp.getId() ),
    m_avp_non_ip_data_delivery_mechanism( "Non-IP-Data-Delivery-Mechanism", m_vnd_3gpp.getId() ),
    m_avp_monitoring_event_configuration( "Monitoring-Event-Configuration", m_vnd_3gpp.getId() ),
    m_avp_destination_realm( "Destination-Realm" ),
    m_avp_session_id( "Session-Id" ),
    m_avp_immediate_response_preferred( "Immediate-Response-Preferred", m_vnd_3gpp.getId() ),
    m_avp_user_name( "User-Name" ),
    m_avp_scef_realm( "SCEF-Realm", m_vnd_3gpp.getId() ),
    m_avp_sipto_permission( "SIPTO-Permission", m_vnd_3gpp.getId() ),
    m_avp_emergency_info( "Emergency-Info", m_vnd_3gpp.getId() ),
    m_avp_mdt_allowed_plmn_id( "MDT-Allowed-PLMN-Id", m_vnd_3gpp.getId() ),
    m_avp_report_amount( "Report-Amount", m_vnd_3gpp.getId() ),
    m_avp_csg_subscription_data( "CSG-Subscription-Data", m_vnd_3gpp.getId() ),
    m_avp_external_client( "External-Client", m_vnd_3gpp.getId() ),
    m_avp_mip_home_agent_address( "MIP-Home-Agent-Address" ),
    m_avp_ue_srvcc_capability( "UE-SRVCC-Capability", m_vnd_3gpp.getId() ),
    m_avp_measurement_period_umts( "Measurement-Period-UMTS", m_vnd_3gpp.getId() ),
    m_avp_scef_reference_id_for_deletion( "SCEF-Reference-ID-for-Deletion", m_vnd_3gpp.getId() ),
    m_avp_prose_subscription_data( "ProSe-Subscription-Data", m_vnd_3gpp.getId() ),
    m_avp_service_result( "Service-Result", m_vnd_3gpp.getId() ),
    m_avp_location_information_configuration( "Location-Information-Configuration", m_vnd_3gpp.getId() ),
    m_avp_ext_pdp_address( "Ext-PDP-Address", m_vnd_3gpp.getId() ),
    m_avp_regional_subscription_zone_code( "Regional-Subscription-Zone-Code", m_vnd_3gpp.getId() )
{
    std::cout << "Registering s6as6d dictionary" << std::endl;
};

Dictionary::~Dictionary()
{
};

UPLRcmd::UPLRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdUPLR() ),
      m_app( app )
{
}

UPLRcmd::~UPLRcmd()
{
}

UPLRreq::UPLRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdUPLR() ),
      m_app(app)
{
}

UPLRreq::~UPLRreq()
{
}

 
CALRcmd::CALRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdCALR() ),
      m_app( app )
{
}

CALRcmd::~CALRcmd()
{
}

CALRreq::CALRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdCALR() ),
      m_app(app)
{
}

CALRreq::~CALRreq()
{
}

 
AUIRcmd::AUIRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdAUIR() ),
      m_app( app )
{
}

AUIRcmd::~AUIRcmd()
{
}

AUIRreq::AUIRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdAUIR() ),
      m_app(app)
{
}

AUIRreq::~AUIRreq()
{
}

 
INSDRcmd::INSDRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdINSDR() ),
      m_app( app )
{
}

INSDRcmd::~INSDRcmd()
{
}

INSDRreq::INSDRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdINSDR() ),
      m_app(app)
{
}

INSDRreq::~INSDRreq()
{
}

 
DESDRcmd::DESDRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdDESDR() ),
      m_app( app )
{
}

DESDRcmd::~DESDRcmd()
{
}

DESDRreq::DESDRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdDESDR() ),
      m_app(app)
{
}

DESDRreq::~DESDRreq()
{
}

 
PUURcmd::PUURcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdPUUR() ),
      m_app( app )
{
}

PUURcmd::~PUURcmd()
{
}

PUURreq::PUURreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdPUUR() ),
      m_app(app)
{
}

PUURreq::~PUURreq()
{
}

 
RERcmd::RERcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdRER() ),
      m_app( app )
{
}

RERcmd::~RERcmd()
{
}

RERreq::RERreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdRER() ),
      m_app(app)
{
}

RERreq::~RERreq()
{
}

 
ApplicationBase::ApplicationBase()
{
    setDictionaryEntry( &m_dict.app() );

};

ApplicationBase::~ApplicationBase()
{
};



MmeUserStateExtractor::MmeUserStateExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMmeUserState() ),
    user_state( *this, dict.avpUserState() )
{
    add( user_state );
}

MmeUserStateExtractor::~MmeUserStateExtractor()
{
}

SgsnUserStateExtractor::SgsnUserStateExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSgsnUserState() ),
    user_state( *this, dict.avpUserState() )
{
    add( user_state );
}

SgsnUserStateExtractor::~SgsnUserStateExtractor()
{
}

EpsUserStateExtractor::EpsUserStateExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEpsUserState() ),
    mme_user_state( *this, dict ),
    sgsn_user_state( *this, dict )
{
    add( mme_user_state );
    add( sgsn_user_state );
}

EpsUserStateExtractor::~EpsUserStateExtractor()
{
}

EUtranVectorExtractor::EUtranVectorExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEUtranVector() ),
    item_number( *this, dict.avpItemNumber() ),
    rand( *this, dict.avpRand() ),
    xres( *this, dict.avpXres() ),
    autn( *this, dict.avpAutn() ),
    kasme( *this, dict.avpKasme() )
{
    add( item_number );
    add( rand );
    add( xres );
    add( autn );
    add( kasme );
}

EUtranVectorExtractor::~EUtranVectorExtractor()
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

AmbrExtractor::AmbrExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAmbr() ),
    max_requested_bandwidth_ul( *this, dict.avpMaxRequestedBandwidthUl() ),
    max_requested_bandwidth_dl( *this, dict.avpMaxRequestedBandwidthDl() )
{
    add( max_requested_bandwidth_ul );
    add( max_requested_bandwidth_dl );
}

AmbrExtractor::~AmbrExtractor()
{
}

PdpContextExtractor::PdpContextExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpPdpContext() ),
    context_identifier( *this, dict.avpContextIdentifier() ),
    pdp_type( *this, dict.avpPdpType() ),
    pdp_address( *this, dict.avpPdpAddress() ),
    qos_subscribed( *this, dict.avpQosSubscribed() ),
    vplmn_dynamic_address_allowed( *this, dict.avpVplmnDynamicAddressAllowed() ),
    service_selection( *this, dict.avpServiceSelection() ),
    tgpp_charging_characteristics( *this, dict.avp3gppChargingCharacteristics() ),
    ext_pdp_type( *this, dict.avpExtPdpType() ),
    ext_pdp_address( *this, dict.avpExtPdpAddress() ),
    ambr( *this, dict ),
    apn_oi_replacement( *this, dict.avpApnOiReplacement() ),
    sipto_permission( *this, dict.avpSiptoPermission() ),
    lipa_permission( *this, dict.avpLipaPermission() ),
    restoration_priority( *this, dict.avpRestorationPriority() ),
    sipto_local_network_permission( *this, dict.avpSiptoLocalNetworkPermission() ),
    non_ip_data_delivery_mechanism( *this, dict.avpNonIpDataDeliveryMechanism() ),
    scef_id( *this, dict.avpScefId() )
{
    add( context_identifier );
    add( pdp_type );
    add( pdp_address );
    add( qos_subscribed );
    add( vplmn_dynamic_address_allowed );
    add( service_selection );
    add( tgpp_charging_characteristics );
    add( ext_pdp_type );
    add( ext_pdp_address );
    add( ambr );
    add( apn_oi_replacement );
    add( sipto_permission );
    add( lipa_permission );
    add( restoration_priority );
    add( sipto_local_network_permission );
    add( non_ip_data_delivery_mechanism );
    add( scef_id );
}

PdpContextExtractor::~PdpContextExtractor()
{
}

PdpContextExtractorList::PdpContextExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpPdpContext() ),
    m_dict( dict )
{
}

PdpContextExtractorList::~PdpContextExtractorList()
{
}

GprsSubscriptionDataExtractor::GprsSubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpGprsSubscriptionData() ),
    complete_data_list_included_indicator( *this, dict.avpCompleteDataListIncludedIndicator() ),
    pdp_context( *this, dict )
{
    add( complete_data_list_included_indicator );
    add( pdp_context );
}

GprsSubscriptionDataExtractor::~GprsSubscriptionDataExtractor()
{
}

ServiceResultExtractor::ServiceResultExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServiceResult() ),
    vendor_id( *this, dict.avpVendorId() ),
    service_result_code( *this, dict.avpServiceResultCode() )
{
    add( vendor_id );
    add( service_result_code );
}

ServiceResultExtractor::~ServiceResultExtractor()
{
}

ServiceReportExtractor::ServiceReportExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServiceReport() ),
    service_result( *this, dict ),
    node_type( *this, dict.avpNodeType() )
{
    add( service_result );
    add( node_type );
}

ServiceReportExtractor::~ServiceReportExtractor()
{
}

ServiceReportExtractorList::ServiceReportExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpServiceReport() ),
    m_dict( dict )
{
}

ServiceReportExtractorList::~ServiceReportExtractorList()
{
}

MonitoringEventConfigStatusExtractor::MonitoringEventConfigStatusExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMonitoringEventConfigStatus() ),
    service_report( *this, dict ),
    scef_reference_id( *this, dict.avpScefReferenceId() ),
    scef_id( *this, dict.avpScefId() )
{
    add( service_report );
    add( scef_reference_id );
    add( scef_id );
}

MonitoringEventConfigStatusExtractor::~MonitoringEventConfigStatusExtractor()
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

EdrxCycleLengthExtractor::EdrxCycleLengthExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEdrxCycleLength() ),
    rat_type( *this, dict.avpRatType() ),
    edrx_cycle_length_value( *this, dict.avpEdrxCycleLengthValue() )
{
    add( rat_type );
    add( edrx_cycle_length_value );
}

EdrxCycleLengthExtractor::~EdrxCycleLengthExtractor()
{
}

ExternalClientExtractor::ExternalClientExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpExternalClient() ),
    client_identity( *this, dict.avpClientIdentity() ),
    gmlc_restriction( *this, dict.avpGmlcRestriction() ),
    notification_to_ue_user( *this, dict.avpNotificationToUeUser() )
{
    add( client_identity );
    add( gmlc_restriction );
    add( notification_to_ue_user );
}

ExternalClientExtractor::~ExternalClientExtractor()
{
}

ExternalClientExtractorList::ExternalClientExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpExternalClient() ),
    m_dict( dict )
{
}

ExternalClientExtractorList::~ExternalClientExtractorList()
{
}

ServiceTypeExtractor::ServiceTypeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpServiceType() ),
    servicetypeidentity( *this, dict.avpServicetypeidentity() ),
    gmlc_restriction( *this, dict.avpGmlcRestriction() ),
    notification_to_ue_user( *this, dict.avpNotificationToUeUser() )
{
    add( servicetypeidentity );
    add( gmlc_restriction );
    add( notification_to_ue_user );
}

ServiceTypeExtractor::~ServiceTypeExtractor()
{
}

ServiceTypeExtractorList::ServiceTypeExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpServiceType() ),
    m_dict( dict )
{
}

ServiceTypeExtractorList::~ServiceTypeExtractorList()
{
}

LcsPrivacyexceptionExtractor::LcsPrivacyexceptionExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLcsPrivacyexception() ),
    ss_code( *this, dict.avpSsCode() ),
    ss_status( *this, dict.avpSsStatus() ),
    notification_to_ue_user( *this, dict.avpNotificationToUeUser() ),
    external_client( *this, dict ),
    plmn_client( *this, dict.avpPlmnClient() ),
    service_type( *this, dict )
{
    add( ss_code );
    add( ss_status );
    add( notification_to_ue_user );
    add( external_client );
    add( plmn_client );
    add( service_type );
}

LcsPrivacyexceptionExtractor::~LcsPrivacyexceptionExtractor()
{
}

V2xSubscriptionDataExtractor::V2xSubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpV2xSubscriptionData() ),
    v2x_permission( *this, dict.avpV2xPermission() ),
    ue_pc5_ambr( *this, dict.avpUePc5Ambr() )
{
    add( v2x_permission );
    add( ue_pc5_ambr );
}

V2xSubscriptionDataExtractor::~V2xSubscriptionDataExtractor()
{
}

AdjacentAccessRestrictionDataExtractor::AdjacentAccessRestrictionDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAdjacentAccessRestrictionData() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() ),
    access_restriction_data( *this, dict.avpAccessRestrictionData() )
{
    add( visited_plmn_id );
    add( access_restriction_data );
}

AdjacentAccessRestrictionDataExtractor::~AdjacentAccessRestrictionDataExtractor()
{
}

GeranVectorExtractor::GeranVectorExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpGeranVector() ),
    item_number( *this, dict.avpItemNumber() ),
    rand( *this, dict.avpRand() ),
    sres( *this, dict.avpSres() ),
    kc( *this, dict.avpKc() )
{
    add( item_number );
    add( rand );
    add( sres );
    add( kc );
}

GeranVectorExtractor::~GeranVectorExtractor()
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

MmeLocationInformationExtractor::MmeLocationInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMmeLocationInformation() ),
    e_utran_cell_global_identity( *this, dict.avpEUtranCellGlobalIdentity() ),
    tracking_area_identity( *this, dict.avpTrackingAreaIdentity() ),
    geographical_information( *this, dict.avpGeographicalInformation() ),
    geodetic_information( *this, dict.avpGeodeticInformation() ),
    current_location_retrieved( *this, dict.avpCurrentLocationRetrieved() ),
    age_of_location_information( *this, dict.avpAgeOfLocationInformation() ),
    user_csg_information( *this, dict ),
    enodeb_id( *this, dict.avpEnodebId() ),
    extended_enodeb_id( *this, dict.avpExtendedEnodebId() )
{
    add( e_utran_cell_global_identity );
    add( tracking_area_identity );
    add( geographical_information );
    add( geodetic_information );
    add( current_location_retrieved );
    add( age_of_location_information );
    add( user_csg_information );
    add( enodeb_id );
    add( extended_enodeb_id );
}

MmeLocationInformationExtractor::~MmeLocationInformationExtractor()
{
}

SgsnLocationInformationExtractor::SgsnLocationInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSgsnLocationInformation() ),
    cell_global_identity( *this, dict.avpCellGlobalIdentity() ),
    location_area_identity( *this, dict.avpLocationAreaIdentity() ),
    service_area_identity( *this, dict.avpServiceAreaIdentity() ),
    routing_area_identity( *this, dict.avpRoutingAreaIdentity() ),
    geographical_information( *this, dict.avpGeographicalInformation() ),
    geodetic_information( *this, dict.avpGeodeticInformation() ),
    current_location_retrieved( *this, dict.avpCurrentLocationRetrieved() ),
    age_of_location_information( *this, dict.avpAgeOfLocationInformation() ),
    user_csg_information( *this, dict )
{
    add( cell_global_identity );
    add( location_area_identity );
    add( service_area_identity );
    add( routing_area_identity );
    add( geographical_information );
    add( geodetic_information );
    add( current_location_retrieved );
    add( age_of_location_information );
    add( user_csg_information );
}

SgsnLocationInformationExtractor::~SgsnLocationInformationExtractor()
{
}

EpsLocationInformationExtractor::EpsLocationInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEpsLocationInformation() ),
    mme_location_information( *this, dict ),
    sgsn_location_information( *this, dict )
{
    add( mme_location_information );
    add( sgsn_location_information );
}

EpsLocationInformationExtractor::~EpsLocationInformationExtractor()
{
}

AdjacentPlmnsExtractor::AdjacentPlmnsExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAdjacentPlmns() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() )
{
    add( visited_plmn_id );
}

AdjacentPlmnsExtractor::~AdjacentPlmnsExtractor()
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

CallBarringInfoExtractor::CallBarringInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCallBarringInfo() ),
    ss_code( *this, dict.avpSsCode() ),
    ss_status( *this, dict.avpSsStatus() )
{
    add( ss_code );
    add( ss_status );
}

CallBarringInfoExtractor::~CallBarringInfoExtractor()
{
}

MoLrExtractor::MoLrExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMoLr() ),
    ss_code( *this, dict.avpSsCode() ),
    ss_status( *this, dict.avpSsStatus() )
{
    add( ss_code );
    add( ss_status );
}

MoLrExtractor::~MoLrExtractor()
{
}

Mip6AgentInfoExtractor::Mip6AgentInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMip6AgentInfo() ),
    mip_home_agent_address( *this, dict.avpMipHomeAgentAddress() ),
    mip_home_agent_host( *this, dict.avpMipHomeAgentHost() ),
    mip6_home_link_prefix( *this, dict.avpMip6HomeLinkPrefix() )
{
    add( mip_home_agent_address );
    add( mip_home_agent_host );
    add( mip6_home_link_prefix );
}

Mip6AgentInfoExtractor::~Mip6AgentInfoExtractor()
{
}

SpecificApnInfoExtractor::SpecificApnInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSpecificApnInfo() ),
    service_selection( *this, dict.avpServiceSelection() ),
    mip6_agent_info( *this, dict ),
    visited_network_identifier( *this, dict.avpVisitedNetworkIdentifier() )
{
    add( service_selection );
    add( mip6_agent_info );
    add( visited_network_identifier );
}

SpecificApnInfoExtractor::~SpecificApnInfoExtractor()
{
}

SpecificApnInfoExtractorList::SpecificApnInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpSpecificApnInfo() ),
    m_dict( dict )
{
}

SpecificApnInfoExtractorList::~SpecificApnInfoExtractorList()
{
}

ActiveApnExtractor::ActiveApnExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpActiveApn() ),
    context_identifier( *this, dict.avpContextIdentifier() ),
    service_selection( *this, dict.avpServiceSelection() ),
    mip6_agent_info( *this, dict ),
    visited_network_identifier( *this, dict.avpVisitedNetworkIdentifier() ),
    specific_apn_info( *this, dict )
{
    add( context_identifier );
    add( service_selection );
    add( mip6_agent_info );
    add( visited_network_identifier );
    add( specific_apn_info );
}

ActiveApnExtractor::~ActiveApnExtractor()
{
}

WlanOffloadabilityExtractor::WlanOffloadabilityExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpWlanOffloadability() ),
    wlan_offloadability_eutran( *this, dict.avpWlanOffloadabilityEutran() ),
    wlan_offloadability_utran( *this, dict.avpWlanOffloadabilityUtran() )
{
    add( wlan_offloadability_eutran );
    add( wlan_offloadability_utran );
}

WlanOffloadabilityExtractor::~WlanOffloadabilityExtractor()
{
}

ScheduledCommunicationTimeExtractor::ScheduledCommunicationTimeExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpScheduledCommunicationTime() ),
    day_of_week_mask( *this, dict.avpDayOfWeekMask() ),
    time_of_day_start( *this, dict.avpTimeOfDayStart() ),
    time_of_day_end( *this, dict.avpTimeOfDayEnd() )
{
    add( day_of_week_mask );
    add( time_of_day_start );
    add( time_of_day_end );
}

ScheduledCommunicationTimeExtractor::~ScheduledCommunicationTimeExtractor()
{
}

ScheduledCommunicationTimeExtractorList::ScheduledCommunicationTimeExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpScheduledCommunicationTime() ),
    m_dict( dict )
{
}

ScheduledCommunicationTimeExtractorList::~ScheduledCommunicationTimeExtractorList()
{
}

CommunicationPatternSetExtractor::CommunicationPatternSetExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCommunicationPatternSet() ),
    periodic_communication_indicator( *this, dict.avpPeriodicCommunicationIndicator() ),
    communication_duration_time( *this, dict.avpCommunicationDurationTime() ),
    periodic_time( *this, dict.avpPeriodicTime() ),
    scheduled_communication_time( *this, dict ),
    stationary_indication( *this, dict.avpStationaryIndication() ),
    reference_id_validity_time( *this, dict.avpReferenceIdValidityTime() )
{
    add( periodic_communication_indicator );
    add( communication_duration_time );
    add( periodic_time );
    add( scheduled_communication_time );
    add( stationary_indication );
    add( reference_id_validity_time );
}

CommunicationPatternSetExtractor::~CommunicationPatternSetExtractor()
{
}

EquivalentPlmnListExtractor::EquivalentPlmnListExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEquivalentPlmnList() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() )
{
    add( visited_plmn_id );
}

EquivalentPlmnListExtractor::~EquivalentPlmnListExtractor()
{
}

SupportedServicesExtractor::SupportedServicesExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSupportedServices() ),
    supported_monitoring_events( *this, dict.avpSupportedMonitoringEvents() )
{
    add( supported_monitoring_events );
}

SupportedServicesExtractor::~SupportedServicesExtractor()
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

UtranVectorExtractor::UtranVectorExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUtranVector() ),
    item_number( *this, dict.avpItemNumber() ),
    rand( *this, dict.avpRand() ),
    xres( *this, dict.avpXres() ),
    autn( *this, dict.avpAutn() ),
    confidentiality_key( *this, dict.avpConfidentialityKey() ),
    integrity_key( *this, dict.avpIntegrityKey() )
{
    add( item_number );
    add( rand );
    add( xres );
    add( autn );
    add( confidentiality_key );
    add( integrity_key );
}

UtranVectorExtractor::~UtranVectorExtractor()
{
}

UeReachabilityConfigurationExtractor::UeReachabilityConfigurationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUeReachabilityConfiguration() ),
    reachability_type( *this, dict.avpReachabilityType() ),
    maximum_response_time( *this, dict.avpMaximumResponseTime() )
{
    add( reachability_type );
    add( maximum_response_time );
}

UeReachabilityConfigurationExtractor::~UeReachabilityConfigurationExtractor()
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

EpsSubscribedQosProfileExtractor::EpsSubscribedQosProfileExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEpsSubscribedQosProfile() ),
    qos_class_identifier( *this, dict.avpQosClassIdentifier() ),
    allocation_retention_priority( *this, dict )
{
    add( qos_class_identifier );
    add( allocation_retention_priority );
}

EpsSubscribedQosProfileExtractor::~EpsSubscribedQosProfileExtractor()
{
}

ApnConfigurationExtractor::ApnConfigurationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpApnConfiguration() ),
    context_identifier( *this, dict.avpContextIdentifier() ),
    served_party_ip_address( *this, dict.avpServedPartyIpAddress() ),
    pdn_type( *this, dict.avpPdnType() ),
    service_selection( *this, dict.avpServiceSelection() ),
    eps_subscribed_qos_profile( *this, dict ),
    vplmn_dynamic_address_allowed( *this, dict.avpVplmnDynamicAddressAllowed() ),
    mip6_agent_info( *this, dict ),
    visited_network_identifier( *this, dict.avpVisitedNetworkIdentifier() ),
    pdn_gw_allocation_type( *this, dict.avpPdnGwAllocationType() ),
    tgpp_charging_characteristics( *this, dict.avp3gppChargingCharacteristics() ),
    ambr( *this, dict ),
    specific_apn_info( *this, dict ),
    apn_oi_replacement( *this, dict.avpApnOiReplacement() ),
    sipto_permission( *this, dict.avpSiptoPermission() ),
    lipa_permission( *this, dict.avpLipaPermission() ),
    restoration_priority( *this, dict.avpRestorationPriority() ),
    sipto_local_network_permission( *this, dict.avpSiptoLocalNetworkPermission() ),
    wlan_offloadability( *this, dict ),
    non_ip_pdn_type_indicator( *this, dict.avpNonIpPdnTypeIndicator() ),
    non_ip_data_delivery_mechanism( *this, dict.avpNonIpDataDeliveryMechanism() ),
    scef_id( *this, dict.avpScefId() ),
    scef_realm( *this, dict.avpScefRealm() ),
    preferred_data_mode( *this, dict.avpPreferredDataMode() ),
    pdn_connection_continuity( *this, dict.avpPdnConnectionContinuity() )
{
    add( context_identifier );
    add( served_party_ip_address );
    add( pdn_type );
    add( service_selection );
    add( eps_subscribed_qos_profile );
    add( vplmn_dynamic_address_allowed );
    add( mip6_agent_info );
    add( visited_network_identifier );
    add( pdn_gw_allocation_type );
    add( tgpp_charging_characteristics );
    add( ambr );
    add( specific_apn_info );
    add( apn_oi_replacement );
    add( sipto_permission );
    add( lipa_permission );
    add( restoration_priority );
    add( sipto_local_network_permission );
    add( wlan_offloadability );
    add( non_ip_pdn_type_indicator );
    add( non_ip_data_delivery_mechanism );
    add( scef_id );
    add( scef_realm );
    add( preferred_data_mode );
    add( pdn_connection_continuity );
}

ApnConfigurationExtractor::~ApnConfigurationExtractor()
{
}

LocalTimeZoneExtractor::LocalTimeZoneExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLocalTimeZone() ),
    time_zone( *this, dict.avpTimeZone() ),
    daylight_saving_time( *this, dict.avpDaylightSavingTime() )
{
    add( time_zone );
    add( daylight_saving_time );
}

LocalTimeZoneExtractor::~LocalTimeZoneExtractor()
{
}

EUtranVectorExtractorList::EUtranVectorExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpEUtranVector() ),
    m_dict( dict )
{
}

EUtranVectorExtractorList::~EUtranVectorExtractorList()
{
}

UtranVectorExtractorList::UtranVectorExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpUtranVector() ),
    m_dict( dict )
{
}

UtranVectorExtractorList::~UtranVectorExtractorList()
{
}

GeranVectorExtractorList::GeranVectorExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpGeranVector() ),
    m_dict( dict )
{
}

GeranVectorExtractorList::~GeranVectorExtractorList()
{
}

AuthenticationInfoExtractor::AuthenticationInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAuthenticationInfo() ),
    e_utran_vector( *this, dict ),
    utran_vector( *this, dict ),
    geran_vector( *this, dict )
{
    add( e_utran_vector );
    add( utran_vector );
    add( geran_vector );
}

AuthenticationInfoExtractor::~AuthenticationInfoExtractor()
{
}

ImsiGroupIdExtractor::ImsiGroupIdExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpImsiGroupId() ),
    group_service_id( *this, dict.avpGroupServiceId() ),
    group_plmn_id( *this, dict.avpGroupPlmnId() ),
    local_group_id( *this, dict.avpLocalGroupId() )
{
    add( group_service_id );
    add( group_plmn_id );
    add( local_group_id );
}

ImsiGroupIdExtractor::~ImsiGroupIdExtractor()
{
}

LcsPrivacyexceptionExtractorList::LcsPrivacyexceptionExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpLcsPrivacyexception() ),
    m_dict( dict )
{
}

LcsPrivacyexceptionExtractorList::~LcsPrivacyexceptionExtractorList()
{
}

MoLrExtractorList::MoLrExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpMoLr() ),
    m_dict( dict )
{
}

MoLrExtractorList::~MoLrExtractorList()
{
}

LcsInfoExtractor::LcsInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLcsInfo() ),
    gmlc_number( *this, dict.avpGmlcNumber() ),
    lcs_privacyexception( *this, dict ),
    mo_lr( *this, dict )
{
    add( gmlc_number );
    add( lcs_privacyexception );
    add( mo_lr );
}

LcsInfoExtractor::~LcsInfoExtractor()
{
}

TeleserviceListExtractor::TeleserviceListExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTeleserviceList() ),
    ts_code( *this, dict.avpTsCode() )
{
    add( ts_code );
}

TeleserviceListExtractor::~TeleserviceListExtractor()
{
}

CallBarringInfoExtractorList::CallBarringInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpCallBarringInfo() ),
    m_dict( dict )
{
}

CallBarringInfoExtractorList::~CallBarringInfoExtractorList()
{
}

ApnConfigurationExtractorList::ApnConfigurationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpApnConfiguration() ),
    m_dict( dict )
{
}

ApnConfigurationExtractorList::~ApnConfigurationExtractorList()
{
}

ApnConfigurationProfileExtractor::ApnConfigurationProfileExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpApnConfigurationProfile() ),
    context_identifier( *this, dict.avpContextIdentifier() ),
    additional_context_identifier( *this, dict.avpAdditionalContextIdentifier() ),
    all_apn_configurations_included_indicator( *this, dict.avpAllApnConfigurationsIncludedIndicator() ),
    apn_configuration( *this, dict )
{
    add( context_identifier );
    add( additional_context_identifier );
    add( all_apn_configurations_included_indicator );
    add( apn_configuration );
}

ApnConfigurationProfileExtractor::~ApnConfigurationProfileExtractor()
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

CsgSubscriptionDataExtractor::CsgSubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpCsgSubscriptionData() ),
    csg_id( *this, dict.avpCsgId() ),
    expiration_date( *this, dict.avpExpirationDate() ),
    service_selection( *this, dict.avpServiceSelection() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() )
{
    add( csg_id );
    add( expiration_date );
    add( service_selection );
    add( visited_plmn_id );
}

CsgSubscriptionDataExtractor::~CsgSubscriptionDataExtractor()
{
}

CsgSubscriptionDataExtractorList::CsgSubscriptionDataExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpCsgSubscriptionData() ),
    m_dict( dict )
{
}

CsgSubscriptionDataExtractorList::~CsgSubscriptionDataExtractorList()
{
}

ProseSubscriptionDataExtractor::ProseSubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpProseSubscriptionData() ),
    prose_permission( *this, dict.avpProsePermission() )
{
    add( prose_permission );
}

ProseSubscriptionDataExtractor::~ProseSubscriptionDataExtractor()
{
}

AdjacentAccessRestrictionDataExtractorList::AdjacentAccessRestrictionDataExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAdjacentAccessRestrictionData() ),
    m_dict( dict )
{
}

AdjacentAccessRestrictionDataExtractorList::~AdjacentAccessRestrictionDataExtractorList()
{
}

ImsiGroupIdExtractorList::ImsiGroupIdExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpImsiGroupId() ),
    m_dict( dict )
{
}

ImsiGroupIdExtractorList::~ImsiGroupIdExtractorList()
{
}

CommunicationPatternSetExtractorList::CommunicationPatternSetExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpCommunicationPatternSet() ),
    m_dict( dict )
{
}

CommunicationPatternSetExtractorList::~CommunicationPatternSetExtractorList()
{
}

AeseCommunicationPatternExtractor::AeseCommunicationPatternExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAeseCommunicationPattern() ),
    scef_reference_id( *this, dict.avpScefReferenceId() ),
    scef_id( *this, dict.avpScefId() ),
    scef_reference_id_for_deletion( *this, dict.avpScefReferenceIdForDeletion() ),
    communication_pattern_set( *this, dict )
{
    add( scef_reference_id );
    add( scef_id );
    add( scef_reference_id_for_deletion );
    add( communication_pattern_set );
}

AeseCommunicationPatternExtractor::~AeseCommunicationPatternExtractor()
{
}

AeseCommunicationPatternExtractorList::AeseCommunicationPatternExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAeseCommunicationPattern() ),
    m_dict( dict )
{
}

AeseCommunicationPatternExtractorList::~AeseCommunicationPatternExtractorList()
{
}

LocationInformationConfigurationExtractor::LocationInformationConfigurationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpLocationInformationConfiguration() ),
    monte_location_type( *this, dict.avpMonteLocationType() ),
    accuracy( *this, dict.avpAccuracy() )
{
    add( monte_location_type );
    add( accuracy );
}

LocationInformationConfigurationExtractor::~LocationInformationConfigurationExtractor()
{
}

MonitoringEventConfigurationExtractor::MonitoringEventConfigurationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMonitoringEventConfiguration() ),
    scef_reference_id( *this, dict.avpScefReferenceId() ),
    scef_id( *this, dict.avpScefId() ),
    monitoring_type( *this, dict.avpMonitoringType() ),
    scef_reference_id_for_deletion( *this, dict.avpScefReferenceIdForDeletion() ),
    maximum_number_of_reports( *this, dict.avpMaximumNumberOfReports() ),
    monitoring_duration( *this, dict.avpMonitoringDuration() ),
    charged_party( *this, dict.avpChargedParty() ),
    ue_reachability_configuration( *this, dict ),
    location_information_configuration( *this, dict ),
    scef_realm( *this, dict.avpScefRealm() )
{
    add( scef_reference_id );
    add( scef_id );
    add( monitoring_type );
    add( scef_reference_id_for_deletion );
    add( maximum_number_of_reports );
    add( monitoring_duration );
    add( charged_party );
    add( ue_reachability_configuration );
    add( location_information_configuration );
    add( scef_realm );
}

MonitoringEventConfigurationExtractor::~MonitoringEventConfigurationExtractor()
{
}

MonitoringEventConfigurationExtractorList::MonitoringEventConfigurationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpMonitoringEventConfiguration() ),
    m_dict( dict )
{
}

MonitoringEventConfigurationExtractorList::~MonitoringEventConfigurationExtractorList()
{
}

EmergencyInfoExtractor::EmergencyInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEmergencyInfo() ),
    mip6_agent_info( *this, dict )
{
    add( mip6_agent_info );
}

EmergencyInfoExtractor::~EmergencyInfoExtractor()
{
}

EdrxCycleLengthExtractorList::EdrxCycleLengthExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpEdrxCycleLength() ),
    m_dict( dict )
{
}

EdrxCycleLengthExtractorList::~EdrxCycleLengthExtractorList()
{
}

SubscriptionDataExtractor::SubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSubscriptionData() ),
    subscriber_status( *this, dict.avpSubscriberStatus() ),
    msisdn( *this, dict.avpMsisdn() ),
    a_msisdn( *this, dict.avpAMsisdn() ),
    stn_sr( *this, dict.avpStnSr() ),
    ics_indicator( *this, dict.avpIcsIndicator() ),
    network_access_mode( *this, dict.avpNetworkAccessMode() ),
    operator_determined_barring( *this, dict.avpOperatorDeterminedBarring() ),
    hplmn_odb( *this, dict.avpHplmnOdb() ),
    regional_subscription_zone_code( *this, dict.avpRegionalSubscriptionZoneCode() ),
    access_restriction_data( *this, dict.avpAccessRestrictionData() ),
    apn_oi_replacement( *this, dict.avpApnOiReplacement() ),
    lcs_info( *this, dict ),
    teleservice_list( *this, dict ),
    call_barring_info( *this, dict ),
    tgpp_charging_characteristics( *this, dict.avp3gppChargingCharacteristics() ),
    ambr( *this, dict ),
    apn_configuration_profile( *this, dict ),
    rat_frequency_selection_priority_id( *this, dict.avpRatFrequencySelectionPriorityId() ),
    trace_data( *this, dict ),
    gprs_subscription_data( *this, dict ),
    csg_subscription_data( *this, dict ),
    roaming_restricted_due_to_unsupported_feature( *this, dict.avpRoamingRestrictedDueToUnsupportedFeature() ),
    subscribed_periodic_rau_tau_timer( *this, dict.avpSubscribedPeriodicRauTauTimer() ),
    mps_priority( *this, dict.avpMpsPriority() ),
    vplmn_lipa_allowed( *this, dict.avpVplmnLipaAllowed() ),
    relay_node_indicator( *this, dict.avpRelayNodeIndicator() ),
    mdt_user_consent( *this, dict.avpMdtUserConsent() ),
    subscribed_vsrvcc( *this, dict.avpSubscribedVsrvcc() ),
    prose_subscription_data( *this, dict ),
    subscription_data_flags( *this, dict.avpSubscriptionDataFlags() ),
    adjacent_access_restriction_data( *this, dict ),
    dl_buffering_suggested_packet_count( *this, dict.avpDlBufferingSuggestedPacketCount() ),
    imsi_group_id( *this, dict ),
    ue_usage_type( *this, dict.avpUeUsageType() ),
    aese_communication_pattern( *this, dict ),
    monitoring_event_configuration( *this, dict ),
    emergency_info( *this, dict ),
    v2x_subscription_data( *this, dict ),
    edrx_cycle_length( *this, dict )
{
    add( subscriber_status );
    add( msisdn );
    add( a_msisdn );
    add( stn_sr );
    add( ics_indicator );
    add( network_access_mode );
    add( operator_determined_barring );
    add( hplmn_odb );
    add( regional_subscription_zone_code );
    add( access_restriction_data );
    add( apn_oi_replacement );
    add( lcs_info );
    add( teleservice_list );
    add( call_barring_info );
    add( tgpp_charging_characteristics );
    add( ambr );
    add( apn_configuration_profile );
    add( rat_frequency_selection_priority_id );
    add( trace_data );
    add( gprs_subscription_data );
    add( csg_subscription_data );
    add( roaming_restricted_due_to_unsupported_feature );
    add( subscribed_periodic_rau_tau_timer );
    add( mps_priority );
    add( vplmn_lipa_allowed );
    add( relay_node_indicator );
    add( mdt_user_consent );
    add( subscribed_vsrvcc );
    add( prose_subscription_data );
    add( subscription_data_flags );
    add( adjacent_access_restriction_data );
    add( dl_buffering_suggested_packet_count );
    add( imsi_group_id );
    add( ue_usage_type );
    add( aese_communication_pattern );
    add( monitoring_event_configuration );
    add( emergency_info );
    add( v2x_subscription_data );
    add( edrx_cycle_length );
}

SubscriptionDataExtractor::~SubscriptionDataExtractor()
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

RequestedUtranGeranAuthenticationInfoExtractor::RequestedUtranGeranAuthenticationInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRequestedUtranGeranAuthenticationInfo() ),
    number_of_requested_vectors( *this, dict.avpNumberOfRequestedVectors() ),
    immediate_response_preferred( *this, dict.avpImmediateResponsePreferred() ),
    re_synchronization_info( *this, dict.avpReSynchronizationInfo() )
{
    add( number_of_requested_vectors );
    add( immediate_response_preferred );
    add( re_synchronization_info );
}

RequestedUtranGeranAuthenticationInfoExtractor::~RequestedUtranGeranAuthenticationInfoExtractor()
{
}

SubscriptionDataDeletionExtractor::SubscriptionDataDeletionExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSubscriptionDataDeletion() ),
    dsr_flags( *this, dict.avpDsrFlags() ),
    scef_id( *this, dict.avpScefId() ),
    context_identifier( *this, dict.avpContextIdentifier() ),
    trace_reference( *this, dict.avpTraceReference() ),
    ts_code( *this, dict.avpTsCode() ),
    ss_code( *this, dict.avpSsCode() )
{
    add( dsr_flags );
    add( scef_id );
    add( context_identifier );
    add( trace_reference );
    add( ts_code );
    add( ss_code );
}

SubscriptionDataDeletionExtractor::~SubscriptionDataDeletionExtractor()
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

TerminalInformationExtractor::TerminalInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpTerminalInformation() ),
    imei( *this, dict.avpImei() ),
    tgpp2_meid( *this, dict.avp3gpp2Meid() ),
    software_version( *this, dict.avpSoftwareVersion() )
{
    add( imei );
    add( tgpp2_meid );
    add( software_version );
}

TerminalInformationExtractor::~TerminalInformationExtractor()
{
}

RequestedEutranAuthenticationInfoExtractor::RequestedEutranAuthenticationInfoExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRequestedEutranAuthenticationInfo() ),
    number_of_requested_vectors( *this, dict.avpNumberOfRequestedVectors() ),
    immediate_response_preferred( *this, dict.avpImmediateResponsePreferred() ),
    re_synchronization_info( *this, dict.avpReSynchronizationInfo() )
{
    add( number_of_requested_vectors );
    add( immediate_response_preferred );
    add( re_synchronization_info );
}

RequestedEutranAuthenticationInfoExtractor::~RequestedEutranAuthenticationInfoExtractor()
{
}

MonitoringEventReportExtractor::MonitoringEventReportExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMonitoringEventReport() ),
    scef_reference_id( *this, dict.avpScefReferenceId() ),
    scef_id( *this, dict.avpScefId() ),
    reachability_information( *this, dict.avpReachabilityInformation() ),
    eps_location_information( *this, dict ),
    monitoring_type( *this, dict.avpMonitoringType() )
{
    add( scef_reference_id );
    add( scef_id );
    add( reachability_information );
    add( eps_location_information );
    add( monitoring_type );
}

MonitoringEventReportExtractor::~MonitoringEventReportExtractor()
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

SupportedFeaturesExtractorList::SupportedFeaturesExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpSupportedFeatures() ),
    m_dict( dict )
{
}

SupportedFeaturesExtractorList::~SupportedFeaturesExtractorList()
{
}

ActiveApnExtractorList::ActiveApnExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpActiveApn() ),
    m_dict( dict )
{
}

ActiveApnExtractorList::~ActiveApnExtractorList()
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

UpdateLocationRequestExtractor::UpdateLocationRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    user_name( *this, dict.avpUserName() ),
    oc_supported_features( *this, dict ),
    supported_features( *this, dict ),
    terminal_information( *this, dict ),
    rat_type( *this, dict.avpRatType() ),
    ulr_flags( *this, dict.avpUlrFlags() ),
    ue_srvcc_capability( *this, dict.avpUeSrvccCapability() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() ),
    sgsn_number( *this, dict.avpSgsnNumber() ),
    homogeneous_support_of_ims_voice_over_ps_sessions( *this, dict.avpHomogeneousSupportOfImsVoiceOverPsSessions() ),
    gmlc_address( *this, dict.avpGmlcAddress() ),
    active_apn( *this, dict ),
    equivalent_plmn_list( *this, dict ),
    mme_number_for_mt_sms( *this, dict.avpMmeNumberForMtSms() ),
    sms_register_request( *this, dict.avpSmsRegisterRequest() ),
    sgs_mme_identity( *this, dict.avpSgsMmeIdentity() ),
    coupled_node_diameter_id( *this, dict.avpCoupledNodeDiameterId() ),
    adjacent_plmns( *this, dict ),
    supported_services( *this, dict ),
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
    add( user_name );
    add( oc_supported_features );
    add( supported_features );
    add( terminal_information );
    add( rat_type );
    add( ulr_flags );
    add( ue_srvcc_capability );
    add( visited_plmn_id );
    add( sgsn_number );
    add( homogeneous_support_of_ims_voice_over_ps_sessions );
    add( gmlc_address );
    add( active_apn );
    add( equivalent_plmn_list );
    add( mme_number_for_mt_sms );
    add( sms_register_request );
    add( sgs_mme_identity );
    add( coupled_node_diameter_id );
    add( adjacent_plmns );
    add( supported_services );
    add( proxy_info );
    add( route_record );
}

UpdateLocationRequestExtractor::~UpdateLocationRequestExtractor()
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

UpdateLocationAnswerExtractor::UpdateLocationAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    error_diagnostic( *this, dict.avpErrorDiagnostic() ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    load( *this, dict ),
    supported_features( *this, dict ),
    ula_flags( *this, dict.avpUlaFlags() ),
    subscription_data( *this, dict ),
    reset_id( *this, dict.avpResetId() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( result_code );
    add( experimental_result );
    add( error_diagnostic );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( oc_supported_features );
    add( oc_olr );
    add( load );
    add( supported_features );
    add( ula_flags );
    add( subscription_data );
    add( reset_id );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

UpdateLocationAnswerExtractor::~UpdateLocationAnswerExtractor()
{
}

CancelLocationRequestExtractor::CancelLocationRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    user_name( *this, dict.avpUserName() ),
    supported_features( *this, dict ),
    cancellation_type( *this, dict.avpCancellationType() ),
    clr_flags( *this, dict.avpClrFlags() ),
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
    add( user_name );
    add( supported_features );
    add( cancellation_type );
    add( clr_flags );
    add( proxy_info );
    add( route_record );
}

CancelLocationRequestExtractor::~CancelLocationRequestExtractor()
{
}

CancelLocationAnswerExtractor::CancelLocationAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    supported_features( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( supported_features );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

CancelLocationAnswerExtractor::~CancelLocationAnswerExtractor()
{
}

AuthenticationInformationRequestExtractor::AuthenticationInformationRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    user_name( *this, dict.avpUserName() ),
    oc_supported_features( *this, dict ),
    supported_features( *this, dict ),
    requested_eutran_authentication_info( *this, dict ),
    requested_utran_geran_authentication_info( *this, dict ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() ),
    air_flags( *this, dict.avpAirFlags() ),
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
    add( user_name );
    add( oc_supported_features );
    add( supported_features );
    add( requested_eutran_authentication_info );
    add( requested_utran_geran_authentication_info );
    add( visited_plmn_id );
    add( air_flags );
    add( proxy_info );
    add( route_record );
}

AuthenticationInformationRequestExtractor::~AuthenticationInformationRequestExtractor()
{
}

AuthenticationInformationAnswerExtractor::AuthenticationInformationAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    error_diagnostic( *this, dict.avpErrorDiagnostic() ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    load( *this, dict ),
    supported_features( *this, dict ),
    authentication_info( *this, dict ),
    ue_usage_type( *this, dict.avpUeUsageType() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( result_code );
    add( experimental_result );
    add( error_diagnostic );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( oc_supported_features );
    add( oc_olr );
    add( load );
    add( supported_features );
    add( authentication_info );
    add( ue_usage_type );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

AuthenticationInformationAnswerExtractor::~AuthenticationInformationAnswerExtractor()
{
}

InsertSubscriberDataRequestExtractor::InsertSubscriberDataRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    user_name( *this, dict.avpUserName() ),
    supported_features( *this, dict ),
    subscription_data( *this, dict ),
    idr_flags( *this, dict.avpIdrFlags() ),
    reset_id( *this, dict.avpResetId() ),
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
    add( user_name );
    add( supported_features );
    add( subscription_data );
    add( idr_flags );
    add( reset_id );
    add( proxy_info );
    add( route_record );
}

InsertSubscriberDataRequestExtractor::~InsertSubscriberDataRequestExtractor()
{
}

MonitoringEventReportExtractorList::MonitoringEventReportExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpMonitoringEventReport() ),
    m_dict( dict )
{
}

MonitoringEventReportExtractorList::~MonitoringEventReportExtractorList()
{
}

MonitoringEventConfigStatusExtractorList::MonitoringEventConfigStatusExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpMonitoringEventConfigStatus() ),
    m_dict( dict )
{
}

MonitoringEventConfigStatusExtractorList::~MonitoringEventConfigStatusExtractorList()
{
}

InsertSubscriberDataAnswerExtractor::InsertSubscriberDataAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    supported_features( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    ims_voice_over_ps_sessions_supported( *this, dict.avpImsVoiceOverPsSessionsSupported() ),
    last_ue_activity_time( *this, dict.avpLastUeActivityTime() ),
    rat_type( *this, dict.avpRatType() ),
    ida_flags( *this, dict.avpIdaFlags() ),
    eps_user_state( *this, dict ),
    eps_location_information( *this, dict ),
    local_time_zone( *this, dict ),
    supported_services( *this, dict ),
    monitoring_event_report( *this, dict ),
    monitoring_event_config_status( *this, dict ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( supported_features );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( ims_voice_over_ps_sessions_supported );
    add( last_ue_activity_time );
    add( rat_type );
    add( ida_flags );
    add( eps_user_state );
    add( eps_location_information );
    add( local_time_zone );
    add( supported_services );
    add( monitoring_event_report );
    add( monitoring_event_config_status );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

InsertSubscriberDataAnswerExtractor::~InsertSubscriberDataAnswerExtractor()
{
}

DeleteSubscriberDataRequestExtractor::DeleteSubscriberDataRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    user_name( *this, dict.avpUserName() ),
    supported_features( *this, dict ),
    dsr_flags( *this, dict.avpDsrFlags() ),
    scef_id( *this, dict.avpScefId() ),
    context_identifier( *this, dict.avpContextIdentifier() ),
    trace_reference( *this, dict.avpTraceReference() ),
    ts_code( *this, dict.avpTsCode() ),
    ss_code( *this, dict.avpSsCode() ),
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
    add( user_name );
    add( supported_features );
    add( dsr_flags );
    add( scef_id );
    add( context_identifier );
    add( trace_reference );
    add( ts_code );
    add( ss_code );
    add( proxy_info );
    add( route_record );
}

DeleteSubscriberDataRequestExtractor::~DeleteSubscriberDataRequestExtractor()
{
}

DeleteSubscriberDataAnswerExtractor::DeleteSubscriberDataAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    supported_features( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    dsa_flags( *this, dict.avpDsaFlags() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( supported_features );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( dsa_flags );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

DeleteSubscriberDataAnswerExtractor::~DeleteSubscriberDataAnswerExtractor()
{
}

PurgeUeRequestExtractor::PurgeUeRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    user_name( *this, dict.avpUserName() ),
    oc_supported_features( *this, dict ),
    pur_flags( *this, dict.avpPurFlags() ),
    supported_features( *this, dict ),
    eps_location_information( *this, dict ),
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
    add( user_name );
    add( oc_supported_features );
    add( pur_flags );
    add( supported_features );
    add( eps_location_information );
    add( proxy_info );
    add( route_record );
}

PurgeUeRequestExtractor::~PurgeUeRequestExtractor()
{
}

PurgeUeAnswerExtractor::PurgeUeAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    supported_features( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    load( *this, dict ),
    pua_flags( *this, dict.avpPuaFlags() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( supported_features );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( oc_supported_features );
    add( oc_olr );
    add( load );
    add( pua_flags );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

PurgeUeAnswerExtractor::~PurgeUeAnswerExtractor()
{
}

ResetRequestExtractor::ResetRequestExtractor( FDMessage &msg, Dictionary &dict )
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
    user_id( *this, dict.avpUserId() ),
    reset_id( *this, dict.avpResetId() ),
    subscription_data( *this, dict ),
    subscription_data_deletion( *this, dict ),
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
    add( user_id );
    add( reset_id );
    add( subscription_data );
    add( subscription_data_deletion );
    add( proxy_info );
    add( route_record );
}

ResetRequestExtractor::~ResetRequestExtractor()
{
}

ResetAnswerExtractor::ResetAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    vendor_specific_application_id( *this, dict ),
    supported_features( *this, dict ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( vendor_specific_application_id );
    add( supported_features );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

ResetAnswerExtractor::~ResetAnswerExtractor()
{
}


}
