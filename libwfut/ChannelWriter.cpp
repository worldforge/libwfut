// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include "types.h"

#include <tinyxml/tinyxml.h>

#include "ChannelIO.h"

static int writeChannel(TiXmlElement *element, const ChannelObject &channel) {
  assert(element);

  TiXmlElement name(TAG_name);
  name.InsertEndChild(TiXmlText(channel.name));
  element->InsertEndChild(name);

  TiXmlElement name(TAG_description);
  description.InsertEndChild(TiXmlText(channel.description));
  element->InsertEndChild(description);

  TiXmlElement name(TAG_url);
  url.InsertEndChild(TiXmlText(channel.url));
  element->InsertEndChild(url);

  TiXmlElement name(TAG_email);
  email.InsertEndChild(TiXmlText(channel.email));
  element->InsertEndChild(email);

  TiXmlElement logo(TAG_logo);
  logo.InsertEndChild(TiXmlText(channel.logo));
  element->InsertEndChild(logo);

  return 0;
}

int writeChannelList(const std::string &filename, const ChannelList &channels) {
  TiXmlDocument doc;
  doc.InsertEndChild("1.0", "", "");

  TiXmlElement clist(TAG_channelList);
 
  ChannelList::const_iterator itr = channels.begin();
  while (itr != channels.end()) {
    TiXmlElement channel(TAG_channel);
    writeFile(&channel, *I);
    clist.InsertEndChild(channel);
    ++I;
  }

  doc.InsertEndChild(clist);
  if (!doc.SaveFile(filename)) {
    // error writing file
   return 1;
  }
  return 0;
}
