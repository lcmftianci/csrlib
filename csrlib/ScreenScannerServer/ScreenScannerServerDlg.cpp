
// ScreenScannerServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenScannerServer.h"
#include "ScreenScannerServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_BUFF 99999

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


// CScreenScannerServerDlg dialog



CScreenScannerServerDlg::CScreenScannerServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SCREENSCANNERSERVER_DIALOG, pParent)
{
	//初始化数据
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_JPGSize = 0;
	m_IsScreen = FALSE;
	m_pNewBmp = NULL;
	m_ClientIP = "127.0.0.1";
}

void CScreenScannerServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenScannerServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(CM_RECEIVED, OnReceived)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CScreenScannerServerDlg message handlers

BOOL CScreenScannerServerDlg::OnInitDialog()
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

	ShowWindow(SW_MAXIMIZE);
	// TODO: Add extra initialization here
	//获取本机IP
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	/*
	hostent* phost = gethostbyname("");
	char* localIP = inet_ntoa(*(struct in_addr *)*phost->h_addr_list);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(localIP);
	addr.sin_port = htons(6000);
	*/

	WORD wVersionRequested;
	WSADATA wsadata;
	wVersionRequested = MAKEWORD(1, 1);
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)return 1;
	if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
	{
		WSACleanup();
		return 1;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(6000);
	//创建套接字
	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		MessageBox("套接字创建失败!");
	}
	//绑定套接字
	if (bind(m_Socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		MessageBox("套接字绑定失败!");
	}
	m_BmpData = new char[1024 * 1024];
	memset(m_BmpData, 0, 1024 * 1024);
	m_TempData = new char[1024 * 1024 * 2];
	memset(m_TempData, 0, 1024 * 1024 * 2);
	m_Header = m_BmpData;
	WSAAsyncSelect(m_Socket, m_hWnd, CM_RECEIVED, FD_READ);
	m_ShowBmp = FALSE;
	m_BmpSize = 0;
	m_RecSize = 0;
	m_ClientPort = 0;
	CScreenScannerServerApp* pApp = (CScreenScannerServerApp*)AfxGetApp();
	CString sql = "select * from tb_ClientInfo";
	char IP[MAX_PATH] = { 0 };
	GetPrivateProfileString("ClientInfo", "IP", "127.0.0.1", IP, MAX_PATH, "./Client.ini");
	m_ClientIP = IP;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScreenScannerServerDlg::OnDestroy()
{
	m_BmpData = m_Header;
	delete[] m_BmpData;
	delete[] m_TempData;
	if (m_pNewBmp)
	{
		delete m_pNewBmp;
		m_pNewBmp = NULL;
	}
	GdiplusShutdown(m_pGdiToken);
	WSACleanup();
	CDialog::OnDestroy();
}

void CScreenScannerServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScreenScannerServerDlg::OnPaint()
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
HCURSOR CScreenScannerServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//双击函数
void CScreenScannerServerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
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

//显示图片
void CScreenScannerServerDlg::ShowJPEG(void* pData, int DataSize)
{
	if (m_ShowBmp)
	{
		HGLOBAL m_hMem1 = GlobalAlloc(GMEM_MOVEABLE, DataSize);
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
		delete graphics;
		GlobalFree(m_hMem1);
		::ReleaseDC(m_hWnd, hDC);
	}
}

//接受消息
LRESULT CScreenScannerServerDlg::OnReceived(WPARAM wParam, LPARAM lParam)
{
	//接收数据
	BYTE* buffer = new BYTE[MAX_BUFF];
	sockaddr_in addr;
	int factsize = sizeof(sockaddr);
	int ret = recvfrom(m_Socket, (char*)buffer, MAX_BUFF, 0, (sockaddr*)&addr, &factsize);

	if (ret != -1)
	{
		CString revIP = inet_ntoa(addr.sin_addr);
		if (revIP != m_ClientIP)
		{
			delete[] buffer;
			return S_FALSE;
		}
		/*序号2位||结束标记2位||JPG数据||JPG数据大小4位||JPG数据总大小4位||数据报大小4位*/
		m_ClientPort = ntohs(addr.sin_port);
		//记录接收的数据报大小
		m_RecSize += ret;
		//读取序号
		WORD orderID = *(WORD*)&buffer[0];
		//读取结束标记
		WORD endID = *(WORD*)&buffer[2];
		//读取位图大小
		int bmpsize = *(int*)&buffer[ret - 12];
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(m_ClientPort);
		addr.sin_addr.S_un.S_addr = inet_addr(m_ClientIP.GetBuffer(0));
		int size = sizeof(addr);
		m_JPGSize = 0;
		//发送确认信息
		char* pData = "Confirm";
		sendto(m_Socket, pData, 8, 0, (sockaddr*)&addr, size);
		if (orderID == 0)
		{
			m_BmpSize = bmpsize;
			m_BmpData = m_Header;
		}
		else
			m_BmpSize += bmpsize;
		memcpy(m_BmpData, &buffer[4], bmpsize);
		m_BmpData += bmpsize;
		m_ShowBmp = FALSE;
		if (endID == 1)
		{
			m_ShowBmp = TRUE;
			m_BmpData = m_Header;
			m_JPGSize = *(int*)&buffer[ret - 8];
			memset(m_TempData, 0, 1024 * 1024 * 2);
			memcpy(m_TempData, m_Header, 1024 * 1024);
			ShowJPEG(m_TempData, m_JPGSize);
			m_RecSize = 0;
		}
	}
	delete[] buffer;
}