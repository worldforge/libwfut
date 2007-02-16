// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall


#include "tinyxml.h"

#include "libwfut/types.h"
#include "libwfut/FileIO.h"
#include "libwfut/ChannelFileList.h"
#include "libwfut/Encoder.h"

namespace WFUT {

static int writeFile(TiXmlElement *element, const FileObject &file) {
  assert(element);

  // TODO need to convert numbers to string as tinyxml doesn'y support 
  // unsigned lonfs

  element->SetAttribute(TAG_filename, Encoder::encodeString(file.filename));
  element->SetAttribute(TAG_version, file.version);
  element->SetAttribute(TAG_crc32, file.crc32);
  element->SetAttribute(TAG_size, file.size);
  // This also need to be converted to a string
  element->SetAttribute(TAG_execute, file.execute);

  return 0;
}

int writeFileList(const std::string &filename, const ChannelFileList &files) {
  TiXmlDocument doc;
  doc.InsertEndChild(TiXmlDeclaration("1.0", "", ""));

  TiXmlElement flist(TAG_filelist);
  flist.SetAttribute(TAG_dir, files.getName());
 
  const FileMap filemap = files.getFiles();
  FileMap::const_iterator itr = filemap.begin();
  while (itr != filemap.end()) {
    TiXmlElement file(TAG_file);
    writeFile(&file, itr->second);
    flist.InsertEndChild(file);
    ++itr;
  }

  doc.InsertEndChild(flist);
  if (!doc.SaveFile(filename)) {
    // error writing file
   return 1;
  }
  return 0;
}

} /* namespace WFUT */
