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

#include <map>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "libwfut/platform.h"

namespace WFUT {

#ifdef PLATFORM_WIN32
// This is a bit of a nasty hack to make sure that when we
// close a tmp file, we also delete it.
typedef std::map<FILE *, char *> TmpFileMap;
static TmpFileMap tmp_file_map;
#endif

FILE *os_create_tmpfile() {
#ifdef PLATFORM_WIN32
  // Check to see if there are temp dirs defined, else use current directory.
  static const char cwd[] = ".";
  const char *tmpdir = getenv("TMP");
  const char *tempdir = getenv("TEMP");
  // Make the choice
  const char *t_dir = (tmpdir != 0) ? (tmpdir) : ( (tempdir != 0) ? (tempdir) : (cwd)); 
  char *filename = tempnam(t_dir, "wfut");
  FILE *fp = fopen(filename, "w+b");
  if (fp != NULL) {
    // Record the filename for the FILE pointer
    tmp_file_map[fp] = filename;
  }
  return fp;
#else
  return tmpfile();
#endif
}

void os_free_tmpfile(FILE *fp) {
  assert(fp != 0);
#ifdef PLATFORM_WIN32
  TmpFileMap::iterator I = tmp_file_map.find(fp);
  assert (I != tmp_file_map.end());
  fclose(I->first);       // Close file handle
  unlink(I->second);      // Delete the tmp file
  free(I->second);        // Free up the mem allocated to the filename
  tmp_file_map.erase(I);  // Get rid of the record.
#else 
 // tmpfile cleans up after itself
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

bool os_exists(const std::string &file) {
//#ifdef PLATORM_WIN32
  
//#else
  struct stat info;
  if (::stat(file.c_str(), &info) == 0) {
    return true;
  } else {
    return false;
  }
//#endif
}

int os_set_executable(const std::string &file) {
#ifdef PLATFORM_WIN32
  // nothing to do for windows
  return 0;
#else
  // TODO: Should we restrict these permissions some more?
  //        E.g., only user?
  return chmod(file.c_str(), S_IXGRP | S_IXOTH | S_IEXEC | S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR);
#endif
}


} /* namespace WFUT */
