// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2006 Simon Goodall

#include "WFUT.h"
#include "types.h"
#include "IO.h"
#include "FileIO.h"
#include "ChannelIO.h"

namespace WFUT {

void msg(const std::string &u, const std::string &f)  {
  printf("Downloaded %s  -> %s\n", u.c_str(), f.c_str());
}
void msg2(const std::string &u, const std::string &f, const std::string &r)  {
  printf("Failed %s -> %s\n",u.c_str(), r.c_str());
}

int WFUTClient::init() {
  assert (m_initialised == false);

  m_io = new IO();
  if (m_io->init()) {
    delete m_io;
    m_io = NULL;
    return 1;
  }

  m_io->DownloadComplete.connect(sigc::ptr_fun(msg));
  m_io->DownloadFailed.connect(sigc::ptr_fun(msg2));

  m_initialised = true;

  return 0;
}

int WFUTClient::shutdown() {
  assert (m_initialised == true);

  m_io->shutdown();
  delete m_io;
  m_io = NULL;

  m_initialised = false;

  return 0;
}

void WFUTClient::updateChannel(const FileList &updates,
                               const std::string &urlPrefix,
                               const std::string &pathPrefix) {
  assert (m_initialised == true);
  FileList::const_iterator I = updates.begin();
  while (I != updates.end()) {
    FileObject f = *I++;
    // TODO  prepend root values
    std::string filename = pathPrefix + f.filename;
    std::string url = urlPrefix + f.filename;
    m_io->queueFile(filename, url, f.crc32);
  }
}

ChannelList WFUTClient::getChannelList(const std::string &url) {
  assert (m_initialised == true);
  ChannelList channels;
  // TODO: this is currently platform dependant!
  char filename[] = "/tmp/wfut.XXXXXX";
  int fd = mkstemp(filename);

  if (m_io->downloadFile(filename, url, 0)) {
    // error
    fprintf(stderr, "Error downloading channel list\n");
    close(fd);
    unlink(filename);
    return channels;
  }
  if (parseChannelList(filename, channels)) {
    // Error
    fprintf(stderr, "Error parsing channel list\n");
    close(fd);
    unlink(filename);
    return channels;
  }
  close(fd);
  unlink(filename);
  return channels;
}

FileList WFUTClient::getFileList(const std::string &url) {
  assert (m_initialised == true);
  FileList files;
  // TODO: this is currently platform dependant!
  char filename[] = "/tmp/wfut.XXXXXX";
  int fd = mkstemp(filename);
  if (m_io->downloadFile(filename, url, 0)) {
    // error
    fprintf(stderr, "Error downloading file list\n");
    close(fd);
    unlink(filename);
    return files;
  }

  if (parseFileList(filename, files)) {
    // Error
    fprintf(stderr, "Error parsing file list\n");
    close(fd);
    unlink(filename);
  }
  close(fd);
  unlink(filename);
  return files;
}

FileList WFUTClient::getLocalList(const std::string &filename) {
  assert (m_initialised == true);
  FileList files;
  if (parseFileList(filename, files)) {
    // Error
  }
  return files;
}

int WFUTClient::saveLocalList(const FileList &files, const std::string &filename) {
  assert (m_initialised == true);
  if (writeFileList(filename, files)) {
    // Error
  }
  return 0;
}

int WFUTClient::poll() {
  assert (m_initialised == true);
  return m_io->poll();
}

}
