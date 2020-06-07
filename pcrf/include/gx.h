/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __GX_H
#define __GX_H

#include "fd.h"

namespace gx {

//
// Primary Objects
//

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    FDDictionaryEntryApplication &app()                                 { return m_app; }

    FDDictionaryEntryCommand &cmdCRCR()                                	{ return m_cmd_crcr; }
    FDDictionaryEntryCommand &cmdCRCA()                                	{ return m_cmd_crca; }
    FDDictionaryEntryCommand &cmdREAR()                                	{ return m_cmd_rear; }
    FDDictionaryEntryCommand &cmdREAA()                                	{ return m_cmd_reaa; }

    FDDictionaryEntryVendor &vndETSI()                                 	{ return m_vnd_Etsi; }
    FDDictionaryEntryVendor &vnd3GPP2()                                	{ return m_vnd_3gpp2; }
    FDDictionaryEntryVendor &vnd3GPP()                                 	{ return m_vnd_3gpp; }

    FDDictionaryEntryAVP &avpTdfDestinationHost()                      	{ return m_avp_tdf_destination_host; }
    FDDictionaryEntryAVP &avpPreEmptionCapability()                    	{ return m_avp_pre_emption_capability; }
    FDDictionaryEntryAVP &avpPacketFilterContent()                     	{ return m_avp_packet_filter_content; }
    FDDictionaryEntryAVP &avpFeatureListId()                           	{ return m_avp_feature_list_id; }
    FDDictionaryEntryAVP &avpResourceReleaseNotification()             	{ return m_avp_resource_release_notification; }
    FDDictionaryEntryAVP &avpRanRuleSupport()                          	{ return m_avp_ran_rule_support; }
    FDDictionaryEntryAVP &avpServiceIdentifier()                       	{ return m_avp_service_identifier; }
    FDDictionaryEntryAVP &avpPhysicalAccessId()                        	{ return m_avp_physical_access_id; }
    FDDictionaryEntryAVP &avpCsgAccessMode()                           	{ return m_avp_csg_access_mode; }
    FDDictionaryEntryAVP &avpHenbLocalIpAddress()                      	{ return m_avp_henb_local_ip_address; }
    FDDictionaryEntryAVP &avpFlowNumber()                              	{ return m_avp_flow_number; }
    FDDictionaryEntryAVP &avpBearerIdentifier()                        	{ return m_avp_bearer_identifier; }
    FDDictionaryEntryAVP &avpApnAggregateMaxBitrateUl()                	{ return m_avp_apn_aggregate_max_bitrate_ul; }
    FDDictionaryEntryAVP &avpNetworkRequestSupport()                   	{ return m_avp_network_request_support; }
    FDDictionaryEntryAVP &avpTerminationCause()                        	{ return m_avp_termination_cause; }
    FDDictionaryEntryAVP &avpTrafficSteeringPolicyIdentifierUl()       	{ return m_avp_traffic_steering_policy_identifier_ul; }
    FDDictionaryEntryAVP &avpSponsorIdentity()                         	{ return m_avp_sponsor_identity; }
    FDDictionaryEntryAVP &avp3gppRatType()                             	{ return m_avp_3gpp_rat_type; }
    FDDictionaryEntryAVP &avpAfSignallingProtocol()                    	{ return m_avp_af_signalling_protocol; }
    FDDictionaryEntryAVP &avpPacketFilterUsage()                       	{ return m_avp_packet_filter_usage; }
    FDDictionaryEntryAVP &avpOcSequenceNumber()                        	{ return m_avp_oc_sequence_number; }
    FDDictionaryEntryAVP &avpUsageMonitoringSupport()                  	{ return m_avp_usage_monitoring_support; }
    FDDictionaryEntryAVP &avpTrackingAreaIdentity()                    	{ return m_avp_tracking_area_identity; }
    FDDictionaryEntryAVP &avpLoadValue()                               	{ return m_avp_load_value; }
    FDDictionaryEntryAVP &avpFeatureList()                             	{ return m_avp_feature_list; }
    FDDictionaryEntryAVP &avpOmcId()                                   	{ return m_avp_omc_id; }
    FDDictionaryEntryAVP &avpRai()                                     	{ return m_avp_rai; }
    FDDictionaryEntryAVP &avpOcReportType()                            	{ return m_avp_oc_report_type; }
    FDDictionaryEntryAVP &avpTftFilter()                               	{ return m_avp_tft_filter; }
    FDDictionaryEntryAVP &avpSecondaryEventChargingFunctionName()      	{ return m_avp_secondary_event_charging_function_name; }
    FDDictionaryEntryAVP &avpPcscfRestorationIndication()              	{ return m_avp_pcscf_restoration_indication; }
    FDDictionaryEntryAVP &avpLoad()                                    	{ return m_avp_load; }
    FDDictionaryEntryAVP &avpOcFeatureVector()                         	{ return m_avp_oc_feature_vector; }
    FDDictionaryEntryAVP &avpCcServiceSpecificUnits()                  	{ return m_avp_cc_service_specific_units; }
    FDDictionaryEntryAVP &avpPraRemove()                               	{ return m_avp_pra_remove; }
    FDDictionaryEntryAVP &avpCcTime()                                  	{ return m_avp_cc_time; }
    FDDictionaryEntryAVP &avpListOfMeasurements()                      	{ return m_avp_list_of_measurements; }
    FDDictionaryEntryAVP &avpQosInformation()                          	{ return m_avp_qos_information; }
    FDDictionaryEntryAVP &avpFinalUnitAction()                         	{ return m_avp_final_unit_action; }
    FDDictionaryEntryAVP &avpConditionalPolicyInformation()            	{ return m_avp_conditional_policy_information; }
    FDDictionaryEntryAVP &avpDrmp()                                    	{ return m_avp_drmp; }
    FDDictionaryEntryAVP &avpPraInstall()                              	{ return m_avp_pra_install; }
    FDDictionaryEntryAVP &avpLogicalAccessId()                         	{ return m_avp_logical_access_id; }
    FDDictionaryEntryAVP &avpDynamicAddressFlagExtension()             	{ return m_avp_dynamic_address_flag_extension; }
    FDDictionaryEntryAVP &avpResourceAllocationNotification()          	{ return m_avp_resource_allocation_notification; }
    FDDictionaryEntryAVP &avpRuleDeactivationTime()                    	{ return m_avp_rule_deactivation_time; }
    FDDictionaryEntryAVP &avpFlowStatus()                              	{ return m_avp_flow_status; }
    FDDictionaryEntryAVP &avpPriorityLevel()                           	{ return m_avp_priority_level; }
    FDDictionaryEntryAVP &avpPreEmptionVulnerability()                 	{ return m_avp_pre_emption_vulnerability; }
    FDDictionaryEntryAVP &avpBearerUsage()                             	{ return m_avp_bearer_usage; }
    FDDictionaryEntryAVP &avpReportingReason()                         	{ return m_avp_reporting_reason; }
    FDDictionaryEntryAVP &avpQosClassIdentifier()                      	{ return m_avp_qos_class_identifier; }
    FDDictionaryEntryAVP &avpChargingCorrelationIndicator()            	{ return m_avp_charging_correlation_indicator; }
    FDDictionaryEntryAVP &avpAreaScope()                               	{ return m_avp_area_scope; }
    FDDictionaryEntryAVP &avpTraceNeTypeList()                         	{ return m_avp_trace_ne_type_list; }
    FDDictionaryEntryAVP &avpEUtranCellGlobalIdentity()                	{ return m_avp_e_utran_cell_global_identity; }
    FDDictionaryEntryAVP &avpNbifomSupport()                           	{ return m_avp_nbifom_support; }
    FDDictionaryEntryAVP &avpUsageMonitoringLevel()                    	{ return m_avp_usage_monitoring_level; }
    FDDictionaryEntryAVP &avpRedirectHostUsage()                       	{ return m_avp_redirect_host_usage; }
    FDDictionaryEntryAVP &avpAnGwAddress()                             	{ return m_avp_an_gw_address; }
    FDDictionaryEntryAVP &avpChargingRuleDefinition()                  	{ return m_avp_charging_rule_definition; }
    FDDictionaryEntryAVP &avpTrafficSteeringPolicyIdentifierDl()       	{ return m_avp_traffic_steering_policy_identifier_dl; }
    FDDictionaryEntryAVP &avpSecondaryChargingCollectionFunctionName() 	{ return m_avp_secondary_charging_collection_function_name; }
    FDDictionaryEntryAVP &avpRoutingFilter()                           	{ return m_avp_routing_filter; }
    FDDictionaryEntryAVP &avpTdfInformation()                          	{ return m_avp_tdf_information; }
    FDDictionaryEntryAVP &avpDestinationHost()                         	{ return m_avp_destination_host; }
    FDDictionaryEntryAVP &avpLocationAreaIdentity()                    	{ return m_avp_location_area_identity; }
    FDDictionaryEntryAVP &avpLoggingInterval()                         	{ return m_avp_logging_interval; }
    FDDictionaryEntryAVP &avpReAuthRequestType()                       	{ return m_avp_re_auth_request_type; }
    FDDictionaryEntryAVP &avpFlowInformation()                         	{ return m_avp_flow_information; }
    FDDictionaryEntryAVP &avpUeLocalIpAddress()                        	{ return m_avp_ue_local_ip_address; }
    FDDictionaryEntryAVP &avpTunnelInformation()                       	{ return m_avp_tunnel_information; }
    FDDictionaryEntryAVP &avpSourceid()                                	{ return m_avp_sourceid; }
    FDDictionaryEntryAVP &avpRequiredAccessInfo()                      	{ return m_avp_required_access_info; }
    FDDictionaryEntryAVP &avpGuaranteedBitrateUl()                     	{ return m_avp_guaranteed_bitrate_ul; }
    FDDictionaryEntryAVP &avpEventChargingTimestamp()                  	{ return m_avp_event_charging_timestamp; }
    FDDictionaryEntryAVP &avpChargingRuleInstall()                     	{ return m_avp_charging_rule_install; }
    FDDictionaryEntryAVP &avp3gppGgsnAddress()                         	{ return m_avp_3gpp_ggsn_address; }
    FDDictionaryEntryAVP &avpApplicationServiceProviderIdentity()      	{ return m_avp_application_service_provider_identity; }
    FDDictionaryEntryAVP &avpContentVersion()                          	{ return m_avp_content_version; }
    FDDictionaryEntryAVP &avpEventReportIndication()                   	{ return m_avp_event_report_indication; }
    FDDictionaryEntryAVP &avpJobType()                                 	{ return m_avp_job_type; }
    FDDictionaryEntryAVP &avpBearerOperation()                         	{ return m_avp_bearer_operation; }
    FDDictionaryEntryAVP &avpUserEquipmentInfoType()                   	{ return m_avp_user_equipment_info_type; }
    FDDictionaryEntryAVP &avpRedirectServerAddress()                   	{ return m_avp_redirect_server_address; }
    FDDictionaryEntryAVP &avpCcRequestNumber()                         	{ return m_avp_cc_request_number; }
    FDDictionaryEntryAVP &avpFramedIpv6Prefix()                        	{ return m_avp_framed_ipv6_prefix; }
    FDDictionaryEntryAVP &avpTdfApplicationInstanceIdentifier()        	{ return m_avp_tdf_application_instance_identifier; }
    FDDictionaryEntryAVP &avp3gppChargingCharacteristics()             	{ return m_avp_3gpp_charging_characteristics; }
    FDDictionaryEntryAVP &avpProxyInfo()                               	{ return m_avp_proxy_info; }
    FDDictionaryEntryAVP &avpUsedServiceUnit()                         	{ return m_avp_used_service_unit; }
    FDDictionaryEntryAVP &avp3gppSgsnIpv6Address()                     	{ return m_avp_3gpp_sgsn_ipv6_address; }
    FDDictionaryEntryAVP &avpOriginRealm()                             	{ return m_avp_origin_realm; }
    FDDictionaryEntryAVP &avpTwanIdentifier()                          	{ return m_avp_twan_identifier; }
    FDDictionaryEntryAVP &avpOriginHost()                              	{ return m_avp_origin_host; }
    FDDictionaryEntryAVP &avpFlowLabel()                               	{ return m_avp_flow_label; }
    FDDictionaryEntryAVP &avp3gppGgsnIpv6Address()                     	{ return m_avp_3gpp_ggsn_ipv6_address; }
    FDDictionaryEntryAVP &avpGuaranteedBitrateDl()                     	{ return m_avp_guaranteed_bitrate_dl; }
    FDDictionaryEntryAVP &avpRestrictionFilterRule()                   	{ return m_avp_restriction_filter_rule; }
    FDDictionaryEntryAVP &avp3gppSgsnAddress()                         	{ return m_avp_3gpp_sgsn_address; }
    FDDictionaryEntryAVP &avpRedirectAddressType()                     	{ return m_avp_redirect_address_type; }
    FDDictionaryEntryAVP &avpTdfDestinationRealm()                     	{ return m_avp_tdf_destination_realm; }
    FDDictionaryEntryAVP &avpEventThresholdRsrq()                      	{ return m_avp_event_threshold_rsrq; }
    FDDictionaryEntryAVP &avpSubscriptionIdData()                      	{ return m_avp_subscription_id_data; }
    FDDictionaryEntryAVP &avpCcRequestType()                           	{ return m_avp_cc_request_type; }
    FDDictionaryEntryAVP &avpNbifomMode()                              	{ return m_avp_nbifom_mode; }
    FDDictionaryEntryAVP &avpFinalUnitIndication()                     	{ return m_avp_final_unit_indication; }
    FDDictionaryEntryAVP &avpUsageMonitoringReport()                   	{ return m_avp_usage_monitoring_report; }
    FDDictionaryEntryAVP &avpSessionReleaseCause()                     	{ return m_avp_session_release_cause; }
    FDDictionaryEntryAVP &avpFlowDescription()                         	{ return m_avp_flow_description; }
    FDDictionaryEntryAVP &avpChargingRuleBaseName()                    	{ return m_avp_charging_rule_base_name; }
    FDDictionaryEntryAVP &avpTraceCollectionEntity()                   	{ return m_avp_trace_collection_entity; }
    FDDictionaryEntryAVP &avpUserEquipmentInfoValue()                  	{ return m_avp_user_equipment_info_value; }
    FDDictionaryEntryAVP &avpRouteRecord()                             	{ return m_avp_route_record; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaIdentifier()         	{ return m_avp_presence_reporting_area_identifier; }
    FDDictionaryEntryAVP &avpCsgInformationReporting()                 	{ return m_avp_csg_information_reporting; }
    FDDictionaryEntryAVP &avpFilterId()                                	{ return m_avp_filter_id; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaInformation()        	{ return m_avp_presence_reporting_area_information; }
    FDDictionaryEntryAVP &avpCsgId()                                   	{ return m_avp_csg_id; }
    FDDictionaryEntryAVP &avpSsid()                                    	{ return m_avp_ssid; }
    FDDictionaryEntryAVP &avpMeteringMethod()                          	{ return m_avp_metering_method; }
    FDDictionaryEntryAVP &avpFramedIpAddress()                         	{ return m_avp_framed_ip_address; }
    FDDictionaryEntryAVP &avp3gppPsDataOffStatus()                     	{ return m_avp_3gpp_ps_data_off_status; }
    FDDictionaryEntryAVP &avpLoggingDuration()                         	{ return m_avp_logging_duration; }
    FDDictionaryEntryAVP &avpApnAggregateMaxBitrateDl()                	{ return m_avp_apn_aggregate_max_bitrate_dl; }
    FDDictionaryEntryAVP &avpConditionalApnAggregateMaxBitrate()       	{ return m_avp_conditional_apn_aggregate_max_bitrate; }
    FDDictionaryEntryAVP &avpAccessNetworkChargingIdentifierGx()       	{ return m_avp_access_network_charging_identifier_gx; }
    FDDictionaryEntryAVP &avpPositioningMethod()                       	{ return m_avp_positioning_method; }
    FDDictionaryEntryAVP &avpOcOlr()                                   	{ return m_avp_oc_olr; }
    FDDictionaryEntryAVP &avpRoutingRuleInstall()                      	{ return m_avp_routing_rule_install; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaStatus()             	{ return m_avp_presence_reporting_area_status; }
    FDDictionaryEntryAVP &avpTraceData()                               	{ return m_avp_trace_data; }
    FDDictionaryEntryAVP &avpAnGwStatus()                              	{ return m_avp_an_gw_status; }
    FDDictionaryEntryAVP &avpExecutionTime()                           	{ return m_avp_execution_time; }
    FDDictionaryEntryAVP &avpSubscriptionIdType()                      	{ return m_avp_subscription_id_type; }
    FDDictionaryEntryAVP &avpAccessNetworkChargingIdentifierValue()    	{ return m_avp_access_network_charging_identifier_value; }
    FDDictionaryEntryAVP &avpCreditManagementStatus()                  	{ return m_avp_credit_management_status; }
    FDDictionaryEntryAVP &avpExponent()                                	{ return m_avp_exponent; }
    FDDictionaryEntryAVP &avpOcReductionPercentage()                   	{ return m_avp_oc_reduction_percentage; }
    FDDictionaryEntryAVP &avpDefaultQosName()                          	{ return m_avp_default_qos_name; }
    FDDictionaryEntryAVP &avpRoutingRuleDefinition()                   	{ return m_avp_routing_rule_definition; }
    FDDictionaryEntryAVP &avpReportInterval()                          	{ return m_avp_report_interval; }
    FDDictionaryEntryAVP &avpMdtConfiguration()                        	{ return m_avp_mdt_configuration; }
    FDDictionaryEntryAVP &avpErrorReportingHost()                      	{ return m_avp_error_reporting_host; }
    FDDictionaryEntryAVP &avpChargingRuleRemove()                      	{ return m_avp_charging_rule_remove; }
    FDDictionaryEntryAVP &avp3gppSgsnMccMnc()                          	{ return m_avp_3gpp_sgsn_mcc_mnc; }
    FDDictionaryEntryAVP &avpUnitValue()                               	{ return m_avp_unit_value; }
    FDDictionaryEntryAVP &avpBearerControlMode()                       	{ return m_avp_bearer_control_mode; }
    FDDictionaryEntryAVP &avpEventThresholdEvent1i()                   	{ return m_avp_event_threshold_event_1i; }
    FDDictionaryEntryAVP &avpRatingGroup()                             	{ return m_avp_rating_group; }
    FDDictionaryEntryAVP &avpRatType()                                 	{ return m_avp_rat_type; }
    FDDictionaryEntryAVP &avpOnline()                                  	{ return m_avp_online; }
    FDDictionaryEntryAVP &avpDefaultAccess()                           	{ return m_avp_default_access; }
    FDDictionaryEntryAVP &avpEventThresholdEvent1f()                   	{ return m_avp_event_threshold_event_1f; }
    FDDictionaryEntryAVP &avpReportingLevel()                          	{ return m_avp_reporting_level; }
    FDDictionaryEntryAVP &avpAllocationRetentionPriority()             	{ return m_avp_allocation_retention_priority; }
    FDDictionaryEntryAVP &avpCoaIpAddress()                            	{ return m_avp_coa_ip_address; }
    FDDictionaryEntryAVP &avpCellGlobalIdentity()                      	{ return m_avp_cell_global_identity; }
    FDDictionaryEntryAVP &avpAccessAvailabilityChangeReason()          	{ return m_avp_access_availability_change_reason; }
    FDDictionaryEntryAVP &avpOcValidityDuration()                      	{ return m_avp_oc_validity_duration; }
    FDDictionaryEntryAVP &avpCsgMembershipIndication()                 	{ return m_avp_csg_membership_indication; }
    FDDictionaryEntryAVP &avpMediaComponentStatus()                    	{ return m_avp_media_component_status; }
    FDDictionaryEntryAVP &avpSharingKeyDl()                            	{ return m_avp_sharing_key_dl; }
    FDDictionaryEntryAVP &avpDefaultEpsBearerQos()                     	{ return m_avp_default_eps_bearer_qos; }
    FDDictionaryEntryAVP &avpExperimentalResult()                      	{ return m_avp_experimental_result; }
    FDDictionaryEntryAVP &avpUserEquipmentInfo()                       	{ return m_avp_user_equipment_info; }
    FDDictionaryEntryAVP &avpRedirectSupport()                         	{ return m_avp_redirect_support; }
    FDDictionaryEntryAVP &avpMeasurementPeriodLte()                    	{ return m_avp_measurement_period_lte; }
    FDDictionaryEntryAVP &avpRoutingRuleReport()                       	{ return m_avp_routing_rule_report; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthDl()                 	{ return m_avp_max_requested_bandwidth_dl; }
    FDDictionaryEntryAVP &avpQuotaConsumptionTime()                    	{ return m_avp_quota_consumption_time; }
    FDDictionaryEntryAVP &avpOriginStateId()                           	{ return m_avp_origin_state_id; }
    FDDictionaryEntryAVP &avpQosNegotiation()                          	{ return m_avp_qos_negotiation; }
    FDDictionaryEntryAVP &avpMonitoringKey()                           	{ return m_avp_monitoring_key; }
    FDDictionaryEntryAVP &avpRanNasReleaseCause()                      	{ return m_avp_ran_nas_release_cause; }
    FDDictionaryEntryAVP &avpSharingKeyUl()                            	{ return m_avp_sharing_key_ul; }
    FDDictionaryEntryAVP &avpNetlocAccessSupport()                     	{ return m_avp_netloc_access_support; }
    FDDictionaryEntryAVP &avpTraceEventList()                          	{ return m_avp_trace_event_list; }
    FDDictionaryEntryAVP &avp3gppUserLocationInfo()                    	{ return m_avp_3gpp_user_location_info; }
    FDDictionaryEntryAVP &avpResultCode()                              	{ return m_avp_result_code; }
    FDDictionaryEntryAVP &avpTraceInterfaceList()                      	{ return m_avp_trace_interface_list; }
    FDDictionaryEntryAVP &avpFixedUserLocationInfo()                   	{ return m_avp_fixed_user_location_info; }
    FDDictionaryEntryAVP &avpDefaultQosInformation()                   	{ return m_avp_default_qos_information; }
    FDDictionaryEntryAVP &avpRoutingRuleIdentifier()                   	{ return m_avp_routing_rule_identifier; }
    FDDictionaryEntryAVP &avpRedirectMaxCacheTime()                    	{ return m_avp_redirect_max_cache_time; }
    FDDictionaryEntryAVP &avpRuleActivationTime()                      	{ return m_avp_rule_activation_time; }
    FDDictionaryEntryAVP &avpRoutingRuleRemove()                       	{ return m_avp_routing_rule_remove; }
    FDDictionaryEntryAVP &avpRedirectServer()                          	{ return m_avp_redirect_server; }
    FDDictionaryEntryAVP &avpAnTrusted()                               	{ return m_avp_an_trusted; }
    FDDictionaryEntryAVP &avpPrecedence()                              	{ return m_avp_precedence; }
    FDDictionaryEntryAVP &avpCalledStationId()                         	{ return m_avp_called_station_id; }
    FDDictionaryEntryAVP &avpMaximumWaitTime()                         	{ return m_avp_maximum_wait_time; }
    FDDictionaryEntryAVP &avp3gppSelectionMode()                       	{ return m_avp_3gpp_selection_mode; }
    FDDictionaryEntryAVP &avpPccRuleStatus()                           	{ return m_avp_pcc_rule_status; }
    FDDictionaryEntryAVP &avpOcSupportedFeatures()                     	{ return m_avp_oc_supported_features; }
    FDDictionaryEntryAVP &avpPacketFilterIdentifier()                  	{ return m_avp_packet_filter_identifier; }
    FDDictionaryEntryAVP &avpCcOutputOctets()                          	{ return m_avp_cc_output_octets; }
    FDDictionaryEntryAVP &avpPacketFilterOperation()                   	{ return m_avp_packet_filter_operation; }
    FDDictionaryEntryAVP &avpProxyHost()                               	{ return m_avp_proxy_host; }
    FDDictionaryEntryAVP &avpEventThresholdRsrp()                      	{ return m_avp_event_threshold_rsrp; }
    FDDictionaryEntryAVP &avpUserLocationInfoTime()                    	{ return m_avp_user_location_info_time; }
    FDDictionaryEntryAVP &avpPacketFilterInformation()                 	{ return m_avp_packet_filter_information; }
    FDDictionaryEntryAVP &avpMonitoringFlags()                         	{ return m_avp_monitoring_flags; }
    FDDictionaryEntryAVP &avpExperimentalResultCode()                  	{ return m_avp_experimental_result_code; }
    FDDictionaryEntryAVP &avpCollectionPeriodRrmLte()                  	{ return m_avp_collection_period_rrm_lte; }
    FDDictionaryEntryAVP &avpPdnConnectionId()                         	{ return m_avp_pdn_connection_id; }
    FDDictionaryEntryAVP &avpAccessNetworkChargingAddress()            	{ return m_avp_access_network_charging_address; }
    FDDictionaryEntryAVP &avpAuthApplicationId()                       	{ return m_avp_auth_application_id; }
    FDDictionaryEntryAVP &avpRevalidationTime()                        	{ return m_avp_revalidation_time; }
    FDDictionaryEntryAVP &avpValueDigits()                             	{ return m_avp_value_digits; }
    FDDictionaryEntryAVP &avpEventTrigger()                            	{ return m_avp_event_trigger; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaElementsList()       	{ return m_avp_presence_reporting_area_elements_list; }
    FDDictionaryEntryAVP &avpChargingInformation()                     	{ return m_avp_charging_information; }
    FDDictionaryEntryAVP &avpTftPacketFilterInformation()              	{ return m_avp_tft_packet_filter_information; }
    FDDictionaryEntryAVP &avp3gppMsTimezone()                          	{ return m_avp_3gpp_ms_timezone; }
    FDDictionaryEntryAVP &avpChargingRuleName()                        	{ return m_avp_charging_rule_name; }
    FDDictionaryEntryAVP &avp3gpp2Bsid()                               	{ return m_avp_3gpp2_bsid; }
    FDDictionaryEntryAVP &avpDynamicAddressFlag()                      	{ return m_avp_dynamic_address_flag; }
    FDDictionaryEntryAVP &avpSubscriptionId()                          	{ return m_avp_subscription_id; }
    FDDictionaryEntryAVP &avpCollectionPeriodRrmUmts()                 	{ return m_avp_collection_period_rrm_umts; }
    FDDictionaryEntryAVP &avpChargingRuleReport()                      	{ return m_avp_charging_rule_report; }
    FDDictionaryEntryAVP &avpIpCanType()                               	{ return m_avp_ip_can_type; }
    FDDictionaryEntryAVP &avpOffline()                                 	{ return m_avp_offline; }
    FDDictionaryEntryAVP &avpUdpSourcePort()                           	{ return m_avp_udp_source_port; }
    FDDictionaryEntryAVP &avpRoutingIpAddress()                        	{ return m_avp_routing_ip_address; }
    FDDictionaryEntryAVP &avpRedirectInformation()                     	{ return m_avp_redirect_information; }
    FDDictionaryEntryAVP &avpPrimaryEventChargingFunctionName()        	{ return m_avp_primary_event_charging_function_name; }
    FDDictionaryEntryAVP &avpMediaComponentNumber()                    	{ return m_avp_media_component_number; }
    FDDictionaryEntryAVP &avpTariffTimeChange()                        	{ return m_avp_tariff_time_change; }
    FDDictionaryEntryAVP &avpErrorMessage()                            	{ return m_avp_error_message; }
    FDDictionaryEntryAVP &avpQosUpgrade()                              	{ return m_avp_qos_upgrade; }
    FDDictionaryEntryAVP &avpIpCanSessionChargingScope()               	{ return m_avp_ip_can_session_charging_scope; }
    FDDictionaryEntryAVP &avpReportingTrigger()                        	{ return m_avp_reporting_trigger; }
    FDDictionaryEntryAVP &avpFailedAvp()                               	{ return m_avp_failed_avp; }
    FDDictionaryEntryAVP &avpRoutingAreaIdentity()                     	{ return m_avp_routing_area_identity; }
    FDDictionaryEntryAVP &avpUsageMonitoringInformation()              	{ return m_avp_usage_monitoring_information; }
    FDDictionaryEntryAVP &avpCcTotalOctets()                           	{ return m_avp_cc_total_octets; }
    FDDictionaryEntryAVP &avpSecurityParameterIndex()                  	{ return m_avp_security_parameter_index; }
    FDDictionaryEntryAVP &avpTraceReference()                          	{ return m_avp_trace_reference; }
    FDDictionaryEntryAVP &avpFlowDirection()                           	{ return m_avp_flow_direction; }
    FDDictionaryEntryAVP &avpTdfApplicationIdentifier()                	{ return m_avp_tdf_application_identifier; }
    FDDictionaryEntryAVP &avpCurrencyCode()                            	{ return m_avp_currency_code; }
    FDDictionaryEntryAVP &avpCcInputOctets()                           	{ return m_avp_cc_input_octets; }
    FDDictionaryEntryAVP &avpMeasurementQuantity()                     	{ return m_avp_measurement_quantity; }
    FDDictionaryEntryAVP &avpRemovalOfAccess()                         	{ return m_avp_removal_of_access; }
    FDDictionaryEntryAVP &avpMeasurementPeriodUmts()                   	{ return m_avp_measurement_period_umts; }
    FDDictionaryEntryAVP &avpTraceDepth()                              	{ return m_avp_trace_depth; }
    FDDictionaryEntryAVP &avpProxyState()                              	{ return m_avp_proxy_state; }
    FDDictionaryEntryAVP &avpRuleFailureCode()                         	{ return m_avp_rule_failure_code; }
    FDDictionaryEntryAVP &avpAfChargingIdentifier()                    	{ return m_avp_af_charging_identifier; }
    FDDictionaryEntryAVP &avpTunnelHeaderLength()                      	{ return m_avp_tunnel_header_length; }
    FDDictionaryEntryAVP &avpRoutingRuleFailureCode()                  	{ return m_avp_routing_rule_failure_code; }
    FDDictionaryEntryAVP &avpCoaInformation()                          	{ return m_avp_coa_information; }
    FDDictionaryEntryAVP &avpDefaultBearerIndication()                 	{ return m_avp_default_bearer_indication; }
    FDDictionaryEntryAVP &avpVendorId()                                	{ return m_avp_vendor_id; }
    FDDictionaryEntryAVP &avpGrantedServiceUnit()                      	{ return m_avp_granted_service_unit; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthUl()                 	{ return m_avp_max_requested_bandwidth_ul; }
    FDDictionaryEntryAVP &avpSupportedFeatures()                       	{ return m_avp_supported_features; }
    FDDictionaryEntryAVP &avpLoadType()                                	{ return m_avp_load_type; }
    FDDictionaryEntryAVP &avpRedirectHost()                            	{ return m_avp_redirect_host; }
    FDDictionaryEntryAVP &avpDestinationRealm()                        	{ return m_avp_destination_realm; }
    FDDictionaryEntryAVP &avpSessionId()                               	{ return m_avp_session_id; }
    FDDictionaryEntryAVP &avpTosTrafficClass()                         	{ return m_avp_tos_traffic_class; }
    FDDictionaryEntryAVP &avpOriginationTimeStamp()                    	{ return m_avp_origination_time_stamp; }
    FDDictionaryEntryAVP &avpBssid()                                   	{ return m_avp_bssid; }
    FDDictionaryEntryAVP &avpCcMoney()                                 	{ return m_avp_cc_money; }
    FDDictionaryEntryAVP &avpApplicationDetectionInformation()         	{ return m_avp_application_detection_information; }
    FDDictionaryEntryAVP &avpMdtAllowedPlmnId()                        	{ return m_avp_mdt_allowed_plmn_id; }
    FDDictionaryEntryAVP &avpTariffChangeUsage()                       	{ return m_avp_tariff_change_usage; }
    FDDictionaryEntryAVP &avpReportAmount()                            	{ return m_avp_report_amount; }
    FDDictionaryEntryAVP &avpMuteNotification()                        	{ return m_avp_mute_notification; }
    FDDictionaryEntryAVP &avpPdnConnectionChargingId()                 	{ return m_avp_pdn_connection_charging_id; }
    FDDictionaryEntryAVP &avpTdfIpAddress()                            	{ return m_avp_tdf_ip_address; }
    FDDictionaryEntryAVP &avpFlows()                                   	{ return m_avp_flows; }
    FDDictionaryEntryAVP &avpPsToCsSessionContinuity()                 	{ return m_avp_ps_to_cs_session_continuity; }
    FDDictionaryEntryAVP &avpPrimaryChargingCollectionFunctionName()   	{ return m_avp_primary_charging_collection_function_name; }
    FDDictionaryEntryAVP &avpUserCsgInformation()                      	{ return m_avp_user_csg_information; }

private:
    FDDictionaryEntryApplication m_app;                                

    FDDictionaryEntryCommand m_cmd_crcr;
    FDDictionaryEntryCommand m_cmd_crca;
    FDDictionaryEntryCommand m_cmd_rear;
    FDDictionaryEntryCommand m_cmd_reaa;

    FDDictionaryEntryVendor m_vnd_Etsi;
    FDDictionaryEntryVendor m_vnd_3gpp2;
    FDDictionaryEntryVendor m_vnd_3gpp;

    FDDictionaryEntryAVP m_avp_tdf_destination_host;
    FDDictionaryEntryAVP m_avp_pre_emption_capability;
    FDDictionaryEntryAVP m_avp_packet_filter_content;
    FDDictionaryEntryAVP m_avp_feature_list_id;
    FDDictionaryEntryAVP m_avp_resource_release_notification;
    FDDictionaryEntryAVP m_avp_ran_rule_support;
    FDDictionaryEntryAVP m_avp_service_identifier;
    FDDictionaryEntryAVP m_avp_physical_access_id;
    FDDictionaryEntryAVP m_avp_csg_access_mode;
    FDDictionaryEntryAVP m_avp_henb_local_ip_address;
    FDDictionaryEntryAVP m_avp_flow_number;
    FDDictionaryEntryAVP m_avp_bearer_identifier;
    FDDictionaryEntryAVP m_avp_apn_aggregate_max_bitrate_ul;
    FDDictionaryEntryAVP m_avp_network_request_support;
    FDDictionaryEntryAVP m_avp_termination_cause;
    FDDictionaryEntryAVP m_avp_traffic_steering_policy_identifier_ul;
    FDDictionaryEntryAVP m_avp_sponsor_identity;
    FDDictionaryEntryAVP m_avp_3gpp_rat_type;
    FDDictionaryEntryAVP m_avp_af_signalling_protocol;
    FDDictionaryEntryAVP m_avp_packet_filter_usage;
    FDDictionaryEntryAVP m_avp_oc_sequence_number;
    FDDictionaryEntryAVP m_avp_usage_monitoring_support;
    FDDictionaryEntryAVP m_avp_tracking_area_identity;
    FDDictionaryEntryAVP m_avp_load_value;
    FDDictionaryEntryAVP m_avp_feature_list;
    FDDictionaryEntryAVP m_avp_omc_id;
    FDDictionaryEntryAVP m_avp_rai;
    FDDictionaryEntryAVP m_avp_oc_report_type;
    FDDictionaryEntryAVP m_avp_tft_filter;
    FDDictionaryEntryAVP m_avp_secondary_event_charging_function_name;
    FDDictionaryEntryAVP m_avp_pcscf_restoration_indication;
    FDDictionaryEntryAVP m_avp_load;
    FDDictionaryEntryAVP m_avp_oc_feature_vector;
    FDDictionaryEntryAVP m_avp_cc_service_specific_units;
    FDDictionaryEntryAVP m_avp_pra_remove;
    FDDictionaryEntryAVP m_avp_cc_time;
    FDDictionaryEntryAVP m_avp_list_of_measurements;
    FDDictionaryEntryAVP m_avp_qos_information;
    FDDictionaryEntryAVP m_avp_final_unit_action;
    FDDictionaryEntryAVP m_avp_conditional_policy_information;
    FDDictionaryEntryAVP m_avp_drmp;
    FDDictionaryEntryAVP m_avp_pra_install;
    FDDictionaryEntryAVP m_avp_logical_access_id;
    FDDictionaryEntryAVP m_avp_dynamic_address_flag_extension;
    FDDictionaryEntryAVP m_avp_resource_allocation_notification;
    FDDictionaryEntryAVP m_avp_rule_deactivation_time;
    FDDictionaryEntryAVP m_avp_flow_status;
    FDDictionaryEntryAVP m_avp_priority_level;
    FDDictionaryEntryAVP m_avp_pre_emption_vulnerability;
    FDDictionaryEntryAVP m_avp_bearer_usage;
    FDDictionaryEntryAVP m_avp_reporting_reason;
    FDDictionaryEntryAVP m_avp_qos_class_identifier;
    FDDictionaryEntryAVP m_avp_charging_correlation_indicator;
    FDDictionaryEntryAVP m_avp_area_scope;
    FDDictionaryEntryAVP m_avp_trace_ne_type_list;
    FDDictionaryEntryAVP m_avp_e_utran_cell_global_identity;
    FDDictionaryEntryAVP m_avp_nbifom_support;
    FDDictionaryEntryAVP m_avp_usage_monitoring_level;
    FDDictionaryEntryAVP m_avp_redirect_host_usage;
    FDDictionaryEntryAVP m_avp_an_gw_address;
    FDDictionaryEntryAVP m_avp_charging_rule_definition;
    FDDictionaryEntryAVP m_avp_traffic_steering_policy_identifier_dl;
    FDDictionaryEntryAVP m_avp_secondary_charging_collection_function_name;
    FDDictionaryEntryAVP m_avp_routing_filter;
    FDDictionaryEntryAVP m_avp_tdf_information;
    FDDictionaryEntryAVP m_avp_destination_host;
    FDDictionaryEntryAVP m_avp_location_area_identity;
    FDDictionaryEntryAVP m_avp_logging_interval;
    FDDictionaryEntryAVP m_avp_re_auth_request_type;
    FDDictionaryEntryAVP m_avp_flow_information;
    FDDictionaryEntryAVP m_avp_ue_local_ip_address;
    FDDictionaryEntryAVP m_avp_tunnel_information;
    FDDictionaryEntryAVP m_avp_sourceid;
    FDDictionaryEntryAVP m_avp_required_access_info;
    FDDictionaryEntryAVP m_avp_guaranteed_bitrate_ul;
    FDDictionaryEntryAVP m_avp_event_charging_timestamp;
    FDDictionaryEntryAVP m_avp_charging_rule_install;
    FDDictionaryEntryAVP m_avp_3gpp_ggsn_address;
    FDDictionaryEntryAVP m_avp_application_service_provider_identity;
    FDDictionaryEntryAVP m_avp_content_version;
    FDDictionaryEntryAVP m_avp_event_report_indication;
    FDDictionaryEntryAVP m_avp_job_type;
    FDDictionaryEntryAVP m_avp_bearer_operation;
    FDDictionaryEntryAVP m_avp_user_equipment_info_type;
    FDDictionaryEntryAVP m_avp_redirect_server_address;
    FDDictionaryEntryAVP m_avp_cc_request_number;
    FDDictionaryEntryAVP m_avp_framed_ipv6_prefix;
    FDDictionaryEntryAVP m_avp_tdf_application_instance_identifier;
    FDDictionaryEntryAVP m_avp_3gpp_charging_characteristics;
    FDDictionaryEntryAVP m_avp_proxy_info;
    FDDictionaryEntryAVP m_avp_used_service_unit;
    FDDictionaryEntryAVP m_avp_3gpp_sgsn_ipv6_address;
    FDDictionaryEntryAVP m_avp_origin_realm;
    FDDictionaryEntryAVP m_avp_twan_identifier;
    FDDictionaryEntryAVP m_avp_origin_host;
    FDDictionaryEntryAVP m_avp_flow_label;
    FDDictionaryEntryAVP m_avp_3gpp_ggsn_ipv6_address;
    FDDictionaryEntryAVP m_avp_guaranteed_bitrate_dl;
    FDDictionaryEntryAVP m_avp_restriction_filter_rule;
    FDDictionaryEntryAVP m_avp_3gpp_sgsn_address;
    FDDictionaryEntryAVP m_avp_redirect_address_type;
    FDDictionaryEntryAVP m_avp_tdf_destination_realm;
    FDDictionaryEntryAVP m_avp_event_threshold_rsrq;
    FDDictionaryEntryAVP m_avp_subscription_id_data;
    FDDictionaryEntryAVP m_avp_cc_request_type;
    FDDictionaryEntryAVP m_avp_nbifom_mode;
    FDDictionaryEntryAVP m_avp_final_unit_indication;
    FDDictionaryEntryAVP m_avp_usage_monitoring_report;
    FDDictionaryEntryAVP m_avp_session_release_cause;
    FDDictionaryEntryAVP m_avp_flow_description;
    FDDictionaryEntryAVP m_avp_charging_rule_base_name;
    FDDictionaryEntryAVP m_avp_trace_collection_entity;
    FDDictionaryEntryAVP m_avp_user_equipment_info_value;
    FDDictionaryEntryAVP m_avp_route_record;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_identifier;
    FDDictionaryEntryAVP m_avp_csg_information_reporting;
    FDDictionaryEntryAVP m_avp_filter_id;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_information;
    FDDictionaryEntryAVP m_avp_csg_id;
    FDDictionaryEntryAVP m_avp_ssid;
    FDDictionaryEntryAVP m_avp_metering_method;
    FDDictionaryEntryAVP m_avp_framed_ip_address;
    FDDictionaryEntryAVP m_avp_3gpp_ps_data_off_status;
    FDDictionaryEntryAVP m_avp_logging_duration;
    FDDictionaryEntryAVP m_avp_apn_aggregate_max_bitrate_dl;
    FDDictionaryEntryAVP m_avp_conditional_apn_aggregate_max_bitrate;
    FDDictionaryEntryAVP m_avp_access_network_charging_identifier_gx;
    FDDictionaryEntryAVP m_avp_positioning_method;
    FDDictionaryEntryAVP m_avp_oc_olr;
    FDDictionaryEntryAVP m_avp_routing_rule_install;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_status;
    FDDictionaryEntryAVP m_avp_trace_data;
    FDDictionaryEntryAVP m_avp_an_gw_status;
    FDDictionaryEntryAVP m_avp_execution_time;
    FDDictionaryEntryAVP m_avp_subscription_id_type;
    FDDictionaryEntryAVP m_avp_access_network_charging_identifier_value;
    FDDictionaryEntryAVP m_avp_credit_management_status;
    FDDictionaryEntryAVP m_avp_exponent;
    FDDictionaryEntryAVP m_avp_oc_reduction_percentage;
    FDDictionaryEntryAVP m_avp_default_qos_name;
    FDDictionaryEntryAVP m_avp_routing_rule_definition;
    FDDictionaryEntryAVP m_avp_report_interval;
    FDDictionaryEntryAVP m_avp_mdt_configuration;
    FDDictionaryEntryAVP m_avp_error_reporting_host;
    FDDictionaryEntryAVP m_avp_charging_rule_remove;
    FDDictionaryEntryAVP m_avp_3gpp_sgsn_mcc_mnc;
    FDDictionaryEntryAVP m_avp_unit_value;
    FDDictionaryEntryAVP m_avp_bearer_control_mode;
    FDDictionaryEntryAVP m_avp_event_threshold_event_1i;
    FDDictionaryEntryAVP m_avp_rating_group;
    FDDictionaryEntryAVP m_avp_rat_type;
    FDDictionaryEntryAVP m_avp_online;
    FDDictionaryEntryAVP m_avp_default_access;
    FDDictionaryEntryAVP m_avp_event_threshold_event_1f;
    FDDictionaryEntryAVP m_avp_reporting_level;
    FDDictionaryEntryAVP m_avp_allocation_retention_priority;
    FDDictionaryEntryAVP m_avp_coa_ip_address;
    FDDictionaryEntryAVP m_avp_cell_global_identity;
    FDDictionaryEntryAVP m_avp_access_availability_change_reason;
    FDDictionaryEntryAVP m_avp_oc_validity_duration;
    FDDictionaryEntryAVP m_avp_csg_membership_indication;
    FDDictionaryEntryAVP m_avp_media_component_status;
    FDDictionaryEntryAVP m_avp_sharing_key_dl;
    FDDictionaryEntryAVP m_avp_default_eps_bearer_qos;
    FDDictionaryEntryAVP m_avp_experimental_result;
    FDDictionaryEntryAVP m_avp_user_equipment_info;
    FDDictionaryEntryAVP m_avp_redirect_support;
    FDDictionaryEntryAVP m_avp_measurement_period_lte;
    FDDictionaryEntryAVP m_avp_routing_rule_report;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_dl;
    FDDictionaryEntryAVP m_avp_quota_consumption_time;
    FDDictionaryEntryAVP m_avp_origin_state_id;
    FDDictionaryEntryAVP m_avp_qos_negotiation;
    FDDictionaryEntryAVP m_avp_monitoring_key;
    FDDictionaryEntryAVP m_avp_ran_nas_release_cause;
    FDDictionaryEntryAVP m_avp_sharing_key_ul;
    FDDictionaryEntryAVP m_avp_netloc_access_support;
    FDDictionaryEntryAVP m_avp_trace_event_list;
    FDDictionaryEntryAVP m_avp_3gpp_user_location_info;
    FDDictionaryEntryAVP m_avp_result_code;
    FDDictionaryEntryAVP m_avp_trace_interface_list;
    FDDictionaryEntryAVP m_avp_fixed_user_location_info;
    FDDictionaryEntryAVP m_avp_default_qos_information;
    FDDictionaryEntryAVP m_avp_routing_rule_identifier;
    FDDictionaryEntryAVP m_avp_redirect_max_cache_time;
    FDDictionaryEntryAVP m_avp_rule_activation_time;
    FDDictionaryEntryAVP m_avp_routing_rule_remove;
    FDDictionaryEntryAVP m_avp_redirect_server;
    FDDictionaryEntryAVP m_avp_an_trusted;
    FDDictionaryEntryAVP m_avp_precedence;
    FDDictionaryEntryAVP m_avp_called_station_id;
    FDDictionaryEntryAVP m_avp_maximum_wait_time;
    FDDictionaryEntryAVP m_avp_3gpp_selection_mode;
    FDDictionaryEntryAVP m_avp_pcc_rule_status;
    FDDictionaryEntryAVP m_avp_oc_supported_features;
    FDDictionaryEntryAVP m_avp_packet_filter_identifier;
    FDDictionaryEntryAVP m_avp_cc_output_octets;
    FDDictionaryEntryAVP m_avp_packet_filter_operation;
    FDDictionaryEntryAVP m_avp_proxy_host;
    FDDictionaryEntryAVP m_avp_event_threshold_rsrp;
    FDDictionaryEntryAVP m_avp_user_location_info_time;
    FDDictionaryEntryAVP m_avp_packet_filter_information;
    FDDictionaryEntryAVP m_avp_monitoring_flags;
    FDDictionaryEntryAVP m_avp_experimental_result_code;
    FDDictionaryEntryAVP m_avp_collection_period_rrm_lte;
    FDDictionaryEntryAVP m_avp_pdn_connection_id;
    FDDictionaryEntryAVP m_avp_access_network_charging_address;
    FDDictionaryEntryAVP m_avp_auth_application_id;
    FDDictionaryEntryAVP m_avp_revalidation_time;
    FDDictionaryEntryAVP m_avp_value_digits;
    FDDictionaryEntryAVP m_avp_event_trigger;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_elements_list;
    FDDictionaryEntryAVP m_avp_charging_information;
    FDDictionaryEntryAVP m_avp_tft_packet_filter_information;
    FDDictionaryEntryAVP m_avp_3gpp_ms_timezone;
    FDDictionaryEntryAVP m_avp_charging_rule_name;
    FDDictionaryEntryAVP m_avp_3gpp2_bsid;
    FDDictionaryEntryAVP m_avp_dynamic_address_flag;
    FDDictionaryEntryAVP m_avp_subscription_id;
    FDDictionaryEntryAVP m_avp_collection_period_rrm_umts;
    FDDictionaryEntryAVP m_avp_charging_rule_report;
    FDDictionaryEntryAVP m_avp_ip_can_type;
    FDDictionaryEntryAVP m_avp_offline;
    FDDictionaryEntryAVP m_avp_udp_source_port;
    FDDictionaryEntryAVP m_avp_routing_ip_address;
    FDDictionaryEntryAVP m_avp_redirect_information;
    FDDictionaryEntryAVP m_avp_primary_event_charging_function_name;
    FDDictionaryEntryAVP m_avp_media_component_number;
    FDDictionaryEntryAVP m_avp_tariff_time_change;
    FDDictionaryEntryAVP m_avp_error_message;
    FDDictionaryEntryAVP m_avp_qos_upgrade;
    FDDictionaryEntryAVP m_avp_ip_can_session_charging_scope;
    FDDictionaryEntryAVP m_avp_reporting_trigger;
    FDDictionaryEntryAVP m_avp_failed_avp;
    FDDictionaryEntryAVP m_avp_routing_area_identity;
    FDDictionaryEntryAVP m_avp_usage_monitoring_information;
    FDDictionaryEntryAVP m_avp_cc_total_octets;
    FDDictionaryEntryAVP m_avp_security_parameter_index;
    FDDictionaryEntryAVP m_avp_trace_reference;
    FDDictionaryEntryAVP m_avp_flow_direction;
    FDDictionaryEntryAVP m_avp_tdf_application_identifier;
    FDDictionaryEntryAVP m_avp_currency_code;
    FDDictionaryEntryAVP m_avp_cc_input_octets;
    FDDictionaryEntryAVP m_avp_measurement_quantity;
    FDDictionaryEntryAVP m_avp_removal_of_access;
    FDDictionaryEntryAVP m_avp_measurement_period_umts;
    FDDictionaryEntryAVP m_avp_trace_depth;
    FDDictionaryEntryAVP m_avp_proxy_state;
    FDDictionaryEntryAVP m_avp_rule_failure_code;
    FDDictionaryEntryAVP m_avp_af_charging_identifier;
    FDDictionaryEntryAVP m_avp_tunnel_header_length;
    FDDictionaryEntryAVP m_avp_routing_rule_failure_code;
    FDDictionaryEntryAVP m_avp_coa_information;
    FDDictionaryEntryAVP m_avp_default_bearer_indication;
    FDDictionaryEntryAVP m_avp_vendor_id;
    FDDictionaryEntryAVP m_avp_granted_service_unit;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_ul;
    FDDictionaryEntryAVP m_avp_supported_features;
    FDDictionaryEntryAVP m_avp_load_type;
    FDDictionaryEntryAVP m_avp_redirect_host;
    FDDictionaryEntryAVP m_avp_destination_realm;
    FDDictionaryEntryAVP m_avp_session_id;
    FDDictionaryEntryAVP m_avp_tos_traffic_class;
    FDDictionaryEntryAVP m_avp_origination_time_stamp;
    FDDictionaryEntryAVP m_avp_bssid;
    FDDictionaryEntryAVP m_avp_cc_money;
    FDDictionaryEntryAVP m_avp_application_detection_information;
    FDDictionaryEntryAVP m_avp_mdt_allowed_plmn_id;
    FDDictionaryEntryAVP m_avp_tariff_change_usage;
    FDDictionaryEntryAVP m_avp_report_amount;
    FDDictionaryEntryAVP m_avp_mute_notification;
    FDDictionaryEntryAVP m_avp_pdn_connection_charging_id;
    FDDictionaryEntryAVP m_avp_tdf_ip_address;
    FDDictionaryEntryAVP m_avp_flows;
    FDDictionaryEntryAVP m_avp_ps_to_cs_session_continuity;
    FDDictionaryEntryAVP m_avp_primary_charging_collection_function_name;
    FDDictionaryEntryAVP m_avp_user_csg_information;

};

//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - CreditControlRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// CRCR request from Clients.
//-
class CRCRcmd : public FDCommandRequest
{
public:
    CRCRcmd( Application &app );
    ~CRCRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    CRCRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - CreditControlRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a CRCR
// to the Diameter application and process the corresponding answer.
//
class CRCRreq : public FDMessageRequest
{
public:
    CRCRreq( Application &app );
    virtual ~CRCRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }
    void addSessionId( FDDictionaryEntryAVP &deSessionId ) { m_session.addSessionId( *this, deSessionId ); }

private:
    CRCRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - ReAuthRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// REAR request from Clients.
//-
class REARcmd : public FDCommandRequest
{
public:
    REARcmd( Application &app );
    ~REARcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    REARcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - ReAuthRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a REAR
// to the Diameter application and process the corresponding answer.
//
class REARreq : public FDMessageRequest
{
public:
    REARreq( Application &app );
    virtual ~REARreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }
    void addSessionId( FDDictionaryEntryAVP &deSessionId ) { m_session.addSessionId( *this, deSessionId ); }

private:
    REARreq();
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
class LoadExtractor : public FDExtractor
{
 public:
    LoadExtractor( FDExtractor &parent, Dictionary &dict );
    ~LoadExtractor();

    FDExtractorAvp load_type;
    FDExtractorAvp load_value;
    FDExtractorAvp sourceid;
};

class PraRemoveExtractor : public FDExtractor
{
 public:
    PraRemoveExtractor( FDExtractor &parent, Dictionary &dict );
    ~PraRemoveExtractor();

    FDExtractorAvpList presence_reporting_area_identifier;
};

class AllocationRetentionPriorityExtractor : public FDExtractor
{
 public:
    AllocationRetentionPriorityExtractor( FDExtractor &parent, Dictionary &dict );
    ~AllocationRetentionPriorityExtractor();

    FDExtractorAvp priority_level;
    FDExtractorAvp pre_emption_capability;
    FDExtractorAvp pre_emption_vulnerability;
};

class ConditionalApnAggregateMaxBitrateExtractor : public FDExtractor
{
 public:
    ConditionalApnAggregateMaxBitrateExtractor( FDExtractor &parent, Dictionary &dict );
    ~ConditionalApnAggregateMaxBitrateExtractor();

    FDExtractorAvp apn_aggregate_max_bitrate_ul;
    FDExtractorAvp apn_aggregate_max_bitrate_dl;
    FDExtractorAvpList ip_can_type;
    FDExtractorAvpList rat_type;
};

class ConditionalApnAggregateMaxBitrateExtractorList : public FDExtractorList
{
 public:
    ConditionalApnAggregateMaxBitrateExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ConditionalApnAggregateMaxBitrateExtractorList();

    FDExtractor *createExtractor() { return new ConditionalApnAggregateMaxBitrateExtractor( getParent(), m_dict ); }
    std::list<ConditionalApnAggregateMaxBitrateExtractor*> &getList() { return (std::list<ConditionalApnAggregateMaxBitrateExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class QosInformationExtractor : public FDExtractor
{
 public:
    QosInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~QosInformationExtractor();

    FDExtractorAvp qos_class_identifier;
    FDExtractorAvp max_requested_bandwidth_ul;
    FDExtractorAvp max_requested_bandwidth_dl;
    FDExtractorAvp guaranteed_bitrate_ul;
    FDExtractorAvp guaranteed_bitrate_dl;
    FDExtractorAvp bearer_identifier;
    AllocationRetentionPriorityExtractor allocation_retention_priority;
    FDExtractorAvp apn_aggregate_max_bitrate_ul;
    FDExtractorAvp apn_aggregate_max_bitrate_dl;
    ConditionalApnAggregateMaxBitrateExtractorList conditional_apn_aggregate_max_bitrate;
};

class DefaultEpsBearerQosExtractor : public FDExtractor
{
 public:
    DefaultEpsBearerQosExtractor( FDExtractor &parent, Dictionary &dict );
    ~DefaultEpsBearerQosExtractor();

    FDExtractorAvp qos_class_identifier;
    AllocationRetentionPriorityExtractor allocation_retention_priority;
};

class ConditionalPolicyInformationExtractor : public FDExtractor
{
 public:
    ConditionalPolicyInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~ConditionalPolicyInformationExtractor();

    FDExtractorAvp execution_time;
    DefaultEpsBearerQosExtractor default_eps_bearer_qos;
    FDExtractorAvp apn_aggregate_max_bitrate_ul;
    FDExtractorAvp apn_aggregate_max_bitrate_dl;
    ConditionalApnAggregateMaxBitrateExtractorList conditional_apn_aggregate_max_bitrate;
};

class PresenceReportingAreaInformationExtractor : public FDExtractor
{
 public:
    PresenceReportingAreaInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~PresenceReportingAreaInformationExtractor();

    FDExtractorAvp presence_reporting_area_identifier;
    FDExtractorAvp presence_reporting_area_status;
    FDExtractorAvp presence_reporting_area_elements_list;
};

class PresenceReportingAreaInformationExtractorList : public FDExtractorList
{
 public:
    PresenceReportingAreaInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~PresenceReportingAreaInformationExtractorList();

    FDExtractor *createExtractor() { return new PresenceReportingAreaInformationExtractor( getParent(), m_dict ); }
    std::list<PresenceReportingAreaInformationExtractor*> &getList() { return (std::list<PresenceReportingAreaInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class PraInstallExtractor : public FDExtractor
{
 public:
    PraInstallExtractor( FDExtractor &parent, Dictionary &dict );
    ~PraInstallExtractor();

    PresenceReportingAreaInformationExtractorList presence_reporting_area_information;
};

class AreaScopeExtractor : public FDExtractor
{
 public:
    AreaScopeExtractor( FDExtractor &parent, Dictionary &dict );
    ~AreaScopeExtractor();

    FDExtractorAvpList cell_global_identity;
    FDExtractorAvpList e_utran_cell_global_identity;
    FDExtractorAvpList routing_area_identity;
    FDExtractorAvpList location_area_identity;
    FDExtractorAvpList tracking_area_identity;
};

class FlowInformationExtractor : public FDExtractor
{
 public:
    FlowInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~FlowInformationExtractor();

    FDExtractorAvp flow_description;
    FDExtractorAvp packet_filter_identifier;
    FDExtractorAvp packet_filter_usage;
    FDExtractorAvp tos_traffic_class;
    FDExtractorAvp security_parameter_index;
    FDExtractorAvp flow_label;
    FDExtractorAvp flow_direction;
    FDExtractorAvp routing_rule_identifier;
};

class FlowInformationExtractorList : public FDExtractorList
{
 public:
    FlowInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~FlowInformationExtractorList();

    FDExtractor *createExtractor() { return new FlowInformationExtractor( getParent(), m_dict ); }
    std::list<FlowInformationExtractor*> &getList() { return (std::list<FlowInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class FlowsExtractor : public FDExtractor
{
 public:
    FlowsExtractor( FDExtractor &parent, Dictionary &dict );
    ~FlowsExtractor();

    FDExtractorAvp media_component_number;
    FDExtractorAvpList flow_number;
    FDExtractorAvpList content_version;
    FDExtractorAvp final_unit_action;
    FDExtractorAvp media_component_status;
};

class FlowsExtractorList : public FDExtractorList
{
 public:
    FlowsExtractorList( FDExtractor &parent, Dictionary &dict );
    ~FlowsExtractorList();

    FDExtractor *createExtractor() { return new FlowsExtractor( getParent(), m_dict ); }
    std::list<FlowsExtractor*> &getList() { return (std::list<FlowsExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class RedirectInformationExtractor : public FDExtractor
{
 public:
    RedirectInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~RedirectInformationExtractor();

    FDExtractorAvp redirect_support;
    FDExtractorAvp redirect_address_type;
    FDExtractorAvp redirect_server_address;
};

class ChargingRuleDefinitionExtractor : public FDExtractor
{
 public:
    ChargingRuleDefinitionExtractor( FDExtractor &parent, Dictionary &dict );
    ~ChargingRuleDefinitionExtractor();

    FDExtractorAvp charging_rule_name;
    FDExtractorAvp service_identifier;
    FDExtractorAvp rating_group;
    FlowInformationExtractorList flow_information;
    FDExtractorAvp default_bearer_indication;
    FDExtractorAvp tdf_application_identifier;
    FDExtractorAvp flow_status;
    QosInformationExtractor qos_information;
    FDExtractorAvp ps_to_cs_session_continuity;
    FDExtractorAvp reporting_level;
    FDExtractorAvp online;
    FDExtractorAvp offline;
    FDExtractorAvp metering_method;
    FDExtractorAvp precedence;
    FDExtractorAvp af_charging_identifier;
    FlowsExtractorList flows;
    FDExtractorAvp monitoring_key;
    RedirectInformationExtractor redirect_information;
    FDExtractorAvp mute_notification;
    FDExtractorAvp af_signalling_protocol;
    FDExtractorAvp sponsor_identity;
    FDExtractorAvp application_service_provider_identity;
    FDExtractorAvpList required_access_info;
    FDExtractorAvp sharing_key_dl;
    FDExtractorAvp sharing_key_ul;
    FDExtractorAvp traffic_steering_policy_identifier_dl;
    FDExtractorAvp traffic_steering_policy_identifier_ul;
    FDExtractorAvp content_version;
};

class RoutingFilterExtractor : public FDExtractor
{
 public:
    RoutingFilterExtractor( FDExtractor &parent, Dictionary &dict );
    ~RoutingFilterExtractor();

    FDExtractorAvp flow_description;
    FDExtractorAvp flow_direction;
    FDExtractorAvp tos_traffic_class;
    FDExtractorAvp security_parameter_index;
    FDExtractorAvp flow_label;
};

class TdfInformationExtractor : public FDExtractor
{
 public:
    TdfInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~TdfInformationExtractor();

    FDExtractorAvp tdf_destination_realm;
    FDExtractorAvp tdf_destination_host;
    FDExtractorAvp tdf_ip_address;
};

class TunnelInformationExtractor : public FDExtractor
{
 public:
    TunnelInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~TunnelInformationExtractor();

    FDExtractorAvp tunnel_header_length;
};

class ChargingRuleDefinitionExtractorList : public FDExtractorList
{
 public:
    ChargingRuleDefinitionExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ChargingRuleDefinitionExtractorList();

    FDExtractor *createExtractor() { return new ChargingRuleDefinitionExtractor( getParent(), m_dict ); }
    std::list<ChargingRuleDefinitionExtractor*> &getList() { return (std::list<ChargingRuleDefinitionExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ChargingRuleInstallExtractor : public FDExtractor
{
 public:
    ChargingRuleInstallExtractor( FDExtractor &parent, Dictionary &dict );
    ~ChargingRuleInstallExtractor();

    ChargingRuleDefinitionExtractorList charging_rule_definition;
    FDExtractorAvpList charging_rule_name;
    FDExtractorAvpList charging_rule_base_name;
    FDExtractorAvp bearer_identifier;
    FDExtractorAvp monitoring_flags;
    FDExtractorAvp rule_activation_time;
    FDExtractorAvp rule_deactivation_time;
    FDExtractorAvp resource_allocation_notification;
    FDExtractorAvp charging_correlation_indicator;
    FDExtractorAvp ip_can_type;
};

class UserCsgInformationExtractor : public FDExtractor
{
 public:
    UserCsgInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~UserCsgInformationExtractor();

    FDExtractorAvp csg_id;
    FDExtractorAvp csg_access_mode;
    FDExtractorAvp csg_membership_indication;
};

class MdtConfigurationExtractor : public FDExtractor
{
 public:
    MdtConfigurationExtractor( FDExtractor &parent, Dictionary &dict );
    ~MdtConfigurationExtractor();

    FDExtractorAvp job_type;
    AreaScopeExtractor area_scope;
    FDExtractorAvp list_of_measurements;
    FDExtractorAvp reporting_trigger;
    FDExtractorAvp report_interval;
    FDExtractorAvp report_amount;
    FDExtractorAvp event_threshold_rsrp;
    FDExtractorAvp event_threshold_rsrq;
    FDExtractorAvp logging_interval;
    FDExtractorAvp logging_duration;
    FDExtractorAvp measurement_period_lte;
    FDExtractorAvp measurement_period_umts;
    FDExtractorAvp collection_period_rrm_lte;
    FDExtractorAvp collection_period_rrm_umts;
    FDExtractorAvp positioning_method;
    FDExtractorAvp measurement_quantity;
    FDExtractorAvp event_threshold_event_1f;
    FDExtractorAvp event_threshold_event_1i;
    FDExtractorAvpList mdt_allowed_plmn_id;
};

class TraceDataExtractor : public FDExtractor
{
 public:
    TraceDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~TraceDataExtractor();

    FDExtractorAvp trace_reference;
    FDExtractorAvp trace_depth;
    FDExtractorAvp trace_ne_type_list;
    FDExtractorAvp trace_interface_list;
    FDExtractorAvp trace_event_list;
    FDExtractorAvp omc_id;
    FDExtractorAvp trace_collection_entity;
    MdtConfigurationExtractor mdt_configuration;
};

class EventReportIndicationExtractor : public FDExtractor
{
 public:
    EventReportIndicationExtractor( FDExtractor &parent, Dictionary &dict );
    ~EventReportIndicationExtractor();

    FDExtractorAvp an_trusted;
    FDExtractorAvpList event_trigger;
    UserCsgInformationExtractor user_csg_information;
    FDExtractorAvp ip_can_type;
    FDExtractorAvpList an_gw_address;
    FDExtractorAvp tgpp_sgsn_address;
    FDExtractorAvp tgpp_sgsn_ipv6_address;
    FDExtractorAvp tgpp_sgsn_mcc_mnc;
    FDExtractorAvp framed_ip_address;
    FDExtractorAvp rat_type;
    FDExtractorAvp rai;
    FDExtractorAvp tgpp_user_location_info;
    TraceDataExtractor trace_data;
    FDExtractorAvp trace_reference;
    FDExtractorAvp tgpp2_bsid;
    FDExtractorAvp tgpp_ms_timezone;
    FDExtractorAvp routing_ip_address;
    FDExtractorAvp ue_local_ip_address;
    FDExtractorAvp henb_local_ip_address;
    FDExtractorAvp udp_source_port;
    PresenceReportingAreaInformationExtractor presence_reporting_area_information;
};

class ProxyInfoExtractor : public FDExtractor
{
 public:
    ProxyInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProxyInfoExtractor();

    FDExtractorAvp proxy_host;
    FDExtractorAvp proxy_state;
};

class UnitValueExtractor : public FDExtractor
{
 public:
    UnitValueExtractor( FDExtractor &parent, Dictionary &dict );
    ~UnitValueExtractor();

    FDExtractorAvp value_digits;
    FDExtractorAvp exponent;
};

class CcMoneyExtractor : public FDExtractor
{
 public:
    CcMoneyExtractor( FDExtractor &parent, Dictionary &dict );
    ~CcMoneyExtractor();

    UnitValueExtractor unit_value;
    FDExtractorAvp currency_code;
};

class UsedServiceUnitExtractor : public FDExtractor
{
 public:
    UsedServiceUnitExtractor( FDExtractor &parent, Dictionary &dict );
    ~UsedServiceUnitExtractor();

    FDExtractorAvp reporting_reason;
    FDExtractorAvp tariff_change_usage;
    FDExtractorAvp cc_time;
    CcMoneyExtractor cc_money;
    FDExtractorAvp cc_total_octets;
    FDExtractorAvp cc_input_octets;
    FDExtractorAvp cc_output_octets;
    FDExtractorAvp cc_service_specific_units;
    FDExtractorAvpList event_charging_timestamp;
};

class RedirectServerExtractor : public FDExtractor
{
 public:
    RedirectServerExtractor( FDExtractor &parent, Dictionary &dict );
    ~RedirectServerExtractor();

    FDExtractorAvp redirect_address_type;
    FDExtractorAvp redirect_server_address;
};

class FinalUnitIndicationExtractor : public FDExtractor
{
 public:
    FinalUnitIndicationExtractor( FDExtractor &parent, Dictionary &dict );
    ~FinalUnitIndicationExtractor();

    FDExtractorAvp final_unit_action;
    FDExtractorAvpList restriction_filter_rule;
    FDExtractorAvpList filter_id;
    RedirectServerExtractor redirect_server;
};

class AccessNetworkChargingIdentifierGxExtractor : public FDExtractor
{
 public:
    AccessNetworkChargingIdentifierGxExtractor( FDExtractor &parent, Dictionary &dict );
    ~AccessNetworkChargingIdentifierGxExtractor();

    FDExtractorAvp access_network_charging_identifier_value;
    FDExtractorAvpList charging_rule_base_name;
    FDExtractorAvpList charging_rule_name;
    FDExtractorAvp ip_can_session_charging_scope;
};

class OcOlrExtractor : public FDExtractor
{
 public:
    OcOlrExtractor( FDExtractor &parent, Dictionary &dict );
    ~OcOlrExtractor();

    FDExtractorAvp oc_sequence_number;
    FDExtractorAvp oc_report_type;
    FDExtractorAvp oc_reduction_percentage;
    FDExtractorAvp oc_validity_duration;
};

class RoutingFilterExtractorList : public FDExtractorList
{
 public:
    RoutingFilterExtractorList( FDExtractor &parent, Dictionary &dict );
    ~RoutingFilterExtractorList();

    FDExtractor *createExtractor() { return new RoutingFilterExtractor( getParent(), m_dict ); }
    std::list<RoutingFilterExtractor*> &getList() { return (std::list<RoutingFilterExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class RoutingRuleDefinitionExtractor : public FDExtractor
{
 public:
    RoutingRuleDefinitionExtractor( FDExtractor &parent, Dictionary &dict );
    ~RoutingRuleDefinitionExtractor();

    FDExtractorAvp routing_rule_identifier;
    RoutingFilterExtractorList routing_filter;
    FDExtractorAvp precedence;
    FDExtractorAvp routing_ip_address;
    FDExtractorAvp ip_can_type;
};

class RoutingRuleDefinitionExtractorList : public FDExtractorList
{
 public:
    RoutingRuleDefinitionExtractorList( FDExtractor &parent, Dictionary &dict );
    ~RoutingRuleDefinitionExtractorList();

    FDExtractor *createExtractor() { return new RoutingRuleDefinitionExtractor( getParent(), m_dict ); }
    std::list<RoutingRuleDefinitionExtractor*> &getList() { return (std::list<RoutingRuleDefinitionExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class RoutingRuleInstallExtractor : public FDExtractor
{
 public:
    RoutingRuleInstallExtractor( FDExtractor &parent, Dictionary &dict );
    ~RoutingRuleInstallExtractor();

    RoutingRuleDefinitionExtractorList routing_rule_definition;
};

class ChargingRuleRemoveExtractor : public FDExtractor
{
 public:
    ChargingRuleRemoveExtractor( FDExtractor &parent, Dictionary &dict );
    ~ChargingRuleRemoveExtractor();

    FDExtractorAvpList charging_rule_name;
    FDExtractorAvpList charging_rule_base_name;
    FDExtractorAvpList required_access_info;
    FDExtractorAvp resource_release_notification;
};

class ExperimentalResultExtractor : public FDExtractor
{
 public:
    ExperimentalResultExtractor( FDExtractor &parent, Dictionary &dict );
    ~ExperimentalResultExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp experimental_result_code;
};

class UserEquipmentInfoExtractor : public FDExtractor
{
 public:
    UserEquipmentInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~UserEquipmentInfoExtractor();

    FDExtractorAvp user_equipment_info_type;
    FDExtractorAvp user_equipment_info_value;
};

class RoutingRuleReportExtractor : public FDExtractor
{
 public:
    RoutingRuleReportExtractor( FDExtractor &parent, Dictionary &dict );
    ~RoutingRuleReportExtractor();

    FDExtractorAvpList routing_rule_identifier;
    FDExtractorAvp pcc_rule_status;
    FDExtractorAvp routing_rule_failure_code;
};

class FixedUserLocationInfoExtractor : public FDExtractor
{
 public:
    FixedUserLocationInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~FixedUserLocationInfoExtractor();

    FDExtractorAvp ssid;
    FDExtractorAvp bssid;
    FDExtractorAvp logical_access_id;
    FDExtractorAvp physical_access_id;
};

class DefaultQosInformationExtractor : public FDExtractor
{
 public:
    DefaultQosInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~DefaultQosInformationExtractor();

    FDExtractorAvp qos_class_identifier;
    FDExtractorAvp max_requested_bandwidth_ul;
    FDExtractorAvp max_requested_bandwidth_dl;
    FDExtractorAvp default_qos_name;
};

class RoutingRuleRemoveExtractor : public FDExtractor
{
 public:
    RoutingRuleRemoveExtractor( FDExtractor &parent, Dictionary &dict );
    ~RoutingRuleRemoveExtractor();

    FDExtractorAvpList routing_rule_identifier;
};

class OcSupportedFeaturesExtractor : public FDExtractor
{
 public:
    OcSupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict );
    ~OcSupportedFeaturesExtractor();

    FDExtractorAvp oc_feature_vector;
};

class PacketFilterInformationExtractor : public FDExtractor
{
 public:
    PacketFilterInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~PacketFilterInformationExtractor();

    FDExtractorAvp packet_filter_identifier;
    FDExtractorAvp precedence;
    FDExtractorAvp packet_filter_content;
    FDExtractorAvp tos_traffic_class;
    FDExtractorAvp security_parameter_index;
    FDExtractorAvp flow_label;
    FDExtractorAvp flow_direction;
};

class ChargingInformationExtractor : public FDExtractor
{
 public:
    ChargingInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~ChargingInformationExtractor();

    FDExtractorAvp primary_event_charging_function_name;
    FDExtractorAvp secondary_event_charging_function_name;
    FDExtractorAvp primary_charging_collection_function_name;
    FDExtractorAvp secondary_charging_collection_function_name;
};

class TftPacketFilterInformationExtractor : public FDExtractor
{
 public:
    TftPacketFilterInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~TftPacketFilterInformationExtractor();

    FDExtractorAvp precedence;
    FDExtractorAvp tft_filter;
    FDExtractorAvp tos_traffic_class;
    FDExtractorAvp security_parameter_index;
    FDExtractorAvp flow_label;
    FDExtractorAvp flow_direction;
};

class SubscriptionIdExtractor : public FDExtractor
{
 public:
    SubscriptionIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~SubscriptionIdExtractor();

    FDExtractorAvp subscription_id_type;
    FDExtractorAvp subscription_id_data;
};

class ChargingRuleReportExtractor : public FDExtractor
{
 public:
    ChargingRuleReportExtractor( FDExtractor &parent, Dictionary &dict );
    ~ChargingRuleReportExtractor();

    FDExtractorAvpList charging_rule_name;
    FDExtractorAvpList charging_rule_base_name;
    FDExtractorAvp bearer_identifier;
    FDExtractorAvp pcc_rule_status;
    FDExtractorAvp rule_failure_code;
    FinalUnitIndicationExtractor final_unit_indication;
    FDExtractorAvpList ran_nas_release_cause;
    FDExtractorAvpList content_version;
};

class FailedAvpExtractor : public FDExtractor
{
 public:
    FailedAvpExtractor( FDExtractor &parent, Dictionary &dict );
    ~FailedAvpExtractor();


};

class GrantedServiceUnitExtractor : public FDExtractor
{
 public:
    GrantedServiceUnitExtractor( FDExtractor &parent, Dictionary &dict );
    ~GrantedServiceUnitExtractor();

    FDExtractorAvp tariff_time_change;
    FDExtractorAvp cc_time;
    CcMoneyExtractor cc_money;
    FDExtractorAvp cc_total_octets;
    FDExtractorAvp cc_input_octets;
    FDExtractorAvp cc_output_octets;
    FDExtractorAvp cc_service_specific_units;
};

class GrantedServiceUnitExtractorList : public FDExtractorList
{
 public:
    GrantedServiceUnitExtractorList( FDExtractor &parent, Dictionary &dict );
    ~GrantedServiceUnitExtractorList();

    FDExtractor *createExtractor() { return new GrantedServiceUnitExtractor( getParent(), m_dict ); }
    std::list<GrantedServiceUnitExtractor*> &getList() { return (std::list<GrantedServiceUnitExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class UsedServiceUnitExtractorList : public FDExtractorList
{
 public:
    UsedServiceUnitExtractorList( FDExtractor &parent, Dictionary &dict );
    ~UsedServiceUnitExtractorList();

    FDExtractor *createExtractor() { return new UsedServiceUnitExtractor( getParent(), m_dict ); }
    std::list<UsedServiceUnitExtractor*> &getList() { return (std::list<UsedServiceUnitExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class UsageMonitoringInformationExtractor : public FDExtractor
{
 public:
    UsageMonitoringInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~UsageMonitoringInformationExtractor();

    FDExtractorAvp monitoring_key;
    GrantedServiceUnitExtractorList granted_service_unit;
    UsedServiceUnitExtractorList used_service_unit;
    FDExtractorAvp quota_consumption_time;
    FDExtractorAvp usage_monitoring_level;
    FDExtractorAvp usage_monitoring_report;
    FDExtractorAvp usage_monitoring_support;
};

class CoaInformationExtractor : public FDExtractor
{
 public:
    CoaInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~CoaInformationExtractor();

    TunnelInformationExtractor tunnel_information;
    FDExtractorAvp coa_ip_address;
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

class ApplicationDetectionInformationExtractor : public FDExtractor
{
 public:
    ApplicationDetectionInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~ApplicationDetectionInformationExtractor();

    FDExtractorAvp tdf_application_identifier;
    FDExtractorAvp tdf_application_instance_identifier;
    FlowInformationExtractorList flow_information;
};

class SubscriptionIdExtractorList : public FDExtractorList
{
 public:
    SubscriptionIdExtractorList( FDExtractor &parent, Dictionary &dict );
    ~SubscriptionIdExtractorList();

    FDExtractor *createExtractor() { return new SubscriptionIdExtractor( getParent(), m_dict ); }
    std::list<SubscriptionIdExtractor*> &getList() { return (std::list<SubscriptionIdExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
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

class PacketFilterInformationExtractorList : public FDExtractorList
{
 public:
    PacketFilterInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~PacketFilterInformationExtractorList();

    FDExtractor *createExtractor() { return new PacketFilterInformationExtractor( getParent(), m_dict ); }
    std::list<PacketFilterInformationExtractor*> &getList() { return (std::list<PacketFilterInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class TftPacketFilterInformationExtractorList : public FDExtractorList
{
 public:
    TftPacketFilterInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~TftPacketFilterInformationExtractorList();

    FDExtractor *createExtractor() { return new TftPacketFilterInformationExtractor( getParent(), m_dict ); }
    std::list<TftPacketFilterInformationExtractor*> &getList() { return (std::list<TftPacketFilterInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ChargingRuleReportExtractorList : public FDExtractorList
{
 public:
    ChargingRuleReportExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ChargingRuleReportExtractorList();

    FDExtractor *createExtractor() { return new ChargingRuleReportExtractor( getParent(), m_dict ); }
    std::list<ChargingRuleReportExtractor*> &getList() { return (std::list<ChargingRuleReportExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ApplicationDetectionInformationExtractorList : public FDExtractorList
{
 public:
    ApplicationDetectionInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ApplicationDetectionInformationExtractorList();

    FDExtractor *createExtractor() { return new ApplicationDetectionInformationExtractor( getParent(), m_dict ); }
    std::list<ApplicationDetectionInformationExtractor*> &getList() { return (std::list<ApplicationDetectionInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AccessNetworkChargingIdentifierGxExtractorList : public FDExtractorList
{
 public:
    AccessNetworkChargingIdentifierGxExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AccessNetworkChargingIdentifierGxExtractorList();

    FDExtractor *createExtractor() { return new AccessNetworkChargingIdentifierGxExtractor( getParent(), m_dict ); }
    std::list<AccessNetworkChargingIdentifierGxExtractor*> &getList() { return (std::list<AccessNetworkChargingIdentifierGxExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class CoaInformationExtractorList : public FDExtractorList
{
 public:
    CoaInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~CoaInformationExtractorList();

    FDExtractor *createExtractor() { return new CoaInformationExtractor( getParent(), m_dict ); }
    std::list<CoaInformationExtractor*> &getList() { return (std::list<CoaInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class UsageMonitoringInformationExtractorList : public FDExtractorList
{
 public:
    UsageMonitoringInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~UsageMonitoringInformationExtractorList();

    FDExtractor *createExtractor() { return new UsageMonitoringInformationExtractor( getParent(), m_dict ); }
    std::list<UsageMonitoringInformationExtractor*> &getList() { return (std::list<UsageMonitoringInformationExtractor*>&)FDExtractorList::getList(); }

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

class CreditControlRequestExtractor : public FDExtractor
{
 public:
    CreditControlRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~CreditControlRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp auth_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp cc_request_type;
    FDExtractorAvp cc_request_number;
    FDExtractorAvp credit_management_status;
    FDExtractorAvp destination_host;
    FDExtractorAvp origin_state_id;
    SubscriptionIdExtractorList subscription_id;
    OcSupportedFeaturesExtractor oc_supported_features;
    SupportedFeaturesExtractorList supported_features;
    TdfInformationExtractor tdf_information;
    FDExtractorAvp network_request_support;
    PacketFilterInformationExtractorList packet_filter_information;
    FDExtractorAvp packet_filter_operation;
    FDExtractorAvp bearer_identifier;
    FDExtractorAvp bearer_operation;
    FDExtractorAvp dynamic_address_flag;
    FDExtractorAvp dynamic_address_flag_extension;
    FDExtractorAvp pdn_connection_charging_id;
    FDExtractorAvp framed_ip_address;
    FDExtractorAvp framed_ipv6_prefix;
    FDExtractorAvp ip_can_type;
    FDExtractorAvp tgpp_rat_type;
    FDExtractorAvp an_trusted;
    FDExtractorAvp rat_type;
    FDExtractorAvp termination_cause;
    UserEquipmentInfoExtractor user_equipment_info;
    QosInformationExtractor qos_information;
    FDExtractorAvp qos_negotiation;
    FDExtractorAvp qos_upgrade;
    DefaultEpsBearerQosExtractor default_eps_bearer_qos;
    DefaultQosInformationExtractor default_qos_information;
    FDExtractorAvpList an_gw_address;
    FDExtractorAvp an_gw_status;
    FDExtractorAvp tgpp_sgsn_mcc_mnc;
    FDExtractorAvp tgpp_sgsn_address;
    FDExtractorAvp tgpp_sgsn_ipv6_address;
    FDExtractorAvp tgpp_ggsn_address;
    FDExtractorAvp tgpp_ggsn_ipv6_address;
    FDExtractorAvp tgpp_selection_mode;
    FDExtractorAvp rai;
    FDExtractorAvp tgpp_user_location_info;
    FixedUserLocationInfoExtractor fixed_user_location_info;
    FDExtractorAvp user_location_info_time;
    UserCsgInformationExtractor user_csg_information;
    FDExtractorAvp twan_identifier;
    FDExtractorAvp tgpp_ms_timezone;
    FDExtractorAvpList ran_nas_release_cause;
    FDExtractorAvp tgpp_charging_characteristics;
    FDExtractorAvp called_station_id;
    FDExtractorAvp pdn_connection_id;
    FDExtractorAvp bearer_usage;
    FDExtractorAvp online;
    FDExtractorAvp offline;
    TftPacketFilterInformationExtractorList tft_packet_filter_information;
    ChargingRuleReportExtractorList charging_rule_report;
    ApplicationDetectionInformationExtractorList application_detection_information;
    FDExtractorAvpList event_trigger;
    EventReportIndicationExtractor event_report_indication;
    FDExtractorAvp access_network_charging_address;
    AccessNetworkChargingIdentifierGxExtractorList access_network_charging_identifier_gx;
    CoaInformationExtractorList coa_information;
    UsageMonitoringInformationExtractorList usage_monitoring_information;
    FDExtractorAvp nbifom_support;
    FDExtractorAvp nbifom_mode;
    FDExtractorAvp default_access;
    FDExtractorAvp origination_time_stamp;
    FDExtractorAvp maximum_wait_time;
    FDExtractorAvp access_availability_change_reason;
    RoutingRuleInstallExtractor routing_rule_install;
    RoutingRuleRemoveExtractor routing_rule_remove;
    FDExtractorAvp henb_local_ip_address;
    FDExtractorAvp ue_local_ip_address;
    FDExtractorAvp udp_source_port;
    PresenceReportingAreaInformationExtractorList presence_reporting_area_information;
    FDExtractorAvp logical_access_id;
    FDExtractorAvp physical_access_id;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
    FDExtractorAvp tgpp_ps_data_off_status;
};

class ChargingRuleRemoveExtractorList : public FDExtractorList
{
 public:
    ChargingRuleRemoveExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ChargingRuleRemoveExtractorList();

    FDExtractor *createExtractor() { return new ChargingRuleRemoveExtractor( getParent(), m_dict ); }
    std::list<ChargingRuleRemoveExtractor*> &getList() { return (std::list<ChargingRuleRemoveExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ChargingRuleInstallExtractorList : public FDExtractorList
{
 public:
    ChargingRuleInstallExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ChargingRuleInstallExtractorList();

    FDExtractor *createExtractor() { return new ChargingRuleInstallExtractor( getParent(), m_dict ); }
    std::list<ChargingRuleInstallExtractor*> &getList() { return (std::list<ChargingRuleInstallExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class QosInformationExtractorList : public FDExtractorList
{
 public:
    QosInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~QosInformationExtractorList();

    FDExtractor *createExtractor() { return new QosInformationExtractor( getParent(), m_dict ); }
    std::list<QosInformationExtractor*> &getList() { return (std::list<QosInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class RoutingRuleReportExtractorList : public FDExtractorList
{
 public:
    RoutingRuleReportExtractorList( FDExtractor &parent, Dictionary &dict );
    ~RoutingRuleReportExtractorList();

    FDExtractor *createExtractor() { return new RoutingRuleReportExtractor( getParent(), m_dict ); }
    std::list<RoutingRuleReportExtractor*> &getList() { return (std::list<RoutingRuleReportExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ConditionalPolicyInformationExtractorList : public FDExtractorList
{
 public:
    ConditionalPolicyInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ConditionalPolicyInformationExtractorList();

    FDExtractor *createExtractor() { return new ConditionalPolicyInformationExtractor( getParent(), m_dict ); }
    std::list<ConditionalPolicyInformationExtractor*> &getList() { return (std::list<ConditionalPolicyInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class LoadExtractorList : public FDExtractorList
{
 public:
    LoadExtractorList( FDExtractor &parent, Dictionary &dict );
    ~LoadExtractorList();

    FDExtractor *createExtractor() { return new LoadExtractor( getParent(), m_dict ); }
    std::list<LoadExtractor*> &getList() { return (std::list<LoadExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class CreditControlAnswerExtractor : public FDExtractor
{
 public:
    CreditControlAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~CreditControlAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp auth_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp cc_request_type;
    FDExtractorAvp cc_request_number;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp bearer_control_mode;
    FDExtractorAvpList event_trigger;
    EventReportIndicationExtractor event_report_indication;
    FDExtractorAvp origin_state_id;
    FDExtractorAvpList redirect_host;
    FDExtractorAvp redirect_host_usage;
    FDExtractorAvp redirect_max_cache_time;
    ChargingRuleRemoveExtractorList charging_rule_remove;
    ChargingRuleInstallExtractorList charging_rule_install;
    ChargingInformationExtractor charging_information;
    FDExtractorAvp online;
    FDExtractorAvp offline;
    QosInformationExtractorList qos_information;
    FDExtractorAvp revalidation_time;
    DefaultEpsBearerQosExtractor default_eps_bearer_qos;
    DefaultQosInformationExtractor default_qos_information;
    FDExtractorAvp bearer_usage;
    UsageMonitoringInformationExtractorList usage_monitoring_information;
    FDExtractorAvpList csg_information_reporting;
    UserCsgInformationExtractor user_csg_information;
    PraInstallExtractor pra_install;
    PraRemoveExtractor pra_remove;
    PresenceReportingAreaInformationExtractor presence_reporting_area_information;
    FDExtractorAvp session_release_cause;
    FDExtractorAvp nbifom_support;
    FDExtractorAvp nbifom_mode;
    FDExtractorAvp default_access;
    FDExtractorAvp ran_rule_support;
    RoutingRuleReportExtractorList routing_rule_report;
    ConditionalPolicyInformationExtractorList conditional_policy_information;
    FDExtractorAvp removal_of_access;
    FDExtractorAvp ip_can_type;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
    LoadExtractorList load;
};

class ReAuthRequestExtractor : public FDExtractor
{
 public:
    ReAuthRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~ReAuthRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp auth_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp re_auth_request_type;
    FDExtractorAvp session_release_cause;
    FDExtractorAvp origin_state_id;
    OcSupportedFeaturesExtractor oc_supported_features;
    FDExtractorAvpList event_trigger;
    EventReportIndicationExtractor event_report_indication;
    ChargingRuleRemoveExtractorList charging_rule_remove;
    ChargingRuleInstallExtractorList charging_rule_install;
    DefaultEpsBearerQosExtractor default_eps_bearer_qos;
    QosInformationExtractorList qos_information;
    DefaultQosInformationExtractor default_qos_information;
    FDExtractorAvp revalidation_time;
    UsageMonitoringInformationExtractorList usage_monitoring_information;
    FDExtractorAvp pcscf_restoration_indication;
    ConditionalPolicyInformationExtractorList conditional_policy_information;
    FDExtractorAvp removal_of_access;
    FDExtractorAvp ip_can_type;
    PraInstallExtractor pra_install;
    PraRemoveExtractor pra_remove;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class ReAuthAnswerExtractor : public FDExtractor
{
 public:
    ReAuthAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~ReAuthAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp origin_state_id;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    FDExtractorAvp ip_can_type;
    FDExtractorAvp rat_type;
    FDExtractorAvp an_trusted;
    FDExtractorAvpList an_gw_address;
    FDExtractorAvp tgpp_sgsn_mcc_mnc;
    FDExtractorAvp tgpp_sgsn_address;
    FDExtractorAvp tgpp_sgsn_ipv6_address;
    FDExtractorAvp rai;
    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp user_location_info_time;
    FDExtractorAvp netloc_access_support;
    UserCsgInformationExtractor user_csg_information;
    FDExtractorAvp tgpp_ms_timezone;
    DefaultQosInformationExtractor default_qos_information;
    ChargingRuleReportExtractorList charging_rule_report;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
};


}

#endif // __GX_H

