
// ScreenScannerServerDlg.h : header file
//

#pragma once

#include <gdiplus.h>

//命名空间
using namespace Gdiplus;

//自定义消息
#define CM_RECEIVED  WM_USER+1001

#pragma comment(lib, "ws2_32.lib")

// CScreenScannerServerDlg dialog
class CScreenScannerServerDlg : public CDialogEx
{
// Construction
public:
	CScreenScannerServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENSCANNERSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

public://自定义函数
	void ShowJPEG(void* pData, int DataSize);
	afx_msg LRESULT OnReceived(WPARAM wParam, LPARAM lParam);

private://通讯相关
	SOCKET  m_Socket;					//定义套接字
	CString m_ClientIP;					//客户端IP
	UINT    m_ClientPort;				//客户端端口
	
private://截屏相关
	char*	m_BmpData;					//位图数据指针
	char*   m_Header;					//临时指针
	BOOL    m_ShowBmp;					//是否显示位图
	char*   m_TempData;					//临时数据指针
	int     m_BmpSize;					//记录位图大小
	int     m_RecSize;					//实际接收的大小
	DWORD   m_JPGSize;					//JPG数据的总大小
	BOOL    m_IsScreen;					//是否全屏显示
	WINDOWPLACEMENT m_OldPlacement;		//原来的窗口模式
	CRect m_FullScreenRect;				//全屏显示区域
	IStream *m_pStm;
	Bitmap  *m_pNewBmp;
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
public:
	//afx_msg void OnBnClickedStart();
};
