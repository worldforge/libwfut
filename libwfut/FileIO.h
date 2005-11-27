// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#ifndef LIBWFUT_FILEIO_H
#define LIBWFUT_FILEIO_H 1

#include <string>

namespace WFUT {

class FileList;

static const std::string TAG_name = "name";
static const std::string TAG_version = "version";
static const std::string TAG_crc32 = "crc32";
static const std::string TAG_size = "size";
static const std::string TAG_execute = "execute";
static const std::string TAG_file = "file";
static const std::string TAG_filelist = "fileList";

int parseFileList(const std::string &filename, FileList &files);
int parseFileListXML(const std::string &xml, FileList &files);

int writeFileList(const std::string &filename, const FileList &files);
} /* namespace WFUT */

#endif /* LIBWFUT_FILEIO_H */
