// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include <zlib.h>
#include <string>
#include <stdio.h>
long calcCRC32(const std::string &filename) {
  uLong crc = crc32(0L, Z_NULL,0);

  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    // Error!
    return -1;
  }
  char buf[8192];
  size_t len;
  while ((len = fread(&buf[0], sizeof(char), 8192, fp)) != 0) {
    crc = crc32(crc, buf, len);
  }
  fclose(fp);
  return (long)crc;
}
