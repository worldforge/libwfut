// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#ifndef LIBWFUT_HEADER_H
#define LIBWFUT_HEADER_H 1

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#if defined (WIN32) || defined (_WIN32) || defined( __WIN32__)
#define WINDOWS_BUILD 1
#endif

#if defined(WINDOWS_BUILD) && false
  #ifdef BUILD_LIBWFUT_DLL
    #define LIBWFUTAPI EXTERN __declspec(dllexport)
  #else
    #define LIBWFUTAPI EXTERN __declspec(dllimport)
  #endif
#else
  #define LIBWFUTAPI EXTERN
#endif

#endif // LIBWFUT_HEADER
