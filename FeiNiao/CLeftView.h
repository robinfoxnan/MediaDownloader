﻿#pragma once
#include "pch.h"

// CLeftView 窗体视图

class CLeftView : public CFormView
{
	DECLARE_DYNCREATE(CLeftView)

protected:
	CLeftView();           // 动态创建所使用的受保护的构造函数
	virtual ~CLeftView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW_LEFT1 };
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
	CListBox mList;
	void reSize(int width);
	bool bInited;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	
	afx_msg void OnLbnSelchangeList();
};


