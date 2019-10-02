// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#include <zlib.h>
#include <string>

namespace WFUT {

  int calcCRC32(const std::string &filename, uLong &crc);

}
