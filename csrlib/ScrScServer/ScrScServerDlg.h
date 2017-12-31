
// ScrScServerDlg.h : header file
//

#pragma once

#define MAX_BUFFER 65535
#define PICPACKSIZE 65000
#include <Gdiplus.h>

using namespace Gdiplus;

#define CM_RECEIVED  WM_USER + 1001

struct UDPPACKAGE 
{
	int iIndex;
	DWORD JpegSize;
	int buffersize;
	bool bFinish;
	int PageCount;
	char buffer[65000];
};


// CScrScServerDlg dialog
class CScrScServerDlg : public CDialogEx
{
// Construction
public:
	CScrScServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRSCSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	ULONG_PTR			m_pGdiToken;
	GdiplusStartupInput m_gdiplusinput;
	SOCKET				m_Socket;
	BOOL				m_IsScreen;
	WINDOWPLACEMENT     m_OldPlacement;
	CRect				m_FullScreenRect;
	IStream*			m_pStm;
	Bitmap*				m_pNewBmp;
	sockaddr_in			m_addrServ;
	int					m_RecvCount;
	char				m_tmp[650000];

public:
	afx_msg LRESULT OnReceived(WPARAM wParam, LPARAM lParam);
	void ShowScreen(int DataSize, void* pData);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL DestroyWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
