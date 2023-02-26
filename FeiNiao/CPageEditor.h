#pragma once


// CPageEditor 对话框
#include "RobinEditor.h"

class CPageEditor : public CDialog
{
	DECLARE_DYNAMIC(CPageEditor)

public:
	CPageEditor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageEditor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	RobinEditor* pEditor;

	//Member variables
	HINSTANCE m_hScintilla;
	HINSTANCE m_hLexilla;
	Lexilla::CreateLexerFn m_pCreateLexer;
public:
	void OnSetScript(const string& content);
public:
	RobinEditor* NewEditor(int nID, CRect rect, int nStyle);
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
