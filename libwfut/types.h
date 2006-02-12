// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2006 Simon Goodall

#ifndef LIBWFUT_TYPES_H
#define LIBWFUT_TYPES_H 1

#include <string>
#include <list>

#include <zlib.h>

typedef struct {
  std::string name;
  std::string description;
  std::string url;
  std::string email;
  std::string logo;
} ChannelObject;

typedef std::list<ChannelObject> ChannelList;

typedef struct {
  std::string filename;
  int version;
  uLong crc32;
  long size;
  bool execute;
} FileObject;

typedef std::list<FileObject> FileList;

#endif /* LIBWFUT_TYPES_H */
