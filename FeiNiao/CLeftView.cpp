// CLeftView.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CLeftView.h"
#include "GlobalData.h"


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CFormView)

CLeftView::CLeftView()
	: CFormView(IDD_FORMVIEW_LEFT1)
{
	bInited = false;
}

CLeftView::~CLeftView()
{
}

void CLeftView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mList);
}

BEGIN_MESSAGE_MAP(CLeftView, CFormView)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_LIST1, &CLeftView::OnLbnSelchangeList)
END_MESSAGE_MAP()


// CLeftView 诊断

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeftView 消息处理程序

void CLeftView::reSize(int width)
{

	
}


void CLeftView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (bInited)
	{
		
		CRect rt(0, 0, cx, cy);
		
		mList.MoveWindow(&rt, TRUE);
	}

	// TODO: 在此处添加消息处理程序代码
}





void CLeftView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//mList.AddString("喜马拉雅专辑下载");
	//mList.AddString("酷我搜索关键字");
	//mList.SetItemHeight(0, 25);
	//mList.SetItemHeight(1, 25);

	GlobalData::instance().loadScripts();
	int index = 0;
	for (auto it : GlobalData::instance().getScripts())
	{
		mList.AddString(it.first.c_str());
		mList.SetItemHeight(index++, 25);
	}

}


void CLeftView::OnLbnSelchangeList()
{

	int index = mList.GetCurSel();
	if (index != LB_ERR)
	{
		CString str;
		mList.GetText(index, str);
		GlobalData::instance().setScriptSel((LPCTSTR)str);
		GlobalData::instance().NotifyMessage(MSG_SCRIPT_CHANGE);
	}
		

}
