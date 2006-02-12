// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2006 Simon Goodall

#include "WFUT.h"
#include "types.h"
#include "IO.h"
#include "FileIO.h"
#include "ChannelIO.h"

namespace WFUT {


// Place holder function.
std::string getFile(const std::string &url) {
  return "";
}

int WFUT::init() {
  assert (m_initialised == false);

  m_io = new IO();
  if (m_io->init()) {
    delete m_io;
    m_io = NULL;
    return 1;
  }

  m_initialised = true;

  return 0;
}

int WFUT::shutdown() {
  assert (m_initialised == true);

  m_io->shutdown();
  delete m_io;
  m_io = NULL;

  m_initialised = false;

  return 0;
}

void WFUT::updateChannel(const FileList &updates) {
  assert (m_initialised == true);
  FileList::const_iterator I = updates.begin();
  while (I != updates.end()) {
    FileObject f = *I;
    // TODO  prepend root values
    std::string filename = f.filename;
    std::string url = filename;
    m_io->queueFile(filename, url, f.crc32);
  }
}

ChannelList WFUT::getChannelList(const std::string &url) {
  ChannelList channels;
  std::string fname = getFile(url);
  if (parseChannelList(fname, channels)) {
    // Error
  }
  return channels;
}

FileList WFUT::getFileList(const std::string &url) {
  FileList files;
  std::string fname = getFile(url);
  if (parseFileList(fname, files)) {
    // Error
  }
  return files;
}

FileList WFUT::getLocalList(const std::string &filename) {
  FileList files;
  if (parseFileList(filename, files)) {
    // Error
  }
  return files;
}

int WFUT::saveLocalList(const FileList &files, const std::string &filename) {
  if (writeFileList(filename, files)) {
    // Error
  }
  return 0;
}

}
