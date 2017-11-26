
// DocManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DocManager.h"
#include "DocManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDocManagerDlg 对话框



CDocManagerDlg::CDocManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DOCMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDocManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECENT, m_RecentList);
}

BEGIN_MESSAGE_MAP(CDocManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CDocManagerDlg 消息处理程序

BOOL CDocManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//if (m_RecentImageList.GetSafeHandle() == NULL)
	//{
	//	m_RecentImageList.Create(80, 80, ILC_MASK | ILC_COLOR32, 0, 0);
	//	m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//	m_RecentList.SetImageList(&m_RecentImageList, LVSIL_NORMAL);
	//}

	//加载BMP图片
	//if (m_RecentImageList.GetSafeHandle() == NULL)
	//{
	//	CBitmap Image;
	//	BITMAP ImageInfo;
	//	Image.LoadBitmap(L"E:\\书籍\\视频\\文件\\2015task\\arm\\图片文件\\water.bmp");
	//	Image.GetBitmap(&ImageInfo);
	//	m_RecentImageList.Create(18, ImageInfo.bmHeight, ILC_COLOR16 | ILC_MASK, 0, 0);
	//	m_RecentImageList.Add(&Image, RGB(255, 0, 255));
	//}

	//m_RecentList.SetImageList(&m_RecentImageList, LVSIL_SMALL);

	//m_RecentList.SetExtendedStyle(m_RecentList.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);
	//m_RecentList.InsertColumn(0, L"#", LVCFMT_CENTER, 0);
	//m_RecentList.InsertColumn(1, _T("T1"), LVCFMT_LEFT, 100);
	//m_RecentList.InsertColumn(2, _T("A1"), LVCFMT_LEFT, 190);
	//m_RecentList.InsertColumn(3, _T("A2"), LVCFMT_LEFT, 190);

	//m_RecentImageList.Create(150, 200, ILC_COLOR32 | ILC_MASK, 8, 1);   //----- 实例图象框；
	//HBITMAP hBitmap;
	//CBitmap *pBitmap;
	//CString FilePathName = L"E:\\书籍\\视频\\文件\\2015task\\arm\\图片文件\\water.bmp";   //在资源的IDB_BITMAP1文件就是face.bmp导入的
	//pBitmap = new  CBitmap;
	//hBitmap = (HBITMAP)LoadImage(NULL, FilePathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//pBitmap->Attach(hBitmap);
	//m_RecentImageList.Add(pBitmap, RGB(0, 0, 0));

	//m_RecentList.SetImageList(&m_RecentImageList, LVSIL_SMALL);

	//DWORD dw = m_RecentImageList.GetImageCount();
	//CString str;
	//for (int j = 0;j < 3;j++) {
	//	m_RecentList.InsertItem(j, L"123", 0);
	//	LVITEM item = { 0 };
	//	item.iItem = j;
	//	item.iSubItem = 2;
	//	item.mask = LVIF_TEXT | LVIF_IMAGE;
	//	item.iImage = 0;                  //图片索引值
	//	item.pszText = _T("Virqin");
	//	item.lParam = (LPARAM)m_hWnd;
	//	m_RecentList.SetItem(&item);
	//}
	//m_RecentImageList.Detach();

	//使用ICON
	/*CImageList m_ImageList;*/
	//m_RecentList.SetExtendedStyle(m_RecentList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	//m_RecentImageList.Create(32, 16, ILC_COLOR, 8, 4);

	//int m_nIdxIco1 = m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));  //使用索引  
	//int m_nIdxIco2 = m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//int m_nIdxIco3 = m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//int m_nIdxIco4 = m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//m_RecentList.SetImageList(&m_RecentImageList, LVSIL_SMALL);
	//m_RecentList.InsertColumn(0, L"ColName1", LVCFMT_CENTER, 40);
	//m_RecentList.InsertColumn(1, L"ColName2", LVCFMT_LEFT, 50);
	//m_RecentList.InsertItem(0, L"", m_nIdxIco1);
	//m_RecentList.InsertItem(1, L"", m_nIdxIco2);
	//m_RecentList.InsertItem(2, L"", m_nIdxIco3);
	//m_RecentList.InsertItem(3, L"", m_nIdxIco4);

	//m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));  //不使用索引  
	//m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//m_RecentImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//m_RecentList.SetImageList(&m_RecentImageList, LVSIL_SMALL);
	//m_RecentList.InsertColumn(0, L"ColName1", LVCFMT_CENTER, 40);
	//m_RecentList.InsertColumn(1, L"ColName2", LVCFMT_LEFT, 50);
	//m_RecentList.InsertItem(0, L"", 0);
	//m_RecentList.InsertItem(1, L"", 1);
	//m_RecentList.InsertItem(2, L"", 2);
	//m_RecentList.InsertItem(3, L"", 3);
	//m_RecentList.SetItem(0, 1, LVIF_TEXT, L"111", 0, 0, 0, 0);
	//m_RecentList.SetItem(1, 1, LVIF_IMAGE, L"222", 1, 0, 0, 0);
	//m_RecentList.SetItem(2, 1, LVIF_TEXT, L"333", 2, 0, 0, 0);
	//m_RecentList.SetItem(3, 1, LVIF_IMAGE, L"444", 3, 0, 0, 0);

	m_RecentList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_RecentList.SetIconSpacing(CSize(140, 130));



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDocManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDocManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDocManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

