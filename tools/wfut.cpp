// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#include <getopt.h>

#include <sigc++/bind.h>

#include <libwfut/WFUT.h>

using namespace WFUT;

static struct option long_options [] =
{
  { "update", 1, 0, 'u' },
  { "version", 0, 0, 'v' },

};

static char short_options [] = "uv";

static bool file_exists(const std::string &filename) {
  FILE * fp = fopen(filename.c_str(), "r");
  if (fp == 0) return false;
  fclose(fp);
  return true;

}

void onDownloadComplete(const std::string &u, const std::string &f, const ChannelFileList &updates, ChannelFileList &local)  {
  printf("Downloaded %s\n", f.c_str());
//  FileObject &fo = updates.getFiles().find(f);
}


void onDownloadFailed(const std::string &u, const std::string &f, const std::string &r)  {
  fprintf(stderr,"Error downloading %s\n", u.c_str());
}


int main(int argc, char *argv[]) {

  std::string server_root = "http://white.worldforge.org/downloads/WFUT/";
  std::string channel_file = "wfut.xml";

  std::string channel = ".";
  std::string local_path = "./";
  std::string system_path = "";
 
  while (true) {
    int opt_index = 0;
    int c = getopt_long(argc, argv, short_options, long_options, &opt_index);
    if (c == -1) break;

    switch (c) {
      case 0:
        printf("You should not see this message\n");
        break;
      case 'v':
        printf("WFUT Version: %s\n", "No Version");
        break;
      case 'u':
        if (optarg) {
          channel = optarg;
        } else {
          printf("Missing channel name\n");
        }
        break;
      default:
        printf("Unknown command %c\n", c);
    }
  }

  printf("Channel: %s\n", channel.c_str());

  std::string local_wfut  = local_path  + "/" + channel + "/" + channel_file;
  std::string system_wfut = system_path + "/" + channel + "/" + channel_file; 
  std::string server_wfut = server_root + "/" + channel + "/" + channel_file; 

  std::string local_root = local_path + "/" + channel + "/";

  printf("Local wfut: %s\n", local_wfut.c_str());
  printf("System wfut: %s\n", system_wfut.c_str());
  printf("Server wfut: %s\n", server_wfut.c_str());
  printf("Local Root: %s\n", local_root.c_str());

  WFUTClient wfut;
  wfut.init();




  ChannelFileList local, system, server, updates;

  if (file_exists(local_wfut)) {
    if (wfut.getLocalList(local_wfut, local)) {
      printf("Error reading local wfut.xml file\n");
    } else {
      
    }
  }

  if (wfut.getFileList(server_wfut, server)) {

  }

  if (wfut.calculateUpdates(server, system, local, updates, local_root)) {
    printf("Error during update\n");
    wfut.shutdown();
    return 1;
  }
  wfut.DownloadComplete.connect(sigc::bind(sigc::ptr_fun(onDownloadComplete), updates, local));
  wfut.DownloadFailed.connect(sigc::ptr_fun(onDownloadFailed));

  wfut.updateChannel(updates, server_root + "/" + channel, local_root);

  // Do the downloads
  while (wfut.poll());

  // Save the completed download list
  // TODO: This only saves the list of files updates.
  // It is not savinig all files, nor does it take into account failed files!.
  wfut.saveLocalList(updates, local_wfut);

  wfut.shutdown();


  return 0;

}
