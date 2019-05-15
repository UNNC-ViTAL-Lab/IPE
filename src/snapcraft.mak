# -*- makefile -*-
# --------------------------------------------------------------------
#
# Ipe configuration for building an AppImage (http://appimage.org)
# or a snap (snapcraft.io)
#
IPEBUNDLE  = 1
#
ifdef IPEAPPIMAGE
CPPFLAGS     += -DIPEAPPIMAGE
else
CPPFLAGS     += -DIPESNAPCRAFT
IPEPREFIX    := $(IPESRCDIR)/../../install
endif
#
ZLIB_CFLAGS   ?=
ZLIB_LIBS     ?= -lz
JPEG_CFLAGS   ?=
JPEG_LIBS     ?= -ljpeg
PNG_CFLAGS    ?= $(shell pkg-config --cflags libpng)
PNG_LIBS      ?= $(shell pkg-config --libs libpng)
FREETYPE_CFLAGS ?= $(shell pkg-config --cflags freetype2)
FREETYPE_LIBS ?= $(shell pkg-config --libs freetype2)
CAIRO_CFLAGS  ?= $(shell pkg-config --cflags cairo)
CAIRO_LIBS    ?= $(shell pkg-config --libs cairo)
LUA_CFLAGS    ?= $(shell pkg-config --cflags lua5.3)
LUA_LIBS      ?= $(shell pkg-config --libs lua5.3)
QT_CFLAGS     ?= $(shell pkg-config --cflags Qt5Gui Qt5Widgets Qt5Core)
QT_LIBS	      ?= $(shell pkg-config --libs Qt5Gui Qt5Widgets Qt5Core)
DL_LIBS       ?= -ldl
MOC	      ?= moc
CXX = g++
DLL_CFLAGS = -fPIC
#
# --------------------------------------------------------------------
#
IPEVERS = 7.2.12
#
IPEBINDIR  = $(IPEPREFIX)/bin
IPELIBDIR  = $(IPEPREFIX)/lib
IPEHEADERDIR = $(IPEPREFIX)/include/ipe
IPELETDIR = $(IPEPREFIX)/ipe/ipelets
IPELUADIR = $(IPEPREFIX)/ipe/lua
IPESCRIPTDIR = $(IPEPREFIX)/ipe/scripts
IPESTYLEDIR = $(IPEPREFIX)/ipe/styles
IPEICONDIR = $(IPEPREFIX)/ipe/icons
IPEDOCDIR = $(IPEPREFIX)/ipe/doc
IPEMANDIR = $(IPEPREFIX)/man
#
# --------------------------------------------------------------------
