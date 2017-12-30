
// ScrScClientDlg.h : header file
//

#pragma once

#include <gdiplus.h>

using namespace Gdiplus;

#define WM_TRAYMESSAGE WM_USER + 2000
#define PICPACKSIZE 65000
#define MAX_BUFFER 65535

struct UDPPACKAGE
{
	int iIndex;
	DWORD JpegSize;
	int buffersize;
	bool bFinish;
	int PageCount;//传递多少次数据报
	char buffer[65000];
};


// CScrScClientDlg dialog
class CScrScClientDlg : public CDialogEx
{
// Construction
public:
	CScrScClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRSCCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CMenu m_Menu;
	BOOL bHide;
	int m_JpegSize;
	ULONG_PTR m_pGdiTocken;
	GdiplusStartupInput m_gdiplusStartupInput;

	//网络操作
	SOCKET m_Socket;
	int m_Counter;
	sockaddr_in m_addrClient;
	int m_bHide;

public:
	//托盘服务
	NOTIFYICONDATA m_IconData;				// 定义系统托盘结构变量
	afx_msg LRESULT OnTrayMessage(WPARAM wParam, LPARAM lParam);
	void GetPictureData();

	//网络相关
	int SendData(int totalsize, char* pSendBuf);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL DestroyWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuHelp();
	afx_msg void OnMenuExit();
	afx_msg void OnDestroy();
};
