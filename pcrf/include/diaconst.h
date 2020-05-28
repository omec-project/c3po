/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __DIACONST_H
#define __DIACONST_H

#define DIAMETER_SUCCESS                     2001

#define DIAMETER_UNKNOWN_SESSION_ID          5002
#define DIAMETER_INVALID_AVP_VALUE           5004
#define DIAMETER_MISSING_AVP                 5005
#define DIAMETER_UNABLE_TO_COMPLY            5012

#define DIAMETER_ERROR_INITIAL_PARAMETERS    5140
#define DIAMETER_PCC_RULE_EVENT              5142

#define VEND_3GPP                            10415

#define BEARER_CTL_MODE_UE_ONLY              0
#define BEARER_CTL_MODE_UE_NETWORK           2

#define NETWORK_REQUEST_SUPPORT_NOT_SUPPORTED   0
#define NETWORK_REQUEST_SUPPORT_SUPPORTED       1

#define RESULTCODE( __msg__, __resultcode__ ) \
{ \
   __msg__.add( getDict().avpResultCode(), __resultcode__ ); \
}

#define EXP_RESULTCODE( __msg__, __vendorid__, __resultcode__ ) \
{ \
   FDAvp __erc( getDict().avpExperimentalResult() ); \
   if ( __vendorid__ != 0 ) \
      __erc.add( getDict().avpVendorId(), __vendorid__ ); \
   __erc.add( getDict().avpExperimentalResultCode(), __resultcode__ ); \
   __msg__.add( __erc ); \
}

#define RESULTCODE_WITH_FAILEDAVP1( __msg__, __resultcode__, __avp__ ) \
{ \
   FDAvp failedAvp( getDict().avpFailedAvp() ); \
   failedAvp.add( __avp__ ); \
   RESULTCODE( __msg__, __resultcode__ ); \
   __msg__.add( failedAvp ); \
}

#define RESULTCODE_WITH_FAILEDAVP2( __msg__, __resultcode__, __avpde__, __value__ ) \
{ \
   FDAvp failedAvp( getDict().avpFailedAvp() ); \
   failedAvp.add( getDict().__avpde__, __value__ ); \
   RESULTCODE( __msg__, __resultcode__ ); \
   __msg__.add( failedAvp ); \
}

#define EXP_RESULTCODE_WITH_FAILEDAVP1( __msg__, __vendorid__, __resultcode__, __avp__ ) \
{ \
   FDAvp failedAvp( getDict().avpFailedAvp() ); \
   failedAvp.add( __avp__ ); \
   EXP_RESULTCODE( __msg__, __vendorid__, __resultcode__ ); \
   __msg__.add( failedAvp ); \
}

#define EXP_RESULTCODE_WITH_FAILEDAVP2( __msg__, __vendorid__, __resultcode__, __avpde__, __value__ ) \
{ \
   FDAvp failedAvp( getDict().avpFailedAvp() ); \
   failedAvp.add( getDict().__avpde__, __value__ ); \
   EXP_RESULTCODE( __msg__, __vendorid__, __resultcode__ ); \
   __msg__.add( failedAvp ); \
}

#endif // __DIACONST_H
