// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2007 Simon Goodall


#if defined (WIN32) || defined (_WIN32) || defined( __WIN32__) 
#define PLATFORM_WIN32
#else
#define PLATFORM_NIX
#endif

#include <cassert>
#include <stdlib.h>
#include <stdio.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "libwfut/platform.h"

namespace WFUT {

FILE *os_create_tmpfile() {
#ifdef PLATFORM_WIN32
  const char *tmpdir = getenv("TMP");
  char *filename = ::tempnam(tmpdir, "wfut");
  FILE *fp = fopen(filename, "w+b");
  free(filename);
  return fp;
#else
  return tmpfile();
#endif
}

void os_free_tmpfile(FILE *fp) {
  assert(fp != 0);
#ifdef PLATFORM_WIN32
  // TODO delete this file
  fclose(fp);
#else 
 // tmpfile cleanup after itself
  fclose(fp);
#endif
}

int os_mkdir(const std::string &dir) {
#ifdef PLATFORM_WIN32
  return mkdir(dir.c_str());
#else
  return mkdir(dir.c_str(), 0700);
#endif
}

} /* namespace WFUT */
