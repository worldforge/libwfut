// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#ifndef LIBWFUT_IO_H
#define LIBWFUT_IO_H 1

#include  <cassert>

#include <string>
#include <map>

#include <stdio.h>

#include <zlib.h>

#include <curl/curl.h>

#include <sigc++/signal.h>

#include <libwfut/types.h>

namespace WFUT {

class IO {
public:
  typedef struct {
    std::string filename;
    std::string url;
    FILE *fp;
    uLong actual_crc32;
    uLong expected_crc32;
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
  int queueFile(const std::string &filename, const std::string &url, uLong expected_crc32);

  SigC::Signal2<void, const std::string&, const std::string&> DownloadComplete;
  SigC::Signal2<void, const std::string&, const std::string&> DownloadFailed;

private:
  bool m_initialised;
  CURLM *m_mhandle;
  std::map<std::string, DataStruct*> m_files;
};

} /* namespace WFUT */

#endif /* LIBEFUT_IO_H */
