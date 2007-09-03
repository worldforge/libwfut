%module WFUT

## TODO: Need to implement the callback functiosn somehow

## TODO: Need to check destructors, e.g. no FileMap destructor
##       Possibly a const related problem

## TODO: Need to auto de-init wfutclient objects

%{
#include <libwfut/types.h>
#include <libwfut/crc32.h>
#include <libwfut/ChannelFileList.h>
#include <libwfut/ChannelIO.h>
#include <libwfut/Encoder.h>
#include <libwfut/FileIO.h>
#include <libwfut/IO.h>
#include <libwfut/WFUT.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
%}

%import "std_string.i"
%import "std_map.i"
%import "std_list.i"
%import "std_vector.i"

%{
  void cb_download_complete(const std::string &s1, const std::string &s2, PyObject *f) {
    PyObject *args = Py_BuildValue("(ss)", s1.c_str(), s2.c_str());
    assert(args != 0);

    PyObject *r = PyObject_CallObject(f, args);
    if (r != 0) {
      Py_XDECREF(r);
    } else {
      // Failed -- Dont care
      // Maybe we should re-throw the exception?
    }

    Py_XDECREF(args);
  }

  void cb_download_failed(const std::string &s1, const std::string &s2, const std::string &s3, PyObject *f) {
    PyObject *args = Py_BuildValue("(sss)", s1.c_str(), s2.c_str(), s3.c_str());
    assert(args != 0);

    PyObject *r = PyObject_CallObject(f, args);
    if (r != 0) {
      Py_XDECREF(r);
    } else {
      // Failed -- Dont care
      // Maybe we should re-throw the exception?
    }

    Py_XDECREF(args);
  }
%}

## Need to map uLong to a python type
%typedef unsigned int uLong;

%include <libwfut/types.h>
%include <libwfut/crc32.h>
%include <libwfut/ChannelFileList.h>
%include <libwfut/ChannelIO.h>
%include <libwfut/Encoder.h>
%include <libwfut/FileIO.h>
%include <libwfut/IO.h>
%include <libwfut/WFUT.h>

%extend WFUT::WFUTClient {
  void WFUT::WFUTClient::DownloadCompleteCB(PyObject *f) {
    self->DownloadComplete.connect(sigc::bind(sigc::ptr_fun(cb_download_complete),f));
  }
  void WFUT::WFUTClient::DownloadFailedCB(PyObject *f) {
    self->DownloadFailed.connect(sigc::bind(sigc::ptr_fun(cb_download_failed),f));
  }
}
