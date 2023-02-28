
// FeiNiaoDlg.h: 头文件
//

#pragma once
#include "CLeftView.h"
#include "CRightView.h"

// CFeiNiaoDlg 对话框
class CFeiNiaoDlg : public CDialogEx
{
// 构造
public:
	CFeiNiaoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	//enum { IDD = IDD_FEINIAO_DIALOG };
	enum {
		IDD = IDD_FEINIAO_DIALOG, 
		IDR_MAIN_MENU = IDR_MENU_MAIN, 
		IDR_TOOLBAR = IDR_TOOLBAR1
};
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CFrameWnd * m_pFrameWnd;
	CSplitterWnd  m_splitterWnd;
	bool bInited;
	CLeftView* pLeft;
	CRightView* pRight;
	CToolBar  m_wndToolBar;


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileSetting();
	afx_msg void OnFileSavescript();
};
