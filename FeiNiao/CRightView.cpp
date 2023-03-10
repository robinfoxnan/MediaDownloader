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
	DDX_Control(pDX, IDC_EDIT1, mValueEdit1);
	DDX_Control(pDX, IDC_EDIT2, mValueEdit2);
	DDX_Control(pDX, IDC_EDIT3, mValueEdit3);
	DDX_Control(pDX, IDC_BUTTON1, mBtmExe);
	DDX_Control(pDX, IDC_BUTTON2, mBtnDown);
	DDX_Control(pDX, IDC_BUTTON3, mBtnClear);
	DDX_Control(pDX, IDC_STATIC_PROGESS, mTxtProgress);
}

BEGIN_MESSAGE_MAP(CRightView, CFormView)
	ON_WM_SIZE()
	ON_EN_MAXTEXT(IDC_EDIT_INFO, &CRightView::OnEnMaxtextEditInfo)
	ON_BN_CLICKED(IDC_BUTTON1, &CRightView::OnClickExe)
	ON_BN_CLICKED(IDC_BUTTON2, &CRightView::OnStartDownLoad)
	ON_BN_CLICKED(IDC_BUTTON3, &CRightView::onClearList)
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
	mTab.InsertItem("  API接口测试  ", &mPageLog);
	mTab.InsertItem("  脚本编辑  ", &mPageEditor);

	// 全局引用
	GlobalData::pRightView = this;
	GlobalData::pPageEditor = &mPageEditor;
	GlobalData::pPageMusic = &mPageMusic;
	GlobalData::pPageVideo = &mPageVideo;
	GlobalData::pPageLog = &mPageLog;

	mValueEdit1.SetWindowText(_T("歌手歌名"));
	mValueEdit2.SetWindowText(__T("1"));
	mValueEdit3.SetWindowText(__T("30"));


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

		CRect rtProgress(5, rt.Height() - 280, rt.Width(), rt.Height()-255);
		mTxtProgress.MoveWindow(&rtProgress);

		CRect rectTab(0, 150, rt.Width(), rt.Height()-285);
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
	auto args = getArgs();
	GlobalData::instance().setSearchkeys(args);
	GlobalData::instance().startThread(0);
}


std::vector<string> CRightView::getArgs()
{
	std::vector<string> args;
	
	CString txt1;
	CString txt2;
	CString txt3;
	mValueEdit1.GetWindowText(txt1);
	mValueEdit2.GetWindowText(txt2);
	mValueEdit3.GetWindowText(txt3);

	args.push_back((LPCTSTR)txt1);
	args.push_back((LPCTSTR)txt2);
	args.push_back((LPCTSTR)txt3);
	return args;
}

void CRightView::onNotifyData(int dataType, const std::map<string, string>* data)
{
	
	if (data ->size() < 3)
		return;
	std::stringstream ss;
	ss << data->at("url") << "    [";
	ss << data->at("bytes") << " / ";
	ss << data->at("all") << "]";

	mTxtProgress.SetWindowText(ss.str().c_str());

	if (dataType >= 0)
	{
		
		mPageMusic.onNotifyData(3, data);
	}
}

void CRightView::onStart()
{
	mBtmExe.EnableWindow(FALSE);
	mBtnClear.EnableWindow(FALSE);
	mBtnDown.EnableWindow(FALSE);
}

void CRightView::onStop()
{
	mBtmExe.EnableWindow(TRUE);
	mBtnClear.EnableWindow(TRUE);
	mBtnDown.EnableWindow(TRUE);
}

void CRightView::OnStartDownLoad()
{
	size_t n = mPageMusic.getSelectionMusics();
	if (n == 0)
	{
		AfxMessageBox("请先选择至少一个条目，右键全选或者全去");
		return;
	}

	mBtmExe.EnableWindow(FALSE);
	mBtnClear.EnableWindow(FALSE);
	mBtnDown.EnableWindow(FALSE);

	GlobalData::instance().startThread(1);

}


void CRightView::onClearList()
{
	mPageMusic.onNotifyData(-1, 0);
}


LRESULT CRightView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_DOWNLOAD_PROGRESS)
	{
		std::map<string, string>* data = (std::map<string, string>  * )lParam;
		this->onNotifyData((int)wParam, data);
	}

	return CFormView::WindowProc(message, wParam, lParam);
}
