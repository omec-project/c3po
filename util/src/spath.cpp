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


#include <string.h>

#include "spath.h"
#include "serror.h"
#include "sdir.h"
#include "sutility.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::string SPath::changeExtension( const char *path, const char *extension )
{
   std::string newPath;

   if (!path || !*path)
   {
      newPath = "";
      return newPath;
   }

   if (SUtility::indexOfAny(path, getInvalidPathChars()) != -1)
      SError::throwRuntimeException( "Illegal characters in path." );

   int iExt = findExtension(path);

   if (!extension || !*extension)
   {
      if (iExt < 0)
         newPath = path;
      else
         newPath.assign(path, &path[iExt]);
   }
   else if (iExt < 0)
   {
      newPath.assign(path, &path[iExt]);
      if (*extension != '.')
         newPath += ".";
      newPath += extension;
   }
   else if (iExt > 0)
   {
      newPath.assign(path, &path[iExt + 1]);
      newPath += (*extension == '.') ? &extension[1] : extension;
   }
   else
   {
      newPath = extension;
   }

   return newPath;
}

std::string SPath::combine( const char *path1, const char *path2 )
{
   std::string path;

   if (!path1)
      SError::throwRuntimeException( "Argument Exception - path1" );
   else if (!path2)
      SError::throwRuntimeException( "Argument Exception - path2" );
   if (*path1 == '\0')
      path = path2;
   else if (*path2 == '\0')
      path = path1;
   else if (SUtility::indexOfAny(path1, getInvalidPathChars()) != -1)
      SError::throwRuntimeException( "Argument Exception - Illegal characters in path1" );
   else if (SUtility::indexOfAny(path2, getInvalidPathChars()) != -1)
      SError::throwRuntimeException( "Argument Exception - Illegal characters in path2" );

   if (isPathRooted(path2))
   {
      path = path2;
      return path;
   }

   char p1end = path1[strlen(path1) - 1];
   path = path1;
   if (p1end != getDirectorySeparatorChar() && p1end != getAltDirectorySeparatorChar() && p1end != getVolumeSeparatorChar())
      path.append(getDirectorySeparatorString());
   path.append(path2);

   return path;
}

std::string SPath::getDirectoryName( const char *path )
{
   std::string dirName;

   if (!path || !*path)
      SError::throwRuntimeException("Argument Exception - Invalid path");

   if (SUtility::indexOfAny(path, getInvalidPathChars()) != -1)
      SError::throwRuntimeException("Argument Exception - path contains invalid characters");

   int nLast = SUtility::lastIndexOfAny(path, getPathSeparatorChars());
   if (nLast == 0)
      nLast++;

   if (nLast > 0)
   {
      dirName.assign(path, nLast);

      cleanPath(dirName.c_str(), dirName);
   }
   else
   {
      dirName = "";
   }

   return dirName;
}

std::string SPath::getExtension( const char *path )
{
   std::string ext;

   if (!path || !*path)
   {
      ext = "";
   }
   else
   {
      if (SUtility::indexOfAny(path, getInvalidPathChars()) != -1)
         SError::throwRuntimeException( "Argument Exception - Illegal characters in path" );

      int iExt = findExtension(path);

      if (iExt > -1)
      {
         if (iExt < ((int)strlen(path)) - 1)
            ext = &path[iExt];
         else
            ext = "";
      }
      else
      {
         ext = "";
      }
   }

   return ext;
}

std::string SPath::getFileName( const char *path )
{
   std::string fileName;

   if (!path || !*path)
   {
      fileName = "";
      return fileName;;
   }

   if (SUtility::indexOfAny(path, getInvalidPathChars()) != -1)
         SError::throwRuntimeException( "Argument Exception - Illegal characters in path" );

   int nLast = SUtility::lastIndexOfAny(path, getPathSeparatorChars());
   if (nLast >= 0)
      fileName = &path[nLast + 1];
   else
      fileName = path;

   return fileName;
}

std::string SPath::getFileNameWithoutExtension( const char *path )
{
   std::string fileName = getFileName( path );
   return changeExtension( fileName.c_str(), NULL );
}

std::string SPath::getPathRoot( const char *path )
{
   std::string root;

   if (!*path || !*path)
      SError::throwRuntimeException( "Argument Exception - the specified path is not of a legal form.");

   if (isPathRooted(path))
   {
      root = "";
   }
   else
   {
      root = isDsc(*path) ? getDirectorySeparatorString() : "";
   }

   return root;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void SPath::cleanPath(const char *path, std::string &dirName)
{
   int l = (int)strlen(path);
   int sub = 0;
   int start = 0;

   char p0 = path[0];
   if (l > 2 && p0 == '\\' && path[1] == '\\')
      start = 2;

   if (l == 1 && (p0 == getDirectorySeparatorChar() || p0 == getAltDirectorySeparatorChar()))
   {
      dirName = path;
   }
   else
   {
      for (int i=start; i < l; i++)
      {
         char c = path[i];
         if (c != getDirectorySeparatorChar() && c != getAltDirectorySeparatorChar())
            continue;
         if (i+1 == l)
            sub++;
         else
         {
            c = path[i+1];
            if (c == getDirectorySeparatorChar() || c == getAltDirectorySeparatorChar())
               sub++;
         }
      }

      if (!sub)
      {
         dirName = path;
      }
      else
      {
         char copy[FILENAME_MAX];
         int len = l - sub;
         if (start != 0)
         {
            copy[0] = '\\';
            copy[1] = '\\';
         }

         for (int i = start, j = start; i < l && j < len; i++)
         {
            char c = path[i];

            if (c != getDirectorySeparatorChar() && c != getAltDirectorySeparatorChar())
            {
               copy[j++] = c;
               continue;
            }

            // for non-trailing cases
            if (j+1 != len)
            {
               copy [j++] = getDirectorySeparatorChar();
               for (; i< l-i; i++)
               {
                  c = path[i+1];
                  if (c != getDirectorySeparatorChar() && c != getAltDirectorySeparatorChar())
                     break;
               }
            }
         }

         copy[len] = '\0';
         dirName = copy;
      }
   }
}

bool SPath::isPathRooted(const char *path)
{
   if (!path || !*path)
      return false;

   if (SUtility::indexOfAny(path, getInvalidPathChars()) != -1)
      SError::throwRuntimeException( "Argument Exception - Illegal characters in path" );

   int len = (int)strlen(path);
   char c = *path;
   return (c == getDirectorySeparatorChar() ||
         c == getAltDirectorySeparatorChar() ||
         (getDirectorySeparatorChar() == getVolumeSeparatorChar() && len > 1 && path[1] == getVolumeSeparatorChar()));
}

int SPath::findExtension(const char *path) 
{ 
   // method should return the index of the path extension 
   // start or -1 if no valid extension 
   int iLastDot = SUtility::lastIndexOfAny(path, ".");
   int iLastSep = SUtility::lastIndexOfAny(path, getPathSeparatorChars());

   if (iLastDot > iLastSep)
      return iLastDot;

   return -1;
} 
