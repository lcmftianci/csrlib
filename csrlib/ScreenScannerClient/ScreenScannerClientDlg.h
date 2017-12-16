
// ScreenScannerClientDlg.h : header file
//

#pragma once

#include <gdiplus.h>


using namespace Gdiplus;

//自定义一个消息
#define CM_RECEIVED  WM_USER+2
//定义每个数据报中包含位图数据的大小
#define GraphSize    40960

// CScreenScannerClientDlg dialog
class CScreenScannerClientDlg : public CDialogEx
{
// Construction
public:
	CScreenScannerClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENSCANNERCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:
	int  SendData(UINT index, int mod, int bmpsize, int totalsize, int frames, char *pSendBuf, sockaddr_in &addr);
	afx_msg LRESULT OnReceived(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTimer(UINT nIDEvent);
	
private:
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;

	SOCKET  m_Socket;		//客户端套接字
	CString m_ServerIP;		//服务器IP
	BOOL	m_Confirm;		//确认信息
	int     m_FrameIndex;	//记录索引
	int     m_Mod;			//记录余数
	int     m_Count;		//每一个位图分报的数量
	int     m_Bmpsize;		//位图的实际大小
	char*	m_pSendBuf;		//发送缓冲区指针
	char*	m_pHeader;		//临时指针
	sockaddr_in  m_Addr;	//套接字地址
	BOOL    m_Received;		//是否接收到服务器发来的确认信息
	int     m_Counter;		//记数器
	DWORD	m_JpegSize;		//JPEG数据总大小
public:
	afx_msg void OnBnClickedBtnStart();
};
