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

#ifndef C3PO_HSS_INCLUDE_UTIL_H_
#define C3PO_HSS_INCLUDE_UTIL_H_

typedef union
{
   uint64_t u64;
   uint8_t u8[8];
} SqnU64Union;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define SQN_TO_U64(_sqn,_u) \
{ \
   _u.u64 = 0; \
   _u.u8[5] = _sqn[0]; \
   _u.u8[4] = _sqn[1]; \
   _u.u8[3] = _sqn[2]; \
   _u.u8[2] = _sqn[3]; \
   _u.u8[1] = _sqn[4]; \
   _u.u8[0] = _sqn[5]; \
}
#define U64_TO_SQN(_u,_sqn) \
{ \
   _sqn[0] = _u.u8[5]; \
   _sqn[1] = _u.u8[4]; \
   _sqn[2] = _u.u8[3]; \
   _sqn[3] = _u.u8[2]; \
   _sqn[4] = _u.u8[1]; \
   _sqn[5] = _u.u8[0]; \
}
#else
#define SQN_TO_U64(_sqn,_u) \
{ \
   _u.u64 = 0; \
   _u.u8[2] = sqn[0]; \
   _u.u8[3] = sqn[1]; \
   _u.u8[4] = sqn[2]; \
   _u.u8[5] = sqn[3]; \
   _u.u8[6] = sqn[4]; \
   _u.u8[7] = sqn[5]; \
}
#define U64_TO_SQN(_u,_sqn) \
{ \
   _sqn[0] = _u.u8[2]; \
   _sqn[1] = _u.u8[3]; \
   _sqn[2] = _u.u8[4]; \
   _sqn[3] = _u.u8[5]; \
   _sqn[4] = _u.u8[6]; \
   _sqn[5] = _u.u8[7]; \
}
#endif

class Utility
{
public:
	static std::string bytes2hex(const uint8_t *bytes, size_t len, char delim='\0', bool upper=false);
};


#endif /* C3PO_HSS_INCLUDE_UTIL_H_ */
