// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2007 Simon Goodall

#include <cstdio>

#include "libwfut/Encoder.h"
#ifdef _WIN32
#define snprintf _snprintf
#endif
namespace WFUT {

std::string Encoder::encodeString(const std::string &str) {
  std::string out;

  for (size_t i = 0; i < str.size(); ++i) {
    char c = str[i];
    // Check char ranges
    if ((c >= 'a' && c <= 'z')
      || (c >= 'A' && c <= 'Z')
      || (c >= '0' && c <= '9') ) {
      out += c;
    // Check individual chars
    } else if (c == '-' || c == '_' || c == '.' || c == '*') {
      out += c;
    // Special case for a space
    } else if (c == ' ') {
      out += '+';
    // Convert everything else to char codes
    } else {
      char buf[4];
      buf[3] = '\0';
      snprintf(buf, 4, "%%%2.2X", c);
      out += buf;
    }
  }

  return out;
}

std::string Encoder::decodeString(const std::string &str) {
  std::string out;
  
  for (size_t i = 0; i < str.size(); ++i) {
    char c = str[i];
    if (c == '%') {
      const char *buf = &str.c_str()[i];
      unsigned int u;
      sscanf(buf, "%%%2x", &u);
      out += (char)u;
      i+=2; // Increment counter against extra chars just used
    } else if (c == '+') {
      out += ' ';
    } else {
      out +=c;
    }
  }

  return out;
}

std::string Encoder::encodeURL(const std::string &str) {
  std::string out;

  for (size_t i = 0; i < str.size(); ++i) {
    char c = str[i];
    // Check char ranges
    if ((c >= 'a' && c <= 'z')
      || (c >= 'A' && c <= 'Z')
      || (c >= '0' && c <= '9') ) {
      out += c;
    // Special case chars
    } else if (c == '-' || c == '_' || c == '.' || c == '|' || c == '~' || c == '*' || c == '\'' || c == '(' || c == ')') {
      out += c;
    // More special case chars
    } else if (c == ';' || c == '/' || c == '?' || c == ':' || c == '@' || c == '&' || c == '=' || c == '+' || c == '$' ||  c == ',') {
      out += c;
    // Convert everything else to char codes
    } else {
      char buf[4];
      buf[3] = '\0';
      snprintf(buf, 4, "%%%2.2X", c);
      out += buf;
    }
  }

  return out;
}

} /* namespace WFUT */
