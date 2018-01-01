
// ScrScServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScrScServer.h"
#include "ScrScServerDlg.h"
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


// CScrScServerDlg dialog



CScrScServerDlg::CScrScServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SCRSCSERVER_DIALOG, pParent)
{
	m_pNewBmp = NULL;
	m_IsScreen = FALSE;
	m_RecvCount = 1;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScrScServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScrScServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(CM_RECEIVED, OnReceived)
END_MESSAGE_MAP()


// CScrScServerDlg message handlers

LRESULT CScrScServerDlg::OnReceived(WPARAM wParam, LPARAM lParam)
{
	BYTE* buffer = new BYTE[MAX_BUFFER];
	int factsize = sizeof(sockaddr);
	int ret = recvfrom(m_Socket, (char*)buffer, MAX_BUFFER, 0, (sockaddr*)&m_addrServ, &factsize);
	if (ret == -1)
		m_RecvCount = 1;

	UDPPACKAGE *pack;
	pack = (UDPPACKAGE*)buffer;
	int iCount = pack->iIndex;
	int iJpegSize = pack->JpegSize;
	int iBufferSize = pack->buffersize;
	bool bFinish = pack->bFinish;
	if (m_RecvCount == iCount)
	{
		memcpy(m_tmp + PICPACKSIZE*(m_RecvCount - 1), pack->buffer, iBufferSize);
		m_RecvCount += 1;
		if (bFinish)
		{
			m_RecvCount = 1;
			ShowScreen(iJpegSize, m_tmp);
		}
	}
	else
	{
		m_RecvCount = 1;
	}
	delete buffer;
	
	//if (ret == -1)
	//	MessageBox("接受错误");

	////接受图片信息
	//int nFile;
	//nFile = _open("test.jpg", O_APPEND | O_CREAT | O_BINARY | O_RDWR);
	//_write(nFile, buffer, ret);
	//_commit(nFile);
	//_close(nFile);
	return S_OK;
}

void CScrScServerDlg::ShowScreen(int DataSize, void* pData)
{
	HGLOBAL	m_hMem1 = GlobalAlloc(GMEM_MOVEABLE, PICPACKSIZE * 10);
	LPBYTE lpData1 = (LPBYTE)GlobalLock(m_hMem1);
	memcpy(lpData1, pData, DataSize);
	GlobalUnlock(m_hMem1);
	::CreateStreamOnHGlobal(m_hMem1, TRUE, &m_pStm);
	if (m_pNewBmp)
	{
		delete m_pNewBmp;
		m_pNewBmp = NULL;
	}
	m_pNewBmp = Bitmap::FromStream(m_pStm);
	CRect rc;
	GetClientRect(rc);
	HDC hDC = GetDC()->m_hDC;
	Graphics *graphics = Graphics::FromHDC(hDC);
	graphics->DrawImage(m_pNewBmp, 1, 1, rc.Width(), rc.Height());
	m_pStm->Release();
	m_pStm = NULL;
	GlobalFree(m_hMem1);
	::ReleaseDC(m_hWnd, hDC);
}

BOOL CScrScServerDlg::OnInitDialog()
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
	//启动socket
	WSADATA data;
	WSAStartup(2, &data);
	GdiplusStartup(&m_pGdiToken, &m_gdiplusinput, NULL);

	//多播结构
	struct ip_mreq  ipmr;
	int len = sizeof(ipmr);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(6002);
	ipmr.imr_multiaddr.S_un.S_addr = inet_addr("192.168.100.24");
	ipmr.imr_interface.S_un.S_addr = htonl(INADDR_ANY);

	//创建套接字
	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)
		MessageBox("无效的套接字");
	if (bind(m_Socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		MessageBox("绑定失败");

	//将套接字设置为多播组中，可以接受服务器发送过来的数据
	setsockopt(m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&ipmr, len);

	//设置异步通信方式
	WSAAsyncSelect(m_Socket, m_hWnd, CM_RECEIVED, FD_READ);
	m_RecvCount = 1;	//接受包的顺序值，默认为第一个

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScrScServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScrScServerDlg::OnPaint()
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

void CScrScServerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_IsScreen = !m_IsScreen;
	if (m_IsScreen)
	{
		GetWindowPlacement(&m_OldPlacement);
		CRect WndRect;
		GetWindowRect(&WndRect);
		int Width = GetSystemMetrics(SM_CXSCREEN);
		int Height = GetSystemMetrics(SM_CYSCREEN);
		CRect ClientRect;
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect);
		ClientToScreen(&ClientRect);

		m_FullScreenRect.left = WndRect.left - ClientRect.left;
		m_FullScreenRect.top = WndRect.top - ClientRect.top;
		m_FullScreenRect.right = WndRect.right - ClientRect.right + Width;
		m_FullScreenRect.bottom = WndRect.bottom - ClientRect.bottom + Height;

		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		wndpl.flags = 0;
		wndpl.showCmd = SW_SHOWNORMAL;
		wndpl.rcNormalPosition = m_FullScreenRect;
		SetWindowPlacement(&wndpl);
	}
	else
	{
		SetWindowPlacement(&m_OldPlacement);
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL CScrScServerDlg::DestroyWindow()
{
	GdiplusShutdown(m_pGdiToken);
	WSACleanup();
	return CDialogEx::DestroyWindow();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScrScServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

