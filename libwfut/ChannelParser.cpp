// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall


#include "tinyxml.h"

#include "libwfut/types.h"
#include "libwfut/ChannelIO.h"

namespace WFUT {

static int parseChannel(TiXmlElement *element, ChannelObject &channel) {
  assert(element);
  TiXmlNode * node;
  const char *name_val = element->Attribute(TAG_name);
  if (name_val != NULL) {
    channel.name = name_val;
  }

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

static int parseChannels(TiXmlNode *element, ChannelList &channels) {
  assert(element);

  TiXmlElement *e = element->FirstChildElement(TAG_channel);
  while (e) {
    ChannelObject channel;
    parseChannel(e, channel);
    channels.push_back(channel);
    e = e->NextSiblingElement();
  }
  return 0;
}
int parseChannelList(const std::string &filename, ChannelList &channels) {

  TiXmlDocument doc(filename);

  if (!doc.LoadFile()) {
    return 1;
  }

  TiXmlNode *node = doc.FirstChild(TAG_channellist);

  if (!node) {
    // missing root node
    return 1;
  }

  return parseChannels(node, channels);

}

int parseChannelListXML(const std::string &xml, ChannelList &channels) {

  TiXmlDocument doc;

  if (!doc.Parse(xml.c_str())) {
    return 1;
  }

  TiXmlNode *node = doc.FirstChild(TAG_channellist);

  if (!node) {
    // missing root node
    return 1;
  }

  return parseChannels(node, channels);

}

} /* namespace WFUT */
