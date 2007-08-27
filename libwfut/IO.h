// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#ifndef LIBWFUT_IO_H
#define LIBWFUT_IO_H 1

#include <cassert>
#include <string>
#include <map>
#include <stdio.h>

#include <zlib.h>
#include <curl/curl.h>
#include <sigc++/signal.h>

#include <libwfut/types.h>

namespace WFUT {
  typedef struct {
    std::string filename;
    std::string path;
    std::string url;
    bool executable;
    FILE *fp;
    uLong actual_crc32;
    uLong expected_crc32;
    CURL *handle;
  } DataStruct;


class IO {
public:
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
  int downloadFile(const std::string &filename, const std::string &url, uLong expected_crc32);
  int downloadFile(FILE *fp, const std::string &url, uLong expected_crc32);
  int queueFile(const std::string &path, const std::string &filename, const std::string &url, uLong expected_crc32, bool executable);

  // void yadda(url, filename)
  sigc::signal<void, const std::string&, const std::string&> DownloadComplete;
  // void yadda(url, filename, reason)
  sigc::signal<void, const std::string&, const std::string&, const std::string&> DownloadFailed;

private:
  bool m_initialised;
  CURLM *m_mhandle;
  std::map<std::string, DataStruct*> m_files;
};

} /* namespace WFUT */

#endif /* LIBEFUT_IO_H */
