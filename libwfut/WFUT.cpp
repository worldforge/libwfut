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

void writeHeader(FILE *fp, const std::string &channel) {
  assert(fp != NULL);
  fprintf(fp, "<?xml version=\"1.0\"?>\n");
  fprintf(fp, "<?xml-stylesheet type=\"text/xsl\" href=\"filelist.xsl\"?>\n");
  fprintf(fp, "<fileList dir=\"%s\">\n", channel.c_str());
}

void writeTempList(const std::string &tmpfile, const std::string channel, const FileObject &fo) {
  FILE *fp = fopen(tmpfile.c_str(), "wt");
  if (fp == 0) {
    // error opening file    
    return;
  }
}

int WFUTClient::init() {
  assert (m_initialised == false);

  m_io = new IO();
  if (m_io->init()) {
    delete m_io;
    m_io = NULL;
    return 1;
  }
  // TODO, these should really be installed by the client app
  m_io->DownloadComplete.connect(sigc::mem_fun(this, &WFUTClient::onDownloadComplete));
  m_io->DownloadFailed.connect(sigc::mem_fun(this, &WFUTClient::onDownloadFailed));

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

void WFUTClient::updateChannel(const ChannelFileList &updates,
                               const std::string &urlPrefix,
                               const std::string &pathPrefix) {
  assert (m_initialised == true);
  const FileMap &files = updates.getFiles();

  FileMap::const_iterator I = files.begin();
  while (I != files.end()) {
    const FileObject &f = (I++)->second;

    std::string url = urlPrefix + updates.getName() + "/" + f.filename;
    m_io->queueFile(pathPrefix, f.filename, url, f.crc32);
  }
}

int WFUTClient::getChannelList(const std::string &url, ChannelList &channels) {
  assert (m_initialised == true);
  // TODO: this is currently platform dependant!
  char filename[] = "/tmp/wfut.XXXXXX";
  int fd = mkstemp(filename);

  if (m_io->downloadFile(filename, url, 0)) {
    // error
    fprintf(stderr, "Error downloading channel list\n");
    close(fd);
    unlink(filename);
    return 1;
  }
  if (parseChannelList(filename, channels)) {
    // Error
    fprintf(stderr, "Error parsing channel list\n");
    close(fd);
    unlink(filename);
    return 2;
  }
  close(fd);
  unlink(filename);
  return 0;
}

int WFUTClient::getFileList(const std::string &url, ChannelFileList &files) {
  assert (m_initialised == true);
  // TODO: this is currently platform dependant!
  char filename[] = "/tmp/wfut.XXXXXX";
  int fd = mkstemp(filename);
  if (m_io->downloadFile(filename, url, 0)) {
    // error
    fprintf(stderr, "Error downloading file list\n");
    close(fd);
    unlink(filename);
    return 1;
  }

  if (parseFileList(filename, files)) {
    // Error
    fprintf(stderr, "Error parsing file list\n");
    close(fd);
    unlink(filename);
    return 2;
  }
  close(fd);
  unlink(filename);
  return 0;
}

int WFUTClient::getLocalList(const std::string &filename, ChannelFileList &files) {
  assert (m_initialised == true);
  if (parseFileList(filename, files)) {
    printf("Error parsing local file list\n");
    return 1;
  }
  return 0;
}

int WFUTClient::saveLocalList(const ChannelFileList &files, const std::string &filename) {
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

int WFUTClient::calculateUpdates(const ChannelFileList &server, const ChannelFileList &system, const ChannelFileList &local, ChannelFileList &updates, const std::string &prefix) {
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
  return 0;
}

}
