// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#include "libwfut/WFUT.h"
#include "libwfut/types.h"
#include "libwfut/IO.h"
#include "libwfut/FileIO.h"
#include "libwfut/ChannelIO.h"
#include "libwfut/ChannelFileList.h"
#include "libwfut/crc32.h"

namespace WFUT {

void WFUTClient::onDownloadComplete(const std::string &u, const std::string &f)  {
  DownloadComplete.emit(u, f);
}
void WFUTClient::onDownloadFailed(const std::string &u, const std::string &f, const std::string &r)  {
  DownloadFailed.emit(u,f,r);
}

WFUTError WFUTClient::init() {
  assert (m_initialised == false);

  m_io = new IO();
  if (m_io->init()) {
    delete m_io;
    m_io = NULL;
    return WFUT_GENERAL_ERROR;
  }
 
  m_io->DownloadComplete.connect(sigc::mem_fun(this, &WFUTClient::onDownloadComplete));
  m_io->DownloadFailed.connect(sigc::mem_fun(this, &WFUTClient::onDownloadFailed));

  m_initialised = true;

  return WFUT_NO_ERROR;
}

WFUTError WFUTClient::shutdown() {
  assert (m_initialised == true);

  m_io->shutdown();
  delete m_io;
  m_io = NULL;

  m_initialised = false;

  return WFUT_NO_ERROR;
}

void WFUTClient::updateChannel(const ChannelFileList &updates,
                               const std::string &urlPrefix,
                               const std::string &pathPrefix) {
  assert (m_initialised == true);
  const FileMap &files = updates.getFiles();

  FileMap::const_iterator I = files.begin();
  FileMap::const_iterator Iend = files.end();
  while (I != Iend) {
    const FileObject &f = (I++)->second;

    const std::string &url = urlPrefix + updates.getName() + "/" + f.filename;
    m_io->queueFile(pathPrefix, f.filename, url, f.crc32);
  }
}

WFUTError WFUTClient::getChannelList(const std::string &url, ChannelList &channels) {
  assert (m_initialised == true);
  // TODO: this is currently platform dependant!
  char filename[] = "/tmp/wfut.XXXXXX";
  int fd = mkstemp(filename);

  if (m_io->downloadFile(filename, url, 0)) {
    // error
    fprintf(stderr, "Error downloading channel list\n");
    close(fd);
    unlink(filename);
    return WFUT_DOWNLOAD_ERROR;
  }
  if (parseChannelList(filename, channels)) {
    // Error
    fprintf(stderr, "Error parsing channel list\n");
    close(fd);
    unlink(filename);
    return WFUT_PARSE_ERROR;
  }
  close(fd);
  unlink(filename);
  return WFUT_NO_ERROR;
}

WFUTError WFUTClient::getFileList(const std::string &url, ChannelFileList &files) {
  assert (m_initialised == true);
  // TODO: this is currently platform dependant!
  char filename[] = "/tmp/wfut.XXXXXX";
  int fd = mkstemp(filename);
  if (m_io->downloadFile(filename, url, 0)) {
    // error
    fprintf(stderr, "Error downloading file list\n");
    close(fd);
    unlink(filename);
    return WFUT_DOWNLOAD_ERROR;
  }

  if (parseFileList(filename, files)) {
    // Error
    fprintf(stderr, "Error parsing file list\n");
    close(fd);
    unlink(filename);
    return WFUT_PARSE_ERROR;
  }
  close(fd);
  unlink(filename);
  return WFUT_NO_ERROR;
}

WFUTError WFUTClient::getLocalList(const std::string &filename, ChannelFileList &files) {
  assert (m_initialised == true);
  if (parseFileList(filename, files)) {
    printf("Error parsing local file list\n");
    return WFUT_PARSE_ERROR;
  }
  return WFUT_NO_ERROR;
}

WFUTError WFUTClient::saveLocalList(const ChannelFileList &files, const std::string &filename) {
  assert (m_initialised == true);
  if (writeFileList(filename, files)) {
    // Error
    return WFUT_WRITE_ERROR;
  }
  return WFUT_NO_ERROR;
}

int WFUTClient::poll() {
  assert (m_initialised == true);
  return m_io->poll();
}

WFUTError WFUTClient::calculateUpdates(const ChannelFileList &server, const ChannelFileList &system, const ChannelFileList &local, ChannelFileList &updates, const std::string &prefix) {
  const FileMap &server_map = server.getFiles();
  const FileMap &system_map = system.getFiles();
  const FileMap &local_map = local.getFiles();

  FileMap::const_iterator I = server_map.begin();
  FileMap::const_iterator Iend = server_map.end();

  for (; I !=  Iend; ++I) {
    const FileObject &server_obj = I->second;
    // find the matching local one
    FileMap::const_iterator sys_iter = system_map.find(I->first);
    FileMap::const_iterator loc_iter = local_map.find(I->first);

    if (loc_iter == local_map.end()) {
      if (sys_iter == system_map.end()) {
printf("Adding %s as no local version\n", server_obj.filename.c_str());
        updates.addFile(server_obj);
      } else if (server_obj.version > sys_iter->second.version) {
printf("Updating %s as server is newer than sys\n", server_obj.filename.c_str());
        updates.addFile(server_obj);
      } else {
        // Assume the sys location is valid, so no need to update
      }
    } else if (server_obj.version > loc_iter->second.version) {
printf("Updating %s as server is newer than local\n", server_obj.filename.c_str());
      updates.addFile(server_obj);
    } else {
      // According to xml files, the local version is the same as the server
      // Lets check that it exists and has a matching CRC value.
      uLong crc32;
      if (calcCRC32(prefix + loc_iter->second.filename.c_str(), crc32) == -1) {
        // Can't read file, so lets add it
printf("Adding %s as local is missing\n", server_obj.filename.c_str());
        updates.addFile(server_obj);
      } else {
        // Do a CRC check and warn user that the file is modified.
        if (crc32 != server_obj.crc32) {
          // Modified!
          printf("File %s is modified. Remove to update.\n", loc_iter->second.filename.c_str());
        }
      }
    }
  }
  return WFUT_NO_ERROR;
}

}
