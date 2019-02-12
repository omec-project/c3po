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
