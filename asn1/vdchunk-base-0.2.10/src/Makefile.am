
EXTRA_DIST = docu.cpp

lib_LTLIBRARIES = libvdchunk-base.la
libvdchunk_base_la_SOURCES = vdchunk-simple.cpp vdchunk-base.cpp lhexlib.cpp lhexlib.h
include_HEADERS = vdchunk-base.h vdchunk-simple.h lhexlib.h

bin_PROGRAMS = vdctest
vdctest_SOURCES = vdctest.cpp
vdctest_LDADD = libvdchunk-base.la
