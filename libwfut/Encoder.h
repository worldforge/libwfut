// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2007 Simon Goodall

#ifndef LIBWFUT_ENCODER_H
#define LIBWFUT_ENCODER_H 1

#include <string>

namespace WFUT {

/**
 * The Encoder class provides a few utility functions to convert strings to a 
 * format suitable for storing in an XML document and as a URL.
 */ 
class Encoder {
public:
  /**
   * Encode the input string so that is can safely be stored in an XML/HTML 
   * document.
   */
  static std::string encodeString(const std::string &str);

  /**
   * Dencode the input string from an XML encoded string back to it's original
   * form.
   */
  static std::string decodeString(const std::string &str);

  /**
   * Encodes characters in the input string so that it is a valid URL string.
   */
  static std::string encodeURL(const std::string &str);
};

} /* namespace WFUT */

#endif /* LIBWFUT_ENCODER_H */
