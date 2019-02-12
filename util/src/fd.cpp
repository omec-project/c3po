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


#include <string>
#include <iostream>

#include "fd.h"
#include "fdjson.h"
#include "sutility.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void copyall( msg_or_avp *from, msg_or_avp *to );

static void copy( struct avp *from, msg_or_avp *to )
{
   int ret;
   struct dict_object *model;
   struct dict_avp_data avp_data;
   struct avp *a;

   ret = fd_msg_model( from, &model );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Error [%d] retrieving avp dict_object",
         __FILE__, __LINE__, ret )
      );

   ret = fd_dict_getval( model, &avp_data );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Error [%d] retrieving avp dictionary data",
         __FILE__, __LINE__, ret )
      );

   // create the AVP
   ret = fd_msg_avp_new( model, 0, &a );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Error [%d] createing new AVP [%s]",
         __FILE__, __LINE__, ret, avp_data.avp_name )
      );

   // check for individual or grouped AVP
   if ( avp_data.avp_basetype == AVP_TYPE_GROUPED )
   {
      // copy all AVP's from the "from" AVP to the new AVP "a"
      copyall( from, a );
   }
   else
   {
      struct avp_hdr *hdr;

      // get the avp header
      ret = fd_msg_avp_hdr( from, &hdr );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - fd_msg_avp_hdr returned %d retrieving header for [%s]",
            __FILE__, __LINE__, ret, avp_data.avp_name )
         );

      // set the value of the new AVP from the existing
      ret = fd_msg_avp_setvalue( a, hdr->avp_value );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - fd_msg_avp_setvalue returned %d setting the AVP value for [%s]",
            __FILE__, __LINE__, ret, avp_data.avp_name )
         );
   }

   // add the avp to the destination
   ret = fd_msg_avp_add( to, MSG_BRW_LAST_CHILD, a );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Error [%d] adding [%s] AVP",
         __FILE__, __LINE__, ret, avp_data.avp_name )
      );
}

static void copyall( msg_or_avp *from, msg_or_avp *to )
{
   int ret;

   // iterate through all "source" AVP's adding them to the newly create AVP
   struct avp *loopavp;

   // get the first child AVP
   ret = fd_msg_browse_internal( from, MSG_BRW_FIRST_CHILD, (msg_or_avp**)&loopavp, NULL);
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - browse returned %d direction MSG_BRW_FIRST_CHILD",
         __FILE__, __LINE__, ret )
      );

   while ( loopavp )
   {
      // add the avp
      copy( loopavp, to );

      // get the next AVP
      ret = fd_msg_browse_internal( loopavp, MSG_BRW_NEXT, (msg_or_avp**)&loopavp, NULL );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - browse returned %d direction MSG_BRW_NEXT",
            __FILE__, __LINE__, ret )
         );
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDEngine::FDEngine( const char *cfgfile )
{
   if ( cfgfile )
      setConfigFile( cfgfile );
}

FDEngine::FDEngine( const std::string &cfgfile )
{
   setConfigFile( cfgfile );
}

FDEngine::~FDEngine()
{
}

bool FDEngine::init()
{
   int ret;

   // initialize the freeDiameter core
   if ( ( ret = fd_core_initialize() ) != 0 )
   {
      std::cout << "An error occurred during freeDiameter core library initialization." << std::endl;
      return false;
   }

   // Parse the configuration file
   if ( ( ret = fd_core_parseconf( m_cfgfile.c_str() ) ) != 0 )
   {
      std::cout << "An error occurred parsing the freeDiameter configuration file." << std::endl;
      return false;
   }

   return true;
}

bool FDEngine::start()
{
   int ret;

   // Start the servers
   if ( ( ret = fd_core_start() ) != 0 )
   {
      std::cout << "An error occurred in fd_core_start()." << std::endl;
      return false;
   }

   std::cout << "The freeDiameter engine has been started" << std::endl;

   return true;
}

void FDEngine::uninit( bool wait )
{
   fd_core_shutdown();
   if ( wait )
   {
      waitForShutdown();
      std::cout << "The freeDiameter engine has been stopped" << std::endl;
   }
}

void FDEngine::waitForShutdown()
{
   fd_core_wait_shutdown_complete();
}

void FDEngine::advertiseSupport( FDDictionaryEntryApplication &app, int auth, int acct )
{
   int ret;

   ret = fd_disp_app_support( app.getEntry(), NULL, auth, acct );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Unable to register support for [%s] ret=%d",
         __FILE__, __LINE__, app.getName(), ret )
      );
}

void FDEngine::advertiseSupport( FDDictionaryEntryApplication &app, FDDictionaryEntryVendor &vendor, int auth, int acct )
{
   int ret;

   // always advertise support in using top level Auth-Application-ID or Acct-Applicaiton-ID
   //advertiseSupport( app, auth, acct );

   // aadvertise support using Vendor-Specific-Application-Id
   ret = fd_disp_app_support( app.getEntry(), vendor.getEntry(), auth, acct );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Unable to register support for [%s] vendor [%s] ret=%d",
         __FILE__, __LINE__, app.getName(), vendor.getName(), ret )
      );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDDictionaryEntry::FDDictionaryEntry()
{
   m_destroy = false;
   m_de = NULL;
}

FDDictionaryEntry::FDDictionaryEntry( const void *what, enum dict_object_type type, int criteria, struct dictionary *dict )
{
   m_destroy = false;
   init( what, type, criteria, dict );
}

void FDDictionaryEntry::init( const void *what, enum dict_object_type type, int criteria, struct dictionary *dict )
{
   // assign the dictionary
   m_dict = dict ? dict : fd_g_config->cnf_dict;

   // look up the dictionary entry
   if ( fd_dict_search( m_dict, type, criteria, what, &m_de, ENOENT ) != 0 )
      m_de = NULL;
}

void FDDictionaryEntry::init( struct dict_object *de, struct dictionary *dict )
{
   if ( !de )
      return;

   if ( dict == NULL )
   {
      int ret;

      ret = fd_dict_getdict( de, &dict );
      if ( ret != 0 )
         return;
   }

   m_dict = dict;
   m_de = de;
}

FDDictionaryEntry::~FDDictionaryEntry()
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDDictionaryEntryApplication::FDDictionaryEntryApplication( const char *name, struct dictionary *dict )
   : FDDictionaryEntry( name, DICT_APPLICATION, APPLICATION_BY_NAME, dict )
{
   if ( !isValid() )
      throw FDException(
         SUtility::string_format( "%s:%d - Unable to find application dictionary entry for [%s]",
         __FILE__, __LINE__, name )
      );

   init();
}

bool FDDictionaryEntryApplication::init()
{
   int ret;

   ret = fd_dict_getval( getEntry(), &m_data );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format( "%s:%d - Unable to find get data associated with application",
         __FILE__, __LINE__ )
      );

   return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDDictionaryEntryVendor::FDDictionaryEntryVendor( const char *name, struct dictionary *dict )
   : FDDictionaryEntry( name, DICT_VENDOR, VENDOR_BY_NAME, dict )
{
   if ( !isValid() )
      throw FDException(
         SUtility::string_format( "%s:%d - Unable to find vendor dictionary entry for [%s]",
         __FILE__, __LINE__, name )
      );

   init();
}

FDDictionaryEntryVendor::FDDictionaryEntryVendor( vendor_id_t vendorid, struct dictionary *dict )
   : FDDictionaryEntry( &vendorid, DICT_VENDOR, VENDOR_BY_ID, dict )
{
   if ( !isValid() )
      throw FDException(
         SUtility::string_format( "%s:%d - Unable to find vendor dictionary entry for vendor id [%u]",
         __FILE__, __LINE__, vendorid )
      );

   init();
}

FDDictionaryEntryVendor::FDDictionaryEntryVendor( const FDDictionaryEntryApplication &app, struct dictionary *dict )
   : FDDictionaryEntry( app.getEntry(), DICT_VENDOR, VENDOR_OF_APPLICATION, dict )
{
   if ( !isValid() )
      throw FDException(
         SUtility::string_format( "%s:%d - Unable to find vendor dictionary entry for application [%s]",
         __FILE__, __LINE__, app.getName() )
      );

   init();
}

bool FDDictionaryEntryVendor::init()
{
   int ret;

   ret = fd_dict_getval( getEntry(), &m_data );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format( "%s:%d - Unable to find get data associated with vendor",
         __FILE__, __LINE__ )
      );

   return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDDictionaryEntryAVP::FDDictionaryEntryAVP( const char *name, bool allVendors , struct dictionary *dict )
   : FDDictionaryEntry( name, DICT_AVP, allVendors ? AVP_BY_NAME_ALL_VENDORS : AVP_BY_NAME, dict ),
     m_name( name ),
     m_vendorid( 0 ),
     m_derivedtype( NULL ),
     m_isderived( false ),
     m_datatype( DDTUnknown )
{
   memset( &m_basedata, 0, sizeof( m_basedata ) );
   memset( &m_derivedtypedata, 0, sizeof( m_derivedtypedata ) );

   if ( isValid() )
      getTypeInfo();
   else
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to find AVP dictionary entry for [%s]",
         __FILE__, __LINE__, name )
      );
}

FDDictionaryEntryAVP::FDDictionaryEntryAVP( const char *name, vendor_id_t vendorid, struct dictionary *dict )
   : FDDictionaryEntry(),
     m_vendorid( 0 ),
     m_derivedtype( NULL ),
     m_isderived( false ),
     m_datatype( DDTUnknown )
{
   memset( &m_basedata, 0, sizeof( m_basedata ) );

   struct dict_avp_request what;
   what.avp_vendor = vendorid;
   what.avp_name   = (char*)name;

   init( &what, DICT_AVP, AVP_BY_NAME_AND_VENDOR, dict );

   if ( isValid() )
      getTypeInfo();
   else
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to find AVP dictionary entry for [%s] vendor [%u]",
         __FILE__, __LINE__, name, vendorid )
      );
}

FDDictionaryEntryAVP::FDDictionaryEntryAVP( struct dict_object *de )
   : FDDictionaryEntry(),
     m_vendorid( 0 ),
     m_derivedtype( NULL ),
     m_isderived( false ),
     m_datatype( DDTUnknown )
{
   memset( &m_basedata, 0, sizeof( m_basedata ) );

   init( de );

   if ( isValid() )
      getTypeInfo();
   else
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - dict_object* is NULL",
         __FILE__, __LINE__ )
      );
}

void FDDictionaryEntryAVP::getTypeInfo()
{
   int ret = fd_dict_getval( getEntry(), &m_basedata );
   if ( ret == 0 )
   {
      struct dictionary *dict;
      ret = fd_dict_getdict( getEntry(), &dict );
      if ( ret == 0 )
      {
         ret = fd_dict_search( dict, DICT_TYPE, TYPE_OF_AVP, getEntry(), &m_derivedtype, EINVAL );
         if ( ret == 0 )
         {
            ret = fd_dict_getval( m_derivedtype, &m_derivedtypedata );
            if ( ret == 0 )
            {
               m_isderived = true;
               if      ( !strcmp( m_derivedtypedata.type_name, "Time" ) )              m_datatype = DDTTime;
               else if ( !strcmp( m_derivedtypedata.type_name, "Address" ) )           m_datatype = DDTAddress;
               else if ( !strcmp( m_derivedtypedata.type_name, "UTF8String" ) )        m_datatype = DDTUTF8String;
               else if ( !strcmp( m_derivedtypedata.type_name, "DiameterIdentity" ) )  m_datatype = DDTDiameterIdentity;
               else if ( !strcmp( m_derivedtypedata.type_name, "DiameterURI" ) )       m_datatype = DDTDiameterURI;
               else if ( !strcmp( m_derivedtypedata.type_name, "IPFilterRule" ) )      m_datatype = DDTIPFilterRule;
            }
         }
         else
         {
            switch ( m_basedata.avp_basetype )
            {
               case AVP_TYPE_GROUPED:      { m_datatype = DDTGrouped;     break; }
	       case AVP_TYPE_OCTETSTRING:  { m_datatype = DDTOctetString; break; }
	       case AVP_TYPE_INTEGER32:    { m_datatype = DDTI32;         break; }
	       case AVP_TYPE_INTEGER64:    { m_datatype = DDTI64;         break; }
	       case AVP_TYPE_UNSIGNED32:   { m_datatype = DDTU32;         break; }
	       case AVP_TYPE_UNSIGNED64:   { m_datatype = DDTU64;         break; }
	       case AVP_TYPE_FLOAT32:      { m_datatype = DDTF32;         break; }
	       case AVP_TYPE_FLOAT64:      { m_datatype = DDTF64;         break; }
            }
            
            m_derivedtype = NULL;
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDDictionaryEntryCommand::FDDictionaryEntryCommand( const char *name, struct dictionary *dict )
   : FDDictionaryEntry( name, DICT_COMMAND, CMD_BY_NAME, dict )
{
   if ( !isValid() )
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to find command dictionary entry for [%s]",
         __FILE__, __LINE__, name )
      );

   int ret = fd_dict_getval( getEntry(), &m_data );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to retrieve command data for [%s]",
         __FILE__, __LINE__, name )
      );
}

FDDictionaryEntryCommand::FDDictionaryEntryCommand( command_code_t cmdid, struct dictionary *dict )
   : FDDictionaryEntry( (const void *)&cmdid, DICT_COMMAND, CMD_BY_CODE_R, dict )
{
   if ( !isValid() )
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to find command dictionary entry for command code [%u]",
         __FILE__, __LINE__, cmdid )
      );

   int ret = fd_dict_getval( getEntry(), &m_data );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to find command dictionary entry for command code [%u]",
         __FILE__, __LINE__, cmdid )
      );
}

FDDictionaryEntryCommand::FDDictionaryEntryCommand( const FDDictionaryEntryCommand &req, struct dictionary *dict )
   : FDDictionaryEntry( req.getEntry(), DICT_COMMAND, CMD_ANSWER, dict != NULL ? dict : (struct dictionary *)req.getDictionary() )
{
   if ( !isValid() )
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to find answer command dictionary entry for request command [%s]",
         __FILE__, __LINE__, req.m_data.cmd_name )
      );

   int ret = fd_dict_getval( getEntry(), &m_data );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to find answer command dictionary entry for answer command [%s]",
         __FILE__, __LINE__, req.m_data.cmd_name )
      );
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// FDApplication

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDAvp::FDAvp( FDDictionaryEntryAVP &de, bool dedel )
   : m_de( &de ),
     m_avp( NULL ),
     m_avphdr( NULL ),
     m_buf( NULL ),
     m_assigned( false ),
     m_dedel( dedel )
{
   init();
}

FDAvp::FDAvp( FDDictionaryEntryAVP &de, struct avp *a, bool dedel )
   : m_de( &de ),
     m_avp( a ),
     m_avphdr( NULL ),
     m_buf( NULL ),
     m_assigned( true ),
     m_dedel( dedel )
{
   memset( &m_value, 0, sizeof( m_value ) );

   if ( m_avp )
   {
      int ret = fd_msg_avp_hdr( m_avp, &m_avphdr );
      if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - error retrieving AVP header",
         __FILE__, __LINE__ )
      );
   }
}

FDAvp::~FDAvp()
{
   if ( m_buf )
      delete m_buf;

   if ( !m_assigned && m_avp )
      fd_msg_free( m_avp );

   if ( m_dedel )
      delete m_de;
}

FDAvp &FDAvp::add( FDAvp &avp )
{
   avp.addTo( m_avp );
   return *this;
}

bool FDAvp::get( int64_t &v )
{
   if ( m_avphdr == NULL )
      return false;

   if ( m_de->getDataType() == DDTTime )
   {
      union {
         uint32_t u;
         uint8_t u8[ sizeof( uint32_t ) ];
      } val;

      val.u = *(uint32_t*)m_avphdr->avp_value->os.data;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      uint8_t u8;
      u8 = val.u8[0]; val.u8[0] = val.u8[3]; val.u8[3] = u8;
      u8 = val.u8[1]; val.u8[1] = val.u8[2]; val.u8[2] = u8;
#endif

      v = ((int64_t)val.u) - (int64_t)2208988800;
   }
   else
   {
      v = m_avphdr->avp_value->i64;
   }

   return true;
}

bool FDAvp::get( std::string &v )
{
   if ( m_avphdr == NULL )
      return false;

   switch ( m_de->getDataType() )
   {
      case DDTAddress:
      {
         char str[INET6_ADDRSTRLEN];
         size_t len = sizeof( str );

         if ( get( str, len ) )
         {
            v.assign( str, len );
         }
         else
         {
            v.clear();
            return false;
         }
         break;
      }
      case DDTUTF8String:
      case DDTDiameterIdentity:
      case DDTDiameterURI:
      {
         v.assign( (char *)m_avphdr->avp_value->os.data, m_avphdr->avp_value->os.len );
         break;
      }
      default:
      {
         v.clear();
         return false;
      }
   }

   return true;
}

bool FDAvp::get( char *data, size_t &len )
{
   if ( m_avphdr == NULL )
      return false;

   switch ( m_de->getDataType() )
   {
      case DDTAddress:
      {
         sSS ss;
         char str[INET6_ADDRSTRLEN];

         if ( !get( ss ) )
            return false;

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

         if ( strlen( str ) >= len ) // no enough root
            return false;

         strcpy( data, str );
         len = strlen( str );

         break;
      }
      case DDTUTF8String:
      case DDTDiameterIdentity:
      case DDTDiameterURI:
      {
         if ( m_avphdr->avp_value->os.len >= len )
            return false;

         memcpy( data, m_avphdr->avp_value->os.data, m_avphdr->avp_value->os.len );
         data[ m_avphdr->avp_value->os.len ] = '\0';
         len = m_avphdr->avp_value->os.len;

         break;
      }
      case DDTOctetString:
      {
         if ( m_avphdr->avp_value->os.len >= len )
            return false;
 
         memcpy( data, m_avphdr->avp_value->os.data, m_avphdr->avp_value->os.len );
         len = m_avphdr->avp_value->os.len;

         break;
      }
      default:
      {
         break; 
      }
   }

   return true;
}

bool FDAvp::get( uint8_t *data, size_t &len )
{
   if ( m_avphdr == NULL )
      return false;

   if ( m_de->getDataType() != DDTOctetString )
      return false;

   if ( m_avphdr->avp_value->os.len > len )
      return false;

   memcpy( data, m_avphdr->avp_value->os.data, m_avphdr->avp_value->os.len );
   len = m_avphdr->avp_value->os.len;

  return true;
}

bool FDAvp::get( sSS &ss )
{
   if ( m_avphdr == NULL )
      return false;

   memset( &ss, 0, sizeof(ss) );

   uint16_t addressType = ntohs(*(uint16_t *)m_avphdr->avp_value->os.data);

   switch ( addressType )
   {
      case 1: // IPV4
      {
         if ( m_avphdr->avp_value->os.len != 6 )
            return false;

         ((sSA4*)&ss)->sin_family = AF_INET;
         memcpy( &((sSA4*)&ss)->sin_addr.s_addr, &m_avphdr->avp_value->os.data[2], 4 );

         break;
      }
      case 2: // IPV6
      {
         if ( m_avphdr->avp_value->os.len != 18 )
            return false;

         ((sSA6*)&ss)->sin6_family = AF_INET6;
         memcpy( &((sSA6*)&ss)->sin6_addr.s6_addr, &m_avphdr->avp_value->os.data[2], 16 );

         break;
      }
      default:
      {
         return false;
      }
   }

   return true;
}

bool FDAvp::get( STime &v )
{
   if ( m_avphdr == NULL )
      return false;

   if ( m_de->getDataType() == DDTTime )
   {
      union {
         uint32_t u;
         uint8_t u8[ sizeof( uint32_t ) ];
      } val;

      val.u = *(uint32_t*)m_avphdr->avp_value->os.data;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      uint8_t u8;
      u8 = val.u8[0]; val.u8[0] = val.u8[3]; val.u8[3] = u8;
      u8 = val.u8[1]; val.u8[1] = val.u8[2]; val.u8[2] = u8;
#endif

      ntp_time_t ntp;

      ntp.second = val.u;
      ntp.fraction = 0;

      v.setNTPTime( ntp );
   }
   else
   {
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to assign [%s] to STime value",
         __FILE__, __LINE__, m_de->getName() )
      );
   }

   return true;
}

FDAvp &FDAvp::set( int64_t v )
{
   if ( m_de->getDataType() == DDTTime )
   {
      m_buf = new FDBuffer<uint8_t>( sizeof( uint32_t ) );

      union {
         uint32_t u;
         uint8_t u8[ sizeof( uint32_t ) ];
      } val;

      val.u = (uint32_t)v + 2208988800UL;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      uint8_t u8;
      u8 = val.u8[0]; val.u8[0] = val.u8[3]; val.u8[3] = u8;
      u8 = val.u8[1]; val.u8[1] = val.u8[2]; val.u8[2] = u8;
#endif
      set( val.u8, sizeof(uint32_t) );
      //*((uint32_t*)m_buf->get()) = val.u;
   }
   else
   {
      m_value.i64 = v; return *this;
   }

   assignValue();

   return *this;
}

FDAvp &FDAvp::set( const char *v, size_t len )
{
   size_t rawlen = len;
   sSS ss;

   if ( m_de->getDataType() == DDTAddress )
   {
      if ( inet_pton( AF_INET, v, &((sSA4*)&ss)->sin_addr ) == 1 )
      {
         m_buf = new FDBuffer<uint8_t>( 18 );
         *(uint16_t *)m_buf->get() = htons(1);
         memcpy( m_buf->get() + 2, &((sSA4*)&ss)->sin_addr.s_addr, 4 );
         set( m_buf->get(), 6 );
      }
      else if ( inet_pton( AF_INET6, v, &((sSA6*)&ss)->sin6_addr ) == 1 )
      {
         m_buf = new FDBuffer<uint8_t>( 18 );
         *(uint16_t *)m_buf->get() = htons(2);
         memcpy( m_buf->get() + 2, &((sSA6*)&ss)->sin6_addr.s6_addr, 16 );
         set( m_buf->get(), 18 );
      }
      else
      {
         set( (uint8_t*)v, rawlen );
      }
   }
   else
   {
      set( (uint8_t*)v, rawlen );
   }

   assignValue();

   return *this;
}

FDAvp &FDAvp::set( const STime &v )
{
   if ( m_de->getDataType() == DDTTime )
   {
      m_buf = new FDBuffer<uint8_t>( sizeof( uint32_t ) );

      union {
         uint32_t u;
         uint8_t u8[ sizeof( uint32_t ) ];
      } val;

      ntp_time_t ntp;

      v.getNTPTime( ntp );

      val.u = ntp.second; // + 2208988800UL;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
      uint8_t u8;
      u8 = val.u8[0]; val.u8[0] = val.u8[3]; val.u8[3] = u8;
      u8 = val.u8[1]; val.u8[1] = val.u8[2]; val.u8[2] = u8;
#endif

      set( val.u8, sizeof(uint32_t) );
   }
   else
   {
      throw FDException(
         SUtility::string_format( "%s:%d - INFO - Unable to assign STime value to [%s]",
         __FILE__, __LINE__, m_de->getName() )
      );
   }

   assignValue();

   return *this;
}

FDAvp FDAvp::getNext( bool &found )
{
   int ret;
   FDDictionaryEntryAVP *de = NULL;
   struct avp *a = NULL;

   found = fd_msg_browse_internal( m_avp, MSG_BRW_NEXT, (msg_or_avp**)&a, NULL ) == 0;

   if ( found )
   {
      struct dict_object *model;

      ret = fd_msg_model( a, &model );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - INFO - unalbe to retrieve avp model",
            __FILE__, __LINE__, ret )
         );

      de = new FDDictionaryEntryAVP( model );
   }

   if( de != NULL )
	return FDAvp( *de, a );
   else
	return *this;
}

FDAvp FDAvp::getChild( bool &found )
{
   int ret;
   FDDictionaryEntryAVP *de = NULL;
   struct avp *a = NULL;

   if ( m_de->getDataType() == DDTGrouped )
   {
      found = fd_msg_browse_internal( m_avp, MSG_BRW_LAST_CHILD, (msg_or_avp**)&a, NULL ) == 0;

      if ( found )
      {
         struct dict_object *model;

         ret = fd_msg_model( a, &model );
         if ( ret != 0 )
            throw FDException(
               SUtility::string_format("%s:%d - INFO - unalbe to retrieve avp model",
               __FILE__, __LINE__, ret )
            );

         de = new FDDictionaryEntryAVP( model );
      }
   }

   if ( de != NULL )
	return FDAvp( *de, a );
   else
	return *this;
}

void FDAvp::addTo( msg_or_avp *reference )
{
   int ret;

   if ( ( ret = fd_msg_avp_add( reference, MSG_BRW_LAST_CHILD, m_avp ) ) != 0)
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Error [%d] adding [%s] AVP",
         __FILE__, __LINE__, ret, m_de->getName() )
      );

   m_assigned = true;
}

void FDAvp::init()
{
   int ret;

   if ( !m_avp && ( ret = fd_msg_avp_new( m_de->getEntry(), 0, &m_avp ) ) != 0)
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Error [%d] creating [%s] AVP",
         __FILE__, __LINE__, ret, m_de->getName() )
      );

   memset( &m_value, 0, sizeof( m_value ) );
}

void FDAvp::assignValue()
{
   try
   {
      if ( m_de->getDataType() != DDTGrouped )
      {
         int ret;

         if ( ( ret = fd_msg_avp_setvalue( m_avp, &m_value ) ) != 0)
            throw FDException(
               SUtility::string_format("%s:%d - ERROR - Error [%d] setting AVP value for [%s]",
               __FILE__, __LINE__, ret, m_de->getName() )
            );
      }
   }
   catch (...)
   {
      fd_msg_free( m_avp );
      m_avp = NULL;
      throw;
   }
}

FDAvp &FDAvp::setAvp( struct avp *a )
{
   if ( m_avp )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - m_avp already assigned",
         __FILE__, __LINE__ )
      );

   m_avp = a;

   int ret = fd_msg_avp_hdr( m_avp, &m_avphdr );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - error retrieving AVP header fd_msg_avp_hdr ret=%d",
         __FILE__, __LINE__, ret )
      );

   return *this;
}

FDAvp &FDAvp::add( FDExtractor &e )
{
   if ( e.exists() )
   {
      copy( (struct avp *)e.getReference(), getAvp() );
   }

   return *this;
}

FDAvp &FDAvp::add( FDExtractorList &el )
{
   for ( std::list<FDExtractor*>::iterator it = el.getList().begin();
         it != el.getList().end();
         ++it )
   {
      add( *(*it) );
   }

   return *this;
}

FDAvp &FDAvp::add( FDExtractorAvp &ea )
{
   if ( ea.exists() )
   {
      copy( ea.getAvp(), getAvp() );
   }

   return *this;
}

FDAvp &FDAvp::add( FDExtractorAvpList &eal )
{
   for ( std::list<FDExtractorAvp*>::iterator it = eal.getList().begin();
         it != eal.getList().end();
         ++it )
   {
      add( *(*it) );
   }

   return *this;
}

FDAvp &FDAvp::addJson( const char *json )
{
   fdJsonAddAvps( json, getAvp(), NULL );

   return *this;
}

bool FDAvp::getJson( std::string &json )
{
   fdJsonGetJSON( getAvp(), json, NULL );
   return true;
}

void FDAvp::dump()
{
   struct dictionary *dict;
   char * buf = NULL;
   size_t len = 0;
   int ret;

   ret = fd_dict_getdict( m_de->getEntry(), &dict );
   if ( ret != 0 )
      dict = fd_g_config->cnf_dict;

   fd_msg_dump_treeview(&buf, &len, NULL, m_avp, dict, 0, 1);

   std::cout << buf << std::endl;

   free(buf);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDMessage::FDMessage( bool req2ans, FDDictionaryEntryCommand *de, struct msg *pmsg, bool dedel, bool msgdel )
   : m_de( de ),
     m_dedel( dedel ),
     m_msgdel( msgdel ),
     m_msg( pmsg )
{
   int ret;

   if ( m_msg )
   {
      struct msg_hdr *hdr;

      ret = fd_msg_hdr( pmsg, &hdr );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - Error retrieving message header fd_msg_hdr ret=%d",
            __FILE__, __LINE__, ret )
         );

      if ( FDISREQUEST(hdr) )
      {
         // create the message from the request
         ret = fd_msg_new_answer_from_req( m_de->getDictionary(), &m_msg, 0 );
         if ( ret != 0 )
            throw FDException(
               SUtility::string_format("%s:%d - ERROR - Error creating answer message from request fd_msg_new_answer_from_rea ret=%d",
               __FILE__, __LINE__, ret )
            );

         setMsgDelete( false );
      }
      else
      {
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - Expected msg is not a request",
            __FILE__, __LINE__ )
         );
      }
   }
   else
   {
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - Error creating command",
         __FILE__, __LINE__ )
      );
   }
}

FDMessage::FDMessage( FDDictionaryEntryCommand *de, struct msg *pmsg, bool dedel )
   : m_de( de ),
     m_dedel( dedel ),
     m_msgdel( true ),
     m_msg( pmsg )
{
   int ret;

   if ( !m_msg )
   {
      ret = fd_msg_new( m_de->getEntry(), 0, &m_msg );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - Error creating command",
            __FILE__, __LINE__ )
         );
   }
}

FDMessage::FDMessage( FDDictionaryEntryApplication *ade, FDDictionaryEntryCommand *cde, struct msg *pmsg, bool dedel )
   : m_de( cde ),
     m_dedel( dedel ),
     m_msgdel( true ),
     m_msg( pmsg )
{
   int ret;

   if ( !m_msg )
   {
      ret = fd_msg_new_appl( m_de->getEntry(), ade->getEntry(), 0, &m_msg );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - Error creating command",
            __FILE__, __LINE__ )
         );
   }
}

FDMessage::~FDMessage()
{
   if ( m_msg && m_msgdel )
      fd_msg_free( m_msg );
   if ( m_dedel )
      delete m_de;
}

FDAvp FDMessage::findAVP( FDDictionaryEntryAVP &de )
{
   int ret;
   struct avp *a;

   ret =  fd_msg_search_avp ( m_msg, de.getEntry(), &a );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - AVP not found for [%s] (vendor id %u)",
         __FILE__, __LINE__, de.getName(), de.getVendorId() )
      );

   return FDAvp( de, a );
}

FDAvp FDMessage::getFirstAVP( bool &found )
{
   int ret;
   FDDictionaryEntryAVP *de = NULL;
   struct avp *child = NULL;

   found = fd_msg_browse_internal( m_msg, MSG_BRW_FIRST_CHILD, (msg_or_avp**)&child, NULL ) == 0;

   if ( found )
   {
      struct dict_object *model;

      ret = fd_msg_model( child, &model );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - INFO - unalbe to regrieve avp model",
            __FILE__, __LINE__, ret )
         );

      de = new FDDictionaryEntryAVP( model );
   }
   return FDAvp( *de, child, true );
}

void FDMessage::dump()
{
   struct dictionary *dict;
   char * buf = NULL;
   size_t len = 0;
   int ret;

   ret = fd_dict_getdict( m_de->getEntry(), &dict );
   if ( ret != 0 )
      dict = fd_g_config->cnf_dict;

   fd_msg_dump_treeview(&buf, &len, NULL, m_msg, dict, 0, 1);

   std::cout << buf << std::endl;

   free(buf);
}

FDMessage &FDMessage::sendRequest( void (*anscb)(void*,struct msg**), FDMessageRequest &req )
{
   int ret;

   // send the message
   ret = fd_msg_send( &m_msg, anscb, &req );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - INFO - error attempting to send request message ret=%d",
         __FILE__, __LINE__, ret )
      );

   return *this;
}

FDMessage &FDMessage::sendAnswer()
{
   int ret;

   // send the message
   ret = fd_msg_send( &m_msg, NULL, NULL );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - INFO - error attempting to send answer message ret=%d",
         __FILE__, __LINE__, ret )
      );

   return *this;
}

void FDMessage::addOrigin()
{
   int ret = fd_msg_add_origin( m_msg, 0 );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - Exception - error adding Origin-Host and Origin-Realm ret=%d",
         __FILE__, __LINE__, ret )
      );
}

FDMessage &FDMessage::add( FDExtractor &e )
{
   if ( e.exists() )
   {
      copy( (struct avp *)e.getReference(), getMsg() );
   }

   return *this;
}

FDMessage &FDMessage::add( FDExtractorList &el )
{
   for ( std::list<FDExtractor*>::iterator it = el.getList().begin();
         it != el.getList().end();
         ++it )
   {
      add( *(*it) );
   }

   return *this;
}

FDMessage &FDMessage::add( FDExtractorAvp &ea )
{
   if ( ea.exists() )
   {
      copy( ea.getAvp(), getMsg() );
   }

   return *this;
}

FDMessage &FDMessage::add( FDExtractorAvpList &eal )
{
   for ( std::list<FDExtractorAvp*>::iterator it = eal.getList().begin();
         it != eal.getList().end();
         ++it )
   {
      add( *(*it) );
   }

   return *this;
}

FDMessage &FDMessage::addJson( const char *json )
{
   fdJsonAddAvps( json, getMsg(), NULL );

   return *this;
}

bool FDMessage::getJson( std::string &json )
{
   fdJsonGetJSON( getMsg(), json, NULL );
   return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDMessageAnswer::FDMessageAnswer( FDDictionaryEntryCommand *cmd, struct msg *pmsg )
   : FDMessage( cmd, pmsg )
{
}

FDMessageAnswer::FDMessageAnswer( FDMessageRequest *req )
   : FDMessage( true, req->getCommand(), req->getMsg(), false, false )
{
   req->setMsgDelete( false );
}

FDMessageAnswer &FDMessageAnswer::send()
{
   sendAnswer();
   return *this;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDMessageRequest::FDMessageRequest( FDDictionaryEntryCommand *cde )
   : FDMessage( cde )
{
   if ( cde->isAnswer() )
      throw FDException(
         SUtility::string_format("%s:%d - INFO - attempt to create request with answer dictionary entry",
         __FILE__, __LINE__ )
      );
}

FDMessageRequest::FDMessageRequest( FDDictionaryEntryCommand *cde, struct msg *pmsg )
   : FDMessage( cde, pmsg )
{
   if ( cde->isAnswer() )
      throw FDException(
         SUtility::string_format("%s:%d - INFO - attempt to create request with answer dictionary entry",
         __FILE__, __LINE__ )
      );
}

FDMessageRequest::FDMessageRequest( FDDictionaryEntryApplication *ade, FDDictionaryEntryCommand *cde )
   : FDMessage( ade, cde )
{
   if ( cde->isAnswer() )
      throw FDException(
         SUtility::string_format("%s:%d - INFO - attempt to create request with answer dictionary entry",
         __FILE__, __LINE__ )
      );
}

FDMessageRequest::~FDMessageRequest()
{
}

FDMessageRequest &FDMessageRequest::send()
{
   m_timer.Start();
   sendRequest( anscb, *this );
   return *this;
}

void FDMessageRequest::processAnswer( FDMessageAnswer &ans )
{
}

void FDMessageRequest::anscb( void * data, struct msg ** pmsg )
{
   // set the "this" pointer
   FDMessageRequest *pthis = (FDMessageRequest*)data;

   // construct the FDMessageAnswer object
   FDDictionaryEntryCommand anscmd( *pthis->getCommand() );
   FDMessageAnswer ans( &anscmd, *pmsg );

   // process the answer
   pthis->processAnswer( ans );

   // free the request now that the answer has been processed
   delete pthis;

   // clear the pmsg pointer, it will be freed by the FDMessageAnswer destructor
   *pmsg = NULL;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDCommand::FDCommand( FDDictionaryEntryCommand &de )
   : m_de( de )
{
}

FDCommandRequest::FDCommandRequest( FDDictionaryEntryCommand &de )
   : FDCommand( de )
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDApplication::FDApplication( FDDictionaryEntryApplication *de )
   : m_de( de )
{
}

FDApplication::~FDApplication()
{
}

FDApplication &FDApplication::registerHandler( FDCommandRequest &cmd )
{
   for ( std::list<FDCommandRequest*>::const_iterator it = m_cmds.begin();
         it != m_cmds.end();
         ++it )
   {
      if ( (*it)->getDictionaryEntry().getCommandCode() == cmd.getDictionaryEntry().getCommandCode() )
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - a matching command object is already registered",
            __FILE__, __LINE__ )
         );
   }

   struct disp_when when = {
      .app     = m_de->getEntry(),
      .command = cmd.getDictionaryEntry().getEntry(),
      .avp     = NULL,
      .value   = NULL
   };

   int ret = fd_disp_register( commandcb, DISP_HOW_CC, &when, &cmd, NULL );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - ERROR - fd_disp_register failed ret=%d",
         __FILE__, __LINE__, ret )
      );
   
   m_cmds.push_back( &cmd );

   return *this;
}

int FDApplication::commandcb( struct msg **m, struct avp *avp, struct session *session, void *data, enum disp_action *action )
{
   FDCommandRequest *cmd = (FDCommandRequest*)data;
   struct msg_hdr *hdr = NULL;

   int ret = fd_msg_hdr( *m, &hdr );
   if ( ret != 0 )
   {
      // unable to retrieve message header
      return EINVAL;
   }

   if ( FDISANSWER(hdr) )
   {
      // the answer message should have been processed in the fd_msg_send callback
      return EINVAL;
   }

   if ( cmd->isAnswer() )
   {
      // this callback should only handle incoming requests
      // incoming answers should be handled in the fd_msg_send callback
      return EINVAL;
   }

   FDMessageRequest *req = new FDMessageRequest( &cmd->getDictionaryEntry(), *m );

   ret = cmd->process( req );

   // the message will be free'ed by either the request object or the answer
   // object (if the an answer was constructed from the request)
   *m = NULL;

   return ret;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDSession::FDSession()
   : m_session( NULL )
{
#if 0
   int ret;

   ret = fd_sess_new( &m_session, fd_g_config->cnf_diamid, 0, NULL, 0 );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - EXCEPTION - Error creating session ret=%d",
         __FILE__, __LINE__, ret )
      );

   char *sid;
   ret = fd_sess_getsid( m_session, (os0_t *)&sid, NULL );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - EXCEPTION - Error retrieving sid for session ret=%d",
         __FILE__, __LINE__, ret )
      );

   m_sid = sid;
#endif
}

FDSession::~FDSession()
{
   if ( m_session )
   {
      fd_sess_destroy( &m_session );
      //if ( ret != 0 )
      //   throw FDException(
      //      SUtility::string_format("%s:%d - EXCEPTION - Error destroying session ret=%d",
      //      __FILE__, __LINE__, ret )
   }
}

void FDSession::addSessionId( FDMessage &msg, FDDictionaryEntryAVP &deSessionId )
{
   if ( !m_session )
      init();

   msg.add( deSessionId, m_sid );

   int ret = fd_msg_sess_set( msg.getMsg(), m_session );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - EXCEPTION - fd_msg_sess_set failed ret=%d",
         __FILE__, __LINE__, ret )
      );
}

const std::string &FDSession::getSessionId()
{
   if ( !m_session )
      init();
   return m_sid;
}

void FDSession::init()
{
   if ( m_session )
      return;

   int ret;

   ret = fd_sess_new( &m_session, fd_g_config->cnf_diamid, 0, NULL, 0 );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - EXCEPTION - Error creating session ret=%d",
         __FILE__, __LINE__, ret )
      );

   char *sid;
   ret = fd_sess_getsid( m_session, (os0_t *)&sid, NULL );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - EXCEPTION - Error retrieving sid for session ret=%d",
         __FILE__, __LINE__, ret )
      );

   m_sid = sid;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDPeer::FDPeer()
{
   init();
}

FDPeer::FDPeer( DiamId_t diamid, uint16_t port )
{
   init();
   setDiameterId( diamid );
   setPort( port );
   add();
}

FDPeer::FDPeer( const std::string &diamid, uint16_t port )
{
   init();
   setDiameterId( (DiamId_t)diamid.c_str() );
   setPort( port );
   add();
}

FDPeerState FDPeer::getState()
{
   FDPeerState ps = PSInvalid;
   int ret;
   
   if ( !m_peer )
   {
      ret = fd_peer_getbyid( (DiamId_t)m_diamid.c_str(), m_diamid.size(), 0, &m_peer );
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - Error retrieving peer [%s] ret=%d",
            __FILE__, __LINE__, m_diamid.c_str(), ret )
         );
   }

   ret = fd_peer_get_state( m_peer );
   switch ( ret )
   {
      case STATE_NEW:             { ps = PSNew; break; }
      case STATE_OPEN:            { ps = PSOpen; break; }
      case STATE_CLOSED:          { ps = PSClosed; break; }
      case STATE_CLOSING:         { ps = PSClosing; break; }
      case STATE_WAITCNXACK:      { ps = PSWaitCnxAck; break; }
      case STATE_WAITCNXACK_ELEC: { ps = PSWaitCnxAckElec; break; }
      case STATE_WAITCEA:         { ps = PSWaitCEA; break; }
      case STATE_OPEN_HANDSHAKE:  { ps = PSOpenHandshake; break; }
      case STATE_SUSPECT:         { ps = PSSuspect; break; }
      case STATE_REOPEN:          { ps = PSReOpen; break; }
      case STATE_OPEN_NEW:        { ps = PSOpenNew; break; }
      case STATE_CLOSING_GRACE:   { ps = PSClosingGrace; break; }
      case STATE_ZOMBIE:          { ps = PSZombie; break; }
   }

   return ps;
}

void FDPeer::add()
{
   if ( m_diamid.empty() )
      throw FDException(
         SUtility::string_format("%s:%d - Diameter ID not set",
         __FILE__, __LINE__ )
      );

   struct peer_info pi;
   memset( &pi, 0, sizeof(pi) );

   pi.pi_diamid = (DiamId_t)m_diamid.c_str();
   pi.pi_diamidlen = m_diamid.size();
   pi.config.pic_port = m_port;

   pi.config.pic_flags.pro3 = PI_P3_IP;
   pi.config.pic_flags.pro4 = PI_P4_TCP;
   pi.config.pic_flags.alg = PI_ALGPREF_TCP;
   pi.config.pic_flags.sec = PI_SEC_NONE;
   pi.config.pic_flags.exp = PI_EXP_NONE;
   pi.config.pic_flags.persist = PI_PRST_ALWAYS;

   int ret;
   struct addrinfo *ai = NULL;

   if ( !m_destip.empty() )
   {
      struct addrinfo hints;
      int disc = 0;

      memset( &hints, 0, sizeof( hints ) );
      hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST;
      ret = getaddrinfo( m_destip.c_str(), NULL, &hints, &ai );
      if ( ret == EAI_NONAME )
      {
         disc = EP_FL_DISC;
         hints.ai_flags &= ~ AI_NUMERICHOST;
         ret = getaddrinfo( m_destip.c_str(), NULL, &hints, &ai );
      }

      if ( ret )
      {
         freeaddrinfo( ai );
         throw FDException(
            SUtility::string_format("%s:%d - Error [%d] retrieving address info - %s",
            __FILE__, __LINE__, ret, gai_strerror(ret) )
         );
      }

      ret = fd_ep_add_merge( &pi.pi_endpoints, ai->ai_addr, ai->ai_addrlen, EP_FL_CONF | (disc ? EP_ACCEPTALL : 0) );
      if ( ret != 0 )
      {
         freeaddrinfo( ai );
         throw FDException(
            SUtility::string_format("%s:%d - Error adding endpoint [%s] ret=%d",
            __FILE__, __LINE__, m_destip.c_str(), ret )
         );
      }
   }

   ret = fd_peer_add( &pi, "addPeer", peercb, this );
   if ( ret != 0 )
   {
      if ( ret != EEXIST )
         throw FDException(
            SUtility::string_format("%s:%d - Error adding diameter peer [%s] ret=%d",
            __FILE__, __LINE__, m_diamid.c_str(), ret )
         );

      freeaddrinfo( ai );
   }

   ret = fd_peer_getbyid( (DiamId_t)m_diamid.c_str(), m_diamid.size(), 0, &m_peer );
   if ( ret != 0 )
      throw FDException(
         SUtility::string_format("%s:%d - Error retrieving peer [%s] ret=%d",
         __FILE__, __LINE__, m_diamid.c_str(), ret )
      );
}

void FDPeer::init()
{
   m_port = 3868;
   m_peer = NULL;
}

void FDPeer::peercb( struct peer_info *pi, void *data )
{
   FDPeer *ths = (FDPeer*)data;
//std::cout << "**** peercb [" << ths->getDiameterId() << "] state=" << ths->getState() << std::endl;

   if ( ths->getState() == PSOpen )
   {
      ths->m_destrealm = ths->m_peer->info.runtime.pir_realm;
   }
}

FDPeerList::FDPeerList()
{
}

FDPeerList::~FDPeerList()
{
   while ( !empty() )
   {
      FDPeer *peer = *begin();
      pop_front();
      delete peer;
   }
}

bool FDPeerList::isPeerOpen()
{
   for ( FDPeerList::const_iterator it = begin();
         it != end();
         ++it )
   {
      if ( (*it)->isOpen() )
         return true;
   }

   return false;
}

FDPeer *FDPeerList::getOpenPeer()
{
   for ( FDPeerList::iterator it = begin();
         it != end();
         ++it )
   {
      if ( (*it)->isOpen() )
         return *it;
   }

   return NULL;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

FDExtractor::FDExtractor()
   : FDExtractorBase( NULL ),
     m_parent( NULL ),
     m_reference( NULL ),
     m_index( 1 )
{
}

FDExtractor::FDExtractor( FDMessage &msg )
   : FDExtractorBase( NULL ),
     m_parent( NULL ),
     m_reference( msg.getMsg() ),
     m_index( 1 )
{
}

FDExtractor::FDExtractor( FDExtractor &parent, FDDictionaryEntryAVP &de )
   : FDExtractorBase( &de ),
     m_parent( &parent ),
     m_reference( NULL ),
     m_index( 1 )
{
}

FDExtractor::~FDExtractor()
{
}

void FDExtractor::add( FDExtractorBase &base )
{
   FDExtractorKey k( base.getDictionaryEntry()->getVendorId(), base.getDictionaryEntry()->getAvpCode() );
   m_entries[k] = &base;
}

bool FDExtractor::exists( bool skipResolve )
{
   if ( !getResolved() && !skipResolve )
      resolve();

   return FDExtractorBase::exists();
}

msg_or_avp *FDExtractor::getReference()
{
   if ( !getResolved() )
      resolve();

   return m_reference;
}

void FDExtractor::resolve()
{
   if ( getResolved() )
      return;

   // if the reference is not set, then this means that the grouped AVP
   // that this extractor refers to has not been resolved.  We need to 
   // locate the grouped AVP in the parent AVP collection (msg or avp).
   if ( !m_reference )
   {
      if ( m_parent )
      {
         if ( !getDictionaryEntry() )
            throw FDException(
               SUtility::string_format("%s:%d - ERROR - FDExtractor dictionary entry not defined for grouped AVP",
               __FILE__, __LINE__ )
            );

         m_parent->resolve();

         if ( !exists( true ) )
            return;
      }
      else
      {
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - FDExtractor reference and parent are not set",
            __FILE__, __LINE__ )
         );
      }
   }

   bool needToResolve = !getResolved();

   setResolved();

   // check to see if the parent AVP (reference) exists
   //if ( m_reference && !getResolved() )
   if ( m_reference && needToResolve )
   {
      msg_or_avp *loopavp;

      // get the first child AVP
      int ret = fd_msg_browse_internal( m_reference, MSG_BRW_FIRST_CHILD, (msg_or_avp**)&loopavp, NULL);
      if ( ret != 0 )
         throw FDException(
            SUtility::string_format("%s:%d - ERROR - FDExtractor browse returned %d direction MSG_BRW_FIRST_CHILD",
            __FILE__, __LINE__, ret )
         );

      struct avp_hdr *ah;
      FDExtractorKey key;
      std::map<FDExtractorKey,FDExtractorBase*>::iterator it;

      while ( loopavp )
      {
         // get a pointer to the avp header to access the vendor id and avp code
         ret = fd_msg_avp_hdr( (struct avp *)loopavp, &ah );
         if ( ret != 0 )
            throw FDException(
               SUtility::string_format("%s:%d - ERROR - FDExtractor fd_msg_avp_hdr returned %d",
               __FILE__, __LINE__, ret )
            );
   
         // set the vendor id and avp code in the key
         key.setVendor( ah->avp_vendor );
         key.setAvpCode( ah->avp_code );
      
         // lookup up the entry
         if ( (it = m_entries.find(key)) != m_entries.end() )
         {

            switch ( it->second->getExtractorType() )
            {
               case etAvp:
               {
                  FDExtractorAvp *a = (FDExtractorAvp*)it->second;
                  a->setIndex( m_index++ );
                  a->setResolved();
                  a->setAvp( (struct avp *)loopavp );
                  break;
               }
               case etAvpList:
               {
                  FDExtractorAvpList *al = (FDExtractorAvpList*)it->second;
                  FDExtractorAvp *a = new FDExtractorAvp( *this, *al->getDictionaryEntry() );
                  a->setIndex( m_index++ );
                  a->setResolved();
                  a->setAvp( (struct avp *)loopavp );
                  al->getList().push_back( a );
                  al->setResolved();
                  break;
               }
               case etExtractor:
               {
                  FDExtractor *e = (FDExtractor*)it->second;
                  e->setIndex( m_index++ );
                  e->setReference( loopavp );
                  break;
               }
               case etExtractorList:
               {
                  FDExtractorList *el = (FDExtractorList*)it->second;
                  FDExtractor *e = el->createExtractor();
                  e->setIndex( m_index++ );
                  e->setReference( loopavp );
                  el->getList().push_back( e );
                  el->setResolved();
                  break;
               }
            }
         }

         // get the next AVP
         ret = fd_msg_browse_internal( loopavp, MSG_BRW_NEXT, (msg_or_avp**)&loopavp, NULL );
         if ( ret != 0 )
            throw FDException(
               SUtility::string_format("%s:%d - ERROR - FDExtractor browse returned %d direction MSG_BRW_NEXT",
               __FILE__, __LINE__, ret )
            );
      }
   }

//   setResolved();
}

void FDExtractor::dump()
{
   if ( !getResolved() )
      resolve();

   if ( !m_reference )
      return;

   struct dictionary *dict;
   char * buf = NULL;
   size_t len = 0;
   int ret;

   ret = fd_dict_getdict( getDictionaryEntry()->getEntry(), &dict );
   if ( ret != 0 )
      dict = fd_g_config->cnf_dict;

   fd_msg_dump_treeview(&buf, &len, NULL, m_reference, dict, 0, 1);

   std::cout << buf << std::endl;

   free(buf);
}

bool FDExtractor::getJson( std::string &json )
{
   if ( exists() )
   {
      fdJsonGetJSON( getReference(), json, NULL );
      return true;
   }

   return false;
}

////////////////////////////////////////////////////////////////////////////////

FDExtractorList::FDExtractorList( FDExtractor &parent, FDDictionaryEntryAVP &de )
   : FDExtractorBase( &de ),
     m_parent( &parent )
{
}

FDExtractorList::~FDExtractorList()
{
   std::list<FDExtractor*>::iterator it;

   while ( (it = m_list.begin()) != m_list.end() )
   {
      delete *it;
      m_list.pop_front();
   }
}

void FDExtractorList::addExtractor( FDExtractor *e )
{
   m_list.push_back( e );
}

bool FDExtractorList::exists()
{
   return !getList().empty();
}

std::list<FDExtractor*> &FDExtractorList::getList()
{
   if ( !getResolved() && m_parent )
      m_parent->resolve();

   return m_list;
}

////////////////////////////////////////////////////////////////////////////////

FDExtractorAvp::FDExtractorAvp( FDExtractor &extractor, FDDictionaryEntryAVP &de, bool dedel )
   : FDExtractorBase( &de ),
     m_extractor( extractor ),
     m_avp( de, NULL, dedel )
{
}

FDExtractorAvp::~FDExtractorAvp()
{
}

bool FDExtractorAvp::exists()
{
   if ( !getResolved() )
      m_extractor.resolve();

   return FDExtractorBase::exists();
}

void FDExtractorAvp::dump()
{
   if ( m_extractor.getResolved() )
      m_extractor.resolve();

   m_avp.dump();
}

bool FDExtractorAvp::getJson( std::string &json )
{
   if ( exists() )
      return m_avp.getJson( json );

   return false;
}

////////////////////////////////////////////////////////////////////////////////

FDExtractorAvpList::FDExtractorAvpList( FDExtractor &extractor, FDDictionaryEntryAVP &de )
   : FDExtractorBase( &de ),
     m_parent( &extractor )
{
}

FDExtractorAvpList::~FDExtractorAvpList()
{
   std::list<FDExtractorAvp*>::iterator it;

   while ( (it = m_list.begin()) != m_list.end() )
   {
      delete *it;
      m_list.pop_front();
   }
}

bool FDExtractorAvpList::exists()
{
   return !getList().empty();
}

std::list<FDExtractorAvp*> &FDExtractorAvpList::getList()
{
   if ( !getResolved() && m_parent )
      m_parent->resolve();

   return m_list;
}

void FDExtractorAvpList::dump()
{
   if ( !m_parent )
      return;

   if ( !m_parent->getResolved() )
      m_parent->resolve();

   for ( std::list<FDExtractorAvp*>::iterator a = getList().begin(); a != getList().end(); ++a )
      (*a)->dump();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void FDUtility::splitDiameterFQDN( std::string &fqdn, std::string &host, std::string &realm )
{
   size_t pos = fqdn.find( '.' );
   if ( pos != std::string::npos )
   {
      host = fqdn.substr( 0, pos );
      realm = fqdn.substr( pos + 1, std::string::npos );
   }
   else
   {
      host.clear();
      realm.clear();
   }
}

#define AQFD_CHAR2TBCD(c) \
( \
   c >= '0' && c <= '9' ? c - '0' : \
   c == '*' ? 10 : \
   c == '#' ? 11 : \
   (c == 'a' || c == 'A') ? 12 : \
   (c == 'b' || c == 'B') ? 13 : \
   (c == 'c' || c == 'C') ? 14 : 15 \
)

size_t FDUtility::str2tbcd( const char *src, size_t srclen, uint8_t *dst, size_t dstlen )
{
   size_t len, idx;

   for ( len=0, idx=0; len < srclen; len++ )
   {
      if ( len & 1 )
         dst[idx++] |= AQFD_CHAR2TBCD( src[ len ] );
      else
         dst[idx] = AQFD_CHAR2TBCD( src[ len ]) << 4;
   }

   // in case of an odd number of characters, pad with 0x0f
   if ( len & 1 )
      dst[idx++] |= AQFD_CHAR2TBCD( 0 );

   return idx;
}

size_t FDUtility::str2tbcd( const char *src, uint8_t *dst, size_t dstlen )
{
   size_t srclen = strlen( src );
   return str2tbcd( src, srclen, dst, dstlen );
}

size_t FDUtility::str2tbcd( const std::string &src, uint8_t *dst, size_t dstlen )
{
   return str2tbcd( src.c_str(), src.size(), dst, dstlen );
}

#if 0
size_t FDUtility::str2tbcd( const char *src, uint8_t *dst, size_t dstlen )
{
   size_t len;

   for (len = 0; src[len << 1]; len++)
   {
      if ( len > dstlen )
         return 0;

      dst[len] = (
            AQFD_CHAR2TBCD( src[ len << 1]) << 4 ) |
            AQFD_CHAR2TBCD( src[ (len << 1) + 1]
         );

      if ( !src[ (len << 1) + 1 ] )
         break;
   }

   return len;
}
#endif

#define AQFD_LOW_NIBBLE(b) (b & 0x0f)
#define AQFD_HIGH_NIBBLE(b) (AQFD_LOW_NIBBLE(b >> 4))

size_t FDUtility::tbcd2str( uint8_t *src, size_t srclen, char *dst, size_t dstlen )
{
   static const uint8_t *__TBCD__ = (uint8_t*)"0123456789*#abc";

   size_t srcidx;
   size_t len = 0;

   for ( srcidx = 0; srcidx < srclen; srcidx++ )
   {
      if ( len > dstlen )
         return 0;

      dst[ len ] = __TBCD__[ AQFD_HIGH_NIBBLE( src[srcidx] ) ];

      if ( dst[ len ] )
      {
         len++;

         if ( len > dstlen )
            return 0;

         dst[ len ] = __TBCD__[ AQFD_LOW_NIBBLE( src[srcidx] ) ];
         if ( dst[ len ] )
            len++;
      }
   }

   dst[ len ] = 0;

   return len;
}

size_t FDUtility::tbcd2str( uint8_t *src, size_t srclen, std::string &dst )
{
   char buffer[ srclen * 2 ];
   size_t len = tbcd2str( src, srclen, buffer, srclen * 2 );

   dst.assign( buffer, len );

   return len;
}
