// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#ifndef LIBWFUT_IO_H
#define LIBWFUT_IO_H 1

#include  <cassert>

#include <string>
#include <map>

#include <stdio.h>

#include <curl/curl.h>

namespace WFUT {

class IO {
public:
  typedef struct {
    std::string filename;
    std::string url;
    FILE *fp;
    CURL *handle;
  } DataStruct;

  IO() :
     m_initialised(false),
     m_mhandle(NULL)
  {}

  virtual ~IO() {
    assert(m_initialised == false);
  }

  int init();
  int shutdown();
 
  int poll();
  int queueFile(const std::string &filename, const std::string &url);

private:
  bool m_initialised;
  CURLM *m_mhandle;
  std::map<std::string, DataStruct*> m_files;
};

} /* namespace WFUT */

#endif /* LIBEFUT_IO_H */
