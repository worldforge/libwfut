// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#ifndef LIBWFUT_CHANNELFILELIST_H
#define LIBWFUT_CHANNELFILELIST_H 1

#include <string>

#include <libwfut/types.h>

namespace WFUT {

class ChannelFileList {
public:
  ChannelFileList() {}
  virtual ~ChannelFileList() {}

  std::string getName() const { return m_name; }
  void setName(const std::string &name) { m_name = name; }

  FileMap getFiles() const { return m_fileMap; }
  void addFile(const FileObject &fo) { m_fileMap[fo.filename] = fo ; }
  void removeFile(const std::string &filename) {
    FileMap::iterator I = m_fileMap.find(filename);
    if (I != m_fileMap.end()) m_fileMap.erase(I);
  }

private:
  std::string m_name;
  std::map<std::string, FileObject> m_fileMap;

};

} /* namespace WFUT */

#endif /* LIBWFUT_CHANNELLIST_H */
