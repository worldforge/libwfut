// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall


#ifndef LIBWFUT_WFUT_H
#define LIBWFUT_WFUT_H 1

#include <string>
#include <cassert>

namespace WFUT {

class IO;

class WFUT {
public:
  WFUT():
    m_initialised(false),
    m_io(NULL)
  {}
  virtual ~WFUT() { assert(m_initialised == false); }

  int init();
  int shutdown();

  void updateChannel(const FileList &updates);
  ChannelList getChannelList(const std::string &url);
  FileList getFileList(const std::string &url);
  FileList getLocalList(const std::string &filename);
  int saveLocalList(const FileList &files, const std::string &filename);

  int poll();

private:
  bool m_initialised;
  IO m_io;
};

} /* namespace WFUT */

#endif /* LIBWFUT_WFUT_H */
