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
#include "libwfut/platform.h"

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
    if (f.deleted) continue;

    const std::string &url = urlPrefix + updates.getName() + "/" + f.filename;
    m_io->queueFile(pathPrefix, f.filename, url, f.crc32, f.execute);
  }
}


void WFUTClient::updateFile(const FileObject &file,
                            const std::string &urlPrefix,
                            const std::string &pathPrefix) {
  assert (m_initialised == true);
  if (file.deleted) return;

  const std::string &url = urlPrefix + "/" + file.filename;
  m_io->queueFile(pathPrefix, file.filename, url, file.crc32, false);
}

WFUTError WFUTClient::getMirrorList(const std::string &url, MirrorList &mirrors) {
  assert (m_initialised == true);

  FILE *fp = os_create_tmpfile();
  if (!fp) {
    return WFUT_GENERAL_ERROR;
  }

  if (m_io->downloadFile(fp, url, 0)) {
    // error
//    fprintf(stderr, "Error downloading file list\n");
    os_free_tmpfile(fp);
    return WFUT_DOWNLOAD_ERROR;
  }

  std::string xml;
  // Pre-allocate string memory
  xml.reserve(ftell(fp));
  rewind(fp);
  char buf[1024];
  size_t n;
  while ((n = fread(buf, sizeof(char), 1024, fp) ) > 0) {
    xml.append(buf, n);  
  }

  os_free_tmpfile(fp);

  if (parseMirrorListXML(xml, mirrors)) {
    // Error
//    fprintf(stderr, "Error parsing file list\n");
    return WFUT_PARSE_ERROR;
  }

  return WFUT_NO_ERROR;
}


WFUTError WFUTClient::getChannelList(const std::string &url, ChannelList &channels) {
  assert (m_initialised == true);

  FILE *fp = os_create_tmpfile();
  if (!fp) {
    return WFUT_GENERAL_ERROR;
  }

  if (m_io->downloadFile(fp, url, 0)) {
    // error
//    fprintf(stderr, "Error downloading file list\n");
    os_free_tmpfile(fp);
    return WFUT_DOWNLOAD_ERROR;
  }

  std::string xml;
  // Pre-allocate string memory
  xml.reserve(ftell(fp));
  rewind(fp);
  char buf[1024];
  size_t n;
  while ((n = fread(buf, sizeof(char), 1024, fp) ) > 0) {
    xml.append(buf, n);  
  }

  os_free_tmpfile(fp);

  if (parseChannelListXML(xml, channels)) {
    // Error
//    fprintf(stderr, "Error parsing file list\n");
    return WFUT_PARSE_ERROR;
  }

  return WFUT_NO_ERROR;
}

WFUTError WFUTClient::getFileList(const std::string &url, ChannelFileList &files) {
  assert (m_initialised == true);

  FILE *fp = os_create_tmpfile();
  if (!fp) {
//    fprintf(stderr, "Unable to create temporary file\n");
//    perror("");
    return WFUT_GENERAL_ERROR;
  }

  if (m_io->downloadFile(fp, url, 0)) {
    // error
//    fprintf(stderr, "Error downloading file list\n");
    os_free_tmpfile(fp);
    return WFUT_DOWNLOAD_ERROR;
  }

  std::string xml;
  // Pre-allocate string memory
  xml.reserve(ftell(fp));
  rewind(fp);
  char buf[1024];
  size_t n;
  while ((n = fread(buf, sizeof(char), 1024, fp) ) > 0) {
    xml.append(buf, n);  
  }

  os_free_tmpfile(fp);

  if (parseFileListXML(xml, files)) {
    // Error
//    fprintf(stderr, "Error parsing file list\n");
    return WFUT_PARSE_ERROR;
  }

  return WFUT_NO_ERROR;
}

WFUTError WFUTClient::getLocalList(const std::string &filename, ChannelFileList &files) {
  assert (m_initialised == true);
  if (parseFileList(filename, files)) {
//    printf("Error parsing local file list\n");
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
    // Server side deleted? Just ignore for now
    if (server_obj.deleted) {
      UpdateReason.emit(server_obj.filename, WFUT_UPDATE_DELETED);
      continue;
    }
    // find the matching local one
    FileMap::const_iterator sys_iter = system_map.find(I->first);
    FileMap::const_iterator loc_iter = local_map.find(I->first);

    if (loc_iter == local_map.end()) {
      if (sys_iter == system_map.end()) {
        // No local version, or system version
        UpdateReason.emit(server_obj.filename, WFUT_UPDATE_NO_LOCAL);
        updates.addFile(server_obj);
      } else if (server_obj.version > sys_iter->second.version) {
        // Servere version is newer than sys version, and no local version
        UpdateReason.emit(server_obj.filename, WFUT_UPDATE_SERVER_SYSTEM);
        updates.addFile(server_obj);
      } else {
        // Assume the sys location is valid, so no need to update
        // No update required
        UpdateReason.emit(server_obj.filename, WFUT_UPDATE_NONE);
      }
    } else if (server_obj.version > loc_iter->second.version) {
      UpdateReason.emit(server_obj.filename, WFUT_UPDATE_SERVER_LOCAL);
      updates.addFile(server_obj);
    } else {
      // According to xml files, the local version is the same as the server
      // Lets check that it exists and has a matching CRC value.
      uLong crc32;
      if (calcCRC32(prefix + loc_iter->second.filename.c_str(), crc32) == -1) {
        // Can't read file, so lets add it
        UpdateReason.emit(server_obj.filename, WFUT_UPDATE_MISSING);
        updates.addFile(server_obj);
      } else {
        // Do a CRC check and warn user that the file is modified.
        if (crc32 != server_obj.crc32) {
          // Modified!
          UpdateReason.emit(server_obj.filename, WFUT_UPDATE_MODIFIED);
        } else {
          // No update required
          UpdateReason.emit(server_obj.filename, WFUT_UPDATE_NONE);
        }
      }
    }
  }
  return WFUT_NO_ERROR;
}

void WFUTClient::cancelAll() {
  m_io->cancelAll();
}

}
