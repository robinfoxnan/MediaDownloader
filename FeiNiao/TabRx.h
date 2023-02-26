#if !defined(AFX_TABRX_H__894E4D5F_C2F3_4899_9137_EC33E0AEA941__INCLUDED_)
#define AFX_TABRX_H__894E4D5F_C2F3_4899_9137_EC33E0AEA941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabRx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabRx window
#include <afxcoll.h>

class CTabRx : public CTabCtrl
{
// Construction
public:
	CTabRx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabRx)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InsertItem(LPCTSTR str, CWnd * pPage);
	void MoveTab(CWnd * p);
	void GetWorkRect(CRect & rt);
	void ChangeTab(int t);
	void SetSize();
	CPtrArray ItemArray;
	int index;

	BOOL bMove;
	int  tickDelay;
	CWnd * page;
	int left;
	int pix;

	
	virtual ~CTabRx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabRx)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRX_H__894E4D5F_C2F3_4899_9137_EC33E0AEA941__INCLUDED_)
