
// NotepadView.cpp : implementation of the CNotepadView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Notepad.h"
#endif

#include "NotepadDoc.h"
#include "NotepadView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNotepadView

IMPLEMENT_DYNCREATE(CNotepadView, CView)

BEGIN_MESSAGE_MAP(CNotepadView, CView)
	// Standard printing commands

	ON_WM_PAINT()
	//ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//ON_COMMAND()

	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CNotepadView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CNotepadView construction/destruction

CNotepadView::CNotepadView()
{
	// TODO: add construction code here

}

CNotepadView::~CNotepadView()
{
}

BOOL CNotepadView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CNotepadView drawing

void CNotepadView::OnDraw(CDC* /*pDC*/)
{
	CNotepadDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CNotepadView printing


void CNotepadView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CNotepadView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNotepadView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNotepadView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CNotepadView::OnPaint()
{

}

void CNotepadView::OnCreate(LPCREATESTRUCT lpcs)
{

}

void CNotepadView::OnSize(UINT nType, int cx, int cy)
{

}

void CNotepadView::OnFileNew()
{

}

void CNotepadView::OnSetFocus(CWnd* pOldWnd)
{

}

void CNotepadView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNotepadView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CNotepadView diagnostics

#ifdef _DEBUG
void CNotepadView::AssertValid() const
{
	CView::AssertValid();
}

void CNotepadView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNotepadDoc* CNotepadView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNotepadDoc)));
	return (CNotepadDoc*)m_pDocument;
}
#endif //_DEBUG


// CNotepadView message handlers
