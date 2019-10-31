// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#ifndef LIBWFUT_CHANNELFILELIST_H
#define LIBWFUT_CHANNELFILELIST_H 1

#include <string>

#include <libwfut/types.h>

namespace WFUT {
/**
 * The ChannelFileList class represents a WFUT channel. It maintainns a list
 * of FileObjects representing a local or remote directory of files.
 */ 
class ChannelFileList {
public:
  ChannelFileList() = default;
  virtual ~ChannelFileList() = default;

  /**
   * Returns the name of the channel.
   */
  std::string getName() const { return m_name; }

  /**
   * Set the name of the channel.
   */
  void setName(const std::string &name) { m_name = name; }

  /**
   * Get the FileMap object storing all of the FileObjects for this channel. 
   */

  const FileMap &getFiles() const { return m_fileMap; }

  /**
   * Adds a FileObject to this channel.
   */
  void addFile(const FileObject &fo) { m_fileMap[fo.filename] = fo ; }

  /**
   * Removes a FileObject from this channel by it's filename.
   */ 
  void removeFile(const std::string &filename) {
    auto I = m_fileMap.find(filename);
    if (I != m_fileMap.end()) m_fileMap.erase(I);
  }

  /**
   * Clear all the files out of the channel.
   */ 
  void clear() { m_fileMap.clear(); }

  /**
   * Returns the number of files in the channel.
   */
  size_t size() const { return m_fileMap.size(); }

private:
  std::string m_name;
  std::map<std::string, FileObject> m_fileMap;

};

} /* namespace WFUT */

#endif /* LIBWFUT_CHANNELLIST_H */
