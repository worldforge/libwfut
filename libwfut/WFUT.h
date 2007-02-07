// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#ifndef LIBWFUT_WFUT_H
#define LIBWFUT_WFUT_H 1

#include <string>
#include <cassert>
#include <libwfut/types.h>
#include <libwfut/ChannelFileList.h>

namespace WFUT {

class IO;

class WFUTClient {
public:
  WFUTClient():
    m_initialised(false),
    m_io(NULL)
  {}
  virtual ~WFUTClient() { assert(m_initialised == false); }

  int init();
  int shutdown();

  void updateChannel(const ChannelFileList &updates, 
                     const std::string &urlPrefix,
                     const std::string &pathPrefix);

  ChannelList getChannelList(const std::string &url);

  ChannelFileList getFileList(const std::string &url);

  ChannelFileList getLocalList(const std::string &filename);

  int saveLocalList(const ChannelFileList &files, const std::string &filename);

  int calculateUpdates(const ChannelFileList &server,
                       const ChannelFileList &system,
                       const ChannelFileList &local,
                       ChannelFileList &updates,
                       const std::string &prefix);


  int poll();

private:
  bool m_initialised;
  IO *m_io;
};

} /* namespace WFUT */

#endif /* LIBWFUT_WFUT_H */
