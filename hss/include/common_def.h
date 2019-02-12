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

#ifndef SRC_OAI_HSS_FDHSS_INCLUDE_COMMON_DEF_H_
#define SRC_OAI_HSS_FDHSS_INCLUDE_COMMON_DEF_H_

#include <inttypes.h>

#define IMSI_LENGTH (15)
#define IMEI_LENGTH (15)
#define SV_LENGTH   (2)

/* Vendors */
#define VENDOR_3GPP (10415)

#define DIAMETER_SUCCESS                          (2001)
#define DIAMETER_UNABLE_TO_COMPLY                 (5012)

#define DIAMETER_ERROR_USER_UNKNOWN               (5001)
#define DIAMETER_ERROR_ROAMING_NOT_ALLOWED        (5004)
#define DIAMETER_ERROR_UNKNOWN_EPS_SUBSCRIPTION   (5420)
#define DIAMETER_ERROR_RAT_NOT_ALLOWED            (5421)
#define DIAMETER_ERROR_EQUIPMENT_UNKNOWN          (5422)
#define DIAMETER_ERROR_UNKNOWN_SERVING_NODE       (5423)
#define DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE  (4181)
#define DIAMETER_ERROR_USER_NO_APN_SUBSCRIPTION   (5451)
#define DIAMETER_ERROR_ABSENT_USER                (5550)

#define DIAMETER_ERROR_IS_VENDOR(x)                    \
   ((x == DIAMETER_ERROR_USER_UNKNOWN)              || \
    (x == DIAMETER_ERROR_ROAMING_NOT_ALLOWED)       || \
    (x == DIAMETER_ERROR_UNKNOWN_EPS_SUBSCRIPTION)  || \
    (x == DIAMETER_ERROR_RAT_NOT_ALLOWED)           || \
    (x == DIAMETER_ERROR_EQUIPMENT_UNKNOWN)         || \
    (x == DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE) || \
    (x == DIAMETER_ERROR_UNKNOWN_SERVING_NODE)      || \
    (x == DIAMETER_ERROR_ABSENT_USER))


#define DIAMETER_ERROR_CONFIGURATION_EVENT_STORAGE_NOT_SUCCESSFUL (5513)
#define DIAMETER_ERROR_CONFIGURATION_EVENT_NON_EXISTANT           (5514)

#define FLAG_IS_SET(x, flag)   \
    ((x) & (flag))

#define FLAGS_SET(x, flags) \
    ((x) |= (flags))

#define FLAGS_CLEAR(x, flags)   \
    ((x) = (x) & ~(flags))


const uint32_t ABSENT_SUBSCRIBER          = 1;

const uint32_t LOCATION_REPORTING_EVT     = 2;
const uint32_t CHANGE_IMSI_IMEI_SV_ASSN   = 3;
const uint32_t ROAMING_STATUS_EVT         = 4;


#define LOW_NIBBLE(a) (((unsigned char)a) & 0x0f)
#define HIGH_NIBBLE(a) ((((unsigned char)a) & 0xf0) >> 4)

#define PARSE_MNC(a,b) { \
   if ( HIGH_NIBBLE(b[1]) == 0x0f ) { \
      a[0] = '0'; \
      a[1] = LOW_NIBBLE(b[2]) + '0'; \
      a[2] = HIGH_NIBBLE(b[2]) + '0'; \
      a[3] = '\0'; \
   } else { \
      a[0] = LOW_NIBBLE(b[2]) + '0'; \
      a[1] = HIGH_NIBBLE(b[2]) + '0'; \
      a[2] = HIGH_NIBBLE(b[1]) + '0'; \
      a[3] = '\0'; \
   } \
}

#define PARSE_MCC(a,b) { \
   a[0] = LOW_NIBBLE(b[0]) + '0';  \
   a[1] = HIGH_NIBBLE(b[0]) + '0'; \
   a[2] = LOW_NIBBLE(b[1]) + '0'; \
   a[3] = '\0'; \
}

#define PARSE_PLMNID(a) \
   char mnc[4], mcc[4]; \
   PARSE_MNC(mnc,a); \
   PARSE_MCC(mcc,a);


#define PLMNID_TO_HEX_STR(valhex, plmnid) \
   std::stringstream ss; \
   ss << std::hex << std::setfill('0') << std::setw(2) << std::uppercase; \
   for(int i(0);i<=2;++i){ \
       ss<< (unsigned int)plmnid[i]; \
   } \
   valhex = ss.str(); \

#endif /* SRC_OAI_HSS_FDHSS_INCLUDE_COMMON_DEF_H_ */
