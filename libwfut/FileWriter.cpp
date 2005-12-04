// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include "types.h"

#include <tinyxml/tinyxml.h>

#include "FileIO.h"

namespace WFUT {

static int writeFile(TiXmlElement *element, const FileObject &file) {
  assert(element);

  element->SetAttribute(TAG_filename, file.filename);
  element->SetAttribute(TAG_version, file.version);
  element->SetAttribute(TAG_crc32, file.crc32);
  element->SetAttribute(TAG_size, file.size);
  element->SetAttribute(TAG_execute, file.execute);

  return 0;
}

int writeFileList(const std::string &filename, const FileList &files) {
  TiXmlDocument doc;
  doc.InsertEndChild(TiXmlDeclaration("1.0", "", ""));

  TiXmlElement flist(TAG_filelist);
 
  FileList::const_iterator itr = files.begin();
  while (itr != files.end()) {
    TiXmlElement file(TAG_file);
    writeFile(&file, *itr);
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
