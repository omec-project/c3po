CREATE KEYSPACE pcrf WITH replication = {'class': 'NetworkTopologyStrategy', 'DC1': '1'}  AND durable_writes = true;

CREATE TABLE pcrf.rules (
    rulename text PRIMARY KEY,
    basename text,
    definition_json text,
    featuremask bigint,
    sy_required boolean,
    timemask bigint,
    timeofday_json text,
    type text,
    usage_monitoring_info text
);

CREATE TABLE pcrf.subscriber_apn (
    imsi text,
    apn text,
    computed_rules text,
    domain text,
    membership_value text,
    sy_required boolean,
    transfer_policy text,
    PRIMARY KEY (imsi, apn)
);

CREATE TABLE pcrf.subscriber (
    imsi text PRIMARY KEY,
    fail_on_uninstallable_rule boolean
);

CREATE TABLE pcrf.apns (
    apn text PRIMARY KEY,
    auto_create_spr boolean,
    computed_rules text,
    default_bearer_ctl_mode int,
    domain text,
    fail_on_uninstallable_rule boolean,
    force_default_bearer_ctl_mode boolean,
    membership_value text,
    sy_required boolean
);

CREATE TABLE pcrf.session (
    imsi text,
    apn text,
    ipv4 text,
    ipv6 text,
    pcef_endpoint text,
    pcef_sessionid text,
    pcrf_endpoint text,
    tdf_endpoint text,
    tdf_sessionid text,
    tsc_endpoint text,
    tsc_sessionid text,
    PRIMARY KEY (imsi, apn)
);

CREATE TABLE pcrf.rule_membership (
    rulename text PRIMARY KEY,
    domain text,
    match_time int,
    membership_value text
);

CREATE TABLE pcrf.session_ip (
    ip text,
    imsi text,
    apn text,
    PRIMARY KEY (ip, imsi, apn)
);

CREATE TABLE pcrf.endpoints (
    endpointid text PRIMARY KEY,
    assigned_tdf text,
    assigned_tsc text,
    capabilities bigint,
    deletable boolean,
    featuremask bigint
);



