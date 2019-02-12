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

drop table rules;
create table rules (
	rulename text,
	basename text,
	type text,
	definition_json text,
	timeofday_json text,
	usage_monitoring_info text,
	sy_required boolean,
	timemask bigint,
	featuremask bigint,
	primary key (rulename)
);

insert into rules (rulename,basename,type,definition_json,timeofday_json,usage_monitoring_info,sy_required,timemask,featuremask)
values ('retail.rule1','retail','CHARGING','{"Charging-Rule-Definition":{"Charging-Rule-Name":"retail.rule1"}}',
'','',false,0,0);

drop table rule_membership;
create table rule_membership (
	rulename text,
	membership_value text,
	domain text,
	match_time int,
	primary key (rulename)
);

drop table subscriber;
create table subscriber (
	imsi text,
	sy_subscription_id text,
	fail_on_uninstallable_rule boolean,
	primary key (imsi)
);

insert into subscriber (imsi,sy_subscription_id,fail_on_uninstallable_rule) values ('001014567891234','',true);

drop table subscriber_apn;
create table subscriber_apn (
	imsi text,
	apn text,
	membership_value text,
	domain text,
	computed_rules text,
	sy_required boolean,
	transfer_policy text,	
	primary key (imsi,apn)
);

drop table apns;
create table apns (
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

insert into apns
(apn,sy_subscription_id,fail_on_uninstallable_rule,auto_create_spr,membership_value,domain,
computed_rules,sy_required,default_bearer_ctl_mode,force_default_bearer_ctl_mode) values (
'r.ispsn','',true,true,'retail','sprint.com','retail.rule1',false,2,true);

drop table endpoints;
create table endpoints (
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

insert into endpoints (endpointid,endpointrealm,endpointip,capabilities,featuremask,
supported_feature_persistance,assigned_tdf,assigned_tssf) values (
'pcrf.test3gpp.net','test3gpp.net','127.0.0.1',-1,-1,false,'tdf.test3gpp.net','tssf.test3gpp.net');

insert into endpoints (endpointid,endpointrealm,endpointip,capabilities,featuremask,
supported_feature_persistance,assigned_tdf,assigned_tssf) values (
'pcef.test3gpp.net','test3gpp.net','127.0.0.1',-1,-1,false,'','');

insert into endpoints (endpointid,endpointrealm,endpointip,capabilities,featuremask,
supported_feature_persistance,assigned_tdf,assigned_tssf) values (
'tdf.test3gpp.net','test3gpp.net','127.0.0.1',-1,-1,false,'','');

insert into endpoints (endpointid,endpointrealm,endpointip,capabilities,featuremask,
supported_feature_persistance,assigned_tdf,assigned_tssf) values (
'tssf.test3gpp.net','test3gpp.net','127.0.0.1',-1,-1,false,'','');

drop table session;
create table session (
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

drop table session_ip;
create table session_ip (
	ip text,
	imsi text,
	apn text,
	primary key (ip,imsi,apn)
);

drop table session_bearer;
create table session_bearer (
	imsi text,
	apn text,
	ebi int,
	computed_rules text,
	pcef_installed_rules text,
	tdf_installed_rules text,
	tsc_installed_rules text,
	primary key (imsi,apn,ebi)
);
