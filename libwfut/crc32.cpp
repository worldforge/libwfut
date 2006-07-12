// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2006 Simon Goodall

#include <zlib.h>
#include <string>
#include <stdio.h>

uLong calcCRC32(const std::string &filename) {
  // Open file for writing
  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    // Error!
    return -1;
  }
  // Initialise CRC32 value
  uLong crc = crc32(0L, Z_NULL, 0);
  char buf[8192];
  size_t len;
  // Read all bytes in file and calculate the crc32 value
  while ((len = fread(&buf[0], sizeof(char), 8192, fp)) != 0) {
    crc = crc32(crc, buf, len);
  }
  // Close file handle
  fclose(fp);
  // Return calculated CRC32
  return crc;
}
