// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#ifndef LIBWFUT_FILEIO_H
#define LIBWFUT_FILEIO_H 1

#include <string>

#include <libwfut/types.h>

namespace WFUT {

class ChannelFileList;

static const std::string TAG_filename = "filename";
static const std::string TAG_version = "version";
static const std::string TAG_crc32 = "crc32";
static const std::string TAG_size = "size";
static const std::string TAG_execute = "execute";
static const std::string TAG_file = "file";
static const std::string TAG_filelist = "fileList";
static const std::string TAG_dir = "dir";
static const std::string TAG_deleted = "deleted";

int parseFileList(const std::string &filename, ChannelFileList &files);
int parseFileListXML(const std::string &xml, ChannelFileList &files);

int writeFileList(const std::string &filename, const ChannelFileList &files);
} /* namespace WFUT */

#endif /* LIBWFUT_FILEIO_H */
