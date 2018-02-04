
// NotepadView.h : interface of the CNotepadView class
//

#pragma once


class CNotepadView : public CView
{
protected: // create from serialization only
	CNotepadView();
	DECLARE_DYNCREATE(CNotepadView)

// Attributes
public:
	CNotepadDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	
	CEdit   m_wndEdit;
	afx_msg void OnPaint();
	BOOL	IsTextSelected();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditDelete();
	afx_msg void OnEditUndo();

	afx_msg void OnUpdateEditCut();
	afx_msg void OnUpdateEditCopy();
	afx_msg void OnUpdateEditPaste();
	afx_msg void OnUpdateEditDelete();
	afx_msg void OnUpdateEditUndo();

	afx_msg void OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileNew();
	afx_msg void OnSetFocus(CWnd* pOldWnd);


// Implementation
public:
	virtual ~CNotepadView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NotepadView.cpp
inline CNotepadDoc* CNotepadView::GetDocument() const
   { return reinterpret_cast<CNotepadDoc*>(m_pDocument); }
#endif

