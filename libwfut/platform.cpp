// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2007 Simon Goodall


#include <cassert>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

//get MSVC header here: http://www.softagalleria.net/download/dirent/dirent-1.11.zip
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <map>
#include <string>
#include <list>
#include <algorithm>

#include "libwfut/platform.h"


#ifdef _WIN32
#include <direct.h>
#define mkdir(path,mode) _mkdir(path)
#define stat _stat
#endif

namespace WFUT {

FILE *os_create_tmpfile() {
#ifdef _MSC_VER
  char path[MAX_PATH];
  int ret = GetTempPathA(MAX_PATH, path);
  if(ret > MAX_PATH || (ret == 0)){
    strcpy(path, ".\\");
  }
  char filename[MAX_PATH];
  //only 3 prefix characters allowed
  ret = GetTempFileNameA(path,"wfu",0,filename);
  if(ret == 0){
    sprintf(filename,"%swfut%d.tmp",path,rand());
  }
  //There is a special 'D' sign, which means temporary and delete on close 
  //http://msdn.microsoft.com/en-us/library/yeby3zcb%28v=vs.71%29.aspx
  return fopen(filename, "w+bD");
#else
  return tmpfile();
#endif
}

void os_free_tmpfile(FILE *fp) {
  assert(fp != 0);
  // tmpfile cleans up after itself
  fclose(fp);
}

int os_mkdir(const std::string &dir) {
  return mkdir(dir.c_str(), 0700);
}

bool os_exists(const std::string &file) {
  struct stat info;
  if (::stat(file.c_str(), &info) == 0) {
    return true;
  } else {
    return false;
  }
}

int os_set_executable(const std::string &file) {
#ifdef _WIN32
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
  DIR *d = opendir(path.c_str());
  if (d != 0) {
    struct dirent *dent = readdir(d);
    while (dent != 0) {
      const std::string d_name(dent->d_name);
      if (d_name != "." && d_name != ".." && std::find(excludes.begin(), excludes.end(), d_name) == excludes.end()) {
      //NOTE: dent->d_type member is not existing in mingw32, so we need to use stat.
      struct stat filestat;
      stat(d_name.c_str(), &filestat);
      if (S_ISDIR(filestat.st_mode)) {
        const std::string &pathname = path + "/" + d_name;
        os_dir_walk(pathname, excludes, files);
      } else if (S_ISREG(filestat.st_mode)) { 
        const std::string &filename = path + "/" + d_name;
        files.push_back(filename);
      }}
      dent = readdir(d);
    }
  }

  return 0;
}


} /* namespace WFUT */