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

#ifndef __S6T_H
#define __S6T_H

#include "fd.h"

namespace s6t {

//
// Primary Objects
//

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    FDDictionaryEntryApplication &app()                              { return m_app; }

    FDDictionaryEntryCommand &cmdCOIR()                             	{ return m_cmd_coir; }
    FDDictionaryEntryCommand &cmdCOIA()                             	{ return m_cmd_coia; }
    FDDictionaryEntryCommand &cmdREIR()                             	{ return m_cmd_reir; }
    FDDictionaryEntryCommand &cmdREIA()                             	{ return m_cmd_reia; }
    FDDictionaryEntryCommand &cmdNIIR()                             	{ return m_cmd_niir; }
    FDDictionaryEntryCommand &cmdNIIA()                             	{ return m_cmd_niia; }

    FDDictionaryEntryVendor &vnd3GPP()                              	{ return m_vnd_3gpp; }

    FDDictionaryEntryAVP &avpGroupReportingGuardTimer()             	{ return m_avp_group_reporting_guard_timer; }
    FDDictionaryEntryAVP &avpMonitoringEventConfigStatus()          	{ return m_avp_monitoring_event_config_status; }
    FDDictionaryEntryAVP &avpOcReportType()                         	{ return m_avp_oc_report_type; }
    FDDictionaryEntryAVP &avpServiceAreaIdentity()                  	{ return m_avp_service_area_identity; }
    FDDictionaryEntryAVP &avpDayOfWeekMask()                        	{ return m_avp_day_of_week_mask; }
    FDDictionaryEntryAVP &avpMaximumUeAvailabilityTime()            	{ return m_avp_maximum_ue_availability_time; }
    FDDictionaryEntryAVP &avpMaximumResponseTime()                  	{ return m_avp_maximum_response_time; }
    FDDictionaryEntryAVP &avpProxyInfo()                            	{ return m_avp_proxy_info; }
    FDDictionaryEntryAVP &avpCsgAccessMode()                        	{ return m_avp_csg_access_mode; }
    FDDictionaryEntryAVP &avpTrackingAreaIdentity()                 	{ return m_avp_tracking_area_identity; }
    FDDictionaryEntryAVP &avpLoadValue()                            	{ return m_avp_load_value; }
    FDDictionaryEntryAVP &avpFeatureList()                          	{ return m_avp_feature_list; }
    FDDictionaryEntryAVP &avpExternalIdentifier()                   	{ return m_avp_external_identifier; }
    FDDictionaryEntryAVP &avpFeatureListId()                        	{ return m_avp_feature_list_id; }
    FDDictionaryEntryAVP &avpExperimentalResult()                   	{ return m_avp_experimental_result; }
    FDDictionaryEntryAVP &avpPeriodicCommunicationIndicator()       	{ return m_avp_periodic_communication_indicator; }
    FDDictionaryEntryAVP &avpCommunicationDurationTime()            	{ return m_avp_communication_duration_time; }
    FDDictionaryEntryAVP &avpMmeLocationInformation()               	{ return m_avp_mme_location_information; }
    FDDictionaryEntryAVP &avpAuthSessionState()                     	{ return m_avp_auth_session_state; }
    FDDictionaryEntryAVP &avpDrmp()                                 	{ return m_avp_drmp; }
    FDDictionaryEntryAVP &avpAeseErrorReport()                      	{ return m_avp_aese_error_report; }
    FDDictionaryEntryAVP &avpCiaFlags()                             	{ return m_avp_cia_flags; }
    FDDictionaryEntryAVP &avpLoadType()                             	{ return m_avp_load_type; }
    FDDictionaryEntryAVP &avpReachabilityInformation()              	{ return m_avp_reachability_information; }
    FDDictionaryEntryAVP &avpDestinationHost()                      	{ return m_avp_destination_host; }
    FDDictionaryEntryAVP &avpRestrictedPlmnList()                   	{ return m_avp_restricted_plmn_list; }
    FDDictionaryEntryAVP &avpMsisdn()                               	{ return m_avp_msisdn; }
    FDDictionaryEntryAVP &avpCommunicationPatternSet()              	{ return m_avp_communication_pattern_set; }
    FDDictionaryEntryAVP &avpScheduledCommunicationTime()           	{ return m_avp_scheduled_communication_time; }
    FDDictionaryEntryAVP &avpChargedParty()                         	{ return m_avp_charged_party; }
    FDDictionaryEntryAVP &avpSupportedServices()                    	{ return m_avp_supported_services; }
    FDDictionaryEntryAVP &avpReachabilityType()                     	{ return m_avp_reachability_type; }
    FDDictionaryEntryAVP &avpGroupMonitoringEventReport()           	{ return m_avp_group_monitoring_event_report; }
    FDDictionaryEntryAVP &avpEnhancedCoverageRestriction()          	{ return m_avp_enhanced_coverage_restriction; }
    FDDictionaryEntryAVP &avpNiddAuthorizationUpdate()              	{ return m_avp_nidd_authorization_update; }
    FDDictionaryEntryAVP &avpOriginRealm()                          	{ return m_avp_origin_realm; }
    FDDictionaryEntryAVP &avpOriginHost()                           	{ return m_avp_origin_host; }
    FDDictionaryEntryAVP &avpCellGlobalIdentity()                   	{ return m_avp_cell_global_identity; }
    FDDictionaryEntryAVP &avpUeReachabilityConfiguration()          	{ return m_avp_ue_reachability_configuration; }
    FDDictionaryEntryAVP &avpLocationInformationConfiguration()     	{ return m_avp_location_information_configuration; }
    FDDictionaryEntryAVP &avpMonteLocationType()                    	{ return m_avp_monte_location_type; }
    FDDictionaryEntryAVP &avpAssociationType()                      	{ return m_avp_association_type; }
    FDDictionaryEntryAVP &avpEpsLocationInformation()               	{ return m_avp_eps_location_information; }
    FDDictionaryEntryAVP &avpRouteRecord()                          	{ return m_avp_route_record; }
    FDDictionaryEntryAVP &avpReferenceIdValidityTime()              	{ return m_avp_reference_id_validity_time; }
    FDDictionaryEntryAVP &avpServiceSelection()                     	{ return m_avp_service_selection; }
    FDDictionaryEntryAVP &avpScefId()                               	{ return m_avp_scef_id; }
    FDDictionaryEntryAVP &avpCsgId()                                	{ return m_avp_csg_id; }
    FDDictionaryEntryAVP &avpCurrentLocationRetrieved()             	{ return m_avp_current_location_retrieved; }
    FDDictionaryEntryAVP &avpMonitoringType()                       	{ return m_avp_monitoring_type; }
    FDDictionaryEntryAVP &avpTimeOfDayEnd()                         	{ return m_avp_time_of_day_end; }
    FDDictionaryEntryAVP &avpOcOlr()                                	{ return m_avp_oc_olr; }
    FDDictionaryEntryAVP &avpMaximumDetectionTime()                 	{ return m_avp_maximum_detection_time; }
    FDDictionaryEntryAVP &avpAgeOfLocationInformation()             	{ return m_avp_age_of_location_information; }
    FDDictionaryEntryAVP &avpGeodeticInformation()                  	{ return m_avp_geodetic_information; }
    FDDictionaryEntryAVP &avpServiceResult()                        	{ return m_avp_service_result; }
    FDDictionaryEntryAVP &avpOcReductionPercentage()                	{ return m_avp_oc_reduction_percentage; }
    FDDictionaryEntryAVP &avpServiceResultCode()                    	{ return m_avp_service_result_code; }
    FDDictionaryEntryAVP &avpSupportedFeatures()                    	{ return m_avp_supported_features; }
    FDDictionaryEntryAVP &avpNiddAuthorizationResponse()            	{ return m_avp_nidd_authorization_response; }
    FDDictionaryEntryAVP &avpSupportedMonitoringEvents()            	{ return m_avp_supported_monitoring_events; }
    FDDictionaryEntryAVP &avpUserIdentifier()                       	{ return m_avp_user_identifier; }
    FDDictionaryEntryAVP &avpAccuracy()                             	{ return m_avp_accuracy; }
    FDDictionaryEntryAVP &avpOcValidityDuration()                   	{ return m_avp_oc_validity_duration; }
    FDDictionaryEntryAVP &avpCsgMembershipIndication()              	{ return m_avp_csg_membership_indication; }
    FDDictionaryEntryAVP &avpDlBufferingSuggestedPacketCount()      	{ return m_avp_dl_buffering_suggested_packet_count; }
    FDDictionaryEntryAVP &avpMaximumLatency()                       	{ return m_avp_maximum_latency; }
    FDDictionaryEntryAVP &avpSourceid()                             	{ return m_avp_sourceid; }
    FDDictionaryEntryAVP &avpAeseCommunicationPatternConfigStatus() 	{ return m_avp_aese_communication_pattern_config_status; }
    FDDictionaryEntryAVP &avpAllowedPlmnList()                      	{ return m_avp_allowed_plmn_list; }
    FDDictionaryEntryAVP &avpSgsnLocationInformation()              	{ return m_avp_sgsn_location_information; }
    FDDictionaryEntryAVP &avpEventHandling()                        	{ return m_avp_event_handling; }
    FDDictionaryEntryAVP &avpResultCode()                           	{ return m_avp_result_code; }
    FDDictionaryEntryAVP &avpTypeOfExternalIdentifier()             	{ return m_avp_type_of_external_identifier; }
    FDDictionaryEntryAVP &avpS6tHssCause()                          	{ return m_avp_s6t_hss_cause; }
    FDDictionaryEntryAVP &avpTimeOfDayStart()                       	{ return m_avp_time_of_day_start; }
    FDDictionaryEntryAVP &avpEnhancedCoverageRestrictionData()      	{ return m_avp_enhanced_coverage_restriction_data; }
    FDDictionaryEntryAVP &avpScefReferenceId()                      	{ return m_avp_scef_reference_id; }
    FDDictionaryEntryAVP &avpMonitoringEventConfiguration()         	{ return m_avp_monitoring_event_configuration; }
    FDDictionaryEntryAVP &avpLoad()                                 	{ return m_avp_load; }
    FDDictionaryEntryAVP &avpEUtranCellGlobalIdentity()             	{ return m_avp_e_utran_cell_global_identity; }
    FDDictionaryEntryAVP &avpOcSupportedFeatures()                  	{ return m_avp_oc_supported_features; }
    FDDictionaryEntryAVP &avpProxyHost()                            	{ return m_avp_proxy_host; }
    FDDictionaryEntryAVP &avpExperimentalResultCode()               	{ return m_avp_experimental_result_code; }
    FDDictionaryEntryAVP &avpMaximumNumberOfReports()               	{ return m_avp_maximum_number_of_reports; }
    FDDictionaryEntryAVP &avpCirFlags()                             	{ return m_avp_cir_flags; }
    FDDictionaryEntryAVP &avpMonitoringEventReport()                	{ return m_avp_monitoring_event_report; }
    FDDictionaryEntryAVP &avpExtendedEnodebId()                     	{ return m_avp_extended_enodeb_id; }
    FDDictionaryEntryAVP &avpAeseCommunicationPattern()             	{ return m_avp_aese_communication_pattern; }
    FDDictionaryEntryAVP &avpOcFeatureVector()                      	{ return m_avp_oc_feature_vector; }
    FDDictionaryEntryAVP &avpMonitoringDuration()                   	{ return m_avp_monitoring_duration; }
    FDDictionaryEntryAVP &avpLossOfConnectivityReason()             	{ return m_avp_loss_of_connectivity_reason; }
    FDDictionaryEntryAVP &avpNodeType()                             	{ return m_avp_node_type; }
    FDDictionaryEntryAVP &avpVisitedPlmnId()                        	{ return m_avp_visited_plmn_id; }
    FDDictionaryEntryAVP &avpStationaryIndication()                 	{ return m_avp_stationary_indication; }
    FDDictionaryEntryAVP &avpGrantedValidityTime()                  	{ return m_avp_granted_validity_time; }
    FDDictionaryEntryAVP &avpFailedAvp()                            	{ return m_avp_failed_avp; }
    FDDictionaryEntryAVP &avpRoutingAreaIdentity()                  	{ return m_avp_routing_area_identity; }
    FDDictionaryEntryAVP &avpRequestedValidityTime()                	{ return m_avp_requested_validity_time; }
    FDDictionaryEntryAVP &avpGeographicalInformation()              	{ return m_avp_geographical_information; }
    FDDictionaryEntryAVP &avpServiceReport()                        	{ return m_avp_service_report; }
    FDDictionaryEntryAVP &avpProxyState()                           	{ return m_avp_proxy_state; }
    FDDictionaryEntryAVP &avpEnodebId()                             	{ return m_avp_enodeb_id; }
    FDDictionaryEntryAVP &avpPeriodicTime()                         	{ return m_avp_periodic_time; }
    FDDictionaryEntryAVP &avpNiddAuthorizationRequest()             	{ return m_avp_nidd_authorization_request; }
    FDDictionaryEntryAVP &avpVendorId()                             	{ return m_avp_vendor_id; }
    FDDictionaryEntryAVP &avpOcSequenceNumber()                     	{ return m_avp_oc_sequence_number; }
    FDDictionaryEntryAVP &avpGroupMonitoringEventReportItem()       	{ return m_avp_group_monitoring_event_report_item; }
    FDDictionaryEntryAVP &avpDestinationRealm()                     	{ return m_avp_destination_realm; }
    FDDictionaryEntryAVP &avpSessionId()                            	{ return m_avp_session_id; }
    FDDictionaryEntryAVP &avpUserName()                             	{ return m_avp_user_name; }
    FDDictionaryEntryAVP &avpRoamingInformation()                   	{ return m_avp_roaming_information; }
    FDDictionaryEntryAVP &avpImeiChange()                           	{ return m_avp_imei_change; }
    FDDictionaryEntryAVP &avpScefReferenceIdForDeletion()           	{ return m_avp_scef_reference_id_for_deletion; }
    FDDictionaryEntryAVP &avpRirFlags()                             	{ return m_avp_rir_flags; }
    FDDictionaryEntryAVP &avpUserCsgInformation()                   	{ return m_avp_user_csg_information; }

private:
    FDDictionaryEntryApplication m_app;                             

    FDDictionaryEntryCommand m_cmd_coir;
    FDDictionaryEntryCommand m_cmd_coia;
    FDDictionaryEntryCommand m_cmd_reir;
    FDDictionaryEntryCommand m_cmd_reia;
    FDDictionaryEntryCommand m_cmd_niir;
    FDDictionaryEntryCommand m_cmd_niia;

    FDDictionaryEntryVendor m_vnd_3gpp;

    FDDictionaryEntryAVP m_avp_group_reporting_guard_timer;
    FDDictionaryEntryAVP m_avp_monitoring_event_config_status;
    FDDictionaryEntryAVP m_avp_oc_report_type;
    FDDictionaryEntryAVP m_avp_service_area_identity;
    FDDictionaryEntryAVP m_avp_day_of_week_mask;
    FDDictionaryEntryAVP m_avp_maximum_ue_availability_time;
    FDDictionaryEntryAVP m_avp_maximum_response_time;
    FDDictionaryEntryAVP m_avp_proxy_info;
    FDDictionaryEntryAVP m_avp_csg_access_mode;
    FDDictionaryEntryAVP m_avp_tracking_area_identity;
    FDDictionaryEntryAVP m_avp_load_value;
    FDDictionaryEntryAVP m_avp_feature_list;
    FDDictionaryEntryAVP m_avp_external_identifier;
    FDDictionaryEntryAVP m_avp_feature_list_id;
    FDDictionaryEntryAVP m_avp_experimental_result;
    FDDictionaryEntryAVP m_avp_periodic_communication_indicator;
    FDDictionaryEntryAVP m_avp_communication_duration_time;
    FDDictionaryEntryAVP m_avp_mme_location_information;
    FDDictionaryEntryAVP m_avp_auth_session_state;
    FDDictionaryEntryAVP m_avp_drmp;
    FDDictionaryEntryAVP m_avp_aese_error_report;
    FDDictionaryEntryAVP m_avp_cia_flags;
    FDDictionaryEntryAVP m_avp_load_type;
    FDDictionaryEntryAVP m_avp_reachability_information;
    FDDictionaryEntryAVP m_avp_destination_host;
    FDDictionaryEntryAVP m_avp_restricted_plmn_list;
    FDDictionaryEntryAVP m_avp_msisdn;
    FDDictionaryEntryAVP m_avp_communication_pattern_set;
    FDDictionaryEntryAVP m_avp_scheduled_communication_time;
    FDDictionaryEntryAVP m_avp_charged_party;
    FDDictionaryEntryAVP m_avp_supported_services;
    FDDictionaryEntryAVP m_avp_reachability_type;
    FDDictionaryEntryAVP m_avp_group_monitoring_event_report;
    FDDictionaryEntryAVP m_avp_enhanced_coverage_restriction;
    FDDictionaryEntryAVP m_avp_nidd_authorization_update;
    FDDictionaryEntryAVP m_avp_origin_realm;
    FDDictionaryEntryAVP m_avp_origin_host;
    FDDictionaryEntryAVP m_avp_cell_global_identity;
    FDDictionaryEntryAVP m_avp_ue_reachability_configuration;
    FDDictionaryEntryAVP m_avp_location_information_configuration;
    FDDictionaryEntryAVP m_avp_monte_location_type;
    FDDictionaryEntryAVP m_avp_association_type;
    FDDictionaryEntryAVP m_avp_eps_location_information;
    FDDictionaryEntryAVP m_avp_route_record;
    FDDictionaryEntryAVP m_avp_reference_id_validity_time;
    FDDictionaryEntryAVP m_avp_service_selection;
    FDDictionaryEntryAVP m_avp_scef_id;
    FDDictionaryEntryAVP m_avp_csg_id;
    FDDictionaryEntryAVP m_avp_current_location_retrieved;
    FDDictionaryEntryAVP m_avp_monitoring_type;
    FDDictionaryEntryAVP m_avp_time_of_day_end;
    FDDictionaryEntryAVP m_avp_oc_olr;
    FDDictionaryEntryAVP m_avp_maximum_detection_time;
    FDDictionaryEntryAVP m_avp_age_of_location_information;
    FDDictionaryEntryAVP m_avp_geodetic_information;
    FDDictionaryEntryAVP m_avp_service_result;
    FDDictionaryEntryAVP m_avp_oc_reduction_percentage;
    FDDictionaryEntryAVP m_avp_service_result_code;
    FDDictionaryEntryAVP m_avp_supported_features;
    FDDictionaryEntryAVP m_avp_nidd_authorization_response;
    FDDictionaryEntryAVP m_avp_supported_monitoring_events;
    FDDictionaryEntryAVP m_avp_user_identifier;
    FDDictionaryEntryAVP m_avp_accuracy;
    FDDictionaryEntryAVP m_avp_oc_validity_duration;
    FDDictionaryEntryAVP m_avp_csg_membership_indication;
    FDDictionaryEntryAVP m_avp_dl_buffering_suggested_packet_count;
    FDDictionaryEntryAVP m_avp_maximum_latency;
    FDDictionaryEntryAVP m_avp_sourceid;
    FDDictionaryEntryAVP m_avp_aese_communication_pattern_config_status;
    FDDictionaryEntryAVP m_avp_allowed_plmn_list;
    FDDictionaryEntryAVP m_avp_sgsn_location_information;
    FDDictionaryEntryAVP m_avp_event_handling;
    FDDictionaryEntryAVP m_avp_result_code;
    FDDictionaryEntryAVP m_avp_type_of_external_identifier;
    FDDictionaryEntryAVP m_avp_s6t_hss_cause;
    FDDictionaryEntryAVP m_avp_time_of_day_start;
    FDDictionaryEntryAVP m_avp_enhanced_coverage_restriction_data;
    FDDictionaryEntryAVP m_avp_scef_reference_id;
    FDDictionaryEntryAVP m_avp_monitoring_event_configuration;
    FDDictionaryEntryAVP m_avp_load;
    FDDictionaryEntryAVP m_avp_e_utran_cell_global_identity;
    FDDictionaryEntryAVP m_avp_oc_supported_features;
    FDDictionaryEntryAVP m_avp_proxy_host;
    FDDictionaryEntryAVP m_avp_experimental_result_code;
    FDDictionaryEntryAVP m_avp_maximum_number_of_reports;
    FDDictionaryEntryAVP m_avp_cir_flags;
    FDDictionaryEntryAVP m_avp_monitoring_event_report;
    FDDictionaryEntryAVP m_avp_extended_enodeb_id;
    FDDictionaryEntryAVP m_avp_aese_communication_pattern;
    FDDictionaryEntryAVP m_avp_oc_feature_vector;
    FDDictionaryEntryAVP m_avp_monitoring_duration;
    FDDictionaryEntryAVP m_avp_loss_of_connectivity_reason;
    FDDictionaryEntryAVP m_avp_node_type;
    FDDictionaryEntryAVP m_avp_visited_plmn_id;
    FDDictionaryEntryAVP m_avp_stationary_indication;
    FDDictionaryEntryAVP m_avp_granted_validity_time;
    FDDictionaryEntryAVP m_avp_failed_avp;
    FDDictionaryEntryAVP m_avp_routing_area_identity;
    FDDictionaryEntryAVP m_avp_requested_validity_time;
    FDDictionaryEntryAVP m_avp_geographical_information;
    FDDictionaryEntryAVP m_avp_service_report;
    FDDictionaryEntryAVP m_avp_proxy_state;
    FDDictionaryEntryAVP m_avp_enodeb_id;
    FDDictionaryEntryAVP m_avp_periodic_time;
    FDDictionaryEntryAVP m_avp_nidd_authorization_request;
    FDDictionaryEntryAVP m_avp_vendor_id;
    FDDictionaryEntryAVP m_avp_oc_sequence_number;
    FDDictionaryEntryAVP m_avp_group_monitoring_event_report_item;
    FDDictionaryEntryAVP m_avp_destination_realm;
    FDDictionaryEntryAVP m_avp_session_id;
    FDDictionaryEntryAVP m_avp_user_name;
    FDDictionaryEntryAVP m_avp_roaming_information;
    FDDictionaryEntryAVP m_avp_imei_change;
    FDDictionaryEntryAVP m_avp_scef_reference_id_for_deletion;
    FDDictionaryEntryAVP m_avp_rir_flags;
    FDDictionaryEntryAVP m_avp_user_csg_information;

};

//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - ConfigurationInformationRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// COIR request from Clients.
//-
class COIRcmd : public FDCommandRequest
{
public:
    COIRcmd( Application &app );
    ~COIRcmd();

    virtual int process( FDMessageRequest *req );

private:
    COIRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - ConfigurationInformationRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a COIR
// to the Diameter application and process the corresponding answer.
//
class COIRreq : public FDMessageRequest
{
public:
    COIRreq( Application &app );
    virtual ~COIRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    COIRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - ReportingInformationRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// REIR request from Clients.
//-
class REIRcmd : public FDCommandRequest
{
public:
    REIRcmd( Application &app );
    ~REIRcmd();

    virtual int process( FDMessageRequest *req );

private:
    REIRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - ReportingInformationRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a REIR
// to the Diameter application and process the corresponding answer.
//
class REIRreq : public FDMessageRequest
{
public:
    REIRreq( Application &app );
    virtual ~REIRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    REIRreq();
    Application &m_app;
    FDSession m_session;
};
 
//Forward declaration of Application;
class Application;

//
// DIAMETER REQUEST Processing - NiddInformationRequest (Application / Receiver)
// This message is used by a peer acting as the application to listen for
// NIIR request from Clients.
//-
class NIIRcmd : public FDCommandRequest
{
public:
    NIIRcmd( Application &app );
    ~NIIRcmd();

    virtual int process( FDMessageRequest *req );

private:
    NIIRcmd();

    Application &m_app;
};

//
// DIAMETER REQUEST - NiddInformationRequest (Client / Sender)
// This message is used by a peer acting as a Client to send a NIIR
// to the Diameter application and process the corresponding answer.
//
class NIIRreq : public FDMessageRequest
{
public:
    NIIRreq( Application &app );
    virtual ~NIIRreq();

    virtual void processAnswer( FDMessageAnswer &ans );
 
    Application &getApplication() { return m_app; }
    const std::string &getSessionId() { return m_session.getSessionId(); }

private:
    NIIRreq();
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

class ProxyInfoExtractor : public FDExtractor
{
 public:
    ProxyInfoExtractor( FDExtractor &parent, Dictionary &dict );
    ~ProxyInfoExtractor();

    FDExtractorAvp proxy_host;
    FDExtractorAvp proxy_state;
};

class ExperimentalResultExtractor : public FDExtractor
{
 public:
    ExperimentalResultExtractor( FDExtractor &parent, Dictionary &dict );
    ~ExperimentalResultExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp experimental_result_code;
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
    FDExtractorAvp destination_host;
};

class AeseErrorReportExtractor : public FDExtractor
{
 public:
    AeseErrorReportExtractor( FDExtractor &parent, Dictionary &dict );
    ~AeseErrorReportExtractor();

    ServiceResultExtractor service_result;
};

class RestrictedPlmnListExtractor : public FDExtractor
{
 public:
    RestrictedPlmnListExtractor( FDExtractor &parent, Dictionary &dict );
    ~RestrictedPlmnListExtractor();

    FDExtractorAvpList visited_plmn_id;
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

class SupportedServicesExtractor : public FDExtractor
{
 public:
    SupportedServicesExtractor( FDExtractor &parent, Dictionary &dict );
    ~SupportedServicesExtractor();

    FDExtractorAvp supported_monitoring_events;
    FDExtractorAvp node_type;
};

class UserIdentifierExtractor : public FDExtractor
{
 public:
    UserIdentifierExtractor( FDExtractor &parent, Dictionary &dict );
    ~UserIdentifierExtractor();

    FDExtractorAvp user_name;
    FDExtractorAvp msisdn;
    FDExtractorAvp external_identifier;
    FDExtractorAvp type_of_external_identifier;
};

class SgsnLocationInformationExtractor : public FDExtractor
{
 public:
    SgsnLocationInformationExtractor( FDExtractor &parent, Dictionary &dict );
    ~SgsnLocationInformationExtractor();

    FDExtractorAvp cell_global_identity;
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

class GroupMonitoringEventReportItemExtractor : public FDExtractor
{
 public:
    GroupMonitoringEventReportItemExtractor( FDExtractor &parent, Dictionary &dict );
    ~GroupMonitoringEventReportItemExtractor();

    UserIdentifierExtractor user_identifier;
    FDExtractorAvp visited_plmn_id;
    FDExtractorAvp roaming_information;
    FDExtractorAvp reachability_information;
    FDExtractorAvp maximum_ue_availability_time;
    EpsLocationInformationExtractor eps_location_information;
    FDExtractorAvp monitoring_type;
    ServiceReportExtractorList service_report;
    FDExtractorAvp s6t_hss_cause;
};

class GroupMonitoringEventReportItemExtractorList : public FDExtractorList
{
 public:
    GroupMonitoringEventReportItemExtractorList( FDExtractor &parent, Dictionary &dict );
    ~GroupMonitoringEventReportItemExtractorList();

    FDExtractor *createExtractor() { return new GroupMonitoringEventReportItemExtractor( getParent(), m_dict ); }
    std::list<GroupMonitoringEventReportItemExtractor*> &getList() { return (std::list<GroupMonitoringEventReportItemExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class GroupMonitoringEventReportExtractor : public FDExtractor
{
 public:
    GroupMonitoringEventReportExtractor( FDExtractor &parent, Dictionary &dict );
    ~GroupMonitoringEventReportExtractor();

    FDExtractorAvp scef_reference_id;
    FDExtractorAvp scef_id;
    GroupMonitoringEventReportItemExtractorList group_monitoring_event_report_item;
};

class AllowedPlmnListExtractor : public FDExtractor
{
 public:
    AllowedPlmnListExtractor( FDExtractor &parent, Dictionary &dict );
    ~AllowedPlmnListExtractor();

    FDExtractorAvpList visited_plmn_id;
};

class EnhancedCoverageRestrictionExtractor : public FDExtractor
{
 public:
    EnhancedCoverageRestrictionExtractor( FDExtractor &parent, Dictionary &dict );
    ~EnhancedCoverageRestrictionExtractor();

    RestrictedPlmnListExtractor restricted_plmn_list;
    AllowedPlmnListExtractor allowed_plmn_list;
};

class NiddAuthorizationUpdateExtractor : public FDExtractor
{
 public:
    NiddAuthorizationUpdateExtractor( FDExtractor &parent, Dictionary &dict );
    ~NiddAuthorizationUpdateExtractor();

    FDExtractorAvp msisdn;
    FDExtractorAvp user_name;
    FDExtractorAvp external_identifier;
    FDExtractorAvp granted_validity_time;
};

class UeReachabilityConfigurationExtractor : public FDExtractor
{
 public:
    UeReachabilityConfigurationExtractor( FDExtractor &parent, Dictionary &dict );
    ~UeReachabilityConfigurationExtractor();

    FDExtractorAvp reachability_type;
    FDExtractorAvp maximum_latency;
    FDExtractorAvp maximum_response_time;
};

class LocationInformationConfigurationExtractor : public FDExtractor
{
 public:
    LocationInformationConfigurationExtractor( FDExtractor &parent, Dictionary &dict );
    ~LocationInformationConfigurationExtractor();

    FDExtractorAvp monte_location_type;
    FDExtractorAvp accuracy;
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

class SupportedFeaturesExtractor : public FDExtractor
{
 public:
    SupportedFeaturesExtractor( FDExtractor &parent, Dictionary &dict );
    ~SupportedFeaturesExtractor();

    FDExtractorAvp vendor_id;
    FDExtractorAvp feature_list_id;
    FDExtractorAvp feature_list;
};

class NiddAuthorizationResponseExtractor : public FDExtractor
{
 public:
    NiddAuthorizationResponseExtractor( FDExtractor &parent, Dictionary &dict );
    ~NiddAuthorizationResponseExtractor();

    FDExtractorAvp msisdn;
    FDExtractorAvp user_name;
    FDExtractorAvp external_identifier;
    FDExtractorAvp granted_validity_time;
};

class AeseCommunicationPatternConfigStatusExtractor : public FDExtractor
{
 public:
    AeseCommunicationPatternConfigStatusExtractor( FDExtractor &parent, Dictionary &dict );
    ~AeseCommunicationPatternConfigStatusExtractor();

    FDExtractorAvp scef_reference_id;
    FDExtractorAvp scef_id;
    AeseErrorReportExtractor aese_error_report;
};

class EnhancedCoverageRestrictionDataExtractor : public FDExtractor
{
 public:
    EnhancedCoverageRestrictionDataExtractor( FDExtractor &parent, Dictionary &dict );
    ~EnhancedCoverageRestrictionDataExtractor();

    EnhancedCoverageRestrictionExtractor enhanced_coverage_restriction;
    FDExtractorAvp visited_plmn_id;
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
    FDExtractorAvp maximum_detection_time;
    UeReachabilityConfigurationExtractor ue_reachability_configuration;
    LocationInformationConfigurationExtractor location_information_configuration;
    FDExtractorAvp association_type;
    FDExtractorAvp dl_buffering_suggested_packet_count;
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

class MonitoringEventReportExtractor : public FDExtractor
{
 public:
    MonitoringEventReportExtractor( FDExtractor &parent, Dictionary &dict );
    ~MonitoringEventReportExtractor();

    FDExtractorAvp scef_reference_id;
    FDExtractorAvp scef_id;
    FDExtractorAvp visited_plmn_id;
    FDExtractorAvp roaming_information;
    FDExtractorAvp imei_change;
    FDExtractorAvp reachability_information;
    FDExtractorAvp maximum_ue_availability_time;
    EpsLocationInformationExtractor eps_location_information;
    FDExtractorAvp monitoring_type;
    FDExtractorAvp event_handling;
    ServiceReportExtractorList service_report;
    FDExtractorAvp loss_of_connectivity_reason;
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

class FailedAvpExtractor : public FDExtractor
{
 public:
    FailedAvpExtractor( FDExtractor &parent, Dictionary &dict );
    ~FailedAvpExtractor();


};

class NiddAuthorizationRequestExtractor : public FDExtractor
{
 public:
    NiddAuthorizationRequestExtractor( FDExtractor &parent, Dictionary &dict );
    ~NiddAuthorizationRequestExtractor();

    FDExtractorAvp service_selection;
    FDExtractorAvp requested_validity_time;
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

class ConfigurationInformationRequestExtractor : public FDExtractor
{
 public:
    ConfigurationInformationRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~ConfigurationInformationRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    UserIdentifierExtractor user_identifier;
    OcSupportedFeaturesExtractor oc_supported_features;
    SupportedFeaturesExtractorList supported_features;
    MonitoringEventConfigurationExtractorList monitoring_event_configuration;
    FDExtractorAvp cir_flags;
    AeseCommunicationPatternExtractorList aese_communication_pattern;
    EnhancedCoverageRestrictionExtractor enhanced_coverage_restriction;
    FDExtractorAvp group_reporting_guard_timer;
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

class AeseCommunicationPatternConfigStatusExtractorList : public FDExtractorList
{
 public:
    AeseCommunicationPatternConfigStatusExtractorList( FDExtractor &parent, Dictionary &dict );
    ~AeseCommunicationPatternConfigStatusExtractorList();

    FDExtractor *createExtractor() { return new AeseCommunicationPatternConfigStatusExtractor( getParent(), m_dict ); }
    std::list<AeseCommunicationPatternConfigStatusExtractor*> &getList() { return (std::list<AeseCommunicationPatternConfigStatusExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class SupportedServicesExtractorList : public FDExtractorList
{
 public:
    SupportedServicesExtractorList( FDExtractor &parent, Dictionary &dict );
    ~SupportedServicesExtractorList();

    FDExtractor *createExtractor() { return new SupportedServicesExtractor( getParent(), m_dict ); }
    std::list<SupportedServicesExtractor*> &getList() { return (std::list<SupportedServicesExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ConfigurationInformationAnswerExtractor : public FDExtractor
{
 public:
    ConfigurationInformationAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~ConfigurationInformationAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    LoadExtractorList load;
    SupportedFeaturesExtractorList supported_features;
    UserIdentifierExtractor user_identifier;
    MonitoringEventReportExtractorList monitoring_event_report;
    MonitoringEventConfigStatusExtractorList monitoring_event_config_status;
    AeseCommunicationPatternConfigStatusExtractorList aese_communication_pattern_config_status;
    SupportedServicesExtractorList supported_services;
    FDExtractorAvp s6t_hss_cause;
    EnhancedCoverageRestrictionDataExtractor enhanced_coverage_restriction_data;
    FDExtractorAvp cia_flags;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class GroupMonitoringEventReportExtractorList : public FDExtractorList
{
 public:
    GroupMonitoringEventReportExtractorList( FDExtractor &parent, Dictionary &dict );
    ~GroupMonitoringEventReportExtractorList();

    FDExtractor *createExtractor() { return new GroupMonitoringEventReportExtractor( getParent(), m_dict ); }
    std::list<GroupMonitoringEventReportExtractor*> &getList() { return (std::list<GroupMonitoringEventReportExtractor*>&)FDExtractorList::getList(); }

 private:

    Dictionary &m_dict;
};

class ReportingInformationRequestExtractor : public FDExtractor
{
 public:
    ReportingInformationRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~ReportingInformationRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    SupportedFeaturesExtractorList supported_features;
    UserIdentifierExtractor user_identifier;
    MonitoringEventReportExtractorList monitoring_event_report;
    GroupMonitoringEventReportExtractorList group_monitoring_event_report;
    FDExtractorAvp rir_flags;
    SupportedServicesExtractorList supported_services;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class ReportingInformationAnswerExtractor : public FDExtractor
{
 public:
    ReportingInformationAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~ReportingInformationAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    SupportedFeaturesExtractorList supported_features;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class NiddInformationRequestExtractor : public FDExtractor
{
 public:
    NiddInformationRequestExtractor( FDMessage &msg, Dictionary &dict );
    ~NiddInformationRequestExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    FDExtractorAvp destination_host;
    FDExtractorAvp destination_realm;
    UserIdentifierExtractor user_identifier;
    OcSupportedFeaturesExtractor oc_supported_features;
    SupportedFeaturesExtractorList supported_features;
    NiddAuthorizationRequestExtractor nidd_authorization_request;
    NiddAuthorizationUpdateExtractor nidd_authorization_update;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};

class NiddInformationAnswerExtractor : public FDExtractor
{
 public:
    NiddInformationAnswerExtractor( FDMessage &msg, Dictionary &dict );
    ~NiddInformationAnswerExtractor();

    FDExtractorAvp session_id;
    FDExtractorAvp drmp;
    FDExtractorAvp result_code;
    ExperimentalResultExtractor experimental_result;
    FDExtractorAvp auth_session_state;
    FDExtractorAvp origin_host;
    FDExtractorAvp origin_realm;
    OcSupportedFeaturesExtractor oc_supported_features;
    OcOlrExtractor oc_olr;
    LoadExtractorList load;
    SupportedFeaturesExtractorList supported_features;
    UserIdentifierExtractor user_identifier;
    NiddAuthorizationResponseExtractor nidd_authorization_response;
    FailedAvpExtractor failed_avp;
    ProxyInfoExtractorList proxy_info;
    FDExtractorAvpList route_record;
};


}

#endif // __S6T_H

