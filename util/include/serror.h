/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __SERROR_H
#define __SERROR_H

#include <stdarg.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>

#include <errno.h>

class SError
{
public:
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

   static void throwRuntimeExceptionWithErrno(std::string & msg)
   {
      throwRuntimeExceptionWithErrno(msg.c_str());
   }

   static void throwRuntimeExceptionWithErrno(const char * msg)
   {
      std::ostringstream buf;
      buf << msg << " errno = " << errno;
      throw std::runtime_error(buf.str());
   }

   static void throwRuntimeExceptionWithErrno(std::string & msg, int err)
   {
      throwRuntimeExceptionWithErrno(msg.c_str(), err);
   }

   static void throwRuntimeExceptionWithErrno(const char * msg, int err)
   {
      std::ostringstream buf;
      buf << msg << " error value = " << err;
      throw std::runtime_error(buf.str());
   }

   static void throwRuntimeException(std::string & msg)
   {
      throwRuntimeException(msg.c_str());
   }

   static void throwRuntimeException( const char *format, ... )
   {
      va_list args;

      va_start( args, format );
      size_t size = vsnprintf( NULL, 0, format, args ) + 1; // Extra space for '\0'
      va_end( args );

      Buffer<char> buf( size );

      va_start( args, format );
      vsnprintf( buf.get(), size, format, args  );
      va_end( args );

      throw std::runtime_error( buf.get() );
   }
};

#endif // #define __SERROR_H
