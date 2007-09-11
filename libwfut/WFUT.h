// This file may be redistributed and modified only under the terms of
// the GNU Lesser General Public License (See COPYING for details).
// Copyright (C) 2005 - 2007 Simon Goodall

#ifndef LIBWFUT_WFUT_H
#define LIBWFUT_WFUT_H 1

#include <string>
#include <cassert>

#include <sigc++/signal.h>

#include <libwfut/types.h>
#include <libwfut/ChannelFileList.h>

namespace WFUT {

class IO;

/** This enum lists the possible return values of the WFUTClient class.
 */
typedef enum {
  WFUT_NO_ERROR = 0,   // No error occured. Success
  WFUT_GENERAL_ERROR,  // A general error happened. Nothing specific case.
  WFUT_DOWNLOAD_ERROR, // An error happened during download.
  WFUT_PARSE_ERROR,    // An error happened during the parsing of a file
  WFUT_WRITE_ERROR     // An error happened trying to write a file.
} WFUTError;

typedef enum {
  WFUT_UPDATE_NONE = 0,      // No update required
  WFUT_UPDATE_NO_LOCAL,      // No local version of the file
  WFUT_UPDATE_SERVER_SYSTEM, // Server copy is newer than system version
  WFUT_UPDATE_SERVER_LOCAL,  // Server copy is newer than local version
  WFUT_UPDATE_MISSING,       // Local copy is missing
  WFUT_UPDATE_MODIFIED       // Local copy is modified
} WFUTUpdateReason;

/** The WFUTClient class should be the main class that client applications use.
 * It wraps up the underlying function calls. The IO class may be of more 
 * interest to those looking for more direct control of downloads. The channel
 * listings and channel file lists arae downloaded as a blocking function. It
 * is expected that such files are quick to download. The main files to
 * download are processed in chunks and progress through repeated calls to the
 * poll function. 
 * It is intended that for a given channel, there will be a system version.
 * These will be in a read-only location and may have been installed as part of
 * a system packge. There will be a user writable location, referred to as the
 * local version. The server version is the file stored on the remote web
 * server.
 */

class WFUTClient {
public:
  WFUTClient():
    m_initialised(false),
    m_io(NULL)
  {}
  virtual ~WFUTClient() { 
    if (m_initialised) shutdown(); 
  }

  /** Initialise the WFUT object. This in turn will call IO::init which will 
   * initialise curl.
   */
  WFUTError init();

  /** Clean up the WFUT object. This will in turn call IO::shutdown which will
   * cleanup curl
   */
  WFUTError shutdown();

  /** This function queues a list of files to download. The urlPrefix is 
   * prefixed to the FileObject filename to find the server side version. The
   * pathPrefix is prefixed to the FileObject filename to determine the client
   * side file location. The poll function must be called to perform the 
   * downloading and the DownloadComplete and DownloadFailed signals can be used
   * to track the result of each file download.
   * The updates channel name is appended to the urlPrefix. 
   */
  void updateChannel(const ChannelFileList &updates, 
                     const std::string &urlPrefix,
                     const std::string &pathPrefix);

  /** This function is similar to updateChannel, except it takes a single 
   * FileObject, rather than a whole list.
   */
  void updateFile(const FileObject &file, 
                  const std::string &urlPrefix,
                  const std::string &pathPrefix);

  /** This function retrieves a list of mirrors from the given url.
   * These mirrors will be the root url.
   */
  WFUTError getMirrorList(const std::string &url, MirrorList &mirrors);

  /** This function directly download and parses a channel listings file from 
   * the specified URL. This function will block until complete. This is the
   * file that lists all the channels that are available at a that location.
   */
  WFUTError getChannelList(const std::string &url, ChannelList &channels);

  /** This function will download a channel file from the specified URL. This
   * function will block until complete. This is the wfut.xml file.
   */
  WFUTError getFileList(const std::string &url, ChannelFileList &files);

  /** This function will parse a local channel file from the given filename	   * This can be used for both the local and system file lists.
   */
  WFUTError getLocalList(const std::string &filename, ChannelFileList &files);

  /** This function will save a channel file list to the given filename. This
   * should be called once all files have finished downloading to create the
   * local file list.
   */
  WFUTError saveLocalList(const ChannelFileList &files,
                          const std::string &filename);

  /** This function takes three sets of channel file lists and calculates which
   * files need updating. The server list is the server side file list. The
   * system list is intended for data stored in a read only location. This may
   * be empty i no such location exists. The local list is the list of files
   * stored in a writable location. This may be empty if no files have yet been
   * downloaded. The updates list is the list of FileObjects in the server list
   * that require downloading. The prefix argument is the path prefix to the 
   * local files so that the CRC32 value maybe checked. This process also
   * doubles up to check to see if local files have been deleted and need 
   * re-downloading.
   */
  WFUTError calculateUpdates(const ChannelFileList &server,
                             const ChannelFileList &system,
                             const ChannelFileList &local,
                             ChannelFileList &updates,
                             const std::string &prefix);


  /** The poll function is used to perform a chunk of downloading. This means
   * that it needs to be called frequently. It returns the number of files still
   * being downloaded. It will return 0 when all files have finished
   * downloading.
   */
  int poll();

   /** The DownloadComplete signal is fired every time a file is successfully 
    * downloaded. The first argument is the source url and the second argument
    * is the filename from the FileObject.
    */
  sigc::signal<void, const std::string&, const std::string&> DownloadComplete;

   /** The DownloadFailed signal is fired when there was a problem downloading 
    * a file. This could happen due to a broken url, or a problem saving the
    * file in the temp location, or copying it from the tmp location to the
    * destination. The first argument is the source url and the second argument
    * is the filename from the FileObject. The third argument is a message 
    * indicating a general reason why the download failed.
    */
  sigc::signal<void, const std::string&, const std::string&, const std::string&> DownloadFailed;

  /** The UpdateReason signal is fired every time a FileObject is processed
   * in the calculateUpdates function. If WFUT_UPDATE_MODIFIED is passed as a
   * reason code, then this file has not been added to the updates list because
   * there are modifications to the local file. If an update is required, this
   * file can be manually added to the updates list.
   */
  sigc::signal<void, const std::string&, const WFUTUpdateReason> UpdateReason;

private:
  void onDownloadComplete(const std::string &url, const std::string &filename);
  void onDownloadFailed(const std::string &url, const std::string &filename, const std::string &reason);

  bool m_initialised;
  IO *m_io;
};

} /* namespace WFUT */

#endif /* LIBWFUT_WFUT_H */
