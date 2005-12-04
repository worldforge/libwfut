// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include "types.h"

#include <tinyxml/tinyxml.h>

#include "FileIO.h"

namespace WFUT {

static int parseFile(TiXmlElement *element, FileObject &file) {
  assert(element);

  // TODO: Possible NULL pointer errors here.
  // Attribute returns null on missing atribute
  file.filename = element->Attribute(TAG_filename);
  sscanf(element->Attribute(TAG_version), "%d", &file.version);
  sscanf(element->Attribute(TAG_crc32), "%ld", &file.crc32);
  sscanf(element->Attribute(TAG_size), "%ld", &file.size);
  char buf[4];
  sscanf(element->Attribute(TAG_execute), "%4s", buf);
  if (strncmp(buf, "true", 4)) file.execute = false;
  else file.execute = true;

  return 0;
}

static int parseFiles(TiXmlNode *element, FileList &files) {
  assert(element);

  TiXmlElement *e = element->FirstChildElement(TAG_file);
  while (e) {
    FileObject file;
    parseFile(e, file);
    files.push_back(file);
    e = e->NextSiblingElement();
  }

  return 0;
}

int parseFileList(const std::string &filename, FileList &files) {

  TiXmlDocument doc(filename);

  if (!doc.LoadFile()) {
    // Error parsing file
    return 1;
  }

  TiXmlNode *node = doc.FirstChild("fileList");

  if (!node) {
    // missing root node
    return 1;
  }

  return parseFiles(node, files);

}
int parseFileListXML(const std::string &xml, FileList &files) {

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
