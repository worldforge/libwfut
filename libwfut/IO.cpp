// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2008 Simon Goodall

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

#include "libwfut/IO.h"
#include "libwfut/Encoder.h"
#include "libwfut/platform.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace WFUT {

static const bool debug = false;

// Create the parent dir of the file.
// TODO Does this work if the parent parent dir does not exist?
static int createParentDirs(const std::string &filename) {
  int err = 1;
  // TODO This function may not work correctly or be portable.
  // Perhaps should only search for '\\' on win32, '/' otherwise
  size_t pos = filename.find_last_of("\\/");
  // Return if no separator is found, or if it is the first
  // character, e.g. /home
  if (pos == std::string::npos || pos == 0) return 0;

  const std::string &path = filename.substr(0, pos);

  if ((err = createParentDirs(path))) {
    // There was an error creating the parent path.
    return err;
  }

  // See if the directory already exists
  DIR *d = opendir(path.c_str());
  if (!d) {
    // Make dir as it doesn't exist
    err = os_mkdir(path);
  } else{
    closedir(d);
    err = 0;
  }
  return err;
}

static int copy_file(FILE *fp, const std::string &target_filename) {

  if (createParentDirs(target_filename)) {
    // Error making dir structure
    fprintf(stderr, "There was an error creating the required directory tree for %s.\n", target_filename.c_str());
    return 1;
  }
  FILE *tp = fopen(target_filename.c_str(), "wb");
  if (!tp) {
    // Error opening file to write
    return 1;
  }

  if (fp) {
    rewind(fp);
    char buf[1024];
    size_t num;
    while ((num = fread(buf, sizeof(char), 1024, fp)) != 0) {
      fwrite(buf, sizeof(char), num, tp);
    }
  } else {
    // No fp? we should only get here if the file was empty
    // so all this function will do is create an empty file.
  }
  fclose(tp);

  return 0;
}

// Callback function to write downloaded data to a file.
static size_t write_data(void *buffer, size_t size, size_t nmemb,void *userp) {
  assert(userp != NULL);
  DataStruct *ds = reinterpret_cast<DataStruct*>(userp);
  
  // Need to create a file in fp is NULL
  if (ds->fp == NULL) {
    // Open File handle
    ds->fp = os_create_tmpfile();
    // TODO Check that filehandle is valid
    if (ds->fp == NULL) {
      fprintf(stderr, "Error opening file for writing\n");
      return 0;
    }
    // Initialise CRC32 value
    ds->actual_crc32 = crc32(0L, Z_NULL,  0);
  }

  assert(ds->fp != NULL);

  // Update crc32 value
  ds->actual_crc32 = crc32(ds->actual_crc32, reinterpret_cast<Bytef*>(buffer), size * nmemb);

  // Write data to file
  return fwrite(buffer, size, nmemb, ds->fp);
}

/**
 * Set some options that each handle needs
 */
static int setDefaultOpts(CURL *handle) {
  curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(handle, CURLOPT_FAILONERROR, 1);
  return 0;
}

int IO::init() {
  assert (m_initialised == false);

  curl_global_init(CURL_GLOBAL_ALL);

  m_mhandle = curl_multi_init();
#ifdef HAVE_CURL_MULTI_PIPELINING
  curl_multi_setopt(m_mhandle, CURLMOPT_PIPELINING, 1);
#endif

  m_initialised = true;

  return 0;
}

int IO::shutdown(){
  assert (m_initialised == true);

  curl_multi_cleanup(m_mhandle);
  m_mhandle = NULL;

  while (!m_files.empty()) {
    DataStruct *ds = m_files.begin()->second;
    if (ds->handle) {
      curl_easy_cleanup(ds->handle);
      ds->handle = NULL;
    }
    if (ds->fp) {
      fclose(ds->fp);
      ds->fp = NULL;
    }
    delete ds;
    m_files.erase(m_files.begin());
  }

  curl_global_cleanup();

  m_initialised = false;

  return 0;
}

int IO::downloadFile(const std::string &filename, const std::string &url, uLong expected_crc32) {

  DataStruct ds;
  ds.fp = NULL;
  ds.url = Encoder::encodeURL(url);
  ds.filename = filename;
  ds.executable = false;
  ds.actual_crc32 = crc32(0L, Z_NULL,  0);
  ds.expected_crc32 = expected_crc32;
  ds.handle = curl_easy_init();

  setDefaultOpts(ds.handle);
  curl_easy_setopt(ds.handle, CURLOPT_URL, ds.url.c_str());
  curl_easy_setopt(ds.handle, CURLOPT_WRITEDATA, &ds);

  CURLcode err = curl_easy_perform(ds.handle);
  // TODO: Report back the error message
  //       Either convert code to message
  //       Or set CURLOPT_ERRORBUFFER using curl_easy_setopt 
  //       to record the message.
  int error = 1;
  if (err == 0) {
    if (copy_file(ds.fp, ds.filename) == 0) {
      error = 0;
    }
  }

  if (ds.fp) fclose(ds.fp);
  curl_easy_cleanup(ds.handle);

  // Zero on success
  return error;
}

int IO::downloadFile(FILE *fp, const std::string &url, uLong expected_crc32) {

  DataStruct ds;
  ds.fp = fp;
  ds.url = Encoder::encodeURL(url);
  ds.executable = false;
  ds.filename = "";
  ds.actual_crc32 = crc32(0L, Z_NULL,  0);
  ds.expected_crc32 = expected_crc32;
  ds.handle = curl_easy_init();

  setDefaultOpts(ds.handle);
  curl_easy_setopt(ds.handle, CURLOPT_URL, ds.url.c_str());
  curl_easy_setopt(ds.handle, CURLOPT_WRITEDATA, &ds);
  CURLcode err = curl_easy_perform(ds.handle);

  curl_easy_cleanup(ds.handle);

  // TODO: Report back the error message
  //       Either convert code to message
  //       Or set CURLOPT_ERRORBUFFER using curl_easy_setopt 
  //       to record the message.

  // Zero on success
  return (err != 0);
}

int IO::queueFile(const std::string &path, const std::string &filename, const std::string &url, uLong expected_crc32, bool executable) {
  if (m_files.find(url) != m_files.end()) {
    fprintf(stderr, "Error file is already in queue\n");
    // Url already in queue
    return 1;
  }

  DataStruct *ds = new DataStruct();
  ds->fp = NULL;
  ds->url = Encoder::encodeURL(url);
  ds->filename = filename;
  ds->path = path;
  ds->executable = executable;
  ds->actual_crc32 = crc32(0L, Z_NULL,  0);
  ds->expected_crc32 = expected_crc32;
  ds->handle = curl_easy_init();

  m_files[ds->url] = ds;
  setDefaultOpts(ds->handle);
  curl_easy_setopt(ds->handle, CURLOPT_URL, ds->url.c_str());
  curl_easy_setopt(ds->handle, CURLOPT_WRITEDATA, ds);
  curl_easy_setopt(ds->handle, CURLOPT_PRIVATE, ds);

  // Add handle to our queue instead of to curl directly so that
  // we can limit the number of connections we make to the server.
  m_handles.push_back(ds->handle);

  return 0;
}

int IO::poll() {
  // Do some work and get number of handles in progress
  int num_handles;
  curl_multi_perform(m_mhandle, &num_handles);

  struct CURLMsg *msg = NULL;
  int msgs;

  // Get messages back out of CURL
  while ((msg = curl_multi_info_read(m_mhandle, &msgs)) != NULL) {

    DataStruct *ds = NULL;
    int err = curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &ds);
    if (err != CURLE_OK) {
      // Do something on error
      fprintf(stderr, "Got some error on curl_easy_getinfo (%d)\n", err);
      continue;
    }

    bool failed = true; 
    std::string errormsg = "Unknown failure";
    switch (msg->msg) {
      case CURLMSG_DONE: {
        if (msg->data.result == CURLE_OK) {
          assert(ds != NULL);
          if (ds->expected_crc32 == 0L || 
              ds->expected_crc32 == ds->actual_crc32) {
            // Download success!
            failed = false;
            // Copy file to proper location
            if (copy_file(ds->fp, ds->path + "/" + ds->filename)) {
              errormsg = "Error copying file to target location.\n";
              failed = true;
            }

            // Set executable if required
            if (ds->executable) {
              os_set_executable(ds->path + "/" + ds->filename);
            }
          } else {
            // CRC32 check failed
            failed = true;
            errormsg = "CRC32 mismatch";
          }
        } else {
          // Error downloading file
          failed = true;
          errormsg = "There was an error downloading the requested file: "
                   + std::string(curl_easy_strerror(msg->data.result));
        }
        break;
      }
      default:
        // Something not too good with curl...
        failed = true;
        errormsg = "There was an unknown error downloading the requested file";
    }

    if (debug) printf("Removing Handle\n");

    // Close handle	
    curl_multi_remove_handle(m_mhandle, msg->easy_handle);

    // Clean up
    if (ds) {
      if (ds->fp) os_free_tmpfile(ds->fp);
      ds->fp = NULL;
      if (failed) {
        if (debug) printf("Download Failed\n");
        DownloadFailed.emit(ds->url, ds->filename, errormsg);
      } else {
        if (debug) printf("Download Complete\n");
        DownloadComplete.emit(ds->url, ds->filename);
      }
      m_files.erase(m_files.find(ds->url));
      curl_easy_cleanup(ds->handle);
      delete ds;
    }
  }

  // Spare capacity? Lets queue some more items.
  int diff = m_num_to_process - num_handles;
  if (diff > 0) {
    while (diff--) {

      if (!m_handles.empty()) {
        // This is where we tell curl about our downloads.
        curl_multi_add_handle(m_mhandle, m_handles.front());
        m_handles.pop_front();
        ++num_handles;
      }
    }
  }

  return num_handles;
}

/**
 * Abort all current and pending downloads.
 */
void IO::abortAll() {

  while (!m_files.empty()) {
    DataStruct *ds = (m_files.begin())->second;
    abortDownload(ds);
    delete ds;
    m_files.erase(m_files.begin());
  }
}

/**
 * Abort all current and pending downloads.
 */
void IO::abortDownload(const std::string &filename) {
  std::map<std::string, DataStruct*>::iterator I = m_files.find(filename);

  if (I != m_files.end()) {
    DataStruct *ds = I->second;
    abortDownload(ds);
    delete ds;
    m_files.erase(I);
  }
}

void IO::abortDownload(DataStruct *ds) {

  if (ds->handle) {
    // Find handle in pending list
    std::deque<CURL*>::iterator I = std::find(m_handles.begin(), m_handles.end(), ds->handle);
    // Not found? Must be currently downloading.
    if (I != m_handles.end()) {
      m_handles.erase(I);
    } else {
      curl_multi_remove_handle(m_mhandle, ds->handle);
    }

    // Clean up curl handle
    curl_easy_cleanup(ds->handle);
    ds->handle = NULL;
  }

  // Clean up file pointer
  if (ds->fp) {
    os_free_tmpfile(ds->fp);
    ds->fp = NULL;
  }

  // Trigger user feedback
  DownloadFailed.emit(ds->url, ds->filename, "Aborted");
}

} /* namespace WFUT */
