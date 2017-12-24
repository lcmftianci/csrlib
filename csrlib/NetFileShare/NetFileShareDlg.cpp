
// NetFileShareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetFileShare.h"
#include "NetFileShareDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetFileShareDlg dialog



CNetFileShareDlg::CNetFileShareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NETFILESHARE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetFileShareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetFileShareDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, &CNetFileShareDlg::OnBnClickedBtnSend)
	ON_MESSAGE(WM_RECVDATA, &CNetFileShareDlg::OnRecvData)
	ON_BN_CLICKED(IDC_BTN_SENDFILE, &CNetFileShareDlg::OnBnClickedBtnSendfile)
END_MESSAGE_MAP()


// CNetFileShareDlg message handlers

BOOL CNetFileShareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_csrnet.csrudpinitsocket(6000);
	RecvData struData;
	struData.hwnd = m_hWnd;
	struData.sock = m_csrnet.GetSocket();
	m_csrnet.csrudpgetmessage(&struData);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNetFileShareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetFileShareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNetFileShareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CNetFileShareDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
	CString strData = (char*)lParam;
	CString strTmp;
	GetDlgItemText(IDC_EDIT_GET, strTmp);
	strData += "\r\n";
	strData += strTmp;
	SetDlgItemText(IDC_EDIT_GET, strData);
	return S_OK;
}

void CNetFileShareDlg::OnBnClickedBtnSend()
{
	// TODO: Add your control notification handler code here
	DWORD dwIP;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIP);
	CString strSend;
	GetDlgItemText(IDC_BTN_SEND, strSend);
	m_csrnet.csrudpsendmessage(dwIP, strSend, 6000);
	SetDlgItemText(IDC_BTN_SEND, "");
}


void CNetFileShareDlg::OnBnClickedBtnSendfile()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL/*"*.*"*/, /*""*/NULL, OFN_HIDEREADONLY/*|OFN_OVERWRITEPROMPT*/, "文本文档(*.txt)|*.txt|PDF文档(*.pdf)|*.pdf|所有文件(*.*)|*.*||"/*"图片文件(*.jpg)|(*.jpg)|所有文件(*.*)|(*.*)||"*/);
	dlg.DoModal();
}
