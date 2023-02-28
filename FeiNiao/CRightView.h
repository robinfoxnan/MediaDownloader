#pragma once

#include "TabRx.h"
#include "CPageMusic.h"
#include "CPageVideo.h"
#include "CPageLog.h"
#include "CPageEditor.h"
#include <vector>

// CRightView 窗体视图

class CRightView : public CFormView
{
	DECLARE_DYNCREATE(CRightView)

protected:
	CRightView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRightView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW_RIGHT1 };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void reSize(int width);
	
	virtual void OnInitialUpdate();

private:
	CTabRx mTab;
	CPageMusic mPageMusic;
	CPageVideo mPageVideo;
	CPageLog   mPageLog;
	CPageEditor mPageEditor;
	bool bInited;
	//CEdit mEdit;
	CString mInput1;
	CString mInput2;
	CString mInput3;
	CString mScriptDesc;
public:
	void setInfo(int index, string& info);
	void showMsg(const string& str);
	void clearMsg();
	std::vector<string> getArgs();
	void onNotifyData(int dataType, const std::map<string, string>& data);
	void onStart();
	void onStop();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	CEdit mInfo;
	CEdit  mValueEdit1;
	CEdit  mValueEdit2;
	CEdit  mValueEdit3;

	afx_msg void OnEnMaxtextEditInfo();
	afx_msg void OnClickExe();

	CButton mBtmExe;
	afx_msg void OnStartDownLoad();
	afx_msg void onClearList();
	CButton mBtnDown;
	CButton mBtnClear;
};


