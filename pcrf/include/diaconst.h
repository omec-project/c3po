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
