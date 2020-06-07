/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __ST_H
#define __ST_H

#include "fd.h"

namespace st {

//
// Primary Objects
//

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    FDDictionaryEntryApplication &app()                            { return m_app; }

    FDDictionaryEntryCommand &cmdTSR()                            	{ return m_cmd_tsr; }
    FDDictionaryEntryCommand &cmdTSA()                            	{ return m_cmd_tsa; }
    FDDictionaryEntryCommand &cmdSETR()                           	{ return m_cmd_setr; }
    FDDictionaryEntryCommand &cmdSETA()                           	{ return m_cmd_seta; }
    FDDictionaryEntryCommand &cmdTNR()                            	{ return m_cmd_tnr; }
    FDDictionaryEntryCommand &cmdTNA()                            	{ return m_cmd_tna; }

    FDDictionaryEntryVendor &vnd3GPP2()                           	{ return m_vnd_3gpp2; }
    FDDictionaryEntryVendor &vnd3GPP()                            	{ return m_vnd_3gpp; }

    FDDictionaryEntryAVP &avpPreEmptionCapability()               	{ return m_avp_pre_emption_capability; }
    FDDictionaryEntryAVP &avpFeatureListId()                      	{ return m_avp_feature_list_id; }
    FDDictionaryEntryAVP &avpServiceIdentifier()                  	{ return m_avp_service_identifier; }
    FDDictionaryEntryAVP &avpVendorSpecificApplicationId()        	{ return m_avp_vendor_specific_application_id; }
    FDDictionaryEntryAVP &avpCsgAccessMode()                      	{ return m_avp_csg_access_mode; }
    FDDictionaryEntryAVP &avpHenbLocalIpAddress()                 	{ return m_avp_henb_local_ip_address; }
    FDDictionaryEntryAVP &avpApnAggregateMaxBitrateUl()           	{ return m_avp_apn_aggregate_max_bitrate_ul; }
    FDDictionaryEntryAVP &avp3gppSgsnIpv6Address()                	{ return m_avp_3gpp_sgsn_ipv6_address; }
    FDDictionaryEntryAVP &avpTrafficSteeringPolicyIdentifierUl()  	{ return m_avp_traffic_steering_policy_identifier_ul; }
    FDDictionaryEntryAVP &avpPacketFilterUsage()                  	{ return m_avp_packet_filter_usage; }
    FDDictionaryEntryAVP &avpTrackingAreaIdentity()               	{ return m_avp_tracking_area_identity; }
    FDDictionaryEntryAVP &avpLoadValue()                          	{ return m_avp_load_value; }
    FDDictionaryEntryAVP &avpFeatureList()                        	{ return m_avp_feature_list; }
    FDDictionaryEntryAVP &avpOmcId()                              	{ return m_avp_omc_id; }
    FDDictionaryEntryAVP &avpRai()                                	{ return m_avp_rai; }
    FDDictionaryEntryAVP &avpOcReportType()                       	{ return m_avp_oc_report_type; }
    FDDictionaryEntryAVP &avpExperimentalResult()                 	{ return m_avp_experimental_result; }
    FDDictionaryEntryAVP &avpLoadType()                           	{ return m_avp_load_type; }
    FDDictionaryEntryAVP &avpOcFeatureVector()                    	{ return m_avp_oc_feature_vector; }
    FDDictionaryEntryAVP &avpRedirectInformation()                	{ return m_avp_redirect_information; }
    FDDictionaryEntryAVP &avpListOfMeasurements()                 	{ return m_avp_list_of_measurements; }
    FDDictionaryEntryAVP &avpQosInformation()                     	{ return m_avp_qos_information; }
    FDDictionaryEntryAVP &avpFinalUnitAction()                    	{ return m_avp_final_unit_action; }
    FDDictionaryEntryAVP &avpDrmp()                               	{ return m_avp_drmp; }
    FDDictionaryEntryAVP &avpRuleDeactivationTime()               	{ return m_avp_rule_deactivation_time; }
    FDDictionaryEntryAVP &avpFlowStatus()                         	{ return m_avp_flow_status; }
    FDDictionaryEntryAVP &avpPriorityLevel()                      	{ return m_avp_priority_level; }
    FDDictionaryEntryAVP &avpPreEmptionVulnerability()            	{ return m_avp_pre_emption_vulnerability; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaStatus()        	{ return m_avp_presence_reporting_area_status; }
    FDDictionaryEntryAVP &avpQosClassIdentifier()                 	{ return m_avp_qos_class_identifier; }
    FDDictionaryEntryAVP &avpAreaScope()                          	{ return m_avp_area_scope; }
    FDDictionaryEntryAVP &avpTraceNeTypeList()                    	{ return m_avp_trace_ne_type_list; }
    FDDictionaryEntryAVP &avpPrecedence()                         	{ return m_avp_precedence; }
    FDDictionaryEntryAVP &avpRedirectHostUsage()                  	{ return m_avp_redirect_host_usage; }
    FDDictionaryEntryAVP &avpAnGwAddress()                        	{ return m_avp_an_gw_address; }
    FDDictionaryEntryAVP &avpClass()                              	{ return m_avp_class; }
    FDDictionaryEntryAVP &avpTrafficSteeringPolicyIdentifierDl()  	{ return m_avp_traffic_steering_policy_identifier_dl; }
    FDDictionaryEntryAVP &avpDestinationHost()                    	{ return m_avp_destination_host; }
    FDDictionaryEntryAVP &avpLocationAreaIdentity()               	{ return m_avp_location_area_identity; }
    FDDictionaryEntryAVP &avpLoggingInterval()                    	{ return m_avp_logging_interval; }
    FDDictionaryEntryAVP &avpFlowInformation()                    	{ return m_avp_flow_information; }
    FDDictionaryEntryAVP &avpUeLocalIpAddress()                   	{ return m_avp_ue_local_ip_address; }
    FDDictionaryEntryAVP &avpGuaranteedBitrateUl()                	{ return m_avp_guaranteed_bitrate_ul; }
    FDDictionaryEntryAVP &avpEventReportIndication()              	{ return m_avp_event_report_indication; }
    FDDictionaryEntryAVP &avpJobType()                            	{ return m_avp_job_type; }
    FDDictionaryEntryAVP &avpFramedIpv6Prefix()                   	{ return m_avp_framed_ipv6_prefix; }
    FDDictionaryEntryAVP &avpProxyInfo()                          	{ return m_avp_proxy_info; }
    FDDictionaryEntryAVP &avpTerminationCause()                   	{ return m_avp_termination_cause; }
    FDDictionaryEntryAVP &avpOriginRealm()                        	{ return m_avp_origin_realm; }
    FDDictionaryEntryAVP &avpOriginHost()                         	{ return m_avp_origin_host; }
    FDDictionaryEntryAVP &avpFlowLabel()                          	{ return m_avp_flow_label; }
    FDDictionaryEntryAVP &avpGuaranteedBitrateDl()                	{ return m_avp_guaranteed_bitrate_dl; }
    FDDictionaryEntryAVP &avpRestrictionFilterRule()              	{ return m_avp_restriction_filter_rule; }
    FDDictionaryEntryAVP &avp3gppSgsnAddress()                    	{ return m_avp_3gpp_sgsn_address; }
    FDDictionaryEntryAVP &avpRedirectAddressType()                	{ return m_avp_redirect_address_type; }
    FDDictionaryEntryAVP &avpRedirectServerAddress()              	{ return m_avp_redirect_server_address; }
    FDDictionaryEntryAVP &avpAdcRuleBaseName()                    	{ return m_avp_adc_rule_base_name; }
    FDDictionaryEntryAVP &avpFinalUnitIndication()                	{ return m_avp_final_unit_indication; }
    FDDictionaryEntryAVP &avpFlowDescription()                    	{ return m_avp_flow_description; }
    FDDictionaryEntryAVP &avpReportInterval()                     	{ return m_avp_report_interval; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaElementsList()  	{ return m_avp_presence_reporting_area_elements_list; }
    FDDictionaryEntryAVP &avpRouteRecord()                        	{ return m_avp_route_record; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaIdentifier()    	{ return m_avp_presence_reporting_area_identifier; }
    FDDictionaryEntryAVP &avpTraceEventList()                     	{ return m_avp_trace_event_list; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaInformation()   	{ return m_avp_presence_reporting_area_information; }
    FDDictionaryEntryAVP &avpTdfApplicationIdentifier()           	{ return m_avp_tdf_application_identifier; }
    FDDictionaryEntryAVP &avpMeteringMethod()                     	{ return m_avp_metering_method; }
    FDDictionaryEntryAVP &avpFramedIpAddress()                    	{ return m_avp_framed_ip_address; }
    FDDictionaryEntryAVP &avpLoggingDuration()                    	{ return m_avp_logging_duration; }
    FDDictionaryEntryAVP &avpApnAggregateMaxBitrateDl()           	{ return m_avp_apn_aggregate_max_bitrate_dl; }
    FDDictionaryEntryAVP &avpCollectionPeriodRrmUmts()            	{ return m_avp_collection_period_rrm_umts; }
    FDDictionaryEntryAVP &avpPositioningMethod()                  	{ return m_avp_positioning_method; }
    FDDictionaryEntryAVP &avpOcOlr()                              	{ return m_avp_oc_olr; }
    FDDictionaryEntryAVP &avpTraceData()                          	{ return m_avp_trace_data; }
    FDDictionaryEntryAVP &avpSourceid()                           	{ return m_avp_sourceid; }
    FDDictionaryEntryAVP &avpBearerIdentifier()                   	{ return m_avp_bearer_identifier; }
    FDDictionaryEntryAVP &avpSponsorIdentity()                    	{ return m_avp_sponsor_identity; }
    FDDictionaryEntryAVP &avpOcReductionPercentage()              	{ return m_avp_oc_reduction_percentage; }
    FDDictionaryEntryAVP &avpTraceCollectionEntity()              	{ return m_avp_trace_collection_entity; }
    FDDictionaryEntryAVP &avpMdtConfiguration()                   	{ return m_avp_mdt_configuration; }
    FDDictionaryEntryAVP &avpErrorReportingHost()                 	{ return m_avp_error_reporting_host; }
    FDDictionaryEntryAVP &avpAdcRuleRemove()                      	{ return m_avp_adc_rule_remove; }
    FDDictionaryEntryAVP &avp3gppSgsnMccMnc()                     	{ return m_avp_3gpp_sgsn_mcc_mnc; }
    FDDictionaryEntryAVP &avpOcSequenceNumber()                   	{ return m_avp_oc_sequence_number; }
    FDDictionaryEntryAVP &avpEventThresholdEvent1i()              	{ return m_avp_event_threshold_event_1i; }
    FDDictionaryEntryAVP &avpRatingGroup()                        	{ return m_avp_rating_group; }
    FDDictionaryEntryAVP &avpRatType()                            	{ return m_avp_rat_type; }
    FDDictionaryEntryAVP &avpOnline()                             	{ return m_avp_online; }
    FDDictionaryEntryAVP &avpApplicationServiceProviderIdentity() 	{ return m_avp_application_service_provider_identity; }
    FDDictionaryEntryAVP &avpEventThresholdEvent1f()              	{ return m_avp_event_threshold_event_1f; }
    FDDictionaryEntryAVP &avpReportingLevel()                     	{ return m_avp_reporting_level; }
    FDDictionaryEntryAVP &avpAllocationRetentionPriority()        	{ return m_avp_allocation_retention_priority; }
    FDDictionaryEntryAVP &avpCellGlobalIdentity()                 	{ return m_avp_cell_global_identity; }
    FDDictionaryEntryAVP &avpOcValidityDuration()                 	{ return m_avp_oc_validity_duration; }
    FDDictionaryEntryAVP &avpCsgMembershipIndication()            	{ return m_avp_csg_membership_indication; }
    FDDictionaryEntryAVP &avpAdcRuleDefinition()                  	{ return m_avp_adc_rule_definition; }
    FDDictionaryEntryAVP &avpRedirectHost()                       	{ return m_avp_redirect_host; }
    FDDictionaryEntryAVP &avpMeasurementPeriodLte()               	{ return m_avp_measurement_period_lte; }
    FDDictionaryEntryAVP &avpAdcRuleReport()                      	{ return m_avp_adc_rule_report; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthDl()            	{ return m_avp_max_requested_bandwidth_dl; }
    FDDictionaryEntryAVP &avpOriginStateId()                      	{ return m_avp_origin_state_id; }
    FDDictionaryEntryAVP &avpFilterId()                           	{ return m_avp_filter_id; }
    FDDictionaryEntryAVP &avpRequestType()                        	{ return m_avp_request_type; }
    FDDictionaryEntryAVP &avp3gppUserLocationInfo()               	{ return m_avp_3gpp_user_location_info; }
    FDDictionaryEntryAVP &avpResultCode()                         	{ return m_avp_result_code; }
    FDDictionaryEntryAVP &avpTraceInterfaceList()                 	{ return m_avp_trace_interface_list; }
    FDDictionaryEntryAVP &avpCsgId()                              	{ return m_avp_csg_id; }
    FDDictionaryEntryAVP &avpRoutingRuleIdentifier()              	{ return m_avp_routing_rule_identifier; }
    FDDictionaryEntryAVP &avpRedirectMaxCacheTime()               	{ return m_avp_redirect_max_cache_time; }
    FDDictionaryEntryAVP &avpRuleActivationTime()                 	{ return m_avp_rule_activation_time; }
    FDDictionaryEntryAVP &avpLoad()                               	{ return m_avp_load; }
    FDDictionaryEntryAVP &avpAcctApplicationId()                  	{ return m_avp_acct_application_id; }
    FDDictionaryEntryAVP &avpRedirectServer()                     	{ return m_avp_redirect_server; }
    FDDictionaryEntryAVP &avpAnTrusted()                          	{ return m_avp_an_trusted; }
    FDDictionaryEntryAVP &avpEUtranCellGlobalIdentity()           	{ return m_avp_e_utran_cell_global_identity; }
    FDDictionaryEntryAVP &avpCalledStationId()                    	{ return m_avp_called_station_id; }
    FDDictionaryEntryAVP &avpPccRuleStatus()                      	{ return m_avp_pcc_rule_status; }
    FDDictionaryEntryAVP &avpOcSupportedFeatures()                	{ return m_avp_oc_supported_features; }
    FDDictionaryEntryAVP &avpPacketFilterIdentifier()             	{ return m_avp_packet_filter_identifier; }
    FDDictionaryEntryAVP &avpProxyHost()                          	{ return m_avp_proxy_host; }
    FDDictionaryEntryAVP &avpEventThresholdRsrp()                 	{ return m_avp_event_threshold_rsrp; }
    FDDictionaryEntryAVP &avpEventThresholdRsrq()                 	{ return m_avp_event_threshold_rsrq; }
    FDDictionaryEntryAVP &avpExperimentalResultCode()             	{ return m_avp_experimental_result_code; }
    FDDictionaryEntryAVP &avpCollectionPeriodRrmLte()             	{ return m_avp_collection_period_rrm_lte; }
    FDDictionaryEntryAVP &avpAuthApplicationId()                  	{ return m_avp_auth_application_id; }
    FDDictionaryEntryAVP &avpEventTrigger()                       	{ return m_avp_event_trigger; }
    FDDictionaryEntryAVP &avpMonitoringKey()                      	{ return m_avp_monitoring_key; }
    FDDictionaryEntryAVP &avpAdcRuleInstall()                     	{ return m_avp_adc_rule_install; }
    FDDictionaryEntryAVP &avp3gppMsTimezone()                     	{ return m_avp_3gpp_ms_timezone; }
    FDDictionaryEntryAVP &avp3gpp2Bsid()                          	{ return m_avp_3gpp2_bsid; }
    FDDictionaryEntryAVP &avpMonitoringFlags()                    	{ return m_avp_monitoring_flags; }
    FDDictionaryEntryAVP &avpConditionalApnAggregateMaxBitrate()  	{ return m_avp_conditional_apn_aggregate_max_bitrate; }
    FDDictionaryEntryAVP &avpIpCanType()                          	{ return m_avp_ip_can_type; }
    FDDictionaryEntryAVP &avpOffline()                            	{ return m_avp_offline; }
    FDDictionaryEntryAVP &avpUdpSourcePort()                      	{ return m_avp_udp_source_port; }
    FDDictionaryEntryAVP &avpRoutingIpAddress()                   	{ return m_avp_routing_ip_address; }
    FDDictionaryEntryAVP &avpAdcRuleName()                        	{ return m_avp_adc_rule_name; }
    FDDictionaryEntryAVP &avpErrorMessage()                       	{ return m_avp_error_message; }
    FDDictionaryEntryAVP &avpMdtAllowedPlmnId()                   	{ return m_avp_mdt_allowed_plmn_id; }
    FDDictionaryEntryAVP &avpReportingTrigger()                   	{ return m_avp_reporting_trigger; }
    FDDictionaryEntryAVP &avpFailedAvp()                          	{ return m_avp_failed_avp; }
    FDDictionaryEntryAVP &avpRoutingAreaIdentity()                	{ return m_avp_routing_area_identity; }
    FDDictionaryEntryAVP &avpTraceReference()                     	{ return m_avp_trace_reference; }
    FDDictionaryEntryAVP &avpFlowDirection()                      	{ return m_avp_flow_direction; }
    FDDictionaryEntryAVP &avpMeasurementQuantity()                	{ return m_avp_measurement_quantity; }
    FDDictionaryEntryAVP &avpTraceDepth()                         	{ return m_avp_trace_depth; }
    FDDictionaryEntryAVP &avpProxyState()                         	{ return m_avp_proxy_state; }
    FDDictionaryEntryAVP &avpRuleFailureCode()                    	{ return m_avp_rule_failure_code; }
    FDDictionaryEntryAVP &avpVendorId()                           	{ return m_avp_vendor_id; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthUl()            	{ return m_avp_max_requested_bandwidth_ul; }
    FDDictionaryEntryAVP &avpSupportedFeatures()                  	{ return m_avp_supported_features; }
    FDDictionaryEntryAVP &avpRedirectSupport()                    	{ return m_avp_redirect_support; }
    FDDictionaryEntryAVP &avpDestinationRealm()                   	{ return m_avp_destination_realm; }
    FDDictionaryEntryAVP &avpSessionId()                          	{ return m_avp_session_id; }
    FDDictionaryEntryAVP &avpTosTrafficClass()                    	{ return m_avp_tos_traffic_class; }
    FDDictionaryEntryAVP &avpReportAmount()                       	{ return m_avp_report_amount; }
    FDDictionaryEntryAVP &avpMuteNotification()                   	{ return m_avp_mute_notification; }
    FDDictionaryEntryAVP &avpSecurityParameterIndex()             	{ return m_avp_security_parameter_index; }
    FDDictionaryEntryAVP &avpMeasurementPeriodUmts()              	{ return m_avp_measurement_period_umts; }
    FDDictionaryEntryAVP &avpUserCsgInformation()                 	{ return m_avp_user_csg_information; }

private:
    FDDictionaryEntryApplication m_app;                           

    FDDictionaryEntryCommand m_cmd_tsr;
    FDDictionaryEntryCommand m_cmd_tsa;
    FDDictionaryEntryCommand m_cmd_setr;
    FDDictionaryEntryCommand m_cmd_seta;
    FDDictionaryEntryCommand m_cmd_tnr;
    FDDictionaryEntryCommand m_cmd_tna;

    FDDictionaryEntryVendor m_vnd_3gpp2;
    FDDictionaryEntryVendor m_vnd_3gpp;

    FDDictionaryEntryAVP m_avp_pre_emption_capability;
    FDDictionaryEntryAVP m_avp_feature_list_id;
    FDDictionaryEntryAVP m_avp_service_identifier;
    FDDictionaryEntryAVP m_avp_vendor_specific_application_id;
    FDDictionaryEntryAVP m_avp_csg_access_mode;
    FDDictionaryEntryAVP m_avp_henb_local_ip_address;
    FDDictionaryEntryAVP m_avp_apn_aggregate_max_bitrate_ul;
    FDDictionaryEntryAVP m_avp_3gpp_sgsn_ipv6_address;
    FDDictionaryEntryAVP m_avp_traffic_steering_policy_identifier_ul;
    FDDictionaryEntryAVP m_avp_packet_filter_usage;
    FDDictionaryEntryAVP m_avp_tracking_area_identity;
    FDDictionaryEntryAVP m_avp_load_value;
    FDDictionaryEntryAVP m_avp_feature_list;
    FDDictionaryEntryAVP m_avp_omc_id;
    FDDictionaryEntryAVP m_avp_rai;
    FDDictionaryEntryAVP m_avp_oc_report_type;
    FDDictionaryEntryAVP m_avp_experimental_result;
    FDDictionaryEntryAVP m_avp_load_type;
    FDDictionaryEntryAVP m_avp_oc_feature_vector;
    FDDictionaryEntryAVP m_avp_redirect_information;
    FDDictionaryEntryAVP m_avp_list_of_measurements;
    FDDictionaryEntryAVP m_avp_qos_information;
    FDDictionaryEntryAVP m_avp_final_unit_action;
    FDDictionaryEntryAVP m_avp_drmp;
    FDDictionaryEntryAVP m_avp_rule_deactivation_time;
    FDDictionaryEntryAVP m_avp_flow_status;
    FDDictionaryEntryAVP m_avp_priority_level;
    FDDictionaryEntryAVP m_avp_pre_emption_vulnerability;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_status;
    FDDictionaryEntryAVP m_avp_qos_class_identifier;
    FDDictionaryEntryAVP m_avp_area_scope;
    FDDictionaryEntryAVP m_avp_trace_ne_type_list;
    FDDictionaryEntryAVP m_avp_precedence;
    FDDictionaryEntryAVP m_avp_redirect_host_usage;
    FDDictionaryEntryAVP m_avp_an_gw_address;
    FDDictionaryEntryAVP m_avp_class;
    FDDictionaryEntryAVP m_avp_traffic_steering_policy_identifier_dl;
    FDDictionaryEntryAVP m_avp_destination_host;
    FDDictionaryEntryAVP m_avp_location_area_identity;
    FDDictionaryEntryAVP m_avp_logging_interval;
    FDDictionaryEntryAVP m_avp_flow_information;
    FDDictionaryEntryAVP m_avp_ue_local_ip_address;
    FDDictionaryEntryAVP m_avp_guaranteed_bitrate_ul;
    FDDictionaryEntryAVP m_avp_event_report_indication;
    FDDictionaryEntryAVP m_avp_job_type;
    FDDictionaryEntryAVP m_avp_framed_ipv6_prefix;
    FDDictionaryEntryAVP m_avp_proxy_info;
    FDDictionaryEntryAVP m_avp_termination_cause;
    FDDictionaryEntryAVP m_avp_origin_realm;
    FDDictionaryEntryAVP m_avp_origin_host;
    FDDictionaryEntryAVP m_avp_flow_label;
    FDDictionaryEntryAVP m_avp_guaranteed_bitrate_dl;
    FDDictionaryEntryAVP m_avp_restriction_filter_rule;
    FDDictionaryEntryAVP m_avp_3gpp_sgsn_address;
    FDDictionaryEntryAVP m_avp_redirect_address_type;
    FDDictionaryEntryAVP m_avp_redirect_server_address;
    FDDictionaryEntryAVP m_avp_adc_rule_base_name;
    FDDictionaryEntryAVP m_avp_final_unit_indication;
    FDDictionaryEntryAVP m_avp_flow_description;
    FDDictionaryEntryAVP m_avp_report_interval;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_elements_list;
    FDDictionaryEntryAVP m_avp_route_record;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_identifier;
    FDDictionaryEntryAVP m_avp_trace_event_list;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_information;
    FDDictionaryEntryAVP m_avp_tdf_application_identifier;
    FDDictionaryEntryAVP m_avp_metering_method;
    FDDictionaryEntryAVP m_avp_framed_ip_address;
    FDDictionaryEntryAVP m_avp_logging_duration;
    FDDictionaryEntryAVP m_avp_apn_aggregate_max_bitrate_dl;
    FDDictionaryEntryAVP m_avp_collection_period_rrm_umts;
    FDDictionaryEntryAVP m_avp_positioning_method;
    FDDictionaryEntryAVP m_avp_oc_olr;
    FDDictionaryEntryAVP m_avp_trace_data;
    FDDictionaryEntryAVP m_avp_sourceid;
    FDDictionaryEntryAVP m_avp_bearer_identifier;
    FDDictionaryEntryAVP m_avp_sponsor_identity;
    FDDictionaryEntryAVP m_avp_oc_reduction_percentage;
    FDDictionaryEntryAVP m_avp_trace_collection_entity;
    FDDictionaryEntryAVP m_avp_mdt_configuration;
    FDDictionaryEntryAVP m_avp_error_reporting_host;
    FDDictionaryEntryAVP m_avp_adc_rule_remove;
    FDDictionaryEntryAVP m_avp_3gpp_sgsn_mcc_mnc;
    FDDictionaryEntryAVP m_avp_oc_sequence_number;
    FDDictionaryEntryAVP m_avp_event_threshold_event_1i;
    FDDictionaryEntryAVP m_avp_rating_group;
    FDDictionaryEntryAVP m_avp_rat_type;
    FDDictionaryEntryAVP m_avp_online;
    FDDictionaryEntryAVP m_avp_application_service_provider_identity;
    FDDictionaryEntryAVP m_avp_event_threshold_event_1f;
    FDDictionaryEntryAVP m_avp_reporting_level;
    FDDictionaryEntryAVP m_avp_allocation_retention_priority;
    FDDictionaryEntryAVP m_avp_cell_global_identity;
    FDDictionaryEntryAVP m_avp_oc_validity_duration;
    FDDictionaryEntryAVP m_avp_csg_membership_indication;
    FDDictionaryEntryAVP m_avp_adc_rule_definition;
    FDDictionaryEntryAVP m_avp_redirect_host;
    FDDictionaryEntryAVP m_avp_measurement_period_lte;
    FDDictionaryEntryAVP m_avp_adc_rule_report;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_dl;
    FDDictionaryEntryAVP m_avp_origin_state_id;
    FDDictionaryEntryAVP m_avp_filter_id;
    FDDictionaryEntryAVP m_avp_request_type;
    FDDictionaryEntryAVP m_avp_3gpp_user_location_info;
    FDDictionaryEntryAVP m_avp_result_code;
    FDDictionaryEntryAVP m_avp_trace_interface_list;
    FDDictionaryEntryAVP m_avp_csg_id;
    FDDictionaryEntryAVP m_avp_routing_rule_identifier;
    FDDictionaryEntryAVP m_avp_redirect_max_cache_time;
    FDDictionaryEntryAVP m_avp_rule_activation_time;
    FDDictionaryEntryAVP m_avp_load;
    FDDictionaryEntryAVP m_avp_acct_application_id;
    FDDictionaryEntryAVP m_avp_redirect_server;
    FDDictionaryEntryAVP m_avp_an_trusted;
    FDDictionaryEntryAVP m_avp_e_utran_cell_global_identity;
    FDDictionaryEntryAVP m_avp_called_station_id;
    FDDictionaryEntryAVP m_avp_pcc_rule_status;
    FDDictionaryEntryAVP m_avp_oc_supported_features;
    FDDictionaryEntryAVP m_avp_packet_filter_identifier;
    FDDictionaryEntryAVP m_avp_proxy_host;
    FDDictionaryEntryAVP m_avp_event_threshold_rsrp;
    FDDictionaryEntryAVP m_avp_event_threshold_rsrq;
    FDDictionaryEntryAVP m_avp_experimental_result_code;
    FDDictionaryEntryAVP m_avp_collection_period_rrm_lte;
    FDDictionaryEntryAVP m_avp_auth_application_id;
    FDDictionaryEntryAVP m_avp_event_trigger;
    FDDictionaryEntryAVP m_avp_monitoring_key;
    FDDictionaryEntryAVP m_avp_adc_rule_install;
    FDDictionaryEntryAVP m_avp_3gpp_ms_timezone;
    FDDictionaryEntryAVP m_avp_3gpp2_bsid;
    FDDictionaryEntryAVP m_avp_monitoring_flags;
    FDDictionaryEntryAVP m_avp_conditional_apn_aggregate_max_bitrate;
    FDDictionaryEntryAVP m_avp_ip_can_type;
    FDDictionaryEntryAVP m_avp_offline;
    FDDictionaryEntryAVP m_avp_udp_source_port;
    FDDictionaryEntryAVP m_avp_routing_ip_address;
    FDDictionaryEntryAVP m_avp_adc_rule_name;
    FDDictionaryEntryAVP m_avp_error_message;
    FDDictionaryEntryAVP m_avp_mdt_allowed_plmn_id;
    FDDictionaryEntryAVP m_avp_reporting_trigger;
    FDDictionaryEntryAVP m_avp_failed_avp;
    FDDictionaryEntryAVP m_avp_routing_area_identity;
    FDDictionaryEntryAVP m_avp_trace_reference;
    FDDictionaryEntryAVP m_avp_flow_direction;
    FDDictionaryEntryAVP m_avp_measurement_quantity;
    FDDictionaryEntryAVP m_avp_trace_depth;
    FDDictionaryEntryAVP m_avp_proxy_state;
    FDDictionaryEntryAVP m_avp_rule_failure_code;
    FDDictionaryEntryAVP m_avp_vendor_id;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_ul;
    FDDictionaryEntryAVP m_avp_supported_features;
    FDDictionaryEntryAVP m_avp_redirect_support;
    FDDictionaryEntryAVP m_avp_destination_realm;
    FDDictionaryEntryAVP m_avp_session_id;
    FDDictionaryEntryAVP m_avp_tos_traffic_class;
    FDDictionaryEntryAVP m_avp_report_amount;
    FDDictionaryEntryAVP m_avp_mute_notification;
    FDDictionaryEntryAVP m_avp_security_parameter_index;
    FDDictionaryEntryAVP m_avp_measurement_period_umts;
    FDDictionaryEntryAVP m_avp_user_csg_information;

};

//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - TsRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// TSR request from Clients.
//-
class TSRcmd : public FDCommandRequest
{
public:
    TSRcmd( Application &app );
    ~TSRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    TSRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - TsRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a TSR
// to the Diameter application and process the corresponding answer.
//
class TSRreq : public FDMessageRequest
{
public:
    TSRreq( Application &app );
    virtual ~TSRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }
    void addSessionId( FDDictionaryEntryAVP &deSessionId ) { m_session.addSessionId( *this, deSessionId ); }

private:
    TSRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - SessionTerminationRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// SETR request from Clients.
//-
class SETRcmd : public FDCommandRequest
{
public:
    SETRcmd( Application &app );
    ~SETRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    SETRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - SessionTerminationRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a SETR
// to the Diameter application and process the corresponding answer.
//
class SETRreq : public FDMessageRequest
{
public:
    SETRreq( Application &app );
    virtual ~SETRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }
    void addSessionId( FDDictionaryEntryAVP &deSessionId ) { m_session.addSessionId( *this, deSessionId ); }

private:
    SETRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - TnRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// TNR request from Clients.
//-
class TNRcmd : public FDCommandRequest
{
public:
    TNRcmd( Application &app );
    ~TNRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    TNRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - TnRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a TNR
// to the Diameter application and process the corresponding answer.
//
class TNRreq : public FDMessageRequest
{
public:
    TNRreq( Application &app );
    virtual ~TNRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }
    void addSessionId( FDDictionaryEntryAVP &deSessionId ) { m_session.addSessionId( *this, deSessionId ); }

private:
    TNRreq();
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
class VendorSpecificApplicationIdExtractor : public FDExtractor
{
 public:
    VendorSpecificApplicationIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~VendorSpecificApplicationIdExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp auth_application_id;
    FDExtractorAvp acct_application_id;
};

class ExperimentalResultExtractor : public FDExtractor
{
 public:
    ExperimentalResultExtractor( FDExtractor &parent, Dictionary &dict );
    ~ExperimentalResultExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp experimental_result_code;
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

class PresenceReportingAreaInformationExtractor : public FDExtractor
{
 public:
    PresenceReportingAreaInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~PresenceReportingAreaInformationExtractor();

    FDExtractorAvp presence_reporting_area_identifier;
    FDExtractorAvp presence_reporting_area_status;
    FDExtractorAvp presence_reporting_area_elements_list;
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

class AdcRuleRemoveExtractor : public FDExtractor
{
 public:
    AdcRuleRemoveExtractor( FDExtractor &parent, Dictionary &dict );
    ~AdcRuleRemoveExtractor();

    FDExtractorAvpList adc_rule_name;
    FDExtractorAvpList adc_rule_base_name;
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

class AdcRuleDefinitionExtractor : public FDExtractor
{
 public:
    AdcRuleDefinitionExtractor( FDExtractor &parent, Dictionary &dict );
    ~AdcRuleDefinitionExtractor();

    FDExtractorAvp adc_rule_name;
    FDExtractorAvp tdf_application_identifier;
    FlowInformationExtractorList flow_information;
    FDExtractorAvp service_identifier;
    FDExtractorAvp rating_group;
    FDExtractorAvp reporting_level;
    FDExtractorAvp online;
    FDExtractorAvp offline;
    FDExtractorAvp metering_method;
    FDExtractorAvp precedence;
    FDExtractorAvp flow_status;
    QosInformationExtractor qos_information;
    FDExtractorAvp monitoring_key;
    FDExtractorAvp sponsor_identity;
    FDExtractorAvp application_service_provider_identity;
    RedirectInformationExtractor redirect_information;
    FDExtractorAvp mute_notification;
    FDExtractorAvp traffic_steering_policy_identifier_dl;
    FDExtractorAvp traffic_steering_policy_identifier_ul;
    FDExtractorAvp tos_traffic_class;
};

class AdcRuleReportExtractor : public FDExtractor
{
 public:
    AdcRuleReportExtractor( FDExtractor &parent, Dictionary &dict );
    ~AdcRuleReportExtractor();

    FDExtractorAvpList adc_rule_name;
    FDExtractorAvpList adc_rule_base_name;
    FDExtractorAvp pcc_rule_status;
    FDExtractorAvp rule_failure_code;
    FinalUnitIndicationExtractor final_unit_indication;
};

class LoadExtractor : public FDExtractor
{
 public:
    LoadExtractor( FDExtractor &parent, Dictionary &dict );
    ~LoadExtractor();

    FDExtractorAvp load_type;
    FDExtractorAvp load_value;
    FDExtractorAvp sourceid;
};

class OcSupportedFeaturesExtractor : public FDExtractor
{
 public:
    OcSupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict );
    ~OcSupportedFeaturesExtractor();

    FDExtractorAvp oc_feature_vector;
};

class AdcRuleDefinitionExtractorList : public FDExtractorList
{
 public:
    AdcRuleDefinitionExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AdcRuleDefinitionExtractorList();

    FDExtractor *createExtractor() { return new AdcRuleDefinitionExtractor( getParent(), m_dict ); }
    std::list<AdcRuleDefinitionExtractor*> &getList() { return (std::list<AdcRuleDefinitionExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AdcRuleInstallExtractor : public FDExtractor
{
 public:
    AdcRuleInstallExtractor( FDExtractor &parent, Dictionary &dict );
    ~AdcRuleInstallExtractor();

    AdcRuleDefinitionExtractorList adc_rule_definition;
    FDExtractorAvpList adc_rule_name;
    FDExtractorAvpList adc_rule_base_name;
    FDExtractorAvp monitoring_flags;
    FDExtractorAvp rule_activation_time;
    FDExtractorAvp rule_deactivation_time;
};

class FailedAvpExtractor : public FDExtractor
{
 public:
    FailedAvpExtractor( FDExtractor &parent, Dictionary &dict );
    ~FailedAvpExtractor();


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

class AdcRuleInstallExtractorList : public FDExtractorList
{
 public:
    AdcRuleInstallExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AdcRuleInstallExtractorList();

    FDExtractor *createExtractor() { return new AdcRuleInstallExtractor( getParent(), m_dict ); }
    std::list<AdcRuleInstallExtractor*> &getList() { return (std::list<AdcRuleInstallExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AdcRuleRemoveExtractorList : public FDExtractorList
{
 public:
    AdcRuleRemoveExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AdcRuleRemoveExtractorList();

    FDExtractor *createExtractor() { return new AdcRuleRemoveExtractor( getParent(), m_dict ); }
    std::list<AdcRuleRemoveExtractor*> &getList() { return (std::list<AdcRuleRemoveExtractor*>&)FDExtractorList::getList(); }

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

class TsRequestExtractor : public FDExtractor
{
 public:
    TsRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~TsRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp request_type;
    FDExtractorAvp destination_host;
    FDExtractorAvp origin_state_id;
    OcSupportedFeaturesExtractor oc_supported_features;
    EventReportIndicationExtractor event_report_indication;
    FDExtractorAvp framed_ip_address;
    FDExtractorAvp framed_ipv6_prefix;
    FDExtractorAvp called_station_id;
    AdcRuleInstallExtractorList adc_rule_install;
    AdcRuleRemoveExtractorList adc_rule_remove;
    SupportedFeaturesExtractorList supported_features;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class AdcRuleReportExtractorList : public FDExtractorList
{
 public:
    AdcRuleReportExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AdcRuleReportExtractorList();

    FDExtractor *createExtractor() { return new AdcRuleReportExtractor( getParent(), m_dict ); }
    std::list<AdcRuleReportExtractor*> &getList() { return (std::list<AdcRuleReportExtractor*>&)FDExtractorList::getList(); }

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

class TsAnswerExtractor : public FDExtractor
{
 public:
    TsAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~TsAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp origin_state_id;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    AdcRuleReportExtractorList adc_rule_report;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
    LoadExtractorList load;
};

class SessionTerminationRequestExtractor : public FDExtractor
{
 public:
    SessionTerminationRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~SessionTerminationRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp auth_application_id;
    FDExtractorAvp termination_cause;
    FDExtractorAvp destination_host;
    OcSupportedFeaturesExtractor oc_supported_features;
    FDExtractorAvpList _class;
    FDExtractorAvp origin_state_id;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class SessionTerminationAnswerExtractor : public FDExtractor
{
 public:
    SessionTerminationAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~SessionTerminationAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp result_code;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    FailedAvpExtractor failed_avp;
    FDExtractorAvp origin_state_id;
    FDExtractorAvpList _class;
    FDExtractorAvpList redirect_host;
    FDExtractorAvp redirect_host_usage;
    FDExtractorAvp redirect_max_cache_time;
    ProxyInfoExtractorList proxy_info;
    LoadExtractorList load;
};

class TnRequestExtractor : public FDExtractor
{
 public:
    TnRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~TnRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp origin_state_id;
    AdcRuleReportExtractorList adc_rule_report;
    OcSupportedFeaturesExtractor oc_supported_features;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class TnAnswerExtractor : public FDExtractor
{
 public:
    TnAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~TnAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    FDExtractorAvp origin_state_id;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};


}

#endif // __ST_H

