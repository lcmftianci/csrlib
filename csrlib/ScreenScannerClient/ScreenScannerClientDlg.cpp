
// ScreenScannerClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenScannerClient.h"
#include "ScreenScannerClientDlg.h"
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


// CScreenScannerClientDlg dialog



CScreenScannerClientDlg::CScreenScannerClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SCREENSCANNERCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenScannerClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenScannerClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(CM_RECEIVED, OnReceived)
	ON_BN_CLICKED(IDC_BTN_START, &CScreenScannerClientDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


int GetCodecClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = new ImageCodecInfo[size];
	if (pImageCodecInfo == NULL)
	{
		delete[]pImageCodecInfo;
		return -1;
	}
	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			delete[] pImageCodecInfo;
			return j;
		}
	}
	delete[] pImageCodecInfo;
	return -1;
}

// CScreenScannerClientDlg message handlers

BOOL CScreenScannerClientDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	//获取本机IP
	hostent* phost = gethostbyname("");
	char* localIP = inet_ntoa(*(struct in_addr *)*phost->h_addr_list);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(localIP);
	addr.sin_port = htons(6000);
	//创建套接字
	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		MessageBox("套接字创建失败!");
	}
	char* len = "9999";
	if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, len, 4) != 0)
	{
		MessageBox("设置失败!");
	}
	//绑定套接字
	if (bind(m_Socket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		MessageBox("套接字绑定失败!");
	}
	m_ServerIP = "127.0.0.1";
	GetPrivateProfileString("ServerInfo", "IP", "127.0.0.1", m_ServerIP.GetBuffer(0), MAX_PATH, "./Server.ini");
	m_pSendBuf = new char[1024 * 1024];
	m_pHeader = m_pSendBuf;
	WSAAsyncSelect(m_Socket, m_hWnd, CM_RECEIVED, FD_READ);
	m_Confirm = TRUE;
	OnBnClickedBtnStart();
	m_Received = FALSE;
	m_Counter = 0;
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScreenScannerClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CScreenScannerClientDlg::OnPaint()
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
	//隐藏窗口
	//ShowWindow(SW_HIDE);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScreenScannerClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScreenScannerClientDlg::OnDestroy()
{
	GdiplusShutdown(m_pGdiToken);
	WSACleanup();
	delete[]m_pHeader;
	CDialog::OnCancel();
}

int CScreenScannerClientDlg::SendData(UINT index, int mod, int bmpsize, int totalsize, int frames, char *pSendBuf, sockaddr_in &addr)
{
	/*序号2位||结束标记2位||JPG数据||JPG数据大小4位||JPG数据总大小4位||数据报大小4位*/
	char* pPackage;
	int packsize = 0;
	if (mod == 0 || index != frames - 1)
		packsize = 2 + 2 + bmpsize + 4 + 4 + 4;
	else
		packsize = 2 + 2 + mod + 4 + 4 + 4;
	pPackage = new char[packsize];
	memset(pPackage, 0, packsize);
	//填充数据报
	*(WORD*)&pPackage[0] = index;	//填充序号
	if (index != frames - 1)			//填充结束标记
		*(WORD*)&pPackage[2] = 0;
	else
		*(WORD*)&pPackage[2] = 1;
	//填充位图数据
	pSendBuf += bmpsize*index;
	memcpy(&pPackage[4], pSendBuf, packsize - 12);
	//填充位图大小
	*(int*)&pPackage[packsize - 12] = bmpsize;
	//填充JPG数据总大小
	*(int*)&pPackage[packsize - 8] = totalsize;
	//填充数据报大小
	*(int*)&pPackage[packsize - 4] = packsize;
	m_Confirm = FALSE;
	int ret = sendto(m_Socket, pPackage, packsize, 0, (sockaddr*)&addr, sizeof(addr));
	delete[] pPackage;
	return ret;
}

LRESULT CScreenScannerClientDlg::OnReceived(WPARAM wParam, LPARAM lParam)
{
	char* pData = new char[1024];
	memset(pData, 0, 1024);
	sockaddr_in addr;
	int factsize = sizeof(sockaddr);
	int ret = recvfrom(m_Socket, pData, 1024, 0, (sockaddr*)&addr, &factsize);
	delete[]pData;
	if (ret != -1)
	{
		if (ret == 8)
		{
			m_Received = TRUE;
			m_FrameIndex += 1;
			if (m_FrameIndex < m_Count)
				SendData(m_FrameIndex, m_Mod, m_Bmpsize, m_JpegSize, m_Count, m_pSendBuf, m_Addr);
			else
				OnBnClickedBtnStart();
		}
	}
	return S_OK;
}

void CScreenScannerClientDlg::OnTimer(UINT nIDEvent)
{
	if (!m_Received)
	{
		m_Counter++;
		if (m_Counter > 5)
		{
			m_Counter = 0;
			OnBnClickedBtnStart();  //超过5秒没有收到服务器的应答信息,重发数据
		}
	}
	else
	{
		m_Received = FALSE;
		m_Counter = 0;
	}
	CDialog::OnTimer(nIDEvent);
}

void CScreenScannerClientDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	CDC* pDeskDC = GetDesktopWindow()->GetDC();		//获取桌面画布对象
	CRect rc;
	GetDesktopWindow()->GetClientRect(rc);				//获取屏幕的客户区域

	CDC  memDC;											//定义一个内存画布
	memDC.CreateCompatibleDC(pDeskDC);					//创建一个兼容的画布
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDeskDC, rc.Width(), rc.Height());	//创建兼容位图
	memDC.SelectObject(&bmp);							//选中位图对象	
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);

	int panelsize = 0;									//记录调色板大小
	if (bitmap.bmBitsPixel < 16)							//判断是否为真彩色位图
		panelsize = pow(2, bitmap.bmBitsPixel * sizeof(RGBQUAD));

	BITMAPINFO *pBInfo = (BITMAPINFO*)LocalAlloc(LPTR, sizeof(BITMAPINFO) + panelsize);
	pBInfo->bmiHeader.biBitCount = bitmap.bmBitsPixel;
	pBInfo->bmiHeader.biClrImportant = 0;
	pBInfo->bmiHeader.biCompression = 0;
	pBInfo->bmiHeader.biHeight = bitmap.bmHeight;
	pBInfo->bmiHeader.biPlanes = bitmap.bmPlanes;
	pBInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
	pBInfo->bmiHeader.biSizeImage = bitmap.bmWidthBytes*bitmap.bmHeight;
	pBInfo->bmiHeader.biWidth = bitmap.bmWidth;
	pBInfo->bmiHeader.biXPelsPerMeter = 0;
	pBInfo->bmiHeader.biYPelsPerMeter = 0;

	memDC.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, pDeskDC, 0, 0, SRCCOPY);

	char* pData = new char[bitmap.bmWidthBytes* bitmap.bmHeight];
	::GetDIBits(memDC.m_hDC, bmp, 0, bitmap.bmHeight, pData, pBInfo, DIB_RGB_COLORS);
	int BufSize = panelsize + sizeof(BITMAPINFO) + bitmap.bmWidthBytes*bitmap.bmHeight;
	Bitmap*  mmage;
	mmage = Bitmap::FromBITMAPINFO(pBInfo, pData);

	CLSID clsid;
	GetCodecClsid(L"image/jpeg", &clsid);
	HGLOBAL m_hMem = GlobalAlloc(GMEM_MOVEABLE, 0);
	IStream *pstm = NULL;
	CreateStreamOnHGlobal(m_hMem, TRUE, &pstm);
	mmage->Save(pstm, &clsid, NULL);
	m_JpegSize = GlobalSize(m_hMem);
	LPBYTE lpData = (LPBYTE)GlobalLock(m_hMem);
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(5002);
	m_Addr.sin_addr.S_un.S_addr = inet_addr(m_ServerIP);
	m_Bmpsize = GraphSize;
	//计算每个位图发送的次数
	m_Count = m_JpegSize / GraphSize;
	m_Mod = m_JpegSize % GraphSize;
	if (m_Mod != 0)
		m_Count += 1;
	m_FrameIndex = 0;
	memcpy(m_pSendBuf, lpData, m_JpegSize);
	int ret = SendData(m_FrameIndex, m_Mod, GraphSize, m_JpegSize, m_Count, m_pSendBuf, m_Addr);
	memDC.DeleteDC();
	pDeskDC->DeleteDC();
	pstm->Release();
	if (mmage)
		delete mmage;
	delete[] pData;
	GlobalUnlock(m_hMem);
	GlobalFree(m_hMem);
	::LocalFree((HLOCAL)pBInfo);
	bmp.DeleteObject();
}
