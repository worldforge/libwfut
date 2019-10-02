// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#include <zlib.h>
#include <string>
#include <stdio.h>

#include "crc32.h"
namespace WFUT {

int calcCRC32(const std::string &filename, uLong &crc) {
  // Open file for writing
  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    // Error!
    return -1;
  }
  // Initialise CRC32 value
  crc = crc32(0L, Z_NULL, 0);
  Bytef buf[8192];
  size_t len;
  // Read all bytes in file and calculate the crc32 value
  while ((len = fread(&buf[0], sizeof(Bytef), 8192, fp)) != 0) {
    crc = crc32(crc, buf, len);
  }
  // Close file handle
  fclose(fp);
  // Return calculated CRC32
  return 0;
}

}
