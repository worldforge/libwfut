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
#include <errno.h>
#include <stdio.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <map>
#include <string>
#include <list>
#include <algorithm>

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
 struct stat info;
  if (::stat(file.c_str(), &info) == 0) {

    mode_t mode = info.st_mode;
    mode |= S_IXGRP | S_IXOTH | S_IXUSR;

    // TODO: Should we restrict these permissions some more?
    //        E.g., only user?
    return chmod(file.c_str(), mode);
  }
  return 0;
#endif
}

int os_dir_walk(const std::string &path, const std::list<std::string> &excludes, std::list<std::string> &files) {

#ifdef PLATFORM_WIN32
  // No win32 support yet
  
  return 1;
#else
  DIR *d = opendir(path.c_str());
  if (d != 0) {
    struct dirent *dent = readdir(d);
    while (dent != 0) {
      const std::string d_name(dent->d_name);
      if (d_name != "." && d_name != ".." && std::find(excludes.begin(), excludes.end(), d_name) == excludes.end()) {

      if (dent->d_type == DT_DIR) {
        const std::string &pathname = path + "/" + d_name;
        os_dir_walk(pathname, excludes, files);
      } else if (dent->d_type == DT_REG) { 
        const std::string &filename = path + "/" + d_name;
        files.push_back(filename);
      }}
      dent = readdir(d);
    }
  }

  return 0;
#endif
}


} /* namespace WFUT */
