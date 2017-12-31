#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
// ScrScClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScrScClient.h"
#include "ScrScClientDlg.h"
#include "afxdialogex.h"

using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//获取指定格式编码器的CLSID值
int GetCodecClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	//获取压缩时所用的空间大小
	GetImageEncodersSize(&num, &size);
	if (size == 0) return -1;
	pImageCodecInfo = new ImageCodecInfo[size];
	if (pImageCodecInfo == NULL)
	{
		delete[] pImageCodecInfo;
		return -1;
	}

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT index = 0; index < num; ++index)
	{
		if(wcscmp(pImageCodecInfo[index].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[index].Clsid;
			delete[] pImageCodecInfo;
			return index;
		}
	}
	delete[] pImageCodecInfo;
	return -1;
}


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


// CScrScClientDlg dialog



CScrScClientDlg::CScrScClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SCRSCCLIENT_DIALOG, pParent)
{
	m_bHide = TRUE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Menu.LoadMenu(IDR_MENU_ABOUT);
}

void CScrScClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScrScClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_TRAYMESSAGE, OnTrayMessage)
	ON_COMMAND(ID_MENU_HELP, &CScrScClientDlg::OnMenuHelp)
	ON_COMMAND(ID_MENU_EXIT, &CScrScClientDlg::OnMenuExit)
	ON_COMMAND(ID_POP_HELP, &CScrScClientDlg::OnMenuHelp)
	ON_COMMAND(ID_POP_EXIT, &CScrScClientDlg::OnMenuExit)
END_MESSAGE_MAP()


// CScrScClientDlg message handlers

BOOL CScrScClientDlg::OnInitDialog()
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
	SetMenu(&m_Menu);

	ShowWindow(SW_MAXIMIZE);

	// TODO: Add extra initialization here
	//启动Gdi
	GdiplusStartup(&m_pGdiTocken, &m_gdiplusStartupInput, NULL);

	//启动网络接口
	WSADATA wsadata;
	WSAStartup(2, &wsadata);
	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)
		MessageBox("套接字创建失败");

	//网络地址
	sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(6002);
	if (SOCKET_ERROR == bind(m_Socket, (sockaddr*)&servaddr, sizeof(servaddr)))
		MessageBox("绑定失败");

	BOOL optval = TRUE;
	setsockopt(m_Socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
	m_Counter = 0;
	SetTimer(1, 1000, NULL);

	//设置托盘
	m_IconData.cbSize = sizeof(NOTIFYICONDATA);
	m_IconData.hWnd = m_hWnd;
	char szTip[] = "屏幕录制";
	strncpy(m_IconData.szTip, szTip, strlen(szTip) + 1);
	m_IconData.uCallbackMessage = WM_TRAYMESSAGE;
	m_IconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_IconData.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	Shell_NotifyIcon(NIM_ADD, &m_IconData);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScrScClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScrScClientDlg::OnPaint()
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

	if (m_bHide)
		ShowWindow(SW_HIDE);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScrScClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CScrScClientDlg::DestroyWindow()
{
	KillTimer(1);
	WSACleanup();
	GdiplusShutdown(m_pGdiTocken);
	return CDialogEx::DestroyWindow();
}

void CScrScClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(1);
	for (int i = 0; i < 10; ++i)
	{
		GetPictureData();
	}
	SetTimer(1, 1000, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CScrScClientDlg::OnMenuHelp()
{
	// TODO: Add your command handler code here
	bHide = FALSE;
	ShowWindow(SW_SHOW);
}


void CScrScClientDlg::OnMenuExit()
{
	// TODO: Add your command handler code here
	this->OnCancel();
}

void CScrScClientDlg::OnDestroy()
{
	Shell_NotifyIcon(NIM_DELETE, &m_IconData);
	//CDialogEx::DestroyWindow();
}

//托盘消息函数
LRESULT CScrScClientDlg::OnTrayMessage(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDBLCLK)
	{
		ShowWindow(SW_SHOW);
	}
	else if(lParam == WM_RBUTTONDOWN)
	{
		CPoint CurPt;
		::GetCursorPos(&CurPt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_CONTORL);
		CMenu *pSubMenu = menu.GetSubMenu(0);
		SetForegroundWindow();
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, CurPt.x, CurPt.y, this);
		menu.DestroyMenu();
		PostMessage(WM_NULL, 0, 0);
	}
	return S_OK;
}

//获取图片数据
void CScrScClientDlg::GetPictureData()
{
	//获取桌面DC
	CDC* pDeskDC = GetDesktopWindow()->GetDC();

	//获取屏幕长宽
	CRect rc;
	GetDesktopWindow()->GetClientRect(&rc);

	CDC memDC;
	memDC.CreateCompatibleDC(pDeskDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDeskDC, rc.Width(), rc.Height());
	memDC.SelectObject(&bmp);
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);
	int panelsize = 0;

	//判断真彩色
	if (bitmap.bmBitsPixel < 16)
		panelsize = pow(2, bitmap.bmBitsPixel * sizeof(RGBQUAD));

	BITMAPINFO *pBInfo = (BITMAPINFO*)LocalAlloc(LPTR, sizeof(BITMAPINFO) + panelsize);
	
	//填充BITMAPINFO结构
	pBInfo->bmiHeader.biBitCount = bitmap.bmBitsPixel;
	pBInfo->bmiHeader.biCompression = 0;
	pBInfo->bmiHeader.biClrImportant = 0;
	pBInfo->bmiHeader.biHeight = bitmap.bmHeight;
	pBInfo->bmiHeader.biPlanes = bitmap.bmPlanes;
	pBInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
	pBInfo->bmiHeader.biSizeImage = bitmap.bmWidthBytes*bitmap.bmHeight;
	pBInfo->bmiHeader.biWidth = bitmap.bmWidth;
	pBInfo->bmiHeader.biXPelsPerMeter = 0;
	pBInfo->bmiHeader.biYPelsPerMeter = 0;

	//将桌面的设备上下文复制到内存设备上下文memDC中
	memDC.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, pDeskDC, 0, 0, SRCCOPY);

	//分配保存图片空间
	char* pData = new char[bitmap.bmWidthBytes*bitmap.bmHeight];

	//获取图片数据
	::GetDIBits(memDC.m_hDC, bmp, 0, bitmap.bmHeight, pData, pBInfo, DIB_RGB_COLORS);

	//获取图片数据大小
	int BufSize = panelsize + sizeof(BITMAPINFO) + bitmap.bmWidthBytes*bitmap.bmHeight;
	
	//构建Bitmap对象
	Bitmap* mmage;
	mmage = Bitmap::FromBITMAPINFO(pBInfo, pData);

	//查看编码器
	CLSID clsid;
	GetCodecClsid(L"image/jpeg", &clsid);
	
	//建立内存分配的句柄
	HGLOBAL m_hMem = GlobalAlloc(GMEM_MOVEABLE, 0);

	//使用内存保存为流
	IStream *pstm = NULL;
	CreateStreamOnHGlobal(m_hMem, TRUE, &pstm);

	//数据转换
	mmage->Save(pstm, &clsid, NULL);

	m_JpegSize = GlobalSize(m_hMem);
	LPBYTE lpData = (LPBYTE)GlobalLock(m_hMem);
	int ret = SendData(m_JpegSize, (char*)lpData);

	//释放内存与句柄
	memDC.DeleteDC();
	pDeskDC->DeleteDC();
	pstm->Release();
	if (mmage) delete mmage;
	delete[] pData;
	GlobalUnlock(m_hMem);
	GlobalFree(m_hMem);
	::LocalFree(HLOCAL(pBInfo));
	bmp.DeleteObject();
}

int CScrScClientDlg::SendData(int totalsize, char* pSendBuf)
{
	int iRemainCount = 0;
	int t_nCount, t_nMod;
	t_nCount = totalsize / PICPACKSIZE;
	t_nMod = totalsize%PICPACKSIZE;
	if (t_nMod != 0) t_nCount += 1;
	UDPPACKAGE pack;
	m_addrClient.sin_family = AF_INET;
	m_addrClient.sin_port = htons(6002);
	m_addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	memset(pack.buffer, '\0', PICPACKSIZE);
	pack.bFinish = false;
	pack.buffersize = 0;
	pack.JpegSize = 0;
	pack.iIndex = 0;

	for (int j = 1; j <= t_nCount; ++j)
	{
		pack.JpegSize = totalsize;
		pack.iIndex = j;
		pack.PageCount = t_nCount;
		if (j < t_nCount)
		{
			pack.bFinish = false;
			pack.buffersize = PICPACKSIZE;
			memcpy(pack.buffer, pSendBuf + PICPACKSIZE*(j - 1), PICPACKSIZE);
		}
		else
		{
			pack.bFinish = true;
			pack.buffersize = totalsize - PICPACKSIZE*(t_nCount - 1);
			memcpy(pack.buffer, pSendBuf + PICPACKSIZE*(j - 1), pack.buffersize);
		}
		int ret = sendto(m_Socket, (char*)&pack, sizeof(UDPPACKAGE), 0, (sockaddr*)&m_addrClient, sizeof(m_addrClient));
		Sleep(20);
	}
	return 0;
}

