// CRightView.cpp: 实现文件
//

#include "pch.h"
#include "FeiNiao.h"
#include "CRightView.h"
#include "GlobalData.h"


// CRightView

IMPLEMENT_DYNCREATE(CRightView, CFormView)

CRightView::CRightView()
	: CFormView(IDD_FORMVIEW_RIGHT1)
	, mInput1(_T("关键字"))
	, mInput2(_T("页码"))
	, mInput3(_T("条目"))
	, mScriptDesc(_T("使用方法"))
	, mValue1(_T(""))
	, mValue2(_T(""))
	, mValue3(_T(""))
{
	bInited = false;
}

CRightView::~CRightView()
{
}

void CRightView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTab);
	DDX_Text(pDX, IDC_STATIC_IN1, mInput1);
	DDX_Text(pDX, IDC_STATIC_IN2, mInput2);
	DDX_Text(pDX, IDC_STATIC_IN3, mInput3);
	DDX_Text(pDX, IDC_STATIC_DESC, mScriptDesc);
	DDX_Control(pDX, IDC_EDIT_INFO, mInfo);
	DDX_Text(pDX, IDC_EDIT1, mValue1);
	DDX_Text(pDX, IDC_EDIT2, mValue2);
	DDX_Text(pDX, IDC_EDIT3, mValue3);
}

BEGIN_MESSAGE_MAP(CRightView, CFormView)
	ON_WM_SIZE()
	ON_EN_MAXTEXT(IDC_EDIT_INFO, &CRightView::OnEnMaxtextEditInfo)
	ON_BN_CLICKED(IDC_BUTTON1, &CRightView::OnClickExe)
END_MESSAGE_MAP()


// CRightView 诊断

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRightView 消息处理程序
void CRightView::reSize(int width)
{

}

void CRightView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	mTab.SetItemSize(CSize(250, 30));

	mPageMusic.Create(IDD_MUSIC, &mTab);
	mPageVideo.Create(IDD_VIDEO, &mTab);
	mPageLog.Create(IDD_LOG, &mTab);
	mPageEditor.Create(IDD_EDITOR, &mTab);
	mTab.InsertItem("  音乐搜索  ", &mPageMusic);
	mTab.InsertItem("  视频搜索  ", &mPageVideo);
	mTab.InsertItem("  下载记录  ", &mPageLog);
	mTab.InsertItem("  脚本编辑  ", &mPageEditor);

	// 全局引用
	GlobalData::pRightView = this;
	GlobalData::pPageEditor = &mPageEditor;
	GlobalData::pPageMusic = &mPageMusic;
	GlobalData::pPageVideo = &mPageVideo;
	GlobalData::pPageLog = &mPageLog;

	mValue1 = _T("程响");
	mValue2 = _T("1");
	mValue3 = _T("30");
	UpdateData(FALSE);

	mInfo.SetWindowText("一切就绪\r\n");
	showMsg("一切就绪");
	bInited = true;
}


void CRightView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (bInited)
	{
		CRect rt;
		GetClientRect(&rt);

		CRect rectEdit(0, rt.Height() - 250,  rt.Width(), rt.Height());
		mInfo.MoveWindow(&rectEdit);

		CRect rectTab(0, 150, rt.Width(), rt.Height()-255);
		mTab.MoveWindow(&rectTab);
	}
	
}

void CRightView::setInfo(int index, string& info)
{
	switch (index)
	{
	case 0:
		mScriptDesc = info.c_str();
		break;

	case 1:
		mInput1 = info.c_str();
		break;

	case 2:
		mInput2 = info.c_str();
		break;
	case 3:
		mInput3 = info.c_str();
		break;

	default:
		break;
	}
	UpdateData(FALSE);
}


void CRightView::showMsg(const string& str)
{
	int len = mInfo.GetWindowTextLength();
	mInfo.SetSel(len, len);
	mInfo.ReplaceSel(str.c_str(), TRUE);

	len = mInfo.GetWindowTextLength();
	mInfo.SetSel(len,len );
	mInfo.ReplaceSel("\r\n", TRUE);
	
}

void CRightView::clearMsg()
{
	mInfo.SetWindowText("");
}

void CRightView::OnEnMaxtextEditInfo()
{
	clearMsg();
}



void CRightView::OnClickExe()
{
	GlobalData::instance().exeScript();
}


std::vector<string> CRightView::getArgs()
{
	std::vector<string> args;
	UpdateData(TRUE);
	args.push_back((LPCTSTR)mValue1);
	args.push_back((LPCTSTR)mValue2);
	args.push_back((LPCTSTR)mValue3);
	return args;
}