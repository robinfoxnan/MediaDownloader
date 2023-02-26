// CPageMusic.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CPageMusic.h"
#include "afxdialogex.h"


// CPageMusic 对话框

IMPLEMENT_DYNAMIC(CPageMusic, CDialog)

CPageMusic::CPageMusic(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MUSIC, pParent)
{
	bInited = false;
}

CPageMusic::~CPageMusic()
{
}

void CPageMusic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MUSIC, mList);
}


BEGIN_MESSAGE_MAP(CPageMusic, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPageMusic 消息处理程序


BOOL CPageMusic::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwExStyles = mList.GetExtendedStyle();
	dwExStyles |= LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_ONECLICKACTIVATE |
		LVS_EX_FLATSB |
		LVS_EX_CHECKBOXES;
	mList.SetExtendedStyle(dwExStyles);
	

	mList.InsertColumn(0, "序号", LVCFMT_CENTER, 80, 0);
	mList.InsertColumn(1, "歌手", LVCFMT_CENTER, 100, 0);
	mList.InsertColumn(2, "曲名", LVCFMT_CENTER, 300, 0);
	mList.InsertColumn(3, "专辑", LVCFMT_CENTER, 300, 0);
	mList.InsertColumn(4, "关键字", LVCFMT_CENTER, 200, 0);
	mList.InsertColumn(5, "大小", LVCFMT_CENTER, 100, 0);
	mList.InsertColumn(6, "链接", LVCFMT_CENTER, 500, 0);
	//mList.SetColumnWidth(6, LVSCW_AUTOSIZE_USEHEADER);


	CString str;
	int j = 0;
	for (int i = 0; i < 100; i++)
	{

		mList.InsertItem(i, "1");

		mList.SetItemText(i, 1, "程响");
		mList.SetItemText(i, 2, "云底人间");
		mList.SetItemText(i, 3, "云底人间");
		mList.SetItemText(i, 4, "程响");
		mList.SetItemText(i, 5, "3.5M");
		mList.SetItemText(i, 6, "https://asfafaafaf");
		
	}
	//mList.SetItemColor(RGB(255, 255, 255), RGB(0, 250, 0), 2, 1);

	//mList.SetRedraw(true);
	mList.ShowScrollBar(SB_BOTH, TRUE);

	bInited = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPageMusic::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (bInited)
	{
		CRect rt;
		GetClientRect(&rt);
		mList.MoveWindow(&rt);
	}
	

	// TODO: 在此处添加消息处理程序代码
}


