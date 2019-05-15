// -*- C++ -*-
// --------------------------------------------------------------------
// IpePresenter
// --------------------------------------------------------------------
/*

    This file is part of the extensible drawing editor Ipe.
    Copyright (c) 1993-2019 Otfried Cheong

    Ipe is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, you have permission to link Ipe with the
    CGAL library and distribute executables, as long as you follow the
    requirements of the Gnu General Public License in regard to all of
    the software in the executable aside from CGAL.

    Ipe is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with Ipe; if not, you can find it at
    "http://www.gnu.org/copyleft/gpl.html", or write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef IPEPRESENTER_H
#define IPEPRESENTER_H

#include "ipepdfview.h"

using namespace ipe;

// --------------------------------------------------------------------

class Presenter {
public:
  enum TAction {
    ELeftMouse, EOtherMouse,
    EOpen, EQuit,
    EShowPresentation, EFullScreen,
    EZoomIn, EZoomOut,
    ESetTime, EToggleTimeCounting, ETimeCountdown,
    EResetTime,
    ENextView, EPreviousView, EFirstView, ELastView,
    ENextPage, EPreviousPage, EJumpTo, ESelectPage,
    EAbout,
  };

public:
  void nextView(int delta);
  void nextPage(int delta);
  void firstView();
  void lastView();

  void fitBox(const Rect &box, PdfViewBase *view);
  bool load(const char* fn);

  // use -1 for current and -2 for next
  Rect mediaBox(int pdfpno) const;

protected:
  void collectAnnotations();
  void makePageLabels();
  void collectPageLabels(const PdfDict *d);
  void setViewPage(PdfViewBase *view, int pdfpno);
  String pageLabel(int pdfno);
  String currentLabel();
  void jumpToPage(String page);

protected:
  std::unique_ptr<PdfFile> iPdf;

  String iFileName;
  int iPdfPageNo;
  std::vector<String> iAnnotations;
  std::vector<std::pair<String,int>> iPageLabels;
};

// --------------------------------------------------------------------
#endif
