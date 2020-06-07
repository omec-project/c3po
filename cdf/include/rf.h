/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __RF_H
#define __RF_H

#include "fd.h"

namespace rf {

//
// Primary Objects
//

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    FDDictionaryEntryApplication &app()                                           { return m_app; }

    FDDictionaryEntryCommand &cmdACR()                                           	{ return m_cmd_acr; }
    FDDictionaryEntryCommand &cmdACA()                                           	{ return m_cmd_aca; }

    FDDictionaryEntryVendor &vnd3GPP2()                                          	{ return m_vnd_3gpp2; }
    FDDictionaryEntryVendor &vndETSI()                                           	{ return m_vnd_Etsi; }
    FDDictionaryEntryVendor &vnd3GPP()                                           	{ return m_vnd_3gpp; }

    FDDictionaryEntryAVP &avpLcsClientDialedByMs()                               	{ return m_avp_lcs_client_dialed_by_ms; }
    FDDictionaryEntryAVP &avpMmeRealm()                                          	{ return m_avp_mme_realm; }
    FDDictionaryEntryAVP &avp3gppGgsnMccMnc()                                    	{ return m_avp_3gpp_ggsn_mcc_mnc; }
    FDDictionaryEntryAVP &avpEventTimestamp()                                    	{ return m_avp_event_timestamp; }
    FDDictionaryEntryAVP &avpServiceIdentifier()                                 	{ return m_avp_service_identifier; }
    FDDictionaryEntryAVP &avpCsgAccessMode()                                     	{ return m_avp_csg_access_mode; }
    FDDictionaryEntryAVP &avpProseRangeClass()                                   	{ return m_avp_prose_range_class; }
    FDDictionaryEntryAVP &avpImsEmergencyIndicator()                             	{ return m_avp_ims_emergency_indicator; }
    FDDictionaryEntryAVP &avpOriginatorSccpAddress()                             	{ return m_avp_originator_sccp_address; }
    FDDictionaryEntryAVP &avpScaleFactor()                                       	{ return m_avp_scale_factor; }
    FDDictionaryEntryAVP &avpCnOperatorSelectionEntity()                         	{ return m_avp_cn_operator_selection_entity; }
    FDDictionaryEntryAVP &avpAcctMultiSessionId()                                	{ return m_avp_acct_multi_session_id; }
    FDDictionaryEntryAVP &avpInterUeTransfer()                                   	{ return m_avp_inter_ue_transfer; }
    FDDictionaryEntryAVP &avpRelatedImsChargingIdentifier()                      	{ return m_avp_related_ims_charging_identifier; }
    FDDictionaryEntryAVP &avpInitialImsChargingIdentifier()                      	{ return m_avp_initial_ims_charging_identifier; }
    FDDictionaryEntryAVP &avpCalledIdentity()                                    	{ return m_avp_called_identity; }
    FDDictionaryEntryAVP &avpSdpMediaComponent()                                 	{ return m_avp_sdp_media_component; }
    FDDictionaryEntryAVP &avpRadioParameterSetValues()                           	{ return m_avp_radio_parameter_set_values; }
    FDDictionaryEntryAVP &avpAocServiceType()                                    	{ return m_avp_aoc_service_type; }
    FDDictionaryEntryAVP &avpAcctApplicationId()                                 	{ return m_avp_acct_application_id; }
    FDDictionaryEntryAVP &avpDomainName()                                        	{ return m_avp_domain_name; }
    FDDictionaryEntryAVP &avpRouteHeaderReceived()                               	{ return m_avp_route_header_received; }
    FDDictionaryEntryAVP &avpUwanUserLocationInfo()                              	{ return m_avp_uwan_user_location_info; }
    FDDictionaryEntryAVP &avpPriorityLevel()                                     	{ return m_avp_priority_level; }
    FDDictionaryEntryAVP &avpOnlineChargingFlag()                                	{ return m_avp_online_charging_flag; }
    FDDictionaryEntryAVP &avpAocServiceObligatoryType()                          	{ return m_avp_aoc_service_obligatory_type; }
    FDDictionaryEntryAVP &avpMscAddress()                                        	{ return m_avp_msc_address; }
    FDDictionaryEntryAVP &avpAddressDomain()                                     	{ return m_avp_address_domain; }
    FDDictionaryEntryAVP &avpSgwAddress()                                        	{ return m_avp_sgw_address; }
    FDDictionaryEntryAVP &avpFromAddress()                                       	{ return m_avp_from_address; }
    FDDictionaryEntryAVP &avpServiceInformation()                                	{ return m_avp_service_information; }
    FDDictionaryEntryAVP &avpLcsClientType()                                     	{ return m_avp_lcs_client_type; }
    FDDictionaryEntryAVP &avpLcsClientId()                                       	{ return m_avp_lcs_client_id; }
    FDDictionaryEntryAVP &avpSdpMediaName()                                      	{ return m_avp_sdp_media_name; }
    FDDictionaryEntryAVP &avpSmSequenceNumber()                                  	{ return m_avp_sm_sequence_number; }
    FDDictionaryEntryAVP &avpTimeWindow()                                        	{ return m_avp_time_window; }
    FDDictionaryEntryAVP &avpTokenText()                                         	{ return m_avp_token_text; }
    FDDictionaryEntryAVP &avpTerminatingIoi()                                    	{ return m_avp_terminating_ioi; }
    FDDictionaryEntryAVP &avpPsAppendFreeFormatData()                            	{ return m_avp_ps_append_free_format_data; }
    FDDictionaryEntryAVP &avpSmsResult()                                         	{ return m_avp_sms_result; }
    FDDictionaryEntryAVP &avpGuaranteedBitrateDl()                               	{ return m_avp_guaranteed_bitrate_dl; }
    FDDictionaryEntryAVP &avpSubscriptionIdData()                                	{ return m_avp_subscription_id_data; }
    FDDictionaryEntryAVP &avpLocationEstimateType()                              	{ return m_avp_location_estimate_type; }
    FDDictionaryEntryAVP &avpAdcRuleBaseName()                                   	{ return m_avp_adc_rule_base_name; }
    FDDictionaryEntryAVP &avpLocationInfo()                                      	{ return m_avp_location_info; }
    FDDictionaryEntryAVP &avpMbmsInformation()                                   	{ return m_avp_mbms_information; }
    FDDictionaryEntryAVP &avpAocService()                                        	{ return m_avp_aoc_service; }
    FDDictionaryEntryAVP &avpCsgId()                                             	{ return m_avp_csg_id; }
    FDDictionaryEntryAVP &avpLocationEstimate()                                  	{ return m_avp_location_estimate; }
    FDDictionaryEntryAVP &avpMbmsChargedParty()                                  	{ return m_avp_mbms_charged_party; }
    FDDictionaryEntryAVP &avpProseReasonForCancellation()                        	{ return m_avp_prose_reason_for_cancellation; }
    FDDictionaryEntryAVP &avpPdpAddressPrefixLength()                            	{ return m_avp_pdp_address_prefix_length; }
    FDDictionaryEntryAVP &avpEpdgAddress()                                       	{ return m_avp_epdg_address; }
    FDDictionaryEntryAVP &avpTransitIoiList()                                    	{ return m_avp_transit_ioi_list; }
    FDDictionaryEntryAVP &avpAccessNetworkChargingIdentifierValue()              	{ return m_avp_access_network_charging_identifier_value; }
    FDDictionaryEntryAVP &avpCgAddress()                                         	{ return m_avp_cg_address; }
    FDDictionaryEntryAVP &avpProseDirectCommunicationTransmissionDataContainer() 	{ return m_avp_prose_direct_communication_transmission_data_container; }
    FDDictionaryEntryAVP &avpServiceSpecificData()                               	{ return m_avp_service_specific_data; }
    FDDictionaryEntryAVP &avpSipMethod()                                         	{ return m_avp_sip_method; }
    FDDictionaryEntryAVP &avpCsgMembershipIndication()                           	{ return m_avp_csg_membership_indication; }
    FDDictionaryEntryAVP &avpTimeLastUsage()                                     	{ return m_avp_time_last_usage; }
    FDDictionaryEntryAVP &avpAccountingOutputOctets()                            	{ return m_avp_accounting_output_octets; }
    FDDictionaryEntryAVP &avpSdpSessionDescription()                             	{ return m_avp_sdp_session_description; }
    FDDictionaryEntryAVP &avpSessionDirection()                                  	{ return m_avp_session_direction; }
    FDDictionaryEntryAVP &avpApnRateControlDownlink()                            	{ return m_avp_apn_rate_control_downlink; }
    FDDictionaryEntryAVP &avpCcOutputOctets()                                    	{ return m_avp_cc_output_octets; }
    FDDictionaryEntryAVP &avpCnIpMulticastDistribution()                         	{ return m_avp_cn_ip_multicast_distribution; }
    FDDictionaryEntryAVP &avpSmStatus()                                          	{ return m_avp_sm_status; }
    FDDictionaryEntryAVP &avpProseSourceIpAddress()                              	{ return m_avp_prose_source_ip_address; }
    FDDictionaryEntryAVP &avpProseDirectDiscoveryModel()                         	{ return m_avp_prose_direct_discovery_model; }
    FDDictionaryEntryAVP &avpRateControlMaxMessageSize()                         	{ return m_avp_rate_control_max_message_size; }
    FDDictionaryEntryAVP &avpBaseTimeInterval()                                  	{ return m_avp_base_time_interval; }
    FDDictionaryEntryAVP &avpPocInformation()                                    	{ return m_avp_poc_information; }
    FDDictionaryEntryAVP &avpRequiredMbmsBearerCapabilities()                    	{ return m_avp_required_mbms_bearer_capabilities; }
    FDDictionaryEntryAVP &avp3gppSelectionMode()                                 	{ return m_avp_3gpp_selection_mode; }
    FDDictionaryEntryAVP &avpSupplementaryService()                              	{ return m_avp_supplementary_service; }
    FDDictionaryEntryAVP &avpPc3ControlProtocolCause()                           	{ return m_avp_pc3_control_protocol_cause; }
    FDDictionaryEntryAVP &avpTerminalInformation()                               	{ return m_avp_terminal_information; }
    FDDictionaryEntryAVP &avpPdpAddress()                                        	{ return m_avp_pdp_address; }
    FDDictionaryEntryAVP &avpChargingCharacteristicsSelectionMode()              	{ return m_avp_charging_characteristics_selection_mode; }
    FDDictionaryEntryAVP &avpTransmitterInfo()                                   	{ return m_avp_transmitter_info; }
    FDDictionaryEntryAVP &avpTimeUsage()                                         	{ return m_avp_time_usage; }
    FDDictionaryEntryAVP &avpContentClass()                                      	{ return m_avp_content_class; }
    FDDictionaryEntryAVP &avpApplicId()                                          	{ return m_avp_applic_id; }
    FDDictionaryEntryAVP &avp3gpp2Bsid()                                         	{ return m_avp_3gpp2_bsid; }
    FDDictionaryEntryAVP &avpAocFormat()                                         	{ return m_avp_aoc_format; }
    FDDictionaryEntryAVP &avpUdpSourcePort()                                     	{ return m_avp_udp_source_port; }
    FDDictionaryEntryAVP &avpAdaptations()                                       	{ return m_avp_adaptations; }
    FDDictionaryEntryAVP &avpPositioningData()                                   	{ return m_avp_positioning_data; }
    FDDictionaryEntryAVP &avpErrorMessage()                                      	{ return m_avp_error_message; }
    FDDictionaryEntryAVP &avpImsiUnauthenticatedFlag()                           	{ return m_avp_imsi_unauthenticated_flag; }
    FDDictionaryEntryAVP &avpMandatoryCapability()                               	{ return m_avp_mandatory_capability; }
    FDDictionaryEntryAVP &avpRecipientSccpAddress()                              	{ return m_avp_recipient_sccp_address; }
    FDDictionaryEntryAVP &avpExpires()                                           	{ return m_avp_expires; }
    FDDictionaryEntryAVP &avpFailedAvp()                                         	{ return m_avp_failed_avp; }
    FDDictionaryEntryAVP &avpFlows()                                             	{ return m_avp_flows; }
    FDDictionaryEntryAVP &avpRequestedPlmnIdentifier()                           	{ return m_avp_requested_plmn_identifier; }
    FDDictionaryEntryAVP &avpMbmsUserServiceType()                               	{ return m_avp_mbms_user_service_type; }
    FDDictionaryEntryAVP &avpApplicationServer()                                 	{ return m_avp_application_server; }
    FDDictionaryEntryAVP &avpCcInputOctets()                                     	{ return m_avp_cc_input_octets; }
    FDDictionaryEntryAVP &avpCarrierSelectRoutingInformation()                   	{ return m_avp_carrier_select_routing_information; }
    FDDictionaryEntryAVP &avpTimeStamps()                                        	{ return m_avp_time_stamps; }
    FDDictionaryEntryAVP &avpStartTime()                                         	{ return m_avp_start_time; }
    FDDictionaryEntryAVP &avpAfChargingIdentifier()                              	{ return m_avp_af_charging_identifier; }
    FDDictionaryEntryAVP &avpServiceDataContainer()                              	{ return m_avp_service_data_container; }
    FDDictionaryEntryAVP &avpEarlyMediaDescription()                             	{ return m_avp_early_media_description; }
    FDDictionaryEntryAVP &avpGrantedServiceUnit()                                	{ return m_avp_granted_service_unit; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthUl()                           	{ return m_avp_max_requested_bandwidth_ul; }
    FDDictionaryEntryAVP &avpImei()                                              	{ return m_avp_imei; }
    FDDictionaryEntryAVP &avpMmsInformation()                                    	{ return m_avp_mms_information; }
    FDDictionaryEntryAVP &avpUserName()                                          	{ return m_avp_user_name; }
    FDDictionaryEntryAVP &avpBssid()                                             	{ return m_avp_bssid; }
    FDDictionaryEntryAVP &avpLowPriorityIndicator()                              	{ return m_avp_low_priority_indicator; }
    FDDictionaryEntryAVP &avp3gppPdpType()                                       	{ return m_avp_3gpp_pdp_type; }
    FDDictionaryEntryAVP &avpChangeCondition()                                   	{ return m_avp_change_condition; }
    FDDictionaryEntryAVP &avpDataCodingScheme()                                  	{ return m_avp_data_coding_scheme; }
    FDDictionaryEntryAVP &avpPreEmptionCapability()                              	{ return m_avp_pre_emption_capability; }
    FDDictionaryEntryAVP &avpNniInformation()                                    	{ return m_avp_nni_information; }
    FDDictionaryEntryAVP &avp3gppChargingId()                                    	{ return m_avp_3gpp_charging_id; }
    FDDictionaryEntryAVP &avpUsageInformationReportSequenceNumber()              	{ return m_avp_usage_information_report_sequence_number; }
    FDDictionaryEntryAVP &avpSponsorIdentity()                                   	{ return m_avp_sponsor_identity; }
    FDDictionaryEntryAVP &avp3gppRatType()                                       	{ return m_avp_3gpp_rat_type; }
    FDDictionaryEntryAVP &avpRequestorPlmnIdentifier()                           	{ return m_avp_requestor_plmn_identifier; }
    FDDictionaryEntryAVP &avpPreEmptionVulnerability()                           	{ return m_avp_pre_emption_vulnerability; }
    FDDictionaryEntryAVP &avpAocSubscriptionInformation()                        	{ return m_avp_aoc_subscription_information; }
    FDDictionaryEntryAVP &avpReasonHeader()                                      	{ return m_avp_reason_header; }
    FDDictionaryEntryAVP &avpExternalIdentifier()                                	{ return m_avp_external_identifier; }
    FDDictionaryEntryAVP &avpRai()                                               	{ return m_avp_rai; }
    FDDictionaryEntryAVP &avpInterfacePort()                                     	{ return m_avp_interface_port; }
    FDDictionaryEntryAVP &avpCurrentTariff()                                     	{ return m_avp_current_tariff; }
    FDDictionaryEntryAVP &avpPsInformation()                                     	{ return m_avp_ps_information; }
    FDDictionaryEntryAVP &avpFinalUnitAction()                                   	{ return m_avp_final_unit_action; }
    FDDictionaryEntryAVP &avpCurrencyCode()                                      	{ return m_avp_currency_code; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaStatus()                       	{ return m_avp_presence_reporting_area_status; }
    FDDictionaryEntryAVP &avpMonitoringUeIdentifier()                            	{ return m_avp_monitoring_ue_identifier; }
    FDDictionaryEntryAVP &avpDownlinkRateLimit()                                 	{ return m_avp_downlink_rate_limit; }
    FDDictionaryEntryAVP &avpAccountingRecordNumber()                            	{ return m_avp_accounting_record_number; }
    FDDictionaryEntryAVP &avpAddressType()                                       	{ return m_avp_address_type; }
    FDDictionaryEntryAVP &avpDestinationHost()                                   	{ return m_avp_destination_host; }
    FDDictionaryEntryAVP &avpContentType()                                       	{ return m_avp_content_type; }
    FDDictionaryEntryAVP &avpEvent()                                             	{ return m_avp_event; }
    FDDictionaryEntryAVP &avpRrcCounterTimestamp()                               	{ return m_avp_rrc_counter_timestamp; }
    FDDictionaryEntryAVP &avpGuaranteedBitrateUl()                               	{ return m_avp_guaranteed_bitrate_ul; }
    FDDictionaryEntryAVP &avpAcctSessionId()                                     	{ return m_avp_acct_session_id; }
    FDDictionaryEntryAVP &avpAdditionalContentInformation()                      	{ return m_avp_additional_content_information; }
    FDDictionaryEntryAVP &avpMmeName()                                           	{ return m_avp_mme_name; }
    FDDictionaryEntryAVP &avpMediaInitiatorFlag()                                	{ return m_avp_media_initiator_flag; }
    FDDictionaryEntryAVP &avpUserEquipmentInfoType()                             	{ return m_avp_user_equipment_info_type; }
    FDDictionaryEntryAVP &avpMonitoringUeHplmnIdentifier()                       	{ return m_avp_monitoring_ue_hplmn_identifier; }
    FDDictionaryEntryAVP &avpAnnouncingUeHplmnIdentifier()                       	{ return m_avp_announcing_ue_hplmn_identifier; }
    FDDictionaryEntryAVP &avp3gppChargingCharacteristics()                       	{ return m_avp_3gpp_charging_characteristics; }
    FDDictionaryEntryAVP &avpProxyInfo()                                         	{ return m_avp_proxy_info; }
    FDDictionaryEntryAVP &avpOriginAppLayerUserId()                              	{ return m_avp_origin_app_layer_user_id; }
    FDDictionaryEntryAVP &avpInterfaceId()                                       	{ return m_avp_interface_id; }
    FDDictionaryEntryAVP &avpApnRateControlUplink()                              	{ return m_avp_apn_rate_control_uplink; }
    FDDictionaryEntryAVP &avpApnRateControl()                                    	{ return m_avp_apn_rate_control; }
    FDDictionaryEntryAVP &avpCauseCode()                                         	{ return m_avp_cause_code; }
    FDDictionaryEntryAVP &avpRouteHeaderTransmitted()                            	{ return m_avp_route_header_transmitted; }
    FDDictionaryEntryAVP &avpTypeNumber()                                        	{ return m_avp_type_number; }
    FDDictionaryEntryAVP &avpSmsInformation()                                    	{ return m_avp_sms_information; }
    FDDictionaryEntryAVP &avpSmProtocolId()                                      	{ return m_avp_sm_protocol_id; }
    FDDictionaryEntryAVP &avpChargingRuleBaseName()                              	{ return m_avp_charging_rule_base_name; }
    FDDictionaryEntryAVP &avpTimeFirstReception()                                	{ return m_avp_time_first_reception; }
    FDDictionaryEntryAVP &avpUserEquipmentInfoValue()                            	{ return m_avp_user_equipment_info_value; }
    FDDictionaryEntryAVP &avpRouteRecord()                                       	{ return m_avp_route_record; }
    FDDictionaryEntryAVP &avpClassIdentifier()                                   	{ return m_avp_class_identifier; }
    FDDictionaryEntryAVP &avpEventType()                                         	{ return m_avp_event_type; }
    FDDictionaryEntryAVP &avpCallingPartyAddress()                               	{ return m_avp_calling_party_address; }
    FDDictionaryEntryAVP &avpProseDirectCommunicationReceptionDataContainer()    	{ return m_avp_prose_direct_communication_reception_data_container; }
    FDDictionaryEntryAVP &avpExponent()                                          	{ return m_avp_exponent; }
    FDDictionaryEntryAVP &avpOriginatorInterface()                               	{ return m_avp_originator_interface; }
    FDDictionaryEntryAVP &avpErrorReportingHost()                                	{ return m_avp_error_reporting_host; }
    FDDictionaryEntryAVP &avpNbifomSupport()                                     	{ return m_avp_nbifom_support; }
    FDDictionaryEntryAVP &avpMultipleServicesCreditControl()                     	{ return m_avp_multiple_services_credit_control; }
    FDDictionaryEntryAVP &avpMessageType()                                       	{ return m_avp_message_type; }
    FDDictionaryEntryAVP &avpPdpContextType()                                    	{ return m_avp_pdp_context_type; }
    FDDictionaryEntryAVP &avpApplicationServiceProviderIdentity()                	{ return m_avp_application_service_provider_identity; }
    FDDictionaryEntryAVP &avpProximityAlertIndication()                          	{ return m_avp_proximity_alert_indication; }
    FDDictionaryEntryAVP &avpLcsRequestorIdString()                              	{ return m_avp_lcs_requestor_id_string; }
    FDDictionaryEntryAVP &avpServiceContextId()                                  	{ return m_avp_service_context_id; }
    FDDictionaryEntryAVP &avpMonitoringUeVplmnIdentifier()                       	{ return m_avp_monitoring_ue_vplmn_identifier; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthDl()                           	{ return m_avp_max_requested_bandwidth_dl; }
    FDDictionaryEntryAVP &avpOriginStateId()                                     	{ return m_avp_origin_state_id; }
    FDDictionaryEntryAVP &avpImsApplicationReferenceIdentifier()                 	{ return m_avp_ims_application_reference_identifier; }
    FDDictionaryEntryAVP &avpSessionId()                                         	{ return m_avp_session_id; }
    FDDictionaryEntryAVP &avpBearerService()                                     	{ return m_avp_bearer_service; }
    FDDictionaryEntryAVP &avp3gppUserLocationInfo()                              	{ return m_avp_3gpp_user_location_info; }
    FDDictionaryEntryAVP &avpInterfaceType()                                     	{ return m_avp_interface_type; }
    FDDictionaryEntryAVP &avpDestinationInterface()                              	{ return m_avp_destination_interface; }
    FDDictionaryEntryAVP &avpAccessTransferInformation()                         	{ return m_avp_access_transfer_information; }
    FDDictionaryEntryAVP &avpAccountingRecordType()                              	{ return m_avp_accounting_record_type; }
    FDDictionaryEntryAVP &avpAuthApplicationId()                                 	{ return m_avp_auth_application_id; }
    FDDictionaryEntryAVP &avpLcsApn()                                            	{ return m_avp_lcs_apn; }
    FDDictionaryEntryAVP &avpTadIdentifier()                                     	{ return m_avp_tad_identifier; }
    FDDictionaryEntryAVP &avpRecipientReceivedAddress()                          	{ return m_avp_recipient_received_address; }
    FDDictionaryEntryAVP &avpDynamicAddressFlag()                                	{ return m_avp_dynamic_address_flag; }
    FDDictionaryEntryAVP &avpLocalGwInsertedIndication()                         	{ return m_avp_local_gw_inserted_indication; }
    FDDictionaryEntryAVP &avpProseGroupIpMulticastAddress()                      	{ return m_avp_prose_group_ip_multicast_address; }
    FDDictionaryEntryAVP &avpSmDeviceTriggerIndicator()                          	{ return m_avp_sm_device_trigger_indicator; }
    FDDictionaryEntryAVP &avpImsCommunicationServiceIdentifier()                 	{ return m_avp_ims_communication_service_identifier; }
    FDDictionaryEntryAVP &avpLcsRequestorId()                                    	{ return m_avp_lcs_requestor_id; }
    FDDictionaryEntryAVP &avpVisitedPlmnId()                                     	{ return m_avp_visited_plmn_id; }
    FDDictionaryEntryAVP &avpMtcIwfAddress()                                     	{ return m_avp_mtc_iwf_address; }
    FDDictionaryEntryAVP &avpCcServiceSpecificUnits()                            	{ return m_avp_cc_service_specific_units; }
    FDDictionaryEntryAVP &avpProxyState()                                        	{ return m_avp_proxy_state; }
    FDDictionaryEntryAVP &avpSubmissionTime()                                    	{ return m_avp_submission_time; }
    FDDictionaryEntryAVP &avpSmMessageType()                                     	{ return m_avp_sm_message_type; }
    FDDictionaryEntryAVP &avpAssociatedUri()                                     	{ return m_avp_associated_uri; }
    FDDictionaryEntryAVP &avpSmsNode()                                           	{ return m_avp_sms_node; }
    FDDictionaryEntryAVP &avpLocalSequenceNumber()                               	{ return m_avp_local_sequence_number; }
    FDDictionaryEntryAVP &avpPsFurnishChargingInformation()                      	{ return m_avp_ps_furnish_charging_information; }
    FDDictionaryEntryAVP &avpTdfIpAddress()                                      	{ return m_avp_tdf_ip_address; }
    FDDictionaryEntryAVP &avpTimeFirstTransmission()                             	{ return m_avp_time_first_transmission; }
    FDDictionaryEntryAVP &avpUserCsgInformation()                                	{ return m_avp_user_csg_information; }
    FDDictionaryEntryAVP &avpCalledIdentityChange()                              	{ return m_avp_called_identity_change; }
    FDDictionaryEntryAVP &avpTargetAppLayerUserId()                              	{ return m_avp_target_app_layer_user_id; }
    FDDictionaryEntryAVP &avpVendorSpecificApplicationId()                       	{ return m_avp_vendor_specific_application_id; }
    FDDictionaryEntryAVP &avpDynamicAddressFlagExtension()                       	{ return m_avp_dynamic_address_flag_extension; }
    FDDictionaryEntryAVP &avpServiceSpecificInfo()                               	{ return m_avp_service_specific_info; }
    FDDictionaryEntryAVP &avpAddresseeType()                                     	{ return m_avp_addressee_type; }
    FDDictionaryEntryAVP &avpServiceId()                                         	{ return m_avp_service_id; }
    FDDictionaryEntryAVP &avpSdpMediaDescription()                               	{ return m_avp_sdp_media_description; }
    FDDictionaryEntryAVP &avpNumberPortabilityRoutingInformation()               	{ return m_avp_number_portability_routing_information; }
    FDDictionaryEntryAVP &avpAccumulatedCost()                                   	{ return m_avp_accumulated_cost; }
    FDDictionaryEntryAVP &avpLayer2GroupId()                                     	{ return m_avp_layer_2_group_id; }
    FDDictionaryEntryAVP &avpMbmsDataTransferStop()                              	{ return m_avp_mbms_data_transfer_stop; }
    FDDictionaryEntryAVP &avpAdditionalTypeInformation()                         	{ return m_avp_additional_type_information; }
    FDDictionaryEntryAVP &avpMbmsSessionIdentity()                               	{ return m_avp_mbms_session_identity; }
    FDDictionaryEntryAVP &avpNodeId()                                            	{ return m_avp_node_id; }
    FDDictionaryEntryAVP &avpOriginHost()                                        	{ return m_avp_origin_host; }
    FDDictionaryEntryAVP &avpQosInformation()                                    	{ return m_avp_qos_information; }
    FDDictionaryEntryAVP &avpChargingPerIpCanSessionIndicator()                  	{ return m_avp_charging_per_ip_can_session_indicator; }
    FDDictionaryEntryAVP &avpRoleOfNode()                                        	{ return m_avp_role_of_node; }
    FDDictionaryEntryAVP &avpRateElement()                                       	{ return m_avp_rate_element; }
    FDDictionaryEntryAVP &avpAuxApplicInfo()                                     	{ return m_avp_aux_applic_info; }
    FDDictionaryEntryAVP &avpRateControlMaxRate()                                	{ return m_avp_rate_control_max_rate; }
    FDDictionaryEntryAVP &avpRealTimeTariffInformation()                         	{ return m_avp_real_time_tariff_information; }
    FDDictionaryEntryAVP &avpScsAsAddress()                                      	{ return m_avp_scs_as_address; }
    FDDictionaryEntryAVP &avpQosClassIdentifier()                                	{ return m_avp_qos_class_identifier; }
    FDDictionaryEntryAVP &avpMessageClass()                                      	{ return m_avp_message_class; }
    FDDictionaryEntryAVP &avpAccessTransferType()                                	{ return m_avp_access_transfer_type; }
    FDDictionaryEntryAVP &avpTmgi()                                              	{ return m_avp_tmgi; }
    FDDictionaryEntryAVP &avpMbms2g3gIndicator()                                 	{ return m_avp_mbms_2g_3g_indicator; }
    FDDictionaryEntryAVP &avpMbmsDataTransferStart()                             	{ return m_avp_mbms_data_transfer_start; }
    FDDictionaryEntryAVP &avpLocationType()                                      	{ return m_avp_location_type; }
    FDDictionaryEntryAVP &avpAddressData()                                       	{ return m_avp_address_data; }
    FDDictionaryEntryAVP &avpTariffXml()                                         	{ return m_avp_tariff_xml; }
    FDDictionaryEntryAVP &avpWlanLinkLayerId()                                   	{ return m_avp_wlan_link_layer_id; }
    FDDictionaryEntryAVP &avpMsisdn()                                            	{ return m_avp_msisdn; }
    FDDictionaryEntryAVP &avpRadioResourcesIndicator()                           	{ return m_avp_radio_resources_indicator; }
    FDDictionaryEntryAVP &avpAocCostInformation()                                	{ return m_avp_aoc_cost_information; }
    FDDictionaryEntryAVP &avpPriority()                                          	{ return m_avp_priority; }
    FDDictionaryEntryAVP &avpContentVersion()                                    	{ return m_avp_content_version; }
    FDDictionaryEntryAVP &avpDrmContent()                                        	{ return m_avp_drm_content; }
    FDDictionaryEntryAVP &avpLcsFormatIndicator()                                	{ return m_avp_lcs_format_indicator; }
    FDDictionaryEntryAVP &avpPocServerRole()                                     	{ return m_avp_poc_server_role; }
    FDDictionaryEntryAVP &avpMmtelInformation()                                  	{ return m_avp_mmtel_information; }
    FDDictionaryEntryAVP &avpReadReplyReportRequested()                          	{ return m_avp_read_reply_report_requested; }
    FDDictionaryEntryAVP &avpRecipientInfo()                                     	{ return m_avp_recipient_info; }
    FDDictionaryEntryAVP &avpLcsClientExternalId()                               	{ return m_avp_lcs_client_external_id; }
    FDDictionaryEntryAVP &avpOriginRealm()                                       	{ return m_avp_origin_realm; }
    FDDictionaryEntryAVP &avpImsChargingIdentifier()                             	{ return m_avp_ims_charging_identifier; }
    FDDictionaryEntryAVP &avpMbmsServiceType()                                   	{ return m_avp_mbms_service_type; }
    FDDictionaryEntryAVP &avpProseUeId()                                         	{ return m_avp_prose_ue_id; }
    FDDictionaryEntryAVP &avpProximityAlertTimestamp()                           	{ return m_avp_proximity_alert_timestamp; }
    FDDictionaryEntryAVP &avp3gppNsapi()                                         	{ return m_avp_3gpp_nsapi; }
    FDDictionaryEntryAVP &avpServingPlmnRateControl()                            	{ return m_avp_serving_plmn_rate_control; }
    FDDictionaryEntryAVP &avpEnhancedDiagnostics()                               	{ return m_avp_enhanced_diagnostics; }
    FDDictionaryEntryAVP &avpAccountingSubSessionId()                            	{ return m_avp_accounting_sub_session_id; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaIdentifier()                   	{ return m_avp_presence_reporting_area_identifier; }
    FDDictionaryEntryAVP &avpOriginatorAddress()                                 	{ return m_avp_originator_address; }
    FDDictionaryEntryAVP &avpScefId()                                            	{ return m_avp_scef_id; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaInformation()                  	{ return m_avp_presence_reporting_area_information; }
    FDDictionaryEntryAVP &avpSsid()                                              	{ return m_avp_ssid; }
    FDDictionaryEntryAVP &avpOriginatorReceivedAddress()                         	{ return m_avp_originator_received_address; }
    FDDictionaryEntryAVP &avpMessageSize()                                       	{ return m_avp_message_size; }
    FDDictionaryEntryAVP &avpRadioFrequency()                                    	{ return m_avp_radio_frequency; }
    FDDictionaryEntryAVP &avpApnAggregateMaxBitrateDl()                          	{ return m_avp_apn_aggregate_max_bitrate_dl; }
    FDDictionaryEntryAVP &avpRrcCauseCounter()                                   	{ return m_avp_rrc_cause_counter; }
    FDDictionaryEntryAVP &avpProse3rdPartyApplicationId()                        	{ return m_avp_prose_3rd_party_application_id; }
    FDDictionaryEntryAVP &avpAccountingRealtimeRequired()                        	{ return m_avp_accounting_realtime_required; }
    FDDictionaryEntryAVP &avpBearerIdentifier()                                  	{ return m_avp_bearer_identifier; }
    FDDictionaryEntryAVP &avp3gppSgsnMccMnc()                                    	{ return m_avp_3gpp_sgsn_mcc_mnc; }
    FDDictionaryEntryAVP &avpApplicationSpecificData()                           	{ return m_avp_application_specific_data; }
    FDDictionaryEntryAVP &avpPsFreeFormatData()                                  	{ return m_avp_ps_free_format_data; }
    FDDictionaryEntryAVP &avpRatingGroup()                                       	{ return m_avp_rating_group; }
    FDDictionaryEntryAVP &avpOutgoingSessionId()                                 	{ return m_avp_outgoing_session_id; }
    FDDictionaryEntryAVP &avpMessageId()                                         	{ return m_avp_message_id; }
    FDDictionaryEntryAVP &avpAllocationRetentionPriority()                       	{ return m_avp_allocation_retention_priority; }
    FDDictionaryEntryAVP &avpInstanceId()                                        	{ return m_avp_instance_id; }
    FDDictionaryEntryAVP &avpCalledAssertedIdentity()                            	{ return m_avp_called_asserted_identity; }
    FDDictionaryEntryAVP &avpAccessNetworkInfoChange()                           	{ return m_avp_access_network_info_change; }
    FDDictionaryEntryAVP &avpResultCode()                                        	{ return m_avp_result_code; }
    FDDictionaryEntryAVP &avpAcctInterimInterval()                               	{ return m_avp_acct_interim_interval; }
    FDDictionaryEntryAVP &avpAccountExpiration()                                 	{ return m_avp_account_expiration; }
    FDDictionaryEntryAVP &avpMbmsServiceArea()                                   	{ return m_avp_mbms_service_area; }
    FDDictionaryEntryAVP &avpMmeNumberForMtSms()                                 	{ return m_avp_mme_number_for_mt_sms; }
    FDDictionaryEntryAVP &avpSipRequestTimestamp()                               	{ return m_avp_sip_request_timestamp; }
    FDDictionaryEntryAVP &avpProseValidityTimer()                                	{ return m_avp_prose_validity_timer; }
    FDDictionaryEntryAVP &avpIncomingTrunkGroupId()                              	{ return m_avp_incoming_trunk_group_id; }
    FDDictionaryEntryAVP &avpTrafficDataVolumes()                                	{ return m_avp_traffic_data_volumes; }
    FDDictionaryEntryAVP &avpFixedUserLocationInfo()                             	{ return m_avp_fixed_user_location_info; }
    FDDictionaryEntryAVP &avpSdpOfferTimestamp()                                 	{ return m_avp_sdp_offer_timestamp; }
    FDDictionaryEntryAVP &avpLcsClientName()                                     	{ return m_avp_lcs_client_name; }
    FDDictionaryEntryAVP &avpInterfaceText()                                     	{ return m_avp_interface_text; }
    FDDictionaryEntryAVP &avpCalledStationId()                                   	{ return m_avp_called_station_id; }
    FDDictionaryEntryAVP &avpRelatedChangeConditionInformation()                 	{ return m_avp_related_change_condition_information; }
    FDDictionaryEntryAVP &avpIpRealmDefaultIndication()                          	{ return m_avp_ip_realm_default_indication; }
    FDDictionaryEntryAVP &avpImsVisitedNetworkIdentifier()                       	{ return m_avp_ims_visited_network_identifier; }
    FDDictionaryEntryAVP &avpClientAddress()                                     	{ return m_avp_client_address; }
    FDDictionaryEntryAVP &avpMessageBody()                                       	{ return m_avp_message_body; }
    FDDictionaryEntryAVP &avpLcsInformation()                                    	{ return m_avp_lcs_information; }
    FDDictionaryEntryAVP &avpTimeQuotaMechanism()                                	{ return m_avp_time_quota_mechanism; }
    FDDictionaryEntryAVP &avpAocInformation()                                    	{ return m_avp_aoc_information; }
    FDDictionaryEntryAVP &avpTariffInformation()                                 	{ return m_avp_tariff_information; }
    FDDictionaryEntryAVP &avpSubscriptionId()                                    	{ return m_avp_subscription_id; }
    FDDictionaryEntryAVP &avpServerCapabilities()                                	{ return m_avp_server_capabilities; }
    FDDictionaryEntryAVP &avpRadioParameterSetInfo()                             	{ return m_avp_radio_parameter_set_info; }
    FDDictionaryEntryAVP &avpMediaComponentNumber()                              	{ return m_avp_media_component_number; }
    FDDictionaryEntryAVP &avpServingNodeType()                                   	{ return m_avp_serving_node_type; }
    FDDictionaryEntryAVP &avpMbmsGwAddress()                                     	{ return m_avp_mbms_gw_address; }
    FDDictionaryEntryAVP &avpAccountingInputOctets()                             	{ return m_avp_accounting_input_octets; }
    FDDictionaryEntryAVP &avpScsRealm()                                          	{ return m_avp_scs_realm; }
    FDDictionaryEntryAVP &avpMediaInitiatorParty()                               	{ return m_avp_media_initiator_party; }
    FDDictionaryEntryAVP &avpProseRoleOfUe()                                     	{ return m_avp_prose_role_of_ue; }
    FDDictionaryEntryAVP &avpStopTime()                                          	{ return m_avp_stop_time; }
    FDDictionaryEntryAVP &avpDiagnostics()                                       	{ return m_avp_diagnostics; }
    FDDictionaryEntryAVP &avpReferenceNumber()                                   	{ return m_avp_reference_number; }
    FDDictionaryEntryAVP &avpRelatedImsChargingIdentifierNode()                  	{ return m_avp_related_ims_charging_identifier_node; }
    FDDictionaryEntryAVP &avpProximityCancellationTimestamp()                    	{ return m_avp_proximity_cancellation_timestamp; }
    FDDictionaryEntryAVP &avpMmboxStorageRequested()                             	{ return m_avp_mmbox_storage_requested; }
    FDDictionaryEntryAVP &avpAfCorrelationInformation()                          	{ return m_avp_af_correlation_information; }
    FDDictionaryEntryAVP &avpCcTotalOctets()                                     	{ return m_avp_cc_total_octets; }
    FDDictionaryEntryAVP &avpPreferredAocCurrency()                              	{ return m_avp_preferred_aoc_currency; }
    FDDictionaryEntryAVP &avpSubscriptionIdType()                                	{ return m_avp_subscription_id_type; }
    FDDictionaryEntryAVP &avpPc3EpcControlProtocolCause()                        	{ return m_avp_pc3_epc_control_protocol_cause; }
    FDDictionaryEntryAVP &avpProseInformation()                                  	{ return m_avp_prose_information; }
    FDDictionaryEntryAVP &avpPhysicalAccessId()                                  	{ return m_avp_physical_access_id; }
    FDDictionaryEntryAVP &avpTranscoderInsertedIndication()                      	{ return m_avp_transcoder_inserted_indication; }
    FDDictionaryEntryAVP &avpCpCiotEpsOptimisationIndicator()                    	{ return m_avp_cp_ciot_eps_optimisation_indicator; }
    FDDictionaryEntryAVP &avpServerName()                                        	{ return m_avp_server_name; }
    FDDictionaryEntryAVP &avpApnAggregateMaxBitrateUl()                          	{ return m_avp_apn_aggregate_max_bitrate_ul; }
    FDDictionaryEntryAVP &avpSmDeviceTriggerInformation()                        	{ return m_avp_sm_device_trigger_information; }
    FDDictionaryEntryAVP &avpSdpAnswerTimestamp()                                	{ return m_avp_sdp_answer_timestamp; }
    FDDictionaryEntryAVP &avpAlternateChargedPartyAddress()                      	{ return m_avp_alternate_charged_party_address; }
    FDDictionaryEntryAVP &avpProseAppId()                                        	{ return m_avp_prose_app_id; }
    FDDictionaryEntryAVP &avpTwanUserLocationInfo()                              	{ return m_avp_twan_user_location_info; }
    FDDictionaryEntryAVP &avpCcMoney()                                           	{ return m_avp_cc_money; }
    FDDictionaryEntryAVP &avpLogicalAccessId()                                   	{ return m_avp_logical_access_id; }
    FDDictionaryEntryAVP &avpCellularNetworkInformation()                        	{ return m_avp_cellular_network_information; }
    FDDictionaryEntryAVP &avpOfflineCharging()                                   	{ return m_avp_offline_charging; }
    FDDictionaryEntryAVP &avpSmDischargeTime()                                   	{ return m_avp_sm_discharge_time; }
    FDDictionaryEntryAVP &avp3gppPsDataOffStatus()                               	{ return m_avp_3gpp_ps_data_off_status; }
    FDDictionaryEntryAVP &avpProseFunctionId()                                   	{ return m_avp_prose_function_id; }
    FDDictionaryEntryAVP &avpContentSize()                                       	{ return m_avp_content_size; }
    FDDictionaryEntryAVP &avpUniPduCpOnlyFlag()                                  	{ return m_avp_uni_pdu_cp_only_flag; }
    FDDictionaryEntryAVP &avpNniType()                                           	{ return m_avp_nni_type; }
    FDDictionaryEntryAVP &avpTimeQuotaType()                                     	{ return m_avp_time_quota_type; }
    FDDictionaryEntryAVP &avpUeLocalIpAddress()                                  	{ return m_avp_ue_local_ip_address; }
    FDDictionaryEntryAVP &avpMediaComponentStatus()                              	{ return m_avp_media_component_status; }
    FDDictionaryEntryAVP &avpGgsnAddress()                                       	{ return m_avp_ggsn_address; }
    FDDictionaryEntryAVP &avpNeighbourNodeAddress()                              	{ return m_avp_neighbour_node_address; }
    FDDictionaryEntryAVP &avp3gppImsiMccMnc()                                    	{ return m_avp_3gpp_imsi_mcc_mnc; }
    FDDictionaryEntryAVP &avpValueDigits()                                       	{ return m_avp_value_digits; }
    FDDictionaryEntryAVP &avpFileRepairSupported()                               	{ return m_avp_file_repair_supported; }
    FDDictionaryEntryAVP &avpChangeTime()                                        	{ return m_avp_change_time; }
    FDDictionaryEntryAVP &avpNodeFunctionality()                                 	{ return m_avp_node_functionality; }
    FDDictionaryEntryAVP &avpSessionPriority()                                   	{ return m_avp_session_priority; }
    FDDictionaryEntryAVP &avpRecipientAddress()                                  	{ return m_avp_recipient_address; }
    FDDictionaryEntryAVP &avpLcsNameString()                                     	{ return m_avp_lcs_name_string; }
    FDDictionaryEntryAVP &avpCounterValue()                                      	{ return m_avp_counter_value; }
    FDDictionaryEntryAVP &avpTrunkGroupId()                                      	{ return m_avp_trunk_group_id; }
    FDDictionaryEntryAVP &avpNbifomMode()                                        	{ return m_avp_nbifom_mode; }
    FDDictionaryEntryAVP &avpIncrementalCost()                                   	{ return m_avp_incremental_cost; }
    FDDictionaryEntryAVP &avpTwagAddress()                                       	{ return m_avp_twag_address; }
    FDDictionaryEntryAVP &avpSdpTimestamps()                                     	{ return m_avp_sdp_timestamps; }
    FDDictionaryEntryAVP &avpPresenceReportingAreaElementsList()                 	{ return m_avp_presence_reporting_area_elements_list; }
    FDDictionaryEntryAVP &avpRequestedPartyAddress()                             	{ return m_avp_requested_party_address; }
    FDDictionaryEntryAVP &avpSgiPtpTunnellingMethod()                            	{ return m_avp_sgi_ptp_tunnelling_method; }
    FDDictionaryEntryAVP &avpRelationshipMode()                                  	{ return m_avp_relationship_mode; }
    FDDictionaryEntryAVP &avpConditionalApnAggregateMaxBitrate()                 	{ return m_avp_conditional_apn_aggregate_max_bitrate; }
    FDDictionaryEntryAVP &avpCcUnitType()                                        	{ return m_avp_cc_unit_type; }
    FDDictionaryEntryAVP &avpInterOperatorIdentifier()                           	{ return m_avp_inter_operator_identifier; }
    FDDictionaryEntryAVP &avpCoverageStatus()                                    	{ return m_avp_coverage_status; }
    FDDictionaryEntryAVP &avpOriginatingIoi()                                    	{ return m_avp_originating_ioi; }
    FDDictionaryEntryAVP &avpAccessNetworkInformation()                          	{ return m_avp_access_network_information; }
    FDDictionaryEntryAVP &avpNextTariff()                                        	{ return m_avp_next_tariff; }
    FDDictionaryEntryAVP &avp3gppImsi()                                          	{ return m_avp_3gpp_imsi; }
    FDDictionaryEntryAVP &avpCoverageInfo()                                      	{ return m_avp_coverage_info; }
    FDDictionaryEntryAVP &avpLcsDataCodingScheme()                               	{ return m_avp_lcs_data_coding_scheme; }
    FDDictionaryEntryAVP &avpRequestingEpuid()                                   	{ return m_avp_requesting_epuid; }
    FDDictionaryEntryAVP &avpFlowNumber()                                        	{ return m_avp_flow_number; }
    FDDictionaryEntryAVP &avpRatType()                                           	{ return m_avp_rat_type; }
    FDDictionaryEntryAVP &avpImsInformation()                                    	{ return m_avp_ims_information; }
    FDDictionaryEntryAVP &avpMmtelSserviceType()                                 	{ return m_avp_mmtel_sservice_type; }
    FDDictionaryEntryAVP &avpNetworkCallReferenceNumber()                        	{ return m_avp_network_call_reference_number; }
    FDDictionaryEntryAVP &avpReplyApplicId()                                     	{ return m_avp_reply_applic_id; }
    FDDictionaryEntryAVP &avpDeliveryReportRequested()                           	{ return m_avp_delivery_report_requested; }
    FDDictionaryEntryAVP &avpSmscAddress()                                       	{ return m_avp_smsc_address; }
    FDDictionaryEntryAVP &avpProseRequestTimestamp()                             	{ return m_avp_prose_request_timestamp; }
    FDDictionaryEntryAVP &avpUserEquipmentInfo()                                 	{ return m_avp_user_equipment_info; }
    FDDictionaryEntryAVP &avpAdditionalExceptionReports()                        	{ return m_avp_additional_exception_reports; }
    FDDictionaryEntryAVP &avpScsAddress()                                        	{ return m_avp_scs_address; }
    FDDictionaryEntryAVP &avpQuotaConsumptionTime()                              	{ return m_avp_quota_consumption_time; }
    FDDictionaryEntryAVP &avpRanNasReleaseCause()                                	{ return m_avp_ran_nas_release_cause; }
    FDDictionaryEntryAVP &avpUserSessionId()                                     	{ return m_avp_user_session_id; }
    FDDictionaryEntryAVP &avpMmContentType()                                     	{ return m_avp_mm_content_type; }
    FDDictionaryEntryAVP &avpServiceMode()                                       	{ return m_avp_service_mode; }
    FDDictionaryEntryAVP &avpSmUserDataHeader()                                  	{ return m_avp_sm_user_data_header; }
    FDDictionaryEntryAVP &avpPdnConnectionChargingId()                           	{ return m_avp_pdn_connection_charging_id; }
    FDDictionaryEntryAVP &avpRateControlTimeUnit()                               	{ return m_avp_rate_control_time_unit; }
    FDDictionaryEntryAVP &avpCalledPartyAddress()                                	{ return m_avp_called_party_address; }
    FDDictionaryEntryAVP &avpSmServiceType()                                     	{ return m_avp_sm_service_type; }
    FDDictionaryEntryAVP &avpBearerCapability()                                  	{ return m_avp_bearer_capability; }
    FDDictionaryEntryAVP &avpTimeFirstUsage()                                    	{ return m_avp_time_first_usage; }
    FDDictionaryEntryAVP &avpDeferredLocationEventType()                         	{ return m_avp_deferred_location_event_type; }
    FDDictionaryEntryAVP &avpSdpType()                                           	{ return m_avp_sdp_type; }
    FDDictionaryEntryAVP &avpApplicationServerInformation()                      	{ return m_avp_application_server_information; }
    FDDictionaryEntryAVP &avpProxyHost()                                         	{ return m_avp_proxy_host; }
    FDDictionaryEntryAVP &avpUserLocationInfoTime()                              	{ return m_avp_user_location_info_time; }
    FDDictionaryEntryAVP &avpSgsnAddress()                                       	{ return m_avp_sgsn_address; }
    FDDictionaryEntryAVP &avpAnnouncingUeVplmnIdentifier()                       	{ return m_avp_announcing_ue_vplmn_identifier; }
    FDDictionaryEntryAVP &avpProseEventType()                                    	{ return m_avp_prose_event_type; }
    FDDictionaryEntryAVP &avpVcsInformation()                                    	{ return m_avp_vcs_information; }
    FDDictionaryEntryAVP &avpCpdtInformation()                                   	{ return m_avp_cpdt_information; }
    FDDictionaryEntryAVP &avp3gppMsTimezone()                                    	{ return m_avp_3gpp_ms_timezone; }
    FDDictionaryEntryAVP &avpSgwChange()                                         	{ return m_avp_sgw_change; }
    FDDictionaryEntryAVP &avpIpCanType()                                         	{ return m_avp_ip_can_type; }
    FDDictionaryEntryAVP &avpOptionalCapability()                                	{ return m_avp_optional_capability; }
    FDDictionaryEntryAVP &avpTariffTimeChange()                                  	{ return m_avp_tariff_time_change; }
    FDDictionaryEntryAVP &avpCcTime()                                            	{ return m_avp_cc_time; }
    FDDictionaryEntryAVP &avpServedPartyIpAddress()                              	{ return m_avp_served_party_ip_address; }
    FDDictionaryEntryAVP &avpEnvelopeReporting()                                 	{ return m_avp_envelope_reporting; }
    FDDictionaryEntryAVP &avpProseFunctionIpAddress()                            	{ return m_avp_prose_function_ip_address; }
    FDDictionaryEntryAVP &avpReplyPathRequested()                                	{ return m_avp_reply_path_requested; }
    FDDictionaryEntryAVP &avpSoftwareVersion()                                   	{ return m_avp_software_version; }
    FDDictionaryEntryAVP &avpUplinkRateLimit()                                   	{ return m_avp_uplink_rate_limit; }
    FDDictionaryEntryAVP &avpVendorId()                                          	{ return m_avp_vendor_id; }
    FDDictionaryEntryAVP &avpUnitValue()                                         	{ return m_avp_unit_value; }
    FDDictionaryEntryAVP &avpDestinationRealm()                                  	{ return m_avp_destination_realm; }
    FDDictionaryEntryAVP &avpNumberOfMessagesSent()                              	{ return m_avp_number_of_messages_sent; }
    FDDictionaryEntryAVP &avpRoleOfProseFunction()                               	{ return m_avp_role_of_prose_function; }

private:
    FDDictionaryEntryApplication m_app;                                          

    FDDictionaryEntryCommand m_cmd_acr;
    FDDictionaryEntryCommand m_cmd_aca;

    FDDictionaryEntryVendor m_vnd_3gpp2;
    FDDictionaryEntryVendor m_vnd_Etsi;
    FDDictionaryEntryVendor m_vnd_3gpp;

    FDDictionaryEntryAVP m_avp_lcs_client_dialed_by_ms;
    FDDictionaryEntryAVP m_avp_mme_realm;
    FDDictionaryEntryAVP m_avp_3gpp_ggsn_mcc_mnc;
    FDDictionaryEntryAVP m_avp_event_timestamp;
    FDDictionaryEntryAVP m_avp_service_identifier;
    FDDictionaryEntryAVP m_avp_csg_access_mode;
    FDDictionaryEntryAVP m_avp_prose_range_class;
    FDDictionaryEntryAVP m_avp_ims_emergency_indicator;
    FDDictionaryEntryAVP m_avp_originator_sccp_address;
    FDDictionaryEntryAVP m_avp_scale_factor;
    FDDictionaryEntryAVP m_avp_cn_operator_selection_entity;
    FDDictionaryEntryAVP m_avp_acct_multi_session_id;
    FDDictionaryEntryAVP m_avp_inter_ue_transfer;
    FDDictionaryEntryAVP m_avp_related_ims_charging_identifier;
    FDDictionaryEntryAVP m_avp_initial_ims_charging_identifier;
    FDDictionaryEntryAVP m_avp_called_identity;
    FDDictionaryEntryAVP m_avp_sdp_media_component;
    FDDictionaryEntryAVP m_avp_radio_parameter_set_values;
    FDDictionaryEntryAVP m_avp_aoc_service_type;
    FDDictionaryEntryAVP m_avp_acct_application_id;
    FDDictionaryEntryAVP m_avp_domain_name;
    FDDictionaryEntryAVP m_avp_route_header_received;
    FDDictionaryEntryAVP m_avp_uwan_user_location_info;
    FDDictionaryEntryAVP m_avp_priority_level;
    FDDictionaryEntryAVP m_avp_online_charging_flag;
    FDDictionaryEntryAVP m_avp_aoc_service_obligatory_type;
    FDDictionaryEntryAVP m_avp_msc_address;
    FDDictionaryEntryAVP m_avp_address_domain;
    FDDictionaryEntryAVP m_avp_sgw_address;
    FDDictionaryEntryAVP m_avp_from_address;
    FDDictionaryEntryAVP m_avp_service_information;
    FDDictionaryEntryAVP m_avp_lcs_client_type;
    FDDictionaryEntryAVP m_avp_lcs_client_id;
    FDDictionaryEntryAVP m_avp_sdp_media_name;
    FDDictionaryEntryAVP m_avp_sm_sequence_number;
    FDDictionaryEntryAVP m_avp_time_window;
    FDDictionaryEntryAVP m_avp_token_text;
    FDDictionaryEntryAVP m_avp_terminating_ioi;
    FDDictionaryEntryAVP m_avp_ps_append_free_format_data;
    FDDictionaryEntryAVP m_avp_sms_result;
    FDDictionaryEntryAVP m_avp_guaranteed_bitrate_dl;
    FDDictionaryEntryAVP m_avp_subscription_id_data;
    FDDictionaryEntryAVP m_avp_location_estimate_type;
    FDDictionaryEntryAVP m_avp_adc_rule_base_name;
    FDDictionaryEntryAVP m_avp_location_info;
    FDDictionaryEntryAVP m_avp_mbms_information;
    FDDictionaryEntryAVP m_avp_aoc_service;
    FDDictionaryEntryAVP m_avp_csg_id;
    FDDictionaryEntryAVP m_avp_location_estimate;
    FDDictionaryEntryAVP m_avp_mbms_charged_party;
    FDDictionaryEntryAVP m_avp_prose_reason_for_cancellation;
    FDDictionaryEntryAVP m_avp_pdp_address_prefix_length;
    FDDictionaryEntryAVP m_avp_epdg_address;
    FDDictionaryEntryAVP m_avp_transit_ioi_list;
    FDDictionaryEntryAVP m_avp_access_network_charging_identifier_value;
    FDDictionaryEntryAVP m_avp_cg_address;
    FDDictionaryEntryAVP m_avp_prose_direct_communication_transmission_data_container;
    FDDictionaryEntryAVP m_avp_service_specific_data;
    FDDictionaryEntryAVP m_avp_sip_method;
    FDDictionaryEntryAVP m_avp_csg_membership_indication;
    FDDictionaryEntryAVP m_avp_time_last_usage;
    FDDictionaryEntryAVP m_avp_accounting_output_octets;
    FDDictionaryEntryAVP m_avp_sdp_session_description;
    FDDictionaryEntryAVP m_avp_session_direction;
    FDDictionaryEntryAVP m_avp_apn_rate_control_downlink;
    FDDictionaryEntryAVP m_avp_cc_output_octets;
    FDDictionaryEntryAVP m_avp_cn_ip_multicast_distribution;
    FDDictionaryEntryAVP m_avp_sm_status;
    FDDictionaryEntryAVP m_avp_prose_source_ip_address;
    FDDictionaryEntryAVP m_avp_prose_direct_discovery_model;
    FDDictionaryEntryAVP m_avp_rate_control_max_message_size;
    FDDictionaryEntryAVP m_avp_base_time_interval;
    FDDictionaryEntryAVP m_avp_poc_information;
    FDDictionaryEntryAVP m_avp_required_mbms_bearer_capabilities;
    FDDictionaryEntryAVP m_avp_3gpp_selection_mode;
    FDDictionaryEntryAVP m_avp_supplementary_service;
    FDDictionaryEntryAVP m_avp_pc3_control_protocol_cause;
    FDDictionaryEntryAVP m_avp_terminal_information;
    FDDictionaryEntryAVP m_avp_pdp_address;
    FDDictionaryEntryAVP m_avp_charging_characteristics_selection_mode;
    FDDictionaryEntryAVP m_avp_transmitter_info;
    FDDictionaryEntryAVP m_avp_time_usage;
    FDDictionaryEntryAVP m_avp_content_class;
    FDDictionaryEntryAVP m_avp_applic_id;
    FDDictionaryEntryAVP m_avp_3gpp2_bsid;
    FDDictionaryEntryAVP m_avp_aoc_format;
    FDDictionaryEntryAVP m_avp_udp_source_port;
    FDDictionaryEntryAVP m_avp_adaptations;
    FDDictionaryEntryAVP m_avp_positioning_data;
    FDDictionaryEntryAVP m_avp_error_message;
    FDDictionaryEntryAVP m_avp_imsi_unauthenticated_flag;
    FDDictionaryEntryAVP m_avp_mandatory_capability;
    FDDictionaryEntryAVP m_avp_recipient_sccp_address;
    FDDictionaryEntryAVP m_avp_expires;
    FDDictionaryEntryAVP m_avp_failed_avp;
    FDDictionaryEntryAVP m_avp_flows;
    FDDictionaryEntryAVP m_avp_requested_plmn_identifier;
    FDDictionaryEntryAVP m_avp_mbms_user_service_type;
    FDDictionaryEntryAVP m_avp_application_server;
    FDDictionaryEntryAVP m_avp_cc_input_octets;
    FDDictionaryEntryAVP m_avp_carrier_select_routing_information;
    FDDictionaryEntryAVP m_avp_time_stamps;
    FDDictionaryEntryAVP m_avp_start_time;
    FDDictionaryEntryAVP m_avp_af_charging_identifier;
    FDDictionaryEntryAVP m_avp_service_data_container;
    FDDictionaryEntryAVP m_avp_early_media_description;
    FDDictionaryEntryAVP m_avp_granted_service_unit;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_ul;
    FDDictionaryEntryAVP m_avp_imei;
    FDDictionaryEntryAVP m_avp_mms_information;
    FDDictionaryEntryAVP m_avp_user_name;
    FDDictionaryEntryAVP m_avp_bssid;
    FDDictionaryEntryAVP m_avp_low_priority_indicator;
    FDDictionaryEntryAVP m_avp_3gpp_pdp_type;
    FDDictionaryEntryAVP m_avp_change_condition;
    FDDictionaryEntryAVP m_avp_data_coding_scheme;
    FDDictionaryEntryAVP m_avp_pre_emption_capability;
    FDDictionaryEntryAVP m_avp_nni_information;
    FDDictionaryEntryAVP m_avp_3gpp_charging_id;
    FDDictionaryEntryAVP m_avp_usage_information_report_sequence_number;
    FDDictionaryEntryAVP m_avp_sponsor_identity;
    FDDictionaryEntryAVP m_avp_3gpp_rat_type;
    FDDictionaryEntryAVP m_avp_requestor_plmn_identifier;
    FDDictionaryEntryAVP m_avp_pre_emption_vulnerability;
    FDDictionaryEntryAVP m_avp_aoc_subscription_information;
    FDDictionaryEntryAVP m_avp_reason_header;
    FDDictionaryEntryAVP m_avp_external_identifier;
    FDDictionaryEntryAVP m_avp_rai;
    FDDictionaryEntryAVP m_avp_interface_port;
    FDDictionaryEntryAVP m_avp_current_tariff;
    FDDictionaryEntryAVP m_avp_ps_information;
    FDDictionaryEntryAVP m_avp_final_unit_action;
    FDDictionaryEntryAVP m_avp_currency_code;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_status;
    FDDictionaryEntryAVP m_avp_monitoring_ue_identifier;
    FDDictionaryEntryAVP m_avp_downlink_rate_limit;
    FDDictionaryEntryAVP m_avp_accounting_record_number;
    FDDictionaryEntryAVP m_avp_address_type;
    FDDictionaryEntryAVP m_avp_destination_host;
    FDDictionaryEntryAVP m_avp_content_type;
    FDDictionaryEntryAVP m_avp_event;
    FDDictionaryEntryAVP m_avp_rrc_counter_timestamp;
    FDDictionaryEntryAVP m_avp_guaranteed_bitrate_ul;
    FDDictionaryEntryAVP m_avp_acct_session_id;
    FDDictionaryEntryAVP m_avp_additional_content_information;
    FDDictionaryEntryAVP m_avp_mme_name;
    FDDictionaryEntryAVP m_avp_media_initiator_flag;
    FDDictionaryEntryAVP m_avp_user_equipment_info_type;
    FDDictionaryEntryAVP m_avp_monitoring_ue_hplmn_identifier;
    FDDictionaryEntryAVP m_avp_announcing_ue_hplmn_identifier;
    FDDictionaryEntryAVP m_avp_3gpp_charging_characteristics;
    FDDictionaryEntryAVP m_avp_proxy_info;
    FDDictionaryEntryAVP m_avp_origin_app_layer_user_id;
    FDDictionaryEntryAVP m_avp_interface_id;
    FDDictionaryEntryAVP m_avp_apn_rate_control_uplink;
    FDDictionaryEntryAVP m_avp_apn_rate_control;
    FDDictionaryEntryAVP m_avp_cause_code;
    FDDictionaryEntryAVP m_avp_route_header_transmitted;
    FDDictionaryEntryAVP m_avp_type_number;
    FDDictionaryEntryAVP m_avp_sms_information;
    FDDictionaryEntryAVP m_avp_sm_protocol_id;
    FDDictionaryEntryAVP m_avp_charging_rule_base_name;
    FDDictionaryEntryAVP m_avp_time_first_reception;
    FDDictionaryEntryAVP m_avp_user_equipment_info_value;
    FDDictionaryEntryAVP m_avp_route_record;
    FDDictionaryEntryAVP m_avp_class_identifier;
    FDDictionaryEntryAVP m_avp_event_type;
    FDDictionaryEntryAVP m_avp_calling_party_address;
    FDDictionaryEntryAVP m_avp_prose_direct_communication_reception_data_container;
    FDDictionaryEntryAVP m_avp_exponent;
    FDDictionaryEntryAVP m_avp_originator_interface;
    FDDictionaryEntryAVP m_avp_error_reporting_host;
    FDDictionaryEntryAVP m_avp_nbifom_support;
    FDDictionaryEntryAVP m_avp_multiple_services_credit_control;
    FDDictionaryEntryAVP m_avp_message_type;
    FDDictionaryEntryAVP m_avp_pdp_context_type;
    FDDictionaryEntryAVP m_avp_application_service_provider_identity;
    FDDictionaryEntryAVP m_avp_proximity_alert_indication;
    FDDictionaryEntryAVP m_avp_lcs_requestor_id_string;
    FDDictionaryEntryAVP m_avp_service_context_id;
    FDDictionaryEntryAVP m_avp_monitoring_ue_vplmn_identifier;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_dl;
    FDDictionaryEntryAVP m_avp_origin_state_id;
    FDDictionaryEntryAVP m_avp_ims_application_reference_identifier;
    FDDictionaryEntryAVP m_avp_session_id;
    FDDictionaryEntryAVP m_avp_bearer_service;
    FDDictionaryEntryAVP m_avp_3gpp_user_location_info;
    FDDictionaryEntryAVP m_avp_interface_type;
    FDDictionaryEntryAVP m_avp_destination_interface;
    FDDictionaryEntryAVP m_avp_access_transfer_information;
    FDDictionaryEntryAVP m_avp_accounting_record_type;
    FDDictionaryEntryAVP m_avp_auth_application_id;
    FDDictionaryEntryAVP m_avp_lcs_apn;
    FDDictionaryEntryAVP m_avp_tad_identifier;
    FDDictionaryEntryAVP m_avp_recipient_received_address;
    FDDictionaryEntryAVP m_avp_dynamic_address_flag;
    FDDictionaryEntryAVP m_avp_local_gw_inserted_indication;
    FDDictionaryEntryAVP m_avp_prose_group_ip_multicast_address;
    FDDictionaryEntryAVP m_avp_sm_device_trigger_indicator;
    FDDictionaryEntryAVP m_avp_ims_communication_service_identifier;
    FDDictionaryEntryAVP m_avp_lcs_requestor_id;
    FDDictionaryEntryAVP m_avp_visited_plmn_id;
    FDDictionaryEntryAVP m_avp_mtc_iwf_address;
    FDDictionaryEntryAVP m_avp_cc_service_specific_units;
    FDDictionaryEntryAVP m_avp_proxy_state;
    FDDictionaryEntryAVP m_avp_submission_time;
    FDDictionaryEntryAVP m_avp_sm_message_type;
    FDDictionaryEntryAVP m_avp_associated_uri;
    FDDictionaryEntryAVP m_avp_sms_node;
    FDDictionaryEntryAVP m_avp_local_sequence_number;
    FDDictionaryEntryAVP m_avp_ps_furnish_charging_information;
    FDDictionaryEntryAVP m_avp_tdf_ip_address;
    FDDictionaryEntryAVP m_avp_time_first_transmission;
    FDDictionaryEntryAVP m_avp_user_csg_information;
    FDDictionaryEntryAVP m_avp_called_identity_change;
    FDDictionaryEntryAVP m_avp_target_app_layer_user_id;
    FDDictionaryEntryAVP m_avp_vendor_specific_application_id;
    FDDictionaryEntryAVP m_avp_dynamic_address_flag_extension;
    FDDictionaryEntryAVP m_avp_service_specific_info;
    FDDictionaryEntryAVP m_avp_addressee_type;
    FDDictionaryEntryAVP m_avp_service_id;
    FDDictionaryEntryAVP m_avp_sdp_media_description;
    FDDictionaryEntryAVP m_avp_number_portability_routing_information;
    FDDictionaryEntryAVP m_avp_accumulated_cost;
    FDDictionaryEntryAVP m_avp_layer_2_group_id;
    FDDictionaryEntryAVP m_avp_mbms_data_transfer_stop;
    FDDictionaryEntryAVP m_avp_additional_type_information;
    FDDictionaryEntryAVP m_avp_mbms_session_identity;
    FDDictionaryEntryAVP m_avp_node_id;
    FDDictionaryEntryAVP m_avp_origin_host;
    FDDictionaryEntryAVP m_avp_qos_information;
    FDDictionaryEntryAVP m_avp_charging_per_ip_can_session_indicator;
    FDDictionaryEntryAVP m_avp_role_of_node;
    FDDictionaryEntryAVP m_avp_rate_element;
    FDDictionaryEntryAVP m_avp_aux_applic_info;
    FDDictionaryEntryAVP m_avp_rate_control_max_rate;
    FDDictionaryEntryAVP m_avp_real_time_tariff_information;
    FDDictionaryEntryAVP m_avp_scs_as_address;
    FDDictionaryEntryAVP m_avp_qos_class_identifier;
    FDDictionaryEntryAVP m_avp_message_class;
    FDDictionaryEntryAVP m_avp_access_transfer_type;
    FDDictionaryEntryAVP m_avp_tmgi;
    FDDictionaryEntryAVP m_avp_mbms_2g_3g_indicator;
    FDDictionaryEntryAVP m_avp_mbms_data_transfer_start;
    FDDictionaryEntryAVP m_avp_location_type;
    FDDictionaryEntryAVP m_avp_address_data;
    FDDictionaryEntryAVP m_avp_tariff_xml;
    FDDictionaryEntryAVP m_avp_wlan_link_layer_id;
    FDDictionaryEntryAVP m_avp_msisdn;
    FDDictionaryEntryAVP m_avp_radio_resources_indicator;
    FDDictionaryEntryAVP m_avp_aoc_cost_information;
    FDDictionaryEntryAVP m_avp_priority;
    FDDictionaryEntryAVP m_avp_content_version;
    FDDictionaryEntryAVP m_avp_drm_content;
    FDDictionaryEntryAVP m_avp_lcs_format_indicator;
    FDDictionaryEntryAVP m_avp_poc_server_role;
    FDDictionaryEntryAVP m_avp_mmtel_information;
    FDDictionaryEntryAVP m_avp_read_reply_report_requested;
    FDDictionaryEntryAVP m_avp_recipient_info;
    FDDictionaryEntryAVP m_avp_lcs_client_external_id;
    FDDictionaryEntryAVP m_avp_origin_realm;
    FDDictionaryEntryAVP m_avp_ims_charging_identifier;
    FDDictionaryEntryAVP m_avp_mbms_service_type;
    FDDictionaryEntryAVP m_avp_prose_ue_id;
    FDDictionaryEntryAVP m_avp_proximity_alert_timestamp;
    FDDictionaryEntryAVP m_avp_3gpp_nsapi;
    FDDictionaryEntryAVP m_avp_serving_plmn_rate_control;
    FDDictionaryEntryAVP m_avp_enhanced_diagnostics;
    FDDictionaryEntryAVP m_avp_accounting_sub_session_id;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_identifier;
    FDDictionaryEntryAVP m_avp_originator_address;
    FDDictionaryEntryAVP m_avp_scef_id;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_information;
    FDDictionaryEntryAVP m_avp_ssid;
    FDDictionaryEntryAVP m_avp_originator_received_address;
    FDDictionaryEntryAVP m_avp_message_size;
    FDDictionaryEntryAVP m_avp_radio_frequency;
    FDDictionaryEntryAVP m_avp_apn_aggregate_max_bitrate_dl;
    FDDictionaryEntryAVP m_avp_rrc_cause_counter;
    FDDictionaryEntryAVP m_avp_prose_3rd_party_application_id;
    FDDictionaryEntryAVP m_avp_accounting_realtime_required;
    FDDictionaryEntryAVP m_avp_bearer_identifier;
    FDDictionaryEntryAVP m_avp_3gpp_sgsn_mcc_mnc;
    FDDictionaryEntryAVP m_avp_application_specific_data;
    FDDictionaryEntryAVP m_avp_ps_free_format_data;
    FDDictionaryEntryAVP m_avp_rating_group;
    FDDictionaryEntryAVP m_avp_outgoing_session_id;
    FDDictionaryEntryAVP m_avp_message_id;
    FDDictionaryEntryAVP m_avp_allocation_retention_priority;
    FDDictionaryEntryAVP m_avp_instance_id;
    FDDictionaryEntryAVP m_avp_called_asserted_identity;
    FDDictionaryEntryAVP m_avp_access_network_info_change;
    FDDictionaryEntryAVP m_avp_result_code;
    FDDictionaryEntryAVP m_avp_acct_interim_interval;
    FDDictionaryEntryAVP m_avp_account_expiration;
    FDDictionaryEntryAVP m_avp_mbms_service_area;
    FDDictionaryEntryAVP m_avp_mme_number_for_mt_sms;
    FDDictionaryEntryAVP m_avp_sip_request_timestamp;
    FDDictionaryEntryAVP m_avp_prose_validity_timer;
    FDDictionaryEntryAVP m_avp_incoming_trunk_group_id;
    FDDictionaryEntryAVP m_avp_traffic_data_volumes;
    FDDictionaryEntryAVP m_avp_fixed_user_location_info;
    FDDictionaryEntryAVP m_avp_sdp_offer_timestamp;
    FDDictionaryEntryAVP m_avp_lcs_client_name;
    FDDictionaryEntryAVP m_avp_interface_text;
    FDDictionaryEntryAVP m_avp_called_station_id;
    FDDictionaryEntryAVP m_avp_related_change_condition_information;
    FDDictionaryEntryAVP m_avp_ip_realm_default_indication;
    FDDictionaryEntryAVP m_avp_ims_visited_network_identifier;
    FDDictionaryEntryAVP m_avp_client_address;
    FDDictionaryEntryAVP m_avp_message_body;
    FDDictionaryEntryAVP m_avp_lcs_information;
    FDDictionaryEntryAVP m_avp_time_quota_mechanism;
    FDDictionaryEntryAVP m_avp_aoc_information;
    FDDictionaryEntryAVP m_avp_tariff_information;
    FDDictionaryEntryAVP m_avp_subscription_id;
    FDDictionaryEntryAVP m_avp_server_capabilities;
    FDDictionaryEntryAVP m_avp_radio_parameter_set_info;
    FDDictionaryEntryAVP m_avp_media_component_number;
    FDDictionaryEntryAVP m_avp_serving_node_type;
    FDDictionaryEntryAVP m_avp_mbms_gw_address;
    FDDictionaryEntryAVP m_avp_accounting_input_octets;
    FDDictionaryEntryAVP m_avp_scs_realm;
    FDDictionaryEntryAVP m_avp_media_initiator_party;
    FDDictionaryEntryAVP m_avp_prose_role_of_ue;
    FDDictionaryEntryAVP m_avp_stop_time;
    FDDictionaryEntryAVP m_avp_diagnostics;
    FDDictionaryEntryAVP m_avp_reference_number;
    FDDictionaryEntryAVP m_avp_related_ims_charging_identifier_node;
    FDDictionaryEntryAVP m_avp_proximity_cancellation_timestamp;
    FDDictionaryEntryAVP m_avp_mmbox_storage_requested;
    FDDictionaryEntryAVP m_avp_af_correlation_information;
    FDDictionaryEntryAVP m_avp_cc_total_octets;
    FDDictionaryEntryAVP m_avp_preferred_aoc_currency;
    FDDictionaryEntryAVP m_avp_subscription_id_type;
    FDDictionaryEntryAVP m_avp_pc3_epc_control_protocol_cause;
    FDDictionaryEntryAVP m_avp_prose_information;
    FDDictionaryEntryAVP m_avp_physical_access_id;
    FDDictionaryEntryAVP m_avp_transcoder_inserted_indication;
    FDDictionaryEntryAVP m_avp_cp_ciot_eps_optimisation_indicator;
    FDDictionaryEntryAVP m_avp_server_name;
    FDDictionaryEntryAVP m_avp_apn_aggregate_max_bitrate_ul;
    FDDictionaryEntryAVP m_avp_sm_device_trigger_information;
    FDDictionaryEntryAVP m_avp_sdp_answer_timestamp;
    FDDictionaryEntryAVP m_avp_alternate_charged_party_address;
    FDDictionaryEntryAVP m_avp_prose_app_id;
    FDDictionaryEntryAVP m_avp_twan_user_location_info;
    FDDictionaryEntryAVP m_avp_cc_money;
    FDDictionaryEntryAVP m_avp_logical_access_id;
    FDDictionaryEntryAVP m_avp_cellular_network_information;
    FDDictionaryEntryAVP m_avp_offline_charging;
    FDDictionaryEntryAVP m_avp_sm_discharge_time;
    FDDictionaryEntryAVP m_avp_3gpp_ps_data_off_status;
    FDDictionaryEntryAVP m_avp_prose_function_id;
    FDDictionaryEntryAVP m_avp_content_size;
    FDDictionaryEntryAVP m_avp_uni_pdu_cp_only_flag;
    FDDictionaryEntryAVP m_avp_nni_type;
    FDDictionaryEntryAVP m_avp_time_quota_type;
    FDDictionaryEntryAVP m_avp_ue_local_ip_address;
    FDDictionaryEntryAVP m_avp_media_component_status;
    FDDictionaryEntryAVP m_avp_ggsn_address;
    FDDictionaryEntryAVP m_avp_neighbour_node_address;
    FDDictionaryEntryAVP m_avp_3gpp_imsi_mcc_mnc;
    FDDictionaryEntryAVP m_avp_value_digits;
    FDDictionaryEntryAVP m_avp_file_repair_supported;
    FDDictionaryEntryAVP m_avp_change_time;
    FDDictionaryEntryAVP m_avp_node_functionality;
    FDDictionaryEntryAVP m_avp_session_priority;
    FDDictionaryEntryAVP m_avp_recipient_address;
    FDDictionaryEntryAVP m_avp_lcs_name_string;
    FDDictionaryEntryAVP m_avp_counter_value;
    FDDictionaryEntryAVP m_avp_trunk_group_id;
    FDDictionaryEntryAVP m_avp_nbifom_mode;
    FDDictionaryEntryAVP m_avp_incremental_cost;
    FDDictionaryEntryAVP m_avp_twag_address;
    FDDictionaryEntryAVP m_avp_sdp_timestamps;
    FDDictionaryEntryAVP m_avp_presence_reporting_area_elements_list;
    FDDictionaryEntryAVP m_avp_requested_party_address;
    FDDictionaryEntryAVP m_avp_sgi_ptp_tunnelling_method;
    FDDictionaryEntryAVP m_avp_relationship_mode;
    FDDictionaryEntryAVP m_avp_conditional_apn_aggregate_max_bitrate;
    FDDictionaryEntryAVP m_avp_cc_unit_type;
    FDDictionaryEntryAVP m_avp_inter_operator_identifier;
    FDDictionaryEntryAVP m_avp_coverage_status;
    FDDictionaryEntryAVP m_avp_originating_ioi;
    FDDictionaryEntryAVP m_avp_access_network_information;
    FDDictionaryEntryAVP m_avp_next_tariff;
    FDDictionaryEntryAVP m_avp_3gpp_imsi;
    FDDictionaryEntryAVP m_avp_coverage_info;
    FDDictionaryEntryAVP m_avp_lcs_data_coding_scheme;
    FDDictionaryEntryAVP m_avp_requesting_epuid;
    FDDictionaryEntryAVP m_avp_flow_number;
    FDDictionaryEntryAVP m_avp_rat_type;
    FDDictionaryEntryAVP m_avp_ims_information;
    FDDictionaryEntryAVP m_avp_mmtel_sservice_type;
    FDDictionaryEntryAVP m_avp_network_call_reference_number;
    FDDictionaryEntryAVP m_avp_reply_applic_id;
    FDDictionaryEntryAVP m_avp_delivery_report_requested;
    FDDictionaryEntryAVP m_avp_smsc_address;
    FDDictionaryEntryAVP m_avp_prose_request_timestamp;
    FDDictionaryEntryAVP m_avp_user_equipment_info;
    FDDictionaryEntryAVP m_avp_additional_exception_reports;
    FDDictionaryEntryAVP m_avp_scs_address;
    FDDictionaryEntryAVP m_avp_quota_consumption_time;
    FDDictionaryEntryAVP m_avp_ran_nas_release_cause;
    FDDictionaryEntryAVP m_avp_user_session_id;
    FDDictionaryEntryAVP m_avp_mm_content_type;
    FDDictionaryEntryAVP m_avp_service_mode;
    FDDictionaryEntryAVP m_avp_sm_user_data_header;
    FDDictionaryEntryAVP m_avp_pdn_connection_charging_id;
    FDDictionaryEntryAVP m_avp_rate_control_time_unit;
    FDDictionaryEntryAVP m_avp_called_party_address;
    FDDictionaryEntryAVP m_avp_sm_service_type;
    FDDictionaryEntryAVP m_avp_bearer_capability;
    FDDictionaryEntryAVP m_avp_time_first_usage;
    FDDictionaryEntryAVP m_avp_deferred_location_event_type;
    FDDictionaryEntryAVP m_avp_sdp_type;
    FDDictionaryEntryAVP m_avp_application_server_information;
    FDDictionaryEntryAVP m_avp_proxy_host;
    FDDictionaryEntryAVP m_avp_user_location_info_time;
    FDDictionaryEntryAVP m_avp_sgsn_address;
    FDDictionaryEntryAVP m_avp_announcing_ue_vplmn_identifier;
    FDDictionaryEntryAVP m_avp_prose_event_type;
    FDDictionaryEntryAVP m_avp_vcs_information;
    FDDictionaryEntryAVP m_avp_cpdt_information;
    FDDictionaryEntryAVP m_avp_3gpp_ms_timezone;
    FDDictionaryEntryAVP m_avp_sgw_change;
    FDDictionaryEntryAVP m_avp_ip_can_type;
    FDDictionaryEntryAVP m_avp_optional_capability;
    FDDictionaryEntryAVP m_avp_tariff_time_change;
    FDDictionaryEntryAVP m_avp_cc_time;
    FDDictionaryEntryAVP m_avp_served_party_ip_address;
    FDDictionaryEntryAVP m_avp_envelope_reporting;
    FDDictionaryEntryAVP m_avp_prose_function_ip_address;
    FDDictionaryEntryAVP m_avp_reply_path_requested;
    FDDictionaryEntryAVP m_avp_software_version;
    FDDictionaryEntryAVP m_avp_uplink_rate_limit;
    FDDictionaryEntryAVP m_avp_vendor_id;
    FDDictionaryEntryAVP m_avp_unit_value;
    FDDictionaryEntryAVP m_avp_destination_realm;
    FDDictionaryEntryAVP m_avp_number_of_messages_sent;
    FDDictionaryEntryAVP m_avp_role_of_prose_function;

};

//
// DIAMETER REQUEST Processing - AccountingRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// ACR request from Clients.
//-
class ACRcmd : public FDCommandRequest
{
public:
	ACRcmd( Dictionary &dict );
   ~ACRcmd();

   virtual int process( FDMessageRequest *req );

private:
   ACRcmd();

   Dictionary &m_dict;
};

//
// DIAMETER REQUEST - AccountingRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a ACR
// to the Diameter application and process the corresponding answer.
//
class ACRreq : public FDMessageRequest
{
public:
	ACRreq( Dictionary &dict );
   ~ACRreq();

   void processAnswer( FDMessageAnswer &ans );

   const std::string &getSessionId() { return m_session.getSessionId(); }
private:
   ACRreq();

   Dictionary m_dict;
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
//       example: class AccountingRequestExtractor
//    Multi-Occurance grouped AVP corresponds to a class that is derived from FDExtractorList
//       example: class SubscriptionIdExtractorList
//
class ScaleFactorExtractor : public FDExtractor
{
 public:
    ScaleFactorExtractor( FDExtractor &parent, Dictionary &dict );
    ~ScaleFactorExtractor();

    FDExtractorAvp value_digits;
    FDExtractorAvp exponent;
};

class SdpMediaComponentExtractor : public FDExtractor
{
 public:
    SdpMediaComponentExtractor( FDExtractor &parent, Dictionary &dict );
    ~SdpMediaComponentExtractor();

    FDExtractorAvp sdp_media_name;
    FDExtractorAvpList sdp_media_description;
    FDExtractorAvp local_gw_inserted_indication;
    FDExtractorAvp ip_realm_default_indication;
    FDExtractorAvp transcoder_inserted_indication;
    FDExtractorAvp media_initiator_flag;
    FDExtractorAvp media_initiator_party;
    FDExtractorAvp tgpp_charging_id;
    FDExtractorAvp access_network_charging_identifier_value;
    FDExtractorAvp sdp_type;
};

class UwanUserLocationInfoExtractor : public FDExtractor
{
 public:
    UwanUserLocationInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~UwanUserLocationInfoExtractor();

    FDExtractorAvp ue_local_ip_address;
    FDExtractorAvp udp_source_port;
    FDExtractorAvp ssid;
    FDExtractorAvp bssid;
};

class AddressDomainExtractor : public FDExtractor
{
 public:
    AddressDomainExtractor( FDExtractor &parent, Dictionary &dict );
    ~AddressDomainExtractor();

    FDExtractorAvp domain_name;
    FDExtractorAvp tgpp_imsi_mcc_mnc;
};

class SubscriptionIdExtractor : public FDExtractor
{
 public:
    SubscriptionIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~SubscriptionIdExtractor();

    FDExtractorAvp subscription_id_type;
    FDExtractorAvp subscription_id_data;
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

class AccumulatedCostExtractor : public FDExtractor
{
 public:
    AccumulatedCostExtractor( FDExtractor &parent, Dictionary &dict );
    ~AccumulatedCostExtractor();

    FDExtractorAvp value_digits;
    FDExtractorAvp exponent;
};

class IncrementalCostExtractor : public FDExtractor
{
 public:
    IncrementalCostExtractor( FDExtractor &parent, Dictionary &dict );
    ~IncrementalCostExtractor();

    FDExtractorAvp value_digits;
    FDExtractorAvp exponent;
};

class IncrementalCostExtractorList : public FDExtractorList
{
 public:
    IncrementalCostExtractorList( FDExtractor &parent, Dictionary &dict );
    ~IncrementalCostExtractorList();

    FDExtractor *createExtractor() { return new IncrementalCostExtractor( getParent(), m_dict ); }
    std::list<IncrementalCostExtractor*> &getList() { return (std::list<IncrementalCostExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AocCostInformationExtractor : public FDExtractor
{
 public:
    AocCostInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~AocCostInformationExtractor();

    AccumulatedCostExtractor accumulated_cost;
    IncrementalCostExtractorList incremental_cost;
    FDExtractorAvp currency_code;
};

class RateElementExtractor : public FDExtractor
{
 public:
    RateElementExtractor( FDExtractor &parent, Dictionary &dict );
    ~RateElementExtractor();

    FDExtractorAvp cc_unit_type;
};

class RateElementExtractorList : public FDExtractorList
{
 public:
    RateElementExtractorList( FDExtractor &parent, Dictionary &dict );
    ~RateElementExtractorList();

    FDExtractor *createExtractor() { return new RateElementExtractor( getParent(), m_dict ); }
    std::list<RateElementExtractor*> &getList() { return (std::list<RateElementExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class CurrentTariffExtractor : public FDExtractor
{
 public:
    CurrentTariffExtractor( FDExtractor &parent, Dictionary &dict );
    ~CurrentTariffExtractor();

    FDExtractorAvp currency_code;
    ScaleFactorExtractor scale_factor;
    RateElementExtractorList rate_element;
};

class NextTariffExtractor : public FDExtractor
{
 public:
    NextTariffExtractor( FDExtractor &parent, Dictionary &dict );
    ~NextTariffExtractor();

    FDExtractorAvp currency_code;
    ScaleFactorExtractor scale_factor;
    RateElementExtractorList rate_element;
};

class TariffInformationExtractor : public FDExtractor
{
 public:
    TariffInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~TariffInformationExtractor();

    CurrentTariffExtractor current_tariff;
    FDExtractorAvp tariff_time_change;
    NextTariffExtractor next_tariff;
};

class AocServiceExtractor : public FDExtractor
{
 public:
    AocServiceExtractor( FDExtractor &parent, Dictionary &dict );
    ~AocServiceExtractor();

    FDExtractorAvp aoc_service_obligatory_type;
    FDExtractorAvp aoc_service_type;
};

class AocServiceExtractorList : public FDExtractorList
{
 public:
    AocServiceExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AocServiceExtractorList();

    FDExtractor *createExtractor() { return new AocServiceExtractor( getParent(), m_dict ); }
    std::list<AocServiceExtractor*> &getList() { return (std::list<AocServiceExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AocSubscriptionInformationExtractor : public FDExtractor
{
 public:
    AocSubscriptionInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~AocSubscriptionInformationExtractor();

    AocServiceExtractorList aoc_service;
    FDExtractorAvp aoc_format;
    FDExtractorAvp preferred_aoc_currency;
};

class AocInformationExtractor : public FDExtractor
{
 public:
    AocInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~AocInformationExtractor();

    AocCostInformationExtractor aoc_cost_information;
    TariffInformationExtractor tariff_information;
    AocSubscriptionInformationExtractor aoc_subscription_information;
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

class UserCsgInformationExtractor : public FDExtractor
{
 public:
    UserCsgInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~UserCsgInformationExtractor();

    FDExtractorAvp csg_id;
    FDExtractorAvp csg_access_mode;
    FDExtractorAvp csg_membership_indication;
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

class TwanUserLocationInfoExtractor : public FDExtractor
{
 public:
    TwanUserLocationInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~TwanUserLocationInfoExtractor();

    FDExtractorAvp ssid;
    FDExtractorAvp bssid;
};

class PsFurnishChargingInformationExtractor : public FDExtractor
{
 public:
    PsFurnishChargingInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~PsFurnishChargingInformationExtractor();

    FDExtractorAvp tgpp_charging_id;
    FDExtractorAvp ps_free_format_data;
    FDExtractorAvp ps_append_free_format_data;
};

class TimeQuotaMechanismExtractor : public FDExtractor
{
 public:
    TimeQuotaMechanismExtractor( FDExtractor &parent, Dictionary &dict );
    ~TimeQuotaMechanismExtractor();

    FDExtractorAvp time_quota_type;
    FDExtractorAvp base_time_interval;
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

class MultipleServicesCreditControlExtractor : public FDExtractor
{
 public:
    MultipleServicesCreditControlExtractor( FDExtractor &parent, Dictionary &dict );
    ~MultipleServicesCreditControlExtractor();

    GrantedServiceUnitExtractor granted_service_unit;
};

class MultipleServicesCreditControlExtractorList : public FDExtractorList
{
 public:
    MultipleServicesCreditControlExtractorList( FDExtractor &parent, Dictionary &dict );
    ~MultipleServicesCreditControlExtractorList();

    FDExtractor *createExtractor() { return new MultipleServicesCreditControlExtractor( getParent(), m_dict ); }
    std::list<MultipleServicesCreditControlExtractor*> &getList() { return (std::list<MultipleServicesCreditControlExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class OfflineChargingExtractor : public FDExtractor
{
 public:
    OfflineChargingExtractor( FDExtractor &parent, Dictionary &dict );
    ~OfflineChargingExtractor();

    FDExtractorAvp quota_consumption_time;
    TimeQuotaMechanismExtractor time_quota_mechanism;
    FDExtractorAvp envelope_reporting;
    MultipleServicesCreditControlExtractorList multiple_services_credit_control;
};

class TrafficDataVolumesExtractor : public FDExtractor
{
 public:
    TrafficDataVolumesExtractor( FDExtractor &parent, Dictionary &dict );
    ~TrafficDataVolumesExtractor();

    QosInformationExtractor qos_information;
    FDExtractorAvp accounting_input_octets;
    FDExtractorAvp accounting_output_octets;
    FDExtractorAvp change_condition;
    FDExtractorAvp change_time;
    FDExtractorAvp tgpp_user_location_info;
    UwanUserLocationInfoExtractor uwan_user_location_info;
    FDExtractorAvp tgpp_charging_id;
    FDExtractorAvp presence_reporting_area_status;
    UserCsgInformationExtractor user_csg_information;
    FDExtractorAvp tgpp_rat_type;
};

class TrafficDataVolumesExtractorList : public FDExtractorList
{
 public:
    TrafficDataVolumesExtractorList( FDExtractor &parent, Dictionary &dict );
    ~TrafficDataVolumesExtractorList();

    FDExtractor *createExtractor() { return new TrafficDataVolumesExtractor( getParent(), m_dict ); }
    std::list<TrafficDataVolumesExtractor*> &getList() { return (std::list<TrafficDataVolumesExtractor*>&)FDExtractorList::getList(); }

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

class AfCorrelationInformationExtractor : public FDExtractor
{
 public:
    AfCorrelationInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~AfCorrelationInformationExtractor();

    FDExtractorAvp af_charging_identifier;
    FlowsExtractorList flows;
};

class ServiceSpecificInfoExtractor : public FDExtractor
{
 public:
    ServiceSpecificInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServiceSpecificInfoExtractor();

    FDExtractorAvp service_specific_data;
};

class RelatedChangeConditionInformationExtractor : public FDExtractor
{
 public:
    RelatedChangeConditionInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~RelatedChangeConditionInformationExtractor();

    FDExtractorAvp sgsn_address;
    FDExtractorAvpList change_condition;
    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp tgpp2_bsid;
    UwanUserLocationInfoExtractor uwan_user_location_info;
    FDExtractorAvp presence_reporting_area_status;
    UserCsgInformationExtractor user_csg_information;
    FDExtractorAvp tgpp_rat_type;
};

class ServingPlmnRateControlExtractor : public FDExtractor
{
 public:
    ServingPlmnRateControlExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServingPlmnRateControlExtractor();

    FDExtractorAvp uplink_rate_limit;
    FDExtractorAvp downlink_rate_limit;
};

class ApnRateControlUplinkExtractor : public FDExtractor
{
 public:
    ApnRateControlUplinkExtractor( FDExtractor &parent, Dictionary &dict );
    ~ApnRateControlUplinkExtractor();

    FDExtractorAvp additional_exception_reports;
    FDExtractorAvp rate_control_time_unit;
    FDExtractorAvp rate_control_max_rate;
};

class ApnRateControlDownlinkExtractor : public FDExtractor
{
 public:
    ApnRateControlDownlinkExtractor( FDExtractor &parent, Dictionary &dict );
    ~ApnRateControlDownlinkExtractor();

    FDExtractorAvp rate_control_time_unit;
    FDExtractorAvp rate_control_max_rate;
    FDExtractorAvp rate_control_max_message_size;
};

class ApnRateControlExtractor : public FDExtractor
{
 public:
    ApnRateControlExtractor( FDExtractor &parent, Dictionary &dict );
    ~ApnRateControlExtractor();

    ApnRateControlUplinkExtractor apn_rate_control_uplink;
    ApnRateControlDownlinkExtractor apn_rate_control_downlink;
};

class ServiceDataContainerExtractor : public FDExtractor
{
 public:
    ServiceDataContainerExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServiceDataContainerExtractor();

    AfCorrelationInformationExtractor af_correlation_information;
    FDExtractorAvp charging_rule_base_name;
    FDExtractorAvp accounting_input_octets;
    FDExtractorAvp accounting_output_octets;
    FDExtractorAvp local_sequence_number;
    QosInformationExtractor qos_information;
    FDExtractorAvp rating_group;
    FDExtractorAvp change_time;
    FDExtractorAvp service_identifier;
    ServiceSpecificInfoExtractor service_specific_info;
    FDExtractorAvp adc_rule_base_name;
    FDExtractorAvp sgsn_address;
    FDExtractorAvp time_first_usage;
    FDExtractorAvp time_last_usage;
    FDExtractorAvp time_usage;
    FDExtractorAvpList change_condition;
    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp tgpp2_bsid;
    UwanUserLocationInfoExtractor uwan_user_location_info;
    FDExtractorAvp sponsor_identity;
    FDExtractorAvp application_service_provider_identity;
    PresenceReportingAreaInformationExtractorList presence_reporting_area_information;
    FDExtractorAvp presence_reporting_area_status;
    UserCsgInformationExtractor user_csg_information;
    FDExtractorAvp tgpp_rat_type;
    RelatedChangeConditionInformationExtractor related_change_condition_information;
    ServingPlmnRateControlExtractor serving_plmn_rate_control;
    ApnRateControlExtractor apn_rate_control;
    FDExtractorAvp tgpp_ps_data_off_status;
};

class ServiceDataContainerExtractorList : public FDExtractorList
{
 public:
    ServiceDataContainerExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ServiceDataContainerExtractorList();

    FDExtractor *createExtractor() { return new ServiceDataContainerExtractor( getParent(), m_dict ); }
    std::list<ServiceDataContainerExtractor*> &getList() { return (std::list<ServiceDataContainerExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class UserEquipmentInfoExtractor : public FDExtractor
{
 public:
    UserEquipmentInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~UserEquipmentInfoExtractor();

    FDExtractorAvp user_equipment_info_type;
    FDExtractorAvp user_equipment_info_value;
};

class TerminalInformationExtractor : public FDExtractor
{
 public:
    TerminalInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~TerminalInformationExtractor();

    FDExtractorAvp imei;
    FDExtractorAvp software_version;
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

class EnhancedDiagnosticsExtractor : public FDExtractor
{
 public:
    EnhancedDiagnosticsExtractor( FDExtractor &parent, Dictionary &dict );
    ~EnhancedDiagnosticsExtractor();

    FDExtractorAvp ran_nas_release_cause;
};

class RrcCauseCounterExtractor : public FDExtractor
{
 public:
    RrcCauseCounterExtractor( FDExtractor &parent, Dictionary &dict );
    ~RrcCauseCounterExtractor();

    FDExtractorAvp counter_value;
    FDExtractorAvp rrc_counter_timestamp;
};

class ScsAsAddressExtractor : public FDExtractor
{
 public:
    ScsAsAddressExtractor( FDExtractor &parent, Dictionary &dict );
    ~ScsAsAddressExtractor();

    FDExtractorAvp scs_realm;
    FDExtractorAvp scs_address;
};

class PsInformationExtractor : public FDExtractor
{
 public:
    PsInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~PsInformationExtractor();

    FDExtractorAvp tgpp_charging_id;
    FDExtractorAvp pdn_connection_charging_id;
    FDExtractorAvp node_id;
    FDExtractorAvp tgpp_pdp_type;
    FDExtractorAvpList pdp_address;
    FDExtractorAvp pdp_address_prefix_length;
    FDExtractorAvp dynamic_address_flag;
    FDExtractorAvp dynamic_address_flag_extension;
    QosInformationExtractor qos_information;
    FDExtractorAvpList sgsn_address;
    FDExtractorAvpList ggsn_address;
    FDExtractorAvpList tdf_ip_address;
    FDExtractorAvpList sgw_address;
    FDExtractorAvpList epdg_address;
    FDExtractorAvpList twag_address;
    FDExtractorAvp cg_address;
    FDExtractorAvp serving_node_type;
    FDExtractorAvp sgw_change;
    FDExtractorAvp tgpp_imsi_mcc_mnc;
    FDExtractorAvp imsi_unauthenticated_flag;
    FDExtractorAvp tgpp_ggsn_mcc_mnc;
    FDExtractorAvp tgpp_nsapi;
    FDExtractorAvp called_station_id;
    FDExtractorAvp tgpp_selection_mode;
    FDExtractorAvp tgpp_charging_characteristics;
    FDExtractorAvp charging_characteristics_selection_mode;
    FDExtractorAvp tgpp_sgsn_mcc_mnc;
    FDExtractorAvp tgpp_ms_timezone;
    FDExtractorAvp charging_rule_base_name;
    FDExtractorAvp adc_rule_base_name;
    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp user_location_info_time;
    UserCsgInformationExtractor user_csg_information;
    PresenceReportingAreaInformationExtractorList presence_reporting_area_information;
    FDExtractorAvp tgpp2_bsid;
    TwanUserLocationInfoExtractor twan_user_location_info;
    UwanUserLocationInfoExtractor uwan_user_location_info;
    FDExtractorAvp tgpp_rat_type;
    PsFurnishChargingInformationExtractor ps_furnish_charging_information;
    FDExtractorAvp pdp_context_type;
    OfflineChargingExtractor offline_charging;
    TrafficDataVolumesExtractorList traffic_data_volumes;
    ServiceDataContainerExtractorList service_data_container;
    UserEquipmentInfoExtractor user_equipment_info;
    TerminalInformationExtractor terminal_information;
    FDExtractorAvp start_time;
    FDExtractorAvp stop_time;
    FDExtractorAvp change_condition;
    FDExtractorAvp diagnostics;
    FDExtractorAvp low_priority_indicator;
    FDExtractorAvp nbifom_mode;
    FDExtractorAvp nbifom_support;
    FDExtractorAvp mme_number_for_mt_sms;
    FDExtractorAvp mme_name;
    FDExtractorAvp mme_realm;
    FDExtractorAvp logical_access_id;
    FDExtractorAvp physical_access_id;
    FixedUserLocationInfoExtractor fixed_user_location_info;
    FDExtractorAvp cn_operator_selection_entity;
    EnhancedDiagnosticsExtractor enhanced_diagnostics;
    FDExtractorAvp sgi_ptp_tunnelling_method;
    FDExtractorAvp cp_ciot_eps_optimisation_indicator;
    FDExtractorAvp uni_pdu_cp_only_flag;
    ServingPlmnRateControlExtractor serving_plmn_rate_control;
    ApnRateControlExtractor apn_rate_control;
    FDExtractorAvp charging_per_ip_can_session_indicator;
    RrcCauseCounterExtractor rrc_cause_counter;
    FDExtractorAvp tgpp_ps_data_off_status;
    ScsAsAddressExtractor scs_as_address;
};

class EventTypeExtractor : public FDExtractor
{
 public:
    EventTypeExtractor( FDExtractor &parent, Dictionary &dict );
    ~EventTypeExtractor();

    FDExtractorAvp sip_method;
    FDExtractorAvp event;
    FDExtractorAvp expires;
};

class CalledIdentityChangeExtractor : public FDExtractor
{
 public:
    CalledIdentityChangeExtractor( FDExtractor &parent, Dictionary &dict );
    ~CalledIdentityChangeExtractor();

    FDExtractorAvp called_identity;
    FDExtractorAvp change_time;
};

class TimeStampsExtractor : public FDExtractor
{
 public:
    TimeStampsExtractor( FDExtractor &parent, Dictionary &dict );
    ~TimeStampsExtractor();

    FDExtractorAvp sip_request_timestamp;
};

class ApplicationServerInformationExtractor : public FDExtractor
{
 public:
    ApplicationServerInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~ApplicationServerInformationExtractor();

    FDExtractorAvp application_server;
};

class ApplicationServerInformationExtractorList : public FDExtractorList
{
 public:
    ApplicationServerInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ApplicationServerInformationExtractorList();

    FDExtractor *createExtractor() { return new ApplicationServerInformationExtractor( getParent(), m_dict ); }
    std::list<ApplicationServerInformationExtractor*> &getList() { return (std::list<ApplicationServerInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class InterOperatorIdentifierExtractor : public FDExtractor
{
 public:
    InterOperatorIdentifierExtractor( FDExtractor &parent, Dictionary &dict );
    ~InterOperatorIdentifierExtractor();

    FDExtractorAvp originating_ioi;
    FDExtractorAvp terminating_ioi;
};

class InterOperatorIdentifierExtractorList : public FDExtractorList
{
 public:
    InterOperatorIdentifierExtractorList( FDExtractor &parent, Dictionary &dict );
    ~InterOperatorIdentifierExtractorList();

    FDExtractor *createExtractor() { return new InterOperatorIdentifierExtractor( getParent(), m_dict ); }
    std::list<InterOperatorIdentifierExtractor*> &getList() { return (std::list<InterOperatorIdentifierExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class SdpMediaComponentExtractorList : public FDExtractorList
{
 public:
    SdpMediaComponentExtractorList( FDExtractor &parent, Dictionary &dict );
    ~SdpMediaComponentExtractorList();

    FDExtractor *createExtractor() { return new SdpMediaComponentExtractor( getParent(), m_dict ); }
    std::list<SdpMediaComponentExtractor*> &getList() { return (std::list<SdpMediaComponentExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ServerCapabilitiesExtractor : public FDExtractor
{
 public:
    ServerCapabilitiesExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServerCapabilitiesExtractor();

    FDExtractorAvpList mandatory_capability;
    FDExtractorAvpList optional_capability;
    FDExtractorAvpList server_name;
};

class TrunkGroupIdExtractor : public FDExtractor
{
 public:
    TrunkGroupIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~TrunkGroupIdExtractor();

    FDExtractorAvp incoming_trunk_group_id;
};

class ServiceSpecificInfoExtractorList : public FDExtractorList
{
 public:
    ServiceSpecificInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ServiceSpecificInfoExtractorList();

    FDExtractor *createExtractor() { return new ServiceSpecificInfoExtractor( getParent(), m_dict ); }
    std::list<ServiceSpecificInfoExtractor*> &getList() { return (std::list<ServiceSpecificInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class MessageBodyExtractor : public FDExtractor
{
 public:
    MessageBodyExtractor( FDExtractor &parent, Dictionary &dict );
    ~MessageBodyExtractor();

    FDExtractorAvp content_type;
};

class MessageBodyExtractorList : public FDExtractorList
{
 public:
    MessageBodyExtractorList( FDExtractor &parent, Dictionary &dict );
    ~MessageBodyExtractorList();

    FDExtractor *createExtractor() { return new MessageBodyExtractor( getParent(), m_dict ); }
    std::list<MessageBodyExtractor*> &getList() { return (std::list<MessageBodyExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class SdpTimestampsExtractor : public FDExtractor
{
 public:
    SdpTimestampsExtractor( FDExtractor &parent, Dictionary &dict );
    ~SdpTimestampsExtractor();

    FDExtractorAvp sdp_offer_timestamp;
    FDExtractorAvp sdp_answer_timestamp;
};

class EarlyMediaDescriptionExtractor : public FDExtractor
{
 public:
    EarlyMediaDescriptionExtractor( FDExtractor &parent, Dictionary &dict );
    ~EarlyMediaDescriptionExtractor();

    SdpTimestampsExtractor sdp_timestamps;
    SdpMediaComponentExtractorList sdp_media_component;
    FDExtractorAvpList sdp_session_description;
};

class EarlyMediaDescriptionExtractorList : public FDExtractorList
{
 public:
    EarlyMediaDescriptionExtractorList( FDExtractor &parent, Dictionary &dict );
    ~EarlyMediaDescriptionExtractorList();

    FDExtractor *createExtractor() { return new EarlyMediaDescriptionExtractor( getParent(), m_dict ); }
    std::list<EarlyMediaDescriptionExtractor*> &getList() { return (std::list<EarlyMediaDescriptionExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class RealTimeTariffInformationExtractor : public FDExtractor
{
 public:
    RealTimeTariffInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~RealTimeTariffInformationExtractor();

    TariffInformationExtractor tariff_information;
    FDExtractorAvp tariff_xml;
};

class NniInformationExtractor : public FDExtractor
{
 public:
    NniInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~NniInformationExtractor();

    FDExtractorAvp session_direction;
    FDExtractorAvp nni_type;
    FDExtractorAvp relationship_mode;
    FDExtractorAvp neighbour_node_address;
};

class NniInformationExtractorList : public FDExtractorList
{
 public:
    NniInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~NniInformationExtractorList();

    FDExtractor *createExtractor() { return new NniInformationExtractor( getParent(), m_dict ); }
    std::list<NniInformationExtractor*> &getList() { return (std::list<NniInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AccessNetworkInfoChangeExtractor : public FDExtractor
{
 public:
    AccessNetworkInfoChangeExtractor( FDExtractor &parent, Dictionary &dict );
    ~AccessNetworkInfoChangeExtractor();

    FDExtractorAvpList access_network_information;
    FDExtractorAvp cellular_network_information;
    FDExtractorAvp change_time;
};

class AccessNetworkInfoChangeExtractorList : public FDExtractorList
{
 public:
    AccessNetworkInfoChangeExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AccessNetworkInfoChangeExtractorList();

    FDExtractor *createExtractor() { return new AccessNetworkInfoChangeExtractor( getParent(), m_dict ); }
    std::list<AccessNetworkInfoChangeExtractor*> &getList() { return (std::list<AccessNetworkInfoChangeExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AccessTransferInformationExtractor : public FDExtractor
{
 public:
    AccessTransferInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~AccessTransferInformationExtractor();

    FDExtractorAvp access_transfer_type;
    FDExtractorAvpList access_network_information;
    FDExtractorAvp cellular_network_information;
    FDExtractorAvp inter_ue_transfer;
    UserEquipmentInfoExtractor user_equipment_info;
    FDExtractorAvp instance_id;
    FDExtractorAvp related_ims_charging_identifier;
    FDExtractorAvp related_ims_charging_identifier_node;
    FDExtractorAvp change_time;
};

class AccessTransferInformationExtractorList : public FDExtractorList
{
 public:
    AccessTransferInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AccessTransferInformationExtractorList();

    FDExtractor *createExtractor() { return new AccessTransferInformationExtractor( getParent(), m_dict ); }
    std::list<AccessTransferInformationExtractor*> &getList() { return (std::list<AccessTransferInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ImsInformationExtractor : public FDExtractor
{
 public:
    ImsInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~ImsInformationExtractor();

    EventTypeExtractor event_type;
    FDExtractorAvp role_of_node;
    FDExtractorAvp node_functionality;
    FDExtractorAvp user_session_id;
    FDExtractorAvp outgoing_session_id;
    FDExtractorAvp session_priority;
    FDExtractorAvpList calling_party_address;
    FDExtractorAvp called_party_address;
    FDExtractorAvpList called_asserted_identity;
    CalledIdentityChangeExtractor called_identity_change;
    FDExtractorAvp number_portability_routing_information;
    FDExtractorAvp carrier_select_routing_information;
    FDExtractorAvp alternate_charged_party_address;
    FDExtractorAvpList requested_party_address;
    FDExtractorAvpList associated_uri;
    TimeStampsExtractor time_stamps;
    ApplicationServerInformationExtractorList application_server_information;
    InterOperatorIdentifierExtractorList inter_operator_identifier;
    FDExtractorAvpList transit_ioi_list;
    FDExtractorAvp ims_charging_identifier;
    FDExtractorAvpList sdp_session_description;
    SdpMediaComponentExtractorList sdp_media_component;
    FDExtractorAvp served_party_ip_address;
    ServerCapabilitiesExtractor server_capabilities;
    TrunkGroupIdExtractor trunk_group_id;
    FDExtractorAvp bearer_service;
    FDExtractorAvp service_id;
    ServiceSpecificInfoExtractorList service_specific_info;
    MessageBodyExtractorList message_body;
    FDExtractorAvp cause_code;
    FDExtractorAvpList reason_header;
    FDExtractorAvpList access_network_information;
    FDExtractorAvp cellular_network_information;
    EarlyMediaDescriptionExtractorList early_media_description;
    FDExtractorAvp ims_communication_service_identifier;
    FDExtractorAvp ims_application_reference_identifier;
    FDExtractorAvp online_charging_flag;
    RealTimeTariffInformationExtractor real_time_tariff_information;
    FDExtractorAvp account_expiration;
    FDExtractorAvp initial_ims_charging_identifier;
    NniInformationExtractorList nni_information;
    FDExtractorAvp from_address;
    FDExtractorAvp ims_emergency_indicator;
    FDExtractorAvp ims_visited_network_identifier;
    AccessNetworkInfoChangeExtractorList access_network_info_change;
    AccessTransferInformationExtractorList access_transfer_information;
    FDExtractorAvp related_ims_charging_identifier;
    FDExtractorAvp related_ims_charging_identifier_node;
    FDExtractorAvp route_header_received;
    FDExtractorAvp route_header_transmitted;
    FDExtractorAvp instance_id;
    FDExtractorAvp tad_identifier;
};

class OriginatorAddressExtractor : public FDExtractor
{
 public:
    OriginatorAddressExtractor( FDExtractor &parent, Dictionary &dict );
    ~OriginatorAddressExtractor();

    FDExtractorAvp address_type;
    FDExtractorAvp address_data;
    AddressDomainExtractor address_domain;
};

class RecipientAddressExtractor : public FDExtractor
{
 public:
    RecipientAddressExtractor( FDExtractor &parent, Dictionary &dict );
    ~RecipientAddressExtractor();

    FDExtractorAvp address_type;
    FDExtractorAvp address_data;
    AddressDomainExtractor address_domain;
    FDExtractorAvp addressee_type;
};

class RecipientAddressExtractorList : public FDExtractorList
{
 public:
    RecipientAddressExtractorList( FDExtractor &parent, Dictionary &dict );
    ~RecipientAddressExtractorList();

    FDExtractor *createExtractor() { return new RecipientAddressExtractor( getParent(), m_dict ); }
    std::list<RecipientAddressExtractor*> &getList() { return (std::list<RecipientAddressExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AdditionalContentInformationExtractor : public FDExtractor
{
 public:
    AdditionalContentInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~AdditionalContentInformationExtractor();

    FDExtractorAvp type_number;
    FDExtractorAvp additional_type_information;
    FDExtractorAvp content_size;
};

class AdditionalContentInformationExtractorList : public FDExtractorList
{
 public:
    AdditionalContentInformationExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AdditionalContentInformationExtractorList();

    FDExtractor *createExtractor() { return new AdditionalContentInformationExtractor( getParent(), m_dict ); }
    std::list<AdditionalContentInformationExtractor*> &getList() { return (std::list<AdditionalContentInformationExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class MmContentTypeExtractor : public FDExtractor
{
 public:
    MmContentTypeExtractor( FDExtractor &parent, Dictionary &dict );
    ~MmContentTypeExtractor();

    FDExtractorAvp type_number;
    FDExtractorAvp additional_type_information;
    FDExtractorAvp content_size;
    AdditionalContentInformationExtractorList additional_content_information;
};

class MessageClassExtractor : public FDExtractor
{
 public:
    MessageClassExtractor( FDExtractor &parent, Dictionary &dict );
    ~MessageClassExtractor();

    FDExtractorAvp class_identifier;
    FDExtractorAvp token_text;
};

class MmsInformationExtractor : public FDExtractor
{
 public:
    MmsInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~MmsInformationExtractor();

    OriginatorAddressExtractor originator_address;
    RecipientAddressExtractorList recipient_address;
    FDExtractorAvp submission_time;
    MmContentTypeExtractor mm_content_type;
    FDExtractorAvp priority;
    FDExtractorAvp message_id;
    FDExtractorAvp message_type;
    FDExtractorAvp message_size;
    MessageClassExtractor message_class;
    FDExtractorAvp delivery_report_requested;
    FDExtractorAvp read_reply_report_requested;
    FDExtractorAvp mmbox_storage_requested;
    FDExtractorAvp applic_id;
    FDExtractorAvp reply_applic_id;
    FDExtractorAvp aux_applic_info;
    FDExtractorAvp content_class;
    FDExtractorAvp drm_content;
    FDExtractorAvp adaptations;
};

class LcsClientNameExtractor : public FDExtractor
{
 public:
    LcsClientNameExtractor( FDExtractor &parent, Dictionary &dict );
    ~LcsClientNameExtractor();

    FDExtractorAvp lcs_data_coding_scheme;
    FDExtractorAvp lcs_name_string;
    FDExtractorAvp lcs_format_indicator;
};

class LcsRequestorIdExtractor : public FDExtractor
{
 public:
    LcsRequestorIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~LcsRequestorIdExtractor();

    FDExtractorAvp lcs_data_coding_scheme;
    FDExtractorAvp lcs_requestor_id_string;
};

class LcsClientIdExtractor : public FDExtractor
{
 public:
    LcsClientIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~LcsClientIdExtractor();

    FDExtractorAvp lcs_client_type;
    FDExtractorAvp lcs_client_external_id;
    FDExtractorAvp lcs_client_dialed_by_ms;
    LcsClientNameExtractor lcs_client_name;
    FDExtractorAvp lcs_apn;
    LcsRequestorIdExtractor lcs_requestor_id;
};

class LocationTypeExtractor : public FDExtractor
{
 public:
    LocationTypeExtractor( FDExtractor &parent, Dictionary &dict );
    ~LocationTypeExtractor();

    FDExtractorAvp location_estimate_type;
    FDExtractorAvp deferred_location_event_type;
};

class LcsInformationExtractor : public FDExtractor
{
 public:
    LcsInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~LcsInformationExtractor();

    LcsClientIdExtractor lcs_client_id;
    LocationTypeExtractor location_type;
    FDExtractorAvp location_estimate;
    FDExtractorAvp positioning_data;
    FDExtractorAvp tgpp_imsi;
    FDExtractorAvp msisdn;
};

class PocInformationExtractor : public FDExtractor
{
 public:
    PocInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~PocInformationExtractor();

    FDExtractorAvp poc_server_role;
};

class MbmsInformationExtractor : public FDExtractor
{
 public:
    MbmsInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~MbmsInformationExtractor();

    FDExtractorAvp tmgi;
    FDExtractorAvp mbms_service_type;
    FDExtractorAvp mbms_user_service_type;
    FDExtractorAvp file_repair_supported;
    FDExtractorAvp required_mbms_bearer_capabilities;
    FDExtractorAvp mbms_2g_3g_indicator;
    FDExtractorAvp rai;
    FDExtractorAvpList mbms_service_area;
    FDExtractorAvp mbms_session_identity;
    FDExtractorAvp cn_ip_multicast_distribution;
    FDExtractorAvp mbms_gw_address;
    FDExtractorAvp mbms_charged_party;
    FDExtractorAvpList msisdn;
    FDExtractorAvp mbms_data_transfer_start;
    FDExtractorAvp mbms_data_transfer_stop;
};

class OriginatorInterfaceExtractor : public FDExtractor
{
 public:
    OriginatorInterfaceExtractor( FDExtractor &parent, Dictionary &dict );
    ~OriginatorInterfaceExtractor();

    FDExtractorAvp interface_id;
    FDExtractorAvp interface_text;
    FDExtractorAvp interface_port;
    FDExtractorAvp interface_type;
};

class DestinationInterfaceExtractor : public FDExtractor
{
 public:
    DestinationInterfaceExtractor( FDExtractor &parent, Dictionary &dict );
    ~DestinationInterfaceExtractor();

    FDExtractorAvp interface_id;
    FDExtractorAvp interface_text;
    FDExtractorAvp interface_port;
    FDExtractorAvp interface_type;
};

class RecipientReceivedAddressExtractor : public FDExtractor
{
 public:
    RecipientReceivedAddressExtractor( FDExtractor &parent, Dictionary &dict );
    ~RecipientReceivedAddressExtractor();

    FDExtractorAvp address_type;
    FDExtractorAvp address_data;
    AddressDomainExtractor address_domain;
};

class RecipientReceivedAddressExtractorList : public FDExtractorList
{
 public:
    RecipientReceivedAddressExtractorList( FDExtractor &parent, Dictionary &dict );
    ~RecipientReceivedAddressExtractorList();

    FDExtractor *createExtractor() { return new RecipientReceivedAddressExtractor( getParent(), m_dict ); }
    std::list<RecipientReceivedAddressExtractor*> &getList() { return (std::list<RecipientReceivedAddressExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class RecipientInfoExtractor : public FDExtractor
{
 public:
    RecipientInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~RecipientInfoExtractor();

    DestinationInterfaceExtractor destination_interface;
    RecipientAddressExtractorList recipient_address;
    RecipientReceivedAddressExtractorList recipient_received_address;
    FDExtractorAvp recipient_sccp_address;
    FDExtractorAvp sm_protocol_id;
};

class RecipientInfoExtractorList : public FDExtractorList
{
 public:
    RecipientInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~RecipientInfoExtractorList();

    FDExtractor *createExtractor() { return new RecipientInfoExtractor( getParent(), m_dict ); }
    std::list<RecipientInfoExtractor*> &getList() { return (std::list<RecipientInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class OriginatorReceivedAddressExtractor : public FDExtractor
{
 public:
    OriginatorReceivedAddressExtractor( FDExtractor &parent, Dictionary &dict );
    ~OriginatorReceivedAddressExtractor();

    FDExtractorAvp address_type;
    FDExtractorAvp address_data;
    AddressDomainExtractor address_domain;
};

class SmDeviceTriggerInformationExtractor : public FDExtractor
{
 public:
    SmDeviceTriggerInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~SmDeviceTriggerInformationExtractor();

    FDExtractorAvp mtc_iwf_address;
    FDExtractorAvp reference_number;
};

class SmsInformationExtractor : public FDExtractor
{
 public:
    SmsInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~SmsInformationExtractor();

    FDExtractorAvp sms_node;
    FDExtractorAvp client_address;
    FDExtractorAvp originator_sccp_address;
    FDExtractorAvp smsc_address;
    FDExtractorAvp data_coding_scheme;
    FDExtractorAvp sm_discharge_time;
    FDExtractorAvp sm_message_type;
    OriginatorInterfaceExtractor originator_interface;
    FDExtractorAvp sm_protocol_id;
    FDExtractorAvp reply_path_requested;
    FDExtractorAvp sm_status;
    FDExtractorAvp sm_user_data_header;
    FDExtractorAvp number_of_messages_sent;
    FDExtractorAvp sm_sequence_number;
    RecipientInfoExtractorList recipient_info;
    OriginatorReceivedAddressExtractor originator_received_address;
    FDExtractorAvp sm_service_type;
    FDExtractorAvp sms_result;
    FDExtractorAvp sm_device_trigger_indicator;
    SmDeviceTriggerInformationExtractor sm_device_trigger_information;
};

class VcsInformationExtractor : public FDExtractor
{
 public:
    VcsInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~VcsInformationExtractor();

    FDExtractorAvp bearer_capability;
    FDExtractorAvp network_call_reference_number;
    FDExtractorAvp msc_address;
};

class SupplementaryServiceExtractor : public FDExtractor
{
 public:
    SupplementaryServiceExtractor( FDExtractor &parent, Dictionary &dict );
    ~SupplementaryServiceExtractor();

    FDExtractorAvp mmtel_sservice_type;
    FDExtractorAvp service_mode;
};

class SupplementaryServiceExtractorList : public FDExtractorList
{
 public:
    SupplementaryServiceExtractorList( FDExtractor &parent, Dictionary &dict );
    ~SupplementaryServiceExtractorList();

    FDExtractor *createExtractor() { return new SupplementaryServiceExtractor( getParent(), m_dict ); }
    std::list<SupplementaryServiceExtractor*> &getList() { return (std::list<SupplementaryServiceExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class MmtelInformationExtractor : public FDExtractor
{
 public:
    MmtelInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~MmtelInformationExtractor();

    SupplementaryServiceExtractorList supplementary_service;
};

class LocationInfoExtractor : public FDExtractor
{
 public:
    LocationInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~LocationInfoExtractor();

    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp change_time;
};

class LocationInfoExtractorList : public FDExtractorList
{
 public:
    LocationInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~LocationInfoExtractorList();

    FDExtractor *createExtractor() { return new LocationInfoExtractor( getParent(), m_dict ); }
    std::list<LocationInfoExtractor*> &getList() { return (std::list<LocationInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class CoverageInfoExtractor : public FDExtractor
{
 public:
    CoverageInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~CoverageInfoExtractor();

    FDExtractorAvp coverage_status;
    FDExtractorAvp change_time;
    LocationInfoExtractorList location_info;
};

class CoverageInfoExtractorList : public FDExtractorList
{
 public:
    CoverageInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~CoverageInfoExtractorList();

    FDExtractor *createExtractor() { return new CoverageInfoExtractor( getParent(), m_dict ); }
    std::list<CoverageInfoExtractor*> &getList() { return (std::list<CoverageInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class RadioParameterSetInfoExtractor : public FDExtractor
{
 public:
    RadioParameterSetInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~RadioParameterSetInfoExtractor();

    FDExtractorAvp radio_parameter_set_values;
    FDExtractorAvp change_time;
};

class RadioParameterSetInfoExtractorList : public FDExtractorList
{
 public:
    RadioParameterSetInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~RadioParameterSetInfoExtractorList();

    FDExtractor *createExtractor() { return new RadioParameterSetInfoExtractor( getParent(), m_dict ); }
    std::list<RadioParameterSetInfoExtractor*> &getList() { return (std::list<RadioParameterSetInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class TransmitterInfoExtractor : public FDExtractor
{
 public:
    TransmitterInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~TransmitterInfoExtractor();

    FDExtractorAvp prose_source_ip_address;
    FDExtractorAvp prose_ue_id;
};

class TransmitterInfoExtractorList : public FDExtractorList
{
 public:
    TransmitterInfoExtractorList( FDExtractor &parent, Dictionary &dict );
    ~TransmitterInfoExtractorList();

    FDExtractor *createExtractor() { return new TransmitterInfoExtractor( getParent(), m_dict ); }
    std::list<TransmitterInfoExtractor*> &getList() { return (std::list<TransmitterInfoExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ProseDirectCommunicationTransmissionDataContainerExtractor : public FDExtractor
{
 public:
    ProseDirectCommunicationTransmissionDataContainerExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProseDirectCommunicationTransmissionDataContainerExtractor();

    FDExtractorAvp local_sequence_number;
    FDExtractorAvp coverage_status;
    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp accounting_output_octets;
    FDExtractorAvp change_time;
    FDExtractorAvp change_condition;
    FDExtractorAvp visited_plmn_id;
    FDExtractorAvp usage_information_report_sequence_number;
    FDExtractorAvp radio_resources_indicator;
    FDExtractorAvp radio_frequency;
};

class ProseDirectCommunicationTransmissionDataContainerExtractorList : public FDExtractorList
{
 public:
    ProseDirectCommunicationTransmissionDataContainerExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ProseDirectCommunicationTransmissionDataContainerExtractorList();

    FDExtractor *createExtractor() { return new ProseDirectCommunicationTransmissionDataContainerExtractor( getParent(), m_dict ); }
    std::list<ProseDirectCommunicationTransmissionDataContainerExtractor*> &getList() { return (std::list<ProseDirectCommunicationTransmissionDataContainerExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ProseDirectCommunicationReceptionDataContainerExtractor : public FDExtractor
{
 public:
    ProseDirectCommunicationReceptionDataContainerExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProseDirectCommunicationReceptionDataContainerExtractor();

    FDExtractorAvp local_sequence_number;
    FDExtractorAvp coverage_status;
    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp accounting_input_octets;
    FDExtractorAvp change_time;
    FDExtractorAvp change_condition;
    FDExtractorAvp visited_plmn_id;
    FDExtractorAvp usage_information_report_sequence_number;
    FDExtractorAvp radio_resources_indicator;
    FDExtractorAvp radio_frequency;
};

class ProseDirectCommunicationReceptionDataContainerExtractorList : public FDExtractorList
{
 public:
    ProseDirectCommunicationReceptionDataContainerExtractorList( FDExtractor &parent, Dictionary &dict );
    ~ProseDirectCommunicationReceptionDataContainerExtractorList();

    FDExtractor *createExtractor() { return new ProseDirectCommunicationReceptionDataContainerExtractor( getParent(), m_dict ); }
    std::list<ProseDirectCommunicationReceptionDataContainerExtractor*> &getList() { return (std::list<ProseDirectCommunicationReceptionDataContainerExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ProseInformationExtractor : public FDExtractor
{
 public:
    ProseInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProseInformationExtractor();

    FDExtractorAvp announcing_ue_hplmn_identifier;
    FDExtractorAvp announcing_ue_vplmn_identifier;
    FDExtractorAvp monitoring_ue_hplmn_identifier;
    FDExtractorAvp monitoring_ue_vplmn_identifier;
    FDExtractorAvp role_of_prose_function;
    FDExtractorAvp prose_app_id;
    FDExtractorAvp prose_3rd_party_application_id;
    FDExtractorAvp application_specific_data;
    FDExtractorAvp prose_event_type;
    FDExtractorAvp prose_direct_discovery_model;
    FDExtractorAvp prose_function_ip_address;
    FDExtractorAvp prose_function_id;
    FDExtractorAvp prose_validity_timer;
    FDExtractorAvp prose_role_of_ue;
    FDExtractorAvp prose_request_timestamp;
    FDExtractorAvp pc3_control_protocol_cause;
    FDExtractorAvp monitoring_ue_identifier;
    FDExtractorAvp requestor_plmn_identifier;
    FDExtractorAvp origin_app_layer_user_id;
    FDExtractorAvp wlan_link_layer_id;
    FDExtractorAvp requesting_epuid;
    FDExtractorAvp target_app_layer_user_id;
    FDExtractorAvp requested_plmn_identifier;
    FDExtractorAvp time_window;
    FDExtractorAvp prose_range_class;
    FDExtractorAvp proximity_alert_indication;
    FDExtractorAvp proximity_alert_timestamp;
    FDExtractorAvp proximity_cancellation_timestamp;
    FDExtractorAvp prose_reason_for_cancellation;
    FDExtractorAvp pc3_epc_control_protocol_cause;
    FDExtractorAvp prose_ue_id;
    FDExtractorAvp prose_source_ip_address;
    FDExtractorAvp layer_2_group_id;
    FDExtractorAvp prose_group_ip_multicast_address;
    CoverageInfoExtractorList coverage_info;
    RadioParameterSetInfoExtractorList radio_parameter_set_info;
    TransmitterInfoExtractorList transmitter_info;
    FDExtractorAvp time_first_transmission;
    FDExtractorAvp time_first_reception;
    ProseDirectCommunicationTransmissionDataContainerExtractorList prose_direct_communication_transmission_data_container;
    ProseDirectCommunicationReceptionDataContainerExtractorList prose_direct_communication_reception_data_container;
};

class CpdtInformationExtractor : public FDExtractor
{
 public:
    CpdtInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~CpdtInformationExtractor();

    FDExtractorAvp external_identifier;
    FDExtractorAvp scef_id;
};

class ServiceInformationExtractor : public FDExtractor
{
 public:
    ServiceInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~ServiceInformationExtractor();

    SubscriptionIdExtractorList subscription_id;
    AocInformationExtractor aoc_information;
    PsInformationExtractor ps_information;
    ImsInformationExtractor ims_information;
    MmsInformationExtractor mms_information;
    LcsInformationExtractor lcs_information;
    PocInformationExtractor poc_information;
    MbmsInformationExtractor mbms_information;
    SmsInformationExtractor sms_information;
    VcsInformationExtractor vcs_information;
    MmtelInformationExtractor mmtel_information;
    ProseInformationExtractor prose_information;
    CpdtInformationExtractor cpdt_information;
};

class FailedAvpExtractor : public FDExtractor
{
 public:
    FailedAvpExtractor( FDExtractor &parent, Dictionary &dict );
    ~FailedAvpExtractor();


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

class AccountingRequestExtractor : public FDExtractor
{
 public:
    AccountingRequestExtractor( Dictionary &dict );
    ~AccountingRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp accounting_record_type;
    FDExtractorAvp accounting_record_number;
    FDExtractorAvp acct_application_id;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp user_name;
    FDExtractorAvp destination_host;
    FDExtractorAvp accounting_sub_session_id;
    FDExtractorAvp acct_session_id;
    FDExtractorAvp acct_multi_session_id;
    FDExtractorAvp acct_interim_interval;
    FDExtractorAvp accounting_realtime_required;
    FDExtractorAvp origin_state_id;
    FDExtractorAvp event_timestamp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
    FDExtractorAvp service_context_id;
    ServiceInformationExtractor service_information;
};

class AccountingAnswerExtractor : public FDExtractor
{
 public:
    AccountingAnswerExtractor( Dictionary &dict );
    ~AccountingAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp result_code;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp accounting_record_type;
    FDExtractorAvp accounting_record_number;
    FDExtractorAvp acct_application_id;
    VendorSpecificApplicationIdExtractor vendor_specific_application_id;
    FDExtractorAvp user_name;
    FDExtractorAvp accounting_sub_session_id;
    FDExtractorAvp acct_session_id;
    FDExtractorAvp acct_multi_session_id;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    FDExtractorAvp acct_interim_interval;
    FailedAvpExtractor failed_avp;
    FDExtractorAvp accounting_realtime_required;
    FDExtractorAvp origin_state_id;
    FDExtractorAvp event_timestamp;
    ProxyInfoExtractorList proxy_info;
};


}

#endif // __RF_H

