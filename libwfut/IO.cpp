// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 Simon Goodall

#include "IO.h"

static size_t write_data(void *buffer, size_t size, size_t nmemb,void *userp)  {
  DataStruct *ds = (DataStruct*)userp;
  if (ds->fp == NULL) {
    ds->fp = fopen(ds->filename.c_str(), "wb");
  }
  assert(ds->fp != NULL);
  return fwrite(buffer, size, nmemb, ds->fp);
}

int IO::init() {
  assert (m_initialised == false);
  curl_global_init();
  m_mhandle = curl_multi_init();
  m_initialised = true;
}

int IO::shutdown(){
  assert (m_initialised == true);

  curl_multi_cleanup(m_mhandle);
  m_mhandle = NULL;

  while (!m_files.empty()) {
    DataStruct *ds = m_files.begin()->first;
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

  m_initialised = false;;
}

int IO::queueFile(const std::string &filename, const std::string &url) {
  if (m_files.find(url) != m_files.end()) {
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
  curl_multi_add_handle(m_mhandle, ds->handle);

  return 0;
}

int IO::poll() {
  int num_handles;
  curl_multi_perform(m_mhandle, &num_handles);

  struct CURLMsg *msg = NULL;
  int msgs;

  while ((msg = curl_mutli_info_read(m_mhandle, &msgs)) != NULL) {
    switch (msg->msg) {
      case CURLMSG_DONE:
        // close file etc..
        break;
      default:
        //unhandled case
    }
  }

  return num_handles;
}
