// CPageMusic.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CPageMusic.h"
#include "afxdialogex.h"
#include "GlobalData.h"
#include "./include/PathUtil.h"

using namespace bird2fish;


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
	for (int i = 0; i < 1; i++)
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


void CPageMusic::onNotifyData(int dataType, const std::map<string, string>* data)
{
	/*
	local tbl = {
		singer = "程响",
		song = "云底人间",
		album = "某专辑",
		tags = "云底",
		size = "5.4M",
		url = "HTTS://AFSDAFAAF",
	}
	*/

	int i = 0;
	switch (dataType)
	{
	case -1:
		mList.DeleteAllItems();
		break;

	case 1:
		i = mList.GetItemCount();
		mList.InsertItem(i, std::to_string(i+1).c_str());

		mList.SetItemText(i, 1, data->at("singer").c_str());
		mList.SetItemText(i, 2, data->at("song").c_str());
		mList.SetItemText(i, 3, data->at("album").c_str());
		mList.SetItemText(i, 4, data->at("tags").c_str());
		mList.SetItemText(i, 5, data->at("size").c_str());
		mList.SetItemText(i, 6, data->at("url").c_str());

		mList.Invalidate(TRUE);

	default:
		break;
	}
}

LRESULT CPageMusic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == WM_NOTINY_USER_DATA)
	{
		this->onNotifyData(wParam, (std::map<string, string>*)((void *)lParam));
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPageMusic::getSelectionMusics()
{
	int n = mList.GetItemCount();
	MusicVector vec;
	for (int i = 0; i < n; i++)
	{
		bool b = mList.GetCheck(i);
		MusicItemPtr item = std::make_shared<MusicItem>();
		item->index = i;
		item->percent = 0;
		item->singer = mList.GetItemText(i, 0);
		item->song = mList.GetItemText(i, 1);
		item->url = mList.GetItemText(i, 6);
		item->downloadName = item->singer + "-";
		item->downloadName += item->song;
		item->downloadName += PathUtil::getExt(item->url);
		vec.emplace_back(item);
	}

	GlobalData::instance().setMusicVec(vec);
}
