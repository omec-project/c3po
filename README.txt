C3PO Core applications build, installation, provisioning and run guide.

About this README

This README contains the instructions to build, install, and configure the 
core C3PO modules and all dependent external projects, and the Cassandra 
database, for use.

Supported Operating System: Ubuntu 16.04 (only) 

*** You can skip straight to the INSTALLATION section to get started. Basic
C3PO information is given below.

C3PO is composed of 2 sets of modules:

1. C3PO Modules:

    A] Core C3PO Modules

        Cassandra DB
        HSS - Home Subscriber Server, supports interaction with the MME via the
              S6a interface and with the SCEF via the S6t interface
        MME - Mobile Management Entity
        UTIL - C3PO Utility Library used by all C3PO applications

    B] Optional C3PO Modules
  
        CDF - Charging Data Function, currently a stub that implements a 
              minimal Rf interface
        CTF - Charging Trigger Function, receives billing records from the data
              plan via a CSV file and forwards the data to the CDF via the Rf
              interface
        HSSGTW - Home Subscriber Server Provisioning Gateway, provides a 
              REST-ful interface for provisioning UE's in the HSS's database
        HSSSEC - Home Subscriber Server Security Library, a standalone 
              library that implements the security algorithms utilized by the 
              HSS
        PCRF - Policy and Charging Rules Function, provides the basic 
              functionality to interact with the PCEF, TDF and TSSF

    A minimal, functional, C3PO installation can be achieved by installing only
    the core C3PO modules.

2. External Modules

    All C3PO applications such as the HSS or the MME, require the external 
    modules listed below. An installation script is provided to automate the 
    fetch, build, and installation of each of these components. The external 
    modules are:

    freeDiameter (https://gerrit.opencord.org/#/admin/projects/freeDiameter)
        Diameter protocol implementation, forked from 
    http://www.freediameter.net
    c-ares (https://github.com/c-ares/c-ares.git)
        asynchronous DNS resolver library
    cpp-driver (https://github.com/datastax/cpp-driver.git)
        DataStax C/C++ Driver for Apache Cassandra
    RapidJSON (https://github.com/miloyip/rapidjson.git)
        A fast JSON parser/generator for C++ with both SAX/DOM style API
    spdlog (https://github.com/gabime/spdlog.git)
        Very fast, header only, C++ logging library
    Pistache (https://github.com/oktal/pistache.git)
        Pistache is a modern and elegant HTTP and REST framework for C++

Conventions followed in the document:
1. Text enclosed in <> such as <Cassandra_Server_IP> requires input specific
to your setup, such as an IP address
2. Text enclosed in {} such as {installation_dir} requires a value of your 
choice

============
INSTALLATION 
============

For C3PO deployment with NG40 and NGIC please refer network diagram at:
https://wiki.opencord.org/download/attachments/2556285/INTEL_NG40_Installation_HIO_RACKE3_RAN10_MME_ILEPC3.xlsx?api=v2

A] Installation of C3PO Core Modules

Install each core C3PO module on a separate server/VM.

1. Go to Cassandra server server/VM.

2. Install Cassandra

    (a) On the Cassandra database server, clone the c3po repository into your
        {installation_root} directory.

        #cd {installation_root}
        #git clone https://github.com/omec-project/c3po.git

    (b) Install Java 8 and Cassandra on the Cassandra server

        #sudo add-apt-repository ppa:webupd8team/java
        #sudo apt-get update
        #sudo apt-get install oracle-java8-installer
        #echo "deb http://www.apache.org/dist/cassandra/debian 21x main" | sudo tee -a /etc/apt/sources.list.d/cassandra.sources.list
        #curl https://www.apache.org/dist/cassandra/KEYS | sudo apt-key add -
        #sudo apt-get update
        #sudo apt-get install cassandra

    (c) Verify that Cassandra is installed and running

        #nodetool status

        The output should look something like:
 
        Datacenter: DC1
        ===============
        Status=Up/Down
        |/ State=Normal/Leaving/Joining/Moving
        --  Address         Load       Tokens  Owns   Host ID                               Rack
        UN  127.0.0.1       438.46 KB  256     100.0% a7e9c21e-79b9-4928-a524-1a0ae5164c49  RAC1 

3. Go to Individual servers Install External Modules and C3PO Applications such as HSS, MME

  For all servers or VM's that will run any C3PO applications, perform the
  following steps.

    (a) Clone the c3po repository into your {installation_root} directory.

         #cd {installation_root}
         #git clone https://github.com/omec-project/c3po.git

    (b) Build and install all components by running install.sh and performing
        each step (invoking each option in the menu in sequence).
    
         #./install.sh

    This will install all the required external modules and the core C3PO 
    modules.

=============
CONFIGURATION
=============

A] Configure Cassandra
 
    The Cassandra server does not need any of the C3PO applications, however 
    there are some C3PO scripts and files that are required to configure the 
    database after Cassandra is installed.  

    The minimal Cassandra configuration required to bring up a running C3PO
    is given below:

    (a) Go to Cassandra server/VM
    
    (b) Stop Cassandra and cleanup the log files before modifying the 
        configuration

        #sudo service cassandra stop
        #sudo rm -rf /var/lib/cassandra/data/system/*
        #sudo rm -rf /var/lib/cassandra/commitlog/*
        #sudo rm -rf /var/lib/cassandra/data/system_traces/*
        #sudo rm -rf /var/lib/cassandra/saved_caches/*

    (c) Update the Cassandra configuration

        Update /etc/cassandra/cassandra.yaml as indicated below.  
        The <Cassandra_Server_IP> address should be the IP address of 
        the Cassandra server that the HSS will use to connect to Cassandra. The
        "..." below indicate configuration lines between values that need to be
        modified.

        ...
        cluster_name: "HSS Cluster"
        ...
        seed_provider:
            - class_name: org.apache.cassandra.locator.SimpleSeedProvider
            - seeds: "<Cassandra_Server_IP>"
        ...
        listen_address: <Cassandra_Server_IP>
        ...
        rpc_address: <Cassandra_Server_IP>
        ...
        endpoint_snitch: GossipingPropertyFileSnitch

    (d) Update /etc/cassandra/cassandra-rackdc.properties as indicated below.

        prefer_local=true

    (e) Start Cassandra with the new Configuration

        #sudo service cassandra start

    (f) Verify that Cassandra is running with the updated configuration

        #nodetool status

        The output should look something like:
 
            Datacenter: DC1
            ===============
            Status=Up/Down
            |/ State=Normal/Leaving/Joining/Moving
            --  Address         Load       Tokens  Owns   Host ID                               Rack
            UN  192.168.112.60  438.46 KB  256     100.0% a7e9c21e-79b9-4928-a524-1a0ae5164c49  RAC1

    (g) Create the HSS Database Schema in Cassandra

        #cd {installation_root}
        #cqlsh --file hss/db/oai_db.cql <Cassandra_Server_IP>
    
    For example:  cqlsh --file hss/db/oai_db.cql 192.168.112.60
    For more information refer to {installation_root}/db_docs/Cassandra_Single_Node_Install.docx

B] Configure the C3PO Core

    (a) HSS
        1. Go to HSS server/VM
        
        2. Update the Diameter Configuration File
            In {installation_root}/hss/conf/hss.conf update the following 
            values accordingly:

                Identity - The diameter identity. The default value is 
                           hss.openair4G.eur
                Realm - The diameter realm (everything past the first period 
                        of the Identity).  The default value is openair4G.eur
          
        3. Create Diameter Certificates

        NOTE: The "Diameter Identity" is a fully qualified domain name that is
        used to access the the Diameter peer.  The "Diameter Host" is 
        everything up to the first period of the "Diameter Identity". The 
        "Diameter Realm" is everything after the first period of the 
        "Diameter Identity".  

            Using the "Identity" configured in 
            {installation_root}/hss/conf/hss.conf, execute the following 
            commands.

                #cd {installation_root}/hss/conf
                #../bin/make_certs.sh <diameter_host> <diameter_realm>
    
            For Example: ../bin/make_certs.sh hss openair4G.eur
    
        4. Update the HSS Configuration File

            In {installation_root}/hss/conf/hss.json update the following 
            values accordingly. Assuming no name changes in the other 
            configuration files, the only value that needs to be updated is 
            "casssrv".

                fdcfg - The name of the Diameter configuration file, the 
                        default is conf/hss.conf
                originhost - Same as the Identity set in hss.conf.
                originrealm - Same as the Realm set in hss.conf.
                gtwhost - The IP address(es) the HSS REST interface will 
                          listen to process HSS gateway commands. The default 
                          is "*".
                gtwport - The port the HSS REST interface will listen to 
                          process HSS gateway commands.
                casssrv - The IP address of the Cassandra database server,
                          <Cassandra_Server_IP> that the HSS will connect to. 
                          ***UPDATE "casssrv" ***
                cassusr - The Cassandra user name.  The default value is 
                          "root".
                casspwd - The Cassandra password.  The default value is "root".
                cassdb - The Cassandra keyspace for the HSS data. The default 
                         value is "vhss".
                randv - The default value is true.
                optkey - The operator key. The default value is
                         "63bfa50ee6523365ff14c1f45f88737d" (which should 
                         match the NG40 default key).
                reloadkey - If true, the OPc values for all IMSI's will be re-
                            calculated when the HSS is started.

C] Configure NG40 tool(not part of C3PO) for testing
    Configure NG40 setup to connect with C3PO MME and SGW-data plane as follows.
    Update '{ng40_root_dir}/network.def.ilepc' in NG40 to configure for following:
		
    $S1_DEV : List (comma separated in []) of Linux ethernet device name per 
            RAN used for eNB S1MME interface 
    $S1_IP_NET : IP address / subnet bits of the first eNB S1MME interface. 
            IPs are incremented by 1 for each eNB.
            $S1U_IP : IP address of the first eNB S1U interface.  IPs are 
                     incremented by 1 for each eNB.
            $MME_IP : S11 IP address of MME server.
    $AS_IP : List (comma separated in []) of AppServer start IP per RAN.
    $AS_IPGW : List (comma separated in []) of SGI addresses per RAN used as 
            Gateway for the AppServer.
    $APNs : List (comma separated in []) of APNs per RAN
        $enbnumber : Total number of eNBs in all RANs
    $noofscenarios : Maximum Number of RANs used
    $S1U_DEV : List (comma separated in []) of DPDK device for eNB S1U 
        interface per RAN (DPDK device definition see /opt/ng4t/etc/ports.conf)
    $AS_DEV : List (comma separated in []) of DPDK device for AppServers 
        interface per RAN (DPDK device definition see /opt/ng4t/etc/ports.conf)

    Update '{ng40_root_dir}/commonconf/networkelement.conf' in NG40 to configure
        for following:

    Section [MME-ENB]
    MCC : Mobile Country Code. Same as configure for MME and IMSIs. 
    MNC : Mobile Network Code. Same as configure for MME and IMSIs. 

    Section [ENBs]
    ENBs[0].TRANSPORT[0].SCTP_PORT : eNB SCTP port.

    Update '{ng40_root_dir}/commonconf/subscriber.conf' in NG40 to configure for
    following:
    
    [Subscriber]
    group[0].ki : Subscriber Secret used for all Subscriber
    
    Section [SubscriberProfile]
    profile[0].op : Operator Code
    profile[0].opc_flag : Configure if profile[0].op is the ciphered or unciphered OP (Set to: OP not equal OPc)
                
============
PROVISIONING
============

1. Add Test Data to Cassandra

    (a) Go to Cassandra Server/VM
    
    (b) Adding User Equipment (UEs)

        On the Cassandra server, the 
        {installation_root}/db_docs/data_provisioning_users.sh script can be used 
        to add one or more user equipment entries in the HSS database.
        Syntax:
        #./data_provisioning_users.sh <IMSI> <MSISDN> <APN> <Key> <No of UEs>
        <Cassandra_Server_IP>
    
        The script takes six parameters.

            IMSI - the first IMSI in the range to add. Each subsequent IMSI will be
                added by incrementing the value.
            MSISDN - the first MSISDN in the range to add.  Each subsequent MSISDN
                    will be added by incrementing the value.
                    **NOTE: the MSISDN must be 10 digits long**
            APN - the APN to be associated with each entry.
            KEY - the equipment key.
            NUMBER_OF_USERS - the total number of entries to add.
            CASSANDRA_IP - the IP address of the Cassandra server (same as the
                        <Cassandra_Server_IP> configured above).

        The following example will add 25 entries:
        #./data_provisioning_users.sh 208014567891234 1122334455 apn1 465B5CE8B199B49FAA5F0A2EE238A6BC 25 192.168.112.60

    (c) Adding Core Servers

        On the Cassandra VM, the 
        {installation_root}/db_docs/data_provisioning_mme.sh script can be 
        used to add server entries into the database. The primary purpose is to 
        identify MME's to the HSS.  It is also used to identify other servers to 
        the HSS such as an SMS router. The script takes 6 parameters.
        Syntax:
        ./data_provisioning_mme.sh <ID> <ISDN> <HOST> <REALM> <UE
        Reachability> <Cassandra_Server_IP>

        ID - the unique numeric ID of the server. The ID values typically 
             start at 1.
        ISDN - the ISDN associated with the server. This required for the MME 
               entry.
        HOST - the fully qualified Diameter Identity of the server.
        REALM - the Diameter Realm of the server.
        UEREACHABILITY - indicates the UE reachability value that will be 
                         returned during subsequent HSS processing.
        CASSANDRA_IP - the IP address of the Cassandra server (same as the 
                       <Cassandra_Server_IP> configured above).

        Here are a couple of examples:

        #./data_provisioning_mme.sh 1 19136246000 mme.localdomain localdomain 1 192.168.112.60
        #./data_provisioning_mme.sh 1 19136246000 smsrouter.test3gpp.net test3gpp.net 0 192.168.112.60

    
    (c) Go to HSS server/VM
    
    (d) Calculate the OPc value for each UE (User Equipment).
        IMP: Whenever UE configuration is changed as in step (a) above, 
        following steps command be executed on HSS.
        On the HSS server run the following command:

            #cd {installation_root}/hss
            #./bin/hss -j conf/hss.json --onlyloadkey
    
2. NG40 provisioning
    (a) IMSI and MSISDN settings
        For UEs configured on C3PO setup, configure NG40 settings as follows:
        In file <ng40_root_dir>/subscriber.def.ilepc, add following fields as 
        per configuration in section "PROVISIONING"-> 1 ->(b)
        
        $IMSI - First IMSI value added in Cassandra.
        $MSISDN - First MSISDN value added in Cassandra.
        $IMEI - First IMEISV for first UE, incremented by 100. No need to change


    (c) If <key> in the "PROVISIONING" -> 1 -> (b) is changed then update 
        <ng40_root_dir>/commonconf/subscriber.conf with new key as following 

        ...
        group[0].ki=465B5CE8B199B49FAA5F0A2EE238A6BC
        ...
    
    (d) Number of UEs setting
        Configure test script in NG40 for number of UEs, transaction rate.
        E.g. update following fields in '<ng40_root_dir>/test/mppsshort.cfg' file.

       ...
       set $numran 1 # Number of RANs to start
       set $subs 25 # To set 25 number of UEs under test. At least 25 subscriber * Number of RANs need to be configured
       set $totalrate 100 # To set transaction rate per second.
       set $pps 900000 # To set Packets Per Second to generate.        
       ...

================
RUN C3PO MODULES
================
    
A] Start control and data plane.
    Following are steps for ngic setup.
    1. Start DP
        #cd {ngic_installation_root}/dp
        #./run.sh

    2. Start CP
        #cd {ngic_installation_root}/cp
        #./run.sh

B] Start Cassandra
    
    Cassandra runs as service hence no need to start it every time. Service starts
    when cassandra server boots up. Verify that cassandra is running with command:

        #nodetool status
    
    If Cassandra is not running then start with command:
        #service cassandra start
    
C] Run the HSS

    To run the HSS application, use the following command on the HSS server:

        #cd {installation_root}/hss
        #./bin/hss -j conf/hss.json
  
D] Run the MME

E] Start ng40 test
    #cd {ng40_installation_root}/test
    #ng40test tc_userplane_mpps.ntl mppsshort.cfg
