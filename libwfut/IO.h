// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#ifndef LIBWFUT_IO_H
#define LIBWFUT_IO_H 1

#include <cassert>
#include <string>
#include <map>
#include <queue>
#include <cstdio>

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

/**
 * The IO class wraps most of the CURL related calls, taking a URL to a file and
 * writing it to a local file.
 */ 
class IO {
public:
  IO() :
     m_initialised(false),
     m_mhandle(NULL),
     m_num_to_process(1)
  {}

  virtual ~IO() {
    assert(m_initialised == false);
  }

  /**
   * The init method initialises the CURL backend. 
   */
  int init();

  /**
   * The init method cleans up the CURL backend. 
   */
  int shutdown();

  /**
   * Poll network for data chunks to download and write to disk.
   */ 
  int poll();

  /**
   * Tell CURL to immediately download the file in the given URL and save it to
   * the given filename, optionally checking the CRC32 value. This is a blocking
   *  call.
   *  @param filename Destination filename
   *  @param url Remote location.
   *  @param expected_crc32 The expected crc32 value of the file. 0 to ignore.
   */
  int downloadFile(const std::string &filename, const std::string &url, uLong expected_crc32);

  /**
   * Tell CURL to immediately download the file in the given URL and save it 
   * using the given FILE pointer, optionally checking the CRC32 value. This is
   * a blocking  call.
   *  @param fp Open file descriptor to write file data into.
   *  @param url Remote location.
   *  @param expected_crc32 The expected crc32 value of the file. 0 to ignore.
   */
  int downloadFile(FILE *fp, const std::string &url, uLong expected_crc32);

  /**
   * Queue a file to be downloaded using the poll function.
   * @see IO::poll
   * @param path The directory to save the file into.
   * @param filename The filename minus the directory.
   * @param url The full URL of the file
   * @param expected_crc32 The expected CRC32 value of the file. 0 to ignore.
   * @param executable Flag for whether to set the executable flag when ndownloaded.
   */ 
  int queueFile(const std::string &path, const std::string &filename, const std::string &url, uLong expected_crc32, bool executable);

  /**
   * The DownloadComplete signal is fired when a file is successfully downloade.
   */
  sigc::signal<void, const std::string&, const std::string&> DownloadComplete;
  /**
   * The DownloadFailed signal is fired when a file fails to download 
   * successfully.
   */
  sigc::signal<void, const std::string&, const std::string&, const std::string&> DownloadFailed;

  /**
   * Returns the maximum number of simultaneous downloads allowed.
   */
  int getMaxDownloads() const { return m_num_to_process; }
  /**
   * Set the maximum number of simultaneous downloads allowed. Multiple 
   * downloads will cause multiple connections to be opened, even if the same
   * remote server is used.
   */ 
  void setMaxDownloads(int i) { m_num_to_process = i; }

private:
  bool m_initialised;
  CURLM *m_mhandle;
  std::map<std::string, DataStruct*> m_files;
  std::queue<CURL*> m_handles;
  int m_num_to_process;
};

} /* namespace WFUT */

#endif /* LIBEFUT_IO_H */
