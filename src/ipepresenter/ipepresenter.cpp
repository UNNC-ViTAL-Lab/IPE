// --------------------------------------------------------------------
// IpePresenter common base
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

#include "ipepresenter.h"

// --------------------------------------------------------------------

bool Presenter::load(const char *fname)
{
  std::FILE *pdfFile = Platform::fopen(fname, "rb");
  if (!pdfFile)
    return false;

  FileSource source(pdfFile);
  std::unique_ptr<PdfFile> pdf = std::make_unique<PdfFile>();
  bool okay = pdf->parse(source);
  std::fclose(pdfFile);

  if (!okay)
    return false;

  iPdf = std::move(pdf);

  iFileName = fname;
  iPdfPageNo = 0;

  collectAnnotations();
  makePageLabels();
  return true;
}

// read annotations from PDF
void Presenter::collectAnnotations()
{
  iAnnotations.clear();
  for (int i = 0; i < iPdf->countPages(); ++i) {
    String notes;
    const PdfDict *page = iPdf->page(i);
    const PdfObj *annots = page->get("Annots", iPdf.get());
    if (annots && annots->array()) {
      for (int j = 0; j < annots->array()->count(); ++j) {
	const PdfObj *a = annots->array()->obj(j, iPdf.get());
	if (a && a->dict()) {
	  const PdfObj *type = a->dict()->get("Type", iPdf.get());
	  const PdfObj *subtype = a->dict()->get("Subtype", iPdf.get());
	  const PdfObj *contents = a->dict()->get("Contents", iPdf.get());
	  if (type && type->name() && type->name()->value() == "Annot" &&
	      subtype && subtype->name() && subtype->name()->value() == "Text" &&
	      contents && contents->string()) {
	    if (!notes.empty())
	      notes += "\n";
	    notes += contents->string()->value();
	  }
	}
      }
    }
    iAnnotations.push_back(notes);
  }
}

// create the page labels
void Presenter::makePageLabels()
{
  iPageLabels.clear();
  const PdfObj *d1 = iPdf->catalog()->get("PageLabels", iPdf.get());
  if (d1 && d1->dict()) {
    collectPageLabels(d1->dict());
  } else {
    for (int pno = 0; pno < iPdf->countPages(); ++pno) {
      char buf[16];
      sprintf(buf, "%d", pno + 1);
      iPageLabels.push_back(std::make_pair(String(buf), -1));
    }
  }
}

// this is not a complete implementation,
// just meant to work for beamer output and Ipe
void Presenter::collectPageLabels(const PdfDict *d)
{
  const PdfObj *nums = d->get("Nums", iPdf.get());
  if (nums && nums->array()) {
    int prevNum = 0;
    String prevLabel;
    for (int j = 0; j < nums->array()->count() - 1; j += 2) {
      const PdfObj *num = nums->array()->obj(j, iPdf.get());
      const PdfObj *label = nums->array()->obj(j + 1, iPdf.get());
      if (num->number() && label->dict()) {
	int newNum = int(num->number()->value());
	const PdfObj *p = label->dict()->get("P", iPdf.get());
	String newLabel;
	if (p && p->string())
	  newLabel = p->string()->value();
	bool moreThanOne = (newNum - prevNum) > 1;
	while (size(iPageLabels) < newNum)
	  iPageLabels.push_back(std::make_pair(prevLabel, moreThanOne ?
					       iPageLabels.size() - prevNum : -1));
	prevNum = newNum;
	prevLabel = newLabel;
      }
    }
    bool moreThanOne = (iPdf->countPages() - iPageLabels.size()) > 1;
    while (size(iPageLabels) < iPdf->countPages())
      iPageLabels.push_back(std::make_pair(prevLabel, moreThanOne ?
					   iPageLabels.size() - prevNum : -1));
  }
}

// --------------------------------------------------------------------

void Presenter::setViewPage(PdfViewBase *view, int pdfpno)
{
  view->setPage(iPdf->page(pdfpno), mediaBox(pdfpno));
  view->updatePdf();
}

void Presenter::fitBox(const Rect &box, PdfViewBase *view)
{
  if (box.isEmpty())
    return;
  double xfactor = box.width() > 0.0  ? (view->viewWidth() / box.width()) : 20.0;
  double yfactor = box.height() > 0.0 ? (view->viewHeight() / box.height()) : 20.0;
  double zoom = (xfactor > yfactor) ? yfactor : xfactor;
  view->setPan(0.5 * (box.bottomLeft() + box.topRight()));
  view->setZoom(zoom);
  view->updatePdf();
}

// --------------------------------------------------------------------

String Presenter::pageLabel(int pdfno) {
  auto & pl = iPageLabels[pdfno];
  String s;
  ipe::StringStream ss(s);
  ss << pl.first;
  if (pl.second >= 0) {
    if (pl.first.right(1) != "-")
      ss << "-";
    ss << pl.second + 1;
  }
  return s;
}

String Presenter::currentLabel()
{
  String s = iFileName;
  if (iFileName.rfind('/') >= 0)
    s = iFileName.substr(iFileName.rfind('/') + 1);
  ipe::StringStream ss(s);
  ss << " : " << pageLabel(iPdfPageNo)
     << " / " << iPageLabels.back().first
     << " (" << iPdfPageNo + 1 << " / " << iPdf->countPages() << ")";
  return s;
}

Rect Presenter::mediaBox(int pdfpno) const
{
  if (pdfpno == -1)
    pdfpno = iPdfPageNo;
  else if (pdfpno == -2)
    pdfpno = (iPdfPageNo < iPdf->countPages() - 1) ? iPdfPageNo + 1 : iPdfPageNo;
  return iPdf->mediaBox(iPdf->page(pdfpno));
}

// --------------------------------------------------------------------

void Presenter::jumpToPage(String page)
{
  if (page.empty())
    return;
  for (int i = 0; i < size(iPageLabels); ++i) {
    auto pl = pageLabel(i);
    if (page == pl || (page + "-1" == pl) || page == (pl + "-1")) {
      iPdfPageNo = i;
      return;
    }
  }
}

void Presenter::nextView(int delta)
{
  int npno = iPdfPageNo + delta;
  if (0 <= npno && npno < iPdf->countPages()) {
    iPdfPageNo = npno;
  }
}

void Presenter::nextPage(int delta)
{
  String now = iPageLabels[iPdfPageNo].first;
  while (iPageLabels[iPdfPageNo].first == now &&
	 0 <= iPdfPageNo + delta && iPdfPageNo + delta < iPdf->countPages())
    iPdfPageNo += delta;
  if (delta < 0) {
    // go back to first view of the same page
    String cur = iPageLabels[iPdfPageNo].first;
    while (0 < iPdfPageNo && iPageLabels[iPdfPageNo-1].first == cur)
      iPdfPageNo += delta;
  }
}

void Presenter::firstView()
{
  iPdfPageNo = 0;
}

void Presenter::lastView()
{
  iPdfPageNo = iPdf->countPages() - 1;
}

// --------------------------------------------------------------------
