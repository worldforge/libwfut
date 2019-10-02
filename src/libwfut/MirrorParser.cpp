// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall


#include "tinyxml/tinyxml.h"

#include "libwfut/types.h"
#include "libwfut/ChannelIO.h"

namespace WFUT {

static int parseMirror(TiXmlElement *element, MirrorObject &mirror) {
  assert(element);
  TiXmlNode * node;

  node = element->FirstChildElement(TAG_name);
  if (node) node = node->FirstChild();
  if (node) mirror.name = node->Value();

  node = element->FirstChildElement(TAG_url);
  if (node) node = node->FirstChild();
  if (node) mirror.url = node->Value();

  return 0;
}

static int parseMirrors(TiXmlNode *element, MirrorList &mirrors) {
  assert(element);

  TiXmlElement *e = element->FirstChildElement(TAG_mirror);
  while (e) {
    MirrorObject mirror;
    parseMirror(e, mirror);
    mirrors.push_back(mirror);
    e = e->NextSiblingElement();
  }
  return 0;
}
int parseMirrorList(const std::string &filename, MirrorList &mirrors) {

  TiXmlDocument doc(filename);

  if (!doc.LoadFile()) {
    return 1;
  }

  TiXmlNode *node = doc.FirstChild(TAG_mirrorlist);

  if (!node) {
    // missing root node
    return 1;
  }

  return parseMirrors(node, mirrors);

}

int parseMirrorListXML(const std::string &xml, MirrorList &mirrors) {

  TiXmlDocument doc;

  doc.Parse(xml.c_str());

  if (doc.Error()) {
    return 1;
  }

  TiXmlNode *node = doc.FirstChild(TAG_mirrorlist);

  if (!node) {
    // missing root node
    return 1;
  }

  return parseMirrors(node, mirrors);

}

} /* namespace WFUT */
