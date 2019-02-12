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

/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#define RAPIDJSON_NAMESPACE fdrapidjson

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <arpa/inet.h>

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <unordered_map>

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/libfdproto.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "fdjson.h"
#include "stime.h"
#include "ssync.h"

#define ISHEXDIGIT(x) \
( \
   (x >= '0' && x <= '9') || \
   (x >= 'a' && x <= 'f') || \
   (x >= 'A' && x <= 'F') \
)

#define HEX2BIN(x) ( \
   x >= '0' && x <= '9' ? x - '0' : \
   x >= 'a' && x <= 'f' ? x - 'a' + 10 : \
   x >= 'A' && x <= 'F' ? x - 'A' + 10 : 15 \
)

#define LOWERNIBBLE(_ln_) ((_ln_) & 0x0f)
#define UPPERNIBBLE(_un_) ( LOWERNIBBLE((_un_) >> 4) )

class runtimeInfo : public std::runtime_error
{
public:
   runtimeInfo(const char *m) : std::runtime_error(m) {}
   runtimeInfo(const std::string &m) : std::runtime_error(m) {}
};

class runtimeError : public std::runtime_error
{
public:
   runtimeError(const char *m) : std::runtime_error(m) {}
   runtimeError(const std::string &m) : std::runtime_error(m) {}
};

template<class T>
class Buffer
{
public:
   Buffer(size_t size) { msize = size; mbuf = new T[msize]; }
   ~Buffer() { if (mbuf) delete [] mbuf; }
   T *get() { return mbuf; }
private:
   Buffer();
   size_t msize;
   T *mbuf;
};

std::string string_format( const char *format, ... )
{
   va_list args;

   va_start( args, format );
   size_t size = vsnprintf( NULL, 0, format, args ) + 1; // Extra space for '\0'
   va_end( args );

   Buffer<char> buf( size );

   va_start( args, format );
   vsnprintf( buf.get(), size, format, args  );
   va_end( args );

   return std::string( buf.get(), size - 1 ); // We don't want the '\0' inside
}

enum AvpDataType
{
   ADTUnknown,
   ADTOctetString,
   ADTI32,
   ADTI64,
   ADTU32,
   ADTU64,
   ADTF32,
   ADTF64,
   ADTGrouped,
   ADTAddress,
   ADTTime,
   ADTUTF8String,
   ADTDiameterIdentity,
   ADTDiameterURI,
   ADTEnumerated,
   ADTIPFilterRule
};

class AvpDictionaryEntry
{
public:
   AvpDictionaryEntry()
   {
      m_baseentry = NULL;
      memset(&m_basedata, 0, sizeof(m_basedata));
      m_type = ADTUnknown;
   }
   ~AvpDictionaryEntry()
   {
   }

   void init(const char *avp_name)
   {
      int ret = 0;

      m_avp_name = avp_name;

      /* get the dictionary entry for the AVP */
      ret = fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME_ALL_VENDORS, avp_name, &m_baseentry, ENOENT );
      if (ret != 0)
         throw runtimeInfo(
            string_format("%s:%d - INFO - Unable to find AVP dictionary entry  for [%s]",
            __FILE__, __LINE__, avp_name)
         );

      /* get the dictionary data for the AVP's dictionary entry */
      ret = fd_dict_getval( m_baseentry, &m_basedata );
      if (ret != 0)
         throw runtimeInfo(
            string_format("%s:%d - INFO - Unable to retrieve the dictionary data for the [%s] dictionary entry",
            __FILE__, __LINE__, avp_name)
         );

      struct dictionary *dict = NULL;
      struct dict_object *derivedtype = NULL;

      /* get the dictionary associated with the AVP dictionary entry */
      ret = fd_dict_getdict( m_baseentry, &dict );
      if (ret != 0)
         throw runtimeInfo(
            string_format("%s:%d - INFO - Unable to retrieve the dictionary for the [%s] dictionary entry",
            __FILE__, __LINE__, avp_name)
         );

      /* get the dictionary entry associated with the derived type */
      ret = fd_dict_search( dict, DICT_TYPE, TYPE_OF_AVP, m_baseentry, &derivedtype, EINVAL );
      if (ret == 0) /* if found, then derived */
      {
         struct dict_type_data derived_type_data;

         ret = fd_dict_getval( derivedtype, &derived_type_data );

         if (ret == 0)
         {
            if      ( !strcmp( derived_type_data.type_name, "Enumerated" ) )        m_type = ADTEnumerated;
            else if ( !strcmp( derived_type_data.type_name, "Time" ) )              m_type = ADTTime;
            else if ( !strcmp( derived_type_data.type_name, "Address" ) )           m_type = ADTAddress;
            else if ( !strcmp( derived_type_data.type_name, "UTF8String" ) )        m_type = ADTUTF8String;
            else if ( !strcmp( derived_type_data.type_name, "DiameterIdentity" ) )  m_type = ADTDiameterIdentity;
            else if ( !strcmp( derived_type_data.type_name, "DiameterURI" ) )       m_type = ADTDiameterURI;
            else if ( !strcmp( derived_type_data.type_name, "IPFilterRule" ) )      m_type = ADTIPFilterRule;
            else
            {
               switch ( m_basedata.avp_basetype )
               {
                  case AVP_TYPE_GROUPED:     { m_type = ADTGrouped; break; }
                  case AVP_TYPE_INTEGER32:   { m_type = ADTI32; break; }
                  case AVP_TYPE_INTEGER64:   { m_type = ADTI64; break; }
                  case AVP_TYPE_UNSIGNED32:  { m_type = ADTU32; break; }
                  case AVP_TYPE_UNSIGNED64:  { m_type = ADTU64; break; }
                  case AVP_TYPE_FLOAT32:     { m_type = ADTF32; break; }
                  case AVP_TYPE_FLOAT64:     { m_type = ADTF64; break; }
                  case AVP_TYPE_OCTETSTRING: { m_type = ADTOctetString; break; }
                  default:                   { m_type = ADTUnknown; break; }
               }
            }
         }
         else
         {
            switch ( m_basedata.avp_basetype )
            {
               case AVP_TYPE_GROUPED:     { m_type = ADTGrouped; break; }
               case AVP_TYPE_INTEGER32:   { m_type = ADTI32; break; }
               case AVP_TYPE_INTEGER64:   { m_type = ADTI64; break; }
               case AVP_TYPE_UNSIGNED32:  { m_type = ADTU32; break; }
               case AVP_TYPE_UNSIGNED64:  { m_type = ADTU64; break; }
               case AVP_TYPE_FLOAT32:     { m_type = ADTF32; break; }
               case AVP_TYPE_FLOAT64:     { m_type = ADTF64; break; }
               case AVP_TYPE_OCTETSTRING: { m_type = ADTOctetString; break; }
               default:                   { m_type = ADTUnknown; break; }
            }
         }
      }
      else
      {
         switch ( m_basedata.avp_basetype )
         {
            case AVP_TYPE_GROUPED:     { m_type = ADTGrouped; break; }
            case AVP_TYPE_INTEGER32:   { m_type = ADTI32; break; }
            case AVP_TYPE_INTEGER64:   { m_type = ADTI64; break; }
            case AVP_TYPE_UNSIGNED32:  { m_type = ADTU32; break; }
            case AVP_TYPE_UNSIGNED64:  { m_type = ADTU64; break; }
            case AVP_TYPE_FLOAT32:     { m_type = ADTF32; break; }
            case AVP_TYPE_FLOAT64:     { m_type = ADTF64; break; }
            case AVP_TYPE_OCTETSTRING: { m_type = ADTOctetString; break; }
            default:                   { m_type = ADTUnknown; break; }
         }
      }

   }

   const std::string &getAvpName() { return m_avp_name; }
   struct dict_object *getBaseEntry() { return m_baseentry; }
   struct dict_avp_data &getBaseData() { return m_basedata; }
   AvpDataType getType() { return m_type; }

private:
   std::string m_avp_name;
   struct dict_object *m_baseentry;
   struct dict_avp_data m_basedata;
   AvpDataType m_type;
};

static SMutex dictEntriesMutex;
static std::unordered_map<std::string,AvpDictionaryEntry*> dictEntries;

class AVP
{
public:
   AVP( const char *avp_name ) { _init(avp_name); }
   AVP( const char *avp_name, int32_t v ) { _init(avp_name); set(v); }
   AVP( const char *avp_name, int64_t v ) { _init(avp_name); set(v); }
   AVP( const char *avp_name, uint32_t v ) { _init(avp_name); set(v); }
   AVP( const char *avp_name, uint64_t v ) { _init(avp_name); set(v); }
   AVP( const char *avp_name, float v ) { _init(avp_name); set(v); }
   AVP( const char *avp_name, double v ) { _init(avp_name); set(v); }
   AVP( const char *avp_name, const int8_t *v ) { _init(avp_name); set(v); }
   AVP( const char *avp_name, const int8_t *v, size_t len ) { _init(avp_name); set(v, len); }
   AVP( const char *avp_name, const uint8_t *v ) { _init(avp_name); set(v); }
   AVP( const char *avp_name, const uint8_t *v, size_t len ) { _init(avp_name); set(v, len); }

   ~AVP()
   {
      if (mBuf)
         delete mBuf;
   }

   AVP &set( int32_t v ) { mValue.i32 = v; return *this; }
   AVP &set( int64_t v ) { mValue.i64 = v; return *this; }
   AVP &set( uint32_t v ) { mValue.u32 = v; return *this; }
   AVP &set( uint64_t v ) { mValue.u64 = v; return *this; }
   AVP &set( float v ) { mValue.f32 = v; return *this; }
   AVP &set( double v ) { mValue.f64 = v; return *this; }
   AVP &set( const int8_t *v ) { return set(v, strlen((const char *)v)); }
   AVP &set( const int8_t *v, size_t len ) { mValue.os.data = (uint8_t*)v; mValue.os.len = len; return *this; }
   AVP &set( const uint8_t *v ) { return set(v, strlen((const char *)v)); }
   AVP &set( const uint8_t *v, size_t len ) { mValue.os.data = (uint8_t*)v; mValue.os.len = len; return *this; }

   void addTo( msg_or_avp *reference ) { _addTo(reference); }
   void addTo( AVP &reference ) { _addTo(reference.mAvp); }

   struct avp *getAvp() { return mAvp; }

   void allocBuffer(size_t len) { mBuf = new Buffer<uint8_t>(len); }
   Buffer<uint8_t> &getBuffer() { return *mBuf; }

   dict_avp_basetype getBaseType() { return mBaseData.avp_basetype; }
   AvpDataType getType() { return mType; }

private:

   void _init( const char *avp_name )
   {

      mName = avp_name;
      mBaseEntry = NULL;
      memset( &mBaseData, 0, sizeof(mBaseData) );
      mType = ADTUnknown;
      mBuf = NULL;
      mAvp = NULL;
      memset( &mValue, 0, sizeof(mValue) );

      /* check if an entry exists in dictEntries */
      std::string an = avp_name;
      auto it = dictEntries.find(an);
      if (it == dictEntries.end())
      {
         SMutexLock l(dictEntriesMutex);

         it = dictEntries.find(an);
         if (it == dictEntries.end())
         {
            AvpDictionaryEntry *ade = new AvpDictionaryEntry();
            ade->init(avp_name);
            std::pair<std::string,AvpDictionaryEntry*> data(ade->getAvpName(), ade);
            auto result = dictEntries.insert(data);
            if (result.second == false)
            {
               delete ade;
               throw runtimeInfo(
                  string_format("%s:%d - WARN - Unable to insert AvpDictionaryEntry for [%s]",
                  __FILE__, __LINE__, avp_name)
               );
            }

            it = result.first;
         }
      }

      mBaseEntry = it->second->getBaseEntry();
      memcpy( &mBaseData, &it->second->getBaseData(), sizeof(mBaseData));
      mType = it->second->getType();
   }

   void _addTo( msg_or_avp *reference )
   {
      int ret;

      if ((ret = fd_msg_avp_new(mBaseEntry,0,&mAvp)) != 0)
         throw runtimeError(
            string_format("%s:%d - ERROR - Error [%d] creating [%s] AVP",
            __FILE__, __LINE__, ret, mName)
         );

      try
      {
         if ( mBaseData.avp_basetype != AVP_TYPE_GROUPED )
         {
            if ((ret = fd_msg_avp_setvalue(mAvp,&mValue)) != 0)
               throw runtimeError(
                  string_format("%s:%d - ERROR - Error [%d] setting AVP value for [%s]",
                  __FILE__, __LINE__, ret, mName)
               );
         }

         if ((ret = fd_msg_avp_add(reference,MSG_BRW_LAST_CHILD,mAvp)) != 0)
            throw runtimeError(
               string_format("%s:%d - ERROR - Error [%d] adding [%s] AVP",
               __FILE__, __LINE__, ret, mName)
            );
      }
      catch (...)
      {
         fd_msg_free( mAvp );
         mAvp = NULL;
         throw;
      }
   }

   const char * mName;
   struct dict_object *mBaseEntry;
   struct dict_avp_data mBaseData;
   AvpDataType mType;
   Buffer<uint8_t> *mBuf;
   struct avp *mAvp;
   union avp_value mValue;
};

#define THROW_DATATYPE_MISMATCH() \
{ \
   throw runtimeInfo( string_format("%s:%d - INFO - Datatype mismatch for [%s] - expected datatype compatible with %s, JSON data type was %s", \
      __FILE__, __LINE__, name, \
      avp.getType() == ADTUnknown ? "ADTUnknown" : \
      avp.getType() == ADTOctetString ? "ADTOctetString" : \
      avp.getType() == ADTI32 ? "ADTI32" : \
      avp.getType() == ADTI64 ? "ADTI64" : \
      avp.getType() == ADTU32 ? "ADTU32" : \
      avp.getType() == ADTU64 ? "ADTU64" : \
      avp.getType() == ADTF32 ? "ADTF32" : \
      avp.getType() == ADTF64 ? "ADTF64" : \
      avp.getType() == ADTGrouped ? "ADTGrouped" : \
      avp.getType() == ADTAddress ? "ADTAddress" : \
      avp.getType() == ADTTime ? "ADTTime" : \
      avp.getType() == ADTUTF8String ? "ADTUTF8String" : \
      avp.getType() == ADTDiameterIdentity ? "ADTDiameterIdentity" : \
      avp.getType() == ADTDiameterURI ? "ADTDiameterURI" : \
      avp.getType() == ADTEnumerated ? "ADTEnumerated" : \
      avp.getType() == ADTIPFilterRule ? "ADTIPFilterRule" : "UNKNOWN", \
      value.GetType() == RAPIDJSON_NAMESPACE::kNullType ? "kNullType" : \
      value.GetType() == RAPIDJSON_NAMESPACE::kFalseType ? "kFalseType" : \
      value.GetType() == RAPIDJSON_NAMESPACE::kTrueType ? "kTrueType" : \
      value.GetType() == RAPIDJSON_NAMESPACE::kObjectType ? "kObjectType" : \
      value.GetType() == RAPIDJSON_NAMESPACE::kArrayType ? "kArrayType" : \
      value.GetType() == RAPIDJSON_NAMESPACE::kStringType ? "kStringType" : \
      value.GetType() == RAPIDJSON_NAMESPACE::kNumberType ? "kNumber" : "Unknown") ); \
}

static bool isHexString( const char *s, int len )
{
   if ( len % 2 != 0 )
   {
      /*
      throw runtimeInfo(
         string_format("%s:%d - INFO - Length of HEX string (%d) not divisible by 2",
         __FILE__, __LINE__, len)
      );
      */
      return false;
   }

   if ( len < 2 )
   {
      /*
      throw runtimeInfo(
         string_format("%s:%d - INFO - Length of HEX string (%d) less than 2",
         __FILE__, __LINE__, len)
      );
      */
      return false;
   }

   if ( s[0] != '0' || !(s[1] == 'x' || s[1] == 'X'))
   {
      /*
      throw runtimeInfo(
         string_format("%s:%d - INFO - First 2 characters of hex string are not \"0x\" or \"0X\"",
         __FILE__, __LINE__)
      );
      */
      return false;
   }

   for ( int i=2; i<len; i++ )
      if ( !ISHEXDIGIT( s[i] ) )
      {
         /*
         throw runtimeInfo(
            string_format("%s:%d - INFO - invalid character [%c] found in string of hex digits",
            __FILE__, __LINE__, (char)s[i])
         );
         */
         return false;
      }

   return true;
}

static void fdJsonAddAvps( AVP &avp, const RAPIDJSON_NAMESPACE::Value &element, void (*errfunc)(const char*) );
static void fdJsonAddAvps( msg_or_avp *reference, const RAPIDJSON_NAMESPACE::Value &element, void (*errfunc)(const char*) );

static void fdJsonAddAvp( msg_or_avp *reference, const char *name, const RAPIDJSON_NAMESPACE::Value &value, void (*errfunc)(const char*) )
{
   AVP avp( name );

   switch (value.GetType())
   {
      case RAPIDJSON_NAMESPACE::kFalseType:
      case RAPIDJSON_NAMESPACE::kTrueType:
      {
         throw runtimeInfo( string_format("%s:%d - INFO - Invalid format (true/false) for [%s] in JSON block, ignoring", __FILE__, __LINE__, name) );
         break;
      }
      case RAPIDJSON_NAMESPACE::kNullType:
      {
         throw runtimeInfo( string_format("%s:%d - INFO - Invalid NULL for [%s] in JSON block, ignoring", __FILE__, __LINE__, name) );
         break;
      }
      case RAPIDJSON_NAMESPACE::kNumberType:
      {
         switch ( avp.getBaseType() )
         {
            case AVP_TYPE_INTEGER32: {
               if (!value.IsInt()) THROW_DATATYPE_MISMATCH();
               avp.set( value.GetInt() );
               break;
            }
            case AVP_TYPE_INTEGER64: {
               if (!value.IsInt64()) THROW_DATATYPE_MISMATCH();
               avp.set( value.GetInt64() );
               break;
            }
            case AVP_TYPE_UNSIGNED32: {
               if (!value.IsUint()) THROW_DATATYPE_MISMATCH();
               avp.set( value.GetUint() );
               break;
            }
            case AVP_TYPE_UNSIGNED64: {
               if (!value.IsUint64()) THROW_DATATYPE_MISMATCH();
               avp.set( value.GetUint64() );
               break;
            }
            case AVP_TYPE_FLOAT32: {
               if (!value.IsFloat()) THROW_DATATYPE_MISMATCH();
               avp.set( value.GetFloat() );
               break;
            }
            case AVP_TYPE_FLOAT64: {
               if (!value.IsDouble()) THROW_DATATYPE_MISMATCH();
               avp.set( value.GetDouble() );
               break;
            }
            default:
            {
               THROW_DATATYPE_MISMATCH();
            }
         }

         avp.addTo( reference );

         break;
      }
      case RAPIDJSON_NAMESPACE::kStringType:
      {
         if ( avp.getBaseType() != AVP_TYPE_OCTETSTRING )
            THROW_DATATYPE_MISMATCH();
         
         size_t rawlen = strlen( value.GetString() );

         if ( avp.getType() == ADTAddress )
         {
            sSS ss;
            char abuf[18];

            if (inet_pton(AF_INET,value.GetString(),&((sSA4*)&ss)->sin_addr) == 1)
            {
               *(uint16_t *)abuf = htons(1);
               memcpy(abuf + 2, &((sSA4*)&ss)->sin_addr.s_addr, 4);
               avp.set( (uint8_t*)abuf, 6 );
            }
            else if (inet_pton(AF_INET6,value.GetString(),&((sSA6*)&ss)->sin6_addr) == 1)
            {
               *(uint16_t *)abuf = htons(2);
               memcpy(abuf + 2, &((sSA6*)&ss)->sin6_addr.s6_addr, 16);
               avp.set( (uint8_t*)abuf, 18 );
            }
            else
            {
               avp.set( (uint8_t*)value.GetString(), rawlen );
            }
         }
         else if ( avp.getType() == ADTTime )
         {
            union {
               uint32_t u;
               uint8_t u8[ sizeof( uint32_t ) ];
            } val;
            STime t;
            ntp_time_t ntp;

            t.ParseDateTime( value.GetString(), false );

            t.getNTPTime( ntp );

            val.u = ntp.second;

      #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            uint8_t u8;
            u8 = val.u8[0]; val.u8[0] = val.u8[3]; val.u8[3] = u8;
            u8 = val.u8[1]; val.u8[1] = val.u8[2]; val.u8[2] = u8;
      #endif

            avp.set( val.u8, sizeof(uint32_t) );
         }
         else if ( avp.getType() == ADTOctetString )
         {
            if ( isHexString( (const char *)value.GetString(), rawlen ) )
            {
               /*
                * hex string format is "0x" or "0X" followed by an even number of hex characters
                * binlen is equal to the final length + 1
                */
               size_t binlen = rawlen / 2;

               /*
                * allocate space for the binary string
                */
               avp.allocBuffer( binlen - 1 );

               /*
                * grab a pointer to the hex character buffer
                */
               const uint8_t *p = (const uint8_t*)value.GetString();

               /*
                * create the binary string from the hex digit string
                * start index at 1 (first hex digit divided by number of digits per byte, 2 / 2 = 1)
                * to start at the first hex digit
                */
               for (uint32_t i = 1; i < binlen; i++)
                  avp.getBuffer().get()[i-1] = (HEX2BIN(p[i * 2] ) << 4) + HEX2BIN(p[i * 2 + 1]);

               /*
                * assign the string to the avp
                */
               avp.set( avp.getBuffer().get(), binlen - 1 );
            }
            else
            {
               avp.set( (uint8_t*)value.GetString(), rawlen );
            }
         }
         else // some variant of a standard string
         {
            avp.set( (uint8_t*)value.GetString(), rawlen );
         }

         /* add to the message/grouped avp */
         avp.addTo( reference );

         break;
      }
      case RAPIDJSON_NAMESPACE::kArrayType:
      {
         /* iterate through array elements adding them to reference */
         for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator it = value.Begin();
              it != value.End();
              ++it)
         {
            fdJsonAddAvp( reference, name, *it, errfunc );
         }
         break;
      }
      case RAPIDJSON_NAMESPACE::kObjectType:
      {
         avp.addTo( reference );
         fdJsonAddAvps( avp, value, errfunc );
         break;
      }
      default:
      {
         throw runtimeInfo(
            string_format("%s:%d - INFO - invalid JSON data type encountered in element [%s]",
            __FILE__, __LINE__, name)
         );
         break;
      }
   }
}

static void fdJsonAddAvps( AVP &avp, const RAPIDJSON_NAMESPACE::Value &element, void (*errfunc)(const char*) )
{
   fdJsonAddAvps( avp.getAvp(), element, errfunc );
}

static void fdJsonAddAvps( msg_or_avp *reference, const RAPIDJSON_NAMESPACE::Value &element, void (*errfunc)(const char*) )
{
   /* iterate through each of the child elements adding them to the reference */
   for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator it = element.MemberBegin();
        it != element.MemberEnd();
        ++it)
   {
      try
      {
         fdJsonAddAvp( reference, it->name.GetString(), it->value, errfunc );
      }
      catch (runtimeInfo &exi)
      {
         errfunc( exi.what() );
      }
   }
}

int fdJsonAddAvps( const char *json, msg_or_avp *msg, void (*errfunc)(const char*) )
{
   int ret = FDJSON_SUCCESS;
   RAPIDJSON_NAMESPACE::Document doc;

   if (!json) {
      errfunc( string_format("%s:%d - ERROR - Error parsing JSON string", __FILE__, __LINE__).c_str() );
      return FDJSON_JSON_PARSING_ERROR;
   }

   if (doc.Parse<RAPIDJSON_NAMESPACE::kParseNoFlags>(json).HasParseError()) {
      errfunc( string_format("%s:%d - ERROR - Error parsing JSON string", __FILE__, __LINE__).c_str() );
      return FDJSON_JSON_PARSING_ERROR;
   }

   try
   {
      for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator it = doc.MemberBegin();
           it != doc.MemberEnd();
           ++it)
      {
         fdJsonAddAvp( msg, it->name.GetString(), it->value, errfunc );
      }
   }
   catch (runtimeError &ex)
   {
      errfunc( ex.what() );
      ret = FDJSON_EXCEPTION;
   }

   return ret;
}

std::string fdJsonBinaryToHex( const unsigned char *buffer, size_t len )
{
   static const char *hexDigits = "0123456789ABCDEF";
   std::stringstream ss;

   // add the 0x prefix
   ss << "0x";

   // loop through the buffer
   for ( size_t i = 0; i < len; i++ )
      ss << hexDigits[UPPERNIBBLE(buffer[i])] << hexDigits[LOWERNIBBLE(buffer[i])];

   // return the hex string
   return ss.str();
}

static std::string fdJsonTimeToStr( struct dict_avp_data *dictData, const unsigned char *buffer, size_t len )
{
   STime t;
   ntp_time_t ntp;
   std::string ts;

   if ( len != 4 )
      throw runtimeInfo(
         string_format("%s:%d - INFO - The length for Time is not 4 in AVP [%s]",
         __FILE__, __LINE__, dictData->avp_name)
      );

   ntp.fraction = 0;
   ntp.second = (
         ( (unsigned long)(buffer[0] & 0xff) ) << 24) +
         (                (buffer[1] & 0xff)   << 16) +
         (                (buffer[2] & 0xff) << 8) +
         (                (buffer[3] & 0xff)
      );

   t.setNTPTime( ntp );
   t.Format( ts, "%i", false );

   return ts;
}

static std::string fdJsonAddressToStr( struct dict_avp_data *dictData, const unsigned char *buffer, size_t len )
{
   sSS ss;
   char str[INET6_ADDRSTRLEN];

   // populate the sSS structure
   memset( &ss, 0, sizeof(ss) );

   uint16_t addressType = ntohs( *(uint16_t *)buffer );

   switch ( addressType )
   {
      case 1: // IPV4
      {
         if ( len != 6 )
            throw runtimeInfo(
               string_format("%s:%d - INFO - The length for an IPV4 address is not 6 in AVP [%s]",
               __FILE__, __LINE__, dictData->avp_name)
            );

         ((sSA4*)&ss)->sin_family = AF_INET;
         memcpy( &((sSA4*)&ss)->sin_addr.s_addr, &buffer[2], 4 );

         break;
      }
      case 2: // IPV6
      {
         if ( len != 18 )
            throw runtimeInfo(
               string_format("%s:%d - INFO - The length for an IPV4 address is not 18 in AVP [%s]",
               __FILE__, __LINE__, dictData->avp_name)
            );

         ((sSA6*)&ss)->sin6_family = AF_INET6;
         memcpy( &((sSA6*)&ss)->sin6_addr.s6_addr, &buffer[2], 16 );

         break;
      }
      default:
      {
         throw runtimeInfo(
            string_format("%s:%d - INFO - Unrecognized address type [%d] in AVP [%s]",
            __FILE__, __LINE__, addressType, dictData->avp_name)
         );
      }
   }

   // convert the address to a string
   switch ( ss.ss_family )
   {
      case AF_INET:
      {
         inet_ntop( AF_INET, &((sSA4*)&ss)->sin_addr, str, sizeof( str ) );
         break;
      }
      case AF_INET6:
      {
         inet_ntop( AF_INET6, &((sSA6*)&ss)->sin6_addr, str, sizeof( str ) );
         break;
      }
      default:
      {
         break;
      }
   }

   // return the value
   return std::string( str );
}

static void fdJsonAddMembers( msg_or_avp *ref, RAPIDJSON_NAMESPACE::Value &object, RAPIDJSON_NAMESPACE::Document::AllocatorType &allocator )
{
   int ret;
   struct avp *a;
   struct avp_hdr *hdr;
   struct dict_object *dictEntry;
   struct dict_avp_data dictData;
   struct dict_avp_data derivedData;
   bool isDerived = false;

   if ( fd_msg_browse_internal( ref, MSG_BRW_FIRST_CHILD, (msg_or_avp**)&a, NULL ) != 0 )
      return;

   do
   {
      if ( !a )
         break;

      // get the avp dictionary entry
      ret = fd_msg_model( a, &dictEntry );
      if ( ret != 0 )
         throw runtimeError(
            string_format("%s:%d - INFO - error fd_msg_model() returned %d",
               __FILE__, __LINE__, ret )
         );

      // get the avp data
      ret = fd_dict_getval( dictEntry, &dictData );
      if ( ret != 0 )
         throw runtimeError(
            string_format("%s:%d - INFO - error fd_dict_getval() returned %d",
               __FILE__, __LINE__, ret )
         );

      // the the derived type if applicable
      if ( dictData.avp_basetype == AVP_TYPE_OCTETSTRING )
      {
         struct dictionary *dict = NULL;
         struct dict_object *derivedtype = NULL;

         /* get the dictionary associated with the AVP dictionary entry */
         ret = fd_dict_getdict( dictEntry, &dict );
         if ( ret != 0 )
            throw runtimeInfo(
               string_format("%s:%d - INFO - Unable to retrieve the dictionary for the [%s] dictionary entry",
               __FILE__, __LINE__, dictData.avp_name)
            );

         /* get the dictionary entry associated with the derived type */
         ret = fd_dict_search( dict, DICT_TYPE, TYPE_OF_AVP, dictEntry, &derivedtype, EINVAL );
         if ( ret == 0 ) /* if found, then derived */
            isDerived = fd_dict_getval( derivedtype, &derivedData ) == 0;
      }

      if ( fd_msg_avp_hdr ( a, &hdr ) == 0 )
      {
         RAPIDJSON_NAMESPACE::Value avp_name;
         avp_name.SetString( dictData.avp_name, allocator );

         switch ( dictData.avp_basetype )
         {
            case AVP_TYPE_OCTETSTRING:
            {
               if ( isDerived )
               {
                  if ( !strcmp(derivedData.avp_name,"Address") )
                  {
                     std::string address = fdJsonAddressToStr( &dictData, hdr->avp_value->os.data, hdr->avp_value->os.len );
                     RAPIDJSON_NAMESPACE::Value v;
                     v.SetString( address.c_str(), address.length(), allocator );
                     object.AddMember( avp_name, v, allocator );
                  }
                  else if ( !strcmp(derivedData.avp_name,"Time") )
                  {
                     std::string tm = fdJsonTimeToStr( &dictData, hdr->avp_value->os.data, hdr->avp_value->os.len );
                     RAPIDJSON_NAMESPACE::Value v;
                     v.SetString( tm.c_str(), tm.length(), allocator );
                     object.AddMember( avp_name, v, allocator );
                  }
                  else
                  {
                     std::string str( (const char *)hdr->avp_value->os.data, hdr->avp_value->os.len );
                     RAPIDJSON_NAMESPACE::Value v;
                     v.SetString( str.c_str(), str.length(), allocator );
                     object.AddMember( avp_name, v, allocator );
                  }
               }
               else
               {
                  // convert each byte to 2 hex digits
                  std::string hex = fdJsonBinaryToHex( hdr->avp_value->os.data, hdr->avp_value->os.len );
                  RAPIDJSON_NAMESPACE::Value v;
                  v.SetString( hex.c_str(), hex.length(), allocator );
                  object.AddMember( avp_name, v, allocator );
               }
               break;
            }
            case AVP_TYPE_INTEGER32:
            {
               object.AddMember( avp_name, hdr->avp_value->i32, allocator );
               break;
            }
            case AVP_TYPE_INTEGER64:
            {
               object.AddMember( avp_name, hdr->avp_value->i64, allocator );
               break;
            }
            case AVP_TYPE_UNSIGNED32:
            {
               object.AddMember( avp_name, hdr->avp_value->u32, allocator );
               break;
            }
            case AVP_TYPE_UNSIGNED64:
            {
               object.AddMember( avp_name, hdr->avp_value->u64, allocator );
               break;
            }
            case AVP_TYPE_FLOAT32:
            {
               object.AddMember( avp_name, hdr->avp_value->f32, allocator );
               break;
            }
            case AVP_TYPE_FLOAT64:
            {
               object.AddMember( avp_name, hdr->avp_value->f64, allocator );
               break;
            }
            case AVP_TYPE_GROUPED:
            {
               RAPIDJSON_NAMESPACE::Value grouped( RAPIDJSON_NAMESPACE::kObjectType );
               fdJsonAddMembers( a, grouped, allocator );
               object.AddMember( avp_name, grouped, allocator );
               break;
            }
            default:
            {
            }
         }
      }
      else
      {
         throw runtimeInfo(
            string_format("%s:%d - INFO - Unable to retrieve the AVP header for [%s]",
            __FILE__, __LINE__, dictData.avp_name)
         );
      }
   } while ( fd_msg_browse_internal( a, MSG_BRW_NEXT, (msg_or_avp**)&a, NULL ) == 0 );
}

static std::string fdJsonGetName( msg_or_avp * ref )
{
   int ret;
   msg_or_avp *parent;
   struct dict_object *model;

   // determine if avp or msg
   bool isAvp = fd_msg_browse_internal( ref, MSG_BRW_PARENT, (msg_or_avp**)&parent, NULL ) == 0;

   ret = fd_msg_model( ref, &model );
   if ( ret != 0 )
      throw runtimeError(
         string_format("%s:%d - ERROR - error fd_msg_model() returned %d",
            __FILE__, __LINE__, ret )
      );

   if ( isAvp )
   {
      struct dict_avp_data ad;

      ret = fd_dict_getval( model, &ad );
      if ( ret != 0 )
         throw runtimeError(
            string_format("%s:%d - ERROR - error fd_dict_getval() returned %d",
               __FILE__, __LINE__, ret )
         );

      return std::string( ad.avp_name );
   }
   else
   {
      struct dict_cmd_data cd;

      ret = fd_dict_getval( model, &cd );
      if ( ret != 0 )
         throw runtimeError(
            string_format("%s:%d - ERROR - error fd_dict_getval() returned %d",
               __FILE__, __LINE__, ret )
         );

      return std::string( cd.cmd_name );
   }
}

void fdJsonGetJSON( msg_or_avp *ref, std::string &json, void (*errfunc)(const char *) )
{
   RAPIDJSON_NAMESPACE::Document doc;
   RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = doc.GetAllocator();

   // define the document as an object
   doc.SetObject();

   try
   {
      RAPIDJSON_NAMESPACE::Value object( RAPIDJSON_NAMESPACE::kObjectType );

      fdJsonAddMembers( ref, object, allocator );

      std::string name = fdJsonGetName( ref );
      RAPIDJSON_NAMESPACE::Value vname;
      vname.SetString( name.c_str(), name.length(), allocator );

      doc.AddMember( vname, object, allocator );

      RAPIDJSON_NAMESPACE::StringBuffer sb;
      RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(sb);
      doc.Accept(writer);

      json = sb.GetString();
   }
   catch (runtimeError &ex)
   {
      errfunc( ex.what() );
      return;
   }
}

const char *fdJsonGetJSON( msg_or_avp *ref, void (*errfunc)(const char *) )
{
   std::string json;

   fdJsonGetJSON( ref, json, errfunc );

   return strdup( json.c_str() );
}

bool fdJsonGetValueOfMember( std::string json, std::string member, std::string &value )
{
   RAPIDJSON_NAMESPACE::Document doc;
   doc.Parse(json.c_str());
   if( doc.HasMember(member.c_str()) )
   {
     if( doc[member.c_str()].IsString() )
      {
         value = doc[member.c_str()].GetString();
         return true;
      }
      else
      {
         return false;
      }
   }
   return false;

}

void parseRecursive(std::string scope
                    , RAPIDJSON_NAMESPACE::Value::ConstMemberIterator object
                    , std::unordered_map<std::string, std::string>& values)
{
    if (scope.empty())
    {
        scope = object->name.GetString();
    }
    else
    {
       scope = scope + "::" + object->name.GetString();
    }
    auto inElement = scope + "::";

    if (object->value.IsObject())
    {
        for (auto it = object->value.MemberBegin(); it != object->value.MemberEnd(); ++it)
        {
            parseRecursive(scope, it, values);
        }
    }
    else if ( object->value.IsString())
    {
        values.emplace(inElement, object->value.GetString());
    }
    else if (object->value.IsDouble())
    {
        values.emplace(inElement, std::to_string(object->value.GetDouble()));
    }
    else if (object->value.IsInt())
    {
        values.emplace(inElement, std::to_string(object->value.GetInt()));
    }
    else
    {
        //std::cout << "Unsupported: " << inElement << object->name.GetString() << std::endl ;
    }

}

bool fdJsonGetApnValueFromSubData( std::string json, std::string &apn )
{
    RAPIDJSON_NAMESPACE::Document doc;
    std::unordered_map<std::string, std::string> _values;
    if (!json.c_str() )
    {
        std::cout << __FILE__ ":" << __LINE__ << " - ERROR - Error parsing JSON string" << std::endl;
        return false;
    }

    if (doc.Parse<RAPIDJSON_NAMESPACE::kParseNoFlags>(json.c_str()).HasParseError())
    {
        std::cout << __FILE__ ":" << __LINE__ << " - ERROR - Error parsing JSON string" << std::endl;
        return false;
    }

    try
    {
        for (RAPIDJSON_NAMESPACE::Value::ConstMemberIterator it = doc.MemberBegin();
               it != doc.MemberEnd();
               ++it)
        {
            parseRecursive("", it, _values);
        }

        for (auto& x: _values)
        {
            if( (x.first).find("Service-Selection") != std::string::npos )
            {
                apn = x.second;
                return true;
            }

        }
        return false;
    }

    catch (runtimeError &ex)
    {
        throw runtimeError(
             string_format("%s:%d - ERROR - Error parsing json string",
            __FILE__, __LINE__)
        );
        return false;
    }

}
