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

#include "s6t.h"
#include "s6t_impl.h"

namespace s6t {

Dictionary::Dictionary()
    : m_app( "S6t"),
    m_cmd_coir( "Configuration-Information-Request" ),
    m_cmd_coia( "Configuration-Information-Answer" ),
    m_cmd_reir( "Reporting-Information-Request" ),
    m_cmd_reia( "Reporting-Information-Answer" ),
    m_cmd_niir( "NIDD-Information-Request" ),
    m_cmd_niia( "NIDD-Information-Answer" ),
    m_vnd_3gpp( "3GPP" ),
    m_avp_group_reporting_guard_timer( "Group-Reporting-Guard-Timer", m_vnd_3gpp.getId() ),
    m_avp_monitoring_event_config_status( "Monitoring-Event-Config-Status", m_vnd_3gpp.getId() ),
    m_avp_oc_report_type( "OC-Report-Type" ),
    m_avp_service_area_identity( "Service-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_day_of_week_mask( "Day-Of-Week-Mask" ),
    m_avp_maximum_ue_availability_time( "Maximum-UE-Availability-Time", m_vnd_3gpp.getId() ),
    m_avp_maximum_response_time( "Maximum-Response-Time", m_vnd_3gpp.getId() ),
    m_avp_proxy_info( "Proxy-Info" ),
    m_avp_csg_access_mode( "CSG-Access-Mode", m_vnd_3gpp.getId() ),
    m_avp_tracking_area_identity( "Tracking-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_load_value( "Load-Value" ),
    m_avp_feature_list( "Feature-List", m_vnd_3gpp.getId() ),
    m_avp_external_identifier( "External-Identifier", m_vnd_3gpp.getId() ),
    m_avp_feature_list_id( "Feature-List-ID", m_vnd_3gpp.getId() ),
    m_avp_experimental_result( "Experimental-Result" ),
    m_avp_periodic_communication_indicator( "Periodic-Communication-Indicator", m_vnd_3gpp.getId() ),
    m_avp_communication_duration_time( "Communication-Duration-Time", m_vnd_3gpp.getId() ),
    m_avp_mme_location_information( "MME-Location-Information", m_vnd_3gpp.getId() ),
    m_avp_auth_session_state( "Auth-Session-State" ),
    m_avp_drmp( "DRMP" ),
    m_avp_aese_error_report( "AESE-Error-Report", m_vnd_3gpp.getId() ),
    m_avp_cia_flags( "CIA-Flags", m_vnd_3gpp.getId() ),
    m_avp_load_type( "Load-Type" ),
    m_avp_reachability_information( "Reachability-Information", m_vnd_3gpp.getId() ),
    m_avp_destination_host( "Destination-Host" ),
    m_avp_restricted_plmn_list( "Restricted-PLMN-List", m_vnd_3gpp.getId() ),
    m_avp_msisdn( "MSISDN", m_vnd_3gpp.getId() ),
    m_avp_communication_pattern_set( "Communication-Pattern-Set", m_vnd_3gpp.getId() ),
    m_avp_scheduled_communication_time( "Scheduled-Communication-Time", m_vnd_3gpp.getId() ),
    m_avp_charged_party( "Charged-Party", m_vnd_3gpp.getId() ),
    m_avp_supported_services( "Supported-Services", m_vnd_3gpp.getId() ),
    m_avp_reachability_type( "Reachability-Type", m_vnd_3gpp.getId() ),
    m_avp_group_monitoring_event_report( "Group-Monitoring-Event-Report", m_vnd_3gpp.getId() ),
    m_avp_enhanced_coverage_restriction( "Enhanced-Coverage-Restriction", m_vnd_3gpp.getId() ),
    m_avp_nidd_authorization_update( "NIDD-Authorization-Update", m_vnd_3gpp.getId() ),
    m_avp_origin_realm( "Origin-Realm" ),
    m_avp_origin_host( "Origin-Host" ),
    m_avp_cell_global_identity( "Cell-Global-Identity", m_vnd_3gpp.getId() ),
    m_avp_ue_reachability_configuration( "UE-Reachability-Configuration", m_vnd_3gpp.getId() ),
    m_avp_location_information_configuration( "Location-Information-Configuration", m_vnd_3gpp.getId() ),
    m_avp_monte_location_type( "MONTE-Location-Type", m_vnd_3gpp.getId() ),
    m_avp_association_type( "Association-Type", m_vnd_3gpp.getId() ),
    m_avp_eps_location_information( "EPS-Location-Information", m_vnd_3gpp.getId() ),
    m_avp_route_record( "Route-Record" ),
    m_avp_reference_id_validity_time( "Reference-ID-Validity-Time", m_vnd_3gpp.getId() ),
    m_avp_service_selection( "Service-Selection" ),
    m_avp_scef_id( "SCEF-ID", m_vnd_3gpp.getId() ),
    m_avp_csg_id( "CSG-Id", m_vnd_3gpp.getId() ),
    m_avp_current_location_retrieved( "Current-Location-Retrieved", m_vnd_3gpp.getId() ),
    m_avp_monitoring_type( "Monitoring-Type", m_vnd_3gpp.getId() ),
    m_avp_time_of_day_end( "Time-Of-Day-End" ),
    m_avp_oc_olr( "OC-OLR" ),
    m_avp_maximum_detection_time( "Maximum-Detection-Time", m_vnd_3gpp.getId() ),
    m_avp_age_of_location_information( "Age-Of-Location-Information", m_vnd_3gpp.getId() ),
    m_avp_geodetic_information( "Geodetic-Information", m_vnd_3gpp.getId() ),
    m_avp_service_result( "Service-Result", m_vnd_3gpp.getId() ),
    m_avp_oc_reduction_percentage( "OC-Reduction-Percentage" ),
    m_avp_service_result_code( "Service-Result-Code", m_vnd_3gpp.getId() ),
    m_avp_supported_features( "Supported-Features", m_vnd_3gpp.getId() ),
    m_avp_nidd_authorization_response( "NIDD-Authorization-Response", m_vnd_3gpp.getId() ),
    m_avp_supported_monitoring_events( "Supported-Monitoring-Events", m_vnd_3gpp.getId() ),
    m_avp_user_identifier( "User-Identifier", m_vnd_3gpp.getId() ),
    m_avp_accuracy( "Accuracy", m_vnd_3gpp.getId() ),
    m_avp_oc_validity_duration( "OC-Validity-Duration" ),
    m_avp_csg_membership_indication( "CSG-Membership-Indication", m_vnd_3gpp.getId() ),
    m_avp_dl_buffering_suggested_packet_count( "DL-Buffering-Suggested-Packet-Count", m_vnd_3gpp.getId() ),
    m_avp_maximum_latency( "Maximum-Latency", m_vnd_3gpp.getId() ),
    m_avp_sourceid( "SourceID" ),
    m_avp_aese_communication_pattern_config_status( "AESE-Communication-Pattern-Config-Status", m_vnd_3gpp.getId() ),
    m_avp_allowed_plmn_list( "Allowed-PLMN-List", m_vnd_3gpp.getId() ),
    m_avp_sgsn_location_information( "SGSN-Location-Information", m_vnd_3gpp.getId() ),
    m_avp_event_handling( "Event-Handling", m_vnd_3gpp.getId() ),
    m_avp_result_code( "Result-Code" ),
    m_avp_type_of_external_identifier( "Type-Of-External-Identifier", m_vnd_3gpp.getId() ),
    m_avp_s6t_hss_cause( "S6t-HSS-Cause", m_vnd_3gpp.getId() ),
    m_avp_time_of_day_start( "Time-Of-Day-Start" ),
    m_avp_enhanced_coverage_restriction_data( "Enhanced-Coverage-Restriction-Data", m_vnd_3gpp.getId() ),
    m_avp_scef_reference_id( "SCEF-Reference-ID", m_vnd_3gpp.getId() ),
    m_avp_monitoring_event_configuration( "Monitoring-Event-Configuration", m_vnd_3gpp.getId() ),
    m_avp_load( "Load" ),
    m_avp_e_utran_cell_global_identity( "E-UTRAN-Cell-Global-Identity", m_vnd_3gpp.getId() ),
    m_avp_oc_supported_features( "OC-Supported-Features" ),
    m_avp_proxy_host( "Proxy-Host" ),
    m_avp_experimental_result_code( "Experimental-Result-Code" ),
    m_avp_maximum_number_of_reports( "Maximum-Number-of-Reports", m_vnd_3gpp.getId() ),
    m_avp_cir_flags( "CIR-Flags", m_vnd_3gpp.getId() ),
    m_avp_monitoring_event_report( "Monitoring-Event-Report", m_vnd_3gpp.getId() ),
    m_avp_extended_enodeb_id( "Extended-eNodeB-Id", m_vnd_3gpp.getId() ),
    m_avp_aese_communication_pattern( "AESE-Communication-Pattern", m_vnd_3gpp.getId() ),
    m_avp_oc_feature_vector( "OC-Feature-Vector" ),
    m_avp_monitoring_duration( "Monitoring-Duration", m_vnd_3gpp.getId() ),
    m_avp_loss_of_connectivity_reason( "Loss-Of-Connectivity-Reason", m_vnd_3gpp.getId() ),
    m_avp_node_type( "Node-Type", m_vnd_3gpp.getId() ),
    m_avp_visited_plmn_id( "Visited-PLMN-Id", m_vnd_3gpp.getId() ),
    m_avp_stationary_indication( "Stationary-Indication", m_vnd_3gpp.getId() ),
    m_avp_granted_validity_time( "Granted-Validity-Time", m_vnd_3gpp.getId() ),
    m_avp_failed_avp( "Failed-AVP" ),
    m_avp_routing_area_identity( "Routing-Area-Identity", m_vnd_3gpp.getId() ),
    m_avp_requested_validity_time( "Requested-Validity-Time", m_vnd_3gpp.getId() ),
    m_avp_geographical_information( "Geographical-Information", m_vnd_3gpp.getId() ),
    m_avp_service_report( "Service-Report", m_vnd_3gpp.getId() ),
    m_avp_proxy_state( "Proxy-State" ),
    m_avp_enodeb_id( "eNodeB-Id", m_vnd_3gpp.getId() ),
    m_avp_periodic_time( "Periodic-Time", m_vnd_3gpp.getId() ),
    m_avp_nidd_authorization_request( "NIDD-Authorization-Request", m_vnd_3gpp.getId() ),
    m_avp_vendor_id( "Vendor-Id" ),
    m_avp_oc_sequence_number( "OC-Sequence-Number" ),
    m_avp_group_monitoring_event_report_item( "Group-Monitoring-Event-Report-Item", m_vnd_3gpp.getId() ),
    m_avp_destination_realm( "Destination-Realm" ),
    m_avp_session_id( "Session-Id" ),
    m_avp_user_name( "User-Name" ),
    m_avp_roaming_information( "Roaming-Information", m_vnd_3gpp.getId() ),
    m_avp_imei_change( "IMEI-Change", m_vnd_3gpp.getId() ),
    m_avp_scef_reference_id_for_deletion( "SCEF-Reference-ID-for-Deletion", m_vnd_3gpp.getId() ),
    m_avp_rir_flags( "RIR-Flags", m_vnd_3gpp.getId() ),
    m_avp_user_csg_information( "User-CSG-Information", m_vnd_3gpp.getId() )
{
    std::cout << "Registering s6t dictionary" << std::endl;
};

Dictionary::~Dictionary()
{
};

COIRcmd::COIRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdCOIR() ),
      m_app( app )
{
}

COIRcmd::~COIRcmd()
{
}

COIRreq::COIRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdCOIR() ),
      m_app(app)
{
}

COIRreq::~COIRreq()
{
}

 
REIRcmd::REIRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdREIR() ),
      m_app( app )
{
}

REIRcmd::~REIRcmd()
{
}

REIRreq::REIRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdREIR() ),
      m_app(app)
{
}

REIRreq::~REIRreq()
{
}

 
NIIRcmd::NIIRcmd( Application & app )
    : FDCommandRequest( app.getDict().cmdNIIR() ),
      m_app( app )
{
}

NIIRcmd::~NIIRcmd()
{
}

NIIRreq::NIIRreq( Application &app )
    : FDMessageRequest( &app.getDict().app(), &app.getDict().cmdNIIR() ),
      m_app(app)
{
}

NIIRreq::~NIIRreq()
{
}

 
ApplicationBase::ApplicationBase()
{
    setDictionaryEntry( &m_dict.app() );

};

ApplicationBase::~ApplicationBase()
{
};



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
    extended_enodeb_id( *this, dict.avpExtendedEnodebId() ),
    destination_host( *this, dict.avpDestinationHost() )
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
    add( destination_host );
}

MmeLocationInformationExtractor::~MmeLocationInformationExtractor()
{
}

AeseErrorReportExtractor::AeseErrorReportExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAeseErrorReport() ),
    service_result( *this, dict )
{
    add( service_result );
}

AeseErrorReportExtractor::~AeseErrorReportExtractor()
{
}

RestrictedPlmnListExtractor::RestrictedPlmnListExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpRestrictedPlmnList() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() )
{
    add( visited_plmn_id );
}

RestrictedPlmnListExtractor::~RestrictedPlmnListExtractor()
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

SupportedServicesExtractor::SupportedServicesExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSupportedServices() ),
    supported_monitoring_events( *this, dict.avpSupportedMonitoringEvents() ),
    node_type( *this, dict.avpNodeType() )
{
    add( supported_monitoring_events );
    add( node_type );
}

SupportedServicesExtractor::~SupportedServicesExtractor()
{
}

UserIdentifierExtractor::UserIdentifierExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUserIdentifier() ),
    user_name( *this, dict.avpUserName() ),
    msisdn( *this, dict.avpMsisdn() ),
    external_identifier( *this, dict.avpExternalIdentifier() ),
    type_of_external_identifier( *this, dict.avpTypeOfExternalIdentifier() )
{
    add( user_name );
    add( msisdn );
    add( external_identifier );
    add( type_of_external_identifier );
}

UserIdentifierExtractor::~UserIdentifierExtractor()
{
}

SgsnLocationInformationExtractor::SgsnLocationInformationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpSgsnLocationInformation() ),
    cell_global_identity( *this, dict.avpCellGlobalIdentity() ),
    service_area_identity( *this, dict.avpServiceAreaIdentity() ),
    routing_area_identity( *this, dict.avpRoutingAreaIdentity() ),
    geographical_information( *this, dict.avpGeographicalInformation() ),
    geodetic_information( *this, dict.avpGeodeticInformation() ),
    current_location_retrieved( *this, dict.avpCurrentLocationRetrieved() ),
    age_of_location_information( *this, dict.avpAgeOfLocationInformation() ),
    user_csg_information( *this, dict )
{
    add( cell_global_identity );
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

GroupMonitoringEventReportItemExtractor::GroupMonitoringEventReportItemExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpGroupMonitoringEventReportItem() ),
    user_identifier( *this, dict ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() ),
    roaming_information( *this, dict.avpRoamingInformation() ),
    reachability_information( *this, dict.avpReachabilityInformation() ),
    maximum_ue_availability_time( *this, dict.avpMaximumUeAvailabilityTime() ),
    eps_location_information( *this, dict ),
    monitoring_type( *this, dict.avpMonitoringType() ),
    service_report( *this, dict ),
    s6t_hss_cause( *this, dict.avpS6tHssCause() )
{
    add( user_identifier );
    add( visited_plmn_id );
    add( roaming_information );
    add( reachability_information );
    add( maximum_ue_availability_time );
    add( eps_location_information );
    add( monitoring_type );
    add( service_report );
    add( s6t_hss_cause );
}

GroupMonitoringEventReportItemExtractor::~GroupMonitoringEventReportItemExtractor()
{
}

GroupMonitoringEventReportItemExtractorList::GroupMonitoringEventReportItemExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpGroupMonitoringEventReportItem() ),
    m_dict( dict )
{
}

GroupMonitoringEventReportItemExtractorList::~GroupMonitoringEventReportItemExtractorList()
{
}

GroupMonitoringEventReportExtractor::GroupMonitoringEventReportExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpGroupMonitoringEventReport() ),
    scef_reference_id( *this, dict.avpScefReferenceId() ),
    scef_id( *this, dict.avpScefId() ),
    group_monitoring_event_report_item( *this, dict )
{
    add( scef_reference_id );
    add( scef_id );
    add( group_monitoring_event_report_item );
}

GroupMonitoringEventReportExtractor::~GroupMonitoringEventReportExtractor()
{
}

AllowedPlmnListExtractor::AllowedPlmnListExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAllowedPlmnList() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() )
{
    add( visited_plmn_id );
}

AllowedPlmnListExtractor::~AllowedPlmnListExtractor()
{
}

EnhancedCoverageRestrictionExtractor::EnhancedCoverageRestrictionExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEnhancedCoverageRestriction() ),
    restricted_plmn_list( *this, dict ),
    allowed_plmn_list( *this, dict )
{
    add( restricted_plmn_list );
    add( allowed_plmn_list );
}

EnhancedCoverageRestrictionExtractor::~EnhancedCoverageRestrictionExtractor()
{
}

NiddAuthorizationUpdateExtractor::NiddAuthorizationUpdateExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpNiddAuthorizationUpdate() ),
    msisdn( *this, dict.avpMsisdn() ),
    user_name( *this, dict.avpUserName() ),
    external_identifier( *this, dict.avpExternalIdentifier() ),
    granted_validity_time( *this, dict.avpGrantedValidityTime() )
{
    add( msisdn );
    add( user_name );
    add( external_identifier );
    add( granted_validity_time );
}

NiddAuthorizationUpdateExtractor::~NiddAuthorizationUpdateExtractor()
{
}

UeReachabilityConfigurationExtractor::UeReachabilityConfigurationExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpUeReachabilityConfiguration() ),
    reachability_type( *this, dict.avpReachabilityType() ),
    maximum_latency( *this, dict.avpMaximumLatency() ),
    maximum_response_time( *this, dict.avpMaximumResponseTime() )
{
    add( reachability_type );
    add( maximum_latency );
    add( maximum_response_time );
}

UeReachabilityConfigurationExtractor::~UeReachabilityConfigurationExtractor()
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

NiddAuthorizationResponseExtractor::NiddAuthorizationResponseExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpNiddAuthorizationResponse() ),
    msisdn( *this, dict.avpMsisdn() ),
    user_name( *this, dict.avpUserName() ),
    external_identifier( *this, dict.avpExternalIdentifier() ),
    granted_validity_time( *this, dict.avpGrantedValidityTime() )
{
    add( msisdn );
    add( user_name );
    add( external_identifier );
    add( granted_validity_time );
}

NiddAuthorizationResponseExtractor::~NiddAuthorizationResponseExtractor()
{
}

AeseCommunicationPatternConfigStatusExtractor::AeseCommunicationPatternConfigStatusExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpAeseCommunicationPatternConfigStatus() ),
    scef_reference_id( *this, dict.avpScefReferenceId() ),
    scef_id( *this, dict.avpScefId() ),
    aese_error_report( *this, dict )
{
    add( scef_reference_id );
    add( scef_id );
    add( aese_error_report );
}

AeseCommunicationPatternConfigStatusExtractor::~AeseCommunicationPatternConfigStatusExtractor()
{
}

EnhancedCoverageRestrictionDataExtractor::EnhancedCoverageRestrictionDataExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpEnhancedCoverageRestrictionData() ),
    enhanced_coverage_restriction( *this, dict ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() )
{
    add( enhanced_coverage_restriction );
    add( visited_plmn_id );
}

EnhancedCoverageRestrictionDataExtractor::~EnhancedCoverageRestrictionDataExtractor()
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
    maximum_detection_time( *this, dict.avpMaximumDetectionTime() ),
    ue_reachability_configuration( *this, dict ),
    location_information_configuration( *this, dict ),
    association_type( *this, dict.avpAssociationType() ),
    dl_buffering_suggested_packet_count( *this, dict.avpDlBufferingSuggestedPacketCount() )
{
    add( scef_reference_id );
    add( scef_id );
    add( monitoring_type );
    add( scef_reference_id_for_deletion );
    add( maximum_number_of_reports );
    add( monitoring_duration );
    add( charged_party );
    add( maximum_detection_time );
    add( ue_reachability_configuration );
    add( location_information_configuration );
    add( association_type );
    add( dl_buffering_suggested_packet_count );
}

MonitoringEventConfigurationExtractor::~MonitoringEventConfigurationExtractor()
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

MonitoringEventReportExtractor::MonitoringEventReportExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpMonitoringEventReport() ),
    scef_reference_id( *this, dict.avpScefReferenceId() ),
    scef_id( *this, dict.avpScefId() ),
    visited_plmn_id( *this, dict.avpVisitedPlmnId() ),
    roaming_information( *this, dict.avpRoamingInformation() ),
    imei_change( *this, dict.avpImeiChange() ),
    reachability_information( *this, dict.avpReachabilityInformation() ),
    maximum_ue_availability_time( *this, dict.avpMaximumUeAvailabilityTime() ),
    eps_location_information( *this, dict ),
    monitoring_type( *this, dict.avpMonitoringType() ),
    event_handling( *this, dict.avpEventHandling() ),
    service_report( *this, dict ),
    loss_of_connectivity_reason( *this, dict.avpLossOfConnectivityReason() )
{
    add( scef_reference_id );
    add( scef_id );
    add( visited_plmn_id );
    add( roaming_information );
    add( imei_change );
    add( reachability_information );
    add( maximum_ue_availability_time );
    add( eps_location_information );
    add( monitoring_type );
    add( event_handling );
    add( service_report );
    add( loss_of_connectivity_reason );
}

MonitoringEventReportExtractor::~MonitoringEventReportExtractor()
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

FailedAvpExtractor::FailedAvpExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpFailedAvp() )
{

}

FailedAvpExtractor::~FailedAvpExtractor()
{
}

NiddAuthorizationRequestExtractor::NiddAuthorizationRequestExtractor( FDExtractor &parent, Dictionary &dict )
   : FDExtractor( parent, dict.avpNiddAuthorizationRequest() ),
    service_selection( *this, dict.avpServiceSelection() ),
    requested_validity_time( *this, dict.avpRequestedValidityTime() )
{
    add( service_selection );
    add( requested_validity_time );
}

NiddAuthorizationRequestExtractor::~NiddAuthorizationRequestExtractor()
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

MonitoringEventConfigurationExtractorList::MonitoringEventConfigurationExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpMonitoringEventConfiguration() ),
    m_dict( dict )
{
}

MonitoringEventConfigurationExtractorList::~MonitoringEventConfigurationExtractorList()
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

ProxyInfoExtractorList::ProxyInfoExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpProxyInfo() ),
    m_dict( dict )
{
}

ProxyInfoExtractorList::~ProxyInfoExtractorList()
{
}

ConfigurationInformationRequestExtractor::ConfigurationInformationRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    user_identifier( *this, dict ),
    oc_supported_features( *this, dict ),
    supported_features( *this, dict ),
    monitoring_event_configuration( *this, dict ),
    cir_flags( *this, dict.avpCirFlags() ),
    aese_communication_pattern( *this, dict ),
    enhanced_coverage_restriction( *this, dict ),
    group_reporting_guard_timer( *this, dict.avpGroupReportingGuardTimer() ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( destination_host );
    add( destination_realm );
    add( user_identifier );
    add( oc_supported_features );
    add( supported_features );
    add( monitoring_event_configuration );
    add( cir_flags );
    add( aese_communication_pattern );
    add( enhanced_coverage_restriction );
    add( group_reporting_guard_timer );
    add( proxy_info );
    add( route_record );
}

ConfigurationInformationRequestExtractor::~ConfigurationInformationRequestExtractor()
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

AeseCommunicationPatternConfigStatusExtractorList::AeseCommunicationPatternConfigStatusExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpAeseCommunicationPatternConfigStatus() ),
    m_dict( dict )
{
}

AeseCommunicationPatternConfigStatusExtractorList::~AeseCommunicationPatternConfigStatusExtractorList()
{
}

SupportedServicesExtractorList::SupportedServicesExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpSupportedServices() ),
    m_dict( dict )
{
}

SupportedServicesExtractorList::~SupportedServicesExtractorList()
{
}

ConfigurationInformationAnswerExtractor::ConfigurationInformationAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    load( *this, dict ),
    supported_features( *this, dict ),
    user_identifier( *this, dict ),
    monitoring_event_report( *this, dict ),
    monitoring_event_config_status( *this, dict ),
    aese_communication_pattern_config_status( *this, dict ),
    supported_services( *this, dict ),
    s6t_hss_cause( *this, dict.avpS6tHssCause() ),
    enhanced_coverage_restriction_data( *this, dict ),
    cia_flags( *this, dict.avpCiaFlags() ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( oc_supported_features );
    add( oc_olr );
    add( load );
    add( supported_features );
    add( user_identifier );
    add( monitoring_event_report );
    add( monitoring_event_config_status );
    add( aese_communication_pattern_config_status );
    add( supported_services );
    add( s6t_hss_cause );
    add( enhanced_coverage_restriction_data );
    add( cia_flags );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

ConfigurationInformationAnswerExtractor::~ConfigurationInformationAnswerExtractor()
{
}

GroupMonitoringEventReportExtractorList::GroupMonitoringEventReportExtractorList( FDExtractor &parent, Dictionary &dict )
    : FDExtractorList( parent, dict.avpGroupMonitoringEventReport() ),
    m_dict( dict )
{
}

GroupMonitoringEventReportExtractorList::~GroupMonitoringEventReportExtractorList()
{
}

ReportingInformationRequestExtractor::ReportingInformationRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    supported_features( *this, dict ),
    user_identifier( *this, dict ),
    monitoring_event_report( *this, dict ),
    group_monitoring_event_report( *this, dict ),
    rir_flags( *this, dict.avpRirFlags() ),
    supported_services( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( destination_host );
    add( destination_realm );
    add( supported_features );
    add( user_identifier );
    add( monitoring_event_report );
    add( group_monitoring_event_report );
    add( rir_flags );
    add( supported_services );
    add( proxy_info );
    add( route_record );
}

ReportingInformationRequestExtractor::~ReportingInformationRequestExtractor()
{
}

ReportingInformationAnswerExtractor::ReportingInformationAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    supported_features( *this, dict ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( supported_features );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

ReportingInformationAnswerExtractor::~ReportingInformationAnswerExtractor()
{
}

NiddInformationRequestExtractor::NiddInformationRequestExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    destination_host( *this, dict.avpDestinationHost() ),
    destination_realm( *this, dict.avpDestinationRealm() ),
    user_identifier( *this, dict ),
    oc_supported_features( *this, dict ),
    supported_features( *this, dict ),
    nidd_authorization_request( *this, dict ),
    nidd_authorization_update( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( destination_host );
    add( destination_realm );
    add( user_identifier );
    add( oc_supported_features );
    add( supported_features );
    add( nidd_authorization_request );
    add( nidd_authorization_update );
    add( proxy_info );
    add( route_record );
}

NiddInformationRequestExtractor::~NiddInformationRequestExtractor()
{
}

NiddInformationAnswerExtractor::NiddInformationAnswerExtractor( FDMessage &msg, Dictionary &dict )
   : FDExtractor( msg ),
    session_id( *this, dict.avpSessionId() ),
    drmp( *this, dict.avpDrmp() ),
    result_code( *this, dict.avpResultCode() ),
    experimental_result( *this, dict ),
    auth_session_state( *this, dict.avpAuthSessionState() ),
    origin_host( *this, dict.avpOriginHost() ),
    origin_realm( *this, dict.avpOriginRealm() ),
    oc_supported_features( *this, dict ),
    oc_olr( *this, dict ),
    load( *this, dict ),
    supported_features( *this, dict ),
    user_identifier( *this, dict ),
    nidd_authorization_response( *this, dict ),
    failed_avp( *this, dict ),
    proxy_info( *this, dict ),
    route_record( *this, dict.avpRouteRecord() )
{
    add( session_id );
    add( drmp );
    add( result_code );
    add( experimental_result );
    add( auth_session_state );
    add( origin_host );
    add( origin_realm );
    add( oc_supported_features );
    add( oc_olr );
    add( load );
    add( supported_features );
    add( user_identifier );
    add( nidd_authorization_response );
    add( failed_avp );
    add( proxy_info );
    add( route_record );
}

NiddInformationAnswerExtractor::~NiddInformationAnswerExtractor()
{
}


}
