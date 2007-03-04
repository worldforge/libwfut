// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2007 Simon Goodall

#ifndef LIBWFUT_PLATFORM_H
#define LIBWFUT_PLATFORM_H 1

#include <string>

namespace WFUT {

FILE *os_create_tmpfile();
void os_free_tmpfile(FILE *fp);

int os_mkdir(const std::string &dir);
bool os_exists(const std::string &file);
int os_set_executable(const std::string &file);

} /* namespace WFUT */

#endif /* LIBWFUT_PLATFORM_H */
