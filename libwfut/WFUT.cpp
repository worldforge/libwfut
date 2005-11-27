// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include "WFUT.h"

namespace WFUT {

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
    File f = *I;
    // TODO  prepend root values
    std::string filename = f.getFilename();
    std::string url = g.getURL();
    m_io->queueFile(filename, url);
  }
}

ChannelList WFUT::getChannelList(const std::string &url) {
  ChannelList channels;
  ChannelParser parser;
  std::string fname = getFile(url);
  parser.parseChannelList(fname, channels);
  return channels;
}

FileList WFUT::getFileList(const std::string &url) {
  FileList files;
  FileParser parser;
  std::string fname = getFile(url);
  parser.parseFileList(fname, files);
  return files;
}

FileList WFUT::getLocalList(const std::string &filename) {
  FileList files;
  FileParser parser;
  parser.parseFileList(filename, files);
  return files;
}

int WFUT::saveLocalList(const FileList &files, const std::string &filename) {
  FileWriter writer;
  write.writeFileList(files, filename);
  return 0;
}

}
