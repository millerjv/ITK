/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    genGeneratorBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#include "genGeneratorBase.h"

#include <sys/stat.h>
#include <errno.h>

#if defined(_MSC_VER) || defined(__BORLANDC__)
#include <windows.h>
#include <direct.h>
#define _unlink unlink
inline int Mkdir(const char* dir)
{
  return _mkdir(dir);
}
#else
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
inline int Mkdir(const char* dir)
{
  return mkdir(dir, 00777);
}
#endif


namespace gen
{


/**
 * Print indentation spaces.
 */
void
Indent
::Print(std::ostream& os) const
{
  if(m_Indent <= 0)
    { return; }
  
  // Use blocks of 8 spaces to speed up big indents.
  unsigned int blockCount = m_Indent >> 3;
  unsigned int singleCount = m_Indent & 7;
  while(blockCount-- > 0)
    {
    os << "        ";
    }
  while(singleCount-- > 0)
    {
    os << " ";
    }
}


/**
 * Simplify indentation printing by allowing Indent objects to be added
 * to streams.
 */
std::ostream& operator<<(std::ostream& os, const Indent& indent)
{  
  indent.Print(os);
  return os;
}


/**
 * Simplify the printing of strings.
 */
std::ostream& operator<<(std::ostream& os, const String& str)
{
  os << str.c_str();
  return os;
}


/**
 * Make sure the given path exists, creating it if necessary.
 * Returns false only on error.
 */
bool GeneratorBase::MakeDirectory(const char* path)
{
  std::string dir = path;
  // replace all of the \ with /
  size_t pos = 0;
  while((pos = dir.find('\\', pos)) != std::string::npos)
    {
    dir[pos] = '/';
    pos++;
    }
  pos =  dir.find(':');
  if(pos == std::string::npos)
    {
    pos = 0;
    }
  while((pos = dir.find('/', pos)) != std::string::npos)
    {
    std::string topdir = dir.substr(0, pos);
    Mkdir(topdir.c_str());
    pos++;
    }
  if(Mkdir(path) != 0)
    {
    // if it is some other error besides directory exists
    // then return false
    if(errno != EEXIST)
      {
      return false;
      }
    }
  return true;
}

} // namespace gen
