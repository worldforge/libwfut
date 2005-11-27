// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include "types.h"

#include <tinyxml/tinyxml.h>

#include "FileIO.h"

static int writeFile(TiXmlElement *element, const File &file) {
  assert(element);

  element->setAttribute(TAG_name, file.name);
  element->setAttribute(TAG_version, file.version);
  element->setAttribute(TAG_crc32, file.crc32);
  element->setAttribute(TAG_size, file.size);
  element->setAttribute(TAG_execute, file.execute);

  return 0;
}

int writeFileList(const std::string &filename, const FileList &files) {
  TiXmlDocument doc;
  doc.InsertEndChild("1.0", "", "");

  TiXmlElement flist(TAG_filelist);
 
  FileList::const_iterator itr = files.begin();
  while (itr != files.end()) {
    TiXmlElement file(TAG_file);
    writeFile(&file, *I);
    flist.InsertEndChild(file);
    ++I;
  }

  doc.InsertEndChild(flist);
  if (!doc.SaveFile(filename)) {
    // error writing file
   return 1;
  }
  return 0;
}
