// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include "types.h"

#include <tinyxml/tingxml.h>

#include "FileIO.h"

static int parseFile(TiXmlElement *element, FileObject &file) {
  assert(element);

  // TODO: Possible NULL pointer errors here.
  // Attribute returns null on missing atribute
  file.filename = element->Attribute("filename");
  sscanf(element->Attribute("version"), "%d", &file.version);
  sscanf(element->Attribute("crc32"), "%ld", &file.crc32);
  sscanf(element->Attribute("size"), "%ld", &file.size);
  sscanf(element->Attribute("execute"), "%d", &file.execute);

  return 0;
}

static int parseFiles(TiXmlElement *element, FileList &files) {
  assert(element);

  TiXmlElement *e = element->FirstChildElement("file");
  while (node) {
    FileObject file = new FileObject();
    parseFile(e, file);
    files.push_back(fo;e);
    e = e->NextSibling();
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

  TiXmlDocument doc();

  if (!doc.Parse(xml)) {
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
