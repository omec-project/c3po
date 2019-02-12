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

#ifndef __SPATH_H
#define __SPATH_H

#include <string>

#include "serror.h"

class SPath
{
public:
   static const char *getDirectorySeparatorString();
   static const char getDirectorySeparatorChar();
   static const char getAltDirectorySeparatorChar();
   static const char getVolumeSeparatorChar();

   static const char *getPathSeparatorChars();
   static const char *getInvalidPathChars();
   static const char *getInvalidFileNameChars();

   static std::string changeExtension( const std::string &path, const std::string &extension ) { return changeExtension( path.c_str(), extension.c_str() ); }
   static std::string changeExtension( const char *path, const std::string &extension ) { return changeExtension( path, extension.c_str() ); }
   static std::string changeExtension( const std::string &path, const char *extension ) { return changeExtension( path.c_str(), extension ); }
   static std::string changeExtension( const char *path, const char *extension);

   static std::string combine( const std::string &path1, const std::string &path2 ) { return combine( path1.c_str(), path2.c_str() ); }
   static std::string combine( const std::string &path1, const char *path2 ) { return combine( path1.c_str(), path2 ); }
   static std::string combine( const char *path1, const std::string &path2 ) { return combine( path1, path2.c_str() ); }
   static std::string combine( const char *path1, const char *path2 );

   static std::string getDirectoryName(const char *path);
   static std::string getExtension(const char *path);
   static std::string getFileName(const char *path);
   static std::string getFileNameWithoutExtension(const char *path);
   static std::string getPathRoot(const char *path);

   static std::string getDirectoryName( const std::string &path ) { return getDirectoryName( path.c_str() ); }
   static std::string getExtension( const std::string &path ) { return getExtension( path.c_str() ); }
   static std::string getFileName( const std::string &path ) { return getFileName( path.c_str() ); }
   static std::string getFileNameWithoutExtension( const std::string &path ) { return getFileNameWithoutExtension( path.c_str() ); }
   static std::string getPathRoot( const std::string &path ) { return getPathRoot( path.c_str() ); }

private:
   static bool m_dirEqualsVolume;

   static void cleanPath(const char *path, std::string &cleanPath);
   static void insecureFullPath(const char *path, std::string &fullPath);
   static bool isDsc(const char c);
   static bool isPathRooted(const char *path);
   static int findExtension(const char *path);
};

inline const char *SPath::getDirectorySeparatorString()
{
   return "/";
}

inline const char SPath::getDirectorySeparatorChar()
{
   return '/';
}

inline const char SPath::getAltDirectorySeparatorChar()
{
   return '/';
}

inline const char SPath::getVolumeSeparatorChar()
{
   return ':';
}

inline const char *SPath::getPathSeparatorChars()
{
   return "/:";
}

inline const char *SPath::getInvalidPathChars()
{
   return "";
}

inline const char *SPath::getInvalidFileNameChars()
{
   return "/";
}

inline bool SPath::isDsc(const char c)
{
   return c == getDirectorySeparatorChar() || c == getAltDirectorySeparatorChar();
}

#endif // #define __SPATH_H
