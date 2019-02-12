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

#ifndef __S6AS6D_H
#define __S6AS6D_H

#include "fd.h"

namespace s6as6d {

//
// Primary Objects
//

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    FDDictionaryEntryApplication &app()                                    { return m_app; }

    FDDictionaryEntryCommand &cmdUPLR()                                   	{ return m_cmd_uplr; }
    FDDictionaryEntryCommand &cmdUPLA()                                   	{ return m_cmd_upla; }
    FDDictionaryEntryCommand &cmdCALR()                                   	{ return m_cmd_calr; }
    FDDictionaryEntryCommand &cmdCALA()                                   	{ return m_cmd_cala; }
    FDDictionaryEntryCommand &cmdAUIR()                                   	{ return m_cmd_auir; }
    FDDictionaryEntryCommand &cmdAUIA()                                   	{ return m_cmd_auia; }
    FDDictionaryEntryCommand &cmdINSDR()                                  	{ return m_cmd_insdr; }
    FDDictionaryEntryCommand &cmdINSDA()                                  	{ return m_cmd_insda; }
    FDDictionaryEntryCommand &cmdDESDR()                                  	{ return m_cmd_desdr; }
    FDDictionaryEntryCommand &cmdDESDA()                                  	{ return m_cmd_desda; }
    FDDictionaryEntryCommand &cmdPUUR()                                   	{ return m_cmd_puur; }
    FDDictionaryEntryCommand &cmdPUUA()                                   	{ return m_cmd_puua; }
    FDDictionaryEntryCommand &cmdRER()                                    	{ return m_cmd_rer; }
    FDDictionaryEntryCommand &cmdREA()                                    	{ return m_cmd_rea; }

    FDDictionaryEntryVendor &vnd3GPP()                                    	{ return m_vnd_3gpp; }

    FDDictionaryEntryAVP &avpTsCode()                                     	{ return m_avp_ts_code; }
    FDDictionaryEntryAVP &avpReachabilityType()                           	{ return m_avp_reachability_type; }
    FDDictionaryEntryAVP &avpEpsUserState()                               	{ return m_avp_eps_user_state; }
    FDDictionaryEntryAVP &avpPreEmptionCapability()                       	{ return m_avp_pre_emption_capability; }
    FDDictionaryEntryAVP &avpFeatureListId()                              	{ return m_avp_feature_list_id; }
    FDDictionaryEntryAVP &avpServiceAreaIdentity()                        	{ return m_avp_service_area_identity; }
    FDDictionaryEntryAVP &avpDayOfWeekMask()                              	{ return m_avp_day_of_week_mask; }
    FDDictionaryEntryAVP &avpAllApnConfigurationsIncludedIndicator()      	{ return m_avp_all_apn_configurations_included_indicator; }
    FDDictionaryEntryAVP &avpEUtranVector()                               	{ return m_avp_e_utran_vector; }
    FDDictionaryEntryAVP &avpPuaFlags()                                   	{ return m_avp_pua_flags; }
    FDDictionaryEntryAVP &avpMaximumResponseTime()                        	{ return m_avp_maximum_response_time; }
    FDDictionaryEntryAVP &avpVendorSpecificApplicationId()                	{ return m_avp_vendor_specific_application_id; }
    FDDictionaryEntryAVP &avpCsgAccessMode()                              	{ return m_avp_csg_access_mode; }
    FDDictionaryEntryAVP &avpAdditionalContextIdentifier()                	{ return m_avp_additional_context_identifier; }
    FDDictionaryEntryAVP &avpAccessRestrictionData()                      	{ return m_avp_access_restriction_data; }
    FDDictionaryEntryAVP &avpMip6HomeLinkPrefix()                         	{ return m_avp_mip6_home_link_prefix; }
    FDDictionaryEntryAVP &avpGprsSubscriptionData()                       	{ return m_avp_gprs_subscription_data; }
    FDDictionaryEntryAVP &avpSres()                                       	{ return m_avp_sres; }
    FDDictionaryEntryAVP &avpSourceid()                                   	{ return m_avp_sourceid; }
    FDDictionaryEntryAVP &avpLoadValue()                                  	{ return m_avp_load_value; }
    FDDictionaryEntryAVP &avpFeatureList()                                	{ return m_avp_feature_list; }
    FDDictionaryEntryAVP &avpApnOiReplacement()                           	{ return m_avp_apn_oi_replacement; }
    FDDictionaryEntryAVP &avpMonitoringEventConfigStatus()                	{ return m_avp_monitoring_event_config_status; }
    FDDictionaryEntryAVP &avpOcReportType()                               	{ return m_avp_oc_report_type; }
    FDDictionaryEntryAVP &avpExperimentalResult()                         	{ return m_avp_experimental_result; }
    FDDictionaryEntryAVP &avpEdrxCycleLength()                            	{ return m_avp_edrx_cycle_length; }
    FDDictionaryEntryAVP &avpImsVoiceOverPsSessionsSupported()            	{ return m_avp_ims_voice_over_ps_sessions_supported; }
    FDDictionaryEntryAVP &avpUeUsageType()                                	{ return m_avp_ue_usage_type; }
    FDDictionaryEntryAVP &avpUserId()                                     	{ return m_avp_user_id; }
    FDDictionaryEntryAVP &avpLcsPrivacyexception()                        	{ return m_avp_lcs_privacyexception; }
    FDDictionaryEntryAVP &avpV2xSubscriptionData()                        	{ return m_avp_v2x_subscription_data; }
    FDDictionaryEntryAVP &avpAdjacentAccessRestrictionData()              	{ return m_avp_adjacent_access_restriction_data; }
    FDDictionaryEntryAVP &avpQosClassIdentifier()                         	{ return m_avp_qos_class_identifier; }
    FDDictionaryEntryAVP &avpExtPdpType()                                 	{ return m_avp_ext_pdp_type; }
    FDDictionaryEntryAVP &avpOcFeatureVector()                            	{ return m_avp_oc_feature_vector; }
    FDDictionaryEntryAVP &avpRelayNodeIndicator()                         	{ return m_avp_relay_node_indicator; }
    FDDictionaryEntryAVP &avpCommunicationDurationTime()                  	{ return m_avp_communication_duration_time; }
    FDDictionaryEntryAVP &avpAcctApplicationId()                          	{ return m_avp_acct_application_id; }
    FDDictionaryEntryAVP &avpItemNumber()                                 	{ return m_avp_item_number; }
    FDDictionaryEntryAVP &avpListOfMeasurements()                         	{ return m_avp_list_of_measurements; }
    FDDictionaryEntryAVP &avpGeranVector()                                	{ return m_avp_geran_vector; }
    FDDictionaryEntryAVP &avpAuthSessionState()                           	{ return m_avp_auth_session_state; }
    FDDictionaryEntryAVP &avpEpsLocationInformation()                     	{ return m_avp_eps_location_information; }
    FDDictionaryEntryAVP &avpDrmp()                                       	{ return m_avp_drmp; }
    FDDictionaryEntryAVP &avpTraceNeTypeList()                            	{ return m_avp_trace_ne_type_list; }
    FDDictionaryEntryAVP &avpAdjacentPlmns()                              	{ return m_avp_adjacent_plmns; }
    FDDictionaryEntryAVP &avpPriorityLevel()                              	{ return m_avp_priority_level; }
    FDDictionaryEntryAVP &avpPreEmptionVulnerability()                    	{ return m_avp_pre_emption_vulnerability; }
    FDDictionaryEntryAVP &avpGeodeticInformation()                        	{ return m_avp_geodetic_information; }
    FDDictionaryEntryAVP &avpLoadType()                                   	{ return m_avp_load_type; }
    FDDictionaryEntryAVP &avpAreaScope()                                  	{ return m_avp_area_scope; }
    FDDictionaryEntryAVP &avpPreferredDataMode()                          	{ return m_avp_preferred_data_mode; }
    FDDictionaryEntryAVP &avpImei()                                       	{ return m_avp_imei; }
    FDDictionaryEntryAVP &avpReachabilityInformation()                    	{ return m_avp_reachability_information; }
    FDDictionaryEntryAVP &avpNodeType()                                   	{ return m_avp_node_type; }
    FDDictionaryEntryAVP &avpClrFlags()                                   	{ return m_avp_clr_flags; }
    FDDictionaryEntryAVP &avpCallBarringInfo()                            	{ return m_avp_call_barring_info; }
    FDDictionaryEntryAVP &avpServiceReport()                              	{ return m_avp_service_report; }
    FDDictionaryEntryAVP &avpDestinationHost()                            	{ return m_avp_destination_host; }
    FDDictionaryEntryAVP &avpGmlcAddress()                                	{ return m_avp_gmlc_address; }
    FDDictionaryEntryAVP &avpLocationAreaIdentity()                       	{ return m_avp_location_area_identity; }
    FDDictionaryEntryAVP &avpLoggingInterval()                            	{ return m_avp_logging_interval; }
    FDDictionaryEntryAVP &avpRoamingRestrictedDueToUnsupportedFeature()   	{ return m_avp_roaming_restricted_due_to_unsupported_feature; }
    FDDictionaryEntryAVP &avpRatType()                                    	{ return m_avp_rat_type; }
    FDDictionaryEntryAVP &avpEdrxCycleLengthValue()                       	{ return m_avp_edrx_cycle_length_value; }
    FDDictionaryEntryAVP &avpTimeZone()                                   	{ return m_avp_time_zone; }
    FDDictionaryEntryAVP &avpMsisdn()                                     	{ return m_avp_msisdn; }
    FDDictionaryEntryAVP &avpMoLr()                                       	{ return m_avp_mo_lr; }
    FDDictionaryEntryAVP &avpActiveApn()                                  	{ return m_avp_active_apn; }
    FDDictionaryEntryAVP &avpJobType()                                    	{ return m_avp_job_type; }
    FDDictionaryEntryAVP &avpSsCode()                                     	{ return m_avp_ss_code; }
    FDDictionaryEntryAVP &avpTimeOfDayStart()                             	{ return m_avp_time_of_day_start; }
    FDDictionaryEntryAVP &avpWlanOffloadability()                         	{ return m_avp_wlan_offloadability; }
    FDDictionaryEntryAVP &avpMipHomeAgentHost()                           	{ return m_avp_mip_home_agent_host; }
    FDDictionaryEntryAVP &avpPdnType()                                    	{ return m_avp_pdn_type; }
    FDDictionaryEntryAVP &avpCommunicationPatternSet()                    	{ return m_avp_communication_pattern_set; }
    FDDictionaryEntryAVP &avpPeriodicCommunicationIndicator()             	{ return m_avp_periodic_communication_indicator; }
    FDDictionaryEntryAVP &avpProsePermission()                            	{ return m_avp_prose_permission; }
    FDDictionaryEntryAVP &avpEquivalentPlmnList()                         	{ return m_avp_equivalent_plmn_list; }
    FDDictionaryEntryAVP &avpChargedParty()                               	{ return m_avp_charged_party; }
    FDDictionaryEntryAVP &avpSupportedServices()                          	{ return m_avp_supported_services; }
    FDDictionaryEntryAVP &avp3gppChargingCharacteristics()                	{ return m_avp_3gpp_charging_characteristics; }
    FDDictionaryEntryAVP &avpProxyInfo()                                  	{ return m_avp_proxy_info; }
    FDDictionaryEntryAVP &avpSubscribedPeriodicRauTauTimer()              	{ return m_avp_subscribed_periodic_rau_tau_timer; }
    FDDictionaryEntryAVP &avpSsStatus()                                   	{ return m_avp_ss_status; }
    FDDictionaryEntryAVP &avpEnodebId()                                   	{ return m_avp_enodeb_id; }
    FDDictionaryEntryAVP &avpUtranVector()                                	{ return m_avp_utran_vector; }
    FDDictionaryEntryAVP &avpOriginRealm()                                	{ return m_avp_origin_realm; }
    FDDictionaryEntryAVP &avpOriginHost()                                 	{ return m_avp_origin_host; }
    FDDictionaryEntryAVP &avpEventThresholdRsrq()                         	{ return m_avp_event_threshold_rsrq; }
    FDDictionaryEntryAVP &avpVplmnDynamicAddressAllowed()                 	{ return m_avp_vplmn_dynamic_address_allowed; }
    FDDictionaryEntryAVP &avpAccuracy()                                   	{ return m_avp_accuracy; }
    FDDictionaryEntryAVP &avpPurFlags()                                   	{ return m_avp_pur_flags; }
    FDDictionaryEntryAVP &avpUeReachabilityConfiguration()                	{ return m_avp_ue_reachability_configuration; }
    FDDictionaryEntryAVP &avpApnConfiguration()                           	{ return m_avp_apn_configuration; }
    FDDictionaryEntryAVP &avpAMsisdn()                                    	{ return m_avp_a_msisdn; }
    FDDictionaryEntryAVP &avpMonteLocationType()                          	{ return m_avp_monte_location_type; }
    FDDictionaryEntryAVP &avpLastUeActivityTime()                         	{ return m_avp_last_ue_activity_time; }
    FDDictionaryEntryAVP &avpLocalTimeZone()                              	{ return m_avp_local_time_zone; }
    FDDictionaryEntryAVP &avpAuthenticationInfo()                         	{ return m_avp_authentication_info; }
    FDDictionaryEntryAVP &avpServiceType()                                	{ return m_avp_service_type; }
    FDDictionaryEntryAVP &avpV2xPermission()                              	{ return m_avp_v2x_permission; }
    FDDictionaryEntryAVP &avpPdpAddress()                                 	{ return m_avp_pdp_address; }
    FDDictionaryEntryAVP &avpImsiGroupId()                                	{ return m_avp_imsi_group_id; }
    FDDictionaryEntryAVP &avpDsrFlags()                                   	{ return m_avp_dsr_flags; }
    FDDictionaryEntryAVP &avpCompleteDataListIncludedIndicator()          	{ return m_avp_complete_data_list_included_indicator; }
    FDDictionaryEntryAVP &avpTraceCollectionEntity()                      	{ return m_avp_trace_collection_entity; }
    FDDictionaryEntryAVP &avpWlanOffloadabilityEutran()                   	{ return m_avp_wlan_offloadability_eutran; }
    FDDictionaryEntryAVP &avpRouteRecord()                                	{ return m_avp_route_record; }
    FDDictionaryEntryAVP &avpSubscriptionData()                           	{ return m_avp_subscription_data; }
    FDDictionaryEntryAVP &avpReferenceIdValidityTime()                    	{ return m_avp_reference_id_validity_time; }
    FDDictionaryEntryAVP &avpDlBufferingSuggestedPacketCount()            	{ return m_avp_dl_buffering_suggested_packet_count; }
    FDDictionaryEntryAVP &avpScefId()                                     	{ return m_avp_scef_id; }
    FDDictionaryEntryAVP &avpTraceEventList()                             	{ return m_avp_trace_event_list; }
    FDDictionaryEntryAVP &avpCsgId()                                      	{ return m_avp_csg_id; }
    FDDictionaryEntryAVP &avpMdtConfiguration()                           	{ return m_avp_mdt_configuration; }
    FDDictionaryEntryAVP &avpErrorDiagnostic()                            	{ return m_avp_error_diagnostic; }
    FDDictionaryEntryAVP &avpRoutingAreaIdentity()                        	{ return m_avp_routing_area_identity; }
    FDDictionaryEntryAVP &avpCurrentLocationRetrieved()                   	{ return m_avp_current_location_retrieved; }
    FDDictionaryEntryAVP &avpUserCsgInformation()                         	{ return m_avp_user_csg_information; }
    FDDictionaryEntryAVP &avpMip6AgentInfo()                              	{ return m_avp_mip6_agent_info; }
    FDDictionaryEntryAVP &avpLoggingDuration()                            	{ return m_avp_logging_duration; }
    FDDictionaryEntryAVP &avpGroupPlmnId()                                	{ return m_avp_group_plmn_id; }
    FDDictionaryEntryAVP &avpCollectionPeriodRrmUmts()                    	{ return m_avp_collection_period_rrm_umts; }
    FDDictionaryEntryAVP &avpIdaFlags()                                   	{ return m_avp_ida_flags; }
    FDDictionaryEntryAVP &avpNonIpPdnTypeIndicator()                      	{ return m_avp_non_ip_pdn_type_indicator; }
    FDDictionaryEntryAVP &avpTrackingAreaIdentity()                       	{ return m_avp_tracking_area_identity; }
    FDDictionaryEntryAVP &avpOcOlr()                                      	{ return m_avp_oc_olr; }
    FDDictionaryEntryAVP &avpClientIdentity()                             	{ return m_avp_client_identity; }
    FDDictionaryEntryAVP &avpAgeOfLocationInformation()                   	{ return m_avp_age_of_location_information; }
    FDDictionaryEntryAVP &avpTraceData()                                  	{ return m_avp_trace_data; }
    FDDictionaryEntryAVP &avpSubscribedVsrvcc()                           	{ return m_avp_subscribed_vsrvcc; }
    FDDictionaryEntryAVP &avpPlmnClient()                                 	{ return m_avp_plmn_client; }
    FDDictionaryEntryAVP &avpVplmnLipaAllowed()                           	{ return m_avp_vplmn_lipa_allowed; }
    FDDictionaryEntryAVP &avpReSynchronizationInfo()                      	{ return m_avp_re_synchronization_info; }
    FDDictionaryEntryAVP &avpAirFlags()                                   	{ return m_avp_air_flags; }
    FDDictionaryEntryAVP &avpSpecificApnInfo()                            	{ return m_avp_specific_apn_info; }
    FDDictionaryEntryAVP &avpRand()                                       	{ return m_avp_rand; }
    FDDictionaryEntryAVP &avpOcReductionPercentage()                      	{ return m_avp_oc_reduction_percentage; }
    FDDictionaryEntryAVP &avpReportInterval()                             	{ return m_avp_report_interval; }
    FDDictionaryEntryAVP &avpEpsSubscribedQosProfile()                    	{ return m_avp_eps_subscribed_qos_profile; }
    FDDictionaryEntryAVP &avpServiceResultCode()                          	{ return m_avp_service_result_code; }
    FDDictionaryEntryAVP &avpOmcId()                                      	{ return m_avp_omc_id; }
    FDDictionaryEntryAVP &avpOcSequenceNumber()                           	{ return m_avp_oc_sequence_number; }
    FDDictionaryEntryAVP &avpTeleserviceList()                            	{ return m_avp_teleservice_list; }
    FDDictionaryEntryAVP &avpApnConfigurationProfile()                    	{ return m_avp_apn_configuration_profile; }
    FDDictionaryEntryAVP &avpEventThresholdEvent1i()                      	{ return m_avp_event_threshold_event_1i; }
    FDDictionaryEntryAVP &avpSupportedMonitoringEvents()                  	{ return m_avp_supported_monitoring_events; }
    FDDictionaryEntryAVP &avpDsaFlags()                                   	{ return m_avp_dsa_flags; }
    FDDictionaryEntryAVP &avpEventThresholdEvent1f()                      	{ return m_avp_event_threshold_event_1f; }
    FDDictionaryEntryAVP &avpAllocationRetentionPriority()                	{ return m_avp_allocation_retention_priority; }
    FDDictionaryEntryAVP &avpCellGlobalIdentity()                         	{ return m_avp_cell_global_identity; }
    FDDictionaryEntryAVP &avpOcValidityDuration()                         	{ return m_avp_oc_validity_duration; }
    FDDictionaryEntryAVP &avpUlaFlags()                                   	{ return m_avp_ula_flags; }
    FDDictionaryEntryAVP &avpCsgMembershipIndication()                    	{ return m_avp_csg_membership_indication; }
    FDDictionaryEntryAVP &avpServiceSelection()                           	{ return m_avp_service_selection; }
    FDDictionaryEntryAVP &avpNetworkAccessMode()                          	{ return m_avp_network_access_mode; }
    FDDictionaryEntryAVP &avp3gpp2Meid()                                  	{ return m_avp_3gpp2_meid; }
    FDDictionaryEntryAVP &avpMmeLocationInformation()                     	{ return m_avp_mme_location_information; }
    FDDictionaryEntryAVP &avpSgsnLocationInformation()                    	{ return m_avp_sgsn_location_information; }
    FDDictionaryEntryAVP &avpUlrFlags()                                   	{ return m_avp_ulr_flags; }
    FDDictionaryEntryAVP &avpMeasurementPeriodLte()                       	{ return m_avp_measurement_period_lte; }
    FDDictionaryEntryAVP &avpResultCode()                                 	{ return m_avp_result_code; }
    FDDictionaryEntryAVP &avpGmlcRestriction()                            	{ return m_avp_gmlc_restriction; }
    FDDictionaryEntryAVP &avpUserState()                                  	{ return m_avp_user_state; }
    FDDictionaryEntryAVP &avpSgsMmeIdentity()                             	{ return m_avp_sgs_mme_identity; }
    FDDictionaryEntryAVP &avpCoupledNodeDiameterId()                      	{ return m_avp_coupled_node_diameter_id; }
    FDDictionaryEntryAVP &avpKc()                                         	{ return m_avp_kc; }
    FDDictionaryEntryAVP &avpWlanOffloadabilityUtran()                    	{ return m_avp_wlan_offloadability_utran; }
    FDDictionaryEntryAVP &avpServicetypeidentity()                        	{ return m_avp_servicetypeidentity; }
    FDDictionaryEntryAVP &avpStnSr()                                      	{ return m_avp_stn_sr; }
    FDDictionaryEntryAVP &avpMmeNumberForMtSms()                          	{ return m_avp_mme_number_for_mt_sms; }
    FDDictionaryEntryAVP &avpRequestedUtranGeranAuthenticationInfo()      	{ return m_avp_requested_utran_geran_authentication_info; }
    FDDictionaryEntryAVP &avpNotificationToUeUser()                       	{ return m_avp_notification_to_ue_user; }
    FDDictionaryEntryAVP &avpHomogeneousSupportOfImsVoiceOverPsSessions() 	{ return m_avp_homogeneous_support_of_ims_voice_over_ps_sessions; }
    FDDictionaryEntryAVP &avpPdpContext()                                 	{ return m_avp_pdp_context; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthDl()                    	{ return m_avp_max_requested_bandwidth_dl; }
    FDDictionaryEntryAVP &avpTraceInterfaceList()                         	{ return m_avp_trace_interface_list; }
    FDDictionaryEntryAVP &avpPdnConnectionContinuity()                    	{ return m_avp_pdn_connection_continuity; }
    FDDictionaryEntryAVP &avpScefReferenceId()                            	{ return m_avp_scef_reference_id; }
    FDDictionaryEntryAVP &avpSgsnNumber()                                 	{ return m_avp_sgsn_number; }
    FDDictionaryEntryAVP &avpRatFrequencySelectionPriorityId()            	{ return m_avp_rat_frequency_selection_priority_id; }
    FDDictionaryEntryAVP &avpNumberOfRequestedVectors()                   	{ return m_avp_number_of_requested_vectors; }
    FDDictionaryEntryAVP &avpSubscriptionDataDeletion()                   	{ return m_avp_subscription_data_deletion; }
    FDDictionaryEntryAVP &avpLoad()                                       	{ return m_avp_load; }
    FDDictionaryEntryAVP &avpContextIdentifier()                          	{ return m_avp_context_identifier; }
    FDDictionaryEntryAVP &avpIntegrityKey()                               	{ return m_avp_integrity_key; }
    FDDictionaryEntryAVP &avpConfidentialityKey()                         	{ return m_avp_confidentiality_key; }
    FDDictionaryEntryAVP &avpEUtranCellGlobalIdentity()                   	{ return m_avp_e_utran_cell_global_identity; }
    FDDictionaryEntryAVP &avpOcSupportedFeatures()                        	{ return m_avp_oc_supported_features; }
    FDDictionaryEntryAVP &avpMonitoringType()                             	{ return m_avp_monitoring_type; }
    FDDictionaryEntryAVP &avpUePc5Ambr()                                  	{ return m_avp_ue_pc5_ambr; }
    FDDictionaryEntryAVP &avpProxyHost()                                  	{ return m_avp_proxy_host; }
    FDDictionaryEntryAVP &avpEventThresholdRsrp()                         	{ return m_avp_event_threshold_rsrp; }
    FDDictionaryEntryAVP &avpTerminalInformation()                        	{ return m_avp_terminal_information; }
    FDDictionaryEntryAVP &avpXres()                                       	{ return m_avp_xres; }
    FDDictionaryEntryAVP &avpSubscriberStatus()                           	{ return m_avp_subscriber_status; }
    FDDictionaryEntryAVP &avpExperimentalResultCode()                     	{ return m_avp_experimental_result_code; }
    FDDictionaryEntryAVP &avpCollectionPeriodRrmLte()                     	{ return m_avp_collection_period_rrm_lte; }
    FDDictionaryEntryAVP &avpMaximumNumberOfReports()                     	{ return m_avp_maximum_number_of_reports; }
    FDDictionaryEntryAVP &avpHplmnOdb()                                   	{ return m_avp_hplmn_odb; }
    FDDictionaryEntryAVP &avpMmeUserState()                               	{ return m_avp_mme_user_state; }
    FDDictionaryEntryAVP &avpAuthApplicationId()                          	{ return m_avp_auth_application_id; }
    FDDictionaryEntryAVP &avpResetId()                                    	{ return m_avp_reset_id; }
    FDDictionaryEntryAVP &avpLipaPermission()                             	{ return m_avp_lipa_permission; }
    FDDictionaryEntryAVP &avpRequestedEutranAuthenticationInfo()          	{ return m_avp_requested_eutran_authentication_info; }
    FDDictionaryEntryAVP &avpMpsPriority()                                	{ return m_avp_mps_priority; }
    FDDictionaryEntryAVP &avpMonitoringEventReport()                      	{ return m_avp_monitoring_event_report; }
    FDDictionaryEntryAVP &avpExtendedEnodebId()                           	{ return m_avp_extended_enodeb_id; }
    FDDictionaryEntryAVP &avpIdrFlags()                                   	{ return m_avp_idr_flags; }
    FDDictionaryEntryAVP &avpAeseCommunicationPattern()                   	{ return m_avp_aese_communication_pattern; }
    FDDictionaryEntryAVP &avpVisitedNetworkIdentifier()                   	{ return m_avp_visited_network_identifier; }
    FDDictionaryEntryAVP &avpLcsInfo()                                    	{ return m_avp_lcs_info; }
    FDDictionaryEntryAVP &avpRestorationPriority()                        	{ return m_avp_restoration_priority; }
    FDDictionaryEntryAVP &avpMdtUserConsent()                             	{ return m_avp_mdt_user_consent; }
    FDDictionaryEntryAVP &avpCancellationType()                           	{ return m_avp_cancellation_type; }
    FDDictionaryEntryAVP &avpStationaryIndication()                       	{ return m_avp_stationary_indication; }
    FDDictionaryEntryAVP &avpPdnGwAllocationType()                        	{ return m_avp_pdn_gw_allocation_type; }
    FDDictionaryEntryAVP &avpGmlcNumber()                                 	{ return m_avp_gmlc_number; }
    FDDictionaryEntryAVP &avpSubscriptionDataFlags()                      	{ return m_avp_subscription_data_flags; }
    FDDictionaryEntryAVP &avpIcsIndicator()                               	{ return m_avp_ics_indicator; }
    FDDictionaryEntryAVP &avpVisitedPlmnId()                              	{ return m_avp_visited_plmn_id; }
    FDDictionaryEntryAVP &avpQosSubscribed()                              	{ return m_avp_qos_subscribed; }
    FDDictionaryEntryAVP &avpPositioningMethod()                          	{ return m_avp_positioning_method; }
    FDDictionaryEntryAVP &avpMonitoringDuration()                         	{ return m_avp_monitoring_duration; }
    FDDictionaryEntryAVP &avpReportingTrigger()                           	{ return m_avp_reporting_trigger; }
    FDDictionaryEntryAVP &avpFailedAvp()                                  	{ return m_avp_failed_avp; }
    FDDictionaryEntryAVP &avpAmbr()                                       	{ return m_avp_ambr; }
    FDDictionaryEntryAVP &avpDaylightSavingTime()                         	{ return m_avp_daylight_saving_time; }
    FDDictionaryEntryAVP &avpExpirationDate()                             	{ return m_avp_expiration_date; }
    FDDictionaryEntryAVP &avpGroupServiceId()                             	{ return m_avp_group_service_id; }
    FDDictionaryEntryAVP &avpTraceReference()                             	{ return m_avp_trace_reference; }
    FDDictionaryEntryAVP &avpLocalGroupId()                               	{ return m_avp_local_group_id; }
    FDDictionaryEntryAVP &avpMeasurementQuantity()                        	{ return m_avp_measurement_quantity; }
    FDDictionaryEntryAVP &avpGeographicalInformation()                    	{ return m_avp_geographical_information; }
    FDDictionaryEntryAVP &avpKasme()                                      	{ return m_avp_kasme; }
    FDDictionaryEntryAVP &avpServedPartyIpAddress()                       	{ return m_avp_served_party_ip_address; }
    FDDictionaryEntryAVP &avpTraceDepth()                                 	{ return m_avp_trace_depth; }
    FDDictionaryEntryAVP &avpScheduledCommunicationTime()                 	{ return m_avp_scheduled_communication_time; }
    FDDictionaryEntryAVP &avpProxyState()                                 	{ return m_avp_proxy_state; }
    FDDictionaryEntryAVP &avpOperatorDeterminedBarring()                  	{ return m_avp_operator_determined_barring; }
    FDDictionaryEntryAVP &avpAutn()                                       	{ return m_avp_autn; }
    FDDictionaryEntryAVP &avpPdpType()                                    	{ return m_avp_pdp_type; }
    FDDictionaryEntryAVP &avpPeriodicTime()                               	{ return m_avp_periodic_time; }
    FDDictionaryEntryAVP &avpTimeOfDayEnd()                               	{ return m_avp_time_of_day_end; }
    FDDictionaryEntryAVP &avpSoftwareVersion()                            	{ return m_avp_software_version; }
    FDDictionaryEntryAVP &avpSiptoLocalNetworkPermission()                	{ return m_avp_sipto_local_network_permission; }
    FDDictionaryEntryAVP &avpSmsRegisterRequest()                         	{ return m_avp_sms_register_request; }
    FDDictionaryEntryAVP &avpSgsnUserState()                              	{ return m_avp_sgsn_user_state; }
    FDDictionaryEntryAVP &avpVendorId()                                   	{ return m_avp_vendor_id; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthUl()                    	{ return m_avp_max_requested_bandwidth_ul; }
    FDDictionaryEntryAVP &avpSupportedFeatures()                          	{ return m_avp_supported_features; }
    FDDictionaryEntryAVP &avpNonIpDataDeliveryMechanism()                 	{ return m_avp_non_ip_data_delivery_mechanism; }
    FDDictionaryEntryAVP &avpMonitoringEventConfiguration()               	{ return m_avp_monitoring_event_configuration; }
    FDDictionaryEntryAVP &avpDestinationRealm()                           	{ return m_avp_destination_realm; }
    FDDictionaryEntryAVP &avpSessionId()                                  	{ return m_avp_session_id; }
    FDDictionaryEntryAVP &avpImmediateResponsePreferred()                 	{ return m_avp_immediate_response_preferred; }
    FDDictionaryEntryAVP &avpUserName()                                   	{ return m_avp_user_name; }
    FDDictionaryEntryAVP &avpScefRealm()                                  	{ return m_avp_scef_realm; }
    FDDictionaryEntryAVP &avpSiptoPermission()                            	{ return m_avp_sipto_permission; }
    FDDictionaryEntryAVP &avpEmergencyInfo()                              	{ return m_avp_emergency_info; }
    FDDictionaryEntryAVP &avpMdtAllowedPlmnId()                           	{ return m_avp_mdt_allowed_plmn_id; }
    FDDictionaryEntryAVP &avpReportAmount()                               	{ return m_avp_report_amount; }
    FDDictionaryEntryAVP &avpCsgSubscriptionData()                        	{ return m_avp_csg_subscription_data; }
    FDDictionaryEntryAVP &avpExternalClient()                             	{ return m_avp_external_client; }
    FDDictionaryEntryAVP &avpMipHomeAgentAddress()                        	{ return m_avp_mip_home_agent_address; }
    FDDictionaryEntryAVP &avpUeSrvccCapability()                          	{ return m_avp_ue_srvcc_capability; }
    FDDictionaryEntryAVP &avpMeasurementPeriodUmts()                      	{ return m_avp_measurement_period_umts; }
    FDDictionaryEntryAVP &avpScefReferenceIdForDeletion()                 	{ return m_avp_scef_reference_id_for_deletion; }
    FDDictionaryEntryAVP &avpProseSubscriptionData()                      	{ return m_avp_prose_subscription_data; }
    FDDictionaryEntryAVP &avpServiceResult()                              	{ return m_avp_service_result; }
    FDDictionaryEntryAVP &avpLocationInformationConfiguration()           	{ return m_avp_location_information_configuration; }
    FDDictionaryEntryAVP &avpExtPdpAddress()                              	{ return m_avp_ext_pdp_address; }
    FDDictionaryEntryAVP &avpRegionalSubscriptionZoneCode()               	{ return m_avp_regional_subscription_zone_code; }

private:
    FDDictionaryEntryApplication m_app;                                   

    FDDictionaryEntryCommand m_cmd_uplr;
    FDDictionaryEntryCommand m_cmd_upla;
    FDDictionaryEntryCommand m_cmd_calr;
    FDDictionaryEntryCommand m_cmd_cala;
    FDDictionaryEntryCommand m_cmd_auir;
    FDDictionaryEntryCommand m_cmd_auia;
    FDDictionaryEntryCommand m_cmd_insdr;
    FDDictionaryEntryCommand m_cmd_insda;
    FDDictionaryEntryCommand m_cmd_desdr;
    FDDictionaryEntryCommand m_cmd_desda;
    FDDictionaryEntryCommand m_cmd_puur;
    FDDictionaryEntryCommand m_cmd_puua;
    FDDictionaryEntryCommand m_cmd_rer;
    FDDictionaryEntryCommand m_cmd_rea;

    FDDictionaryEntryVendor m_vnd_3gpp;

    FDDictionaryEntryAVP m_avp_ts_code;
    FDDictionaryEntryAVP m_avp_reachability_type;
    FDDictionaryEntryAVP m_avp_eps_user_state;
    FDDictionaryEntryAVP m_avp_pre_emption_capability;
    FDDictionaryEntryAVP m_avp_feature_list_id;
    FDDictionaryEntryAVP m_avp_service_area_identity;
    FDDictionaryEntryAVP m_avp_day_of_week_mask;
    FDDictionaryEntryAVP m_avp_all_apn_configurations_included_indicator;
    FDDictionaryEntryAVP m_avp_e_utran_vector;
    FDDictionaryEntryAVP m_avp_pua_flags;
    FDDictionaryEntryAVP m_avp_maximum_response_time;
    FDDictionaryEntryAVP m_avp_vendor_specific_application_id;
    FDDictionaryEntryAVP m_avp_csg_access_mode;
    FDDictionaryEntryAVP m_avp_additional_context_identifier;
    FDDictionaryEntryAVP m_avp_access_restriction_data;
    FDDictionaryEntryAVP m_avp_mip6_home_link_prefix;
    FDDictionaryEntryAVP m_avp_gprs_subscription_data;
    FDDictionaryEntryAVP m_avp_sres;
    FDDictionaryEntryAVP m_avp_sourceid;
    FDDictionaryEntryAVP m_avp_load_value;
    FDDictionaryEntryAVP m_avp_feature_list;
    FDDictionaryEntryAVP m_avp_apn_oi_replacement;
    FDDictionaryEntryAVP m_avp_monitoring_event_config_status;
    FDDictionaryEntryAVP m_avp_oc_report_type;
    FDDictionaryEntryAVP m_avp_experimental_result;
    FDDictionaryEntryAVP m_avp_edrx_cycle_length;
    FDDictionaryEntryAVP m_avp_ims_voice_over_ps_sessions_supported;
    FDDictionaryEntryAVP m_avp_ue_usage_type;
    FDDictionaryEntryAVP m_avp_user_id;
    FDDictionaryEntryAVP m_avp_lcs_privacyexception;
    FDDictionaryEntryAVP m_avp_v2x_subscription_data;
    FDDictionaryEntryAVP m_avp_adjacent_access_restriction_data;
    FDDictionaryEntryAVP m_avp_qos_class_identifier;
    FDDictionaryEntryAVP m_avp_ext_pdp_type;
    FDDictionaryEntryAVP m_avp_oc_feature_vector;
    FDDictionaryEntryAVP m_avp_relay_node_indicator;
    FDDictionaryEntryAVP m_avp_communication_duration_time;
    FDDictionaryEntryAVP m_avp_acct_application_id;
    FDDictionaryEntryAVP m_avp_item_number;
    FDDictionaryEntryAVP m_avp_list_of_measurements;
    FDDictionaryEntryAVP m_avp_geran_vector;
    FDDictionaryEntryAVP m_avp_auth_session_state;
    FDDictionaryEntryAVP m_avp_eps_location_information;
    FDDictionaryEntryAVP m_avp_drmp;
    FDDictionaryEntryAVP m_avp_trace_ne_type_list;
    FDDictionaryEntryAVP m_avp_adjacent_plmns;
    FDDictionaryEntryAVP m_avp_priority_level;
    FDDictionaryEntryAVP m_avp_pre_emption_vulnerability;
    FDDictionaryEntryAVP m_avp_geodetic_information;
    FDDictionaryEntryAVP m_avp_load_type;
    FDDictionaryEntryAVP m_avp_area_scope;
    FDDictionaryEntryAVP m_avp_preferred_data_mode;
    FDDictionaryEntryAVP m_avp_imei;
    FDDictionaryEntryAVP m_avp_reachability_information;
    FDDictionaryEntryAVP m_avp_node_type;
    FDDictionaryEntryAVP m_avp_clr_flags;
    FDDictionaryEntryAVP m_avp_call_barring_info;
    FDDictionaryEntryAVP m_avp_service_report;
    FDDictionaryEntryAVP m_avp_destination_host;
    FDDictionaryEntryAVP m_avp_gmlc_address;
    FDDictionaryEntryAVP m_avp_location_area_identity;
    FDDictionaryEntryAVP m_avp_logging_interval;
    FDDictionaryEntryAVP m_avp_roaming_restricted_due_to_unsupported_feature;
    FDDictionaryEntryAVP m_avp_rat_type;
    FDDictionaryEntryAVP m_avp_edrx_cycle_length_value;
    FDDictionaryEntryAVP m_avp_time_zone;
    FDDictionaryEntryAVP m_avp_msisdn;
    FDDictionaryEntryAVP m_avp_mo_lr;
    FDDictionaryEntryAVP m_avp_active_apn;
    FDDictionaryEntryAVP m_avp_job_type;
    FDDictionaryEntryAVP m_avp_ss_code;
    FDDictionaryEntryAVP m_avp_time_of_day_start;
    FDDictionaryEntryAVP m_avp_wlan_offloadability;
    FDDictionaryEntryAVP m_avp_mip_home_agent_host;
    FDDictionaryEntryAVP m_avp_pdn_type;
    FDDictionaryEntryAVP m_avp_communication_pattern_set;
    FDDictionaryEntryAVP m_avp_periodic_communication_indicator;
    FDDictionaryEntryAVP m_avp_prose_permission;
    FDDictionaryEntryAVP m_avp_equivalent_plmn_list;
    FDDictionaryEntryAVP m_avp_charged_party;
    FDDictionaryEntryAVP m_avp_supported_services;
    FDDictionaryEntryAVP m_avp_3gpp_charging_characteristics;
    FDDictionaryEntryAVP m_avp_proxy_info;
    FDDictionaryEntryAVP m_avp_subscribed_periodic_rau_tau_timer;
    FDDictionaryEntryAVP m_avp_ss_status;
    FDDictionaryEntryAVP m_avp_enodeb_id;
    FDDictionaryEntryAVP m_avp_utran_vector;
    FDDictionaryEntryAVP m_avp_origin_realm;
    FDDictionaryEntryAVP m_avp_origin_host;
    FDDictionaryEntryAVP m_avp_event_threshold_rsrq;
    FDDictionaryEntryAVP m_avp_vplmn_dynamic_address_allowed;
    FDDictionaryEntryAVP m_avp_accuracy;
    FDDictionaryEntryAVP m_avp_pur_flags;
    FDDictionaryEntryAVP m_avp_ue_reachability_configuration;
    FDDictionaryEntryAVP m_avp_apn_configuration;
    FDDictionaryEntryAVP m_avp_a_msisdn;
    FDDictionaryEntryAVP m_avp_monte_location_type;
    FDDictionaryEntryAVP m_avp_last_ue_activity_time;
    FDDictionaryEntryAVP m_avp_local_time_zone;
    FDDictionaryEntryAVP m_avp_authentication_info;
    FDDictionaryEntryAVP m_avp_service_type;
    FDDictionaryEntryAVP m_avp_v2x_permission;
    FDDictionaryEntryAVP m_avp_pdp_address;
    FDDictionaryEntryAVP m_avp_imsi_group_id;
    FDDictionaryEntryAVP m_avp_dsr_flags;
    FDDictionaryEntryAVP m_avp_complete_data_list_included_indicator;
    FDDictionaryEntryAVP m_avp_trace_collection_entity;
    FDDictionaryEntryAVP m_avp_wlan_offloadability_eutran;
    FDDictionaryEntryAVP m_avp_route_record;
    FDDictionaryEntryAVP m_avp_subscription_data;
    FDDictionaryEntryAVP m_avp_reference_id_validity_time;
    FDDictionaryEntryAVP m_avp_dl_buffering_suggested_packet_count;
    FDDictionaryEntryAVP m_avp_scef_id;
    FDDictionaryEntryAVP m_avp_trace_event_list;
    FDDictionaryEntryAVP m_avp_csg_id;
    FDDictionaryEntryAVP m_avp_mdt_configuration;
    FDDictionaryEntryAVP m_avp_error_diagnostic;
    FDDictionaryEntryAVP m_avp_routing_area_identity;
    FDDictionaryEntryAVP m_avp_current_location_retrieved;
    FDDictionaryEntryAVP m_avp_user_csg_information;
    FDDictionaryEntryAVP m_avp_mip6_agent_info;
    FDDictionaryEntryAVP m_avp_logging_duration;
    FDDictionaryEntryAVP m_avp_group_plmn_id;
    FDDictionaryEntryAVP m_avp_collection_period_rrm_umts;
    FDDictionaryEntryAVP m_avp_ida_flags;
    FDDictionaryEntryAVP m_avp_non_ip_pdn_type_indicator;
    FDDictionaryEntryAVP m_avp_tracking_area_identity;
    FDDictionaryEntryAVP m_avp_oc_olr;
    FDDictionaryEntryAVP m_avp_client_identity;
    FDDictionaryEntryAVP m_avp_age_of_location_information;
    FDDictionaryEntryAVP m_avp_trace_data;
    FDDictionaryEntryAVP m_avp_subscribed_vsrvcc;
    FDDictionaryEntryAVP m_avp_plmn_client;
    FDDictionaryEntryAVP m_avp_vplmn_lipa_allowed;
    FDDictionaryEntryAVP m_avp_re_synchronization_info;
    FDDictionaryEntryAVP m_avp_air_flags;
    FDDictionaryEntryAVP m_avp_specific_apn_info;
    FDDictionaryEntryAVP m_avp_rand;
    FDDictionaryEntryAVP m_avp_oc_reduction_percentage;
    FDDictionaryEntryAVP m_avp_report_interval;
    FDDictionaryEntryAVP m_avp_eps_subscribed_qos_profile;
    FDDictionaryEntryAVP m_avp_service_result_code;
    FDDictionaryEntryAVP m_avp_omc_id;
    FDDictionaryEntryAVP m_avp_oc_sequence_number;
    FDDictionaryEntryAVP m_avp_teleservice_list;
    FDDictionaryEntryAVP m_avp_apn_configuration_profile;
    FDDictionaryEntryAVP m_avp_event_threshold_event_1i;
    FDDictionaryEntryAVP m_avp_supported_monitoring_events;
    FDDictionaryEntryAVP m_avp_dsa_flags;
    FDDictionaryEntryAVP m_avp_event_threshold_event_1f;
    FDDictionaryEntryAVP m_avp_allocation_retention_priority;
    FDDictionaryEntryAVP m_avp_cell_global_identity;
    FDDictionaryEntryAVP m_avp_oc_validity_duration;
    FDDictionaryEntryAVP m_avp_ula_flags;
    FDDictionaryEntryAVP m_avp_csg_membership_indication;
    FDDictionaryEntryAVP m_avp_service_selection;
    FDDictionaryEntryAVP m_avp_network_access_mode;
    FDDictionaryEntryAVP m_avp_3gpp2_meid;
    FDDictionaryEntryAVP m_avp_mme_location_information;
    FDDictionaryEntryAVP m_avp_sgsn_location_information;
    FDDictionaryEntryAVP m_avp_ulr_flags;
    FDDictionaryEntryAVP m_avp_measurement_period_lte;
    FDDictionaryEntryAVP m_avp_result_code;
    FDDictionaryEntryAVP m_avp_gmlc_restriction;
    FDDictionaryEntryAVP m_avp_user_state;
    FDDictionaryEntryAVP m_avp_sgs_mme_identity;
    FDDictionaryEntryAVP m_avp_coupled_node_diameter_id;
    FDDictionaryEntryAVP m_avp_kc;
    FDDictionaryEntryAVP m_avp_wlan_offloadability_utran;
    FDDictionaryEntryAVP m_avp_servicetypeidentity;
    FDDictionaryEntryAVP m_avp_stn_sr;
    FDDictionaryEntryAVP m_avp_mme_number_for_mt_sms;
    FDDictionaryEntryAVP m_avp_requested_utran_geran_authentication_info;
    FDDictionaryEntryAVP m_avp_notification_to_ue_user;
    FDDictionaryEntryAVP m_avp_homogeneous_support_of_ims_voice_over_ps_sessions;
    FDDictionaryEntryAVP m_avp_pdp_context;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_dl;
    FDDictionaryEntryAVP m_avp_trace_interface_list;
    FDDictionaryEntryAVP m_avp_pdn_connection_continuity;
    FDDictionaryEntryAVP m_avp_scef_reference_id;
    FDDictionaryEntryAVP m_avp_sgsn_number;
    FDDictionaryEntryAVP m_avp_rat_frequency_selection_priority_id;
    FDDictionaryEntryAVP m_avp_number_of_requested_vectors;
    FDDictionaryEntryAVP m_avp_subscription_data_deletion;
    FDDictionaryEntryAVP m_avp_load;
    FDDictionaryEntryAVP m_avp_context_identifier;
    FDDictionaryEntryAVP m_avp_integrity_key;
    FDDictionaryEntryAVP m_avp_confidentiality_key;
    FDDictionaryEntryAVP m_avp_e_utran_cell_global_identity;
    FDDictionaryEntryAVP m_avp_oc_supported_features;
    FDDictionaryEntryAVP m_avp_monitoring_type;
    FDDictionaryEntryAVP m_avp_ue_pc5_ambr;
    FDDictionaryEntryAVP m_avp_proxy_host;
    FDDictionaryEntryAVP m_avp_event_threshold_rsrp;
    FDDictionaryEntryAVP m_avp_terminal_information;
    FDDictionaryEntryAVP m_avp_xres;
    FDDictionaryEntryAVP m_avp_subscriber_status;
    FDDictionaryEntryAVP m_avp_experimental_result_code;
    FDDictionaryEntryAVP m_avp_collection_period_rrm_lte;
    FDDictionaryEntryAVP m_avp_maximum_number_of_reports;
    FDDictionaryEntryAVP m_avp_hplmn_odb;
    FDDictionaryEntryAVP m_avp_mme_user_state;
    FDDictionaryEntryAVP m_avp_auth_application_id;
    FDDictionaryEntryAVP m_avp_reset_id;
    FDDictionaryEntryAVP m_avp_lipa_permission;
    FDDictionaryEntryAVP m_avp_requested_eutran_authentication_info;
    FDDictionaryEntryAVP m_avp_mps_priority;
    FDDictionaryEntryAVP m_avp_monitoring_event_report;
    FDDictionaryEntryAVP m_avp_extended_enodeb_id;
    FDDictionaryEntryAVP m_avp_idr_flags;
    FDDictionaryEntryAVP m_avp_aese_communication_pattern;
    FDDictionaryEntryAVP m_avp_visited_network_identifier;
    FDDictionaryEntryAVP m_avp_lcs_info;
    FDDictionaryEntryAVP m_avp_restoration_priority;
    FDDictionaryEntryAVP m_avp_mdt_user_consent;
    FDDictionaryEntryAVP m_avp_cancellation_type;
    FDDictionaryEntryAVP m_avp_stationary_indication;
    FDDictionaryEntryAVP m_avp_pdn_gw_allocation_type;
    FDDictionaryEntryAVP m_avp_gmlc_number;
    FDDictionaryEntryAVP m_avp_subscription_data_flags;
    FDDictionaryEntryAVP m_avp_ics_indicator;
    FDDictionaryEntryAVP m_avp_visited_plmn_id;
    FDDictionaryEntryAVP m_avp_qos_subscribed;
    FDDictionaryEntryAVP m_avp_positioning_method;
    FDDictionaryEntryAVP m_avp_monitoring_duration;
    FDDictionaryEntryAVP m_avp_reporting_trigger;
    FDDictionaryEntryAVP m_avp_failed_avp;
    FDDictionaryEntryAVP m_avp_ambr;
    FDDictionaryEntryAVP m_avp_daylight_saving_time;
    FDDictionaryEntryAVP m_avp_expiration_date;
    FDDictionaryEntryAVP m_avp_group_service_id;
    FDDictionaryEntryAVP m_avp_trace_reference;
    FDDictionaryEntryAVP m_avp_local_group_id;
    FDDictionaryEntryAVP m_avp_measurement_quantity;
    FDDictionaryEntryAVP m_avp_geographical_information;
    FDDictionaryEntryAVP m_avp_kasme;
    FDDictionaryEntryAVP m_avp_served_party_ip_address;
    FDDictionaryEntryAVP m_avp_trace_depth;
    FDDictionaryEntryAVP m_avp_scheduled_communication_time;
    FDDictionaryEntryAVP m_avp_proxy_state;
    FDDictionaryEntryAVP m_avp_operator_determined_barring;
    FDDictionaryEntryAVP m_avp_autn;
    FDDictionaryEntryAVP m_avp_pdp_type;
    FDDictionaryEntryAVP m_avp_periodic_time;
    FDDictionaryEntryAVP m_avp_time_of_day_end;
    FDDictionaryEntryAVP m_avp_software_version;
    FDDictionaryEntryAVP m_avp_sipto_local_network_permission;
    FDDictionaryEntryAVP m_avp_sms_register_request;
    FDDictionaryEntryAVP m_avp_sgsn_user_state;
    FDDictionaryEntryAVP m_avp_vendor_id;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_ul;
    FDDictionaryEntryAVP m_avp_supported_features;
    FDDictionaryEntryAVP m_avp_non_ip_data_delivery_mechanism;
    FDDictionaryEntryAVP m_avp_monitoring_event_configuration;
    FDDictionaryEntryAVP m_avp_destination_realm;
    FDDictionaryEntryAVP m_avp_session_id;
    FDDictionaryEntryAVP m_avp_immediate_response_preferred;
    FDDictionaryEntryAVP m_avp_user_name;
    FDDictionaryEntryAVP m_avp_scef_realm;
    FDDictionaryEntryAVP m_avp_sipto_permission;
    FDDictionaryEntryAVP m_avp_emergency_info;
    FDDictionaryEntryAVP m_avp_mdt_allowed_plmn_id;
    FDDictionaryEntryAVP m_avp_report_amount;
    FDDictionaryEntryAVP m_avp_csg_subscription_data;
    FDDictionaryEntryAVP m_avp_external_client;
    FDDictionaryEntryAVP m_avp_mip_home_agent_address;
    FDDictionaryEntryAVP m_avp_ue_srvcc_capability;
    FDDictionaryEntryAVP m_avp_measurement_period_umts;
    FDDictionaryEntryAVP m_avp_scef_reference_id_for_deletion;
    FDDictionaryEntryAVP m_avp_prose_subscription_data;
    FDDictionaryEntryAVP m_avp_service_result;
    FDDictionaryEntryAVP m_avp_location_information_configuration;
    FDDictionaryEntryAVP m_avp_ext_pdp_address;
    FDDictionaryEntryAVP m_avp_regional_subscription_zone_code;

};

//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - UpdateLocationRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// UPLR request from Clients.
//-
class UPLRcmd : public FDCommandRequest
{
public:
    UPLRcmd( Application &app );
    ~UPLRcmd();

    virtual int process( FDMessageRequest *req );

private:
    UPLRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - UpdateLocationRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a UPLR
// to the Diameter application and process the corresponding answer.
//
class UPLRreq : public FDMessageRequest
{
public:
    UPLRreq( Application &app );
    virtual ~UPLRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    UPLRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - CancelLocationRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// CALR request from Clients.
//-
class CALRcmd : public FDCommandRequest
{
public:
    CALRcmd( Application &app );
    ~CALRcmd();

    virtual int process( FDMessageRequest *req );

private:
    CALRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - CancelLocationRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a CALR
// to the Diameter application and process the corresponding answer.
//
class CALRreq : public FDMessageRequest
{
public:
    CALRreq( Application &app );
    virtual ~CALRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    CALRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - AuthenticationInformationRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// AUIR request from Clients.
//-
class AUIRcmd : public FDCommandRequest
{
public:
    AUIRcmd( Application &app );
    ~AUIRcmd();

    virtual int process( FDMessageRequest *req );

private:
    AUIRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - AuthenticationInformationRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a AUIR
// to the Diameter application and process the corresponding answer.
//
class AUIRreq : public FDMessageRequest
{
public:
    AUIRreq( Application &app );
    virtual ~AUIRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    AUIRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - InsertSubscriberDataRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// INSDR request from Clients.
//-
class INSDRcmd : public FDCommandRequest
{
public:
    INSDRcmd( Application &app );
    ~INSDRcmd();

    virtual int process( FDMessageRequest *req );

private:
    INSDRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - InsertSubscriberDataRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a INSDR
// to the Diameter application and process the corresponding answer.
//
class INSDRreq : public FDMessageRequest
{
public:
    INSDRreq( Application &app );
    virtual ~INSDRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    INSDRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - DeleteSubscriberDataRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// DESDR request from Clients.
//-
class DESDRcmd : public FDCommandRequest
{
public:
    DESDRcmd( Application &app );
    ~DESDRcmd();

    virtual int process( FDMessageRequest *req );

private:
    DESDRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - DeleteSubscriberDataRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a DESDR
// to the Diameter application and process the corresponding answer.
//
class DESDRreq : public FDMessageRequest
{
public:
    DESDRreq( Application &app );
    virtual ~DESDRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    DESDRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - PurgeUeRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// PUUR request from Clients.
//-
class PUURcmd : public FDCommandRequest
{
public:
    PUURcmd( Application &app );
    ~PUURcmd();

    virtual int process( FDMessageRequest *req );

private:
    PUURcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - PurgeUeRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a PUUR
// to the Diameter application and process the corresponding answer.
//
class PUURreq : public FDMessageRequest
{
public:
    PUURreq( Application &app );
    virtual ~PUURreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    PUURreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - ResetRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// RER request from Clients.
//-
class RERcmd : public FDCommandRequest
{
public:
    RERcmd( Application &app );
    ~RERcmd();

    virtual int process( FDMessageRequest *req );

private:
    RERcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - ResetRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a RER
// to the Diameter application and process the corresponding answer.
//
class RERreq : public FDMessageRequest
{
public:
    RERreq( Application &app );
    virtual ~RERreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    RERreq();
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
class MmeUserStateExtractor : public FDExtractor
{
 public:
    MmeUserStateExtractor( FDExtractor &parent, Dictionary &dict );
    ~MmeUserStateExtractor();

    FDExtractorAvp user_state;
};

class SgsnUserStateExtractor : public FDExtractor
{
 public:
    SgsnUserStateExtractor( FDExtractor &parent, Dictionary &dict );
    ~SgsnUserStateExtractor();

    FDExtractorAvp user_state;
};

class EpsUserStateExtractor : public FDExtractor
{
 public:
    EpsUserStateExtractor( FDExtractor &parent, Dictionary &dict );
    ~EpsUserStateExtractor();

    MmeUserStateExtractor mme_user_state;
    SgsnUserStateExtractor sgsn_user_state;
};

class EUtranVectorExtractor : public FDExtractor
{
 public:
    EUtranVectorExtractor( FDExtractor &parent, Dictionary &dict );
    ~EUtranVectorExtractor();

    FDExtractorAvp item_number;
    FDExtractorAvp rand;
    FDExtractorAvp xres;
    FDExtractorAvp autn;
    FDExtractorAvp kasme;
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

class AmbrExtractor : public FDExtractor
{
 public:
    AmbrExtractor( FDExtractor &parent, Dictionary &dict );
    ~AmbrExtractor();

    FDExtractorAvp max_requested_bandwidth_ul;
    FDExtractorAvp max_requested_bandwidth_dl;
};

class PdpContextExtractor : public FDExtractor
{
 public:
    PdpContextExtractor( FDExtractor &parent, Dictionary &dict );
    ~PdpContextExtractor();

    FDExtractorAvp context_identifier;
    FDExtractorAvp pdp_type;
    FDExtractorAvp pdp_address;
    FDExtractorAvp qos_subscribed;
    FDExtractorAvp vplmn_dynamic_address_allowed;
    FDExtractorAvp service_selection;
    FDExtractorAvp tgpp_charging_characteristics;
    FDExtractorAvp ext_pdp_type;
    FDExtractorAvp ext_pdp_address;
    AmbrExtractor ambr;
    FDExtractorAvp apn_oi_replacement;
    FDExtractorAvp sipto_permission;
    FDExtractorAvp lipa_permission;
    FDExtractorAvp restoration_priority;
    FDExtractorAvp sipto_local_network_permission;
    FDExtractorAvp non_ip_data_delivery_mechanism;
    FDExtractorAvp scef_id;
};

class PdpContextExtractorList : public FDExtractorList
{
 public:
    PdpContextExtractorList( FDExtractor &parent, Dictionary &dict );
    ~PdpContextExtractorList();

    FDExtractor *createExtractor() { return new PdpContextExtractor( getParent(), m_dict ); }
    std::list<PdpContextExtractor*> &getList() { return (std::list<PdpContextExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class GprsSubscriptionDataExtractor : public FDExtractor
{
 public:
    GprsSubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~GprsSubscriptionDataExtractor();

    FDExtractorAvp complete_data_list_included_indicator;
    PdpContextExtractorList pdp_context;
};

class ServiceResultExtractor : public FDExtractor
{
 public:
    ServiceResultExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServiceResultExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp service_result_code;
};

class ServiceReportExtractor : public FDExtractor
{
 public:
    ServiceReportExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServiceReportExtractor();

    ServiceResultExtractor service_result;
    FDExtractorAvp node_type;
};

class ServiceReportExtractorList : public FDExtractorList
{
 public:
    ServiceReportExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ServiceReportExtractorList();

    FDExtractor *createExtractor() { return new ServiceReportExtractor( getParent(), m_dict ); }
    std::list<ServiceReportExtractor*> &getList() { return (std::list<ServiceReportExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class MonitoringEventConfigStatusExtractor : public FDExtractor
{
 public:
    MonitoringEventConfigStatusExtractor( FDExtractor &parent, Dictionary &dict );
    ~MonitoringEventConfigStatusExtractor();

    ServiceReportExtractorList service_report;
    FDExtractorAvp scef_reference_id;
    FDExtractorAvp scef_id;
};

class ExperimentalResultExtractor : public FDExtractor
{
 public:
    ExperimentalResultExtractor( FDExtractor &parent, Dictionary &dict );
    ~ExperimentalResultExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp experimental_result_code;
};

class EdrxCycleLengthExtractor : public FDExtractor
{
 public:
    EdrxCycleLengthExtractor( FDExtractor &parent, Dictionary &dict );
    ~EdrxCycleLengthExtractor();

    FDExtractorAvp rat_type;
    FDExtractorAvp edrx_cycle_length_value;
};

class ExternalClientExtractor : public FDExtractor
{
 public:
    ExternalClientExtractor( FDExtractor &parent, Dictionary &dict );
    ~ExternalClientExtractor();

    FDExtractorAvp client_identity;
    FDExtractorAvp gmlc_restriction;
    FDExtractorAvp notification_to_ue_user;
};

class ExternalClientExtractorList : public FDExtractorList
{
 public:
    ExternalClientExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ExternalClientExtractorList();

    FDExtractor *createExtractor() { return new ExternalClientExtractor( getParent(), m_dict ); }
    std::list<ExternalClientExtractor*> &getList() { return (std::list<ExternalClientExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ServiceTypeExtractor : public FDExtractor
{
 public:
    ServiceTypeExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServiceTypeExtractor();

    FDExtractorAvp servicetypeidentity;
    FDExtractorAvp gmlc_restriction;
    FDExtractorAvp notification_to_ue_user;
};

class ServiceTypeExtractorList : public FDExtractorList
{
 public:
    ServiceTypeExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ServiceTypeExtractorList();

    FDExtractor *createExtractor() { return new ServiceTypeExtractor( getParent(), m_dict ); }
    std::list<ServiceTypeExtractor*> &getList() { return (std::list<ServiceTypeExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class LcsPrivacyexceptionExtractor : public FDExtractor
{
 public:
    LcsPrivacyexceptionExtractor( FDExtractor &parent, Dictionary &dict );
    ~LcsPrivacyexceptionExtractor();

    FDExtractorAvp ss_code;
    FDExtractorAvp ss_status;
    FDExtractorAvp notification_to_ue_user;
    ExternalClientExtractorList external_client;
    FDExtractorAvpList plmn_client;
    ServiceTypeExtractorList service_type;
};

class V2xSubscriptionDataExtractor : public FDExtractor
{
 public:
    V2xSubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~V2xSubscriptionDataExtractor();

    FDExtractorAvp v2x_permission;
    FDExtractorAvp ue_pc5_ambr;
};

class AdjacentAccessRestrictionDataExtractor : public FDExtractor
{
 public:
    AdjacentAccessRestrictionDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~AdjacentAccessRestrictionDataExtractor();

    FDExtractorAvp visited_plmn_id;
    FDExtractorAvp access_restriction_data;
};

class GeranVectorExtractor : public FDExtractor
{
 public:
    GeranVectorExtractor( FDExtractor &parent, Dictionary &dict );
    ~GeranVectorExtractor();

    FDExtractorAvp item_number;
    FDExtractorAvp rand;
    FDExtractorAvp sres;
    FDExtractorAvp kc;
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

class MmeLocationInformationExtractor : public FDExtractor
{
 public:
    MmeLocationInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~MmeLocationInformationExtractor();

    FDExtractorAvp e_utran_cell_global_identity;
    FDExtractorAvp tracking_area_identity;
    FDExtractorAvp geographical_information;
    FDExtractorAvp geodetic_information;
    FDExtractorAvp current_location_retrieved;
    FDExtractorAvp age_of_location_information;
    UserCsgInformationExtractor user_csg_information;
    FDExtractorAvp enodeb_id;
    FDExtractorAvp extended_enodeb_id;
};

class SgsnLocationInformationExtractor : public FDExtractor
{
 public:
    SgsnLocationInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~SgsnLocationInformationExtractor();

    FDExtractorAvp cell_global_identity;
    FDExtractorAvp location_area_identity;
    FDExtractorAvp service_area_identity;
    FDExtractorAvp routing_area_identity;
    FDExtractorAvp geographical_information;
    FDExtractorAvp geodetic_information;
    FDExtractorAvp current_location_retrieved;
    FDExtractorAvp age_of_location_information;
    UserCsgInformationExtractor user_csg_information;
};

class EpsLocationInformationExtractor : public FDExtractor
{
 public:
    EpsLocationInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~EpsLocationInformationExtractor();

    MmeLocationInformationExtractor mme_location_information;
    SgsnLocationInformationExtractor sgsn_location_information;
};

class AdjacentPlmnsExtractor : public FDExtractor
{
 public:
    AdjacentPlmnsExtractor( FDExtractor &parent, Dictionary &dict );
    ~AdjacentPlmnsExtractor();

    FDExtractorAvpList visited_plmn_id;
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

class CallBarringInfoExtractor : public FDExtractor
{
 public:
    CallBarringInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~CallBarringInfoExtractor();

    FDExtractorAvp ss_code;
    FDExtractorAvp ss_status;
};

class MoLrExtractor : public FDExtractor
{
 public:
    MoLrExtractor( FDExtractor &parent, Dictionary &dict );
    ~MoLrExtractor();

    FDExtractorAvp ss_code;
    FDExtractorAvp ss_status;
};

class Mip6AgentInfoExtractor : public FDExtractor
{
 public:
    Mip6AgentInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~Mip6AgentInfoExtractor();

    FDExtractorAvpList mip_home_agent_address;
    FDExtractorAvp mip_home_agent_host;
    FDExtractorAvp mip6_home_link_prefix;
};

class SpecificApnInfoExtractor : public FDExtractor
{
 public:
    SpecificApnInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~SpecificApnInfoExtractor();

    FDExtractorAvp service_selection;
    Mip6AgentInfoExtractor mip6_agent_info;
    FDExtractorAvp visited_network_identifier;
};

class SpecificApnInfoExtractorList : public FDExtractorList
{
 public:
    SpecificApnInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~SpecificApnInfoExtractorList();

    FDExtractor *createExtractor() { return new SpecificApnInfoExtractor( getParent(), m_dict ); }
    std::list<SpecificApnInfoExtractor*> &getList() { return (std::list<SpecificApnInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ActiveApnExtractor : public FDExtractor
{
 public:
    ActiveApnExtractor( FDExtractor &parent, Dictionary &dict );
    ~ActiveApnExtractor();

    FDExtractorAvp context_identifier;
    FDExtractorAvp service_selection;
    Mip6AgentInfoExtractor mip6_agent_info;
    FDExtractorAvp visited_network_identifier;
    SpecificApnInfoExtractorList specific_apn_info;
};

class WlanOffloadabilityExtractor : public FDExtractor
{
 public:
    WlanOffloadabilityExtractor( FDExtractor &parent, Dictionary &dict );
    ~WlanOffloadabilityExtractor();

    FDExtractorAvp wlan_offloadability_eutran;
    FDExtractorAvp wlan_offloadability_utran;
};

class ScheduledCommunicationTimeExtractor : public FDExtractor
{
 public:
    ScheduledCommunicationTimeExtractor( FDExtractor &parent, Dictionary &dict );
    ~ScheduledCommunicationTimeExtractor();

    FDExtractorAvp day_of_week_mask;
    FDExtractorAvp time_of_day_start;
    FDExtractorAvp time_of_day_end;
};

class ScheduledCommunicationTimeExtractorList : public FDExtractorList
{
 public:
    ScheduledCommunicationTimeExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ScheduledCommunicationTimeExtractorList();

    FDExtractor *createExtractor() { return new ScheduledCommunicationTimeExtractor( getParent(), m_dict ); }
    std::list<ScheduledCommunicationTimeExtractor*> &getList() { return (std::list<ScheduledCommunicationTimeExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class CommunicationPatternSetExtractor : public FDExtractor
{
 public:
    CommunicationPatternSetExtractor( FDExtractor &parent, Dictionary &dict );
    ~CommunicationPatternSetExtractor();

    FDExtractorAvp periodic_communication_indicator;
    FDExtractorAvp communication_duration_time;
    FDExtractorAvp periodic_time;
    ScheduledCommunicationTimeExtractorList scheduled_communication_time;
    FDExtractorAvp stationary_indication;
    FDExtractorAvp reference_id_validity_time;
};

class EquivalentPlmnListExtractor : public FDExtractor
{
 public:
    EquivalentPlmnListExtractor( FDExtractor &parent, Dictionary &dict );
    ~EquivalentPlmnListExtractor();

    FDExtractorAvpList visited_plmn_id;
};

class SupportedServicesExtractor : public FDExtractor
{
 public:
    SupportedServicesExtractor( FDExtractor &parent, Dictionary &dict );
    ~SupportedServicesExtractor();

    FDExtractorAvp supported_monitoring_events;
};

class ProxyInfoExtractor : public FDExtractor
{
 public:
    ProxyInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProxyInfoExtractor();

    FDExtractorAvp proxy_host;
    FDExtractorAvp proxy_state;
};

class UtranVectorExtractor : public FDExtractor
{
 public:
    UtranVectorExtractor( FDExtractor &parent, Dictionary &dict );
    ~UtranVectorExtractor();

    FDExtractorAvp item_number;
    FDExtractorAvp rand;
    FDExtractorAvp xres;
    FDExtractorAvp autn;
    FDExtractorAvp confidentiality_key;
    FDExtractorAvp integrity_key;
};

class UeReachabilityConfigurationExtractor : public FDExtractor
{
 public:
    UeReachabilityConfigurationExtractor( FDExtractor &parent, Dictionary &dict );
    ~UeReachabilityConfigurationExtractor();

    FDExtractorAvp reachability_type;
    FDExtractorAvp maximum_response_time;
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

class EpsSubscribedQosProfileExtractor : public FDExtractor
{
 public:
    EpsSubscribedQosProfileExtractor( FDExtractor &parent, Dictionary &dict );
    ~EpsSubscribedQosProfileExtractor();

    FDExtractorAvp qos_class_identifier;
    AllocationRetentionPriorityExtractor allocation_retention_priority;
};

class ApnConfigurationExtractor : public FDExtractor
{
 public:
    ApnConfigurationExtractor( FDExtractor &parent, Dictionary &dict );
    ~ApnConfigurationExtractor();

    FDExtractorAvp context_identifier;
    FDExtractorAvpList served_party_ip_address;
    FDExtractorAvp pdn_type;
    FDExtractorAvp service_selection;
    EpsSubscribedQosProfileExtractor eps_subscribed_qos_profile;
    FDExtractorAvp vplmn_dynamic_address_allowed;
    Mip6AgentInfoExtractor mip6_agent_info;
    FDExtractorAvp visited_network_identifier;
    FDExtractorAvp pdn_gw_allocation_type;
    FDExtractorAvp tgpp_charging_characteristics;
    AmbrExtractor ambr;
    SpecificApnInfoExtractorList specific_apn_info;
    FDExtractorAvp apn_oi_replacement;
    FDExtractorAvp sipto_permission;
    FDExtractorAvp lipa_permission;
    FDExtractorAvp restoration_priority;
    FDExtractorAvp sipto_local_network_permission;
    WlanOffloadabilityExtractor wlan_offloadability;
    FDExtractorAvp non_ip_pdn_type_indicator;
    FDExtractorAvp non_ip_data_delivery_mechanism;
    FDExtractorAvp scef_id;
    FDExtractorAvp scef_realm;
    FDExtractorAvp preferred_data_mode;
    FDExtractorAvp pdn_connection_continuity;
};

class LocalTimeZoneExtractor : public FDExtractor
{
 public:
    LocalTimeZoneExtractor( FDExtractor &parent, Dictionary &dict );
    ~LocalTimeZoneExtractor();

    FDExtractorAvp time_zone;
    FDExtractorAvp daylight_saving_time;
};

class EUtranVectorExtractorList : public FDExtractorList
{
 public:
    EUtranVectorExtractorList( FDExtractor &parent, Dictionary &dict );
    ~EUtranVectorExtractorList();

    FDExtractor *createExtractor() { return new EUtranVectorExtractor( getParent(), m_dict ); }
    std::list<EUtranVectorExtractor*> &getList() { return (std::list<EUtranVectorExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class UtranVectorExtractorList : public FDExtractorList
{
 public:
    UtranVectorExtractorList( FDExtractor &parent, Dictionary &dict );
    ~UtranVectorExtractorList();

    FDExtractor *createExtractor() { return new UtranVectorExtractor( getParent(), m_dict ); }
    std::list<UtranVectorExtractor*> &getList() { return (std::list<UtranVectorExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class GeranVectorExtractorList : public FDExtractorList
{
 public:
    GeranVectorExtractorList( FDExtractor &parent, Dictionary &dict );
    ~GeranVectorExtractorList();

    FDExtractor *createExtractor() { return new GeranVectorExtractor( getParent(), m_dict ); }
    std::list<GeranVectorExtractor*> &getList() { return (std::list<GeranVectorExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AuthenticationInfoExtractor : public FDExtractor
{
 public:
    AuthenticationInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~AuthenticationInfoExtractor();

    EUtranVectorExtractorList e_utran_vector;
    UtranVectorExtractorList utran_vector;
    GeranVectorExtractorList geran_vector;
};

class ImsiGroupIdExtractor : public FDExtractor
{
 public:
    ImsiGroupIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~ImsiGroupIdExtractor();

    FDExtractorAvp group_service_id;
    FDExtractorAvp group_plmn_id;
    FDExtractorAvp local_group_id;
};

class LcsPrivacyexceptionExtractorList : public FDExtractorList
{
 public:
    LcsPrivacyexceptionExtractorList( FDExtractor &parent, Dictionary &dict );
    ~LcsPrivacyexceptionExtractorList();

    FDExtractor *createExtractor() { return new LcsPrivacyexceptionExtractor( getParent(), m_dict ); }
    std::list<LcsPrivacyexceptionExtractor*> &getList() { return (std::list<LcsPrivacyexceptionExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class MoLrExtractorList : public FDExtractorList
{
 public:
    MoLrExtractorList( FDExtractor &parent, Dictionary &dict );
    ~MoLrExtractorList();

    FDExtractor *createExtractor() { return new MoLrExtractor( getParent(), m_dict ); }
    std::list<MoLrExtractor*> &getList() { return (std::list<MoLrExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class LcsInfoExtractor : public FDExtractor
{
 public:
    LcsInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~LcsInfoExtractor();

    FDExtractorAvpList gmlc_number;
    LcsPrivacyexceptionExtractorList lcs_privacyexception;
    MoLrExtractorList mo_lr;
};

class TeleserviceListExtractor : public FDExtractor
{
 public:
    TeleserviceListExtractor( FDExtractor &parent, Dictionary &dict );
    ~TeleserviceListExtractor();

    FDExtractorAvpList ts_code;
};

class CallBarringInfoExtractorList : public FDExtractorList
{
 public:
    CallBarringInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~CallBarringInfoExtractorList();

    FDExtractor *createExtractor() { return new CallBarringInfoExtractor( getParent(), m_dict ); }
    std::list<CallBarringInfoExtractor*> &getList() { return (std::list<CallBarringInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ApnConfigurationExtractorList : public FDExtractorList
{
 public:
    ApnConfigurationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ApnConfigurationExtractorList();

    FDExtractor *createExtractor() { return new ApnConfigurationExtractor( getParent(), m_dict ); }
    std::list<ApnConfigurationExtractor*> &getList() { return (std::list<ApnConfigurationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ApnConfigurationProfileExtractor : public FDExtractor
{
 public:
    ApnConfigurationProfileExtractor( FDExtractor &parent, Dictionary &dict );
    ~ApnConfigurationProfileExtractor();

    FDExtractorAvp context_identifier;
    FDExtractorAvp additional_context_identifier;
    FDExtractorAvp all_apn_configurations_included_indicator;
    ApnConfigurationExtractorList apn_configuration;
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

class CsgSubscriptionDataExtractor : public FDExtractor
{
 public:
    CsgSubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~CsgSubscriptionDataExtractor();

    FDExtractorAvp csg_id;
    FDExtractorAvp expiration_date;
    FDExtractorAvpList service_selection;
    FDExtractorAvp visited_plmn_id;
};

class CsgSubscriptionDataExtractorList : public FDExtractorList
{
 public:
    CsgSubscriptionDataExtractorList( FDExtractor &parent, Dictionary &dict );
    ~CsgSubscriptionDataExtractorList();

    FDExtractor *createExtractor() { return new CsgSubscriptionDataExtractor( getParent(), m_dict ); }
    std::list<CsgSubscriptionDataExtractor*> &getList() { return (std::list<CsgSubscriptionDataExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ProseSubscriptionDataExtractor : public FDExtractor
{
 public:
    ProseSubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProseSubscriptionDataExtractor();

    FDExtractorAvp prose_permission;
};

class AdjacentAccessRestrictionDataExtractorList : public FDExtractorList
{
 public:
    AdjacentAccessRestrictionDataExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AdjacentAccessRestrictionDataExtractorList();

    FDExtractor *createExtractor() { return new AdjacentAccessRestrictionDataExtractor( getParent(), m_dict ); }
    std::list<AdjacentAccessRestrictionDataExtractor*> &getList() { return (std::list<AdjacentAccessRestrictionDataExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ImsiGroupIdExtractorList : public FDExtractorList
{
 public:
    ImsiGroupIdExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ImsiGroupIdExtractorList();

    FDExtractor *createExtractor() { return new ImsiGroupIdExtractor( getParent(), m_dict ); }
    std::list<ImsiGroupIdExtractor*> &getList() { return (std::list<ImsiGroupIdExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class CommunicationPatternSetExtractorList : public FDExtractorList
{
 public:
    CommunicationPatternSetExtractorList( FDExtractor &parent, Dictionary &dict );
    ~CommunicationPatternSetExtractorList();

    FDExtractor *createExtractor() { return new CommunicationPatternSetExtractor( getParent(), m_dict ); }
    std::list<CommunicationPatternSetExtractor*> &getList() { return (std::list<CommunicationPatternSetExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AeseCommunicationPatternExtractor : public FDExtractor
{
 public:
    AeseCommunicationPatternExtractor( FDExtractor &parent, Dictionary &dict );
    ~AeseCommunicationPatternExtractor();

    FDExtractorAvp scef_reference_id;
    FDExtractorAvp scef_id;
    FDExtractorAvpList scef_reference_id_for_deletion;
    CommunicationPatternSetExtractorList communication_pattern_set;
};

class AeseCommunicationPatternExtractorList : public FDExtractorList
{
 public:
    AeseCommunicationPatternExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AeseCommunicationPatternExtractorList();

    FDExtractor *createExtractor() { return new AeseCommunicationPatternExtractor( getParent(), m_dict ); }
    std::list<AeseCommunicationPatternExtractor*> &getList() { return (std::list<AeseCommunicationPatternExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class LocationInformationConfigurationExtractor : public FDExtractor
{
 public:
    LocationInformationConfigurationExtractor( FDExtractor &parent, Dictionary &dict );
    ~LocationInformationConfigurationExtractor();

    FDExtractorAvp monte_location_type;
    FDExtractorAvp accuracy;
};

class MonitoringEventConfigurationExtractor : public FDExtractor
{
 public:
    MonitoringEventConfigurationExtractor( FDExtractor &parent, Dictionary &dict );
    ~MonitoringEventConfigurationExtractor();

    FDExtractorAvp scef_reference_id;
    FDExtractorAvp scef_id;
    FDExtractorAvp monitoring_type;
    FDExtractorAvpList scef_reference_id_for_deletion;
    FDExtractorAvp maximum_number_of_reports;
    FDExtractorAvp monitoring_duration;
    FDExtractorAvp charged_party;
    UeReachabilityConfigurationExtractor ue_reachability_configuration;
    LocationInformationConfigurationExtractor location_information_configuration;
    FDExtractorAvp scef_realm;
};

class MonitoringEventConfigurationExtractorList : public FDExtractorList
{
 public:
    MonitoringEventConfigurationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~MonitoringEventConfigurationExtractorList();

    FDExtractor *createExtractor() { return new MonitoringEventConfigurationExtractor( getParent(), m_dict ); }
    std::list<MonitoringEventConfigurationExtractor*> &getList() { return (std::list<MonitoringEventConfigurationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class EmergencyInfoExtractor : public FDExtractor
{
 public:
    EmergencyInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~EmergencyInfoExtractor();

    Mip6AgentInfoExtractor mip6_agent_info;
};

class EdrxCycleLengthExtractorList : public FDExtractorList
{
 public:
    EdrxCycleLengthExtractorList( FDExtractor &parent, Dictionary &dict );
    ~EdrxCycleLengthExtractorList();

    FDExtractor *createExtractor() { return new EdrxCycleLengthExtractor( getParent(), m_dict ); }
    std::list<EdrxCycleLengthExtractor*> &getList() { return (std::list<EdrxCycleLengthExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class SubscriptionDataExtractor : public FDExtractor
{
 public:
    SubscriptionDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~SubscriptionDataExtractor();

    FDExtractorAvp subscriber_status;
    FDExtractorAvp msisdn;
    FDExtractorAvp a_msisdn;
    FDExtractorAvp stn_sr;
    FDExtractorAvp ics_indicator;
    FDExtractorAvp network_access_mode;
    FDExtractorAvp operator_determined_barring;
    FDExtractorAvp hplmn_odb;
    FDExtractorAvpList regional_subscription_zone_code;
    FDExtractorAvp access_restriction_data;
    FDExtractorAvp apn_oi_replacement;
    LcsInfoExtractor lcs_info;
    TeleserviceListExtractor teleservice_list;
    CallBarringInfoExtractorList call_barring_info;
    FDExtractorAvp tgpp_charging_characteristics;
    AmbrExtractor ambr;
    ApnConfigurationProfileExtractor apn_configuration_profile;
    FDExtractorAvp rat_frequency_selection_priority_id;
    TraceDataExtractor trace_data;
    GprsSubscriptionDataExtractor gprs_subscription_data;
    CsgSubscriptionDataExtractorList csg_subscription_data;
    FDExtractorAvp roaming_restricted_due_to_unsupported_feature;
    FDExtractorAvp subscribed_periodic_rau_tau_timer;
    FDExtractorAvp mps_priority;
    FDExtractorAvp vplmn_lipa_allowed;
    FDExtractorAvp relay_node_indicator;
    FDExtractorAvp mdt_user_consent;
    FDExtractorAvp subscribed_vsrvcc;
    ProseSubscriptionDataExtractor prose_subscription_data;
    FDExtractorAvp subscription_data_flags;
    AdjacentAccessRestrictionDataExtractorList adjacent_access_restriction_data;
    FDExtractorAvp dl_buffering_suggested_packet_count;
    ImsiGroupIdExtractorList imsi_group_id;
    FDExtractorAvp ue_usage_type;
    AeseCommunicationPatternExtractorList aese_communication_pattern;
    MonitoringEventConfigurationExtractorList monitoring_event_configuration;
    EmergencyInfoExtractor emergency_info;
    V2xSubscriptionDataExtractor v2x_subscription_data;
    EdrxCycleLengthExtractorList edrx_cycle_length;
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

class RequestedUtranGeranAuthenticationInfoExtractor : public FDExtractor
{
 public:
    RequestedUtranGeranAuthenticationInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~RequestedUtranGeranAuthenticationInfoExtractor();

    FDExtractorAvp number_of_requested_vectors;
    FDExtractorAvp immediate_response_preferred;
    FDExtractorAvp re_synchronization_info;
};

class SubscriptionDataDeletionExtractor : public FDExtractor
{
 public:
    SubscriptionDataDeletionExtractor( FDExtractor &parent, Dictionary &dict );
    ~SubscriptionDataDeletionExtractor();

    FDExtractorAvp dsr_flags;
    FDExtractorAvp scef_id;
    FDExtractorAvpList context_identifier;
    FDExtractorAvp trace_reference;
    FDExtractorAvpList ts_code;
    FDExtractorAvpList ss_code;
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

class TerminalInformationExtractor : public FDExtractor
{
 public:
    TerminalInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~TerminalInformationExtractor();

    FDExtractorAvp imei;
    FDExtractorAvp tgpp2_meid;
    FDExtractorAvp software_version;
};

class RequestedEutranAuthenticationInfoExtractor : public FDExtractor
{
 public:
    RequestedEutranAuthenticationInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~RequestedEutranAuthenticationInfoExtractor();

    FDExtractorAvp number_of_requested_vectors;
    FDExtractorAvp immediate_response_preferred;
    FDExtractorAvp re_synchronization_info;
};

class MonitoringEventReportExtractor : public FDExtractor
{
 public:
    MonitoringEventReportExtractor( FDExtractor &parent, Dictionary &dict );
    ~MonitoringEventReportExtractor();

    FDExtractorAvp scef_reference_id;
    FDExtractorAvp scef_id;
    FDExtractorAvp reachability_information;
    EpsLocationInformationExtractor eps_location_information;
    FDExtractorAvp monitoring_type;
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

class ActiveApnExtractorList : public FDExtractorList
{
 public:
    ActiveApnExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ActiveApnExtractorList();

    FDExtractor *createExtractor() { return new ActiveApnExtractor( getParent(), m_dict ); }
    std::list<ActiveApnExtractor*> &getList() { return (std::list<ActiveApnExtractor*>&)FDExtractorList::getList(); }

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

class UpdateLocationRequestExtractor : public FDExtractor
{
 public:
    UpdateLocationRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~UpdateLocationRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    FDExtractorAvp user_name;
    OcSupportedFeaturesExtractor oc_supported_features;
    SupportedFeaturesExtractorList supported_features;
    TerminalInformationExtractor terminal_information;
    FDExtractorAvp rat_type;
    FDExtractorAvp ulr_flags;
    FDExtractorAvp ue_srvcc_capability;
    FDExtractorAvp visited_plmn_id;
    FDExtractorAvp sgsn_number;
    FDExtractorAvp homogeneous_support_of_ims_voice_over_ps_sessions;
    FDExtractorAvp gmlc_address;
    ActiveApnExtractorList active_apn;
    EquivalentPlmnListExtractor equivalent_plmn_list;
    FDExtractorAvp mme_number_for_mt_sms;
    FDExtractorAvp sms_register_request;
    FDExtractorAvp sgs_mme_identity;
    FDExtractorAvp coupled_node_diameter_id;
    AdjacentPlmnsExtractor adjacent_plmns;
    SupportedServicesExtractor supported_services;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
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

class UpdateLocationAnswerExtractor : public FDExtractor
{
 public:
    UpdateLocationAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~UpdateLocationAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp error_diagnostic;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    LoadExtractorList load;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp ula_flags;
    SubscriptionDataExtractor subscription_data;
    FDExtractorAvpList reset_id;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class CancelLocationRequestExtractor : public FDExtractor
{
 public:
    CancelLocationRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~CancelLocationRequestExtractor();

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
    FDExtractorAvp cancellation_type;
    FDExtractorAvp clr_flags;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class CancelLocationAnswerExtractor : public FDExtractor
{
 public:
    CancelLocationAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~CancelLocationAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class AuthenticationInformationRequestExtractor : public FDExtractor
{
 public:
    AuthenticationInformationRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~AuthenticationInformationRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    FDExtractorAvp user_name;
    OcSupportedFeaturesExtractor oc_supported_features;
    SupportedFeaturesExtractorList supported_features;
    RequestedEutranAuthenticationInfoExtractor requested_eutran_authentication_info;
    RequestedUtranGeranAuthenticationInfoExtractor requested_utran_geran_authentication_info;
    FDExtractorAvp visited_plmn_id;
    FDExtractorAvp air_flags;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class AuthenticationInformationAnswerExtractor : public FDExtractor
{
 public:
    AuthenticationInformationAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~AuthenticationInformationAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp error_diagnostic;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    LoadExtractorList load;
    SupportedFeaturesExtractorList supported_features;
    AuthenticationInfoExtractor authentication_info;
    FDExtractorAvp ue_usage_type;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class InsertSubscriberDataRequestExtractor : public FDExtractor
{
 public:
    InsertSubscriberDataRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~InsertSubscriberDataRequestExtractor();

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
    SubscriptionDataExtractor subscription_data;
    FDExtractorAvp idr_flags;
    FDExtractorAvpList reset_id;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class MonitoringEventReportExtractorList : public FDExtractorList
{
 public:
    MonitoringEventReportExtractorList( FDExtractor &parent, Dictionary &dict );
    ~MonitoringEventReportExtractorList();

    FDExtractor *createExtractor() { return new MonitoringEventReportExtractor( getParent(), m_dict ); }
    std::list<MonitoringEventReportExtractor*> &getList() { return (std::list<MonitoringEventReportExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class MonitoringEventConfigStatusExtractorList : public FDExtractorList
{
 public:
    MonitoringEventConfigStatusExtractorList( FDExtractor &parent, Dictionary &dict );
    ~MonitoringEventConfigStatusExtractorList();

    FDExtractor *createExtractor() { return new MonitoringEventConfigStatusExtractor( getParent(), m_dict ); }
    std::list<MonitoringEventConfigStatusExtractor*> &getList() { return (std::list<MonitoringEventConfigStatusExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class InsertSubscriberDataAnswerExtractor : public FDExtractor
{
 public:
    InsertSubscriberDataAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~InsertSubscriberDataAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp ims_voice_over_ps_sessions_supported;
    FDExtractorAvp last_ue_activity_time;
    FDExtractorAvp rat_type;
    FDExtractorAvp ida_flags;
    EpsUserStateExtractor eps_user_state;
    EpsLocationInformationExtractor eps_location_information;
    LocalTimeZoneExtractor local_time_zone;
    SupportedServicesExtractor supported_services;
    MonitoringEventReportExtractorList monitoring_event_report;
    MonitoringEventConfigStatusExtractorList monitoring_event_config_status;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class DeleteSubscriberDataRequestExtractor : public FDExtractor
{
 public:
    DeleteSubscriberDataRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~DeleteSubscriberDataRequestExtractor();

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
    FDExtractorAvp dsr_flags;
    FDExtractorAvp scef_id;
    FDExtractorAvpList context_identifier;
    FDExtractorAvp trace_reference;
    FDExtractorAvpList ts_code;
    FDExtractorAvpList ss_code;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class DeleteSubscriberDataAnswerExtractor : public FDExtractor
{
 public:
    DeleteSubscriberDataAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~DeleteSubscriberDataAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp dsa_flags;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class PurgeUeRequestExtractor : public FDExtractor
{
 public:
    PurgeUeRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~PurgeUeRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    FDExtractorAvp user_name;
    OcSupportedFeaturesExtractor oc_supported_features;
    FDExtractorAvp pur_flags;
    SupportedFeaturesExtractorList supported_features;
    EpsLocationInformationExtractor eps_location_information;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class PurgeUeAnswerExtractor : public FDExtractor
{
 public:
    PurgeUeAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~PurgeUeAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    LoadExtractorList load;
    FDExtractorAvp pua_flags;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class ResetRequestExtractor : public FDExtractor
{
 public:
    ResetRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~ResetRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvpList user_id;
    FDExtractorAvpList reset_id;
    SubscriptionDataExtractor subscription_data;
    SubscriptionDataDeletionExtractor subscription_data_deletion;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class ResetAnswerExtractor : public FDExtractor
{
 public:
    ResetAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~ResetAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};


}

#endif // __S6AS6D_H

