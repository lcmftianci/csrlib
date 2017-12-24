
// NetFileShareDlg.h : header file
//

#pragma once
#include "csrnet.h"


// CNetFileShareDlg dialog
class CNetFileShareDlg : public CDialogEx
{
// Construction
public:
	CNetFileShareDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETFILESHARE_DIALOG };
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

	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	csrudpnet m_csrnet;
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnSendfile();
};
