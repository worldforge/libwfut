// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#include "tinyxml.h"

#include "libwfut/types.h"
#include "libwfut/FileIO.h"
#include "libwfut/ChannelFileList.h"
#include "libwfut/Encoder.h"

namespace WFUT {

static int parseFile(TiXmlElement *element, FileObject &file) {
  assert(element);

  const char *fname = element->Attribute(TAG_filename);
  if (fname != NULL) {
    file.filename = Encoder::decode(fname);
  }
  sscanf(element->Attribute(TAG_version), "%d", &file.version);
  sscanf(element->Attribute(TAG_crc32), "%lu", &file.crc32);
  sscanf(element->Attribute(TAG_size), "%ld", &file.size);
  const char *exec = element->Attribute(TAG_execute);
  if (exec && strlen(exec) >= 4 && strncmp(exec, "true", 4)) file.execute = false;
  else file.execute = true;

  return 0;
}

static int parseFiles(TiXmlNode *node, ChannelFileList &files) {
  assert(node);

  const char *dir = node->ToElement()->Attribute(TAG_dir);
  if (dir) {
    files.setName(dir);
  } 

  TiXmlElement *e = node->FirstChildElement(TAG_file);
  while (e) {
    FileObject file;
    parseFile(e, file);
    files.addFile(file);
    e = e->NextSiblingElement();
  }

  return 0;
}

int parseFileList(const std::string &filename, ChannelFileList &files) {

  TiXmlDocument doc(filename);

  if (!doc.LoadFile()) {
    // Error parsing file
    return 1;
  }

  TiXmlNode *node = doc.FirstChild(TAG_filelist);

  if (!node) {
    // missing root node
    return 1;
  }

  return parseFiles(node, files);

}
int parseFileListXML(const std::string &xml, ChannelFileList &files) {

  TiXmlDocument doc;

  if (!doc.Parse(xml.c_str())) {
    // Error parsing file
    return 1;
  }

  TiXmlNode *node = doc.FirstChild(TAG_filelist);

  if (!node) {
    // missing root node
    return 1;
  }

  return parseFiles(node, files);

}

} /* namespaceWFUT */
