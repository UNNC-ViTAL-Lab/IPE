# --------------------------------------------------------------------
# Makefile for IpePresenter
# --------------------------------------------------------------------

OBJDIR = $(BUILDDIR)/obj/ipepresenter
include ../common.mak

TARGET = $(call exe_target,ipepresenter)

CPPFLAGS += -I../include -I../ipecanvas -I../ipecairo \
	$(UI_CFLAGS) $(CAIRO_CFLAGS) $(ZLIB_CFLAGS)
LIBS += -L$(buildlib) -lipecanvas -lipecairo -lipe \
	$(UI_LIBS) $(CAIRO_LIBS) $(ZLIB_LIBS)

# IpePresenter is not built for OSX
ifndef IPEUI_COCOA

all: $(TARGET)

sources = ipepresenter.cpp
win_sources = ipepresenter_win.cpp
qt_sources = ipepresenter_qt.cpp timelabel_qt.cpp
moc_headers = ipepresenter_qt.h timelabel_qt.h

ifdef WIN32
CXXFLAGS += -mwindows
LDFLAGS  += -mwindows

resource = $(OBJDIR)/res.o

$(resource): ipepresenter.rc
	$(WINDRES) -i ipepresenter.rc -o $@ --include-dir=.

else
resource =
endif

$(TARGET): $(objects) $(resource) $(nib)
	$(MAKE_BINDIR)
	$(CXX) $(LDFLAGS) -o $@ $(objects) $(resource) $(LIBS)

clean:
	@-rm -f $(objects) $(resource) $(TARGET) $(DEPEND)

$(DEPEND): Makefile
	$(MAKE_DEPEND)

-include $(DEPEND)

install: $(TARGET)
	$(INSTALL_DIR) $(INSTALL_ROOT)$(IPEBINDIR)
	$(INSTALL_PROGRAMS) $(TARGET) $(INSTALL_ROOT)$(IPEBINDIR)

# --------------------------------------------------------------------
endif
