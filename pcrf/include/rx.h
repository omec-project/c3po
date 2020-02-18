/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __RX_H
#define __RX_H

#include "fd.h"

namespace rx {

//
// Primary Objects
//

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    FDDictionaryEntryApplication &app()                              { return m_app; }

    FDDictionaryEntryCommand &cmdAAR()                              	{ return m_cmd_aar; }
    FDDictionaryEntryCommand &cmdAAA()                              	{ return m_cmd_aaa; }
    FDDictionaryEntryCommand &cmdREAR()                             	{ return m_cmd_rear; }
    FDDictionaryEntryCommand &cmdREAA()                             	{ return m_cmd_reaa; }
    FDDictionaryEntryCommand &cmdSETR()                             	{ return m_cmd_setr; }
    FDDictionaryEntryCommand &cmdSETA()                             	{ return m_cmd_seta; }
    FDDictionaryEntryCommand &cmdABSR()                             	{ return m_cmd_absr; }
    FDDictionaryEntryCommand &cmdABSA()                             	{ return m_cmd_absa; }

    FDDictionaryEntryVendor &vndETSI()                              	{ return m_vnd_Etsi; }
    FDDictionaryEntryVendor &vnd3GPP()                              	{ return m_vnd_3gpp; }

    FDDictionaryEntryAVP &avpMcpttIdentifier()                      	{ return m_avp_mcptt_identifier; }
    FDDictionaryEntryAVP &avpRedirectHostUsage()                    	{ return m_avp_redirect_host_usage; }
    FDDictionaryEntryAVP &avpAfRequestedData()                      	{ return m_avp_af_requested_data; }
    FDDictionaryEntryAVP &avpRedirectMaxCacheTime()                 	{ return m_avp_redirect_max_cache_time; }
    FDDictionaryEntryAVP &avpTerminationCause()                     	{ return m_avp_termination_cause; }
    FDDictionaryEntryAVP &avpExponent()                             	{ return m_avp_exponent; }
    FDDictionaryEntryAVP &avpSourceid()                             	{ return m_avp_sourceid; }
    FDDictionaryEntryAVP &avpLoadValue()                            	{ return m_avp_load_value; }
    FDDictionaryEntryAVP &avpAccessNetworkChargingIdentifierValue() 	{ return m_avp_access_network_charging_identifier_value; }
    FDDictionaryEntryAVP &avpFeatureListId()                        	{ return m_avp_feature_list_id; }
    FDDictionaryEntryAVP &avpExperimentalResult()                   	{ return m_avp_experimental_result; }
    FDDictionaryEntryAVP &avpLoad()                                 	{ return m_avp_load; }
    FDDictionaryEntryAVP &avpOcFeatureVector()                      	{ return m_avp_oc_feature_vector; }
    FDDictionaryEntryAVP &avpSpecificAction()                       	{ return m_avp_specific_action; }
    FDDictionaryEntryAVP &avpAuthSessionState()                     	{ return m_avp_auth_session_state; }
    FDDictionaryEntryAVP &avpCcMoney()                              	{ return m_avp_cc_money; }
    FDDictionaryEntryAVP &avpCcTotalOctets()                        	{ return m_avp_cc_total_octets; }
    FDDictionaryEntryAVP &avpReportingReason()                      	{ return m_avp_reporting_reason; }
    FDDictionaryEntryAVP &avpLoadType()                             	{ return m_avp_load_type; }
    FDDictionaryEntryAVP &avpAnGwAddress()                          	{ return m_avp_an_gw_address; }
    FDDictionaryEntryAVP &avpTcpSourcePort()                        	{ return m_avp_tcp_source_port; }
    FDDictionaryEntryAVP &avpDestinationHost()                      	{ return m_avp_destination_host; }
    FDDictionaryEntryAVP &avpServiceAuthorizationInfo()             	{ return m_avp_service_authorization_info; }
    FDDictionaryEntryAVP &avpGcsIdentifier()                        	{ return m_avp_gcs_identifier; }
    FDDictionaryEntryAVP &avpRetryInterval()                        	{ return m_avp_retry_interval; }
    FDDictionaryEntryAVP &avpAfApplicationIdentifier()              	{ return m_avp_af_application_identifier; }
    FDDictionaryEntryAVP &avpUeLocalIpAddress()                     	{ return m_avp_ue_local_ip_address; }
    FDDictionaryEntryAVP &avpMediaComponentStatus()                 	{ return m_avp_media_component_status; }
    FDDictionaryEntryAVP &avpSipForkingIndication()                 	{ return m_avp_sip_forking_indication; }
    FDDictionaryEntryAVP &avpNetlocAccessSupport()                  	{ return m_avp_netloc_access_support; }
    FDDictionaryEntryAVP &avpMpsIdentifier()                        	{ return m_avp_mps_identifier; }
    FDDictionaryEntryAVP &avpSponsoredConnectivityData()            	{ return m_avp_sponsored_connectivity_data; }
    FDDictionaryEntryAVP &avpAccessNetworkChargingAddress()         	{ return m_avp_access_network_charging_address; }
    FDDictionaryEntryAVP &avpValueDigits()                          	{ return m_avp_value_digits; }
    FDDictionaryEntryAVP &avpUserEquipmentInfoType()                	{ return m_avp_user_equipment_info_type; }
    FDDictionaryEntryAVP &avpFramedIpv6Prefix()                     	{ return m_avp_framed_ipv6_prefix; }
    FDDictionaryEntryAVP &avpRxRequestType()                        	{ return m_avp_rx_request_type; }
    FDDictionaryEntryAVP &avpProxyInfo()                            	{ return m_avp_proxy_info; }
    FDDictionaryEntryAVP &avpUsedServiceUnit()                      	{ return m_avp_used_service_unit; }
    FDDictionaryEntryAVP &avpOriginRealm()                          	{ return m_avp_origin_realm; }
    FDDictionaryEntryAVP &avpTwanIdentifier()                       	{ return m_avp_twan_identifier; }
    FDDictionaryEntryAVP &avpOriginHost()                           	{ return m_avp_origin_host; }
    FDDictionaryEntryAVP &avpClass()                                	{ return m_avp_class; }
    FDDictionaryEntryAVP &avpSubscriptionIdData()                   	{ return m_avp_subscription_id_data; }
    FDDictionaryEntryAVP &avpIpDomainId()                           	{ return m_avp_ip_domain_id; }
    FDDictionaryEntryAVP &avpUserEquipmentInfoValue()               	{ return m_avp_user_equipment_info_value; }
    FDDictionaryEntryAVP &avpRouteRecord()                          	{ return m_avp_route_record; }
    FDDictionaryEntryAVP &avpFramedIpAddress()                      	{ return m_avp_framed_ip_address; }
    FDDictionaryEntryAVP &avpOcOlr()                                	{ return m_avp_oc_olr; }
    FDDictionaryEntryAVP &avpSubscriptionIdType()                   	{ return m_avp_subscription_id_type; }
    FDDictionaryEntryAVP &avpFeatureList()                          	{ return m_avp_feature_list; }
    FDDictionaryEntryAVP &avpSponsorIdentity()                      	{ return m_avp_sponsor_identity; }
    FDDictionaryEntryAVP &avpOcReductionPercentage()                	{ return m_avp_oc_reduction_percentage; }
    FDDictionaryEntryAVP &avpErrorReportingHost()                   	{ return m_avp_error_reporting_host; }
    FDDictionaryEntryAVP &avp3gppSgsnMccMnc()                       	{ return m_avp_3gpp_sgsn_mcc_mnc; }
    FDDictionaryEntryAVP &avpFlowNumber()                           	{ return m_avp_flow_number; }
    FDDictionaryEntryAVP &avpRatType()                              	{ return m_avp_rat_type; }
    FDDictionaryEntryAVP &avpEventChargingTimestamp()               	{ return m_avp_event_charging_timestamp; }
    FDDictionaryEntryAVP &avpApplicationServiceProviderIdentity()   	{ return m_avp_application_service_provider_identity; }
    FDDictionaryEntryAVP &avpOcValidityDuration()                   	{ return m_avp_oc_validity_duration; }
    FDDictionaryEntryAVP &avpOcSequenceNumber()                     	{ return m_avp_oc_sequence_number; }
    FDDictionaryEntryAVP &avpRedirectHost()                         	{ return m_avp_redirect_host; }
    FDDictionaryEntryAVP &avpResultCode()                           	{ return m_avp_result_code; }
    FDDictionaryEntryAVP &avpUserEquipmentInfo()                    	{ return m_avp_user_equipment_info; }
    FDDictionaryEntryAVP &avpOriginStateId()                        	{ return m_avp_origin_state_id; }
    FDDictionaryEntryAVP &avpCcOutputOctets()                       	{ return m_avp_cc_output_octets; }
    FDDictionaryEntryAVP &avpRanNasReleaseCause()                   	{ return m_avp_ran_nas_release_cause; }
    FDDictionaryEntryAVP &avpReservationPriority()                  	{ return m_avp_reservation_priority; }
    FDDictionaryEntryAVP &avp3gppUserLocationInfo()                 	{ return m_avp_3gpp_user_location_info; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthDl()              	{ return m_avp_max_requested_bandwidth_dl; }
    FDDictionaryEntryAVP &avpTariffChangeUsage()                    	{ return m_avp_tariff_change_usage; }
    FDDictionaryEntryAVP &avpPreEmptionControlInfo()                	{ return m_avp_pre_emption_control_info; }
    FDDictionaryEntryAVP &avpAcceptableServiceInfo()                	{ return m_avp_acceptable_service_info; }
    FDDictionaryEntryAVP &avpAbortCause()                           	{ return m_avp_abort_cause; }
    FDDictionaryEntryAVP &avpAnTrusted()                            	{ return m_avp_an_trusted; }
    FDDictionaryEntryAVP &avpCalledStationId()                      	{ return m_avp_called_station_id; }
    FDDictionaryEntryAVP &avpServiceUrn()                           	{ return m_avp_service_urn; }
    FDDictionaryEntryAVP &avpOcSupportedFeatures()                  	{ return m_avp_oc_supported_features; }
    FDDictionaryEntryAVP &avpProxyHost()                            	{ return m_avp_proxy_host; }
    FDDictionaryEntryAVP &avpContentVersion()                       	{ return m_avp_content_version; }
    FDDictionaryEntryAVP &avpUserLocationInfoTime()                 	{ return m_avp_user_location_info_time; }
    FDDictionaryEntryAVP &avpExperimentalResultCode()               	{ return m_avp_experimental_result_code; }
    FDDictionaryEntryAVP &avpAuthApplicationId()                    	{ return m_avp_auth_application_id; }
    FDDictionaryEntryAVP &avp3gppMsTimezone()                       	{ return m_avp_3gpp_ms_timezone; }
    FDDictionaryEntryAVP &avpSubscriptionId()                       	{ return m_avp_subscription_id; }
    FDDictionaryEntryAVP &avpIpCanType()                            	{ return m_avp_ip_can_type; }
    FDDictionaryEntryAVP &avpUdpSourcePort()                        	{ return m_avp_udp_source_port; }
    FDDictionaryEntryAVP &avpMediaComponentNumber()                 	{ return m_avp_media_component_number; }
    FDDictionaryEntryAVP &avpTariffTimeChange()                     	{ return m_avp_tariff_time_change; }
    FDDictionaryEntryAVP &avpSponsoringAction()                     	{ return m_avp_sponsoring_action; }
    FDDictionaryEntryAVP &avpErrorMessage()                         	{ return m_avp_error_message; }
    FDDictionaryEntryAVP &avpRequiredAccessInfo()                   	{ return m_avp_required_access_info; }
    FDDictionaryEntryAVP &avpFailedAvp()                            	{ return m_avp_failed_avp; }
    FDDictionaryEntryAVP &avpFlows()                                	{ return m_avp_flows; }
    FDDictionaryEntryAVP &avpSupportedFeatures()                    	{ return m_avp_supported_features; }
    FDDictionaryEntryAVP &avpCcServiceSpecificUnits()               	{ return m_avp_cc_service_specific_units; }
    FDDictionaryEntryAVP &avpCcTime()                               	{ return m_avp_cc_time; }
    FDDictionaryEntryAVP &avpCurrencyCode()                         	{ return m_avp_currency_code; }
    FDDictionaryEntryAVP &avpCcInputOctets()                        	{ return m_avp_cc_input_octets; }
    FDDictionaryEntryAVP &avpMediaComponentDescription()            	{ return m_avp_media_component_description; }
    FDDictionaryEntryAVP &avpProxyState()                           	{ return m_avp_proxy_state; }
    FDDictionaryEntryAVP &avpAccessNetworkChargingIdentifier()      	{ return m_avp_access_network_charging_identifier; }
    FDDictionaryEntryAVP &avpAfChargingIdentifier()                 	{ return m_avp_af_charging_identifier; }
    FDDictionaryEntryAVP &avpReferenceId()                          	{ return m_avp_reference_id; }
    FDDictionaryEntryAVP &avpVendorId()                             	{ return m_avp_vendor_id; }
    FDDictionaryEntryAVP &avpGrantedServiceUnit()                   	{ return m_avp_granted_service_unit; }
    FDDictionaryEntryAVP &avpMaxRequestedBandwidthUl()              	{ return m_avp_max_requested_bandwidth_ul; }
    FDDictionaryEntryAVP &avpUnitValue()                            	{ return m_avp_unit_value; }
    FDDictionaryEntryAVP &avpFinalUnitAction()                      	{ return m_avp_final_unit_action; }
    FDDictionaryEntryAVP &avpServiceInfoStatus()                    	{ return m_avp_service_info_status; }
    FDDictionaryEntryAVP &avpDestinationRealm()                     	{ return m_avp_destination_realm; }
    FDDictionaryEntryAVP &avpSessionId()                            	{ return m_avp_session_id; }
    FDDictionaryEntryAVP &avpDrmp()                                 	{ return m_avp_drmp; }
    FDDictionaryEntryAVP &avpOcReportType()                         	{ return m_avp_oc_report_type; }

private:
    FDDictionaryEntryApplication m_app;                             

    FDDictionaryEntryCommand m_cmd_aar;
    FDDictionaryEntryCommand m_cmd_aaa;
    FDDictionaryEntryCommand m_cmd_rear;
    FDDictionaryEntryCommand m_cmd_reaa;
    FDDictionaryEntryCommand m_cmd_setr;
    FDDictionaryEntryCommand m_cmd_seta;
    FDDictionaryEntryCommand m_cmd_absr;
    FDDictionaryEntryCommand m_cmd_absa;

    FDDictionaryEntryVendor m_vnd_Etsi;
    FDDictionaryEntryVendor m_vnd_3gpp;

    FDDictionaryEntryAVP m_avp_mcptt_identifier;
    FDDictionaryEntryAVP m_avp_redirect_host_usage;
    FDDictionaryEntryAVP m_avp_af_requested_data;
    FDDictionaryEntryAVP m_avp_redirect_max_cache_time;
    FDDictionaryEntryAVP m_avp_termination_cause;
    FDDictionaryEntryAVP m_avp_exponent;
    FDDictionaryEntryAVP m_avp_sourceid;
    FDDictionaryEntryAVP m_avp_load_value;
    FDDictionaryEntryAVP m_avp_access_network_charging_identifier_value;
    FDDictionaryEntryAVP m_avp_feature_list_id;
    FDDictionaryEntryAVP m_avp_experimental_result;
    FDDictionaryEntryAVP m_avp_load;
    FDDictionaryEntryAVP m_avp_oc_feature_vector;
    FDDictionaryEntryAVP m_avp_specific_action;
    FDDictionaryEntryAVP m_avp_auth_session_state;
    FDDictionaryEntryAVP m_avp_cc_money;
    FDDictionaryEntryAVP m_avp_cc_total_octets;
    FDDictionaryEntryAVP m_avp_reporting_reason;
    FDDictionaryEntryAVP m_avp_load_type;
    FDDictionaryEntryAVP m_avp_an_gw_address;
    FDDictionaryEntryAVP m_avp_tcp_source_port;
    FDDictionaryEntryAVP m_avp_destination_host;
    FDDictionaryEntryAVP m_avp_service_authorization_info;
    FDDictionaryEntryAVP m_avp_gcs_identifier;
    FDDictionaryEntryAVP m_avp_retry_interval;
    FDDictionaryEntryAVP m_avp_af_application_identifier;
    FDDictionaryEntryAVP m_avp_ue_local_ip_address;
    FDDictionaryEntryAVP m_avp_media_component_status;
    FDDictionaryEntryAVP m_avp_sip_forking_indication;
    FDDictionaryEntryAVP m_avp_netloc_access_support;
    FDDictionaryEntryAVP m_avp_mps_identifier;
    FDDictionaryEntryAVP m_avp_sponsored_connectivity_data;
    FDDictionaryEntryAVP m_avp_access_network_charging_address;
    FDDictionaryEntryAVP m_avp_value_digits;
    FDDictionaryEntryAVP m_avp_user_equipment_info_type;
    FDDictionaryEntryAVP m_avp_framed_ipv6_prefix;
    FDDictionaryEntryAVP m_avp_rx_request_type;
    FDDictionaryEntryAVP m_avp_proxy_info;
    FDDictionaryEntryAVP m_avp_used_service_unit;
    FDDictionaryEntryAVP m_avp_origin_realm;
    FDDictionaryEntryAVP m_avp_twan_identifier;
    FDDictionaryEntryAVP m_avp_origin_host;
    FDDictionaryEntryAVP m_avp_class;
    FDDictionaryEntryAVP m_avp_subscription_id_data;
    FDDictionaryEntryAVP m_avp_ip_domain_id;
    FDDictionaryEntryAVP m_avp_user_equipment_info_value;
    FDDictionaryEntryAVP m_avp_route_record;
    FDDictionaryEntryAVP m_avp_framed_ip_address;
    FDDictionaryEntryAVP m_avp_oc_olr;
    FDDictionaryEntryAVP m_avp_subscription_id_type;
    FDDictionaryEntryAVP m_avp_feature_list;
    FDDictionaryEntryAVP m_avp_sponsor_identity;
    FDDictionaryEntryAVP m_avp_oc_reduction_percentage;
    FDDictionaryEntryAVP m_avp_error_reporting_host;
    FDDictionaryEntryAVP m_avp_3gpp_sgsn_mcc_mnc;
    FDDictionaryEntryAVP m_avp_flow_number;
    FDDictionaryEntryAVP m_avp_rat_type;
    FDDictionaryEntryAVP m_avp_event_charging_timestamp;
    FDDictionaryEntryAVP m_avp_application_service_provider_identity;
    FDDictionaryEntryAVP m_avp_oc_validity_duration;
    FDDictionaryEntryAVP m_avp_oc_sequence_number;
    FDDictionaryEntryAVP m_avp_redirect_host;
    FDDictionaryEntryAVP m_avp_result_code;
    FDDictionaryEntryAVP m_avp_user_equipment_info;
    FDDictionaryEntryAVP m_avp_origin_state_id;
    FDDictionaryEntryAVP m_avp_cc_output_octets;
    FDDictionaryEntryAVP m_avp_ran_nas_release_cause;
    FDDictionaryEntryAVP m_avp_reservation_priority;
    FDDictionaryEntryAVP m_avp_3gpp_user_location_info;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_dl;
    FDDictionaryEntryAVP m_avp_tariff_change_usage;
    FDDictionaryEntryAVP m_avp_pre_emption_control_info;
    FDDictionaryEntryAVP m_avp_acceptable_service_info;
    FDDictionaryEntryAVP m_avp_abort_cause;
    FDDictionaryEntryAVP m_avp_an_trusted;
    FDDictionaryEntryAVP m_avp_called_station_id;
    FDDictionaryEntryAVP m_avp_service_urn;
    FDDictionaryEntryAVP m_avp_oc_supported_features;
    FDDictionaryEntryAVP m_avp_proxy_host;
    FDDictionaryEntryAVP m_avp_content_version;
    FDDictionaryEntryAVP m_avp_user_location_info_time;
    FDDictionaryEntryAVP m_avp_experimental_result_code;
    FDDictionaryEntryAVP m_avp_auth_application_id;
    FDDictionaryEntryAVP m_avp_3gpp_ms_timezone;
    FDDictionaryEntryAVP m_avp_subscription_id;
    FDDictionaryEntryAVP m_avp_ip_can_type;
    FDDictionaryEntryAVP m_avp_udp_source_port;
    FDDictionaryEntryAVP m_avp_media_component_number;
    FDDictionaryEntryAVP m_avp_tariff_time_change;
    FDDictionaryEntryAVP m_avp_sponsoring_action;
    FDDictionaryEntryAVP m_avp_error_message;
    FDDictionaryEntryAVP m_avp_required_access_info;
    FDDictionaryEntryAVP m_avp_failed_avp;
    FDDictionaryEntryAVP m_avp_flows;
    FDDictionaryEntryAVP m_avp_supported_features;
    FDDictionaryEntryAVP m_avp_cc_service_specific_units;
    FDDictionaryEntryAVP m_avp_cc_time;
    FDDictionaryEntryAVP m_avp_currency_code;
    FDDictionaryEntryAVP m_avp_cc_input_octets;
    FDDictionaryEntryAVP m_avp_media_component_description;
    FDDictionaryEntryAVP m_avp_proxy_state;
    FDDictionaryEntryAVP m_avp_access_network_charging_identifier;
    FDDictionaryEntryAVP m_avp_af_charging_identifier;
    FDDictionaryEntryAVP m_avp_reference_id;
    FDDictionaryEntryAVP m_avp_vendor_id;
    FDDictionaryEntryAVP m_avp_granted_service_unit;
    FDDictionaryEntryAVP m_avp_max_requested_bandwidth_ul;
    FDDictionaryEntryAVP m_avp_unit_value;
    FDDictionaryEntryAVP m_avp_final_unit_action;
    FDDictionaryEntryAVP m_avp_service_info_status;
    FDDictionaryEntryAVP m_avp_destination_realm;
    FDDictionaryEntryAVP m_avp_session_id;
    FDDictionaryEntryAVP m_avp_drmp;
    FDDictionaryEntryAVP m_avp_oc_report_type;

};

//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - AaRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// AAR request from Clients.
//-
class AARcmd : public FDCommandRequest
{
public:
    AARcmd( Application &app );
    ~AARcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    AARcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - AaRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a AAR
// to the Diameter application and process the corresponding answer.
//
class AARreq : public FDMessageRequest
{
public:
    AARreq( Application &app );
    virtual ~AARreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }
    void addSessionId( FDDictionaryEntryAVP &deSessionId ) { m_session.addSessionId( *this, deSessionId ); }

private:
    AARreq();
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
// DIAMETER REQUEST Processing - AbortSessionRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// ABSR request from Clients.
//-
class ABSRcmd : public FDCommandRequest
{
public:
    ABSRcmd( Application &app );
    ~ABSRcmd();

    virtual int process( FDMessageRequest *req );

    Application &getApplication() { return m_app; }
    Dictionary &getDict();

private:
    ABSRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - AbortSessionRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a ABSR
// to the Diameter application and process the corresponding answer.
//
class ABSRreq : public FDMessageRequest
{
public:
    ABSRreq( Application &app );
    virtual ~ABSRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    Dictionary &getDict();
    const std::string &getSessionId() { return m_session.getSessionId(); }
    void addSessionId( FDDictionaryEntryAVP &deSessionId ) { m_session.addSessionId( *this, deSessionId ); }

private:
    ABSRreq();
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
class ExperimentalResultExtractor : public FDExtractor
{
 public:
    ExperimentalResultExtractor( FDExtractor &parent, Dictionary &dict );
    ~ExperimentalResultExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp experimental_result_code;
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

class SponsoredConnectivityDataExtractor : public FDExtractor
{
 public:
    SponsoredConnectivityDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~SponsoredConnectivityDataExtractor();

    FDExtractorAvp sponsor_identity;
    FDExtractorAvp application_service_provider_identity;
    GrantedServiceUnitExtractor granted_service_unit;
    UsedServiceUnitExtractor used_service_unit;
    FDExtractorAvp sponsoring_action;
};

class ProxyInfoExtractor : public FDExtractor
{
 public:
    ProxyInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProxyInfoExtractor();

    FDExtractorAvp proxy_host;
    FDExtractorAvp proxy_state;
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

class UserEquipmentInfoExtractor : public FDExtractor
{
 public:
    UserEquipmentInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~UserEquipmentInfoExtractor();

    FDExtractorAvp user_equipment_info_type;
    FDExtractorAvp user_equipment_info_value;
};

class MediaComponentDescriptionExtractor : public FDExtractor
{
 public:
    MediaComponentDescriptionExtractor( FDExtractor &parent, Dictionary &dict );
    ~MediaComponentDescriptionExtractor();

    FDExtractorAvp media_component_number;
};

class MediaComponentDescriptionExtractorList : public FDExtractorList
{
 public:
    MediaComponentDescriptionExtractorList( FDExtractor &parent, Dictionary &dict );
    ~MediaComponentDescriptionExtractorList();

    FDExtractor *createExtractor() { return new MediaComponentDescriptionExtractor( getParent(), m_dict ); }
    std::list<MediaComponentDescriptionExtractor*> &getList() { return (std::list<MediaComponentDescriptionExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class AcceptableServiceInfoExtractor : public FDExtractor
{
 public:
    AcceptableServiceInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~AcceptableServiceInfoExtractor();

    MediaComponentDescriptionExtractorList media_component_description;
    FDExtractorAvp max_requested_bandwidth_dl;
    FDExtractorAvp max_requested_bandwidth_ul;
};

class OcSupportedFeaturesExtractor : public FDExtractor
{
 public:
    OcSupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict );
    ~OcSupportedFeaturesExtractor();

    FDExtractorAvp oc_feature_vector;
};

class SubscriptionIdExtractor : public FDExtractor
{
 public:
    SubscriptionIdExtractor( FDExtractor &parent, Dictionary &dict );
    ~SubscriptionIdExtractor();

    FDExtractorAvp subscription_id_type;
    FDExtractorAvp subscription_id_data;
};

class FailedAvpExtractor : public FDExtractor
{
 public:
    FailedAvpExtractor( FDExtractor &parent, Dictionary &dict );
    ~FailedAvpExtractor();


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

class SupportedFeaturesExtractor : public FDExtractor
{
 public:
    SupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict );
    ~SupportedFeaturesExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp feature_list_id;
    FDExtractorAvp feature_list;
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

class AccessNetworkChargingIdentifierExtractor : public FDExtractor
{
 public:
    AccessNetworkChargingIdentifierExtractor( FDExtractor &parent, Dictionary &dict );
    ~AccessNetworkChargingIdentifierExtractor();

    FDExtractorAvp access_network_charging_identifier_value;
    FlowsExtractorList flows;
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

class AaRequestExtractor : public FDExtractor
{
 public:
    AaRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~AaRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp auth_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp ip_domain_id;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp af_application_identifier;
    MediaComponentDescriptionExtractorList media_component_description;
    FDExtractorAvp service_info_status;
    FDExtractorAvp af_charging_identifier;
    FDExtractorAvp sip_forking_indication;
    FDExtractorAvpList specific_action;
    SubscriptionIdExtractorList subscription_id;
    OcSupportedFeaturesExtractor oc_supported_features;
    SupportedFeaturesExtractorList supported_features;
    FDExtractorAvp reservation_priority;
    FDExtractorAvp framed_ip_address;
    FDExtractorAvp framed_ipv6_prefix;
    FDExtractorAvp called_station_id;
    FDExtractorAvp service_urn;
    SponsoredConnectivityDataExtractor sponsored_connectivity_data;
    FDExtractorAvp mps_identifier;
    FDExtractorAvp gcs_identifier;
    FDExtractorAvp mcptt_identifier;
    FDExtractorAvp rx_request_type;
    FDExtractorAvpList required_access_info;
    FDExtractorAvp af_requested_data;
    FDExtractorAvp reference_id;
    FDExtractorAvp pre_emption_control_info;
    FDExtractorAvp origin_state_id;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class AccessNetworkChargingIdentifierExtractorList : public FDExtractorList
{
 public:
    AccessNetworkChargingIdentifierExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AccessNetworkChargingIdentifierExtractorList();

    FDExtractor *createExtractor() { return new AccessNetworkChargingIdentifierExtractor( getParent(), m_dict ); }
    std::list<AccessNetworkChargingIdentifierExtractor*> &getList() { return (std::list<AccessNetworkChargingIdentifierExtractor*>&)FDExtractorList::getList(); }

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

class AaAnswerExtractor : public FDExtractor
{
 public:
    AaAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~AaAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp auth_application_id;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    AccessNetworkChargingIdentifierExtractorList access_network_charging_identifier;
    FDExtractorAvp access_network_charging_address;
    AcceptableServiceInfoExtractor acceptable_service_info;
    FDExtractorAvp an_gw_address;
    FDExtractorAvp an_trusted;
    FDExtractorAvp service_authorization_info;
    FDExtractorAvp ip_can_type;
    FDExtractorAvp netloc_access_support;
    FDExtractorAvp rat_type;
    FlowsExtractorList flows;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    SupportedFeaturesExtractorList supported_features;
    SubscriptionIdExtractorList subscription_id;
    UserEquipmentInfoExtractor user_equipment_info;
    FDExtractorAvp tgpp_sgsn_mcc_mnc;
    FDExtractorAvpList _class;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    FailedAvpExtractor failed_avp;
    FDExtractorAvp retry_interval;
    FDExtractorAvp origin_state_id;
    FDExtractorAvpList redirect_host;
    FDExtractorAvp redirect_host_usage;
    FDExtractorAvp redirect_max_cache_time;
    ProxyInfoExtractorList proxy_info;
    LoadExtractorList load;
};

class ReAuthRequestExtractor : public FDExtractor
{
 public:
    ReAuthRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~ReAuthRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp auth_application_id;
    FDExtractorAvpList specific_action;
    OcSupportedFeaturesExtractor oc_supported_features;
    AccessNetworkChargingIdentifierExtractorList access_network_charging_identifier;
    FDExtractorAvp access_network_charging_address;
    FDExtractorAvp an_gw_address;
    FDExtractorAvp an_trusted;
    FlowsExtractorList flows;
    SubscriptionIdExtractorList subscription_id;
    FDExtractorAvp abort_cause;
    FDExtractorAvp ip_can_type;
    FDExtractorAvp netloc_access_support;
    FDExtractorAvp rat_type;
    SponsoredConnectivityDataExtractor sponsored_connectivity_data;
    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp user_location_info_time;
    FDExtractorAvp tgpp_ms_timezone;
    FDExtractorAvpList ran_nas_release_cause;
    FDExtractorAvp tgpp_sgsn_mcc_mnc;
    FDExtractorAvp twan_identifier;
    FDExtractorAvp tcp_source_port;
    FDExtractorAvp udp_source_port;
    FDExtractorAvp ue_local_ip_address;
    FDExtractorAvp origin_state_id;
    FDExtractorAvpList _class;
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
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    MediaComponentDescriptionExtractorList media_component_description;
    FDExtractorAvp service_urn;
    FDExtractorAvp origin_state_id;
    FDExtractorAvpList _class;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    FDExtractorAvpList redirect_host;
    FDExtractorAvp redirect_host_usage;
    FDExtractorAvp redirect_max_cache_time;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
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
    FDExtractorAvpList required_access_info;
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
    SponsoredConnectivityDataExtractor sponsored_connectivity_data;
    FDExtractorAvp origin_state_id;
    FDExtractorAvp tgpp_user_location_info;
    FDExtractorAvp user_location_info_time;
    FDExtractorAvp tgpp_ms_timezone;
    FDExtractorAvpList ran_nas_release_cause;
    FDExtractorAvp tgpp_sgsn_mcc_mnc;
    FDExtractorAvp twan_identifier;
    FDExtractorAvp tcp_source_port;
    FDExtractorAvp udp_source_port;
    FDExtractorAvp ue_local_ip_address;
    FDExtractorAvp netloc_access_support;
    FDExtractorAvpList _class;
    FDExtractorAvpList redirect_host;
    FDExtractorAvp redirect_host_usage;
    FDExtractorAvp redirect_max_cache_time;
    ProxyInfoExtractorList proxy_info;
    LoadExtractorList load;
};

class AbortSessionRequestExtractor : public FDExtractor
{
 public:
    AbortSessionRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~AbortSessionRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp auth_application_id;
    OcSupportedFeaturesExtractor oc_supported_features;
    FDExtractorAvp abort_cause;
    FDExtractorAvp origin_state_id;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class AbortSessionAnswerExtractor : public FDExtractor
{
 public:
    AbortSessionAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~AbortSessionAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp result_code;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    FDExtractorAvp origin_state_id;
    FDExtractorAvp error_message;
    FDExtractorAvp error_reporting_host;
    FailedAvpExtractor failed_avp;
    FDExtractorAvpList redirect_host;
    FDExtractorAvp redirect_host_usage;
    FDExtractorAvp redirect_max_cache_time;
    ProxyInfoExtractorList proxy_info;
};


}

#endif // __RX_H

