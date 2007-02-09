// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#ifndef LIBWFUT_CHANNELIO_H
#define LIBWFUT_CHANNELIO_H 1

#include <string>

#include <libwfut/types.h>

namespace WFUT {

static const std::string TAG_name = "name";
static const std::string TAG_description = "description";
static const std::string TAG_url = "url";
static const std::string TAG_email = "email";
static const std::string TAG_logo = "logo";
static const std::string TAG_channel = "channel";
static const std::string TAG_channellist = "channelList";

/**
 * Parse an XML document from a file and return the Channels in a 
 * ChannelList object
 * @param filename The filename of the XML file.
 * @param channels A ChannelList object to add parsed channels to.
 * @return 0 on success, error otherwise
 */
int parseChannelList(const std::string &filename, ChannelList &channels);


/**
 * Parse an XML document from a string and return the Channels in a 
 * ChannelList object
 * @param xml The string object containing the XML.
 * @param channels A ChannelList object to add parsed channels to.
 * @return 0 on success, error otherwise
 */
int parseChannelListXML(const std::string &xml, ChannelList &channels);


/**
 * Writes a ChannelList object to a file.
 * @param filename The filename to write to
 * @param channels The ChannelList object to write the file from.
 * @return 0 on success, error otherwise
 */
int writeChannelList(const std::string &filename, const ChannelList &channels);

} /* namespace WFUT */

#endif /* LIBWFUT_CANNELIO_H */
