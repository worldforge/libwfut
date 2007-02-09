// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2007 Simon Goodall

#ifndef LIBWFUT_ENCODER_H
#define LIBWFUT_ENCODER_H 1

#include <string>

namespace WFUT {

class Encoder {
public:
  static std::string encode(const std::string &str);
  static std::string decode(const std::string &str);
};

} /* namespace WFUT */

#endif /* LIBWFUT_ENCODER_H */
