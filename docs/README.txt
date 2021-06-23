--------------------------------------------------------------------------------
This README provides a walk-through of the documentation
--------------------------------------------------------------------------------

[A] "1_0_adding_redis_support_in_hss.txt" helps in understanding how to enable 
    HSS to use Redis as a database.

[B] "2_0_integrating_hss_with_graphene_sgx.txt" provides steps to integrate HSS 
    with Graphene, i.e running HSS within a Graphene instance in SGX mode.

[C] "2_1_integrating_redis_with_graphene_sgx.txt" provides steps to integrate 
    Redis server with Graphene.
    

--------------------------------------------------------------------------------  
One of the important aspects of Intel SGX is RA-TLS (Remote Attestation 
integrated with TLS). Please find the below links that help in understanding 
RA-TLS and how to integrate it with the applications.

1) https://software.intel.com/content/www/us/en/develop/download/integrating-remote-attestation-with-transport-layer-security.html
2) https://graphene.readthedocs.io/en/latest/attestation.html

However it is important to note that the existing RA-TLS flows are based on 
self-signed certificates. Self-signed certificates may serve limited purpose
w.r.t trust, revocation, expiry, etc. For two applications to successfully 
perform Remote Attestation and establish TLS/mTLS session based on CA-signed
certificates there are two approaches that this documentation covers.

    1) Using the Secured Certificate Provisioning Flow:
    
        The documents mentioned in point [D] and [E] provide detailed steps for 
        using the certificate provisioning flow in a Graphene based application.
        
        Also Refer the high level diagram in [I] and the below link
        https://github.com/GSLabDev/graphene/blob/feature_certificate_provisioning_flow/Pal/src/host/Linux-SGX/tools/README.rst#certificate-provisioning-library
        for more understanding of the feature.
        
    2) Using Marblerun:
    
        The documents mentioned in point [F] and [G] provide detailed steps for
        integrating Marblerun with a Graphene based application.
        
        Also refer the official documentation at https://github.com/edgelesssys/marblerun/blob/master/README.md#quickstart-and-documentation
        
--------------------------------------------------------------------------------        
             
[D] "3_0_integrating_secured_cert_prov_with_hss.txt" provides steps to integrate
    the Graphene Secured Certificate Provisioning Flow with HSS.
    
[E] "3_1_integrating_secured_cert_prov_with_redis_server.txt" provides steps to 
    integrate the Graphene Secured Certificate Provisioning Flow with Redis 
    server.

[F] "4_0_building_marblerun.txt" provides steps to build Marblerun. It also 
    provides steps to build Marblerun specific to the platform Quote Verificate 
    Enclave. 

[G] "4_1_integrating_marblerun_with_hss_redis.txt" provides steps to integrate
    Marblerun with HSS and Redis server.

[H] "Performance Metrics Of HSS.xlsx" worksheet provides the performance metrics
    of HSS wrt Redis vs Cassandra and wrt HSS with Graphene SGX vs without 
    Graphene SGX. 

[I] "secured_cert_provisioning.png" diagram explains the Secured Certificate
    Provisioning flow