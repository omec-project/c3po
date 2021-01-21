CREATE KEYSPACE IF NOT EXISTS vpcrf WITH replication = {'class': 'NetworkTopologyStrategy', 'DC1': '1'};

DROP TABLE IF EXISTS vpcrf.rules;

CREATE TABLE vpcrf.rules (
	rulename text,
	basename text,
	type text,
	definition_json text,
	timeofday_json text,
	usage_monitoring_info text,
	sy_required boolean,
	timemask bigint,
	featuremask bigint,
	defaultrule boolean,
	primary key (rulename)
);

insert into vpcrf.rules (rulename,basename,type,definition_json,timeofday_json,usage_monitoring_info,sy_required,timemask,featuremask,defaultrule)
values ('retail.rule1','retail','CHARGING','{"Charging-Rule-Definition":{"Charging-Rule-Name":"retail.rule1", "QoS-Information":{"QoS-Class-Identifier":9, "Max-Requested-Bandwidth-UL": 2345, "Max-Requested-Bandwidth-DL":3456, "Guaranteed-Bitrate-UL":1111, "Guaranteed-Bitrate-DL":2222, "Allocation-Retention-Priority": {"Priority-Level": 1, "Pre-emption-Capability": 1, "Pre-emption-Vulnerability": 1},"APN-Aggregate-Max-Bitrate-UL":4444, "APN-Aggregate-Max-Bitrate-DL": 5555}}}',
'','',false,0,0,true);

insert into vpcrf.rules (rulename,basename,type,definition_json,timeofday_json,usage_monitoring_info,sy_required,timemask,featuremask,defaultrule)
values ('retail.rule2','retail','CHARGING','{"Charging-Rule-Definition":{"Charging-Rule-Name":"retail.rule2", "QoS-Information":{"QoS-Class-Identifier":5, "Max-Requested-Bandwidth-UL": 2345, "Max-Requested-Bandwidth-DL":3456, "Guaranteed-Bitrate-UL":1111, "Guaranteed-Bitrate-DL":2222, "Allocation-Retention-Priority": {"Priority-Level": 1, "Pre-emption-Capability": 1, "Pre-emption-Vulnerability": 1},"APN-Aggregate-Max-Bitrate-UL":4444, "APN-Aggregate-Max-Bitrate-DL": 5555}}}',
'','',false,0,0,false);



DROP TABLE IF EXISTS vpcrf.rule_membership;
CREATE TABLE vpcrf.rule_membership (
	rulename text,
	membership_value text,
	domain text,
	match_time int,
	primary key (rulename)
);

DROP TABLE IF EXISTS vpcrf.subscriber;
CREATE TABLE vpcrf.subscriber (
	imsi text,
	sy_subscription_id text,
	fail_on_uninstallable_rule boolean,
	primary key (imsi)
);

insert into vpcrf.subscriber (imsi,sy_subscription_id,fail_on_uninstallable_rule) values ('001014567891234','',true);

DROP TABLE IF EXISTS vpcrf.subscriber_apn;
CREATE TABLE vpcrf.subscriber_apn (
	imsi text,
	apn text,
	membership_value text,
	domain text,
	computed_rules text,
	sy_required boolean,
	transfer_policy text,	
	primary key (imsi,apn)
);

DROP TABLE IF EXISTS vpcrf.apns;
CREATE TABLE vpcrf.apns (
	apn text,
	sy_subscription_id text,
	fail_on_uninstallable_rule boolean,
	auto_create_spr boolean,
	membership_value text,
	domain text,
	computed_rules text,
	sy_required boolean,
	default_bearer_ctl_mode int,
	force_default_bearer_ctl_mode boolean,
	primary key (apn)
);

insert into vpcrf.apns
(apn,sy_subscription_id,fail_on_uninstallable_rule,auto_create_spr,membership_value,domain,
computed_rules,sy_required,default_bearer_ctl_mode,force_default_bearer_ctl_mode) values (
'internet','',true,true,'retail','sprint.com','retail.rule1,retail.rule2',false,2,true);


DROP TABLE IF EXISTS vpcrf.endpoints;
CREATE TABLE vpcrf.endpoints (
	endpointid text,
	endpointrealm text,
	endpointip text,
	capabilities bigint,
	featuremask bigint,
	supported_feature_persistance boolean,
	assigned_tdf text,
	assigned_tssf text,
	primary key (endpointid)
);

insert into vpcrf.endpoints (endpointid,endpointrealm,endpointip,capabilities,featuremask,
supported_feature_persistance,assigned_tdf,assigned_tssf) values (
'pcrf.test3gpp.net','test3gpp.net','127.0.0.1',-1,-1,false,'tdf.test3gpp.net','tssf.test3gpp.net');

insert into vpcrf.endpoints (endpointid,endpointrealm,endpointip,capabilities,featuremask,
supported_feature_persistance,assigned_tdf,assigned_tssf) values (
'pcef.test3gpp.net','test3gpp.net','127.0.0.1',-1,-1,false,'','');

insert into vpcrf.endpoints (endpointid,endpointrealm,endpointip,capabilities,featuremask,
supported_feature_persistance,assigned_tdf,assigned_tssf) values (
'tdf.test3gpp.net','test3gpp.net','127.0.0.1',-1,-1,false,'','');

insert into vpcrf.endpoints (endpointid,endpointrealm,endpointip,capabilities,featuremask,
supported_feature_persistance,assigned_tdf,assigned_tssf) values (
'tssf.test3gpp.net','test3gpp.net','127.0.0.1',-1,-1,false,'','');

DROP TABLE IF EXISTS vpcrf.session;
CREATE TABLE vpcrf.session (
	imsi text,
	apn text,
	pcrf_endpoint text,
	ipv4 text,
	ipv6 text,
	pcef_sessionid text,
	pcef_endpoint text,
	tdf_sessionid text,
	tdf_endpoint text,
	tsc_sessionid text,
	tsc_endpoint text,
	primary key (imsi,apn)
);

DROP TABLE IF EXISTS vpcrf.session_ip;
CREATE TABLE vpcrf.session_ip (
	ip text,
	imsi text,
	apn text,
	primary key (ip,imsi,apn)
);

DROP TABLE IF EXISTS vpcrf.session_bearer;
CREATE TABLE vpcrf.session_bearer (
	imsi text,
	apn text,
	ebi int,
	computed_rules text,
	pcef_installed_rules text,
	tdf_installed_rules text,
	tsc_installed_rules text,
	primary key (imsi,apn,ebi)
);

