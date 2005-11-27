// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include "types.h"

#include <tinyxml/tinyxml.h>

#include "ChannelIO.h"

static int parseChannel(TiXmlElement *element, ChannelObject &channel) {
  assert(element);
  TiXmlNode * node;

  node = element->FirstChildElement(TAG_name);
  if (node) node = node->FirstChild();
  if (node) channel.name = node->Value();

  node = element->FirstChildElement(TAG_description);
  if (node) node = node->FirstChild();
  if (node) channel.description = node->Value();

  node = element->FirstChildElement(TAG_url);
  if (node) node = node->FirstChild();
  if (node) channel.url = node->Value();

  node = element->FirstChildElement(TAG_email);
  if (node) node = node->FirstChild();
  if (node) channel.email = node->Value();

  node = element->FirstChildElement(TAG_logo);
  if (node) node = node->FirstChild();
  if (node) channel.logo = node->Value();

  return 0;
}

static int parseChannels(TiXmlElement *element, ChannelList &channels) {
  assert(element);

  TiXmlElement *e = element->FirstChildElement(TAG_channel);
  while (node) {
    ChannelObject channel = new ChannelObject();
    parseChannel(e, channel);
    channels.push_back(channel);
    e = e->NextSibling();
  }
  return 0;
}
int parseChannelList(const std::string &filename, ChannelList &channels) {

  TiXmlDocument doc(filename);

  if (!doc.LoadFile()) {
    return 1;
  }

  TiXmlNode *node = doc.FirstChild(TAG_channelList);

  if (!node) {
    // missing root node
    return 1;
  }

  return parseChannels(node, channels);

}

int parseChannelListXML(const std::string &xml, ChannelList &channels) {

  TiXmlDocument doc();

  if (!doc.Parse(xml.c_str())) {
    return 1;
  }

  TiXmlNode *node = doc.FirstChild(TAG_channelList);

  if (!node) {
    // missing root node
    return 1;
  }

  return parseChannels(node, channels);

}
