
// FeiNiaoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FeiNiao.h"
#include "FeiNiaoDlg.h"
#include "afxdialogex.h"
#include "GlobalData.h"


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
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFeiNiaoDlg 对话框



CFeiNiaoDlg::CFeiNiaoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FEINIAO_DIALOG, pParent)
{
	m_pFrameWnd = nullptr;
	bInited = false;
	pLeft = nullptr;
	pRight = nullptr;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFeiNiaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFeiNiaoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFeiNiaoDlg 消息处理程序

BOOL CFeiNiaoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	// 加载菜单
	CMenu* pMenu = this->GetMenu();
	if (pMenu != nullptr)
	{
		// 显示菜单
		//::SetMenu(this->GetSafeHwnd(), pMenu->GetSafeHmenu());
	}

	// 创建工具条
	if (m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TB_AUTOSIZE,
		 WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		// 调整布局
	/*CRect rcClient;
	GetClientRect(rcClient);
	m_wndToolBar.SetWindowPos(nullptr,
		rcClient.left,
		rcClient.top,
		rcClient.Width(),
		24,
		SWP_NOACTIVATE | SWP_NOZORDER);*/
	}

	
	

	// TODO: 在此添加额外的初始化代码
	// //创建分割窗口

	//显示分割窗口
	CRect rect;
	//GetWindowRect(&rect);
	//ScreenToClient(&rect);
	this->GetClientRect(&rect);

	m_splitterWnd.CreateStatic(m_pFrameWnd, 1, 2);
	
	m_splitterWnd.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(210, 100), NULL);
	m_splitterWnd.CreateView(0, 1, RUNTIME_CLASS(CRightView), CSize(500, 100), NULL);
	pLeft = (CLeftView*)m_splitterWnd.GetPane(0, 0);
	pRight = (CRightView*)m_splitterWnd.GetPane(0, 1);
	GlobalData::pLeftView = pLeft;
	GlobalData::pRightView = pRight;

	
	m_splitterWnd.SetColumnInfo(0, 210, 100);
	//pLeft->reSize(210);
	//m_splitterWnd.SetColumnInfo(1, rect.Width() * 0.85, 300);


	
	m_pFrameWnd->MoveWindow(&rect);
	m_pFrameWnd->ShowWindow(SW_SHOW);

	bInited = true;
	pLeft->bInited = true;
	
	ShowWindow(SW_MAXIMIZE);
	//ShowWindow(SW_NORMAL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFeiNiaoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFeiNiaoDlg::OnPaint()
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
HCURSOR CFeiNiaoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CFeiNiaoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	// TODO: Add your specialized creation code here
	CString strMyClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION));

	//创建frame window作为子窗口
	m_pFrameWnd = new CFrameWnd;
	m_pFrameWnd->Create(strMyClass, _T(""), WS_CHILD, CRect(0, 0, 300, 300), this);
	m_pFrameWnd->ShowWindow(SW_SHOW);

	
	return 0;
}


void CFeiNiaoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (bInited == false)
		return;

	if (m_pFrameWnd)
	{
		CRect rect;
		this->GetClientRect(&rect);
		if (rect.Width() == 0 || rect.Height() == 0)
		{
			return;
		}
		m_pFrameWnd->MoveWindow(&rect);
	}
	
}


