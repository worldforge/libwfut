// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2006 Simon Goodall

#include "libwfut/IO.h"

namespace WFUT {

static const bool debug = false;

// Callback function to write downloaded data to a file.
static size_t write_data(void *buffer, size_t size, size_t nmemb,void *userp) {
  assert(userp != NULL);
  IO::DataStruct *ds = reinterpret_cast<IO::DataStruct*>(userp);
  if (ds->fp == NULL) {
    ds->fp = fopen(ds->filename.c_str(), "wb");
  }
  // TODO replace this assert with some error handler
  assert(ds->fp != NULL);
  return fwrite(buffer, size, nmemb, ds->fp);
}

int IO::init() {
  assert (m_initialised == false);
  curl_global_init(CURL_GLOBAL_ALL);
  m_mhandle = curl_multi_init();
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

int IO::queueFile(const std::string &filename, const std::string &url) {
  if (m_files.find(url) != m_files.end()) {
    fprintf(stderr, "Error file is alreay in queue\n");
    // Url already in queue
    return 1;
  }
  DataStruct *ds = new DataStruct();
  ds->fp = NULL;
  ds->url = url;
  ds->filename = filename;
  ds->handle = curl_easy_init();

  m_files[url] = ds;

  curl_easy_setopt(ds->handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(ds->handle, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(ds->handle, CURLOPT_WRITEDATA, ds);
  curl_easy_setopt(ds->handle, CURLOPT_PRIVATE, ds);
  curl_multi_add_handle(m_mhandle, ds->handle);

  return 0;
}

int IO::poll() {
  int num_handles;
  curl_multi_perform(m_mhandle, &num_handles);

  struct CURLMsg *msg = NULL;
  int msgs;

  while ((msg = curl_multi_info_read(m_mhandle, &msgs)) != NULL) {

    DataStruct *ds = NULL;
    int err = curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &ds);
    if (err  != CURLE_OK) {
      // Do something on error
      fprintf(stderr, "Got some error on curl_easy_getinfo (%d)\n", err);
    }

    bool failed = true; 
    std::string errormsg = "Unknown failure";
    switch (msg->msg) {
      case CURLMSG_DONE: {
        if (msg->data.result == CURLE_OK) {
          failed = false;
        } else {
          failed = true;
          errormsg = "There was an error downloading the requested file";
        }
        break;
      }
      default:
        failed = true;
        errormsg = "There was an unknown error downloading the requested file";
    }
    if (ds) {
      if (ds->fp) fclose(ds->fp);
      ds->fp = NULL;
      if (failed) {
        if (debug) printf("Download Failed\n");
        DownloadFailed.emit(ds->url, ds->filename);
      } else {
        if (debug) printf("Download Complete\n");
        DownloadComplete.emit(ds->url, ds->filename);
      }
      m_files.erase(m_files.find(ds->url));
      delete ds;
    }
    if (debug) printf("Removing Handle\n");
    // Close handle	
    curl_multi_remove_handle(m_mhandle, msg->easy_handle);
  }

  return num_handles;
}

} /* namespace WFUT */
