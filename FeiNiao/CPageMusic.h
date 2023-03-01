#pragma once


// CPageMusic 对话框
#include "ColorListCtrl.h"

class CPageMusic : public CDialog
{
	DECLARE_DYNAMIC(CPageMusic)

public:
	CPageMusic(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageMusic();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSIC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CColorListCtrl mList;
	CListCtrl mList;
	bool bInited;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void onNotifyData(int dataType, const std::map<string, string>* data);
	size_t getSelectionMusics();

	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelectionSelectall();
	afx_msg void OnSelectionUnselectall();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnNMRClickListMusic(NMHDR* pNMHDR, LRESULT* pResult);
};
