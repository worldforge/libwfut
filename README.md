# libwfut

[![Join us on Gitter!](https://badges.gitter.im/Worldforge.svg)](https://gitter.im/Worldforge/Lobby)
[![Appveyor build status](https://ci.appveyor.com/api/projects/status/github/worldforge/libwfut?branch=master&svg=true)](https://ci.appveyor.com/project/erikogenvik/libwfut)
[![Travis build status](https://travis-ci.com/worldforge/libwfut.svg?branch=master)](https://travis-ci.com/worldforge/libwfut)
[![Github build status](https://github.com/worldforge/libwfut/actions/workflows/cmake.yml/badge.svg)](https://github.com/worldforge/libwfut/actions/workflows/cmake.yml)

libwfut is a C++ implementation of the client side of the 
[WorldForge](http://worldforge.org/ "The main Worldforge site") Update Tool (WFUT).
The aim of this library is to provide update capabilities for C++ based
clients. As an example of library usage, a command line update tool is also
provided.

## Installation

If you intend to build this as a prerequisite for the Ember client or the Cyphesis server we strongly suggest that you 
use the [Hammer](http://wiki.worldforge.org/wiki/Hammer_Script "The Hammer script") tool to compile it.
This is script provided by the Worldforge project which will download and install all of the required libraries and 
components used by Worldforge.

Alternatively you can use [Conan](https://www.conan.io) to install all dependencies. 
```bash
conan remote add worldforge https://artifactory.ogenvik.org/artifactory/api/conan/conan
mkdir build && cd build
conan install ../tools/conan --build missing
cmake ..
make -j all install
```

Otherwise the library can most easily be built through the following commands.
```bash
mkdir build && cd build
cmake ..
make -j all install
```

### Tests

The test suite can be built and run using the ```check``` target. For example:

```bash
make check
```

### API documentation

If Doxygen is available API documentation can be generated using the ```dox``` target. For example:

```bash
make dox
```

## Dependencies

This library used tinyxml (http://www.grinninglizard.com/tinyxml/) to parse the
XML documents and libcurl (http://curl.haxx.se/) to download files.
Tinyxml is embedded in the library, with the source being kept in the tinyxml
subdir. The only difference between this version and the original version is
the addition of the WFUT namespace to avoid potential conflicts with other
embedded tinyxml versions.

libwfut is split up into a number of classes. There are a number of XML reading
and writing classes (FileParser, FileWriter and ChannelParser). These perform
the basic I/O operations on the file listings. The IO class handles all of the
file downloading. This wraps round the curl library. The final main class is
the WFUTClient class which wraps up the other classes and manages determining
what files to update.

libwfut allows two modes of file download. The first mode is a blocking
download. A call to WFUTClient::downloadFile with a source URL and destination
filename.This function will return when the download completes successfully, or
fails for some reason.

The second mode places multiple files in a download queue. The queue is
processed a bit at a time  by calling the poll function. Signals are fired when
a download completes successfully or fails for some reason.

## How to help

If you're interested in helping out with development you should check out these resources:

* [The main Worldforge site](http://worldforge.org/ "The main Worldforge site")
* [Bugs and feature planning on Launchpad](https://launchpad.net/libwfut "libwfut Launchpad entry")
* [Gitter conversation](https://gitter.im/Worldforge/Lobby "Gitter conversation")
* [Mailing lists](http://mail.worldforge.org/lists/listinfo/ "Mailing lists")
